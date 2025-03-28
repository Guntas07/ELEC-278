#include <stdio.h>
#include <stdlib.h>

// FACTORIAL
// Iterative function
int factorial_iterative(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) { // O(n): loop will go from 1 to n times
        result *= i;
    }
    return result;
}

// Recursive function
int factorial_recursive(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial_recursive(n - 1); // O(n): loop will run from n-1 to 0 times same as n times

}

// SUM ARRAY
// Iterative function
int sumArray_iterative(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) { // O(n): loop runs n times
        sum += arr[i]; 
    }
    return sum; 
}
// Recursive function
int sumArray_recursive(int arr[], int n) {
    if (n == 0) { 
        return 0;
    }
    return arr[n - 1] + sumArray_recursive(arr, n - 1); // O(n): n recursive calls
}

// MERGE TWO LINKED LISTS
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Helper function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* mergeLists_iterative(Node* head1, Node* head2) {
    Node dummy;
    Node* tail = &dummy;
    dummy.next = NULL; 

    while (head1 != NULL && head2 != NULL) {
        if (head1->data <= head2->data) {
            tail->next = head1; 
            head1 = head1->next; 
        } else {
            tail->next = head2; 
            head2 = head2->next;
        }
        tail = tail->next;

    if (head1 != NULL) {
        tail->next = head1;
    } else {
        tail->next = head2;
    }
    }
    
    return dummy.next;
}

Node* mergeLists_recursive(Node* head1, Node* head2) {
     if (head1 == NULL) {
        return head2;
    }
    if (head2 == NULL) {
        return head1;
    }

    if (head1->data <= head2->data) {
        head1->next = mergeLists_recursive(head1->next, head2);
        return head1;
    } else {
        head2->next = mergeLists_recursive(head1, head2->next);
        return head2;
    }
}

// helper function to free a linked list
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// TASK SCHEDULER
typedef struct Task {
    int id;
    int priority;
    struct Task* next;
} Task;

typedef struct Schedule {
    Task* head;
} Schedule;

// Helper function to create a new task
Task* createTask(int id, int priority) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->id = id;
    newTask->priority = priority;
    newTask->next = NULL;
    return newTask;
}

void newTask(Schedule* queue, int task_id, int task_priority) {
    // to do: complete
    Task* new_task = createTask(task_id, task_priority);
    
    // If the list is empty or the new task has higher priority than the head
    if (queue->head == NULL || queue->head->priority < task_priority) {
        new_task->next = queue->head;
        queue->head = new_task;
    } else {
        // Traverse the list to find the correct position for the new task
        Task* current = queue->head;
        while (current->next != NULL && current->next->priority >= task_priority) {
            current = current->next;
        }
        new_task->next = current->next;
        current->next = new_task;
    }
}

Task* execute(Schedule* queue) {
    // to do: complete
    if (queue->head == NULL) {
        printf("No tasks to execute.\n");
        return NULL;
    }
    
    Task* to_execute = queue->head;
    queue->head = queue->head->next;
    
    return to_execute;
}

void printQueue(Task* task) {
    if (task == NULL) {
        printf("\n");
        return;
    }
    printf("Task %d with Priority %d -> ", task->id, task->priority);
    printQueue(task->next);
    return;
}


int main() {
    ///////////////////////////////////////////////
    printf("Testing Factorial\n");
    int number = 5;
    printf("Iterative: %d! = %d\n", number, factorial_iterative(number));
    printf("Recursive: %d! = %d\n", number, factorial_recursive(number));

    ///////////////////////////////////////////////
    printf("\nTesting Sum Array\n");
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Iterative: sum = %d\n", sumArray_iterative(arr, n));
    printf("Recursive: sum = %d\n", sumArray_recursive(arr, n));

    ///////////////////////////////////////////////
    printf("\nTesting Merge Lists\n");
    Node* head1 = createNode(1);
    head1->next = createNode(3);
    // head1->next->next = createNode(5);

    Node* head2 = createNode(2);
    head2->next = createNode(4);
    head2->next->next = createNode(6);

    Node* mergedList_iter = mergeLists_iterative(head1, head2);
    printf("Merged List (Iterative): ");
    for (Node* temp = mergedList_iter; temp; temp = temp->next) {
        printf("%d ", temp->data);
    }
    printf("\n");

    // Free the merged list
    freeList(mergedList_iter);
    
    // Reset the original lists (for demonstration purposes)
    head1 = createNode(1);
    head1->next = createNode(3);
    // head1->next->next = createNode(5);

    head2 = createNode(2);
    head2->next = createNode(4);
    head2->next->next = createNode(6);

    Node* mergedList_recur = mergeLists_recursive(head1, head2);
    printf("Merged List (Recursive): ");
    for (Node* temp = mergedList_recur; temp; temp = temp->next) {
        printf("%d ", temp->data);
    }
    printf("\n");

    // Free the merged list
    freeList(mergedList_iter);

    ///////////////////////////////////////////
    Schedule queue = {NULL};

    newTask(&queue, 1, 3);
    newTask(&queue, 2, 4);
    newTask(&queue, 3, 2);
    newTask(&queue, 4, 3);
    newTask(&queue, 5, 5);
    newTask(&queue, 6, 2);
    printQueue(queue.head);
    
    Task* executed_task;
    while ((executed_task = execute(&queue)) != NULL) {
        printf("Executed Task: ID = %d, Priority = %d\n", executed_task->id, executed_task->priority);
        free(executed_task);
    }

    return 0;
}

