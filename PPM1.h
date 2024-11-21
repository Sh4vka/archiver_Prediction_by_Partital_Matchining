#ifndef PPM1_H
#define PPM1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

void compress_directory_to_file(const char *input_dir, const char *output_file);
void compress_directory_recursive(const char *dir_path, FILE *out, size_t base_len);
void decompress_file_to_directory(const char *input_file, const char *output_dir);
void mkdir_recursive(const char *dir_path);

#endif // PPM1_H