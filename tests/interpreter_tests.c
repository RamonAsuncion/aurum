#include "interpreter.h"

int main(int argc, char *argv[]) {
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

  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: Could not open file '%s'\n", filename);
    return 1;
  }

  // Determine the file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate a buffer to hold the file contents
  char *source_code = (char*)malloc(file_size + 1);
  if (!source_code) {
    printf("Error: Could not allocate memory for file contents\n");
    fclose(file);
    return 1;
  }

  // Read the file contents into the buffer
  size_t bytes_read = fread(source_code, 1, file_size, file);
  if (bytes_read != file_size) {
    printf("Error: Could not read file '%s'\n", filename);
    fclose(file);
    free(source_code);
    return 1;
  }

  // Null-terminate the buffer
  source_code[file_size] = '\0';

  // Close the file
  fclose(file);

  // Run the interpreter on the file contents
  run_interpreter(source_code);

  // Free the buffer
  free(source_code);

  return 0;
}
