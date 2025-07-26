#include <stdio.h>
#include <stdlib.h>
#include "./generate.h"
#include "./token.h"
#include "./Deque.h"

token previous_token; 
int if_counter = 0; 
int last_if_else = 0; 
int while_counter = 0; 

void print_prologue(FILE *output) {
    fprintf(output, "addi sp, sp, -16\n");
    fprintf(output, "sw ra, 12(sp)\n");
    fprintf(output, "sw fp, 8(sp)\n");
    fprintf(output, "addi fp, sp, 16\n");
}

void print_epilogue(FILE *output) {
    fprintf(output, "lw a0, 0(sp)\n");
    fprintf(output, "mv sp, fp\n");          
    fprintf(output, "lw ra, -4(fp)\n");      
    fprintf(output, "lw fp, -8(fp)\n");      
    fprintf(output, "ret\n");               
}

void print_epilogue_alt(FILE *output) {
    fprintf(output, "lw ra, 12(sp)\n");
    fprintf(output, "lw fp, 8(sp)\n");
    fprintf(output, "addi sp, sp, 16\n");
    fprintf(output, "ret\n");
}


/*TO BE IMPLEMENTED: 
	       IF, ELSE, ENDIF,  */

void generate_assembly(FILE *output, token input, Deque *dq, Deque *while_dq, Deque *if_else) {
    if(input.type == LITERAL) {
        int32_t literal_value = input.literal_value; 
        if (input.is_hexadecimal) {
            fprintf(output, "addi sp, sp, -4\n");
            fprintf(output, "li t0, 0x%x\n", literal_value); 
            fprintf(output, "sw t0, 0(sp)\n");       
        } 
        else {
            fprintf(output, "addi sp, sp, -4\n");
            fprintf(output, "li t0, %d\n", literal_value); 
            fprintf(output, "sw t0, 0(sp)\n");       
        }
    } else if(input.type == PLUS) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "add t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");       
    } else if(input.type == MINUS) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "sub t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");                 
    } else if(input.type == MUL) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "mul t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");                
    } else if(input.type == DIV) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "div t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");                   
    } else if(input.type == MOD) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "rem t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");              
    } else if(input.type == AND) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "and t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");          
    } else if(input.type == OR) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");    
        fprintf(output, "or t0, t0, t1\n");  
        fprintf(output, "sw t0, 0(sp)\n");         
    } else if(input.type == NOT) { //CORRECT
        fprintf(output, "lw t0, 0(sp)\n");       
        fprintf(output, "not t0, t0 \n");  
        fprintf(output, "sw t0, 0(sp)\n");        
    } else if(input.type == DUP) { 
        fprintf(output, "lw t0, 0(sp)\n");      
        fprintf(output, "addi sp, sp, -4\n");     
        fprintf(output, "sw t0, 0(sp)\n");           
    } else if(input.type == DROP) {
        fprintf(output, "addi sp, sp, 4\n");      
    } else if(input.type == SWAP) {
        fprintf(output, "lw t0, 0(sp)\n");      
        fprintf(output, "lw t1, 4(sp)\n");    
        fprintf(output, "sw t0, 4(sp)\n");    
        fprintf(output, "sw t1, 0(sp)\n");      
    } else if(input.type == ROT) {
        fprintf(output, "lw t0, 0(sp)\n");      
        fprintf(output, "lw t1, 4(sp)\n");     
        fprintf(output, "lw t2, 8(sp)\n");     
        fprintf(output, "sw t2, 0(sp)\n");       
        fprintf(output, "sw t0, 4(sp)\n");     
        fprintf(output, "sw t1, 8(sp)\n");  
    } else if(input.type == LT) {
        fprintf(output, "lw t0, 0(sp)\n");
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");     
        fprintf(output, "slt t2, t0, t1\n"); 
        fprintf(output, "sw t2, 0(sp)\n");          
    } else if(input.type == EQ) {
        fprintf(output, "lw t0, 0(sp)\n");   
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");      
        fprintf(output, "sub t1, t1, t0 \n"); 
        fprintf(output, "seqz t1, t1 \n"); 
        fprintf(output, "sw t1, 0(sp)\n");          
    } else if(input.type == LE) {
        fprintf(output, "lw t0, 0(sp)\n"); 
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");         
        fprintf(output, "sub t2, t1, t0 \n"); 
        fprintf(output, "seqz t3, t1 \n"); 
        fprintf(output, "slt t2, t0, t1\n"); 
        fprintf(output, "or t2, t2, t3\n"); 
        fprintf(output, "sw t2, 0(sp)\n");     
    } else if(input.type == GT) {
        fprintf(output, "lw t0, 0(sp)\n");  
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");        
        fprintf(output, "slt t2, t1, t0\n"); 
        fprintf(output, "sw t2, 0(sp)\n"); 
    } else if(input.type == GE) {
         fprintf(output, "lw t0, 0(sp)\n"); 
        fprintf(output, "addi sp, sp, 4\n");  
        fprintf(output, "lw t1, 0(sp)\n");         
        fprintf(output, "sub t2, t1, t0 \n"); 
        fprintf(output, "seqz t3, t1 \n"); 
        fprintf(output, "slt t2, t1, t0\n"); 
        fprintf(output, "or t2, t2, t3\n"); 
        fprintf(output, "sw t2, 0(sp)\n");        
    } else if(input.type == SET_ARG) { //CORRECT
        int number = input.arg_no - 1; 
        fprintf(output, "lw a%d, 0(sp)\n", number);     
        fprintf(output, "addi sp, sp, 4\n");        
    } else if(input.type == GET_ARG) { //CORRECT
        int number = input.arg_no - 1; 
        fprintf(output, "addi sp, sp, -4\n");
        fprintf(output, "sw a%d, 0(sp)\n", number);     
    } else if(input.type == IDENT) { 
        if(previous_token.type == DEFUN) { 
            fprintf(output, ".globl %s\n", input.str); 
            fprintf(output, "%s:\n", input.str);
            print_prologue(output); 
        } else {
            fprintf(output, "jal %s\n", input.str);
            fprintf(output, "addi sp, sp, -4\n"); 
            fprintf(output, "sw a0, 0(sp)\n"); 
        }
    } else if(input.type == IF) {
        Deque_Push_Back(dq, if_counter); 
        fprintf(output, "lw t0, 0(sp)\n"); 
        fprintf(output, "addi sp, sp, 4\n"); 
        fprintf(output, "beq x0, t0, ELSE%d\n", if_counter); 
        Deque_Push_Back(if_else, 0); 
        if_counter++; 
    } else if(input.type == ELSE) {
        int payload = 0; 
        Deque_Pop_Back(if_else, &payload); 
        Deque_Push_Back(if_else, 1); 
        Deque_Peek_Back(dq, &payload); 
        fprintf(output, "jal ENDIF%d\n", payload); 
        fprintf(output, "ELSE%d:\n", payload); 
    } else if(input.type == ENDIF) {
        int payload = 0; 
        int seen = 0; 
        Deque_Pop_Back(if_else, &seen); 
        Deque_Pop_Back(dq,&payload); 
        if(seen == 0) {
            fprintf(output, "ELSE%d:\n", payload); 
        }
        fprintf(output, "ENDIF%d:\n", payload); 
    } else if(input.type == WHILE) {
        while_counter += 1; 
        Deque_Push_Back(while_dq, while_counter); 
        fprintf(output, "WHILE%d:\n", while_counter); 
        fprintf(output, "lw t0, 0(sp)\n"); 
        fprintf(output, "addi sp, sp, 4\n"); 
        fprintf(output, "beq x0, t0, ENDWHILE%d\n", while_counter); 
    } else if(input.type == ENDWHILE) {
        int payload = 0;
        Deque_Pop_Back(while_dq, &payload);
        fprintf(output, "jal WHILE%d\n", payload); 
        fprintf(output, "ENDWHILE%d:\n", payload); 
    } else if(input.type == RETURN) { //CORRECT
        print_epilogue(output); 
    } else if(input.type == BAD_TOKEN) {
        fprintf(output, "\n"); 
    }
    previous_token = input; 
}
