#include "ut_bank.h"

user *users_head(void) {
    user *users = (user *) malloc(sizeof(user));
    if (users == NULL) {
        show_messages(USER_ALLOC_ERROR);
        return FALSE;
    } else if (users != NULL) return users;
}

transaction *transaction_head(void) {
    transaction *transactions = (transaction *) malloc(sizeof(transaction));
    if (transactions == NULL) {
        show_messages(TRANSACTION_ALLOC_ERROR);
        return FALSE;
    } else if (transactions != NULL) return transactions;
}

int add_admin(user *users_head_node, char *user_name, char *password, char *phone_number) {
    if (users_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    user *admin = malloc(sizeof(user));
    if (admin == NULL) {
        show_messages(ADMIN_ALLOC_ERROR);
        return FALSE;
    }
    users_head_node->next = admin;
    admin->next = NULL;
    admin->user_name = user_name;
    admin->password = password;
    admin->phone_number = phone_number;
    return TRUE;
}

int read_users_from_file(char *user_path, user *users_head_node, int *id) {
    FILE *users_file = fopen(user_path, "r");
    if (users_file == NULL) {
        users_file = fopen(user_path, "w");
        fprintf(users_file, "%d\n", 0);
        fprintf(users_file, "%d\n", 0);
        fclose(users_file);
        if (users_file == NULL) {
            show_messages(USERS_FILE_ERROR);
            return FALSE;
        }
    }
    users_file = fopen(user_path, "r");
    int users_count;
    fscanf(users_file, "%d", &users_count);

    user **users_list = (user **) malloc(sizeof(user) * users_count);
    if (users_list == NULL) {
        show_messages(USER_ALLOC_ERROR);
        return FALSE;
    }
    for (int i = 0; i < users_count; ++i) {
        user *new_user = (user *) malloc(sizeof(user));
        if (new_user == NULL) {
            show_messages(USER_ALLOC_ERROR);
            return FALSE;
        }

        char *user_name = (char *) malloc(sizeof(char));
        fscanf(users_file, "%s", user_name);
        new_user->user_name = user_name;
        char *password = (char *) malloc(sizeof(char));
        fscanf(users_file, "%s", password);
        new_user->password = password;
        char *phone_number = (char *) malloc(sizeof(char));
        fscanf(users_file, "%s", phone_number);
        new_user->phone_number = phone_number;
        fscanf(users_file, "%d", &new_user->id);
        fscanf(users_file, "%f", &new_user->balance);
        fscanf(users_file, "%d", &new_user->transactions);

        users_list[i] = new_user;
        add_users_to_list(users_head_node, users_list[i]);
    }
    fscanf(users_file, "%d", id);
    fclose(users_file);
    return TRUE;
}

int read_transactions_from_file(char *transactions_path, transaction *transactions_head_node) {
    FILE *transactions_file = fopen(transactions_path, "r");
    if (transactions_file == NULL) {
        transactions_file = fopen(transactions_path, "w");
        fprintf(transactions_file, "%d\n", 0);
        fclose(transactions_file);
        if (transactions_file == NULL) {
            show_messages(TRANSACTION_FILE_ERROR);
            return FALSE;
        }
    }
    transactions_file = fopen(transactions_path, "r");
    int transactions_count;
    fscanf(transactions_file, "%d", &transactions_count);

    transaction **transactions_list = (transaction **) malloc(sizeof(transaction) * transactions_count);
    if (transactions_list == NULL) {
        show_messages(TRANSACTION_ALLOC_ERROR);
        return FALSE;
    }
    for (int i = 0; i < transactions_count; ++i) {
        transaction *new_transaction = (transaction *) malloc(sizeof(transaction));
        if (new_transaction == NULL) {
            show_messages(TRANSACTION_ALLOC_ERROR);
            return FALSE;
        }

        char *transaction_type = (char *) malloc(sizeof(char));
        fscanf(transactions_file, "%s", transaction_type);
        new_transaction->transaction_type = transaction_type;
        char *transaction_source = (char *) malloc(sizeof(char));
        fscanf(transactions_file, "%s", transaction_source);
        new_transaction->source = transaction_source;
        char *transaction_destination = (char *) malloc(sizeof(char));
        fscanf(transactions_file, "%s", transaction_destination);
        new_transaction->destination = transaction_destination;
        fscanf(transactions_file, "%f", &new_transaction->amount);

        transactions_list[i] = new_transaction;
        add_transactions_to_list(transactions_head_node, transactions_list[i]);
    }
    fclose(transactions_file);
    return TRUE;
}

int add_users_to_list(user *users_head_node, user *new_user) {
    if (users_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    user *current = users_head_node->next;
    new_user->next = current->next;
    current->next = new_user;
    return TRUE;
}

int add_transactions_to_list(transaction *transactions_head_node, transaction *new_transaction) {
    if (transactions_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    transaction *current = transactions_head_node;
    new_transaction->next = current->next;
    current->next = new_transaction;
    return TRUE;
}

void get_command(int *role, int *result_login, int *exit, char *user_name, user *users_head_node,
                 transaction *transactions_head_node, int *id) {

    while (TRUE) {

        char *root_command = malloc(sizeof(char));

        scanf("%s", root_command);

        if (strcmp(root_command, "login") == 0) {
            if (*result_login == TRUE) {
                show_messages(LOGOUT_ERROR);
                while (getchar() != '\n');
                break;
            }
            login(users_head_node, user_name, role, result_login);
            break;
        }
        if (strcmp(root_command, "exit") == 0) {
            exit_ut_bank(exit, users_head_node, transactions_head_node, *id);
            break;
        }

        if ((*result_login) == TRUE) {

            if (strcmp(root_command, "add_user") == 0) {
                if (*role == USER) {
                    show_messages(PERMISSION_ERROR);
                    while (getchar() != '\n');
                    break;
                }
                add_user_by_admin(users_head_node, id);
                break;

            } else if (strcmp(root_command, "logout") == 0) {
                logout(role, result_login, user_name);
                break;

            } else if (strcmp(root_command, "deposit") == 0) {
                if (*role == USER) {
                    show_messages(PERMISSION_ERROR);
                    while (getchar() != '\n');
                    break;
                }
                deposit(users_head_node, transactions_head_node);
                break;

            } else if (strcmp(root_command, "withdraw") == 0) {
                if (*role == USER) {
                    show_messages(PERMISSION_ERROR);
                    while (getchar() != '\n');
                    break;
                }
                withdraw(users_head_node, transactions_head_node);
                break;

            } else if (strcmp(root_command, "view") == 0) {
                view(role, users_head_node, transactions_head_node, user_name);
                break;

            } else if (strcmp(root_command, "delete") == 0) {
                if (*role == USER) {
                    show_messages(PERMISSION_ERROR);
                    while (getchar() != '\n');
                    break;
                }
                delete_user(users_head_node);
                break;

            } else if (strcmp(root_command, "transfer") == 0) {
                if (*role == ADMIN) {
                    show_messages(PERMISSION_ERROR);
                    while (getchar() != '\n');
                    break;
                }
                transfer(users_head_node, transactions_head_node, user_name);
                break;

            } else {
                show_messages(ROOT_COMMABD_ERROR);
                while (getchar() != '\n');
            }
        } else if (*result_login == FALSE) {
            show_messages(LOGIN);
            while (getchar() != '\n');
        }
    }
}

void show_messages(int message) {
    switch (message) {
        case WELCOME:
            printf("WELCOME TO UTBANK\n");
            break;
        case LOGIN:
            printf("Please Login To Continue\n");
            break;
        case USER_ALLOC_ERROR:
            printf("A Problem Happens To Allocate Users\n");
            break;
        case TRANSACTION_ALLOC_ERROR:
            printf("A Problem Happens To Allocate Transactions\n");
            break;
        case HEADE_NODE_ERROR:
            printf("No Head Node Found\n");
            break;
        case ADMIN_ALLOC_ERROR:
            printf("A Problem Happens To Allocate Admin User\n");
            break;
        case USERS_FILE_ERROR:
            printf("A Problem Happens To Open Users File\n");
            break;
        case TRANSACTION_FILE_ERROR:
            printf("A Problem Happens To Open Transactions File\n");
            break;
        case ROOT_COMMABD_ERROR:
            printf("Enter Correct Command\n");
            break;
        case LOGIN_ERROR:
            printf("User name Or Password Incorrect\n");
            break;
        case USER_ERROR:
            printf("No User Found\n");
            break;
        case LOGOUT_ERROR:
            printf("Please Logout In Order To Login To Different Account\n");
            break;
        case PERMISSION_ERROR:
            printf("You Don't Have Permission To Do This\n");
            break;
        case TAKEN_USER_NAME_OR_PHONE_NUMBER_ERROR:
            printf("This User Name Or Phone Number Is Taken . Try Another One\n");
            break;
        default:
            printf("?????\n");
    }
}

int login(user *users_head_node, char *result_user_name, int *role, int *login) {

    char user_name[UN_SIZE];
    char password[PS_SIZE];

    scanf(" %s %s", user_name, password);

    user *login_user = search_users(users_head_node, user_name,INPUT_NULL);

    if (login_user == NULL) {
        show_messages(LOGIN_ERROR);
        return FALSE;
    } else if (login_user != NULL) {
        if ((strcmp(login_user->user_name, user_name) == 0) && (strcmp(login_user->password, password) == 0)) {
            strcpy(result_user_name, user_name);
            *login = TRUE;
            if (strcmp(user_name, ADMIN_UN) == 0) *role = ADMIN;
            else *role = USER;
            printf("Welcome %s\n", user_name);
            return TRUE;
        } else {
            show_messages(LOGIN_ERROR);
            return FALSE;
        }
    }
}

user *search_users(user *users_head_node, char *user_name, char* phone_number) {
    if (users_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    user *current = users_head_node->next;
    while (current != NULL) {
        if ((strcmp(current->user_name, user_name) == 0)||(strcmp(current->phone_number, phone_number) == 0))
            return current;
        current = current->next;
    }
    return NULL;
}

transaction *search_transaction(transaction *transactions_head_node, char *user_name, transaction *view_transactions) {
    if (transactions_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    delete_all_transactions_nodes(view_transactions);
    transaction *current = transactions_head_node->next;
    transaction **view_transaction_list = (transaction **) malloc(sizeof(transaction));
    int transaction_count = -1;

    while (current != NULL) {

        if ((strcmp(current->source, user_name) == 0) || (strcmp(current->destination, user_name) == 0)) {

            transaction_count++;
            transaction *new_view_transaction = (transaction *) malloc(sizeof(transaction));
            view_transaction_list = realloc(view_transaction_list, sizeof(transaction));

            new_view_transaction->transaction_type = current->transaction_type;
            new_view_transaction->source = current->source;
            new_view_transaction->destination = current->destination;
            new_view_transaction->amount = current->amount;

            view_transaction_list[transaction_count] = new_view_transaction;
            add_transactions_to_list(view_transactions, view_transaction_list[transaction_count]);
        }

        current = current->next;
    }
    return view_transactions;
}

int users_length(user *users_head_node) {
    int counter = 0;
    user *current = users_head_node->next;
    while (current->next != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}

int transactions_length(transaction *transactions_head_node) {
    int counter = 0;
    transaction *current = transactions_head_node;
    while (current->next != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}

int write_users_to_file(char *user_path, user *users_head_node, transaction *transactions_head_node, int id) {

    FILE *users_file = fopen(user_path, "w");
    if (users_file == NULL) {
        show_messages(USERS_FILE_ERROR);
        return FALSE;
    }
    fprintf(users_file, "%d\n", users_length(users_head_node));
    user *current = users_head_node->next->next;
    while (current != NULL) {
        transaction *view_transactions = transaction_head();
        fprintf(users_file, "%s\n", current->user_name);
        fprintf(users_file, "%s\n", current->password);
        fprintf(users_file, "%s\n", current->phone_number);
        fprintf(users_file, "%d\n", current->id);
        fprintf(users_file, "%f\n", current->balance);
        fprintf(users_file, "%d\n", (transactions_length(search_transaction(transactions_head_node,
                                                                            current->user_name, view_transactions))));
        current = current->next;
    }
    fprintf(users_file, "%d\n", id);
    fclose(users_file);
    return TRUE;
}

int write_transactions_to_file(char *transactions_path, transaction *transactions_head_node) {
    FILE *transactions_file = fopen(transactions_path, "w");
    if (transactions_file == NULL) {
        show_messages(TRANSACTION_FILE_ERROR);
        return FALSE;
    }
    fprintf(transactions_file, "%d\n", transactions_length(transactions_head_node));
    transaction *current = transactions_head_node->next;
    while (current != NULL) {
        fprintf(transactions_file, "%s\n", current->transaction_type);
        fprintf(transactions_file, "%s\n", current->source);
        fprintf(transactions_file, "%s\n", current->destination);
        fprintf(transactions_file, "%f\n", current->amount);
        current = current->next;
    }
    fclose(transactions_file);
    return TRUE;
}

int delete_all_transactions_nodes(transaction *transactions_head_node) {
    if (transactions_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    transaction *current = transactions_head_node;
    transaction *temp;
    while (current!= NULL) {
        temp = current->next;
        free(current);
        current = temp;
    }
    transactions_head_node->next = NULL;
    return TRUE;
}

void exit_ut_bank(int *exit, user *users_head_node, transaction *transactions_head_node, int id) {
    write_users_to_file(USERS_PATH, users_head_node, transactions_head_node, id);
    write_transactions_to_file(TRANSACTIONS_PATH, transactions_head_node);
    *exit = TRUE;
}

void logout(int *role, int *login, char *user_name) {
    *role = FALSE;
    *login = FALSE;
    printf("Good Bye %s\n", user_name);
}

int view(const int *role, user *users_head_node, transaction *transactions_head_node, char *user_name) {
    if (*role == USER) {
        if (getchar() != '\n') {
            show_messages(ROOT_COMMABD_ERROR);
            while (getchar() != '\n');
            return FALSE;
        }
        user *current_user = search_users(users_head_node, user_name,INPUT_NULL);
        printf("\n///////////////YOUR INFO////////////////\n");
        printf("ID : %d\n", current_user->id);
        printf("User Name : %s\n", current_user->user_name);
        printf("Phone Number : %s\n", current_user->phone_number);
        printf("Balance : %f\n", current_user->balance);
        printf("\n///////////////YOUR TRANSACTIONS////////////////\n");
        transaction *view_transactions = transaction_head();
        search_transaction(transactions_head_node, user_name, view_transactions);
        transaction *current_transaction = view_transactions;
        current_transaction = current_transaction->next;
        while (current_transaction != NULL) {
            printf("\n");
            printf("TransactionsType : %s\n", current_transaction->transaction_type);
            printf("From : %s\n", current_transaction->source);
            printf("To : %s\n", current_transaction->destination);
            printf("Amount : %f\n", current_transaction->amount);
            current_transaction = current_transaction->next;
        }
    } else if (*role == ADMIN) {
        char view_user_name[UN_SIZE];
        scanf(" %s", view_user_name);
        if (search_users(users_head_node, view_user_name,INPUT_NULL) == NULL) {
            show_messages(USER_ERROR);
            return FALSE;
        }
        user *view_current_user = search_users(users_head_node, view_user_name,INPUT_NULL);
        printf("\n///////////////%s's INFO////////////////\n\n", view_user_name);
        printf("ID : %d\n", view_current_user->id);
        printf("User Name : %s\n", view_current_user->user_name);
        printf("Phone Number : %s\n", view_current_user->phone_number);
        printf("Balance : %f\n", view_current_user->balance);
        printf("\n///////////////%s's TRANSACTIONS////////////////\n", view_user_name);
        transaction *view_transactions = transaction_head();
        search_transaction(transactions_head_node, view_user_name, view_transactions);
        transaction *current_transaction = view_transactions;
        current_transaction = current_transaction->next;
        while (current_transaction != NULL) {
            printf("\n");
            printf("TransactionsType : %s\n", current_transaction->transaction_type);
            printf("From : %s\n", current_transaction->source);
            printf("To : %s\n", current_transaction->destination);
            printf("Amount : %f\n", current_transaction->amount);
            current_transaction = current_transaction->next;
        }
    }
}

int delete_user(user *users_head_node) {
    char user_name[UN_SIZE];
    scanf(" %s", user_name);
    if (search_users(users_head_node, user_name,INPUT_NULL) == NULL) {
        show_messages(USER_ERROR);
        return FALSE;
    }
    user *current = search_users(users_head_node, user_name,INPUT_NULL);
    user *previous = users_head_node;
    while (strcmp(previous->next->user_name, user_name) != 0) {
        previous = previous->next;
    }
    previous->next = current->next;
    free(current);
    printf("User %s Deleted Successfully\n", user_name);
    return TRUE;
}

int add_user_by_admin(user *users_head_node, int *id) {
    if (users_head_node == NULL) {
        show_messages(HEADE_NODE_ERROR);
        return FALSE;
    }
    char *user_name = (char *) malloc(sizeof(char));
    char *password = (char *) malloc(sizeof(char));
    char *phone_number = (char *) malloc(sizeof(char));
    scanf(" %s %s %s", user_name, password, phone_number);
    if (search_users(users_head_node, user_name,phone_number) != NULL) {
        if ((strcmp(search_users(users_head_node, user_name,phone_number)->user_name, user_name) == 0)
        ||strcmp(search_users(users_head_node, user_name,phone_number)->phone_number, phone_number) == 0) {
            show_messages(TAKEN_USER_NAME_OR_PHONE_NUMBER_ERROR);
            while (getchar() != '\n');
            return FALSE;
        }
    }
    user *new_user = (user *) malloc(sizeof(user));
    new_user->user_name = user_name;
    new_user->password = password;
    new_user->phone_number = phone_number;
    (*id)++;
    new_user->id = *id;
    new_user->balance = 0;
    new_user->transactions = 0;
    user *current = users_head_node->next;
    new_user->next = current->next;
    current->next = new_user;
    printf("User %s Added Successfully\n", user_name);
    return TRUE;
}

int deposit(user *users_head_node, transaction *transactions_head_node) {
    char user_name[UN_SIZE];
    float amount;
    scanf(" %s %f", user_name, &amount);
    if (search_users(users_head_node, user_name,INPUT_NULL) == NULL) {
        show_messages(USER_ERROR);
        return FALSE;
    }
    user *current = search_users(users_head_node, user_name,INPUT_NULL);
    current->balance = (current->balance) + amount;
    transaction *new_transaction = (transaction *) malloc(sizeof(transaction));
    new_transaction->transaction_type = DT;
    new_transaction->source = ADMIN_UN;
    new_transaction->destination = current->user_name;
    new_transaction->amount = amount;
    add_transactions_to_list(transactions_head_node, new_transaction);
    printf("Successfully %f Added To %s\n", amount, user_name);
    return TRUE;
}

int withdraw(user *users_head_node, transaction *transactions_head_node) {
    char user_name[UN_SIZE];
    float amount;
    scanf(" %s %f", user_name, &amount);
    if (search_users(users_head_node, user_name,INPUT_NULL) == NULL) {
        show_messages(USER_ERROR);
        return FALSE;
    }
    user *current = search_users(users_head_node, user_name,INPUT_NULL);
    if (amount > current->balance) {
        printf("Not Enough Balance\n");
        return FALSE;
    }
    current->balance = (current->balance) - amount;
    transaction *new_transaction = (transaction *) malloc(sizeof(transaction));
    new_transaction->transaction_type = WT;
    new_transaction->source = current->user_name;
    new_transaction->destination = ADMIN_UN;
    new_transaction->amount = amount;
    add_transactions_to_list(transactions_head_node, new_transaction);
    printf("Successfully %f Withdrew From %s\n", amount, user_name);
    return TRUE;
}

int transfer(user *users_head_node, transaction *transactions_head_node, char *user_name) {
    char destination_user_name[UN_SIZE];
    float amount;
    scanf(" %s %f", destination_user_name, &amount);
    if (search_users(users_head_node, destination_user_name,INPUT_NULL) == NULL) {
        show_messages(USER_ERROR);
        return FALSE;
    }
    user *destination = search_users(users_head_node, destination_user_name,INPUT_NULL);
    user *source = search_users(users_head_node, user_name,INPUT_NULL);
    if (amount > source->balance) {
        printf("Not Enough Balance\n");
        return FALSE;
    }
    source->balance = (source->balance) - amount;
    destination->balance = (destination->balance) + amount;
    transaction *new_transaction = (transaction *) malloc(sizeof(transaction));
    new_transaction->transaction_type = TT;
    new_transaction->source = source->user_name;
    new_transaction->destination = destination->user_name;
    new_transaction->amount = amount;
    add_transactions_to_list(transactions_head_node, new_transaction);
    printf("Successfully %f Transfer From %s To %s\n", amount, user_name, destination_user_name);
    return TRUE;
}