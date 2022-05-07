/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

#include<sys/stat.h>
#include<algorithm>
#include<dirent.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstring>
#include<ctime>
#include<map>

using namespace std;

string mediaPlayer; // directory and executable for media player to be used
string mediaDirect; // directory to check for subdirectories and media

// prototypes
void configPrompt();
void configGet();
void launchMedia(const string &,const string &);
string stringConvert2L(const string &);
string stringConvert2W(const string &);

class Media{
    private:
        int posIn,filePos;
        string dir; // should be whole path to ensure subdirectories are captured and called correctly
        map<int,string> files;
    public:
        Media(const string &newDir,const int &newFilePos=0){ // argument when called with a pos that was stored in config or library file
            dir=newDir;
            posIn=0;
            filePos=newFilePos;
            scanFiles();
        }
        void addMedia(const string &newFile){
            files[posIn]=newFile;
            posIn++;
        }
        void scanFiles(){ // scan directory function
            DIR *tDir;
            struct dirent *ent;
            // char cDir[1024];
            // sprintf(cDir,"%s%s",stringConvert2L(mediaDirect).c_str(),dir.c_str());
            if((tDir=opendir(dir.c_str()))!=NULL){ // this may need full directory specified
                while((ent=readdir(tDir))!=NULL){
                    string temp=ent->d_name;
                    if(temp.rfind(".mp4")!=string::npos||
                       temp.rfind(".MP4")!=string::npos||
                       temp.rfind(".mkv")!=string::npos||
                       temp.rfind(".MKV")!=string::npos||
                       temp.rfind(".flac")!=string::npos||
                       temp.rfind(".FLAC")!=string::npos||
                       temp.rfind(".mpg")!=string::npos||
                       temp.rfind(".MPG")!=string::npos||
                       temp.rfind(".mov")!=string::npos||
                       temp.rfind(".MOV")!=string::npos||
                       temp.rfind(".mpeg")!=string::npos||
                       temp.rfind(".MPEG")!=string::npos||
                       temp.rfind(".avi")!=string::npos||
                       temp.rfind(".AVI")!=string::npos){ // check for video media extensions, .mpg, .mpeg, .mkv, etc
                        cout << "\t\t" << temp << endl;
                        addMedia(temp);
                    }
                }
                closedir (tDir);
            }
            else cout << "could not open directory" << endl;
        }
        void playMedia(){
            for(auto it:files){ // TODO: handle exiting/quitting
                // TODO: get system time and compare against earlier
                cout << "begin play fcn for " << files[filePos] << " in " << dir <<endl; 
                double length=getLength(files[filePos]);
                // executable path formated for bash terminal, i.e. "system()":
                // char mPlayerPath[]="/mnt/c/Program Files/Combined Community Codec Pack 64bit/MPC/mpc-hc64.exe";
                // media path formatted for argument with above executable:
                // char mediaPath[]="C:\\Users\\Aretaic\\Documents\\cs\\CPP\\spring 2022\\semester project\\";
                char launch[1024];
                sprintf(launch,"\"%s\" \"%s\\%s\"",mediaPlayer.c_str(),stringConvert2W(dir).c_str(),files[filePos].c_str());
                cout << "terminal script accumulation result:\n" << launch << endl;
                system(launch);
                // TODO: optional, check elapsed time against media length
                // if elapsed is <50% of media length, do not update filePos
                // if elapsed >50% then update filePos
                // if(length<"elapsed*2"){
                // else break; // do not increment filePos
                if(filePos<(int)files.size()-1) filePos++;
                else{
                    filePos=0;
                    return;
                }
            }
            // TODO: set short delay to check for user input - i.e. set play=0;
                // consider a 5-10 second timer that waits for input
                    // if no input, continue playback loop
                    // if input, back out to menu
        }
        friend ostream &operator <<(ostream &out,Media &other){ // used with printDirs()
            out << other.dir << endl;
            for(int i=0;i<(int)other.files.size();i++){
                out << "\t\t" << other.files[i] << endl;
            }
            return out;
        }
        double getLength(const string &file){ // get duration in seconds
            char arg[1024]; // char array for ffprobe and argument
            // build argument expression:
            sprintf(arg,"ffprobe -i \"%s/%s\" -show_entries format=duration -v quiet -of csv=\"p=0\" >wO_swap",dir.c_str(),file.c_str());
            // cout << arg << endl;
            system(arg); // takes arguments from above to call ffmpeg's ffprobe function on file
            ifstream wO_swap;
            string result;
            wO_swap.open("wO_swap"); // length is stored in this file
            getline(wO_swap,result);
            wO_swap.close();
            return stod(result);
        }
        bool isEmpty(){ // check to be called by Directories, if empty, delete from map
            return !posIn;
        }
        void resetPos(){
            // TODO: call this from main menu, reset savedPos/playPos
            filePos=0;
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
            scanDirs(stringConvert2L(mediaDirect).c_str());
        }
        void scanDirs(const char* path){
            string temp;
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
                if(S_ISDIR(st.st_mode)){ // store directory in map
                    temp=dent->d_name;
                    cout << "\tsubdirectory " << temp << " found." << endl;
                    string newTemp=path; // send entire path
                    newTemp.append(temp);
                    Media *p=new Media(newTemp);
                    dirs[posIn]=p;
                    // check if empty
                    if(dirs[posIn]->isEmpty()){
                        dirs.erase(posIn);
                        cout << "\t" << path << temp << " does not contain valid media.\n";
                    }
                    else posIn++;
                    cout << "\tscanning subdirectories in ";
                    char newDir[1024];
                    sprintf(newDir,"%s%s/",path,temp.c_str());
                    cout << newDir << endl;
                    scanDirs(newDir);
                    cout << "\tfinished scanning " << newDir << endl;
                }
            }
            closedir(srcdir);
            return;
        }
        void printDirs(){
            cout << dirs.size() << " directories found." << endl;
            for(int i=0;i<(int)dirs.size();i++){
                cout << i << ":\t" << *dirs[i];
            }
        }
        void play(const int &dirNo){
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

string stringConvert2L(const string &toConvert){ // convert a windows format directory string to "/mnt/" linux format
    string temp="/mnt/";
    char drive=tolower(toConvert[0]);
    temp.push_back(drive);
    temp.append(toConvert.substr(2,string::npos));
    replace(temp.begin(),temp.end(),'\\','/'); // replace all '\' with '/'
    // cout << temp << endl;
    return temp;
}

string stringConvert2W(const string &toConvert){ // convert a linux format "/mnt/" directory string to windows format
    string temp="";
    char drive=toupper(toConvert[5]);
    temp.push_back(drive);
    temp.append(":\\");
    temp.append(toConvert.substr(7,string::npos));
    replace(temp.begin(),temp.end(),'/','\\'); // replace all '/' with '\'
    // cout << temp << endl;
    return temp;
}

int main(){
    // config check - get configuration settings or prompt to collect configuration
    configGet();
    int dPos=0;
    // TODO: menu system

    // class testing
    Directories user(dPos);
    user.printDirs();
    string input,entry;
    while(input!="quit"){
        cout << "play directory numbers: ";
        getline(cin,input);
        stringstream sInput(input);
        while(getline(sInput,entry,' ')){
            user.play(stoi(entry));
        }
    }

    // TODO: check for user input to exit loop to main menu

    return 0;
}