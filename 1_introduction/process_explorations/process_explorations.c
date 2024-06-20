#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ordering_parent_child_execution shows that the parent process doesn't need
// to start first. Different usleep value may lead to the OS scheduler to
// i) always schedule the parent process first (the behavior without usleep in
// my Intel architecture MacBook Air), ii) always schedule the child process
// first, or iii) a non-deterministic scheduling where the parent or child may
// start first. The starting value of 375 shows non-determinism.
void ordering_parent_child_execution() {
  printf("Function: %s\n", __func__);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    usleep(375);
    printf("parent of %d (pid:%d)\n", rc, (int) getpid());
  } else {
    printf("          child (pid:%d)\n", (int) getpid());
  }
}

// child_variable_scope shows that child processes can access the variables
// initialized and assigned before the child is forked.
void initialized_variables_in_scope_of_child() {
  printf("Function: %s\n", __func__);
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    printf("parent of %d (pid:%d), variables (x:%d)\n", rc, (int) getpid(), x);
  } else {
    printf("          child (pid:%d), variable (x:%d)\n", (int) getpid(), x);
  }
}

// child_independent_scope shows that even though both the parent
// and child process have in scope the variables and values before `fork` is
// called, after `fork` they have their own function scopes.
void child_independent_scope() {
  printf("Function: %s\n", __func__);
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    x = 10;
    printf("parent of %d (pid:%d), variable (x:%d)\n", rc, (int) getpid(), x);
  } else {
    // x is still 100
    printf("          child (pid:%d), variable (x:%d)\n", (int) getpid(), x);
  }
}

// parent_independent_scope shows that if the child reassigns a variable that
// was initialized before fork was called, the parent still accesses the
// original value that the child changed.
void parent_independent_scope() {
  printf("Function: %s\n", __func__);
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    usleep(600);
    // x is still 100
    printf("parent of %d (pid:%d), variable (x:%d)\n", rc, (int) getpid(), x);
  } else {
    x = 5;
    printf("          child (pid:%d), variable (x:%d)\n", (int) getpid(), x);
  }
}

// child_and_parent_write_to_file shows that 2 active processes can have the
// same file descriptor and use it to write to the same file. The order of
// elements depends on who writes first.
void child_and_parent_write_to_file() {
  printf("Function: %s\n", __func__);

  FILE *fp = fopen("new_file.txt", "w");
  if (fp == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    printf("parent of %d (pid:%d), fd (x:%p), fileno (%d)\n", rc, (int) getpid(), fp, fileno(fp));
    int bytes_written = fputs("1!", fp);

    // Check if writing was successful (fputs returns -1 on error)
    if (bytes_written == -1) {
      fprintf(stderr, "Error writing to file\n");
      fclose(fp);
    }
  } else {
    printf("          child (pid:%d), fd (x:%p), fileno (%d)\n", (int) getpid(), fp, fileno(fp));
    int bytes_written = fputs("2!", fp);

    // Check if writing was successful (fputs returns -1 on error)
    if (bytes_written == -1) {
      fprintf(stderr, "Error writing to file\n");
      fclose(fp);
    }
  }
}

// child_and_parent_write_to_file shows that 2 active processes can have the
// same file descriptor and use it to write to the same file. The order of
// elements depends on who writes first.
void child_writes_to_file_after_parent_closes_fd() {
  printf("Function: %s\n", __func__);

  FILE *fp = fopen("new_file.txt", "w");
  if (fp == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    printf("parent of %d (pid:%d), fd (x:%p), fileno (%d)\n", rc, (int) getpid(), fp, fileno(fp));
    int bytes_written = fputs("1!", fp);

    // Check if writing was successful (fputs returns -1 on error)
    if (bytes_written == -1) {
      fprintf(stderr, "Error writing to file\n");
    }
    fclose(fp);
  } else {
    printf("          child (pid:%d), fd (x:%p), fileno (%d)\n", (int) getpid(), fp, fileno(fp));
    int bytes_written = fputs("2!", fp);

    // Check if writing was successful (fputs returns -1 on error)
    if (bytes_written == -1) {
      fprintf(stderr, "Error writing to file\n");
      fclose(fp);
    }

    printf("          child wrote to file successfully!\n");
  }
}


// cannot_write_to_stdout_after_closing shows that each process has its own
// stdin/stdout/stderr file descriptors: the parent can still write to stdout
// even after the child closes it because they're different processes.
// It also shows that after the child closes stdout it can't write to stdout.
void write_to_stdout_after_closing_in_child() {
  printf("Function: %s\n", __func__);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    wait(NULL);
    printf("parent of %d (pid:%d)\n", rc, (int) getpid());
  } else {
    printf("          child (pid:%d)\n", (int) getpid());
    fclose(stdout);
    printf("This doesn't print because stdout is closed");
  }
}

// nondeterministic_print_output shows a code snippet where the child process
// doesn't always print its 2nd print statement.
void nondeterministic_print_output() {
  printf("Function: %s\n", __func__);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    printf("parent of %d (pid:%d)\n", rc, (int) getpid());
  } else {
    printf("          child (pid:%d)\n", (int) getpid());
    printf("This doesn't always print");
  }
}

// deterministic_print_output shows a code snippet where the child process
// always print its 2nd print statement by adding a newline at the end.
void deterministic_print_output() {
  printf("Function: %s\n", __func__);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc != 0) {
    printf("parent of %d (pid:%d)\n", rc, (int) getpid());
  } else {
    printf("          child (pid:%d)\n", (int) getpid());
    printf("But with a newline it always seems to print\n");
  }
}
