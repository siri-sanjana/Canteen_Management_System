#include <stdio.h>
#include <string.h>

struct MenuItem {
    char name[50];
    float price;
    int quantity;
};
struct Feedback {
    char customerName[50];
    int quantityRating; // Rating from 1 to 5
    int qualityRating; // Rating from 1 to 5
    int serviceRating; // Rating from 1 to 5
    char comments[200];
};

void getFeedback(struct Feedback *feedback) {
    printf("Enter your name: ");
    scanf("%s", feedback->customerName);
    printf("Rate the quantity (1-5): ");
    scanf("%d", &feedback->quantityRating);
    printf("Rate the quality (1-5): ");
    scanf("%d", &feedback->qualityRating);
    printf("Rate the service (1-5): ");
    scanf("%d", &feedback->serviceRating);
    printf("Any additional comments: ");
    scanf(" %[^\n]s", feedback->comments); // Read until newline is found
}

void saveFeedbackToFile(struct Feedback *feedback) {
    FILE *file = fopen("feedback.csv", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    
     fprintf(file,"name,quantity(1-5),quality(1-5),service(1-5), comments \n", feedback->customerName, feedback->quantityRating, feedback->qualityRating, feedback->serviceRating, feedback->comments);
    fprintf(file, "%s,%d,%d,%d,%s\n", feedback->customerName, feedback->quantityRating, feedback->qualityRating, feedback->serviceRating, feedback->comments);

    fclose(file);
}

void loadMenuFromFile(struct MenuItem menu[], int *itemCount) {
    FILE *file = fopen("menu.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading. Creating a new file.\n");
        return;
    }

    *itemCount = 0;

    while (fscanf(file, "%49[^,],%f,%d\n", menu[*itemCount].name, &menu[*itemCount].price, &menu[*itemCount].quantity)==3) {
        (*itemCount)++;
    }

    fclose(file);
}

void saveMenuToFile(struct MenuItem menu[], int itemCount) {
    FILE *file = fopen("menu.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
int i;
    for (i = 0; i < itemCount; i++) {
        fprintf(file, "%s,%.2f,%d\n", menu[i].name, menu[i].price, menu[i].quantity);
    }

    fclose(file);
}

void displayMenu(struct MenuItem menu[], int itemCount) {
    printf("\nMenu:\n");
    int i;
    for (i = 0; i < itemCount; i++) {
    	if (menu[i].quantity!=0)
    	{
    		
		
        printf("%d. %s - Rs %.5f - Quantity: %d\n", i + 1, menu[i].name, menu[i].price, menu[i].quantity);
        
        if (menu[i].quantity < 5) {
            printf("Warning: Quantity of %s is low.\n", menu[i].name);
        }
        }
    }
}

void addItem(struct MenuItem menu[], int *itemCount) {
    printf("Enter the name of the new item: ");
    scanf("%s", menu[*itemCount].name);
    printf("Enter the price of the new item: ");
    scanf("%f", &menu[*itemCount].price);
    printf("Enter the quantity of the new item: ");
    scanf("%d", &menu[*itemCount].quantity);

    (*itemCount)++;
}

void searchItem(struct MenuItem menu[], int itemCount) {
    char searchItem[50];
    int found = 0;

    printf("Enter the name of the item to search: ");
    scanf("%s", searchItem);
int i;
    for (i = 0; i < itemCount; i++) {
        if (strcmp(menu[i].name, searchItem) == 0) {
            printf("%s is in the menu with a price of Rs %.5f\n", menu[i].name, menu[i].price);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("%s is not in the menu.\n", searchItem);
    }
}

void refillItem(struct MenuItem menu[], int itemCount) {
    int choice, quantity;

    displayMenu(menu, itemCount);

    printf("Enter the item number to refill: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > itemCount) {
        printf("Invalid choice. Please try again.\n");
    } else {
        printf("Enter the quantity to add: ");
        scanf("%d", &quantity);

        menu[choice - 1].quantity += quantity;
    }
}

void placeOrder(struct MenuItem menu[], int itemCount, char orders[][50], int orderQuantities[], float subtotals[], int *orderCount) {
    int choice, quantity;

    displayMenu(menu, itemCount);

    printf("Enter the item number to order (0 to finish): ");
    scanf("%d", &choice);

    while (choice != 0) {
        if (choice < 1 || choice > itemCount) {
            printf("Invalid choice. Please try again.\n");
        } else {
            printf("Enter the quantity: ");
            scanf("%d", &quantity);

            if (quantity > menu[choice - 1].quantity) {
                printf("Sorry, we only have %d of %s left in stock.\n", menu[choice - 1].quantity, menu[choice - 1].name);
            } else {
                strcpy(orders[*orderCount], menu[choice - 1].name);
                orderQuantities[*orderCount] = quantity;
                subtotals[*orderCount] = quantity * menu[choice - 1].price;

                menu[choice - 1].quantity -= quantity;

                (*orderCount)++;
            }
        }

        printf("Enter the item number to order (0 to finish): ");
        scanf("%d", &choice);
    }
}

void displayOrder(char orders[][50], int orderQuantities[], float subtotals[], int orderCount) {
    float total = 0;
    float tax;

    printf("\nOrder:\n");
    printf("%-20s|%-20s|%-20s|%-20s\n", "NAME", "PRICE FOR 1", "QUANTITY", "PRICE");
    printf("--------------------------------------------------------------------------------\n");
    int i;
	for ( i = 0; i < orderCount; i++) {
        printf("%-20s|%-20.2f|%-20d|%-20.2f\n", orders[i], subtotals[i] / orderQuantities[i], orderQuantities[i], subtotals[i]);
        total += subtotals[i];
    }
    tax = total * 0.12;
    total += tax;       

    printf("--------------------------------------------------------------------------------\n");
    printf("Tax: Rs%.2f\n", tax);
    printf("Total: Rs%.2f\n", total);
}

void deleteItem(struct MenuItem menu[], int *itemCount) {
    int choice, i;

    displayMenu(menu, *itemCount);

    printf("Enter the item number to delete: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > *itemCount) {
        printf("Invalid choice. Please try again.\n");
    } else {
        for (i = choice - 1; i < *itemCount - 1; i++) {
            strcpy(menu[i].name, menu[i + 1].name);
            menu[i].price = menu[i + 1].price;
            menu[i].quantity = menu[i + 1].quantity;
        }
        (*itemCount)--;
    }
}
void giveFeedback() {
    struct Feedback feedback;
    getFeedback(&feedback);
    saveFeedbackToFile(&feedback);
    printf("Thank you for your feedback!\n");
}
int main() {
    struct MenuItem menu[100];
    int itemCount = 0;
    char orders[100][50];
    int orderQuantities[100];
    float subtotals[100];
    int orderCount = 0;

    loadMenuFromFile(menu, &itemCount);

    int choice;
    do {
        printf("\nCanteen Management System\n");
        printf("1. Display Menu\n");
        printf("2. Add Item\n");
        printf("3. Refill Item\n");
        printf("4. Search Item\n");
        printf("5. Place Order\n");
        printf("6. Display Order\n");
        printf("7. Delete Item\n");
        printf("8. feedback \n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayMenu(menu, itemCount);
                break;
            case 2:
                addItem(menu, &itemCount);
                break;
            case 3:
                refillItem(menu, itemCount);
                break;
            case 4:
                searchItem(menu, itemCount);
                break;
            case 5:
                placeOrder(menu, itemCount, orders, orderQuantities, subtotals, &orderCount);
                break;
            case 6:
                displayOrder(orders, orderQuantities, subtotals, orderCount);
                break;
            case 7:
                deleteItem(menu, &itemCount);
                break;
                case 8:
                	 giveFeedback();
                break;
                	
            case 0:
                printf("Exiting the program. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    saveMenuToFile(menu, itemCount);

    return 0;
}
