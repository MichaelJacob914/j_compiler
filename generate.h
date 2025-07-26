#ifndef GENERATE_H_
#define GENERATE_H_
/*
 * token.h
 */

// the maximum length of a token is 250
#define MAX_TOKEN_LENGTH 250

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "./token.h"
#include "./Deque.h"

void print_prologue(FILE *output); 

void print_epilogue(FILE *output); 

void generate_assembly(FILE *output, token input, Deque *deque, Deque *while_deque, Deque *if_else); 
#endif  // GENERATE_H_
