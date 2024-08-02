#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


<<<<<<< HEAD:f401_display_encoder_fader_audio_integration/Core/Inc/filemanager.h
#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 30
#define MAX_CLASSES 2
=======
#define MAX_FILES 500
#define MAX_FILENAME_LENGTH 30
#define MAX_CLASSES 4
>>>>>>> c7ea2e0bfee7c0fb2f2219cbbc4721e5d281f12d:f401_display_encoder_fader_test/Core/Inc/filemanager.h
#define EXISTS 0
#define NOT_EXISTS -1

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    float classes [MAX_CLASSES];
} File;

typedef struct {
    File files[MAX_FILES];
    File shownFiles[MAX_FILES];
    char current_cursor_filename[25];
    int num_files;
    int num_matched_files;
    int cursor_index;
    int current_file_index;
    float fader_Class[MAX_CLASSES];
} FileManager;

void initializeFileManager(FileManager *fm);
void addFile(FileManager *fm, const char *filename, const float *classes);
void selectFile(FileManager *fm);
void cursorUp(FileManager *fm);
void cursorDown(FileManager *fm);
void safeCurrentFileName(FileManager *fm);
void setCursor(FileManager *fm);



#endif /* FILE_MANAGER_H */
