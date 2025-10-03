#include "kar_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

arch_tree_node* build_tree(int num_files, char *files_to_add[num_files]);

arch_tree_node* create_tree_node(char *filepath){

    // Get the file's metadata
    struct stat st;
    if (stat(filepath, &st) == -1) {
        perror("Failed to get file metadata");
        return NULL;
    }

    // TODO create the node and allocate space on the heap for the node
    arch_tree_node *file_node = malloc(sizeof(*file_node));
    
    // If memory allocation is unsuccessful return null
    if (file_node == NULL) {
        perror("malloc");  // Look up the documentation for perror
        return NULL;
    }

    // Populate the node's attributes
    strcpy(file_node->name, filepath);
    file_node->size = st.st_size;
    file_node->is_directory = S_ISDIR(st.st_mode);
    file_node->next_file = NULL;
    file_node->dir_contents = NULL;

    // return node;
    return file_node;
}

void modify_node_name(arch_tree_node *node1, arch_tree_node *node2) {

    // Create buffer
    char n[WRITE_BUFFER_SIZE]; 

    // Copy the original name to the temporary buffer
    strncpy(n, node2->name, sizeof(n));

    // Perform the modification on the temporary buffer
    if (strrchr(n, '/')) {
        only_filename(n);
    }

    // Copy the modified name back to the node.name field
    strncpy(node1->name, n, sizeof(node1->name));
    node1->size = node2->size;
    node1->is_directory = node2->is_directory;
    node1->next_file = node2->next_file;
    node1->dir_contents = node2->dir_contents;
}

void store_files(arch_tree_node *root, FILE *archive) {

    // Create a copy of the filename
    arch_tree_node *n = create_tree_node(root->name);
    modify_node_name(n, root);

    // Write metadata to archive
    fwrite(n, sizeof(arch_tree_node), 1, archive);

    // Free the copy
    free(n);

    // Open file for reading
    FILE *file = fopen(root->name, "r");

    // Read and write the file's contents to the archive
    char buffer[WRITE_BUFFER_SIZE];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
    fwrite(buffer, 1, bytes_read, archive);

    // Close file
    fclose(file);

    // Traverse tree
    arch_tree_node *curr1 = root->dir_contents;
    arch_tree_node *curr2 = root->next_file;

    // Archive dir_contents first
    if (curr1 != NULL) {
        store_files(curr1, archive);
    }
    // Archive next_file after
    if (curr2 != NULL) {
        store_files(curr2, archive);
    }
}

void directory_update(arch_tree_node *node) {

    // Open and read the directory
    DIR *directory = opendir(node->name);
    struct dirent *file;
    if (directory == NULL) {
        perror("Error opening directory.\n");
    }

    // Count the number of files
    int n = 0, i = 0;
    while ((file = readdir(directory)) != NULL) {
        if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {
            n++;
        } 
    }
    rewinddir(directory);
    // Populate the array with filenames
    char *files_list[n];
    while ((file = readdir(directory)) != NULL) {
        if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {
            char *name = malloc(sizeof(char) * WRITE_BUFFER_SIZE); 
            valid_path(name, node->name, file->d_name);
            files_list[i] = name;
            i++;
        }
    }    
    // Attach recursive tree to the directory
    node->dir_contents = build_tree(n, files_list);
    
    // Close directory
    closedir(directory);
    
    // Free the files list
    for (int j = 0; j < i; j++) {
        free(files_list[j]);
    }
}

arch_tree_node* build_tree(int num_files, char *files_to_add[num_files]) {

    // Clear the memory address root points initially
    arch_tree_node *root = NULL;

    // Current points to the root node
    arch_tree_node *current = root;

    // Add files/directories to the tree
    for (int i = 0; i < num_files; i++) {
        // Create a new node
        arch_tree_node *new_node = create_tree_node(files_to_add[i]);

        // If the node is a directory, update its contents
        if (new_node->is_directory) {
            directory_update(new_node);
        }

        if (root == NULL) {
            // If it's the first node, set it as the root
            root = new_node;
            current = root;
        } else {
            // If it's not the first node, add it to the list
            current->next_file = new_node;
            current = current->next_file;
        }
    }
    return root;
}

int create_archive(char *archive_name, int num_files, char *files_to_add[num_files]){

    // Create the archive file
    FILE *archive = fopen(archive_name, "w");
    if (!archive) {
        perror("Failed to open archive file");
        return 1;
    }

    kar_tree root;
    
    root.root = NULL;
    root.root = build_tree(num_files, files_to_add);
       
    // Store files in archive
    store_files(root.root, archive);

    // Close the archive file
    fclose(archive);

    // Free the DS
    free_tree(root.root);
    return 0;
}



