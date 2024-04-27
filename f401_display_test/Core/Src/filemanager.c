#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filemanager.h"
#include "display.h"

#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 20

void initializeFileManager(FileManager *fm) {
    fm->num_files = 0;
    fm->current_file_index = -1;
    fm->cursor_index = -1;
}

void addFile(FileManager *fm, const char *filename) {
    if (fm->num_files < MAX_FILES) {
        strcpy(fm->files[fm->num_files].filename, filename);
        fm->num_files++;
        if(fm->num_files == 1)
            fm->cursor_index = 0;
    }
}

void cursorUp(FileManager *fm){
    if (fm->cursor_index > 0) {
        fm->cursor_index--;
    } else {
        fm->cursor_index = fm->num_files - 1 ;
    }
}

void cursorDown(FileManager *fm){
    if (fm->cursor_index < fm->num_files - 1) {
        fm->cursor_index++;
    } else {
        fm->cursor_index = 0;
    }
}

void selectFile(FileManager *fm) {
    fm->current_file_index = fm->cursor_index;
}