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
int is_valid_phone(char* phone);
int is_valid_digital_id(char* digital_id);
int is_valid_reg_no(char* reg_no);
int is_date_valid(char* date);
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
    
    struct Meeting_note* meeting1 = (struct Meeting_note*)malloc(sizeof(struct Meeting_note));
    strcpy(meeting1->date, "15-03-2025");
    strcpy(meeting1->summary, "Reviewed the mentee's recent achievements and identified areas for improvement. The mentor advised setting clearer priorities for upcoming tasks and encouraged the mentee to maintain a detailed to-do list. The mentee committed to updating their progress regularly.");
    meeting1->next = NULL;
    rajesh->meetings = meeting1;
    
    struct Meeting_note* meeting2 = (struct Meeting_note*)malloc(sizeof(struct Meeting_note));
    strcpy(meeting2->date, "01-03-2025");
    strcpy(meeting2->summary, "Focused on improving communication between the mentor and mentee. The mentor suggested scheduling brief weekly check-ins to track progress and address concerns more efficiently. Both agreed to maintain a shared document for tracking goals and milestones.");
    meeting2->next = NULL;
    meeting1->next = meeting2;
    
    struct Meeting_note* meeting3 = (struct Meeting_note*)malloc(sizeof(struct Meeting_note));
    strcpy(meeting3->date, "15-02-2025");
    strcpy(meeting3->summary, "Discussed long-term goals and career aspirations. The mentor provided guidance on industry trends and recommended resources for skill development. The mentee shared concerns about balancing academic responsibilities with extracurricular activities.");
    meeting3->next = NULL;
    meeting2->next = meeting3;
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

// returns 1 if valid email
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

int is_valid_phone(char* phone) {
    if (strlen(phone) != 10) return 0;
    
    for (i = 0; i < strlen(phone); i++) {
        if (!isdigit(phone[i])) return 0;
    }
    
    return 1;
}
\
int is_valid_digital_id(char* digital_id) {
    if (strlen(digital_id) != 7) return 0;
    
    for (i = 0; i < strlen(digital_id); i++) {
        if (!isdigit(digital_id[i])) return 0;
    }
    
    return 1;
}
\
int is_valid_reg_no(char* reg_no) {
    if (strlen(reg_no) != 13) return 0;
    
    for (i = 0; i < strlen(reg_no); i++) {
        if (!isdigit(reg_no[i])) return 0;
    }
    
    return 1;
}

// returns 1 if date is valid
int is_date_valid(char* date) {
    if (strlen(date) != 10 || date[2] != '-' || date[5] != '-')
        return 0;
    
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5){
            continue;
        } 
        if (!isdigit(date[i])){
            return 0;
        }
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
    display_header("MENTEE DETAILS");
    printf("Username: %s\n", mentee->username);
    printf("Name: %s\n", mentee->name);
    printf("Email: %s\n", mentee->email);
    printf("Phone: %s\n", mentee->phone);
    printf("Department: %s\n", mentee->department);
    printf("Year of Study: %s\n", mentee->year);
    printf("Digital ID: %s\n", mentee->digital_id);
    printf("Registration Number: %s\n\n", mentee->reg_no);
    printf("Emergency Contact:\n");
    printf("Parent Phone: %s\n", mentee->parent_phone);
    printf("Parent Email: %s\n\n", mentee->parent_email);
    
    //display options to modify only if user is mentor
    if (current_user->role == ROLE_MENTEE) {
        int choice;
        
        printf("[1] Edit My Details\n");
        printf("[0] Back\n\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 
        
        if (choice == 1) {
            edit_mentee_details(mentee);
        }
    } else {
        printf("Press Enter to continue...");
        getchar();
    }
}

void edit_mentee_details(struct User* mentee) {
    char input[STR_LEN];
    
    display_header("EDIT DETAILS");
    
    printf("Current Name: %s\n", mentee->name);
    printf("New Name (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        strcpy(mentee->name, input);
    }
    
    getchar();
    printf("\nCurrent Email: %s\n", mentee->email);
    printf("New Email (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_email(input)) {
            strcpy(mentee->email, input);
        } else {
            printf("Invalid email format. Email remains unchanged.\n");
        }
    }
    
    getchar();
    printf("\nCurrent Phone: %s\n", mentee->phone);
    printf("New Phone (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_phone(input)) {
            strcpy(mentee->phone, input);
        } else {
            printf("Invalid phone number. Phone remains unchanged.\n");
        }
    }
    
    getchar();
    printf("\nCurrent Department: %s\n", mentee->department);
    printf("New Department (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        strcpy(mentee->department, input);
    }
    
    getchar();
    printf("\nCurrent Year of Study: %s\n", mentee->year);
    printf("New Year of Study (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        strcpy(mentee->year, input);
    }
    
    getchar();
    printf("\nCurrent Digital ID: %s\n", mentee->digital_id);
    printf("New Digital ID (7 digits, or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_digital_id(input)) {
            strcpy(mentee->digital_id, input);
        } else {
            printf("Invalid Digital ID format. Digital ID remains unchanged.\n");
        }
    }
    
    getchar();
    printf("\nCurrent Registration Number: %s\n", mentee->reg_no);
    printf("New Registration Number (13 digits, or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_reg_no(input)) {
            strcpy(mentee->reg_no, input);
        } else {
            printf("Invalid Registration Number format. Registration Number remains unchanged.\n");
        }
    }
    
    getchar();
    printf("\nCurrent Parent Phone: %s\n", mentee->parent_phone);
    printf("New Parent Phone (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_phone(input)) {
            strcpy(mentee->parent_phone, input);
        } else {
            printf("Invalid phone number. Parent phone remains unchanged.\n");
        }
    }
    
    getchar();
    printf("\nCurrent Parent Email: %s\n", mentee->parent_email);
    printf("New Parent Email (or press Enter to keep current): ");
    scanf("%[^\n]", input); 

    if (strlen(input) > 0) {
        if (is_valid_email(input)) {
            strcpy(mentee->parent_email, input);
        } else {
            printf("Invalid email format. Parent email remains unchanged.\n");
        }
    }
    
    printf("\nDetails updated successfully!\n");
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
    int choice;
    
    while (1) {
        display_header("MEETING NOTES");
        
        view_meetings(mentee);
        
        printf("\nOptions:\n");
        if (current_user->role == ROLE_MENTOR) {
            printf("[1] Add New Meeting Note\n");
            printf("[2] Edit Meeting Note\n");
            printf("[3] Delete Meeting Note\n");
        }
        printf("[0] Back\n\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 0) {
            return;
        } else if (current_user->role == ROLE_MENTOR && choice >= 1 && choice <= 3) {
            switch (choice) {
                case 1:
                    add_meeting(mentee);
                    break;
                case 2:
                    edit_meeting(mentee);
                    break;
                case 3:
                    delete_meeting(mentee);
                    break;
            }
        } else {
            printf("\nInvalid choice. Please try again.\n");
        }
    }
 }
 
 void view_meetings(struct User* mentee) {
    struct Meeting_note* current = mentee->meetings;
    
    printf("Meeting Notes for %s:\n", mentee->name);
    printf("------------------------------------------\n");
    
    if (current == NULL) {
        printf("No meeting notes recorded yet.\n");
        return;
    }
    
    int count = 1;
    while (current != NULL) {
        printf("Meeting %d - Date: %s\n", count++, current->date);
        printf("%s\n\n", current->summary);
        current = current->next;
    }
 }
 
 void add_meeting(struct User* mentee) {
    display_header("ADD MEETING NOTE");
    
    struct Meeting_note* new_meeting = (struct Meeting_note*)malloc(sizeof(struct Meeting_note));
    if (new_meeting == NULL) {
        printf("\nMemory allocation failed.\n");
        return;
    }
    
    char input[SUMMARY_LEN];
    
    do {
        printf("Enter meeting date (DD-MM-YYYY): ");
        scanf(" %[^\n]", input);
        
        if (!is_date_valid(input)) {
            printf("Invalid date format. Please use DD-MM-YYYY format.\n");
        }
    } while (!is_date_valid(input));
    
    strcpy(new_meeting->date, input);
    
    do {
        printf("Enter meeting summary: ");
        scanf(" %[^\n]", input);
        
        if (strlen(input) == 0) {
            printf("Meeting summary cannot be empty. Please enter a summary.\n");
        }
    } while (strlen(input) == 0);
    
    strcpy(new_meeting->summary, input);
    new_meeting->next = NULL;
    
    if (mentee->meetings == NULL) {
        mentee->meetings = new_meeting;
    } else {
        struct Meeting_note* current = mentee->meetings;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_meeting;
    }
    
    printf("\nMeeting note added successfully!\n");
 }
 
 void edit_meeting(struct User* mentee) {
    struct Meeting_note* current;
    int meeting_no, count = 1;
    char input[SUMMARY_LEN];
    
    view_meetings(mentee);
    
    if (mentee->meetings == NULL) {
        printf("\nNo meeting notes to edit.\n");
        return;
    }
    
    printf("\nEnter meeting number to edit (0 to cancel): ");
    scanf("%d", &meeting_no);
    
    if (meeting_no <= 0) {
        printf("Meeting addition cancelled.\n");
        return;
    }
    
    current = mentee->meetings;
    while (current != NULL && count < meeting_no) {
        current = current->next;
        count++;
    }
    
    if (current == NULL) {
        printf("\nInvalid meeting number.\n");
        return;
    }
    
    display_header("EDIT MEETING NOTE");
    
    printf("Current date: %s\n", current->date);
    printf("New date (DD-MM-YYYY, or press Enter to keep current): ");

    getchar();

    fgets(input, DATE_LEN, stdin);
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) > 0) {
        if (is_date_valid(input)) {
            strcpy(current->date, input);
        } else {
            printf("Invalid date format. Date remains unchanged.\n");
        }
    }
    
    printf("\nCurrent summary: %s\n", current->summary);
    printf("New summary (or press Enter to keep current): ");

    fgets(input, SUMMARY_LEN, stdin);
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) > 0) {
        strcpy(current->summary, input);
    }
    
    printf("\nMeeting note updated successfully!\n");
 }
 
 void delete_meeting(struct User* mentee) {
    int meeting_no, count = 1;
    char confirm;
    
    // Display meetings first
    view_meetings(mentee);
    
    if (mentee->meetings == NULL) {
        printf("\nNo meeting notes to delete.\n");
        return;
    }
    
    printf("\nEnter meeting number to delete (0 to cancel): ");
    scanf("%d", &meeting_no);
    
    if (meeting_no <= 0) {
        printf("Meeting deletion cancelled.\n");
        return;
    }
    getchar();
    printf("Are you sure you want to delete this meeting note? (y/n): ");
    scanf("%c", &confirm);
    
    if (tolower(confirm) != 'y') {
        printf("\nMeeting deletion cancelled.\n");
        return;
    }
    
    if (meeting_no == 1) {
        struct Meeting_note* temp = mentee->meetings;
        mentee->meetings = mentee->meetings->next;
        free(temp);
        printf("\nMeeting note deleted successfully!\n");
        return;
    }
    
    struct Meeting_note* prev = mentee->meetings;
    count = 1;
    while (prev->next != NULL && count < meeting_no - 1) {
        prev = prev->next;
        count++;
    }
    
    if (prev->next == NULL) {
        printf("\nInvalid meeting number.\n");
        return;
    }
    
    struct Meeting_note* temp = prev->next;
    prev->next = temp->next;
    free(temp);
    
    printf("\nMeeting note deleted successfully!\n");
 }
 
 int main() {
    sample_data();
    
    main_menu();
    return 0;
 }
