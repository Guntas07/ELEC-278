// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct User {
    // add attributes
    int user_id;            // User ID
    char name[50];          // User name
    char email[50];         // User email
    struct User** friends;  // Array of pointers to friends
    int friend_count;       // Current number of friends
    int friend_capacity;    // Capacity of the friends array
    struct Message** chats; // Array of chat pointers with friends
} User;

typedef struct Post {
    // add attributes
    int post_id;
    char content[256];
    int like_count;
    User** liked_by;
    User* user;
    int liked_by_capacity; // Capacity of liked_by array
} Post;

typedef struct Message {
    // add attributes
    int message_id;
    char content[256];
    User* sender;
    User* receiver;
} Message;

// Define the queue node structure
typedef struct QueueNode {
    Post* post;
    struct QueueNode* next;
} QueueNode;

// Define the queue structure
typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;


void print_friends(User* user);
void testingParser(int arg1, char* arg2);
User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void add_message_to_chat(User* user, User* other_user, Message* new_msg);
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
int compare_posts(const void* a, const void* b);
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);
void bubble_sort_users(User** users, int count);
void init_queue(Queue* q);
void enqueue(Queue* q, Post* post);
Post* dequeue(Queue* q);
int partition_friends(User** friends, int low, int high);
void quick_sort_friends(User** friends, int low, int high);

#endif