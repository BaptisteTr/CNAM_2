#ifndef MY_SH_H_   /* Include guard */
#define MY_SH_H_


void print_usage(char* bin_name);

void free_if_needed(void* to_free);

char* dup_optarg_str();

int interpret_line(char* input_line, char*** res);

int main(int argc, char** argv);

const char simple_command[] = { "ls", "ps", "who" };

const char built_in_command[] = { "cd", "pwd", "exit", "echo" };

#endif // MY_SH_