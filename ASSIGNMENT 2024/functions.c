#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define MAX_USERS 10000
#define MAX_MESSAGES 50
#define MAX_POSTS 10000
#define FEED_LIMIT 20

// Global variables
User* users[MAX_USERS]; // Array of user pointers
int user_count = 0; //Number of users
int next_user_id = 1; //Each user has a unique ID, this is the next users ID
int next_message_id = 1; // Similar concept with messages
int next_post_id = 1; // Similar concept with post ID's
Post* posts[MAX_POSTS]; //Array of post pointers
int post_count = 0; //Number of total posts

//Initialize a queue data structure to be used for user posts
//Initialize queue
void init_queue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

// Function to enqueue a post
void enqueue(Queue* q, Post* post) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    //If memory allocation fails, quit the function
    if (!newNode) {
        return;
    }
    newNode->post = post;
    newNode->next = NULL;

    //If we are adding to the queue for the first time
    if (q->rear == NULL) {
        // Queue is empty
        q->front = newNode;
        q->rear = newNode;
    } else {
        // Add to the rear of the queue
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->size++;
}

// Function to dequeue a post
Post* dequeue(Queue* q) {
    //If queue is empty, no need to dequeue anything
    if (q->size == 0) {
        return NULL;
    }

    QueueNode* temp = q->front;
    Post* post = temp->post;
    q->front = q->front->next;

    if (q->front == NULL) {
        // Queue is now empty
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return post;
}

//Function to use quick_sort algorithm for printing a users friends
void quick_sort_friends(User** friends, int low, int high) {
    if (low < high) {
        int pi = partition_friends(friends, low, high);
        quick_sort_friends(friends, low, pi - 1);
        quick_sort_friends(friends, pi + 1, high);
    }
}

//Helper function for quick sort
int partition_friends(User** friends, int low, int high) {
    User* pivot = friends[high];
    int i = low - 1;
    for (int k = low; k < high; k++) {
        if (strcmp(friends[k]->name, pivot->name) < 0) {
            i++;
            User* temp = friends[i];
            friends[i] = friends[k];
            friends[k] = temp;
        }
    }
    User* temp = friends[i + 1];
    friends[i + 1] = friends[high];
    friends[high] = temp;
    return i + 1;
}

// Function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}

// Function to initialize user
User* create_user(const char* name, const char* email) {
    //If user already exists, you cannot add them
    if (search_user_by_name(name) || search_user_by_email(email)) {
        printf("Error: Name or email already in use.\n");
        return NULL;
    }
    //If the program is at max users, give an error
    if (user_count >= MAX_USERS) {
        printf("Error: Maximum user capacity reached.\n");
        return NULL;
    }
    //Allocate the array
    User* user = (User*)malloc(sizeof(User));
    if (!user) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    //Add a users id, name and email
    user->user_id = next_user_id++;
    strncpy(user->name, name, 49);
    user->name[49] = '\0';
    strncpy(user->email, email, 49);
    user->email[49] = '\0';

    //Create a list of friends
    user->friends = (User**)calloc(10, sizeof(User*));
    if (!user->friends) {
        printf("Error: Memory allocation failed for friends list.\n");
        free(user);
        return NULL;
    }

    //Set friend count to 0 and friend_capacity to 1
    user->friend_count = 0;
    //Used for allocating space
    user->friend_capacity = 1;

    //Creates space to store user messages
    user->chats = (Message**)calloc(MAX_USERS * MAX_MESSAGES, sizeof(Message*));
    if (!user->chats) {
        printf("Error: Memory allocation failed for chats.\n");
        free(user->friends);
        free(user);
        return NULL;
    }
    //Adds user to the user array
    users[user_count++] = user;
    return user;
}

//Function to print friends, they should already be sorted as that is done when friends are added
void print_friends(User* user) {
    if (user->friend_count == 0) {
        printf("No friends.\n");
        return;
    }

    for (int i = 0; i < user->friend_count; i++) {
        if (i > 0) printf(",");
        printf("%s", user->friends[i]->name);
    }
    printf("\n");
}


User* search_user_by_name(const char* name) {
    //Go through the user array until the name matches
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i]->name, name) == 0) {
            return users[i];
        }
    }
    return NULL;
}

User* search_user_by_email(const char* email) {
    //Go through the email array until the email matches
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i]->email, email) == 0) {
            return users[i];
        }
    }
    return NULL;
}

void add_friend(User* user1, User* user2) {
    //If one of the users doesn't exist, or they are the same user, return an error
    if (!user1 || !user2 || user1 == user2){
        return;
    }

    //If the users are already friends, close the function because the recursion at the bottom needs to be stopped
    for (int i = 0; i < user1->friend_count; i++) {
        if (user1->friends[i] == user2){
            return;
        }
    }

    //Realloc the user friend array if it's at its limit
    if (user1->friend_count >= user1->friend_capacity) {
        user1->friend_capacity += 1;
        user1->friends = (User**)realloc(user1->friends, sizeof(User*) * user1->friend_capacity);
        if (!user1->friends) {
            printf("Error: Memory reallocation failed.\n");
            return;
        }
    }

    //Add user 2 to user1's friend list
    user1->friends[user1->friend_count++] = user2;
    //Sort user 1's friends list everytime a friend is added
    quick_sort_friends(user1->friends, 0, user1->friend_count - 1);
    //Do the same for the other user
    add_friend(user2, user1);
}

void delete_friend(User* user1, User* user2) {
    //Check if user1 is friends with user2
    int found = 0;
    for (int i = 0; i < user1->friend_count; i++) {
        if (user1->friends[i] == user2) {
            //Remove user2 from user1's friend list
            user1->friends[i] = user1->friends[--user1->friend_count];
            found = 1;
            break;
        }
    }
    if(found){
        //This only runs if the friendship was deleted, so it will not run a third time
        delete_friend(user2, user1);
    }
}

void delete_user(User* user) {
    //Remove the user from the user array
    for (int i = 0; i < user_count; i++) {
        if (users[i] == user) {
            users[i] = users[--user_count];
            break;
        }
    }
    //Remove the users friends
    for (int i = 0; i < user->friend_count; i++) {
        delete_friend(user->friends[i], user);
    }

    //If the user's friends list exists, free it
    if(user->friends != NULL){
        free(user->friends);
        user->friends = NULL;
    }

    //Free the memory for user's chats
    if (user->chats != NULL) {
        for (int i = 0; i < MAX_USERS * MAX_MESSAGES; i++) {
            free(user->chats[i]); //Free each chat message
        }
        free(user->chats); // Free the chat array
    }
    free(user); //Free the user
}

void print_users() {
    //Bubble sort to sort the users by alphabetical order
    bubble_sort_users(users, user_count);

    //Go through the user array and print every name
    for (int i = 0; i < user_count; i++) {
        if (i > 0) printf(",");
        printf("%s", users[i]->name);
    }
    printf("\n");
}

void change_user_name(User* user, char* new_name) {
    //Check if the new name is available
    if (search_user_by_name(new_name)) {
        printf("Error: Name already in use.\n");
        return;
    }
    //Change the name
    strncpy(user->name, new_name, 49);
    user->name[49] = '\0';
}

void change_user_email(User* user, char* new_email) {
    //Check if the email is in use
    if (search_user_by_email(new_email)) {
        printf("Error: Email already in use or invalid user.\n");
        return;
    }
    //Change the email
    strncpy(user->email, new_email, 49);
    user->email[49] = '\0';
}

User** mutual_friends(User* user1, User* user2) {
    //Create an array the size of users friend count, this array will be returned as it will contain the mutual friends
    User** mutual = (User**)malloc(sizeof(User*) * user1->friend_count);
    int mutual_count = 0;

    //Nested for loop to find mutual friends
    for (int i = 0; i < user1->friend_count; i++) {
        for (int j = 0; j < user2->friend_count; j++) {
            if (user1->friends[i] == user2->friends[j]) {
                mutual[mutual_count++] = user1->friends[i];
            }
        }
    }
    //Re allocate the memory of the array to be equal to mutual count + 1
    mutual = (User**)realloc(mutual, sizeof(User*) * (mutual_count + 1));
    //Make the last item in the list equal to NULL for easier indexing
    mutual[mutual_count] = NULL;
    return mutual;
}

void bubble_sort_users(User** users, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(users[i]->name, users[j]->name) > 0) {
                // Swap the users
                User* temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }
        }
    }
}

void print_mutual_friends(User** friends) {
    //If the friends array does not exist, print no mutual friends
    if (!friends) {
        printf("No mutual friends.\n");
        return;
    }

    //Get the number of mutual friends
    int count = 0;
    while (friends[count] != NULL){
        count++;
    }

    //The list has NULL as it's last item. If it starts with NULL then there are no mutual friends
    if (count == 0) {
        printf("No mutual friends.\n");
        free(friends);
        return;
    }

    //Bubble sort the friends
    bubble_sort_users(friends, count);

    //Print the sorted friends
    for (int i = 0; i < count; i++) {
        if (i > 0) printf(",");
        printf("%s", friends[i]->name);
    }
    printf("\n");
    free(friends);
}


Message* create_message(User* sender, User* receiver, const char* content) {
    // Check if the string length is within the parameters
    if (strlen(content) > 256) {
        printf("Error: String is too long.\n");
        return NULL;
    }

    // Check if users are friends so that they can message each other
    int is_friend = 0;
    for (int i = 0; i < sender->friend_count; i++) {
        if (sender->friends[i] == receiver) {
            is_friend = 1;
            break;
        }
    }
    if (!is_friend) {
        printf("Error: Users are not friends.\n");
        return NULL;
    }

    // Create the message
    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg) {
        printf("Error: Memory allocation failed for message.\n");
        return NULL;
    }
    // Assign the appropriate information into the message struct
    msg->message_id = next_message_id++;
    strncpy(msg->content, content, 256);
    msg->content[255] = '\0';
    msg->sender = sender;
    msg->receiver = receiver;

    // Add to sender's chat
    int found_slot = 0;
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (!sender->chats[receiver->user_id * MAX_MESSAGES + i]) {
            sender->chats[receiver->user_id * MAX_MESSAGES + i] = msg;
            found_slot = 1;
            break;
        }
    }

    // If no free space in sender's chat, remove the oldest message and add the new one
    if (!found_slot) {
        // Remove the oldest message (shift messages)
        for (int i = 1; i < MAX_MESSAGES; i++) {
            sender->chats[receiver->user_id * MAX_MESSAGES + i - 1] = sender->chats[receiver->user_id * MAX_MESSAGES + i];
        }
        sender->chats[receiver->user_id * MAX_MESSAGES + MAX_MESSAGES - 1] = msg;
    }

    // Add to receiver's chat
    found_slot = 0;
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (!receiver->chats[sender->user_id * MAX_MESSAGES + i]) {
            receiver->chats[sender->user_id * MAX_MESSAGES + i] = msg;
            found_slot = 1;
            break;
        }
    }

    // If no free space in receiver's chat, remove the oldest message and add the new one
    if (!found_slot) {
        // Remove the oldest message (shift messages)
        for (int i = 1; i < MAX_MESSAGES; i++) {
            receiver->chats[sender->user_id * MAX_MESSAGES + i - 1] = receiver->chats[sender->user_id * MAX_MESSAGES + i];
        }
        receiver->chats[sender->user_id * MAX_MESSAGES + MAX_MESSAGES - 1] = msg;
    }

    return msg;
}


//Print the messages
void print_message(Message* message) {
    if (!message) return;
    printf("[%s:]%s", message->sender->name, message->content); // Ensure no extra spaces
}


void display_chat(User* user1, User* user2) {
    Message* messages[MAX_MESSAGES];
    int message_count = 0;

    // Collect messages sent by user1 to user2
    int user1_chat_index = user2->user_id * MAX_MESSAGES;
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (user1->chats[user1_chat_index + i]) {
            messages[message_count++] = user1->chats[user1_chat_index + i];
        }
    }

    // Sort messages by message_id
    for (int i = 0; i < message_count - 1; i++) {
        for (int j = i + 1; j < message_count; j++) {
            if (messages[i]->message_id > messages[j]->message_id) {
                Message* temp = messages[i];
                messages[i] = messages[j];
                messages[j] = temp;
            }
        }
    }

    // Display the messages
    for (int i = 0; i < message_count; i++) {
        if (i > 0) printf(",");
        printf("[%s:]%s", messages[i]->sender->name, messages[i]->content);
    }
    printf("\n");
}

Post* new_post(User* user, const char* content) {
    if (strlen(content) > 256) {
        return NULL;
    }

    if (post_count >= MAX_POSTS) {
        free(posts[0]->liked_by);
        free(posts[0]);

        for (int i = 1; i < post_count; i++) {
            posts[i - 1] = posts[i];
        }

        post_count--;
    }

    Post* post = (Post*)malloc(sizeof(Post));
    if (!post) {
        return NULL;
    }

    post->post_id = next_post_id++;
    strncpy(post->content, content, 256);
    post->content[255] = '\0';
    post->user = user;
    post->like_count = 0;
    post->liked_by_capacity = 10;
    post->liked_by = (User**)malloc(sizeof(User*) * post->liked_by_capacity);

    if (!post->liked_by) {
        free(post);
        return NULL;
    }

    posts[post_count++] = post;
    return post;
}


//Add a like to the post
void add_like(Post* post, User* user) {

    // Check if the user is already in the liked_by list
    for (int i = 0; i < post->like_count; i++) {
        if (post->liked_by[i] == user) {
            printf("Error: User has already liked this post.\n");
            return;
        }
    }

    // Ensure the user is a friend or the post owner
    if (post->user != user) {
        int is_friend = 0;
        for (int i = 0; i < post->user->friend_count; i++) {
            if (post->user->friends[i] == user) {
                is_friend = 1;
                break;
            }
        }
        //If not friends, return out
        if (!is_friend) {
            return;
        }
    }

    // Expand liked_by array if needed
    if (post->like_count >= post->liked_by_capacity) {
        post->liked_by_capacity *= 2;
        User** temp = (User**)realloc(post->liked_by, sizeof(User*) * post->liked_by_capacity);
        if (!temp) {
            printf("Error: Memory allocation failed for liked_by array.\n");
            return;
        }
        post->liked_by = temp;
    }

    // Add the user to the liked_by list
    post->liked_by[post->like_count++] = user;
    printf("User '%s' liked post %d.\n", user->name, post->post_id);
}


//Helper function to compare posts by post ID and sort them in ascending order
int compare_posts(const void* a, const void* b) {
    Post* post1 = *(Post**)a;
    Post* post2 = *(Post**)b;

    // Sort primarily by number of likes (descending order)
    if (post2->like_count != post1->like_count) {
        return post2->like_count - post1->like_count;  // Reverse the order for descending
    }

    // If likes are equal, sort by post_id (ascending order)
    return post1->post_id - post2->post_id;
}

void display_feed(User* user) {

    // Sort posts by post_id in ascending order using quick sort
    qsort(posts, post_count, sizeof(Post*), compare_posts);

    Post* relevant_posts[FEED_LIMIT];
    int relevant_count = 0;

    // Collect only relevant posts
    for (int i = 0; i < post_count; i++) {
        Post* post = posts[i];

        // Check if the post is by the user or their friends
        int relevant = (post->user == user);
        if (!relevant) {
            for (int j = 0; j < user->friend_count; j++) {
                if (user->friends[j] == post->user) {
                    relevant = 1;
                    break;
                }
            }
        }

        if (relevant) {
            // Add to the relevant posts list, maintaining most recent 20
            if (relevant_count < FEED_LIMIT) {
                relevant_posts[relevant_count++] = post;
            } else {
                // If the list exceeds FEED_LIMIT, replace oldest with newest
                for (int k = 1; k < FEED_LIMIT; k++) {
                    relevant_posts[k - 1] = relevant_posts[k];
                }
                relevant_posts[FEED_LIMIT - 1] = post;
            }
        }
    }

    // Display the most recent 20 relevant posts in ascending order
    for (int i = 0; i < relevant_count; i++) {
        printf("[%s:]%s", relevant_posts[i]->user->name, relevant_posts[i]->content);
        if (i < relevant_count - 1) printf(","); // Add a comma between posts
    }
    printf("\n");
}