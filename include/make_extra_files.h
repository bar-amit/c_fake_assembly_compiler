/**
 * Creates the entry file (.etn)
 * 
 * @param entries Table of entries defined in source code.
 * @param file_name The name of file to be created.
 */
void write_entry_file(entry_table* entries, int data_start_address, char* file_name);

/**
 * Creates the external file (.ext)
 * 
 * @param entries Table of entries defined in source code.
 * @param file_name The name of file to be created.
 */
void write_external_file(entry_table* entries, char* file_name);
