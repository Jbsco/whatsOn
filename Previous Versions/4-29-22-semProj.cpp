/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

#include<sys/stat.h>
#include<dirent.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<ctime>
#include<map>

using namespace std;

string mediaPlayer; // directory and executable for media player to be used
string mediaDirect; // directory to check for subdirectories and media

// prototypes
void configPrompt();
void configGet();
void scanDir(const int &);
double getLength(const string);
void launchMedia(const string &,const string &);
class Media;

class Directories{
    private:
        int posIn,playPos;
        map<int,Media> dirs; // vector of directories kept
    public:
        Directories(const int &newPlayPos=0){
            posIn=0;
            playPos=newPlayPos;
            // TODO: check library/config files for existing directories
                // TODO: also check for manual rescan requested
            // if no existing directories or manual rescan requested:
            // fcn: scanDirs()
        }
        void scanDirs(){
            // TODO: scan root media directory "mediaDirect"
            // TODO: scan each subdirectory for playable media
            while(1){ // while scanning media root directory
                string newDir;
                // continue scan
                // update newDir with each directory
                // build individual Media() class for each subdirectory
                while(1){ // while scanning media subdirectory
                // for(auto iterator){
                    string newFile;
                    // update newFile with each file
                    dirs[posIn]=Media(newDir/* ,savedPos from config */);
                    dirs[posIn].addMedia(newFile,newDir);
                    posIn++;
                }
            }
        }
        void play(){
            dirs[playPos].playMedia(); // play selected directory (plays from Media's pos)
        }
};

class Media{
    private:
        int posIn,playPos;
        string dir;
        map<int,string> files;
    public:
        Media(const string &newDir,const int &newPlayPos=0){ // argument when called with a pos that was stored in config or library file
            posIn=0;
            playPos=newPlayPos;
        }
        void addMedia(const string &newFile,const string &newDir=mediaDirect){
            dir=newDir;
            files[posIn]=newFile;
            posIn++;
        }
        void playMedia(){
            bool play=1;
            while(play){ // TODO: handle exiting/quitting
                launchMedia(files[playPos],dir);
                playPos++;
                // TODO: check elapsed against media length and update savdPos
                // TODO: set short delay to check for user input - i.e. set play=0;
            }
            // TODO: consider a 5-10 second timer that waits for input
                // if no input, continue playback loop
                // if input, back out to menu
            // TODO: optional, check elapsed time against media length
                // if elapsed is <50% of media length, do not update savedPos
                // if elapsed >50% then update savedPos
        }
        void resetPos(){
            // TODO: call this from main menu, reset savedPos/playPos
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
    // test resulting char arrays
    cout << "result of path accumulation:\n" << mediaPlayer << endl << mediaDirect << endl;
    // update program variables:
    ofstream confOut;
    confOut.open("wO_config");
    confOut << mediaPlayer << endl << mediaDirect << endl;
    confOut.close();
}

void scanDir(const int &pos){ // scan directory function
    // input pos is requested position in potential files
    // TODO scan directory for media of certain type
    // return filename of media at position from argument
    // else handle it - ex. return a string that is clearly not a filename
        // return "NOT FOUND" signifies no valid media file at that position
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

void launchMedia(const string &file,const string &direct=mediaDirect){
    // executable path formated for bash terminal, i.e. "system()":
    // char mPlayerPath[]="/mnt/c/Program Files/Combined Community Codec Pack 64bit/MPC/mpc-hc64.exe";
    // media path formatted for argument with above executable:
    // char mediaPath[]="C:\\Users\\Aretaic\\Documents\\cs\\CPP\\spring 2022\\semester project\\";
    char launch[1024];
    // sprintf(launch,"\"%s\" \"%s%s\"",mPlayerPath,mediaPath,file);
    sprintf(launch,"\"%s\" \"%s%s\"",mediaPlayer.c_str(),direct.c_str(),file.c_str());
    cout << "terminal script accumulation result:\n" << launch << endl;
    system(launch);
}

int main(){
    // config check - get configuration settings or prompt to collect configuration
    configGet();
    // TODO: menu system
    
    // [TODO] call function to scan directory to get filenames
    /* basic idea
    vector<string> fileName;
    vector<double> mediaLength;
    string temp;
    int i=1;
    while(temp!="NOT FOUND"){
        temp=scanDir(i);
        fileName.push_back(temp);
        mediaLength.push_back(getLength(temp));
        i++;
    }
    */


    /* getLength testing:
    string fileName="test.flac";
    cout << getLength(fileName) << endl;
    // functioning as intended
    
    // play media function:
    launchMedia(fileName);
    // consectutive calls testing:
    string fileName2="test.flac";
    launchMedia(fileName2);
    // note that launchMedia() waits for the child process (the instance of the media player executable) to close before proceeding
    // functioning as intended
    */


    // TODO: scan or import directories
    
    // playMedia() loop, check elapsed against media length
    // check for user input to exit loop to main menu

    // TODO: allow changing current subdirectory

    return 0;
}
