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
map<string,int> savedPositions; // string of directory numbers and saved file positions
bool verbose; // flag used with program arguments

// prototypes
void configPrompt();
void configGet();
void launchMedia(const string &,const string &);
void updateConfigPos();
string stringConvert2L(const string &);
string stringConvert2W(const string &);

class Media{
    private:
        int posIn,filePos;
        string dir; // should be whole path to ensure subdirectories are captured and called correctly
        string dirName; // for reference to savedPosition map and user calls
        map<int,string> files;
    public:
        Media(const string &newDir,const string &newDirName,const int &newFilePos=0){ // argument when called with a pos that was stored in config or library file
            dir=newDir;
            dirName=newDirName;
            posIn=0;
            filePos=newFilePos;
            scanFiles();
        }
        void scanFiles(){ // scan directory function
            DIR *tDir;
            struct dirent *ent;
            if((tDir=opendir(dir.c_str()))!=NULL){
                while((ent=readdir(tDir))!=NULL){
                    string temp=ent->d_name;
                    if(temp.rfind(".mp4")!=string::npos||
                       temp.rfind(".MP4")!=string::npos||
                       temp.rfind(".mkv")!=string::npos||
                       temp.rfind(".MKV")!=string::npos||
                       temp.rfind(".wav")!=string::npos||
                       temp.rfind(".WAV")!=string::npos||
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
                        if(verbose) cout << "\t\t" << temp << endl;
                        files[posIn]=temp;
                        posIn++;
                    }
                }
                closedir (tDir);
            }
            else{
                if(verbose) cout << "could not open directory" << endl;
            }
        }
        void playMedia(){
            for(auto it:files){
                if(verbose) cout << "begin play function for " << files[filePos] << " in " << dir <<endl; 
                double length=getLength(files[filePos]);
                char launch[1024];
                sprintf(launch,"\"%s\" \"%s\\%s\"",mediaPlayer.c_str(),stringConvert2W(dir).c_str(),files[filePos].c_str());
                if(verbose) cout << "terminal script accumulation result:\n" << launch << endl;
                double elapsed=time(NULL); // start elapsed time
                system(launch);
                elapsed=time(NULL)-elapsed; // stop elapsed time
                if(elapsed<(0.5*length)) return; // if elapsed < 50% media length break playback
                if(filePos<(int)files.size()-1){
                    filePos++; // increment file position
                    savedPositions[dirName]=filePos; // update overall map;
                    updateConfigPos(); // update wO_config
                }
                else{ // end of files map, break playback
                    filePos=0;
                    savedPositions.erase(dirName); // erase entry from overall map;
                    updateConfigPos(); // update wO_config
                    return;
                }
            }
            // TODO: set short delay to check for user input - i.e. set play=0;
                // consider a 5-10 second timer that waits for input
                    // if no input, continue playback loop
                    // if input, back out to menu
        }
        friend ostream &operator <<(ostream &out,Media &other){ // print directory and contained files, used with printDirs()
            out << "\t    " << other.dir << endl;
            for(int i=0;i<(int)other.files.size();i++){
                out << "\t\t" << other.files[i] << endl;
            }
            return out;
        }
        double getLength(const string &file){ // get duration in seconds
            char arg[1024]; // char array for ffprobe and argument
            sprintf(arg,"ffprobe -i \"%s/%s\" -show_entries format=duration -v quiet -of csv=\"p=0\" >wO_swap",dir.c_str(),file.c_str());
            system(arg);
            ifstream wO_swap;
            string result;
            wO_swap.open("wO_swap"); // length is stored in this file
            getline(wO_swap,result);
            wO_swap.close();
            return stod(result);
        }
        bool isEmpty(){ // check to be called by Directories, if empty, delete from map
            return !posIn; // if posIn==0, then scanFiles never found anything
        }
        void updatePos(const int &newPos=0){ // update saved position, reset if no argument
            filePos=newPos;
        }
};

class Directories{
    private:
        int posIn;
        map<string,Media*> dirs; // vector of directories kept
    public:
        Directories(){
            posIn=0;
            scanDirs(stringConvert2L(mediaDirect).c_str());
        }
        void scanDirs(const char* path,const string &prev=""){
            string temp,tempName;
            struct dirent* dent;
            DIR* srcdir=opendir(path);
            if(srcdir==NULL){
                perror("opendir");
                return;
            }
            while((dent=readdir(srcdir))!=NULL){
                struct stat st;
                if(strcmp(dent->d_name,".")==0||strcmp(dent->d_name,"..")==0) continue;
                if(fstatat(dirfd(srcdir),dent->d_name,&st,0)<0){ // intellisense hates fstatat and dirfd
                    perror(dent->d_name);
                    continue;
                }
                if(S_ISDIR(st.st_mode)){ // store directory in map
                    temp=dent->d_name;
                    tempName="";
                    tempName.append(prev);
                    if(tempName.size()!=0) tempName.push_back('/');
                    tempName.append(temp);
                    if(verbose) cout << "\tsubdirectory " << temp << " found." << endl;
                    string newTemp=path; // send entire path
                    newTemp.append(temp);
                    Media *p=new Media(newTemp,tempName);
                    dirs[tempName]=p;
                    if(dirs[tempName]->isEmpty()){ // check if empty, erase
                        dirs.erase(tempName);
                        if(verbose) cout << "\t" << path << tempName << " does not contain valid media.\n";
                    }
                    else{
                        if(savedPositions.find(tempName)!=savedPositions.end()) dirs[tempName]->updatePos(savedPositions[tempName]); // update Media saved position
                        posIn++;
                    }
                    if(verbose) cout << "\tscanning subdirectories in ";
                    char newDir[1024];
                    sprintf(newDir,"%s%s/",path,temp.c_str());
                    if(verbose) cout << newDir << endl;
                    scanDirs(newDir,tempName);
                    if(verbose) cout << "\tfinished scanning " << newDir << endl;
                }
            }
            closedir(srcdir);
            return;
        }
        void printDirs(){
            cout << dirs.size() << " directories found." << endl;
            for(auto it:dirs){
                cout << "\t" << it.first << endl;
                // cout << *it.second;
            }
        }
        void play(const string &dirPlay){
            if(dirs.find(dirPlay)!=dirs.end()) dirs[dirPlay]->playMedia();
            else cout << "Invalid entry, try again." << endl;
        }
};


void configPrompt(){ // subroutine to get directory configuration from user
    cout << "Input media player directory (ex \"/mnt/c/files/mpc.exe\"): " << endl;
    getline(cin,mediaPlayer);
    cout << "Input media directories (ex \"c:\\Users\\Documents\\Videos\\\"): " << endl;
    getline(cin,mediaDirect);
    // TODO: populate directories' play positions from config
}

void configGet(){ // routine to import directory configuration
    string temp,temp2;
    ifstream confIn;
    confIn.open("wO_config");
    getline(confIn,mediaPlayer);
    getline(confIn,mediaDirect);
    if(mediaPlayer[0]=='\0'||mediaDirect[0]=='\0') configPrompt(); // call prompt if config did not exist or was empty
    if(verbose) cout << "result of path accumulation:\n" << mediaPlayer << endl << mediaDirect << endl;
    while(!confIn.eof()){
        getline(confIn,temp);
        if(temp=="") break;
        stringstream sTemp(temp);
        getline(sTemp,temp,';');
        getline(sTemp,temp2);
        savedPositions[temp]=stoi(temp2);
    }
    confIn.close();
    ofstream confOut;
    confOut.open("wO_config");
    confOut << mediaPlayer << endl << mediaDirect << endl;
    for(auto it:savedPositions){
        confOut << it.first << ';' << it.second << endl;
    }
    confOut.close();
}

void updateConfigPos(){ // update saved directory/media play position
    string temp;
    ifstream confIn;
    confIn.open("wO_config");
    getline(confIn,mediaPlayer);
    getline(confIn,mediaDirect);
    confIn.close();
    ofstream confOut;
    confOut.open("wO_config");
    confOut << mediaPlayer << endl << mediaDirect << endl;
    for(auto it:savedPositions){
        confOut << it.first << ';' << it.second << endl;
    }
    confOut.close();
}

string stringConvert2L(const string &toConvert){ // convert a windows format directory string to "/mnt/" linux format
    string temp="/mnt/";
    char drive=tolower(toConvert[0]);
    temp.push_back(drive);
    temp.append(toConvert.substr(2,string::npos));
    replace(temp.begin(),temp.end(),'\\','/'); // replace all '\' with '/'
    return temp;
}

string stringConvert2W(const string &toConvert){ // convert a linux format "/mnt/" directory string to windows format
    string temp="";
    char drive=toupper(toConvert[5]);
    temp.push_back(drive);
    temp.append(":\\");
    temp.append(toConvert.substr(7,string::npos));
    replace(temp.begin(),temp.end(),'/','\\'); // replace all '/' with '\'
    return temp;
}

int main(int argc,char *argv[]){
    verbose=0;
    if(argc>=2&&(string)argv[1]=="-v") verbose=1; // verbose flag set, output all terminal messages
    configGet();
    Directories user;
    cout << "What's On v1.1 - now saving your place, per directory!" << endl;
    user.printDirs();
    string input;
    while(1){
        cout << "Input play directory: ";
        getline(cin,input);
        if(input=="quit") break;
        user.play(input);
    }
    return 0;
}
