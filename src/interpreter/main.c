#include "interpreter.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define RL_BUFSIZE 1024

char *read_line(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "Aurum: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF) {
      free(buffer);  // free the buffer to avoid memory leak
      return NULL;   // return NULL when EOF is encountered
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "Aurum: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void interactive_mode(void) 
{
  char *line;
  printf("Welcome to Aurum!\n");
  while (1) {
    printf(">>> ");
    line = read_line();
    if (line == NULL) {    
      break;  
    }

    if (strcmp(line, "exit") == 0) {
      free(line);
      exit(0);
    }

    run_interpreter(line);
    free(line);
  }
}


void run_interpreter_from_file(const char *filename)
{
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("Error opening file for reading");
    return;
  }

  struct stat fileInfo = {0};
  if (fstat(fd, &fileInfo) == -1) {
    perror("Error getting the file size");
    return;
  }

  if (fileInfo.st_size == 0) {
    return;
  }

  char *source_code = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (source_code == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    return;
  }

  // Run the interpreter on the file contents
  run_interpreter(source_code);

  // Unmap and close the file
  if (munmap(source_code, fileInfo.st_size) == -1) {
    perror("Error un-mmapping the file");
  }
  close(fd);
}

int main(int argc, char *argv[]) 
{
  if (argc < 2) {
    interactive_mode();
  } else {
    const char *filename = argv[1];
    const char *fileExtension = strrchr(filename, '.');

    if (!fileExtension || strcmp(fileExtension, ".au") != 0) {
      printf("Error: Unknown-unsupported file format.\n");
      return 1;
    }
    run_interpreter_from_file(filename);
  }
  return 0;
}
