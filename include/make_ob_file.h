/**
 * Writes the code image.
 * 
 * @param code_image Encoded instractions.
 * @param data Data table.
 * @param instraction_count Number of instractions.
 * @param file_name Output file name.
 */
void write_ob_file(unsigned short* code_image, data_table* data, int instraction_count, char* file_name);

/**
 * Second pass of code to encode the instractions.
 * 
 * @param code_image Array to store the code image.
 * @param ob_file Preproccessed code from first pass.
 * @param data Table of data.
 * @param entries Labels of ".extern" of instraciot line.
 * @param errors Errors log.
 */
void make_assembly(unsigned short* code_image,
    file_head* ob_file, data_table* data,
    entry_table* entries, file_head* errors);

/**
 * Encodes an instraction line.
 * 
 * @param operation Operation name.
 * @param first_operand First operand.
 * @param second_operand Second operand.
 * @param errors Errors log.
 * @param source_line Line number in source file.
 */
unsigned short get_instraction_encoding(char* operation,
    char* first_operand, char* second_operand,
    file_head* errors, int source_line);

/**
 * Encodes the address of two registers in one word.
 * 
 * @param source_operand Source register name.
 * @param destination_operand Destination register name.
 */
unsigned short get_registers_encoding(char* source_operand,
    char* destination_operand);

/**
 * Encodes Register address
 * 
 * @param register_name Name of register.
 * @param position Either SOURCE or DESTINATION.
 */
unsigned short get_register_encoding(char* register_name, int position);

/**
 * Gets the number of a register from its name.
 * 
 * @param register_name Name of register. 
 */
unsigned short get_register_number(char* register_name);

/**
 * Encodes a label address.
 * 
 * @param operand Label name.
 * @param position Position of operand (source or destination).
 * @param operand_amount Amount of operands.
 * @param data Data table.
 * @param entries Table of entries.
 * @param instraction_line Line number where the label is used
 */
unsigned short get_label_encoding(char* operand, int position, int operand_amount,
    data_table* data, entry_table* entries, int instraction_line);

/**
 * Encodes data label
 * 
 * @param data_label Data to encode.
 * @param data_start_address Start of data in memory.
 */
unsigned short get_data_encoding(data_unit* data_label,
    int data_start_address);

/**
 * Encodes entry address
 * 
 * @param entry Entry item to encode.
 * @param position Position of operand (source or destination).
 * @param operand_amount Amount of operands.
 * @param instraction_line Line number in code image where the label is used
 */
unsigned short get_entry_encoding(entry_label* entry, int position, int operand_amount, int instraction_line);

/**
 * Encodes an immediate value.
 * 
 * @param operand Value to encode.
 */
unsigned short get_immediate_encoding(char* operand);

/**
 * Encodes an operand addres or value.
 * 
 * @param operand Operand to encode.
 * @param position Either SOURCE or DESTINATION.
 * @param operand_amount Amount of operands in instraction line.
 * @param data Data table.
 * @param entries Entries table.
 * @param instraction_line Line number in code image.
 */
unsigned short get_operand_encoding(char* operand,
    int position, int operand_amount, data_table* data,
    entry_table* entries, int instraction_line);

/**
 * Encodes the addressing method of an operand
 * 
 * @param operand Operand to encode.
 */
unsigned short get_operand_address_method(char* operand);

/**
 * Finds the operation code for an operation.
 * 
 * @param operation Operation name.
 */
unsigned short get_operation_code(char* operation);

/**
 * Gives the amount of operands needed for an operation.
 * 
 * @param operation_code An operation code.
 */
int get_operands_amount(int operation_code);

/**
 * Checks if a given addressing method is acceptable for a given operation.
 * 
 * @param operation_code An operation code.
 * @param operand_position Either SOURCE or DESTINATION.
 * @param address_methos One of ADDRESSING_CODE values.
 * @param errors Errors log.
 * @param source_line Number of line in source file.
 */
void is_address_method_allowed(int operation_code,
    int operand_position, int address_method,
    file_head* errors, int source_line);
