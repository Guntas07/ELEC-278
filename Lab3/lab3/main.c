#include <stdio.h>
#include <stdlib.h> // Include this header for the free function
#include "stack.h"
#include <string.h>

// Define the enum for categories of video game actions
typedef enum {
    PUNCH,
    KICK,
    WILD,
    DOUBLE,
    TRIPLE,
    HIT
} Category;

// Define the structure for an action
typedef struct {
    Category name;
    int x;
} Action;    



// Function to calculate the total points based on actions
int calPoints(Action actions[], int actionSize) {
    struct Stack* stack = createStack();
    
    for (int i = 0; i < actionSize; i++) {
        if (actions[i].name == PUNCH) {
            push(stack, 1);
        } else if (actions[i].name == KICK) {
            push(stack, 2);
        } else if (actions[i].name == WILD) {
            push(stack, actions[i].x);
        } else if (actions[i].name == DOUBLE && !isEmpty(stack)) {
            int last = pop(stack);
            push(stack, 2 * last);
        } else if (actions[i].name == TRIPLE && !isEmpty(stack)) {
            int last = pop(stack);
            push(stack, 3 * last);
        } else if (actions[i].name == HIT) {
            for (int j = 0; j < actions[i].x && !isEmpty(stack); j++) {
                pop(stack);
            }
        }
    }

    // Calculate total points by summing all elements in the stack
    int totalScore = 0;
    while (!isEmpty(stack)) {
        totalScore += pop(stack);
    }

    free(stack); // Free the allocated memory for the stack
    return totalScore;
}

int main() {
    // Test cases to evaluate stack operations
    struct Stack* stack = createStack();

    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    printf("%d popped from stack\n", pop(stack));
    printf("Top element is %d\n", peek(stack));

    // Test case to evaluate game score
    Action actions[] = {
        {HIT, 3},
        {DOUBLE, 0},
        {PUNCH, 0},
        {KICK, 0},
        {DOUBLE, 0},
        {TRIPLE, 0},
        {WILD, 11},
        {KICK, 0},
        {KICK, 0},
        {HIT, 2},
        {DOUBLE, 0}
    };

    /*Action actions[] = {
        {HIT, 0},
        {PUNCH, 0},
        {DOUBLE, 0},
        {TRIPLE, 0},
        {KICK, 10},
        {DOUBLE, 0},
        {KICK, 0},
        {HIT, 3},
        {DOUBLE, 0}
    };*/
    int actionSize = 11;
    printf("Total Points: %d\n", calPoints(actions, actionSize));

    return 0;
}
