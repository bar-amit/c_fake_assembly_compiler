/**
 * Opens macro declarations and format the file.
 * 
 * @param source_file Source file.
 * @param list Macro list of macros in file.
 * @param warnings Warnings log.
 */
file_head* make_am_file(file_head *source_file, macro_list* list, file_head* warnings);

/**
 * Copies the lines of a macro were it is used.
 * 
 * @param source_file Source file.
 * @param am_file Output am_file.
 * @param macro A macro routine to copy.
 */
void spred_macro(file_head* source_file, file_head* am_file, macro_routine* macro);

/**
 * Checks for unused macros and writes warnings if needed.
 * 
 * @param macros Macros list.
 * @param warnings Warnings log.
 */
void warn_unused(macro_list* macros, file_head* warnings);
