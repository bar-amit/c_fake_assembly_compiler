macro_list* get_macros(file_head* file, file_head* errors, file_head* warnings);
int is_macro(char *line);
int is_end(char *line);
int starts_with(char *start, char *str);
void get_name(char **target, char *line);
int validate_name(char *name, macro_list *list);
int is_in_list(char *name, char **list);
