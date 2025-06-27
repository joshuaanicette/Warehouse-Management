#include "warehouse.h"

Warehouse warehouses[MAX_WAREHOUSES];
Trailer   trailers[MAX_TRAILERS];
Customer  customers[MAX_CUSTOMERS];
Pallet    pallets[MAX_PALLETS];
Inventory inventory[MAX_INVENTORY];
Driver    drivers[MAX_DRIVERS];

int warehouse_count = 0;
int trailer_count   = 0;
int customer_count  = 0;
int pallet_count    = 0;
int inventory_count = 0;
int driver_count    = 0;

int main(void) {
    load_data();
    printf("=== Trailer Reassignment System ===\n"
           "Manage trailer assignments to warehouses\n\n");
    
    for (;;) {
        clear_screen();
        printf("1. Reassign Trailer to Warehouse\n"
               "2. List Trailers by Warehouse\n"
               "3. Exit\n"
               "Enter choice: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: reassign_trailer(); break;
            case 2: list_trailers_by_warehouse(); break;
            case 3: save_data(); printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
    return 0;
}
