#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_LEN 100
#define DATE_LEN 11
#define SUMMARY_LEN 500
#define HASH_SIZE 15
#define MAX_MENTEES 10
#define ROLE_MENTEE 0
#define ROLE_MENTOR 1

struct Task {
    char description[STR_LEN];
    char due_date[DATE_LEN];
    struct Task* next;
};

struct Meeting_note {
    char date[DATE_LEN];
    char summary[SUMMARY_LEN];
    struct Meeting_note* next;
};

struct User {
    // Base user information
    char username[STR_LEN];
    char password[STR_LEN];
    char name[STR_LEN];
    int role; // ROLE_MENTEE/ROLE_MENTOR
    
    // Mentee-specific fields
    char email[STR_LEN];
    char phone[STR_LEN];
    char department[STR_LEN];
    char year[STR_LEN];
    char digital_id[STR_LEN];
    char reg_no[STR_LEN];
    char parent_phone[STR_LEN];
    char parent_email[STR_LEN];
    struct Task* tasks;
    struct Meeting_note* meetings;
    
    // Mentor-specific fields 
    struct User* mentees[MAX_MENTEES]; // mentee users 
    int mentee_count;
    
    // For hash table chaining to handle collision handling
    struct User* next;
};

struct User* hash_table[HASH_SIZE];
struct User* current_user = NULL;
struct User* mentor = NULL;
int i;

void sample_data();
void add_user(char* username, char* password, char* name, char* email, int role);
struct User* authenticate_user(char* username, char* password, int role);
int username_exists(char* username);
int is_valid_email(char* email);
void display_header(char* title);
void main_menu();
int login_with_role(int role);
int register_mentee();
void mentor_menu();
void mentee_menu();
void view_mentee_details(struct User* mentee);
void edit_mentee_details(struct User* mentee);
void manage_tasks(struct User* mentee);
void view_tasks(struct User* mentee);
void add_task(struct User* mentee);
void edit_task(struct User* mentee);
void delete_task(struct User* mentee);
void manage_meetings(struct User* mentee);
void view_meetings(struct User* mentee);
void add_meeting(struct User* mentee);
void edit_meeting(struct User* mentee);
void delete_meeting(struct User* mentee);

int hash_function(char* str) {
    int hash = 0;
    for (i = 0; str[i] != '\0'; i++) {
        hash = hash * 31 + str[i];
    }
    return hash % HASH_SIZE;
}

void sample_data() {
    for (i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }
    
    add_user("mentor1", "pass1", "Praveen Sam", "praveen@example.com", ROLE_MENTOR);
    mentor = authenticate_user("mentor1", "pass1", ROLE_MENTOR);

    add_user("mentee1", "pass1", "Rajesh", "rajesh@yahoo.com", ROLE_MENTEE);

    // Setup mentee's details
    struct User* rajesh = authenticate_user("mentee1", "pass1", ROLE_MENTEE);
    mentor->mentees[0] = rajesh;
    mentor->mentee_count = 1;
    
    strcpy(rajesh->phone, "1234567890");
    strcpy(rajesh->department, "Computer Science");
    strcpy(rajesh->year, "First Year");
    strcpy(rajesh->digital_id, "2420491");
    strcpy(rajesh->reg_no, "3122245002123");
    strcpy(rajesh->parent_phone, "9876543210");
    strcpy(rajesh->parent_email, "parent@yahoo.com");
    rajesh->tasks = NULL;
    rajesh->meetings = NULL;
    
}

void add_user(char* username, char* password, char* name, char* email, int role) {
    int index = hash_function(username);
    
    struct User* new_user = (struct User*)malloc(sizeof(struct User));
    if (new_user == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    strcpy(new_user->name, name); 
    strcpy(new_user->email, email);
    new_user->role = role;
    
    if (role == ROLE_MENTOR) {
        new_user->mentee_count = 0;
    } else {
        strcpy(new_user->phone, "");
        strcpy(new_user->department, "");
        strcpy(new_user->year, "");
        strcpy(new_user->digital_id, "");
        strcpy(new_user->reg_no, "");
        strcpy(new_user->parent_phone, "");
        strcpy(new_user->parent_email, "");
        new_user->tasks = NULL;
        new_user->meetings = NULL;
    }
    
    new_user->next = NULL;

    if (hash_table[index] == NULL) {
        hash_table[index] = new_user;
    } else {
        struct User* current = hash_table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_user;
    }
}

// returns user struct with same username and password
struct User* authenticate_user(char* username, char* password, int role) {
    int index = hash_function(username);
    
    struct User* current = hash_table[index];

    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0 && current->role == role) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; //failed
}

// returns 1 if username exists
int username_exists(char* username) {
    int index = hash_function(username);
    
    struct User* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return 1; 
        }
        current = current->next;
    }
    
    return 0; 
}

// returns 1 if valid
int is_valid_email(char* email) {
    char* at_symbol = strchr(email, '@'); //find @ 
    
    // No @ symbol found OR @ is at the beginning
    if (at_symbol == NULL || at_symbol == email) {
        return 0;
    }
    
    // find position of dot after @
    char* dot = strchr(at_symbol, '.');
    
    // No dot after @ or dot is immediately after @
    if (dot == NULL || dot == at_symbol + 1) {
        return 0;
    }

    //
    if (dot[1] == '\0') {
        return 0;
    }
    
    return 1;
}

// Display header
void display_header(char* title) {
    printf("\n=========================================\n");
    printf("                 MENTOREE\n");
    printf("===========================================\n");
    printf("          %s\n", title);
    printf("===========================================\n\n");
}

int register_mentee() {
    char username[STR_LEN];
    char password[STR_LEN];
    char confirm_password[STR_LEN];
    char name[STR_LEN];
    char email[STR_LEN];
    
    display_header("MENTEE REGISTRATION");
    
    getchar();
    printf("Full Name: ");
    fgets(name, STR_LEN, stdin);
    name[strcspn(name, "\n")] = 0; 
    

    printf("Email: ");

    getchar();
    fgets(email, STR_LEN, stdin);
    email[strcspn(email, "\n")] = 0;

    if (!is_valid_email(email)) {
        printf("\nInvalid email format. Registration failed.\n");
        return 0;
    }
    
    printf("Username: ");

    getchar();
    fgets(username, STR_LEN, stdin);
    username[strcspn(username, "\n")] = 0;
    if (username_exists(username)) {
        printf("\nUsername already exists. Please choose another one.\n");
        return 0;
    }
    
    printf("Password: ");
    fgets(password, STR_LEN, stdin);
    password[strcspn(password, "\n")] = 0;

    if (strlen(password) == 0) {
        printf("\nPassword cannot be empty. Registration failed.\n");
        return 0;
    }

    printf("Confirm Password: ");
    fgets(confirm_password, STR_LEN, stdin);
    confirm_password[strcspn(confirm_password, "\n")] = 0;

    // equal password check
    if (strcmp(password, confirm_password) != 0) {
        printf("\nPasswords do not match. Registration failed.\n");
        return 0;
    }
    
    add_user(username, password, name, email, ROLE_MENTEE);
    
    struct User* new_mentee = authenticate_user(username, password, ROLE_MENTEE);
    
    if (mentor->mentee_count < MAX_MENTEES) {
        mentor->mentees[mentor->mentee_count] = new_mentee;
        mentor->mentee_count++;
        printf("\nRegistration successful! You can now log in as a mentee.\n");
        return 1;

    } else {
        printf("\nCannot add more mentees.\n");
        return 0;
    }
}

int login_with_role(int role) {
    char username[STR_LEN];
    char password[STR_LEN];
    int attempts = 0;

    char header[13];
        if (role==ROLE_MENTOR){
            strcpy(header,"MENTOR LOGIN");
        }else{
            strcpy(header,"MENTEE LOGIN");
        }
        

    while (attempts < 3) {
        
        display_header(header);
        
        printf("Username: ");
        scanf("%s", username);
        
        printf("Password: ");
        scanf("%s", password);
        
        struct User* user = authenticate_user(username, password, role);

        if (user != NULL) {
            current_user = user;
            
            printf("\nLogin successful!\n");
            return 1;
        }
        
        printf("\nInvalid username or password. Please try again.\n");
        attempts++;
    }
    
    printf("\nToo many failed attempts.\n");
    return 0;
}

void main_menu() {
    int choice;
    
    while (1) {
        display_header("MAIN MENU");
        
        printf("[1] Login as Mentor\n");
        printf("[2] Login as Mentee\n");
        printf("[3] Sign Up as Mentee\n");
        printf("[4] Exit\n\n");
        
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        
        int succesful_login;
        switch (choice) {
            case 1:
                succesful_login = login_with_role(ROLE_MENTOR);
                if (succesful_login) {
                    mentor_menu();
                }
                break;
            case 2:
                succesful_login = login_with_role(ROLE_MENTEE);
                if (succesful_login) {
                    mentee_menu();
                }
                break;
            case 3:
                register_mentee();
                break;
            case 4:
                printf("\nThank you for using MENTOREE. Goodbye!\n");
                exit(0);
            case 5:
                printf("\nInvalid choice. Please enter a number between 1 and 4.\n");
        }
    }
}

void mentor_menu() {
    int choice;

    while (1) {
        display_header("MENTOR DASHBOARD");
        printf("Welcome, %s\n\n", current_user->name);
        printf("YOUR MENTEES:\n");
        printf("------------------------------------------\n");
        
        for (i = 0; i < mentor->mentee_count; i++) {
            printf("[%d] %s\n", i + 1, mentor->mentees[i]->name);
        }

        printf("\n[0] Logout\n\n");
        printf("Select a mentee (1-%d) or 0 to logout: ", mentor->mentee_count);

        scanf("%d", &choice);
        
        if (choice == 0) {
            current_user = NULL;
            return;

        } else if (choice >= 1 && choice <= mentor->mentee_count) {

            struct User* selected_mentee = mentor->mentees[choice - 1];
            
            while (1) {
                display_header("MENTEE MANAGEMENT");
                printf("Selected Mentee: %s\n\n", selected_mentee->name);
                
                printf("[1] Details\n");
                printf("[2] Progress Tracking\n");
                printf("[3] Meeting History\n");
                printf("[0] Back to Mentee List\n\n");
                printf("Enter your choice: ");
                
                scanf("%d", &choice);
                
                if (choice == 0) {
                    break;
                } else if (choice >= 1 && choice <= 3) {
                    switch (choice) {
                        case 1:
                            view_mentee_details(selected_mentee);
                            break;
                        case 2:
                            manage_tasks(selected_mentee);
                            break;
                        case 3:
                            manage_meetings(selected_mentee);
                            break;
                    }
                } else {
                    printf("\nInvalid choice. Please enter a number between 0 and 3.\n");
                }
            }
        } else {
            printf("\nInvalid choice. Please enter a number between 0 and %d.\n", mentor->mentee_count);
        }
    }
}

void mentee_menu() {
    int choice;
    
    while (1) {
        display_header("MENTEE DASHBOARD");
        printf("Welcome, %s\n\n", current_user->name);
        
        printf("[1] Details\n");
        printf("[2] Progress Tracking\n");
        printf("[3] Meeting History\n");
        printf("[0] Logout\n\n");
        
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        
        if (choice == 0) {
            current_user = NULL;
            return;
        } else if (choice >= 1 && choice <= 3) {
            switch (choice) {
                case 1:
                    view_mentee_details(current_user);
                    break;
                case 2:
                    manage_tasks(current_user);
                    break;
                case 3:
                    manage_meetings(current_user);
                    break;
            }
        } else {
            printf("\nInvalid choice. Please enter a number between 0 and 3.\n");
        }
    }
}

void view_mentee_details(struct User* mentee) {
    //add code
}

void edit_mentee_details(struct User* mentee) {
    //add code
}

void manage_tasks(struct User* mentee) {
    //add code
}

void view_tasks(struct User* mentee) {
    //add code
}

void add_task(struct User* mentee) {
    //add code
 }
 
 void edit_task(struct User* mentee) {
    //add code
 }
 
 void delete_task(struct User* mentee) {
    //add code
 }
 
 void manage_meetings(struct User* mentee) {
    //add code
 }
 
 void view_meetings(struct User* mentee) {
    //add code
 }
 
 void add_meeting(struct User* mentee) {
    //add code
 }
 
 void edit_meeting(struct User* mentee) {
    //add code
 }
 
 void delete_meeting(struct User* mentee) {
    //add code
 }
 
 int main() {
    sample_data();
    
    main_menu();
    return 0;
 }