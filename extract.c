#include "kar_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void extract_files(FILE *archive, char path[]);

arch_tree_node *read_node(FILE *archive){
    arch_tree_node *node = malloc(sizeof(arch_tree_node));
    if (!node) {
        perror("Failed to allocate memory for node");
        return NULL;
    }
    fread(node, sizeof(arch_tree_node), 1, archive);
    return node;
}

void extract_directory(FILE *archive, char path[], arch_tree_node *dir_node) {

    // Make a copy of the path
    char *dir_path = malloc(sizeof(char) * WRITE_BUFFER_SIZE);
    valid_path(dir_path, path, dir_node->name);
    
    // Make a directory
    mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    // Build the tree in the directory
    if (dir_node->dir_contents != NULL) {
        extract_files(archive, dir_path);
    }

    // Free the copy
    free(dir_path);
}

void extract_file_contents(FILE *archive, char path[], arch_tree_node *file_node) {

    // Make a copy of the path
    char *file_path = malloc(sizeof(char) * WRITE_BUFFER_SIZE);
    valid_path(file_path, path, file_node->name);

    // Create the file
    FILE* file = fopen(file_path, "w");
    if (!file) {
        perror("Failed to create file");
        free(file_path);
        return;
    }

    // Read and write the file content from the archive to the new file
    char buffer[file_node->size];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), archive);
    fwrite(buffer, 1, bytes_read, file);

    // Close the file
    fclose(file);

    // Free the copy
    free(file_path);
}

void extract_files(FILE *archive, char path[]) {

    // Declare new node
    arch_tree_node *new_node;

    // Loop until the end of the file
    while ((new_node = read_node(archive))->name[0] != '\0') {
        if (new_node->is_directory) {
            extract_directory(archive, path, new_node);
        } else {
            extract_file_contents(archive, path, new_node);
        }
        // If the next file in the linked list is NULL, just return
        if (new_node->next_file == NULL) {
            free(new_node);
            return;
        }
        free(new_node);
    }
    free(new_node);
}

int extract_archive(char *archive_name){
    FILE* archive = fopen(archive_name, "r");
    if (!archive) {
        perror("Failed to open archive.\n");
        return 1;
    }

    // Read the data structures from the archive
    // such that you can re-create the archived files

    // Empty path is passed as root filename does not contain a path
    char p[1] = "";
    extract_files(archive, p);

    // Close the archive file
    fclose(archive);
    return 0;
}
