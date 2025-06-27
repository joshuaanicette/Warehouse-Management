#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "warehouse.h"

extern Pallet pallets[MAX_PALLETS];
extern int pallet_count;
extern DispatchQueue dispatch_queue[MAX_DISPATCH_QUEUE];
extern int dispatch_queue_count;

void dispatch_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== DISPATCH FACILITY ===\n"
               "1. Enqueue Pallet for Dispatch\n"
               "2. Process Dispatch\n"
               "3. List Dispatch Queue\n"
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
            case 1: enqueue_pallet_for_dispatch(); break;
            case 2: process_dispatch(); break;
            case 3: list_dispatch_queue(); wait_for_enter(); break;
            case 4: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void enqueue_pallet_for_dispatch(void) {
    if (dispatch_queue_count >= MAX_DISPATCH_QUEUE) {
        printf("Dispatch queue is full.\n");
        wait_for_enter();
        return;
    }
    list_pallets();
    int pallet_id;
    printf("Enter pallet ID to dispatch: ");
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
    if (strcmp(pallet->status, "Packaged") != 0) {
        printf("Pallet must be in 'Packaged' status.\n");
        wait_for_enter();
        return;
    }
    for (int i = 0; i < dispatch_queue_count; i++) {
        if (dispatch_queue[i].pallet_id == pallet_id) {
            printf("Pallet is already in the dispatch queue.\n");
            wait_for_enter();
            return;
        }
    }
    char destination[MAX_NAME_LEN];
    printf("Enter destination: ");
    if (fgets(destination, MAX_NAME_LEN, stdin) == NULL) {
        printf("Error reading destination.\n");
        wait_for_enter();
        return;
    }
    destination[strcspn(destination, "\n")] = '\0';
    DispatchQueue q;
    q.pallet_id = pallet_id;
    strncpy(q.destination, destination, MAX_NAME_LEN - 1);
    q.destination[MAX_NAME_LEN - 1] = '\0';
    q.start_time = time(NULL);
    dispatch_queue[dispatch_queue_count++] = q;
    strcpy(pallet->status, "Dispatching");
    printf("Pallet %d enqueued for dispatch to %s.\n", pallet_id, destination);
    wait_for_enter();
}

void process_dispatch(void) {
    if (dispatch_queue_count == 0) {
        printf("Dispatch queue is empty.\n");
        wait_for_enter();
        return;
    }
    int processed = 0;
    time_t now = time(NULL);
    for (int i = 0; i < dispatch_queue_count; i++) {
        DispatchQueue *q = &dispatch_queue[i];
        int pallet_idx = find_pallet_by_id(q->pallet_id);
        if (pallet_idx < 0) continue;
        Pallet *pallet = &pallets[pallet_idx];
        double elapsed = difftime(now, q->start_time);
        if (elapsed >= 10.0) { // Assume 10 seconds for dispatch
            strcpy(pallet->status, "Dispatched");
            for (int j = i; j < dispatch_queue_count - 1; j++) {
                dispatch_queue[j] = dispatch_queue[j + 1];
            }
            dispatch_queue_count--;
            printf("Pallet %d dispatched to %s.\n", q->pallet_id, q->destination);
            i--;
            processed = 1;
        }
    }
    if (!processed) {
        printf("No pallets ready to dispatch.\n");
    }
    wait_for_enter();
}

void list_dispatch_queue(void) {
    if (dispatch_queue_count == 0) {
        printf("Dispatch queue is empty.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-8s %-20s %-10s\n", "Pallet ID", "Destination", "Time Elapsed");
    printf("%-8s %-20s %-10s\n", "--------", "--------------------", "----------");
    time_t now = time(NULL);
    for (int i = 0; i < dispatch_queue_count; i++) {
        DispatchQueue *q = &dispatch_queue[i];
        double elapsed = difftime(now, q->start_time);
        printf("%-8d %-20s %-10.1f\n", q->pallet_id, q->destination, elapsed);
    }
    wait_for_enter();
}
