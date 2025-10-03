#include "kar_tree.h"
#include <stdio.h>
#include <string.h>

void print_help() {
    printf("Usage: kar [COMMAND] [ARCHIVE]\n");
    printf("Creates or extracts files from the kar-formatted ARCHIVE.\n");
    printf("\n");
    printf("COMMAND may be one of the following:\n");
    printf("    create [ARCHIVE] [FILES/DIRS]\n");
    printf("    extract [ARCHIVE]\n");
    printf("    --help\n");
    printf("\n");
    printf("create:\n");
    printf("    Creates an archive with the name specified in [ARCHIVE] consisting of the listed files and dirs. Paths to the files are not preserved, but the structure inside the archive will be.\n");
    printf("\n");
    printf("extract:\n");
    printf("    Extracts the files from the [ARCHIVE] file into the current directory.\n");
    printf("\n");
    printf("--help:\n");
    printf("    Prints this message and exits.\n");
}

int main(int argc, char *argv[]) {

    // Usage string is provided
    // main should only need to check arguments and
    // call the appropriate functions

    if (argc >= 4 && strcmp(argv[1], "create") == 0) {
        char *files_to_add[argc - 3];
        for (int i = 0; i < argc - 3; i++) {
            files_to_add[i] = argv[i + 3];
        }
        return create_archive(argv[2], argc - 3, files_to_add);
    } else if (argc == 3 && strcmp(argv[1], "extract") == 0) {
        return extract_archive(argv[2]);
    } else if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
    } else {
        return 1;
    }
    return 0;
}

