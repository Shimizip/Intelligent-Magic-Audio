#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Maximum number of files and filename length.
 *
 * These constants define the maximum number of files that can be handled and the maximum length of a filename.
 */
#define MAX_FILES 500
#define MAX_FILENAME_LENGTH 30

/**
 * @brief Number of classes for file classification.
 *
 * This constant defines the number of classes used in the sorting algorithm to categorize files.
 */
#define MAX_CLASSES 4

/**
 * @brief File existence status codes.
 *
 * These constants are used to indicate whether a file exists or not.
 */
#define EXISTS 0
#define NOT_EXISTS -1

/**
 * @brief Structure representing a file with its name and classification classes.
 *
 * This structure stores information about a file, including its name and its classification classes.
 */
typedef struct {
    char filename[MAX_FILENAME_LENGTH];    ///< The name of the file.
    float classes[MAX_CLASSES];            ///< The classification classes for the file.
} File;

/**
 * @brief Structure to manage the files and their display on the LCD.
 *
 * This structure is used to manage files, including storing all files, filtering files based on fader settings, and keeping track of the current selection.
 */
typedef struct {
    File files[MAX_FILES];                 ///< Array to store all files that have been classified.
    File shownFiles[MAX_FILES];            ///< Array to store files that match the fader settings.
    char current_cursor_filename[25];      ///< The filename of the file currently selected by the user.
    int num_files;                         ///< The total number of files.
    int num_matched_files;                 ///< The number of files that match the fader settings.
    int cursor_index;                      ///< The index of the current cursor position.
    int current_file_index;                ///< The index position of the selected file.
    float fader_Class[MAX_CLASSES];        ///< The classification classes after the file has been classified.
} FileManager;

void initializeFileManager(FileManager *fm);
void addFile(FileManager *fm, const char *filename, const float *classes);
void selectFile(FileManager *fm);
void cursorUp(FileManager *fm);
void cursorDown(FileManager *fm);
void safeCurrentFileName(FileManager *fm);
void setCursor(FileManager *fm);



#endif /* FILE_MANAGER_H */
