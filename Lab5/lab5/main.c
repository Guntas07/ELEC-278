#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definition of the tree node
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Helper function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


// Definition of the queue node
typedef struct QueueNode {
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

// Definition of the queue
typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Helper functions to manage the queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, Node* treeNode) {
    QueueNode* newQueueNode = (QueueNode*)malloc(sizeof(QueueNode));
    newQueueNode->treeNode = treeNode;
    newQueueNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newQueueNode;
        return;
    }
    
    q->rear->next = newQueueNode;
    q->rear = newQueueNode;
}

Node* dequeue(Queue* q) {
    if (q->front == NULL) {
        return NULL;  // Queue is empty
    }
    
    QueueNode* temp = q->front;
    Node* treeNode = temp->treeNode;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return treeNode;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// HEIGHT OF TREE
// Function to calculate the height of the tree
int getHeight(Node* root) {
    // to do: complete
    if (root == NULL) {
        return -1;  // Base case: height of an empty tree is -1
    }
    int leftHeight = getHeight(root->left);   // Height of the left subtree
    int rightHeight = getHeight(root->right); // Height of the right subtree
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; // Take max height and add 1
}

// Helper function to calculate height and check if balanced
int checkBalance(Node* root, bool* isBalanced) {
    if (root == NULL) {
        return -1; // Height of an empty tree is -1
    }

    // Get heights of left and right subtrees
    int leftHeight = checkBalance(root->left, isBalanced);
    int rightHeight = checkBalance(root->right, isBalanced);

    // If the height difference is greater than 1, mark as unbalanced
    if (abs(leftHeight - rightHeight) > 1) {
        *isBalanced = false;
    }

    // Return the height of the current node
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// BALANCED TREE?

// Function to check if the tree is height balanced
bool isHeightBalanced(Node* root) {
    bool isBalanced = true;
    checkBalance(root, &isBalanced);
    return isBalanced;
}

// ZIG-ZAG LEVEL-ORDER OF A TREE

// Function to print zigzag level order traversal
void printZigzagLevelOrder(Node* root) {
    // to do: complete
     if (root == NULL) {
        return;
    }

    Queue* q = createQueue();
    enqueue(q, root);

    bool leftToRight = true;

    while (!isQueueEmpty(q)) {
        int levelSize = 0;
        Queue* tempQueue = createQueue();
        
        // Count the number of nodes at the current level
        QueueNode* current = q->front;
        while (current != NULL) {
            levelSize++;
            current = current->next;
        }

        // Process all nodes at the current level
        int levelValues[levelSize];
        for (int i = 0; i < levelSize; ++i) {
            Node* currentNode = dequeue(q);
            
            // Store the value in the array in the correct order
            if (leftToRight) {
                levelValues[i] = currentNode->data;
            } else {
                levelValues[levelSize - 1 - i] = currentNode->data;
            }

            // Enqueue children of the current node
            if (currentNode->left) {
                enqueue(tempQueue, currentNode->left);
            }
            if (currentNode->right) {
                enqueue(tempQueue, currentNode->right);
            }
        }

        // Print the current level's values
        for (int i = 0; i < levelSize; ++i) {
            printf("%d ", levelValues[i]);
        }
        printf("\n");

        // Move to the next level
        q = tempQueue;

        // Toggle the direction for the next level
        leftToRight = !leftToRight;
    }
}

// TREASURE LOOT
// Function to calculate the maximum loot in the pyramid
int rob(Node* root) {
    if (root == NULL) {
        return 0;
    }

    // Initialize two sums: one for even levels and one for odd levels
    int evenSum = 0;
    int oddSum = 0;
    bool isEvenLevel = true;

    // Initialize the queue for level-order traversal
    Queue* q = createQueue();
    enqueue(q, root);

    // Perform BFS to calculate the sum of even and odd levels
    while (!isQueueEmpty(q)) {
        int levelSize = 0;
        Queue* tempQueue = createQueue();
        int currentLevelSum = 0;

        // Count the number of nodes at the current level
        QueueNode* current = q->front;
        while (current != NULL) {
            levelSize++;
            current = current->next;
        }

        // Process all nodes at the current level
        for (int i = 0; i < levelSize; ++i) {
            Node* currentNode = dequeue(q);
            currentLevelSum += currentNode->data;

            // Enqueue children of the current node
            if (currentNode->left) {
                enqueue(tempQueue, currentNode->left);
            }
            if (currentNode->right) {
                enqueue(tempQueue, currentNode->right);
            }
        }

        // Add the current level's sum to the appropriate total
        if (isEvenLevel) {
            evenSum += currentLevelSum;
        } else {
            oddSum += currentLevelSum;
        }

        // Toggle the level type
        isEvenLevel = !isEvenLevel;

        // Move to the next level
        q = tempQueue;
    }

    // Return the maximum sum from even and odd levels
    return evenSum > oddSum ? evenSum : oddSum;
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->left->left->left = createNode(6);

    printf("Height of the tree: %d\n", getHeight(root));

    if (isHeightBalanced(root)) {
        printf("The tree is height balanced.\n");
    } else {
        printf("The tree is not height balanced.\n");
    }

    freeTree(root);

    root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);

    printf("Height of the tree: %d\n", getHeight(root));

    if (isHeightBalanced(root)) {
        printf("The tree is height balanced.\n");
    } else {
        printf("The tree is not height balanced.\n");
    }

    printf("Zigzag Level Order Traversal: ");
    printZigzagLevelOrder(root);

    printf("\nMaximum amount of items the looter can loot: %d\n", rob(root));

    freeTree(root);

    return 0;
}
