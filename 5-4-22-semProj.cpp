/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

#include<sys/types.h>
#include<sys/stat.h>
#include<algorithm>
#include<dirent.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<iomanip>
#include<fstream>
#include<cstring>
#include<vector>
#include<ctime>
#include<map>

using namespace std;

string mediaPlayer; // directory and executable for media player to be used
string mediaDirect; // directory to check for subdirectories and media

// prototypes
void configPrompt();
void configGet();
void launchMedia(const string &,const string &);

class Media{
    private:
        int posIn,dirPos;
        string dir;
        map<int,string> files;
    public:
        Media(const string &newDir,const int &newdirPos=0){ // argument when called with a pos that was stored in config or library file
            dir=newDir;
            posIn=0;
            dirPos=newdirPos;
            scanFiles();
        }
        void addMedia(const string &newFile){
            files[posIn]=newFile;
            posIn++;
        }
        void scanFiles(){ // scan directory function
            DIR *tDir;
            struct dirent *ent;
            if((tDir=opendir(dir.c_str()))!=NULL){
                while((ent=readdir(tDir))!=NULL){
                    string temp=ent->d_name;
                    if(temp.rfind(".mp4")!=string::npos||
                       temp.rfind(".mkv")!=string::npos||
                       temp.rfind(".flac")!=string::npos||
                       temp.rfind(".mpg")!=string::npos||
                       temp.rfind(".mpeg")!=string::npos||
                       temp.rfind(".avi")!=string::npos){ // check for video media extensions, .mpg, .mpeg, .mkv, etc
                        cout << temp << endl;
                        addMedia(temp);
                    }
                }
                closedir (tDir);
            }
            else cout << "could not open directory" << endl;
        }
        void playMedia(){
            for(auto it:files){ // TODO: handle exiting/quitting
                // TODO: get system time
                cout << "begin play fcn for " << files[dirPos] << " in " << dir <<endl;
                launchMedia(files[dirPos],dir);
                dirPos++;
                // TODO: get system time and compare against earlier
                // TODO: check elapsed time against media length and update savdPos
                // TODO: set short delay to check for user input - i.e. set play=0;
            }
            // TODO: consider a 5-10 second timer that waits for input
                // if no input, continue playback loop
                // if input, back out to menu
            // TODO: optional, check elapsed time against media length
                // if elapsed is <50% of media length, do not update savedPos
                // if elapsed >50% then update savedPos
        }
        double getLength(const string &file){ // get duration in seconds
            char arg[1024]; // char array for ffprobe and argument
            // build argument expression:
            sprintf(arg,"ffprobe -i \"%s\" -show_entries format=duration -v quiet -of csv=\"p=0\" >wO_swap",file.c_str());
            system(arg); // takes arguments from above to call ffmpeg's ffprobe function on file
            ifstream wO_swap;
            string result;
            wO_swap.open("wO_swap"); // length is stored in this file
            getline(wO_swap,result);
            wO_swap.close();
            return stod(result);
        }
        void resetPos(){
            // TODO: call this from main menu, reset savedPos/playPos
        }
};

class Directories{
    private:
        int posIn,dirPos;
        map<int,Media*> dirs; // vector of directories kept
    public:
        Directories(const int &newDirPos=0){
            posIn=0;
            dirPos=newDirPos;
            // use linux directory format for scanDirs()
            string tempDir="/mnt/";
            char drive=tolower(mediaDirect[0]);
            tempDir.push_back(drive);
            tempDir.append(mediaDirect.substr(2,string::npos));
            replace(tempDir.begin(),tempDir.end(),'\\','/'); // replace all '\' to '/'
            cout << tempDir << endl;
            scanDirs(tempDir.c_str());
        }
        void scanDirs(const char* path){
            struct dirent* dent;
            DIR* srcdir=opendir(path);
            if(srcdir==NULL){
                perror("opendir");
                return;
            }
            while((dent=readdir(srcdir))!=NULL){
                struct stat st;
                if(strcmp(dent->d_name,".")==0||strcmp(dent->d_name,"..")==0) continue;
                if(fstatat(dirfd(srcdir),dent->d_name,&st,0)<0){
                    perror(dent->d_name);
                    continue;
                }
                if(S_ISDIR(st.st_mode)){
                    string temp=dent->d_name;
                    cout << temp << endl;
                    Media *p=new Media(temp);
                    dirs[posIn]=p;
                    posIn++;
                }
            }
            closedir(srcdir);
            return;
        }
        void play(const int &dirNo){
            // dirs[dirPos].playMedia(); // play selected directory (plays from Media's pos)
            dirs[dirNo]->playMedia();
        }
};


void configPrompt(){ // subroutine to get directory configuration from user
    cout << "Input media player directory (ex \"/mnt/c/files/mpc.exe\"): " << endl;
    getline(cin,mediaPlayer);
    cout << "Input media directories (ex \"c:\\Users\\Documents\\Videos\\\"): " << endl;
    getline(cin,mediaDirect);
    // TODO: populate Directories from config or saved library file
}

void configGet(){ // routine to import directory configuration
    ifstream confIn;
    confIn.open("wO_config");
    getline(confIn,mediaPlayer);
    getline(confIn,mediaDirect);
    confIn.close();
    if(mediaPlayer[0]=='\0'||mediaDirect[0]=='\0') configPrompt(); // call prompt if config did not exist or was empty
    // testing resulting char arrays
    cout << "result of path accumulation:\n" << mediaPlayer << endl << mediaDirect << endl;
    // update program variables:
    ofstream confOut;
    confOut.open("wO_config");
    confOut << mediaPlayer << endl << mediaDirect << endl;
    confOut.close();
}

void launchMedia(const string &file,const string &direct=mediaDirect){
    // executable path formated for bash terminal, i.e. "system()":
    // char mPlayerPath[]="/mnt/c/Program Files/Combined Community Codec Pack 64bit/MPC/mpc-hc64.exe";
    // media path formatted for argument with above executable:
    // char mediaPath[]="C:\\Users\\Aretaic\\Documents\\cs\\CPP\\spring 2022\\semester project\\";
    char launch[1024];
    sprintf(launch,"\"%s\" \"%s%s\\%s\"",mediaPlayer.c_str(),mediaDirect.c_str(),direct.c_str(),file.c_str());
    cout << "terminal script accumulation result:\n" << launch << endl;
    system(launch);
}

int main(){
    // config check - get configuration settings or prompt to collect configuration
    configGet();
    int dPos=0;
    // TODO: menu system

    // class testing
    Directories user(dPos);
    user.play(0);
    user.play(1);
    // minimal functionality working as intended

    // TODO: check for user input to exit loop to main menu

    // TODO: allow changing current subdirectory, currently plays all sequentially

    return 0;
}
