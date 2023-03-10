CSCI112 Semester Project - Jacob Seman

Television Broadcast Schedule Emulator

The program should emulate a television broadcast station scheduler. In this scope, the functionality that matters is
the ability to use a program to produce a "channel" wherein video files are scheduled categorically and in alignment with
a typical broadcast schedule. The end user can expect to run the program and select a channel based on a description,
after which an embedded (or thereabouts) media player is evoked and a video file (and/or playlist of upcoming video files)
is queued based on time of day relative to the program's "schedule".

This will require accurate system timekeeping, time scheduling, hooks and arguments to control a media player or generate
a playlist, a method for reading various media file metadata (length, type, etc), as well as a way to read a filesystem
organized in a specific way. It would be beneficial to interpret various formats and syntax of filenames, or disregard
filenames and utilize a type of metadata that is present for all media in the system.

    ex. m:\media\
                \shows\
                      \show1\
                            \show1-ep1.mov
                            \show1-ep2.mov
                            \...
                      \show2\
                            \show2-ep1.mov
                            \show2-ep2.mov
                            \...
                \movies\
                        \movie1.mov
                        \movie2.mov
                        \...
                \commercials\
                        \comm1.mov
                        \...
                \music videos\
                        \mv1.mov
                        \mv2.mov
                        \...

Requirements and expectations:

    Set start time on the clock at 15 min increments
        typically 00 or 30, but also allow for 15 and 45 to accommodate shorter shows

    Fill schedule with 24 hour block rotation
        Each show has a set time block to fill
        Each day the episode number increases
        Cycles back to 1 after reaching maximum

    Stretch: Could have a pool of shorter videos to fill gaps between when the
    episode ends and when the next show starts
        i.e. commercial, music videos, etc
        ex: finishes at 9:25 am, play a music video, collection of old commercials,
        or other interstitial material to fill the space
        This media could be selected randomly or from a themed pool of media files
        in a subdirectory

Deliverables:
    A program that can be instructed to interpret files present in a specified filesystem directory

    Functionality that correctly and consistently schedules media to play during a specified time block

    Functionality that fills time gaps between scheduled media with "filler" media

    Functionality that approximates multiple channels with unique regularly scheduled media


A reduced scope functionality is currently implemented:

    The executable is able to run from any location in a bash terminal on a windows system

    If no configuration file exists, prompt the user for the media player executable path and media "root" folder and store
    this information

    Load the configuration file and scan the subfolder/file structure of the media root folder
        Ignore files that are not playable by the media player (check file extension)
    
        Scan all subdirectories for media files as well

    Prompt the user for a directory to begin playback from (i.e. shows/series/seasons)
        As playback progresses through episodes, log the directory position and file position and save this to the
        configuration file
    
        If position data does not exist, start from the first file and create a new entry for the saved position
    
        If position data is stored in the config file, then resume at that position
    
        If position data progresses to the end of a directory, reset the program position to zero and remove the config
        file entry

    Upon exiting the media player, if elapsed time is less than half of the length of the media, retain that position and
    exit to the program menu prompt

    Running the program with the argument -v (for "verbose") will output all terminal messages - this allows one to view the
    various shell script c_string accumulations that the program is calling to the system, as well as view each and every
    subdirectory and media file found in the media root directory

This reduced scope functionality is sufficient for a use-case where an end-user has some amount of archival video files
organized in a folder hierarchy similar to the structure shown above. The user would be able to run the program, input the
required executable path and media root path, and select a subfolder to begin playing the contents sequentially.

This solves a specific problem when trying to approximate the same functionality with existing media-player programs that
are already able to automatically advance to the next file in a folder, as well as save the last played file to resume upon
reopening the player. This "player only" approximation is not capable of saving a position across different subdirectories
(shows), meaning that a user's position in a certain subdirectory will be lost if another subdirectory is played from. This
program alleviates that slight irritation by tying the progress in a subdirectory to the directory itself.
