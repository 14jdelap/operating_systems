#ifndef PROCESS_EXPLORATIONS
#define PROCESS_EXPLORATIONS

void ordering_parent_child_execution();
void initialized_variables_in_scope_of_child();
void child_independent_scope();
void parent_independent_scope();
void child_and_parent_write_to_file();
void child_writes_to_file_after_parent_closes_fd();
void write_to_stdout_after_closing_in_child();
void nondeterministic_print_output();
void deterministic_print_output();

#endif
