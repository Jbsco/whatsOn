CSCI112 Semester Project - Jacob Seman

Television Broadcast Schedule Emulator

The program should emulate a television broadcast station scheduler. In this scope, the functionality that matters is the ability to use a program to produce a "channel" wherein video files are scheduled categorically and in alignment with a typical broadcast schedule. The end user can expect to run the program and select a channel based on a description, after which an embedded (or therabouts) media player is evoked and a video file (and/or playlist of upcoming video files) is queued based on time of day relative to the program's "schedule".

This will require accurate system timekeeping, time scheduling, hooks and arguments to control a media player or generate a playlist, a method for reading various media file metadata (length, type, etc), as well as a way to read a filesystem organized in a specific way. It would be beneficial to interpret various formats and syntax of filenames, or disregard filenames and utilize a type of metadata that is present for all media in the system.

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
        typically 00 or 30, but also allow for 15 and 45 to accomodate shorter shows

    Fill schedule with 24 hour block rotation
        Each show has a set time block to fill
        Each day the episode number increases
        Cycles back to 1 after reaching maximum

    Stretch: Could have a pool of shorter videos to fill  gaps between when the episode ends and when the next show starts
        i.e. commercial, music videos, etc
        ex: finishes at 9:25 am, play a music video, collection of old commercials, or other interstitial material to fill the space
        This media could be selected randomly or from a themed pool of media files in a subdirectory

Deliverables:
    A program that can be instructed to interpret files present in a specified filesystem directory

    Functionality that correctly and consistently schedules media to play during a specified time block

    Functionality that fills time gaps between scheduled media with "filler" media

    Functionality that approximates multiple channels with unique regularly scheduled media
