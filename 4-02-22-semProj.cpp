/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

#include<iostream>
#include<vector>
#include<ctime>
/*
#include<sstream>
#include<dirent.h>
#include<sys/stat.h> // this will be needed with dirent
*/

using namespace std;

// scan directory function
// input pos is requested position in potential files
string scanDir(const int &pos){
    // [TODO] scan directory for media of certain type
    // return filename of media at position from argument
    // else return a string that is clearly not a filename
        // return "NOT FOUND" signifies no valid media file at that position
}

// get media length function
// input file is requested file to get length of
double getLength(const string &file){
    // [TODO] find file and return media length
}

int main(){
    // [low priority] get requested directory from user
        // just use current directory until later versions
    
    // [TODO] call function to scan directory and get media w/ length metadata
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
