/**
 * Removes last character of str if it is a space.
 */
char* remove_last_space(char* str);

/**
 * Clean whitespace from a line.
 */
char *clean_whitespace(char *str);

/**
 * Remove first character of str if it is a space.
 */
char* remove_first_space(char* str);

/**
 * Strip str from spaces at the start and ending of string
 */
char* strip_string(char* str);

/**
 * A utility to read through tokens in a line.
 */
char* get_next_token(char *str, char* delimiter);

/**
 * Replaces file name ending.
 * 
 * @param file_name Original file name.
 * @param ending New ending to file name.
 */
char* replace_file_ending(char* file_name, char* ending);
