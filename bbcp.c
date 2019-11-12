#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>

#define BUFFSIZE 1024

/*
 * @param: <pathname> 
 * returns true is the pathname is a directory or false if not.
 * */
int isDirectory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int main(int argc, char *argv[])
{
    int fd, fd1, data;
    char *buff[BUFFSIZE];
    struct stat sb, sb1;

    /* Validating arguments - should be equal to 3 */
    if (argc != 3)
    {
        printf("Usage : ./bbcp file1 file2\n");
        exit(EXIT_FAILURE);
    }

    /* Validating if arguement 1 is a directory */
    if (isDirectory(argv[1]))
    {
        printf("error : %s is not a file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if(stat(argv[2], &sb1) == -1) {
        perror("stat");
        if(errno != ENOENT)
            exit(EXIT_FAILURE);
    }
    
    /* Checking if two of the files are hardlinked with each other */
    if(sb.st_ino == sb1.st_ino) {
        printf("error : both files are hardlinked");
        exit(EXIT_FAILURE);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        fprintf(stderr, "Unable to read '%s': %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!isDirectory(argv[2]))
    {
        if ((fd1 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
        {
            fprintf(stderr, "Unable to write to '%s': %s\n", argv[2], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        char *basec, *bname;
        basec = strdup(argv[1]);
        bname = basename(basec);
        strcat(argv[2], "/");
        strcat(argv[2], bname);
        if ((fd1 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            fprintf(stderr, "Unable to write to '%s': %s\n", argv[2], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    while ((data = read(fd, buff, BUFFSIZE)) > 0)
    {
        if (write(fd1, buff, data) != data)
        {
            printf("Error in writing content into the file : %s ", argv[2]);
        }
    }

    if (data == -1)
    {
        printf("Error in reading content on file :: %s\n", argv[1]);
    }

    (void) close(fd);
    (void) close(fd1);
    exit(EXIT_SUCCESS);
}