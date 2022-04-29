/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<ctime>
/*
#include<sstream>
#include<dirent.h>
#include<sys/stat.h> // this will be needed with dirent
*/

using namespace std;

char mediaPlayer[256]; // directory and executable for media player to be used
char mediaDirect[256]; // directories to check for media

class Direct{
    private:
        vector<string> dirs; // vector of directories kept
    public:
        Direct(){
            //
        }
};

void configPrompt(){ // subroutine to get directory configuration from user
    cout << "Input media player directory (ex \"/mnt/c/files/mpc.exe\"): " << endl;
    cin.getline(mediaPlayer,256);
    cout << "Input media directories (ex \"c:\\Users\\Documents\\Videos\\\"): " << endl;
    cin.getline(mediaDirect,256);
}

void configGet(){ // routine to import directory configuration
    ifstream confIn;
    confIn.open("wO_config");
    confIn.getline(mediaPlayer,256);
    confIn.getline(mediaDirect,256);
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

// scan directory function
// input pos is requested position in potential files
void scanDir(const int &pos){
    // [TODO] scan directory for media of certain type
    // return filename of media at position from argument
    // else return a string that is clearly not a filename
        // return "NOT FOUND" signifies no valid media file at that position
}

double getLength(const char file[]){ // get duration in seconds
    char arg[1024]; // char array for ffprobe & argument
    // complete argument expression:
    sprintf(arg,"ffprobe -i \"%s\" -show_entries format=duration -v quiet -of csv=\"p=0\" >wO_swap",file);
    system(arg); // takes arguments from above to call ffmpeg's ffprobe function on file
    ifstream wO_swap;
    string result;
    wO_swap.open("wO_swap"); // length is stored in this file
    getline(wO_swap,result);
    wO_swap.close();
    return stod(result);
}

void launchMedia(const char file[]){
    // executable path formated for bash terminal, i.e. "system()":
    // char mPlayerPath[]="/mnt/c/Program Files/Combined Community Codec Pack 64bit/MPC/mpc-hc64.exe";
    // media path formatted for argument with above executable:
    // char mediaPath[]="C:\\Users\\Aretaic\\Documents\\cs\\CPP\\spring 2022\\semester project\\";
    char launch[1024];
    // sprintf(launch,"\"%s\" \"%s%s\"",mPlayerPath,mediaPath,file);
    sprintf(launch,"\"%s\" \"%s%s\"",mediaPlayer,mediaDirect,file);
    cout << "terminal script accumulation result:\n" << launch << endl;
    system(launch);
}

int main(){
    // [low priority] get requested directory from user
        // just use current directory until later versions
    
    // config check - get configuration settings or prompt to collect configuration
    configGet();
    
    // [TODO] call function to scan directory to get filenames
    /*
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


    // getLength testing:
    char fileName[]="test.flac";
    cout << getLength(fileName) << endl;
    // functioning as intended
    
    // play media function:
    launchMedia(fileName);
    // consectutive calls testing:
    char fileName2[]="test.flac";
    launchMedia(fileName2);
    // note that launchMedia() waits for the child process (the instance of the media player executable) to close before proceeding
    // functioning as intended


    // [TODO] build schedule and find position based on current time
        // [TODO] get basic queueing system working
        // [low pri] build system for buffering program time block
        // with short media when target media is shorter than time block

    // [LOOP]
    // [TODO] for current local time open media player with correct file
        // skip to current time position in file, ffmpeg function[?] ffprobe[?]

    // [TODO] accept input to modify current operation

    return 0;
}