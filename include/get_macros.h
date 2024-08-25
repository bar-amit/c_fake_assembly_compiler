/**
 * Creates a table of macros in the file.
 * 
 * @param file source file.
 * @param errors An error log.
 * @param warnings A warnings log.
*/
macro_list* get_macros(file_head* file, file_head* errors, file_head* warnings);

/**
 * Check if a line is a macro declaration.
 * 
 * @param line Line content.
 */
int is_macro(char *line);

/**
 * Checks if a line is an end of macro declaration.
 * 
 * @param line Line content.
 */
int is_end(char *line);

/**
 * Checks if a line starts with a given word.
 * 
 * @param start Prefix to check for.
 * @param str A string to check in.
 */
int starts_with(char *start, char *str);

/**
 * Gets the name of a macro from declaration.
 * 
 * @param target Target to store the macro name in table.
 * @param line Line to get the macro name from.
 */
void get_name(char **target, char *line);

/**
 * Validates a macro name.
 * 
 * @param name Name to validate.
 * @param list A macro list to look if macro name is free to use.
 */
int validate_name(char *name, macro_list *list);
