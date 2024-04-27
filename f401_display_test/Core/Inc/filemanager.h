#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 20

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
} File;

typedef struct {
    File files[MAX_FILES];
    int num_files;
    int cursor_index;
    int current_file_index;
} FileManager;

void initializeFileManager(FileManager *fm);
void addFile(FileManager *fm, const char *filename);
void selectFile(FileManager *fm);
void cursorUp(FileManager *fm);
void cursorDown(FileManager *fm);


#endif /* FILE_MANAGER_H */
