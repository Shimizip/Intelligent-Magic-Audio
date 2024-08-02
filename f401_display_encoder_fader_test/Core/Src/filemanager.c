/**
  ******************************************************************************
  * @file           : filemanager.c
  * @brief          : File management functions for handling files and cursor operations
  ******************************************************************************
  * @attention
  *
  * This file contains the definitions and functions for managing files within the
  * FileManager structure, including adding files, updating cursor positions, and
  * handling file selection. It serves as the core for file management operations.
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filemanager.h"
#include "display.h"


/**
 * @brief Initializes default values for the FileManager.
 *
 * This function sets default values for the given `FileManager` structure.
 * It initializes various fields to prepare the file manager for use. 
 * 
 * @param[in] fm Pointer to the `FileManager` structure to be initialized.
 */
void initializeFileManager(FileManager *fm) {
    fm->num_files = 0;
    fm->num_matched_files = 0;
    fm->current_file_index = -1;
    fm->cursor_index = -1;
}
/**
 * @brief Adds a file to the FileManager.
 *
 * This function adds a file to the `files[]` array within the `FileManager` structure. It associates the given file name with its corresponding class values and updates the file manager accordingly.
 *
 * @param[in] fm Pointer to the `FileManager` structure to which the file will be added.
 * @param[in] filename The name of the file to be added to the `FileManager`.
 * @param[in] classes Array of float values representing the file's class percentages.
 *
 * @note Ensure that the `FileManager` has sufficient capacity in the `files[]` array to accommodate new files.
 */

void addFile(FileManager *fm, const char *filename, const float *classes){
    if (fm->num_files < MAX_FILES) {
        strcpy(fm->files[fm->num_files].filename, filename);
        for(int i = 0; i < MAX_CLASSES; i++){
            fm->files[fm->num_files].classes[i] = classes[i];
        }
        fm->num_files++;
        if(fm->num_files == 1)
            fm->cursor_index = 0;
    }
}
/**
 * @brief Decrements the cursor index in the FileManager.
 *
 * This function decrements the `cursor_index` within the `FileManager` 
 * structure to move the cursor visually higher on the LCD display. 
 *  
 * @param[in] fm Pointer to the `FileManager` structure whose `cursor_index` is to be decremented.
 *
*/
void cursorUp(FileManager *fm){
    if (fm->cursor_index > 0) {
        fm->cursor_index--;
    } else {
        fm->cursor_index = fm->num_matched_files - 1 ;
    }
}
/**
 * @brief Increments the cursor index in the FileManager.
 *
 * This function increments the `cursor_index` within the `FileManager` 
 * structure to move the cursor visually lower on the LCD display. 
 *  
 * @param[in] fm Pointer to the `FileManager` structure whose `cursor_index` is to be incremented.
 *
*/
void cursorDown(FileManager *fm){
    if (fm->cursor_index < fm->num_matched_files - 1) {
        fm->cursor_index++;
    } else {
        fm->cursor_index = 0;
    }
}
/**
 * @brief Saves the filename at the current cursor index.
 *
 * This function copies the filename of the file currently pointed to by `cursor_index` in the `FileManager` structure to `current_cursor_filename`. 
 * It updates the `current_cursor_filename` with the filename from the `shownFiles` array at the position indicated by `cursor_index`.
 *
 * @param[in] fm Pointer to the `FileManager` structure from which the filename is retrieved and stored.
 *
 * @note Ensure that `cursor_index` is within the valid range of indices in the `shownFiles` array to avoid out-of-bounds access. 
 * The `current_cursor_filename` should have sufficient space allocated to store the filename.
 */
void safeCurrentFileName(FileManager *fm){
	strcpy(fm->current_cursor_filename, fm->shownFiles[fm->cursor_index].filename);
}

/**
 * @brief Sets the cursor position based on the currently displayed files in the FileManager.
 *
 * This function adjusts the `cursor_index` in the `FileManager` based on the currently displayed files and their relevance to the fader settings. The behavior of the function is as follows:
 * 
 * - If no files match the fader settings (`num_matched_files` is 0):
 *   - The cursor is set to the start (index 0).
 * - If the `current_cursor_filename` matches one of the filenames in `shownFiles`:
 *   - The cursor is positioned at the index of the matching file in the `shownFiles` array.
 * - If the `current_cursor_filename` no longer exists in `shownFiles`:
 *   - The `current_cursor_filename` is updated to the filename at the position indicated by `current_file_index`.
 * - If `cursor_index` is larger than the number of matched files:
 *   - The `cursor_index` is set to the last position in the list.
 * 
 * @param[in] fm Pointer to the `FileManager` structure used to adjust the cursor position.
 */

void setCursor(FileManager *fm){
	int res = NOT_EXISTS;
	//If no file match with the fader settings
	if( fm->num_matched_files == 0){
		fm->cursor_index = 0;
		return;
	}
	//If the current current_cursor_filename match with one of the shownFiles on a position in array
	//set the cursor with the position
	for(int i = 0; i < fm->num_matched_files; i++){
		res = strcmp(fm->current_cursor_filename, fm->shownFiles[i].filename);
		if(res == EXISTS){
			fm->cursor_index = i;
			fm->current_file_index = i;
			return;
		}
	}
	//If the file is not exists anymore in shownFiles set the current_cursor_filename with the name
	//of the file on the same position that the current_file_index is.
	if (fm->num_matched_files >= fm->cursor_index){
		strcpy(fm->current_cursor_filename, fm->shownFiles[fm->current_file_index].filename);
	}
	else {
		// when cursor_index is bigger than the list set him as the end position of the list
		fm->cursor_index = fm->num_matched_files - 1;
	}
}
/**
 * @brief Sets the current file index to the cursor position.
 *
 * This function updates the `current_file_index` in the `FileManager` to the current position of the `cursor_index`. 
 * This allows the file at the cursor position to be marked as the selected file, which can then be processed or displayed as needed.
 *
 * @param[in] fm Pointer to the `FileManager` structure in which the `current_file_index` will be set to the value of `cursor_index`.
 */

void selectFile(FileManager *fm) {
	fm->current_file_index = fm->cursor_index;
}

