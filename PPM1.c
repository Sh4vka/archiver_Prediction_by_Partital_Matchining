#include "PPM1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

void compress_directory_to_file(const char *input_dir, const char *output_file) {
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("Ошибка открытия файла для записи");
        exit(EXIT_FAILURE);
    }

    compress_directory_recursive(input_dir, out, strlen(input_dir) + 1);

    fclose(out);
}

void compress_directory_recursive(const char *dir_path, FILE *out, size_t base_len) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Ошибка открытия директории");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (stat(full_path, &statbuf) != 0) {
            perror("Ошибка получения информации о файле");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            compress_directory_recursive(full_path, out, base_len);
        } else if (S_ISREG(statbuf.st_mode)) {
            FILE *in = fopen(full_path, "rb");
            if (!in) {
                perror("Ошибка открытия файла для чтения");
                continue;
            }

            // Запись метаданных
            const char *relative_path = full_path + base_len;
            size_t path_len = strlen(relative_path) + 1; // включая '\0'
            fwrite(&path_len, sizeof(size_t), 1, out);
            fwrite(relative_path, 1, path_len, out);
            fwrite(&statbuf.st_mode, sizeof(mode_t), 1, out);
            fwrite(&statbuf.st_uid, sizeof(uid_t), 1, out);
            fwrite(&statbuf.st_gid, sizeof(gid_t), 1, out);
            fwrite(&statbuf.st_size, sizeof(off_t), 1, out);

            // Запись данных файла
            char buffer[4096];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
                fwrite(buffer, 1, bytes_read, out);
            }

            fclose(in);
        }
    }

    closedir(dir);
}

void decompress_file_to_directory(const char *input_file, const char *output_dir) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("Ошибка открытия файла для чтения");
        exit(EXIT_FAILURE);
    }

    mkdir(output_dir, 0755); // Создаём корневую директорию для разархивирования

    while (!feof(in)) {
        // Чтение метаданных
        size_t path_len;
        if (fread(&path_len, sizeof(size_t), 1, in) != 1) break;

        char relative_path[1024];
        fread(relative_path, 1, path_len, in);

        mode_t mode;
        uid_t uid;
        gid_t gid;
        off_t file_size;

        fread(&mode, sizeof(mode_t), 1, in);
        fread(&uid, sizeof(uid_t), 1, in);
        fread(&gid, sizeof(gid_t), 1, in);
        fread(&file_size, sizeof(off_t), 1, in);

        // Формирование полного пути для разархивирования
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", output_dir, relative_path);

        // Создание директорий для пути
        char *slash = strrchr(full_path, '/');
        if (slash) {
            *slash = '\0';
            mkdir_recursive(full_path);
            *slash = '/';
        }

        // Восстановление файла
        FILE *out = fopen(full_path, "wb");
        if (!out) {
            perror("Ошибка создания файла");
            continue;
        }

        char buffer[4096];
        size_t bytes_left = file_size;
        while (bytes_left > 0) {
            size_t to_read = (bytes_left > sizeof(buffer)) ? sizeof(buffer) : bytes_left;
            size_t bytes_read = fread(buffer, 1, to_read, in);
            fwrite(buffer, 1, bytes_read, out);
            bytes_left -= bytes_read;
        }

        fclose(out);

        // Восстановление прав доступа
        chmod(full_path, mode);
        chown(full_path, uid, gid);
    }

    fclose(in);
}

void mkdir_recursive(const char *dir_path) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", dir_path);
    size_t len = strlen(tmp);

    if (tmp[len - 1] == '/') tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }

    mkdir(tmp, 0755);
}

