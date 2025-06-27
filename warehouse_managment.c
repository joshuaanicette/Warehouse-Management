#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "warehouse.h"

Warehouse warehouses[MAX_WAREHOUSES];
Trailer trailers[MAX_TRAILERS];
Customer customers[MAX_CUSTOMERS];
Pallet pallets[MAX_PALLETS];
Inventory inventory[MAX_INVENTORY];
Driver drivers[MAX_DRIVERS];
Item items[MAX_ITEMS];
PackagingQueue packaging_queue[MAX_PACKAGING_QUEUE];
PackingQueue packing_queue[MAX_PACKAGING_QUEUE];
DispatchQueue dispatch_queue[MAX_DISPATCH_QUEUE];
int warehouse_count = 0;
int trailer_count = 0;
int customer_count = 0;
int pallet_count = 0;
int inventory_count = 0;
int driver_count = 0;
int item_count = 0;
int packaging_queue_count = 0;
int packing_queue_count = 0;
int dispatch_queue_count = 0;

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void wait_for_enter(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
}

int find_warehouse_by_id(const char *id) {
    for (int i = 0; i < warehouse_count; i++) {
        if (strcmp(warehouses[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int find_trailer_by_id(int id) {
    for (int i = 0; i < trailer_count; i++) {
        if (trailers[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_customer_by_id(const char *id) {
    for (int i = 0; i < customer_count; i++) {
        if (strcmp(customers[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int find_pallet_by_id(int id) {
    for (int i = 0; i < pallet_count; i++) {
        if (pallets[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_item_by_id(int id) {
    for (int i = 0; i < item_count; i++) {
        if (items[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_inventory_by_id(int id) {
    return find_item_by_id(id);
}

int find_driver_by_id(int id) {
    for (int i = 0; i < driver_count; i++) {
        if (drivers[i].id == id) {
            return i;
        }
    }
    return -1;
}

void add_warehouse(void) {
    if (warehouse_count >= MAX_WAREHOUSES) {
        printf("Warehouse limit reached.\n");
        wait_for_enter();
        return;
    }
    Warehouse w;
    printf("Enter warehouse ID (3 chars): ");
    scanf("%3s", w.id);
    while (getchar() != '\n');
    if (find_warehouse_by_id(w.id) >= 0) {
        printf("Warehouse ID already exists.\n");
        wait_for_enter();
        return;
    }
    printf("Enter name: ");
    fgets(w.name, MAX_NAME_LEN, stdin);
    w.name[strcspn(w.name, "\n")] = '\0';
    printf("Enter location: ");
    fgets(w.location, MAX_NAME_LEN, stdin);
    w.location[strcspn(w.location, "\n")] = '\0';
    printf("Enter capacity: ");
    if (scanf("%d", &w.capacity) != 1 || w.capacity <= 0) {
        printf("Invalid capacity.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    w.current_trailers = 0;
    warehouses[warehouse_count++] = w;
    printf("Warehouse added.\n");
    wait_for_enter();
}

void edit_warehouse(void) {
    list_warehouses();
    char id[4];
    printf("Enter warehouse ID to edit: ");
    scanf("%3s", id);
    while (getchar() != '\n');
    int idx = find_warehouse_by_id(id);
    if (idx < 0) {
        printf("Warehouse not found.\n");
        wait_for_enter();
        return;
    }
    Warehouse *w = &warehouses[idx];
    printf("Enter new name (current: %s): ", w->name);
    fgets(w->name, MAX_NAME_LEN, stdin);
    w->name[strcspn(w->name, "\n")] = '\0';
    printf("Enter new location (current: %s): ", w->location);
    fgets(w->location, MAX_NAME_LEN, stdin);
    w->location[strcspn(w->location, "\n")] = '\0';
    printf("Enter new capacity (current: %d): ", w->capacity);
    int new_capacity;
    if (scanf("%d", &new_capacity) != 1 || new_capacity < w->current_trailers) {
        printf("Invalid capacity or less than current trailers (%d).\n", w->current_trailers);
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    w->capacity = new_capacity;
    printf("Warehouse updated.\n");
    wait_for_enter();
}

void delete_warehouse(void) {
    list_warehouses();
    char id[4];
    printf("Enter warehouse ID to delete: ");
    scanf("%3s", id);
    while (getchar() != '\n');
    int idx = find_warehouse_by_id(id);
    if (idx < 0) {
        printf("Warehouse not found.\n");
        wait_for_enter();
        return;
    }
    if (warehouses[idx].current_trailers > 0) {
        printf("Cannot delete warehouse with assigned trailers.\n");
        wait_for_enter();
        return;
    }
    for (int i = idx; i < warehouse_count - 1; i++) {
        warehouses[i] = warehouses[i + 1];
    }
    warehouse_count--;
    printf("Warehouse deleted.\n");
    wait_for_enter();
}

void add_trailer(void) {
    if (trailer_count >= MAX_TRAILERS) {
        printf("Trailer limit reached.\n");
        wait_for_enter();
        return;
    }
    Trailer t;
    printf("Enter trailer ID: ");
    if (scanf("%d", &t.id) != 1 || find_trailer_by_id(t.id) >= 0) {
        printf("Invalid or duplicate trailer ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter license plate: ");
    fgets(t.license_plate, 20, stdin);
    t.license_plate[strcspn(t.license_plate, "\n")] = '\0';
    list_warehouses();
    printf("Enter warehouse ID (0 to unassign): ");
    scanf("%3s", t.warehouse_id);
    while (getchar() != '\n');
    if (strcmp(t.warehouse_id, "0") != 0) {
        int idx = find_warehouse_by_id(t.warehouse_id);
        if (idx < 0 || warehouses[idx].current_trailers >= warehouses[idx].capacity) {
            printf("Invalid warehouse ID or warehouse at capacity.\n");
            wait_for_enter();
            return;
        }
        warehouses[idx].current_trailers++;
    }
    strcpy(t.status, "Available");
    strcpy(t.customer_id, "0");
    trailers[trailer_count++] = t;
    printf("Trailer added.\n");
    wait_for_enter();
}

void edit_trailer(void) {
    list_trailers();
    int id;
    printf("Enter trailer ID to edit: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_trailer_by_id(id);
    if (idx < 0) {
        printf("Trailer not found.\n");
        wait_for_enter();
        return;
    }
    Trailer *t = &trailers[idx];
    printf("Enter new license plate (current: %s): ", t->license_plate);
    fgets(t->license_plate, 20, stdin);
    t->license_plate[strcspn(t->license_plate, "\n")] = '\0';
    printf("Trailer updated.\n");
    wait_for_enter();
}

void delete_trailer(void) {
    list_trailers();
    int id;
    printf("Enter trailer ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_trailer_by_id(id);
    if (idx < 0) {
        printf("Trailer not found.\n");
        wait_for_enter();
        return;
    }
    Trailer *t = &trailers[idx];
    if (strcmp(t->warehouse_id, "0") != 0) {
        int w_idx = find_warehouse_by_id(t->warehouse_id);
        if (w_idx >= 0) {
            warehouses[w_idx].current_trailers--;
        }
    }
    for (int i = 0; i < pallet_count; i++) {
        if (pallets[i].trailer_id == id) {
            pallets[i].trailer_id = 0;
        }
    }
    for (int i = idx; i < trailer_count - 1; i++) {
        trailers[i] = trailers[i + 1];
    }
    trailer_count--;
    printf("Trailer deleted.\n");
    wait_for_enter();
}

void add_customer(void) {
    if (customer_count >= MAX_CUSTOMERS) {
        printf("Customer limit reached.\n");
        wait_for_enter();
        return;
    }
    Customer c;
    printf("Enter customer ID (up to 9 chars): ");
    scanf("%9s", c.id);
    while (getchar() != '\n');
    if (find_customer_by_id(c.id) >= 0) {
        printf("Customer ID already exists.\n");
        wait_for_enter();
        return;
    }
    printf("Enter name: ");
    fgets(c.name, MAX_NAME_LEN, stdin);
    c.name[strcspn(c.name, "\n")] = '\0';
    printf("Enter contact: ");
    fgets(c.contact, MAX_NAME_LEN, stdin);
    c.contact[strcspn(c.contact, "\n")] = '\0';
    printf("Enter address: ");
    fgets(c.address, MAX_NAME_LEN, stdin);
    c.address[strcspn(c.address, "\n")] = '\0';
    customers[customer_count++] = c;
    printf("Customer added.\n");
    wait_for_enter();
}

void list_customers(void) {
    if (!customer_count) {
        printf("No customers found.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-10s %-20s %-20s %-20s\n",
           "ID", "Name", "Contact", "Address");
    printf("%-10s %-20s %-20s %-20s\n",
           "---------", "----", "-------", "-------");
    for (int i = 0; i < customer_count; i++) {
        Customer *c = &customers[i];
        printf("%-10s %-20s %-20s %-20s\n",
               c->id, c->name, c->contact, c->address);
    }
    wait_for_enter();
}

void edit_customer(void) {
    list_customers();
    char id[10];
    printf("Enter customer ID to edit: ");
    scanf("%9s", id);
    while (getchar() != '\n');
    int idx = find_customer_by_id(id);
    if (idx < 0) {
        printf("Customer not found.\n");
        wait_for_enter();
        return;
    }
    Customer *c = &customers[idx];
    printf("Enter new name (current: %s): ", c->name);
    fgets(c->name, MAX_NAME_LEN, stdin);
    c->name[strcspn(c->name, "\n")] = '\0';
    printf("Enter new contact (current: %s): ", c->contact);
    fgets(c->contact, MAX_NAME_LEN, stdin);
    c->contact[strcspn(c->contact, "\n")] = '\0';
    printf("Enter new address (current: %s): ", c->address);
    fgets(c->address, MAX_NAME_LEN, stdin);
    c->address[strcspn(c->address, "\n")] = '\0';
    printf("Customer updated.\n");
    wait_for_enter();
}

void delete_customer(void) {
    list_customers();
    char id[10];
    printf("Enter customer ID to delete: ");
    scanf("%9s", id);
    while (getchar() != '\n');
    int idx = find_customer_by_id(id);
    if (idx < 0) {
        printf("Customer not found.\n");
        wait_for_enter();
        return;
    }
    for (int i = 0; i < pallet_count; i++) {
        if (strcmp(pallets[i].customer_id, id) == 0) {
            printf("Cannot delete customer with assigned pallets.\n");
            wait_for_enter();
            return;
        }
    }
    for (int i = idx; i < customer_count - 1; i++) {
        customers[i] = customers[i + 1];
    }
    customer_count--;
    printf("Customer deleted.\n");
    wait_for_enter();
}

void add_pallet(void) {
    if (pallet_count >= MAX_PALLETS) {
        printf("Pallet limit reached.\n");
        wait_for_enter();
        return;
    }
    Pallet p;
    printf("Enter pallet ID: ");
    if (scanf("%d", &p.id) != 1 || find_pallet_by_id(p.id) >= 0) {
        printf("Invalid or duplicate pallet ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter description: ");
    fgets(p.description, MAX_DESC_LEN, stdin);
    p.description[strcspn(p.description, "\n")] = '\0';
    list_customers();
    printf("Enter customer ID: ");
    scanf("%9s", p.customer_id);
    while (getchar() != '\n');
    if (find_customer_by_id(p.customer_id) < 0) {
        printf("Customer not found.\n");
        wait_for_enter();
        return;
    }
    printf("Enter quantity: ");
    if (scanf("%d", &p.quantity) != 1 || p.quantity <= 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    printf("Enter dimension units: ");
    if (scanf("%d", &p.dimension_units) != 1 || p.dimension_units <= 0) {
        printf("Invalid dimension.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    printf("Enter weight units: ");
    if (scanf("%d", &p.weight_units) != 1 || p.weight_units <= 0) {
        printf("Invalid weight.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    strcpy(p.status, "Order Created");
    p.trailer_id = 0;
    pallets[pallet_count++] = p;
    printf("Pallet added.\n");
    wait_for_enter();
}

void list_pallets(void) {
    if (!pallet_count) {
        printf("No pallets found.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-4s %-30s %-10s %-8s %-10s %-10s %-12s %-4s\n",
           "ID", "Description", "Customer", "Qty", "Dim", "Weight", "Status", "Trlr");
    printf("%-4s %-30s %-10s %-8s %-10s %-10s %-12s %-4s\n",
           "---", "-----------", "--------", "---", "---", "------", "------", "----");
    for (int i = 0; i < pallet_count; i++) {
        Pallet *p = &pallets[i];
        printf("%-4d %-30s %-10s %-8d %-10d %-10d %-12s %-4d\n",
               p->id, p->description, p->customer_id, p->quantity,
               p->dimension_units, p->weight_units, p->status, p->trailer_id);
    }
    wait_for_enter();
}

void edit_pallet(void) {
    list_pallets();
    int id;
    printf("Enter pallet ID to edit: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_pallet_by_id(id);
    if (idx < 0) {
        printf("Pallet not found.\n");
        wait_for_enter();
        return;
    }
    Pallet *p = &pallets[idx];
    printf("Enter new description (current: %s): ", p->description);
    fgets(p->description, MAX_DESC_LEN, stdin);
    p->description[strcspn(p->description, "\n")] = '\0';
    printf("Enter new quantity (current: %d): ", p->quantity);
    if (scanf("%d", &p->quantity) != 1 || p->quantity <= 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    printf("Enter new dimension units (current: %d): ", p->dimension_units);
    if (scanf("%d", &p->dimension_units) != 1 || p->dimension_units <= 0) {
        printf("Invalid dimension.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    printf("Enter new weight units (current: %d): ", p->weight_units);
    if (scanf("%d", &p->weight_units) != 1 || p->weight_units <= 0) {
        printf("Invalid weight.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Pallet updated.\n");
    wait_for_enter();
}

void delete_pallet(void) {
    list_pallets();
    int id;
    printf("Enter pallet ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_pallet_by_id(id);
    if (idx < 0) {
        printf("Pallet not found.\n");
        wait_for_enter();
        return;
    }
    for (int i = idx; i < pallet_count - 1; i++) {
        pallets[i] = pallets[i + 1];
    }
    pallet_count--;
    printf("Pallet deleted.\n");
    wait_for_enter();
}

void add_inventory_item(void) {
    if (inventory_count >= MAX_INVENTORY) {
        printf("Inventory limit reached.\n");
        wait_for_enter();
        return;
    }
    Inventory inv;
    printf("Enter item ID: ");
    if (scanf("%d", &inv.id) != 1 || find_inventory_by_id(inv.id) >= 0) {
        printf("Invalid or duplicate item ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter item name: ");
    fgets(inv.item_name, MAX_NAME_LEN, stdin);
    inv.item_name[strcspn(inv.item_name, "\n")] = '\0';
    printf("Enter quantity: ");
    if (scanf("%d", &inv.quantity) != 1 || inv.quantity <= 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter location: ");
    fgets(inv.location, MAX_NAME_LEN, stdin);
    inv.location[strcspn(inv.location, "\n")] = '\0';
    list_warehouses();
    printf("Enter warehouse ID: ");
    scanf("%3s", inv.warehouse_id);
    while (getchar() != '\n');
    if (find_warehouse_by_id(inv.warehouse_id) < 0) {
        printf("Warehouse not found.\n");
        wait_for_enter();
        return;
    }
    inventory[inventory_count++] = inv;
    // Add to items for packing
    Item item;
    item.id = inv.id;
    strcpy(item.item_name, inv.item_name);
    item.quantity = inv.quantity;
    strcpy(item.location, inv.location);
    strcpy(item.warehouse_id, inv.warehouse_id);
    strcpy(item.status, "Stored");
    items[inventory_count - 1] = item;
    item_count = inventory_count;
    printf("Inventory item added.\n");
    wait_for_enter();
}

void list_inventory(void) {
    if (!inventory_count) {
        printf("No inventory items found.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-4s %-20s %-8s %-20s %-4s\n",
           "ID", "Item Name", "Qty", "Location", "Wh");
    printf("%-4s %-20s %-8s %-20s %-4s\n",
           "---", "---------", "---", "--------", "--");
    for (int i = 0; i < inventory_count; i++) {
        Inventory *inv = &inventory[i];
        printf("%-4d %-20s %-8d %-20s %-4s\n",
               inv->id, inv->item_name, inv->quantity, inv->location, inv->warehouse_id);
    }
    wait_for_enter();
}

void edit_inventory_item(void) {
    list_inventory();
    int id;
    printf("Enter item ID to edit: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_inventory_by_id(id);
    if (idx < 0) {
        printf("Item not found.\n");
        wait_for_enter();
        return;
    }
    Inventory *inv = &inventory[idx];
    printf("Enter new item name (current: %s): ", inv->item_name);
    fgets(inv->item_name, MAX_NAME_LEN, stdin);
    inv->item_name[strcspn(inv->item_name, "\n")] = '\0';
    printf("Enter new quantity (current: %d): ", inv->quantity);
    if (scanf("%d", &inv->quantity) != 1 || inv->quantity <= 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter new location (current: %s): ", inv->location);
    fgets(inv->location, MAX_NAME_LEN, stdin);
    inv->location[strcspn(inv->location, "\n")] = '\0';
    printf("Enter new warehouse ID (current: %s): ", inv->warehouse_id);
    scanf("%3s", inv->warehouse_id);
    while (getchar() != '\n');
    if (find_warehouse_by_id(inv->warehouse_id) < 0) {
        printf("Warehouse not found.\n");
        wait_for_enter();
        return;
    }
    // Update item
    Item *item = &items[idx];
    strcpy(item->item_name, inv->item_name);
    item->quantity = inv->quantity;
    strcpy(item->location, inv->location);
    strcpy(item->warehouse_id, inv->warehouse_id);
    printf("Inventory item updated.\n");
    wait_for_enter();
}

void delete_inventory_item(void) {
    list_inventory();
    int id;
    printf("Enter item ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_inventory_by_id(id);
    if (idx < 0) {
        printf("Item not found.\n");
        wait_for_enter();
        return;
    }
    for (int i = idx; i < inventory_count - 1; i++) {
        inventory[i] = inventory[i + 1];
        items[i] = items[i + 1];
    }
    inventory_count--;
    item_count--;
    printf("Inventory item deleted.\n");
    wait_for_enter();
}

void add_driver(void) {
    if (driver_count >= MAX_DRIVERS) {
        printf("Driver limit reached.\n");
        wait_for_enter();
        return;
    }
    Driver d;
    printf("Enter driver ID: ");
    if (scanf("%d", &d.id) != 1 || find_driver_by_id(d.id) >= 0) {
        printf("Invalid or duplicate driver ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    printf("Enter name: ");
    fgets(d.name, MAX_NAME_LEN, stdin);
    d.name[strcspn(d.name, "\n")] = '\0';
    d.assigned_trailer_id = 0;
    d.hours_worked = 0.0;
    strcpy(d.status, "Available");
    d.is_on_shift = 0;
    d.shift_start = 0;
    d.shift_end = 0;
    drivers[driver_count++] = d;
    printf("Driver added.\n");
    wait_for_enter();
}

void list_drivers(void) {
    if (!driver_count) {
        printf("No drivers found.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-4s %-20s %-8s %-6s %-12s %-6s\n",
           "ID", "Name", "Trailer", "Hours", "Status", "Shift");
    printf("%-4s %-20s %-8s %-6s %-12s %-6s\n",
           "---", "----", "-------", "-----", "------", "-----");
    for (int i = 0; i < driver_count; i++) {
        Driver *d = &drivers[i];
        printf("%-4d %-20s %-8d %-6.1f %-12s %-6s\n",
               d->id, d->name, d->assigned_trailer_id, d->hours_worked,
               d->status, d->is_on_shift ? "Yes" : "No");
    }
    wait_for_enter();
}

void assign_pallet_to_trailer(void) {
    list_pallets();
    int pallet_id;
    printf("Enter pallet ID: ");
    if (scanf("%d", &pallet_id) != 1) {
        printf("Invalid pallet ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int pallet_idx = find_pallet_by_id(pallet_id);
    if (pallet_idx < 0) {
        printf("Pallet not found.\n");
        wait_for_enter();
        return;
    }
    list_trailers();
    int trailer_id;
    printf("Enter trailer ID: ");
    if (scanf("%d", &trailer_id) != 1) {
        printf("Invalid trailer ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int trailer_idx = find_trailer_by_id(trailer_id);
    if (trailer_idx < 0) {
        printf("Trailer not found.\n");
        wait_for_enter();
        return;
    }
    pallets[pallet_idx].trailer_id = trailer_id;
    strcpy(trailers[trailer_idx].status, "Loaded");
    printf("Pallet %d assigned to trailer %d.\n", pallet_id, trailer_id);
    wait_for_enter();
}

void assign_driver_to_trailer(void) {
    list_drivers();
    int driver_id;
    printf("Enter driver ID: ");
    if (scanf("%d", &driver_id) != 1) {
        printf("Invalid driver ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int driver_idx = find_driver_by_id(driver_id);
    if (driver_idx < 0) {
        printf("Driver not found.\n");
        wait_for_enter();
        return;
    }
    Driver *d = &drivers[driver_idx];
    if (d->assigned_trailer_id != 0) {
        printf("Driver is already assigned to trailer %d.\n", d->assigned_trailer_id);
        wait_for_enter();
        return;
    }
    list_trailers();
    int trailer_id;
    printf("Enter trailer ID: ");
    if (scanf("%d", &trailer_id) != 1) {
        printf("Invalid trailer ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int trailer_idx = find_trailer_by_id(trailer_id);
    if (trailer_idx < 0) {
        printf("Trailer not found.\n");
        wait_for_enter();
        return;
    }
    d->assigned_trailer_id = trailer_id;
    strcpy(d->status, "Assigned");
    printf("Driver %d assigned to trailer %d.\n", driver_id, trailer_id);
    wait_for_enter();
}

void update_driver_hours(void) {
    list_drivers();
    int id;
    printf("Enter driver ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_driver_by_id(id);
    if (idx < 0) {
        printf("Driver not found.\n");
        wait_for_enter();
        return;
    }
    float hours;
    printf("Enter hours worked: ");
    if (scanf("%f", &hours) != 1 || hours < 0) {
        printf("Invalid hours.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    drivers[idx].hours_worked += hours;
    printf("Driver hours updated.\n");
    wait_for_enter();
}

void start_driver_shift(void) {
    list_drivers();
    int id;
    printf("Enter driver ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_driver_by_id(id);
    if (idx < 0) {
        printf("Driver not found.\n");
        wait_for_enter();
        return;
    }
    Driver *d = &drivers[idx];
    if (d->is_on_shift) {
        printf("Driver is already on shift.\n");
        wait_for_enter();
        return;
    }
    d->is_on_shift = 1;
    d->shift_start = time(NULL);
    strcpy(d->status, "On Shift");
    printf("Driver %d shift started.\n", id);
    wait_for_enter();
}

void end_driver_shift(void) {
    list_drivers();
    int id;
    printf("Enter driver ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_driver_by_id(id);
    if (idx < 0) {
        printf("Driver not found.\n");
        wait_for_enter();
        return;
    }
    Driver *d = &drivers[idx];
    if (!d->is_on_shift) {
        printf("Driver is not on shift.\n");
        wait_for_enter();
        return;
    }
    d->is_on_shift = 0;
    d->shift_end = time(NULL);
    update_driver_shift_hours(d);
    strcpy(d->status, d->assigned_trailer_id ? "Assigned" : "Available");
    printf("Driver %d shift ended.\n", id);
    wait_for_enter();
}

void update_driver_shift_hours(Driver *d) {
    if (d->shift_start && d->shift_end) {
        double hours = difftime(d->shift_end, d->shift_start) / 3600.0;
        d->hours_worked += hours;
        d->shift_start = 0;
        d->shift_end = 0;
    }
}

void check_driver_shifts(void) {
    time_t now = time(NULL);
    int found = 0;
    for (int i = 0; i < driver_count; i++) {
        Driver *d = &drivers[i];
        if (d->is_on_shift && difftime(now, d->shift_start) >= 8 * 3600) {
            printf("Driver %d (%s) has been on shift for over 8 hours.\n", d->id, d->name);
            found = 1;
        }
    }
    if (!found) {
        printf("No drivers on shift over 8 hours.\n");
    }
    wait_for_enter();
}

void unassign_driver_from_trailer(void) {
    list_drivers();
    int id;
    printf("Enter driver ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int idx = find_driver_by_id(id);
    if (idx < 0) {
        printf("Driver not found.\n");
        wait_for_enter();
        return;
    }
    Driver *d = &drivers[idx];
    if (d->assigned_trailer_id == 0) {
        printf("Driver is not assigned to any trailer.\n");
        wait_for_enter();
        return;
    }
    d->assigned_trailer_id = 0;
    strcpy(d->status, d->is_on_shift ? "On Shift" : "Available");
    printf("Driver %d unassigned from trailer.\n", id);
    wait_for_enter();
}

void warehouse_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== WAREHOUSE MANAGEMENT ===\n"
               "1. Add Warehouse\n"
               "2. Edit Warehouse\n"
               "3. Delete Warehouse\n"
               "4. List Warehouses\n"
               "5. List Trailers by Warehouse\n"
               "6. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_warehouse(); break;
            case 2: edit_warehouse(); break;
            case 3: delete_warehouse(); break;
            case 4: list_warehouses(); wait_for_enter(); break;
            case 5: list_trailers_by_warehouse(); break;
            case 6: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void trailer_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== TRAILER MANAGEMENT ===\n"
               "1. Add Trailer\n"
               "2. Edit Trailer\n"
               "3. Delete Trailer\n"
               "4. List Trailers\n"
               "5. Reassign Trailer\n"
               "6. View Trailer Details\n"
               "7. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_trailer(); break;
            case 2: edit_trailer(); break;
            case 3: delete_trailer(); break;
            case 4: list_trailers(); wait_for_enter(); break;
            case 5: reassign_trailer(); break;
            case 6: {
                int id;
                printf("Enter trailer ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    view_trailer_details(id);
                }
                wait_for_enter();
                break;
            }
            case 7: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void customer_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== CUSTOMER MANAGEMENT ===\n"
               "1. Add Customer\n"
               "2. Edit Customer\n"
               "3. Delete Customer\n"
               "4. List Customers\n"
               "5. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_customer(); break;
            case 2: edit_customer(); break;
            case 3: delete_customer(); break;
            case 4: list_customers(); break;
            case 5: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void pallet_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== PALLET ORDERS ===\n"
               "1. Add Pallet\n"
               "2. Edit Pallet\n"
               "3. Delete Pallet\n"
               "4. List Pallets\n"
               "5. Assign Pallet to Trailer\n"
               "6. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_pallet(); break;
            case 2: edit_pallet(); break;
            case 3: delete_pallet(); break;
            case 4: list_pallets(); break;
            case 5: assign_pallet_to_trailer(); break;
            case 6: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void inventory_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== INVENTORY MANAGEMENT ===\n"
               "1. Add Inventory Item\n"
               "2. Edit Inventory Item\n"
               "3. Delete Inventory Item\n"
               "4. List Inventory\n"
               "5. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_inventory_item(); break;
            case 2: edit_inventory_item(); break;
            case 3: delete_inventory_item(); break;
            case 4: list_inventory(); break;
            case 5: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void driver_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== DRIVER MANAGEMENT ===\n"
               "1. Add Driver\n"
               "2. List Drivers\n"
               "3. Assign Driver to Trailer\n"
               "4. Unassign Driver from Trailer\n"
               "5. Update Driver Hours\n"
               "6. Start Driver Shift\n"
               "7. End Driver Shift\n"
               "8. Check Driver Shifts\n"
               "9. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: add_driver(); break;
            case 2: list_drivers(); break;
            case 3: assign_driver_to_trailer(); break;
            case 4: unassign_driver_from_trailer(); break;
            case 5: update_driver_hours(); break;
            case 6: start_driver_shift(); break;
            case 7: end_driver_shift(); break;
            case 8: check_driver_shifts(); break;
            case 9: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void main_menu(void) {
    init_machines();
    load_data();
    int choice;
    for (;;) {
        clear_screen();
        printf("=== WAREHOUSE MANAGEMENT SYSTEM ===\n"
               "1. Warehouse Management\n"
               "2. Trailer Management\n"
               "3. Customer Management\n"
               "4. Pallet Orders\n"
               "5. Inventory Management\n"
               "6. Driver Management\n"
               "7. Packaging Facility\n"
               "8. Packing Facility\n"
               "9. Dispatch Facility\n"
               "10. Exit\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: warehouse_menu(); break;
            case 2: trailer_menu(); break;
            case 3: customer_menu(); break;
            case 4: pallet_menu(); break;
            case 5: inventory_menu(); break;
            case 6: driver_menu(); break;
            case 7: packaging_menu(); break;
            case 8: packing_menu(); break;
            case 9: dispatch_menu(); break;
            case 10: save_data(); printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

int main(void) {
    main_menu();
    return 0;
}
