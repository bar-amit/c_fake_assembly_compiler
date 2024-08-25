#include <stdio.h>

/**
 * Opens a new file stream.
 * 
 * @param file_pointer Pointer to refrence stream.
 * @param file_name Name of file for which to create the stream.
 * @param mode Either 'r' for read or 'w' for write.
 */
FILE *open_stream(FILE *file_pointer, char* file_name, char *mode);

/**
 * Alias to open a file read stream.
 * 
 * @param file_pointer Pointer to refrence stream.
 * @param file_name Name of file for which to create the stream.
 */
FILE *read_stream(FILE *file_pointer, char *file_name);

/**
 * Alias to open a file write stream
 * 
 * @param file_pointer Pointer to refrence stream.
 * @param file_name Name of file for which to create the stream.
 */
FILE *write_steam(FILE *file_pointer, char *file_name);
