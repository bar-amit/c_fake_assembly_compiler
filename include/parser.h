int is_preserved(char *name, macro_list *list);
int is_operation(char* str);
int is_instraction(char* str);
int is_register(char* str);
int is_label(char* str);
int validate_string_data(char* str);
char* parse_string_data(char* str);
int is_in_list(char *name, char *list[]);
