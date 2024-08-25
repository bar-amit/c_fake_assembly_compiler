/**
 * Recieve a new message to log.
 * 
 * @param messages A log of messages to add the message to.
 * @param message A new message to add to log.
 * @param source_line Line number in source file which caused the log message.
 */
void handle_message(file_head* messages, char* message, int source_line);

/**
 * Prints warnings log.
 * 
 * @param warnings Warning messages.
 * @param source_file File which caused warnings.
 */
void print_warnings(file_head* warnings, file_head* source_file);

/**
 * Prints errors log.
 * 
 * @param warnings Error messages.
 * @param source_file File which caused errors.
 */
void print_errors(file_head* errors, file_head* source_file);

/**
 * Prints messages in log.
 * 
 * @param lines Log lines.
 * @param source_file Source file which caused the messages.
 * @param message Prefix frase to put before message line.
 */
void print_messages(file_head* lines, file_head* source_file, char* message);
