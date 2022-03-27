#include <iostream>
#include <string>


struct Node {
    int value;
    Node* next;
};

int num_tokens_general, num_tokens_preferential;
int token_general_count = 1, token_preferential_count = 1;
int general_called = 0, preferential_called = 0;

bool queue_closed = false;

// heads[0] -> preferential line
// heads[1] -> general line
Node *heads[2] = {NULL, NULL};

void initial_configuration(int *qnt_tokens_general_ptr, int *qnt_tokens_preferential_ptr);

void enqueue(int *token_number, int index);
void dequeue(int index, int *called);
int size_queue(int index);

void menu_display();


int main () {

    initial_configuration(&num_tokens_general, &num_tokens_preferential);

    // Main loop:
    while ((num_tokens_general > 0 || num_tokens_preferential > 0 || 
            size_queue(1) > 0 || size_queue(0) > 0) && !queue_closed)
        menu_display();


    // Final view:
    std::cout << "\n**********************\n";
    std::cout << "*     BANK QUEUE     *";
    std::cout << "\n**********************\n\n";

    std::string general_called_text = (general_called == 0) ? "#" : std::to_string(general_called);
    std::string preferential_called_text = (preferential_called == 0) ? "#" : std::to_string(preferential_called);

    std::cout << "--- Now Serving ---\n\n";
    std::cout << "General Token: " << general_called_text << "\n";
    std::cout << "Preferential Token: " << preferential_called_text << "\n";
    std::cout << "-------------------\n\n";
    std::cout << std::endl;


    system("pause");
    return 0;
}


// Configures the initial information about the number of tokens that will be released
// before the program starts:
void initial_configuration(int *qnt_tokens_general_ptr, int *qnt_tokens_preferential_ptr) {

    std::cout << ">> INITIAL CONFIGURATIONS <<\n\n";
    
    std::cout << "First, inform the number of general tokens that will be distributed: ";
    std::cin >> *qnt_tokens_general_ptr;
    
    std::cout << "Second, inform the number of preferential tokens that will be distributed: ";
    std::cin >> *qnt_tokens_preferential_ptr;

    bool confirmation = false;
    while (!confirmation) {
        std::cout << "\n";
        std::cout << *qnt_tokens_general_ptr << " general tokens and " << *qnt_tokens_preferential_ptr << 
        " preferential tokens " << "were created.\nYou confirm the operation (1- Yes / 0 - No)? ";

        int confirmed;
        std::cin >> confirmed;

        if (confirmed == 0) {
            std::cout << "\nInform the new number of general tokens to be distributed: ";
            std::cin >> *qnt_tokens_general_ptr;
            std::cout << "Inform the new number of preferential tokens to be distributed: ";
            std::cin >> *qnt_tokens_preferential_ptr;
            confirmation = false;
        }
        else if (confirmed == 1)
            confirmation = true;
        else
            std::cout << "\n>>> ERROR: Invalid Input. Try again<<<\n";
    }
   

}


// Holds all the content that will appear in the terminal:
void menu_display() {

    static int total_g_tkns = num_tokens_general, total_p_tkns = num_tokens_preferential;

    std::cout << "\n**********************\n";
    std::cout << "*     BANK QUEUE     *";
    std::cout << "\n**********************\n\n";

    std::string general_called_text = (general_called == 0) ? "#" : std::to_string(general_called);
    std::string preferential_called_text = (preferential_called == 0) ? "#" : std::to_string(preferential_called);

    std::cout << "--- Now Serving ---\n\n";
    std::cout << "General Token: " << general_called_text << "\n";
    std::cout << "Preferential Token: " << preferential_called_text << "\n";
    std::cout << "-------------------\n\n";
    std::cout << std::endl;

    std::cout << "-> " << num_tokens_general << " general tokens left from " << total_g_tkns << "\n";
    std::cout << "-> " << num_tokens_preferential << " preferential tokens left from " << total_p_tkns << "\n";
    std::cout << std::endl;

    std::cout << "Choose an action:\n\n";
    std::cout << "[1] - Release a new token\t[2] - Call a new token  \t[3] - End service\n\n>> ";

    int action;
    std::cin >> action;

    switch (action) {

        char type_token;

        case 1:
            if (num_tokens_general > 0 && num_tokens_preferential > 0) {
                std::cout << "Enter 'g' for general token or 'p' for preferential token: ";
                std::cin >> type_token;
                if (tolower(type_token) == 'g') {
                    enqueue(&token_general_count, 1);
                    num_tokens_general--;
                }
                else if (tolower(type_token) == 'p') {
                    enqueue(&token_preferential_count, 0);
                    num_tokens_preferential--;
                }
                else
                    std::cout << "\n>>> ERROR: Invalid Input. Try again <<<\n";
            }
            else if (num_tokens_general > 0) {
                std::cout << "There are only general tokens. Want to get one (1- Yes / 0 - No)? ";
                std::cin >> type_token;
                if (type_token == '1') {
                    enqueue(&token_general_count, 1);
                    num_tokens_general--;
                }
                else if (type_token != '0')
                    std::cout << "\n>>> ERROR: Invalid Input. Try again <<<\n";

            }
            else if (num_tokens_preferential > 0) {
                std::cout << "There are only preferential tokens. Want to get one (1- Yes / 0 - No)? ";
                std::cin >> type_token;
                if (type_token == '1') {
                    enqueue(&token_preferential_count, 0);
                    num_tokens_preferential--;
                }
                else if (type_token != '0')
                    std::cout << "\n>>> ERROR: Invalid Input. Try again <<<\n";

            }
            else
                std::cout << "\n>>> No more tokens available <<<\n";
            break;
        case 2:
            std::cout << "Enter 'g' for general token or 'p' for preferential token: ";
            std::cin >> type_token;
            if (tolower(type_token) == 'g')
                dequeue(1, &general_called);
            else if (tolower(type_token) == 'p')
                dequeue(0, &preferential_called);
            else
                std::cout << "\n>>> ERROR: Invalid Input. Try again <<<\n";

            break;
        case 3:
            std::cout << "\n>>> Service closed! <<<\n";
            queue_closed = true;
            break;
        default:
            std::cout << "\n>>> ERROR: Invalid Input. Try again <<<\n";
            break;
    }

}


// It adds a person to the end of the queue:
void enqueue(int *token_number, int index) {

    Node *ptr = new Node();

    ptr->value = *token_number;
    ptr->next = NULL;

    if (heads[index] == NULL)
        heads[index] = ptr;
    
    else {
        Node *temp = heads[index];

        while (temp->next != NULL) 
            temp = temp->next;
        
        temp->next = ptr;
    }

    *token_number += 1;

}


// He calls the first person in line and removes them from the line
void dequeue(int index, int *called) {

    if (heads[index] == NULL)
        std::cout << "\n>>> There are no people in line to be called <<<\n";
    
    else {

        Node *ptr = heads[index];
        *called = ptr->value;
        heads[index] = heads[index]->next;
        free(ptr);
    }
}


// Returns the size of a queue (recursive solution)
int size_queue(int index) {

    int count = 0;
    Node *current = heads[index];
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;    
    
}