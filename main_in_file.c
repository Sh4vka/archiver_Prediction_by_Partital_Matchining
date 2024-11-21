#include "PPM1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int choose;
    printf("Hello!\nInput 1 for compression\nInput 2 for decompression\n");
    scanf("%d", &choose);  

    if (choose == 1) {
        char dir_name[1024];
        char out_dir_name[1024];
        printf("Enter the path to the directory for compression: ");
        scanf("%s", dir_name);
        printf("Enter the path to the directory result: ");
        scanf("%s", out_dir_name);
        
        compress_directory_to_file(dir_name, out_dir_name);
        printf("Compression completed!\n");

    } else if (choose == 2) {
        char dir_name[1024];
        char out_dir_name[1024];
        printf("Enter the path to the directory for decompression: ");
        scanf("%s", dir_name);
        printf("Enter the path to the directory result: ");
        scanf("%s", out_dir_name);

        decompress_file_to_directory(dir_name, out_dir_name); 
        printf("Decompression completed!\n");

    } else {
        printf("Invalid option. Please enter 1 or 2.\n");
    }

    return 0;
}