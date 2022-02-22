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
#include<ctime> // assuming this will be used

using namespace std;

// class/function(s) for data access/queueing

// class/function for invoking external media player/files
    // investigate system, CreateProcess(), or ShellExecute()
    //     may be Windows only. /sad
    // Find other methods of filesystem access/interaction

// class for channel

// class for timekeeping/scheduling, part of channel class?

int main(/* external arguments !!! */){
    // functions for input/handling
    // set time/call handlers
    // up/down for channel select?
    // try testing on audio files for simplicity/portability at first
    // identify a simple media player that is compatible with needed filetypes
    //  and can accept external arguments (or flags).
    // need some method of determining media playback length - from metadata?
    // functions for computing efficient time block fill
    return 0;
}