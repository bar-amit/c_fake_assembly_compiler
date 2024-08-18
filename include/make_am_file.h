file_head* make_am_file(file_head *source_file, char* file_name, macro_list* list, file_head* warnings);
void spred_macro(file_head* source_file, file_head* am_file, macro_routine* macro);
void warn_unused(macro_list* macros, file_head* warnings);
