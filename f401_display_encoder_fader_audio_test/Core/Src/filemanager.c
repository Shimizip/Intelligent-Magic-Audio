#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filemanager.h"
#include "display.h"

#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 30
#define MAX_CLASSES 2

void initializeFileManager(FileManager *fm) {
    fm->num_files = 0;
    fm->num_matched_files = 0;
    fm->current_file_index = -1;
    fm->cursor_index = -1;
}

void addFile(FileManager *fm, const char *filename, const float *classes){
    if (fm->num_files < MAX_FILES) {
        strcpy(fm->files[fm->num_files].filename, filename);
        for(int i = 0; i < MAX_CLASSES; i++){
            fm->files[fm->num_files].classes[i] = classes[i];        }
        fm->num_files++;
        if(fm->num_files == 1)
            fm->cursor_index = 0;
    }
}

void cursorUp(FileManager *fm){
    if (fm->cursor_index > 0) {
        fm->cursor_index--;
    } else {
        fm->cursor_index = fm->num_matched_files - 1 ;
    }
}

void cursorDown(FileManager *fm){
    if (fm->cursor_index < fm->num_matched_files - 1) {
        fm->cursor_index++;
    } else {
        fm->cursor_index = 0;
    }
}

void safeCurrentFileName(FileManager *fm){
	strcpy(fm->current_cursor_filename, fm->shownFiles[fm->cursor_index].filename);
}

void setCursor(FileManager *fm){
	int res = NOT_EXISTS;

	if( fm->num_matched_files == 0){
		fm->cursor_index = 0;
		return;
	}
	for(int i = 0; i < fm->num_matched_files; i++){
		res = strcmp(fm->current_cursor_filename, fm->shownFiles[i].filename);
		if(res == EXISTS){
			fm->cursor_index = i;
			fm->current_file_index = i;
			return;
		}
	}
	if (fm->num_matched_files >= fm->cursor_index){
		strcpy(fm->current_cursor_filename, fm->shownFiles[fm->current_file_index].filename);
	}
	else {
		fm->cursor_index = fm->num_matched_files - 1;
	}
}

void selectFile(FileManager *fm) {
	fm->current_file_index = fm->cursor_index;
}

