 #include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "warehouse.h"

// External declarations for global arrays defined in warehouse_management.c
extern Warehouse warehouses[MAX_WAREHOUSES];
extern Trailer trailers[MAX_TRAILERS];
extern Customer customers[MAX_CUSTOMERS];
extern Pallet pallets[MAX_PALLETS];
extern Inventory inventory[MAX_INVENTORY];
extern Driver drivers[MAX_DRIVERS];
extern Item items[MAX_ITEMS];
extern int warehouse_count;
extern int trailer_count;
extern int customer_count;
extern int pallet_count;
extern int inventory_count;
extern int driver_count;
extern int item_count;

void save_data(void) {
    FILE *fp = fopen("warehouse_data.bin", "wb");
    if (!fp) {
        printf("Error saving data.\n");
        return;
    }
    fwrite(&warehouse_count, sizeof(int), 1, fp);
    fwrite(warehouses, sizeof(Warehouse), warehouse_count, fp);
    fwrite(&trailer_count, sizeof(int), 1, fp);
    fwrite(trailers, sizeof(Trailer), trailer_count, fp);
    fwrite(&customer_count, sizeof(int), 1, fp);
    fwrite(customers, sizeof(Customer), customer_count, fp);
    fwrite(&pallet_count, sizeof(int), 1, fp);
    fwrite(pallets, sizeof(Pallet), pallet_count, fp);
    fwrite(&inventory_count, sizeof(int), 1, fp);
    fwrite(inventory, sizeof(Inventory), inventory_count, fp);
    fwrite(&driver_count, sizeof(int), 1, fp);
    fwrite(drivers, sizeof(Driver), driver_count, fp);
    fwrite(&item_count, sizeof(int), 1, fp);
    fwrite(items, sizeof(Item), item_count, fp);
    fwrite(&packaging_queue_count, sizeof(int), 1, fp);
    fwrite(packaging_queue, sizeof(PackagingQueue), packaging_queue_count, fp);
    fwrite(&dispatch_queue_count, sizeof(int), 1, fp);
    fwrite(dispatch_queue, sizeof(DispatchQueue), dispatch_queue_count, fp);
    fclose(fp);
}

void load_data(void) {
    FILE *fp = fopen("warehouse_data.bin", "rb");
    if (!fp) {
        return;
    }
    fread(&warehouse_count, sizeof(int), 1, fp);
    fread(warehouses, sizeof(Warehouse), warehouse_count, fp);
    fread(&trailer_count, sizeof(int), 1, fp);
    fread(trailers, sizeof(Trailer), trailer_count, fp);
    fread(&customer_count, sizeof(int), 1, fp);
    fread(customers, sizeof(Customer), customer_count, fp);
    fread(&pallet_count, sizeof(int), 1, fp);
    fread(pallets, sizeof(Pallet), pallet_count, fp);
    fread(&inventory_count, sizeof(int), 1, fp);
    fread(inventory, sizeof(Inventory), inventory_count, fp);
    fread(&driver_count, sizeof(int), 1, fp);
    fread(drivers, sizeof(Driver), driver_count, fp);
    fread(&item_count, sizeof(int), 1, fp);
    fread(items, sizeof(Item), item_count, fp);
    fread(&packaging_queue_count, sizeof(int), 1, fp);
    fread(packaging_queue, sizeof(PackagingQueue), packaging_queue_count, fp);
    fread(&dispatch_queue_count, sizeof(int), 1, fp);
    fread(dispatch_queue, sizeof(DispatchQueue), dispatch_queue_count, fp);
    fclose(fp);
}

void list_warehouses(void) {
    if (!warehouse_count) {
        printf("No warehouses found.\n");
        return;
    }
    printf("\n%-4s %-20s %-20s %-10s %-10s\n",
           "ID", "Name", "Location", "Capacity", "Trailers");
    printf("%-4s %-20s %-20s %-10s %-10s\n",
           "---", "----", "--------", "--------", "--------");
    for (int i = 0; i < warehouse_count; i++) {
        Warehouse *w = &warehouses[i];
        printf("%-4s %-20s %-20s %-10d %-10d\n",
               w->id, w->name, w->location, w->capacity, w->current_trailers);
    }
}

void list_trailers(void) {
    if (!trailer_count) {
        printf("No trailers found.\n");
        return;
    }
    printf("\n%-4s %-20s %-4s %-12s %-10s\n",
           "ID", "License Plate", "Wh", "Status", "Customer");
    printf("%-4s %-20s %-4s %-12s %-10s\n",
           "---", "-------------", "--", "------", "--------");
    for (int i = 0; i < trailer_count; i++) {
        Trailer *t = &trailers[i];
        printf("%-4d %-20s %-4s %-12s %-10s\n",
               t->id, t->license_plate, t->warehouse_id, t->status, t->customer_id);
    }
}

void list_trailers_by_warehouse(void) {
    list_warehouses();
    char id[4];
    printf("Enter warehouse ID: ");
    scanf("%3s", id);
    while (getchar() != '\n');
    int idx = find_warehouse_by_id(id);
    if (idx < 0) {
        printf("Warehouse not found.\n");
        wait_for_enter();
        return;
    }
    printf("\nTrailers in warehouse %s:\n", id);
    printf("%-4s %-20s %-12s %-10s\n",
           "ID", "License Plate", "Status", "Customer");
    printf("%-4s %-20s %-12s %-10s\n",
           "---", "-------------", "------", "--------");
    int found = 0;
    for (int i = 0; i < trailer_count; i++) {
        if (strcmp(trailers[i].warehouse_id, id) == 0) {
            printf("%-4d %-20s %-12s %-10s\n",
                   trailers[i].id, trailers[i].license_plate,
                   trailers[i].status, trailers[i].customer_id);
            found = 1;
        }
    }
    if (!found) {
        printf("No trailers found in warehouse %s.\n", id);
    }
    wait_for_enter();
}

void reassign_trailer(void) {
    list_trailers();
    int trailer_id;
    printf("Enter trailer ID to reassign: ");
    if (scanf("%d", &trailer_id) != 1) {
        printf("Invalid ID.\n");
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
    Trailer *t = &trailers[trailer_idx];
    char old_warehouse_id[4];
    strcpy(old_warehouse_id, t->warehouse_id);
    if (strcmp(old_warehouse_id, "0") != 0) {
        int old_warehouse_idx = find_warehouse_by_id(old_warehouse_id);
        if (old_warehouse_idx >= 0) {
            warehouses[old_warehouse_idx].current_trailers--;
        }
    }
    list_warehouses();
    printf("Enter new warehouse ID (0 to unassign): ");
    char new_warehouse_id[4];
    scanf("%3s", new_warehouse_id);
    while (getchar() != '\n');
    if (strcmp(new_warehouse_id, "0") != 0) {
        int warehouse_idx = find_warehouse_by_id(new_warehouse_id);
        if (warehouse_idx < 0) {
            printf("Invalid warehouse ID.\n");
            wait_for_enter();
            return;
        }
        Warehouse *w = &warehouses[warehouse_idx];
        if (w->current_trailers >= w->capacity) {
            printf("Warehouse at capacity.\n");
            wait_for_enter();
            return;
        }
        w->current_trailers++;
    }
    strcpy(t->warehouse_id, new_warehouse_id);
    printf("Trailer %d reassigned to warehouse %s.\n", trailer_id, new_warehouse_id);
    wait_for_enter();
}

void view_trailer_details(int trailer_id) {
    int idx = find_trailer_by_id(trailer_id);
    if (idx < 0) {
        printf("Trailer not found.\n");
        return;
    }
    Trailer *t = &trailers[idx];
    printf("\nTrailer Details:\n");
    printf("ID: %d\n", t->id);
    printf("License Plate: %s\n", t->license_plate);
    printf("Warehouse ID: %s\n", t->warehouse_id);
    printf("Status: %s\n", t->status);
    printf("Customer ID: %s\n", t->customer_id);
    printf("Assigned Pallets:\n");
    int found = 0;
    for (int i = 0; i < pallet_count; i++) {
        if (pallets[i].trailer_id == trailer_id) {
            printf("- Pallet ID %d: %s\n", pallets[i].id, pallets[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No pallets assigned.\n");
    }
}
