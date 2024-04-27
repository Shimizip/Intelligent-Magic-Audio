#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filemanager.h"

#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 20

void initializeFileManager(FileManager *fm) {
    fm->num_files = 0;
    fm->current_file_index = -1;
}

void addFile(FileManager *fm, const char *filename) {
    if (fm->num_files < MAX_FILES) {
        strcpy(fm->files[fm->num_files].filename, filename);
        fm->num_files++;
    }
}

void selectFile(FileManager *fm, int index) {
    if (index >= 0 && index < fm->num_files) {
        fm->current_file_index = index;
    }
}