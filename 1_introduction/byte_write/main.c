// This solution uses C library functions instead of syscalls
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define ONE_MEG 1 << 20

void handle_error(char* msg) {
  fprintf(stderr, "%s", msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  FILE* f = fopen("./tmp", "a+");
  if (f == NULL) handle_error("failed to open file");

  setbuf(f, NULL);

  int num_blocks = 0;
  struct stat file_stat;

  for (int i = 0; i < ONE_MEG; i++) {
    fwrite("h", sizeof(char), 1, f);
    fflush(f);
    stat("./tmp", &file_stat);
    if (num_blocks != (int)file_stat.st_blocks) {
      printf("File size: %lld, blocks: %lld, bytes on disk: %lld\n", file_stat.st_size, file_stat.st_blocks, file_stat.st_blocks*512);
      num_blocks = (int)file_stat.st_blocks;
    }
  }

  int result = remove("./tmp");
  if (-1 == result) handle_error("failed to delete file");
  return 0;
}
