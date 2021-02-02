#include <iostream>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

int main (int argc, char* argv[]) {
    char mycwd[PATH_MAX];

    if (getcwd(mycwd, PATH_MAX) == NULL) {
        perror("Failed to get current working directory");
        return 1;
    }

    printf("Current working directory: %s\n", mycwd);

    char buf[512]; // buffer for getting values with stat
    struct dirent* direntp; // struct for dirent use
    struct stat fileStat; // struct for stat use
    DIR* dirp = opendir(argv[1]); // open directory
    while ((direntp = readdir(dirp)) != NULL) {        
        // fill buffer
        sprintf(buf, "%s/%s", argv[1], direntp->d_name);
        
        // setup stat
        stat(buf, &fileStat);

        // File permissions & type
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" ");

        // Number of links
        printf("%zu", fileStat.st_nlink);
        printf(" ");

        // Owner ID
        printf("%d", fileStat.st_uid);
        printf(" ");

        // Group ID
        printf("%d", fileStat.st_gid);
        printf(" ");
        
        // File size in bytes
        printf("%zu", fileStat.st_size);
        printf(" ");
        
        // Last modification time
        printf("%s", ctime(&fileStat.st_mtime));
        printf(" ");

        // File/dir name
        printf("%s", direntp->d_name);
        printf("\n");
    }
    
    closedir(dirp); // close directory
    return 0;
}