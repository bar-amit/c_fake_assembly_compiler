int is_preserved(char *name, macro_list *macros, data_table *labels);
int is_operation(char* str);
int is_instraction(char* str);
int is_register(char* str);
int is_indirect_register(char* str);
int are_registers(char* source_operand, char* destination_operand);
int is_label(char* str);
int is_alnum(char *str);
int is_entry(char* str);
int is_data(char* str);
int validate_string_data(char* str);
char* parse_string_data(char* str);
int is_in_list(char *name, char *list[]);
numeric_data* parse_numeric_data(char *str);
int validate_numeric_data(char* str);
void parse_line(file_head* ob_output, data_table* labels, entry_table* entries, file_line* line,
    file_head* errors, file_head* warnings);
file_head* parse_source(file_head* am_file, file_head* errors, file_head* warnings);
file_line* parse_operation(char* operation, char* orpands, int source_line, file_head* errors);
data_unit* parse_data(char* label_name, char* instraction, char* orpand, int source_line, file_head* errors);
int get_data_code(char *instraction);
void parse_entry(entry_table* entries, char* name, int type_code, int source_line, int instraction_line,
    file_head* errors);
int get_entry_code(char *instraction);
int validate_label_name(char* label_name, data_table* labels, macro_list* macros);
