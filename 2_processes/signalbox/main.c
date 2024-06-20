#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

void clear_and_draw_box() {
  struct winsize size;
  ioctl(0, TIOCGWINSZ, &size);

  // Clear the terminal
  system("clear");

    // Calculate centered box dimensions
  int box_width = size.ws_col / 2;
  int box_height = size.ws_row / 2;

  // Calculate offsets for centering
  int top_offset = (size.ws_row - box_height) / 2;
  int left_offset = (size.ws_col - box_width) / 2;

  // Print top empty lines
  for (int i = 0; i < top_offset; i++) {
    printf("\n");
  }

  // Draw top border
  for (int i = 0; i < left_offset; i++) {
    printf(" ");
  }
  for (int i = 0; i < box_width; i++) {
    printf("—");
  }
  printf("\n");

  // Draw vertical sides
  for (int i = 0; i < box_height - 2; i++) {
    // Print leading spaces for centering
    for (int j = 0; j < left_offset; j++) {
      printf(" ");
    }
    printf("|"); // Left side
    for (int j = 0; j < box_width - 2; j++) {
      printf(" "); // Fill the box
    }
    printf("|"); // Right side
    printf("\n");
  }

  // Draw bottom border
  for (int i = 0; i < left_offset; i++) {
    printf(" ");
  }
  for (int i = 0; i < box_width; i++) {
    printf("—");
  }
  printf("\n"); // flush the buffer and print the last row
}

int main() {
  clear_and_draw_box();

  while(1){
    struct sigaction action;
    action.sa_sigaction = clear_and_draw_box;

    sigaction(SIGWINCH, &action, NULL);
  };

  return 0;
}
