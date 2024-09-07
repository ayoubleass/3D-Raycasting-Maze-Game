#include <stdio.h>
#include <dirent.h>
#include "../headers/main.h"



void initTexturePaths() {
    DIR *d;
    struct dirent *dir;
    int i = 0;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        d = opendir("images");
        if (d) {
            while ((dir = readdir(d)) != NULL && i < NUM_TEXTURES) {
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    size_t pathLength = strlen(cwd) + strlen("images") + strlen(dir->d_name) + 2; // +2 for separator and null terminator
                    TexturePaths[i] = malloc(pathLength);
                    if (TexturePaths[i] == NULL) {
                        fprintf(stderr, "Failed to allocate memory for texture\n");
                        continue; 
                    }
                    strcpy(TexturePaths[i], cwd);
                    strcat(TexturePaths[i], "\\images\\");
                    strcat(TexturePaths[i],dir->d_name);
                    i++;
                }
            }
            closedir(d);
        } else {
            perror("opendir");
        }
    } else {
        perror("getcwd() error");
    }
}



void freeTexturePaths() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (TexturePaths[i] != NULL) {
            free(TexturePaths[i]);
            TexturePaths[i] = NULL; 
        }
    }
}