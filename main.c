#include <stdio.h>
#include <stdlib.h>

void hello_user(char* inPath, char* outPath, size_t size)
{
    printf("Hello! I'm archiver that uses PPM algorithm\n");
    printf("Input your directory:\n");
    fgets(inPath, size, stdin);

    printf("Input the final directory:\n");
    fgets(outPath, size, stdin);
}

int main()
{
    size_t buffer_size = 256;
    char* inPath = (char*)malloc(buffer_size * sizeof(char));
    char* outPath = (char*)malloc(buffer_size * sizeof(char));
    hello_user(inPath, outPath, buffer_size);
    printf(outPath);
    return 0;
}