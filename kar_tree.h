#ifndef KAR_TREE_H_
#define KAR_TREE_H_

#define WRITE_BUFFER_SIZE 4096
// You can use this for filenames and for path + filename,
// Make sure the headers in the archive only include the filename*
// usually, there would be 4096 possible characters in a path
#define FILENAME_MAX 255

typedef struct arch_tree_node {
    char name[FILENAME_MAX];
    unsigned int size;
    unsigned char is_directory;
    struct arch_tree_node *next_file;
    struct arch_tree_node *dir_contents;
} arch_tree_node;

typedef struct kar_tree {
    arch_tree_node *root;
} kar_tree;

// In kar_tree.c
void free_tree(arch_tree_node *root);
void only_filename(char* filepath);
void valid_path(char path_buffer[], char *directory, char *filename);

// In archive.c
int create_archive(char *archive_name, int num_files, char *files_to_add[num_files]);

// In extract.c
int extract_archive(char *archive_name);

#endif /* KAR_TREE_H_ */
