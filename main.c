#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void hello_user(char* inPath, char* outPath, size_t size)
{
    printf("Hello! I'm archiver that uses PPM algorithm\n");
    printf("Input your directory:\n");
    fgets(inPath, size, stdin);
    inPath[strcspn(inPath, "\n")] = '\0';

    printf("Input the final directory:\n");
    fgets(outPath, size, stdin);
    outPath[strcspn(outPath, "\n")] = '\0';
}

void work_with_dir(char* inPath)
{
    DIR* dir;
    struct dirent *entry;

    if (!(dir = opendir(inPath)))
    {
        printf("Error open!");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", inPath, entry->d_name);

        struct stat info;
        if (stat(path, &info) == 0)
        {
            if (S_ISDIR(info.st_mode))
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") ==0)
                    continue;
                printf("Dirrectory: %s\n", path);
                work_with_dir(path);
            }
            
            if (S_ISREG(info.st_mode))
            {
                printf("File: %s\n", path);
            }
        }
        else
            printf("Error!");
    }
    closedir(dir);
}

int main()
{
    size_t buffer_size = 256;
    char* inPath = (char*)malloc(buffer_size * sizeof(char));
    char* outPath = (char*)malloc(buffer_size * sizeof(char));
    hello_user(inPath, outPath, buffer_size);
    work_with_dir(inPath);
    return 0;
}