#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define INPUT_NULL ""
#define ADMIN_UN "admin"
#define ADMIN_PASS "123"
#define ADMIN_PN "00000000000"
#define ADMIN 100
#define USER 10
#define PS_SIZE 32
#define UN_SIZE 32
#define TT "Transfer"
#define DT "Deposit"
#define WT "Withdraw"
#define USERS_PATH "/home/amin/Desktop/UT_BANK/users.txt"
#define TRANSACTIONS_PATH "/home/amin/Desktop/UT_BANK/transactions.txt"
#define WELCOME 1
#define LOGIN 2
#define USER_ALLOC_ERROR 3
#define TRANSACTION_ALLOC_ERROR 4
#define HEADE_NODE_ERROR 5
#define ADMIN_ALLOC_ERROR 6
#define USERS_FILE_ERROR 7
#define TRANSACTION_FILE_ERROR 8
#define ROOT_COMMABD_ERROR 9
#define LOGIN_ERROR 10
#define USER_ERROR 11
#define LOGOUT_ERROR 12
#define PERMISSION_ERROR 13
#define TAKEN_USER_NAME_OR_PHONE_NUMBER_ERROR 14

typedef struct users user;
struct users {
    char *user_name;
    char *password;
    char *phone_number;
    int id;
    float balance;
    int transactions;
    user *next;
};

typedef struct transacion_info transaction;
struct transacion_info {
    char *transaction_type;
    char *source;
    char *destination;
    float amount;
    transaction *next;
};

user *users_head(void);

transaction *transaction_head(void);

int add_admin(user *users_head_node, char *user_name, char *password, char *phone_number);

int read_users_from_file(char *user_path, user *users_head_node, int *id);

int read_transactions_from_file(char *transactions_path, transaction *transactions_head_node);

int add_users_to_list(user *users_head_node, user *new_user);

int add_transactions_to_list(transaction *transactions_head_node, transaction *new_transaction);

void get_command(int *role, int *login, int *exit, char *user_name, user *users_head_node,
                 transaction *transactions_head_node, int *id);

void show_messages(int message);

int login(user *users_head_node, char *user_name, int *role, int *login);

user *search_users(user *users_head_node, char *user_name,char* phone_number);

transaction *search_transaction(transaction *transactions_head_node, char *user_name, transaction *view_transactions);

int write_users_to_file(char *user_path, user *users_head_node, transaction *transactions_head_node, int id);

int write_transactions_to_file(char *transactions_path, transaction *transactions_head_node);

int users_length(user *users_head_node);

int transactions_length(transaction *transactions_head_node);

int delete_all_transactions_nodes(transaction *transactions_head_node);

void exit_ut_bank(int *exit, user *users_head_node, transaction *transactions_head_node, int id);

void logout(int *role, int *login, char *user_name);

int view(const int *role, user *users_head_node, transaction *transactions_head_node, char *user_name);

int delete_user(user *users_head_node);

int add_user_by_admin(user *users_head_node, int *id);

int deposit(user *users_head_node, transaction *transactions_head_node);

int withdraw(user *users_head_node, transaction *transactions_head_node);

int transfer(user *users_head_node, transaction *transactions_head_node, char *user_name);