/*******************************************************************************
 *  SEMESTER:PROJECT: Jacob Seman
 * Create a program that approximates the functionality of a television
 * broadcast station(s)
 ******************************************************************************/

/*******************************************************************************
 *  IDEA:
 * Take minimal input but provision for calling of an external media player to
 * play media while adhering to a time schedule.
 * 
 *  CHANNELS:
 * Allow changing of channels per file directories available to the program.
 * This can follow the folder hiearchy:
 *      root/
 *          /media1/ep1.file
 *                 /ep2.file
 *                 /etc...
 *          /media2/ep1.file
 *                 /ep2.file
 *                 /etc...
 * The program should be able to parse different naming conventions.
 * 
 *  TIME:
 * Play new media every time block, could be 15min, 30min, 1hr, etc.
 * If a media file is too short to fill the block, try to pull from
 * miscellaneous media such as music videos, commercials, etc to fill the block.
 * 
 ******************************************************************************/

#include<iostream>
#include<sstream>
#include<ctime> // assuming this will be used extensively
//#include<filesystem> // might need this for filesystem operations
#include<dirent.h> // use dirent instead
#include<sys/stat.h> // this will be needed with dirent
// prefer to avoid the windows specific libraries.

// utilize tool such as ffmpeg for file reporting
// VLC also has these features
// check "spawn" as well
// system call function "exec" -> ffmpeg or vlc with arguments to get metadata
// 

// write tool to interpret ffmpeg output
// wrap into dirent to process subfolders/directories

using namespace std;

// class/function(s) for data access/queueing

// class/function for invoking external media player/files
    // investigate system, CreateProcess(), or ShellExecute()
    //     may be Windows only. /sad
    // Find other methods of filesystem access/interaction

// class for channel

// class for timekeeping/scheduling, part of channel class?

int main(/* external arguments !!! */){

    /* example snippet *********************************************************
    namespace fs = std::filesystem;
    // std::string path = "/path/to/directory";
    string path="/media/video/channel_1";
    for (const auto &entry : fs::directory_iterator(path))
        cout << entry.path() << endl;
    ***************************************************************************/

    /* example snippet *********************************************************
    DIR *dir;
    struct dirent *ent;
    if((dir=opendir("c:\\src\\"))!=NULL) {
        // print all the files and directories within directory
        while((ent=readdir(dir))!=NULL){
            printf("%s\n",ent->d_name);
        }
        closedir(dir);
    }
    else{
        // could not open directory
        perror("");
        return EXIT_FAILURE;
    }
    ***************************************************************************/

    /* example snippet ********************************************************/
    struct stat file_stats;
    DIR *dirp;
    struct dirent* dent;
    dirp=opendir("mediaRoot/dir"); // parent directory of media subdirs
    do{
        dent=readdir(dirp); // read dir to get subdirs/files
        if(dent){
            printf("  file \"%s\"",dent->d_name);
            printf(" is size %u\n",(unsigned int)file_stats.st_size);
        }
    }
    while(dent);
    closedir(dirp);
    
    // consider creating stringstreams or char arrays for channel directories
    /**************************************************************************/

    // functions for input/handling
    // up/down for channel select?
    while(1){
        int ch=3;
        cout << "Welcome to TV Scheduler - Current channel: "
                << ch << endl;
        cout << "Input new channel: ";
        cin >> ch;
    }

    // send channel to directory function above

    // set time/call handlers
    int initialT=time(NULL);
    // check for currentTime%30==0 to start next process
    // or use a timer corresponding to time blocks


    //if(exec play file) wait([metaData read for media play time]);

    // try testing on audio files for simplicity/portability at first
    
    // identify a simple media player that is compatible with needed filetypes
    
    //  and can accept external arguments (or flags).
    
    // need some method of determining media playback length - from metadata?
    
    // functions for computing efficient time block fill
    
    return 0;
}
