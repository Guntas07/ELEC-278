#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Function to create a stack
struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); // Allocate memory
    stack->top = -1; // Initialize top to -1 (empty stack)
    return stack;
}

// Function to check if the stack is empty
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

// Function to check if the stack is full
int isFull(struct Stack* stack) {
    return stack->top == MAX - 1;
}

// Function to push an element onto the stack
void push(struct Stack* stack, int item) {
    if (isFull(stack)) {
        printf("Stack overflow, cannot push %d onto stack.\n", item);
        return;
    }
    stack->array[++(stack->top)] = item;
}

// Function to pop an element from the stack
int pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow, cannot pop from stack.\n");
        return -1; // Return -1 as an error indicator
    }
    return stack->array[(stack->top)--]; // Return the top element and decrement top
}

// Function to peek the top element of the stack
int peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty, cannot peek.\n");
        return -1; // Return -1 as an error indicator
    }
    return stack->array[stack->top]; // Return the top element
}
