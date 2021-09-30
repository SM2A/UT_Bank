#include "ut_bank.h"

int main() {

    int role = FALSE;
    int login = FALSE;
    int exit = FALSE;
    int id = 0;
    char user_name[UN_SIZE];

    user *users_head_node = users_head();
    transaction *transactions_head_node = transaction_head();

    add_admin(users_head_node, ADMIN_UN, ADMIN_PASS, ADMIN_PN);
    read_users_from_file(USERS_PATH, users_head_node, &id);
    read_transactions_from_file(TRANSACTIONS_PATH, transactions_head_node);

    show_messages(WELCOME);
    show_messages(LOGIN);

    while (TRUE) {
        get_command(&role, &login, &exit, user_name, users_head_node, transactions_head_node, &id);
        if (exit == TRUE) break;
    }
    return 0;
}