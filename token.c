#include "./token.h"

// the maximum length of a token is 250
#define MAX_TOKEN_LENGTH 250

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

// WARNING
// WARNING
// WARNING
// Except for possibly line_number, you should not use any global variables
// for token parsing for this assignment. It is likely to cause an issue with the
// autograder if you do. (Good code generally minimizes use of globals anyways).

// line_number : You can use this to keep track of the current line number you
// are at in the J source file which can be useful in reporting syntax errors.
// Be sure to include a line that says 'int line_number;' in token.c outside
// of any functions if you would like to make use of this.
int line_number = 0;

void classify_token(token *output) {
    if (strcmp(output->str, "defun") == 0) {
        output->type = DEFUN;
    } else if (strcmp(output->str, "return") == 0) {
        output->type = RETURN;
    } else if (strcmp(output->str, "+") == 0) {
        output->type = PLUS;
    } else if (strcmp(output->str, "-") == 0) {
        output->type = MINUS;
    } else if (strcmp(output->str, "*") == 0) {
        output->type = MUL;
    } else if (strcmp(output->str, "/") == 0) {
        output->type = DIV;
    } else if (strcmp(output->str, "%") == 0) {
        output->type = MOD;
    } else if (strcmp(output->str, "and") == 0) {
        output->type = AND;
    } else if (strcmp(output->str, "or") == 0) {
        output->type = OR;
    } else if (strcmp(output->str, "not") == 0) {
        output->type = NOT;
    } else if (strcmp(output->str, "lt") == 0) {
        output->type = LT;
    } else if (strcmp(output->str, "le") == 0) {
        output->type = LE;
    } else if (strcmp(output->str, "eq") == 0) {
        output->type = EQ;
    } else if (strcmp(output->str, "ge") == 0) {
        output->type = GE;
    } else if (strcmp(output->str, "gt") == 0) {
        output->type = GT;
    } else if (strcmp(output->str, "if") == 0) {
        output->type = IF;
    } else if (strcmp(output->str, "else") == 0) {
        output->type = ELSE;
    } else if (strcmp(output->str, "endif") == 0) {
        output->type = ENDIF;
    } else if (strcmp(output->str, "while") == 0) {
        output->type = WHILE;
    } else if (strcmp(output->str, "endwhile") == 0) {
        output->type = ENDWHILE;
    } else if (strcmp(output->str, "drop") == 0) {
        output->type = DROP;
    } else if (strcmp(output->str, "dup") == 0) {
        output->type = DUP;
    } else if (strcmp(output->str, "swap") == 0) {
        output->type = SWAP;
    } else if (strcmp(output->str, "rot") == 0) {
        output->type = ROT;
    } else if (strncmp(output->str, "get_arg", 7) == 0 && isdigit(output->str[7])) {
        output->type = GET_ARG;
        output->arg_no = atoi(output->str + 7);
    } else if (strncmp(output->str, "set_arg", 7) == 0 && isdigit(output->str[7])) {
        output->type = SET_ARG;
        output->arg_no = atoi(output->str + 7);
    } else if (strncmp(output->str, "0x", 2) == 0 && isxdigit(output->str[2])) {
        output->type = LITERAL;
        output->literal_value = (int)strtol(output->str, NULL, 16);
        output->is_hexadecimal = true; 
    } else if (isdigit(output->str[0])) {
        output->type = LITERAL;
        output->literal_value = atoi(output->str);
        output->is_hexadecimal = false; 
        output->sign = false; 
    } else if((output->str[0] == '-' && isdigit(output->str[1]))) {
        output->type = LITERAL;
        output->literal_value = atoi(output->str);
        output->is_hexadecimal = false; 
        output->sign = true; 
    }
    
    else {
        output->type = IDENT;  
    }
}

// gets the next J token from the specified input file.
// 
// Arguments:
// - j_file: the file stream to read from. Should be a valid file and have read permissions.
// - output: the output parameter to "return" the read token. The type must always be set.
//           If the token is of type IDENT, the name of the identifier should
//           stored in the field `str` as a null-terminated string.
//           If the token is of type LITERAL, then the field literal_value
//           should contain the equivalent integer value of the literal token.
//           If the token is of type GET_ARG or SET_ARG, then the arg number 'n' should be
//           stored in the field 'arg_no'. For the remaining cases, the
//           field can be set to whatever is desired.
// 
// Return Value:
// - true if a token was read successfully
// - false if a token could not be read due to either hitting the end of file
//   or some unrecoverable error
bool next_token(FILE *j_file, token *output) {
    char string[MAX_TOKEN_LENGTH + 1] = {0};  
    int index = 0;
    int ch;

    while ((ch = fgetc(j_file)) != EOF) {

        if (isspace(ch)) {
            continue;  
        } else if (ch == ';') {
            while ((ch = fgetc(j_file)) != EOF && ch != '\n') {
            }
            if (ch == '\n') {
                continue; 
            }
        } else {
            break;  
        }
    }

    if (ch == EOF) {
        return false;  
    }

    do {
        if (index >= MAX_TOKEN_LENGTH) {
            output->type = BAD_TOKEN; 
            return true;
        }

        string[index++] = ch;
        if (ch == '\n') {
            line_number++; 
        }
    } while ((ch = fgetc(j_file)) != EOF && !isspace(ch) && ch != ';');  

    string[index] = '\0';


    if (ch == ' ' || ch == '\n' || ch == ';') {
        ungetc(ch, j_file);
    }

    strncpy(output->str, string, MAX_TOKEN_LENGTH);
    output->str[MAX_TOKEN_LENGTH] = '\0';  

    classify_token(output);

    return true;
}

// Extra function which you may wish to define and use, but are not required to use

// Prints a token to the specified file
//
// Arguments:
// - f: a file stream that should be valid and have write permissions.
//      The token is printed to this file in whatever format you like.
//      To print to the terminal, pass in 'std_out'.
// - to_print: the token to be printed to the file
void print_token (FILE* f, token output){
    switch (output.type) {
        case DEFUN:
            fprintf(f, "DEFUN\n");
            break;
        case RETURN:
            fprintf(f, "RETURN\n");
            break;
        case PLUS:
            fprintf(f, "PLUS\n");
            break;
        case MINUS:
            fprintf(f, "MINUS\n");
            break;
        case MUL:
            fprintf(f, "MUL\n");
            break;
        case DIV:
            fprintf(f, "DIV\n");
            break;
        case MOD:
            fprintf(f, "MOD\n");
            break;
        case AND:
            fprintf(f, "AND\n");
            break;
        case OR:
            fprintf(f, "OR\n");
            break;
        case NOT:
            fprintf(f, "NOT\n");
            break;
        case LT:
            fprintf(f, "LT\n");
            break;
        case LE:
            fprintf(f, "LE\n");
            break;
        case EQ:
            fprintf(f, "EQ\n");
            break;
        case GE:
            fprintf(f, "GE\n");
            break;
        case GT:
            fprintf(f, "GT\n");
            break;
        case IF:
            fprintf(f, "IF\n");
            break;
        case ELSE:
            fprintf(f, "ELSE\n");
            break;
        case ENDIF:
            fprintf(f, "ENDIF\n");
            break;
        case WHILE:
            fprintf(f, "WHILE\n");
            break;
        case ENDWHILE:
            fprintf(f, "ENDWHILE\n");
            break;
        case DROP:
            fprintf(f, "DROP\n");
            break;
        case DUP:
            fprintf(f, "DUP\n");
            break;
        case SWAP:
            fprintf(f, "SWAP\n");
            break;
        case ROT:
            fprintf(f, "ROT\n");
            break;
        case GET_ARG:
            fprintf(f, "GET_ARG %d\n", output.arg_no);
            break;
        case SET_ARG:
            fprintf(f, "SET_ARG %d\n", output.arg_no);
            break;
        case LITERAL:
            fprintf(f, "LITERAL %d\n", output.literal_value);
            break;
        case IDENT:
            fprintf(f, "IDENT %s\n", output.str);
            break;
        case BAD_TOKEN:
            fprintf(f, "BAD_TOKEN\n");
            break;
        default:
            fprintf(f, "Unknown Token\n");
            break;
    }
}
