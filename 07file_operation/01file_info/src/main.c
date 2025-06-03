#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/* funcs prototypes ------------------*/
/**
 * @brief to print file type and file permissions  
 * @param mode unsigned int<- stat.st_mode
 * @return none
 */
void print_permission(unsigned int mode);


/**
 * @brief to print a file's information
 * @param path full path to the file
 * @param name file name <- dirent->d_name
 * @return none
 */
void print_file_info(const char* path, const char* name);

/**
 * @brief to list all entries in a directory and print each's information
 * 
 * @param dir_path path to the directory to list
 * @return none
 */
void list_dir(const char* dir_path);


int main(int argc, char *argv[]) {
    // get the directory as the argument 
    const char* dir = (argc >= 2) ? argv[1] : ".";  // ls . if no arg given
    list_directory(dir);
    return 0;
}


void print_permission(unsigned int mode) {
    char perms[11];

    perms[0] = S_ISDIR(mode) ? 'd' :        // directory
               S_ISLNK(mode) ? 'l' :        // symbolink 
               S_ISCHR(mode) ? 'c' :        // character divice
               S_ISBLK(mode) ? 'b' :        // block device
               S_ISFIFO(mode) ? 'p' :       // fifo
               S_ISSOCK(mode) ? 's' :       // socket
                '-';                        // normal file

    // owner permissions
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';

    // group permissions
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';

    // other permissions
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';

    perms[10] = '\0';

    printf("%-10s  ", perms);
}

void print_file_info(const char* path, const char* name) {
    struct stat st;
    char atime_str[20], mtime_str[20];

    if (stat(path, &st) == -1) {
        perror("stat");
        return;
    }
    /* 10 2 6 6 15 20 20 120*/

    print_permission(st.st_mode);                                            // permissions
    printf("%-2ld ", (unsigned long)st.st_nlink);                            // hard link even tho I dont know what it is =)) 

    struct passwd *pw = getpwuid(st.st_uid);                                 // owner
    struct group  *gr = getgrgid(st.st_gid);                                 // group 
    printf("%-6s %-6s ", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

    printf("%-15ld ", (long)st.st_size);                                     // size
  
    strftime(atime_str, sizeof(atime_str), "%Y-%m-%d %H:%M:%S", localtime(&st.st_atime));       // last access
    strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));       // last edit

    printf("%-20s %-20s ", atime_str, mtime_str);
    printf("%s\n", name);
}

void list_dir(const char* dir_path) {
    // open the directory
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir"); 
        return;
    }

    printf("%-10s %-2s %-6s %-6s %-15s %-20s %-20s %-20s\n",
           "permissions", " ", "user", "group", "size",
           "last access", "last edit", "file name");

    struct dirent *entry;
    char fullpath[4096];  // full path to file

    // iterate all entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);     // <dir path>/<file name>

        // print each file's info
        print_file_info(fullpath, entry->d_name);
    }

    // close the directory
    closedir(dir);
}


/* 10 2 6 6 15 20 20 120*/
