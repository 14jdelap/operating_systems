#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define ONE_MEG 1 << 20

void handle_error(char* msg) {
  fprintf(stderr, "%s", msg);
  exit(1);
}

int main() {
  int fd = open("/tmp/foo.txt", O_CREAT | O_WRONLY | O_TRUNC);
  if (fd == -1) handle_error("failed to open file");

  int num_blocks = 0;
  struct stat file_stat;

  for (int i = 0; i < ONE_MEG; i++) {
    int bytes_written = write(fd, "h", 1);
    if (bytes_written == -1) handle_error("failed to write to file");

    fstat(fd, &file_stat);
    if (num_blocks != (int)file_stat.st_blocks) {
      printf("File size: %lld, blocks: %lld, bytes on disk: %lld\n",
        file_stat.st_size, file_stat.st_blocks, file_stat.st_blocks*512
      );
      num_blocks = (int)file_stat.st_blocks;
    }
  }

  int result = remove("/tmp/foo.txt");
  if (-1 == result) handle_error("failed to delete file");
  return 0;
}
