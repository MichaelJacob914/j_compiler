
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include "./token.h"
#include "./generate.h"
#include "./Deque.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.j>\n", argv[0]);
        return 1;
    }

    printf("Attempting to open file: %s\n", argv[1]);

    // Open the input file
    FILE *j_file = fopen(argv[1], "r");
    if (!j_file) {
        perror("Error opening input file");
        return 1;
    }

    // Prepare the output file name
    char outfile_name[250];
    strncpy(outfile_name, argv[1], sizeof(outfile_name) - 1);
    outfile_name[sizeof(outfile_name) - 1] = '\0';

    char *dot = strrchr(outfile_name, '.');
    if (dot) {
        strcpy(dot, ".s");
    } else {
        strncat(outfile_name, ".s", sizeof(outfile_name) - strlen(outfile_name) - 1);
    }

        // Remove the extension for the base name
    char base_name[250];
    strncpy(base_name, argv[1], sizeof(base_name) - 1);
    base_name[sizeof(base_name) - 1] = '\0';

    const char *prefix = "j_tests/";
    if (strncmp(base_name, prefix, strlen(prefix)) == 0) {
        memmove(base_name, base_name + strlen(prefix), strlen(base_name) - strlen(prefix) + 1);
    }

    // Remove the file extension from base_name
    dot = strrchr(base_name, '.');
    if (dot) {
        *dot = '\0'; // Truncate at the dot
    }

    FILE *outfile = fopen(outfile_name, "w");
    if (!outfile) {
        perror("Error opening output file");
        fclose(j_file);
        return 1;
    }


    Deque *deque = Deque_Allocate(); 
    Deque *while_dq = Deque_Allocate(); 
    Deque *if_else = Deque_Allocate(); 
    printf("Base name: %s\n", base_name);
    printf("Output will be written to: %s\n", outfile_name);
    //fprintf(outfile, "\t.text\n"); 
    //fprintf(outfile, "\t.p2align	2\n"); 
    //fprintf(outfile, "\t.globl	%s\n", base_name); 
    //Process tokens and generate assembly
    token current_token;
    while (next_token(j_file, &current_token)) {
        //print_tok(outfile, current_token); 
        generate_assembly(outfile, current_token, deque, while_dq, if_else); 
    }

    // Close files
    fclose(j_file);
    fclose(outfile);
    Deque_Free(deque); 
    Deque_Free(while_dq); 
    Deque_Free(if_else); 
    return 0;
}