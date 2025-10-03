#include "kar_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_tree(arch_tree_node *root) {
    if (root == NULL) {
        return;
    }
    if (root->next_file == NULL && root->dir_contents == NULL) {
        free(root);
    } else {
        free_tree(root->next_file);
        free_tree(root->dir_contents);
        free(root);
    }
}

/*
** Some helper functions.
*/
void only_filename(char* filepath){
    int total_chars = strlen(filepath);

    if(filepath[total_chars - 1] == '/'){
        filepath[total_chars - 1] = '\0';
    }

    char* filename = strrchr(filepath, '/');
    for(size_t i = 0; i <= strlen(filename); i++){
        filepath[i] = filename[i + 1];
    }
}

// WARNING: This function assumes that path_buffer is large enough
void valid_path(char path_buffer[], char *directory, char *filename){
    strcpy(path_buffer, directory);

    // If the directory doesn't end with a slash, add one
    if (strlen(path_buffer) > 0){
        if (path_buffer[strlen(path_buffer) - 1] != '/'){
            strcat(path_buffer, "/");
        }
    }
    strcat(path_buffer, filename);
}

