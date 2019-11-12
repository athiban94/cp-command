# cp-command

## Problems encountered:

** To check if a given pathname is a file or directory?
 - used the macro S_ISDIR(m) defined in the stat(2) to check the type 
   of the file. Included <sys/stat.h> header.

** To parse the pathname - Eg: /etc/passwd?
 - used char* basename(char *path); from <libgen.h>. 
   Also, read about basename(3)

** When two files Hard linked i.e when two files point to the same data on the disk?
 - used int stat(const char *path, struct stat *sb); from stat(2) and compared 
   the inode number(ino_t) of both the files to check of similarity.
   Note: Found this test case from the testcp.sh