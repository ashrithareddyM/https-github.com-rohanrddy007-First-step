#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME "credentials.txt"
#define MAX_NAME 100
#define MAX_ITEMS 10
#define MAX_RESTAURANTS 3
#define ORDER_HISTORY_FILE "order_history.txt"

typedef struct {
    char name[MAX_NAME];
    float price;
} MenuItem;

typedef struct {
    char name[MAX_NAME];
    MenuItem menu[MAX_ITEMS];
    int num_items;
} Restaurant;

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} User;

typedef struct {
    MenuItem items[MAX_ITEMS];
    int quantity[MAX_ITEMS];
    int item_count;
} Order;

// Function to check if a username already exists in the credentials file
int checkUsernameExists(char *username) {
    FILE *file;
    char file_username[MAX_USERNAME], file_password[MAX_PASSWORD];

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        return 0; // No users registered yet
    }

    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
        if (strcmp(username, file_username) == 0) {
            fclose(file);
            return 1; // Username exists
        }
    }

    fclose(file);
    return 0; // Username doesn't exist
}

// Function to save the order history to a file
void saveOrderHistory(char *username, Order *order) {
    FILE *file;
    char filename[MAX_NAME];
    snprintf(filename, sizeof(filename), "%s_order_history.txt", username);

    file = fopen(filename, "a"); // Append to the file
    if (file == NULL) {
        printf("Error opening order history file for %s.\n", username);
        return;
    }

    fprintf(file, "\n--- New Order ---\n");
    for (int i = 0; i < order->item_count; i++) {
        fprintf(file, "%d x %s - $%.2f\n", order->quantity[i], order->items[i].name, order->quantity[i] * order->items[i].price);
    }
    fprintf(file, "Total Bill: $%.2f\n", order->item_count);

    fclose(file);
    printf("Order history saved!\n");
}

// Function to validate password (at least 8 characters, containing both letters and digits)
int validatePassword(char *password) {
    int len = strlen(password);
    if (len < 8) {
        printf("Password must be at least 8 characters long.\n");
        return 0;
    }

    int has_digit = 0, has_alpha = 0;
    for (int i = 0; i < len; i++) {
        if (isdigit(password[i])) has_digit = 1;
        if (isalpha(password[i])) has_alpha = 1;
    }

    if (!has_digit || !has_alpha) {
        printf("Password must contain both letters and digits.\n");
        return 0;
    }

    return 1; // Password is valid
}

// Function to sign up a new user
void signup() {
    FILE *file;
    char username[MAX_USERNAME], password[MAX_PASSWORD];

    printf("Enter a username: ");
    scanf("%s", username);

    if (checkUsernameExists(username)) {
        printf("Username already exists! Please try a different one.\n");
        return;
    }

    do {
        printf("Enter a password: ");
        scanf("%s", password);
    } while (!validatePassword(password));

    file = fopen(FILENAME, "a"); // Open file in append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s %s\n", username, password); // Write username and password to the file
    printf("Signup successful! You can now login.\n");

    fclose(file);
}

// Function to log in an existing user
int login(char *username) {
    FILE *file;
    char password[MAX_PASSWORD];
    char file_username[MAX_USERNAME], file_password[MAX_PASSWORD];
    int found = 0;

    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No users registered yet.\n");
        return 0;
    }

    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            printf("Login successful!\n");
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Invalid username or password.\n");
    }

    return found;
}

// Function to display the menu of a restaurant
void displayMenu(Restaurant *restaurant) {
    printf("\nMenu for %s:\n", restaurant->name);
    for (int i = 0; i < restaurant->num_items; i++) {
        printf("%d. %s - $%.2f\n", i + 1, restaurant->menu[i].name, restaurant->menu[i].price);
    }
}

// Function to add or delete food from the order
void modifyOrder(Order *order, Restaurant *restaurant) {
    int action, item_number, quantity;

    printf("\nWould you like to add or delete an item?\n");
    printf("1. Add Item\n2. Delete Item\n0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &action);

    if (action == 1) {
        printf("\nEnter the item number to add: ");
        scanf("%d", &item_number);
        if (item_number <= 0 || item_number > restaurant->num_items) {
            printf("Invalid item number.\n");
            return;
        }

        printf("Enter quantity: ");
        scanf("%d", &quantity);

        order->items[order->item_count] = restaurant->menu[item_number - 1];
        order->quantity[order->item_count] = quantity;
        order->item_count++;
    }
    else if (action == 2) {
        printf("\nEnter the item number to delete: ");
        scanf("%d", &item_number);
        if (item_number <= 0 || item_number > order->item_count) {
            printf("Invalid item number.\n");
            return;
        }

        // Shift items down to delete the selected one
        for (int i = item_number - 1; i < order->item_count - 1; i++) {
            order->items[i] = order->items[i + 1];
            order->quantity[i] = order->quantity[i + 1];
        }
        order->item_count--;
    }
}

// Function to initialize the restaurants and their menus
void initializeRestaurants(Restaurant restaurants[]) {
    // Restaurant 1
    strcpy(restaurants[0].name, "Pizza Place");
    restaurants[0].num_items = 3;
    strcpy(restaurants[0].menu[0].name, "Margherita Pizza");
    restaurants[0].menu[0].price = 12.99;
    strcpy(restaurants[0].menu[1].name, "Pepperoni Pizza");
    restaurants[0].menu[1].price = 14.99;
    strcpy(restaurants[0].menu[2].name, "Veggie Pizza");
    restaurants[0].menu[2].price = 13.99;

    // Restaurant 2
    strcpy(restaurants[1].name, "Burger Joint");
    restaurants[1].num_items = 3;
    strcpy(restaurants[1].menu[0].name, "Cheeseburger");
    restaurants[1].menu[0].price = 9.99;
    strcpy(restaurants[1].menu[1].name, "Bacon Burger");
    restaurants[1].menu[1].price = 11.99;
    strcpy(restaurants[1].menu[2].name, "Veggie Burger");
    restaurants[1].menu[2].price = 10.99;

    // Restaurant 3
    strcpy(restaurants[2].name, "Sushi Spot");
    restaurants[2].num_items = 3;
    strcpy(restaurants[2].menu[0].name, "California Roll");
    restaurants[2].menu[0].price = 8.99;
    strcpy(restaurants[2].menu[1].name, "Spicy Tuna Roll");
    restaurants[2].menu[1].price = 10.99;
    strcpy(restaurants[2].menu[2].name, "Salmon Roll");
    restaurants[2].menu[2].price = 9.99;
}

// Main function to run the combined login, signup, and food order management system
int main() {
    Restaurant restaurants[MAX_RESTAURANTS];
    int choice, restaurantChoice;
    float totalBill = 0.0;
    int loggedIn = 0;
    char username[MAX_USERNAME];
    Order currentOrder;
    currentOrder.item_count = 0;

    // Initialize restaurants and menus
    initializeRestaurants(restaurants);

    while (1) {
        printf("\n--- Food Order Management ---\n");

        if (!loggedIn) {
            printf("1. Login\n");
            printf("2. Signup\n");
        }
        printf("3. Order Food\n");
        printf("4. Modify Order\n");
        printf("5. View Bill\n");
        printf("6. Exit\n");

        if (!loggedIn) {
            printf("Enter your choice: ");
            scanf("%d", &choice);
        } else {
            printf("Enter your choice: ");
            scanf("%d", &choice);
        }

        switch (choice) {
            case 1:
                if (!loggedIn) {
                    loggedIn = login(username);
                } else {
                    printf("You are already logged in!\n");
                }
                break;
            case 2:
                if (!loggedIn) {
                    signup();
                } else {
                    printf("You are already logged in!\n");
                }
                break;
            case 3:
                if (loggedIn) {
                    printf("\nSelect a Restaurant:\n");
                    for (int i = 0; i < MAX_RESTAURANTS; i++) {
                        printf("%d. %s\n", i + 1, restaurants[i].name);
                    }
                    printf("Enter your choice: ");
                    scanf("%d", &restaurantChoice);

                    if (restaurantChoice > 0 && restaurantChoice <= MAX_RESTAURANTS) {
                        displayMenu(&restaurants[restaurantChoice - 1]);
                        // Add food to the order
                        int foodChoice, quantity;
                        printf("Enter the item number to order (0 to finish): ");
                        scanf("%d", &foodChoice);

                        while (foodChoice != 0) {
                            if (foodChoice > 0 && foodChoice <= restaurants[restaurantChoice - 1].num_items) {
                                printf("Enter quantity: ");
                                scanf("%d", &quantity);

                                currentOrder.items[currentOrder.item_count] = restaurants[restaurantChoice - 1].menu[foodChoice - 1];
                                currentOrder.quantity[currentOrder.item_count] = quantity;
                                currentOrder.item_count++;
                            } else {
                                printf("Invalid item number.\n");
                            }
                            printf("Enter the item number to order (0 to finish): ");
                            scanf("%d", &foodChoice);
                        }

                    } else {
                        printf("Invalid restaurant choice! Try again.\n");
                    }
                } else {
                    printf("You must log in first!\n");
                }
                break;

            case 4:
                if (loggedIn) {
                    modifyOrder(&currentOrder, &restaurants[restaurantChoice - 1]);
                } else {
                    printf("You must log in first!\n");
                }
                break;

            case 5:
                if (loggedIn) {
                    float bill = 0.0;
                    for (int i = 0; i < currentOrder.item_count; i++) {
                        bill += currentOrder.items[i].price * currentOrder.quantity[i];
                    }
                    printf("Total Bill: $%.2f\n", bill);
                    saveOrderHistory(username, &currentOrder);
                } else {
                    printf("You must log in first!\n");
                }
                break;

            case 6:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
