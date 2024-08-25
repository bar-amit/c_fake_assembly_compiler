typedef struct parsed_labels{
    entry_table* entries;
    data_table* data;
} pased_labels;

/**
 * Checks if a given name is preserved or allready in use.
 */
int is_preserved(char *name, macro_list *macros, data_table *labels);

/**
 * Checks if a string is an operation name.
 */
int is_operation(char* str);

/**
 * Checks if a string is a name of an instraction.
 */
int is_instraction(char* str);

/**
 * Checks if a string is a register name.
 */
int is_register(char* str);

/**
 * Checks if a register name is given as an indirect address.
 */
int is_indirect_register(char* str);

/**
 * Checks if both operands are registers.
 */
int are_registers(char* source_operand, char* destination_operand);

/**
 * Checks if a string is a ligal label name.
 */
int is_label(char* str);

/**
 * Checks if a string is composed of alphbetic and numeric characters, only.
 */
int is_alnum(char *str);

/**
 * Checks if string is a declaration of ".entry" or ".extern".
 */
int is_entry(char* str);

/**
 * Check if string is a declaration of ".string" or ".data".
 */
int is_data(char* str);

/**
 * Validates a ".string" declaration.
 */
int validate_string_data(char* str);

/**
 * Parses string data given by ".string" declaration.
 */
char* parse_string_data(char* str);

/**
 * Checks if a name is found in list.
 */
int is_in_list(char *name, char *list[]);

/**
 * Parses numeric data given by ".data" declaration. 
 */
numeric_data* parse_numeric_data(char *str);

/**
 * Validates data given to a ".data" declaration.
 */
int validate_numeric_data(char* str);

/**
 * Parses tokens in a line of an am_file.
 * 
 * @param ob_output Output lines after first pass to be proccessed in second pass.
 * @param data A table of data defined in source file.
 * @param entries Table of entries and labels of instraction lines.
 * @param line Line from source to be parsed.
 * @param erros Errors log.
 * @param warnings Warnings log.
 */
void parse_line(
    file_head* ob_output, data_table* data, entry_table* entries,
    file_line* line, file_head* errors, file_head* warnings);

/**
 * First pass of source file to create tables of labels.
 * 
 * @param am_file Source file after macro routines proccessed.
 * @param ob_output Destination of output.
 * @param data Destination of data table.
 * @param entries Destination of entries table.
 * @param errors Errors log.
 * @param warnings Warnings log.
 */
file_head* parse_source(file_head* am_file, file_head* ob_output, data_table* data, entry_table* entries,
    file_head* errors, file_head* warnings);

/**
 * Parse operation line.
 */
file_line* parse_operation(char* operation, char* orpands, int source_line, file_head* errors);

/**
 * Parse data declaration.
 */
data_unit* parse_data(char* label_name, char* instraction, char* orpand, int source_line, file_head* errors);

/**
 * Gets the type of data to parse. Either STRING or NUMERIC.
 */
int get_data_code(char *instraction);

/**
 * Parse label declaration.
 */
void parse_entry(entry_table* entries, char* name, int type_code, int source_line, int instraction_line,
    file_head* errors);

/**
 * Gets the type of label to parse. Either ENTRY or EXTERN.
 */
int get_entry_code(char *instraction);

/**
 * Validates label name.
 */
int validate_label_name(char* label_name, data_table* labels, macro_list* macros);
