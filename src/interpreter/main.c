#include "interpreter.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
  if (argc < 2) {
    printf("Usage: %s <source_file>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  const char *fileExtension = strrchr(filename, '.');

  if (!fileExtension || strcmp(fileExtension, ".au") != 0) {
    printf("Error: Unknown-unsupported file format.\n");
    return 1;
  }

  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("Error opening file for reading");
    return 1;
  }

  struct stat fileInfo = {0};
  if (fstat(fd, &fileInfo) == -1) {
    perror("Error getting the file size");
    return 1;
  }

  if (fileInfo.st_size == 0) {
    return 1;
  }

  char *source_code = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (source_code == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    return 1;
  }

  // Run the interpreter on the file contents
  run_interpreter(source_code);

  // Unmap and close the file
  if (munmap(source_code, fileInfo.st_size) == -1) {
    perror("Error un-mmapping the file");
  }
  close(fd);

  return 0;
}
