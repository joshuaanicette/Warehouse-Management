#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "warehouse.h"

extern Pallet pallets[MAX_PALLETS];
extern int pallet_count;
extern Item items[MAX_ITEMS];
extern int item_count;
extern PackingQueue packing_queue[MAX_PACKAGING_QUEUE];
extern int packing_queue_count;

void list_items(void) {
    if (!item_count) {
        printf("No items found.\n");
        return;
    }
    printf("\n%-4s %-20s %-8s %-20s %-4s %-12s\n",
           "ID", "Item Name", "Qty", "Location", "Wh", "Status");
    printf("%-4s %-20s %-8s %-20s %-4s %-12s\n",
           "---", "---------", "---", "--------", "--", "------");
    for (int i = 0; i < item_count; i++) {
        Item *item = &items[i];
        printf("%-4d %-20s %-8d %-20s %-4s %-12s\n",
               item->id, item->item_name, item->quantity, item->location,
               item->warehouse_id, item->status);
    }
}

void packing_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== PACKING FACILITY ===\n"
               "1. Enqueue Item for Packing\n"
               "2. Process Packing\n"
               "3. List Packing Queue\n"
               "4. Back\n"
               "Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            wait_for_enter();
            continue;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: enqueue_item_for_packing(); break;
            case 2: process_packing(); break;
            case 3: list_packing_queue(); wait_for_enter(); break;
            case 4: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void enqueue_item_for_packing(void) {
    if (packing_queue_count >= MAX_PACKAGING_QUEUE) {
        printf("Packing queue is full.\n");
        wait_for_enter();
        return;
    }
    list_items();
    int item_id;
    printf("Enter item ID to pack: ");
    if (scanf("%d", &item_id) != 1) {
        printf("Invalid item ID.\n");
        while (getchar() != '\n');
        wait_for_enter();
        return;
    }
    while (getchar() != '\n');
    int item_idx = find_item_by_id(item_id);
    if (item_idx < 0) {
        printf("Item not found.\n");
        wait_for_enter();
        return;
    }
    Item *item = &items[item_idx];
    if (strcmp(item->status, "Stored") != 0) {
        printf("Item must be in 'Stored' status.\n");
        wait_for_enter();
        return;
    }
    list_pallets();
    int pallet_id;
    printf("Enter pallet ID to pack onto: ");
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
    Pallet *pallet = &pallets[pallet_idx];
    for (int i = 0; i < packing_queue_count; i++) {
        if (packing_queue[i].item_id == item_id) {
            printf("Item is already in the packing queue.\n");
            wait_for_enter();
            return;
        }
    }
    PackingQueue q;
    q.item_id = item_id;
    q.pallet_id = pallet_id;
    q.current_machine_id = 0; // No machine in packing
    q.start_time = time(NULL);
    packing_queue[packing_queue_count++] = q;
    strcpy(item->status, "Packing");
    printf("Item %d enqueued for packing onto pallet %d.\n", item_id, pallet_id);
    wait_for_enter();
}

void process_packing(void) {
    if (packing_queue_count == 0) {
        printf("Packing queue is empty.\n");
        wait_for_enter();
        return;
    }
    int processed = 0;
    time_t now = time(NULL);
    for (int i = 0; i < packing_queue_count; i++) {
        PackingQueue *q = &packing_queue[i];
        int item_idx = find_item_by_id(q->item_id);
        int pallet_idx = find_pallet_by_id(q->pallet_id);
        if (item_idx < 0 || pallet_idx < 0) continue;
        Item *item = &items[item_idx];
        Pallet *pallet = &pallets[pallet_idx];
        double elapsed = difftime(now, q->start_time);
        if (elapsed >= 10.0) { // Assume 10 seconds for packing
            strcpy(item->status, "Packed");
            for (int j = i; j < packing_queue_count - 1; j++) {
                packing_queue[j] = packing_queue[j + 1];
            }
            packing_queue_count--;
            printf("Item %d packed onto pallet %d.\n", q->item_id, q->pallet_id);
            i--;
            processed = 1;
        }
    }
    if (!processed) {
        printf("No items ready to complete packing.\n");
    }
    wait_for_enter();
}

void list_packing_queue(void) {
    if (packing_queue_count == 0) {
        printf("Packing queue is empty.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-8s %-8s %-10s\n", "Item ID", "Pallet ID", "Time Elapsed");
    printf("%-8s %-8s %-10s\n", "--------", "--------", "----------");
    time_t now = time(NULL);
    for (int i = 0; i < packing_queue_count; i++) {
        PackingQueue *q = &packing_queue[i];
        double elapsed = difftime(now, q->start_time);
        printf("%-8d %-8d %-10.1f\n", q->item_id, q->pallet_id, elapsed);
    }
    wait_for_enter();
}
