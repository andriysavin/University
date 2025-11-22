# Lab: File System
A lab work for Unix course implementing simulation of Unix-style file system based on inodes. This is of course a greatly simplified implementation 
(e.g. it doesn't support folders, and file names are stored in inodes directly).


At the high level the code creates a virtual file system partition stored in a "real" file, copies a text file to two files in the virtual file system, 
doing it in chunks, turn-by-turn to simulate concurrency. Then it reopens the virtual files and copies them to the real file system, so we can inspect them and compare to the original text file to ensure they hold the same content.


The code was lifted a bit to work with modern C++ compilers, and basic CMake configuration was added to be able to build it on macOS and Linux in addition to Windows. 
