#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Do not change: define a singly linked list node
struct Node {
    int val;
    struct Node* next;
};

// Function to create a new node
void push(struct Node** head_ref, int new_val) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->val = new_val;
    new_node->next = *head_ref;
    *head_ref = new_node;
}

// Function to print the singly linked list
void printList(struct Node* head) {
    struct Node* current = head;  // Initialize current node as head
    while (current != NULL) {
        printf("%d -> ", current->val);
        current = current->next;
    }
    printf("NULL\n");
}

// Function to delete duplicates from the linked list
struct Node* deleteDuplicates(struct Node* head) {
    struct Node* current = head;
    while (current != NULL && current->next != NULL) {
        if (current->val == current->next->val) {
            struct Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return head;
}

// Function to reverse a singly linked list
struct Node* reverseList(struct Node* head) {
    struct Node* prev = NULL;
    struct Node* current = head;
    struct Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;  // Return the new head of the reversed list
}

// Function to detect if there is a cycle in the linked list using Floyd's Cycle-Finding Algorithm
bool hasCycle(struct Node* head) {
    struct Node* slow = head;
    struct Node* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;  // Cycle detected
        }
    }

    return false;  // No cycle detected
}

// Function to check if the linked list is a palindrome
bool isPalindrome(struct Node* head) {
    if (head == NULL || head->next == NULL) {
        return true;  // Base case: empty or single-node list is a palindrome
    }

    struct Node* slow = head;
    struct Node* fast = head;

    // Find the midpoint using slow and fast pointers
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Reverse the second half of the list
    struct Node* secondHalf = reverseList(slow);
    struct Node* firstHalf = head;

    // Compare the first and second halves
    struct Node* tempSecondHalf = secondHalf;
    while (tempSecondHalf != NULL) {
        if (firstHalf->val != tempSecondHalf->val) {
            reverseList(secondHalf);  // Restore the list
            return false;  // Not a palindrome
        }
        firstHalf = firstHalf->next;
        tempSecondHalf = tempSecondHalf->next;
    }

    // Restore the list to its original form
    reverseList(secondHalf);
    return true;  // The list is a palindrome
}

// Test cases
int main() {
    printf("Test case 1.\n");

    // Create a singly linked list: 5 -> 10 -> 15 -> 20
    struct Node* head = NULL;

    // Use the push function to add elements to the list
    int arr[4] = {20, 15, 10, 5};
    for (int i = 0; i < 4; i++) {
        push(&head, arr[i]);
    }

    // Print the original list
    printf("Original list: ");
    printList(head);

    // Check if the list is a palindrome
    if (isPalindrome(head)) {
        printf("The list is a palindrome.\n");
    } else {
        printf("The list is not a palindrome.\n");
    }

    // Reverse the list
    head = reverseList(head);
    printf("Reversed list: ");
    printList(head);

    // Delete duplicates
    head = deleteDuplicates(head);
    printf("Duplicates deleted: ");
    printList(head);

    // Check for a cycle (expecting no cycle)
    if (hasCycle(head)) {
        printf("The list has a cycle.\n");
    } else {
        printf("The list does not have a cycle.\n");
    }

    // Creating a cycle for testing
    head->next->next->next = head->next;  // Introduce a cycle

    // Check for a cycle again
    if (hasCycle(head)) {
        printf("The list has a cycle.\n");
    } else {
        printf("The list does not have a cycle.\n");
    }

    return 0;
}
