#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "warehouse.h"

extern Pallet pallets[MAX_PALLETS];
extern int pallet_count;
extern PackagingQueue packaging_queue[MAX_PACKAGING_QUEUE];
extern int packaging_queue_count;

Machine machines[MAX_MACHINES];

void init_machines(void) {
    strcpy(machines[0].name, "Labeling");
    machines[0].machine_id = 1;
    strcpy(machines[0].status, "Idle");
    machines[0].current_pallet_id = 0;

    strcpy(machines[1].name, "Wrapping");
    machines[1].machine_id = 2;
    strcpy(machines[1].status, "Idle");
    machines[1].current_pallet_id = 0;

    strcpy(machines[2].name, "Inspection");
    machines[2].machine_id = 3;
    strcpy(machines[2].status, "Idle");
    machines[2].current_pallet_id = 0;
}

void packaging_menu(void) {
    int choice;
    for (;;) {
        clear_screen();
        printf("=== PACKAGING FACILITY ===\n"
               "1. Enqueue Pallet for Packaging\n"
               "2. Process Packaging\n"
               "3. List Packaging Queue\n"
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
            case 1: enqueue_pallet_for_packaging(); break;
            case 2: process_packaging(); break;
            case 3: list_packaging_queue(); wait_for_enter(); break;
            case 4: return;
            default: printf("Invalid choice.\n"); wait_for_enter();
        }
    }
}

void enqueue_pallet_for_packaging(void) {
    if (packaging_queue_count >= MAX_PACKAGING_QUEUE) {
        printf("Packaging queue is full.\n");
        wait_for_enter();
        return;
    }
    list_pallets();
    int pallet_id;
    printf("Enter pallet ID to package: ");
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
    if (strcmp(pallet->status, "Order Created") != 0 && strcmp(pallet->status, "Packed") != 0) {
        printf("Pallet must be in 'Order Created' or 'Packed' status.\n");
        wait_for_enter();
        return;
    }
    for (int i = 0; i < packaging_queue_count; i++) {
        if (packaging_queue[i].pallet_id == pallet_id) {
            printf("Pallet is already in the packaging queue.\n");
            wait_for_enter();
            return;
        }
    }
    PackagingQueue q;
    q.item_id = 0; // No item in packaging
    q.pallet_id = pallet_id;
    q.current_machine_id = 1; // Start with Labeling
    q.start_time = time(NULL);
    packaging_queue[packaging_queue_count++] = q;
    strcpy(pallet->status, "Packaging");
    printf("Pallet %d enqueued for packaging.\n", pallet_id);
    wait_for_enter();
}

void process_packaging(void) {
    if (packaging_queue_count == 0) {
        printf("Packaging queue is empty.\n");
        wait_for_enter();
        return;
    }
    int processed = 0;
    time_t now = time(NULL);
    for (int i = 0; i < packaging_queue_count; i++) {
        PackagingQueue *q = &packaging_queue[i];
        int pallet_idx = find_pallet_by_id(q->pallet_id);
        if (pallet_idx < 0) continue;
        Pallet *pallet = &pallets[pallet_idx];
        Machine *m = NULL;
        for (int j = 0; j < MAX_MACHINES; j++) {
            if (machines[j].machine_id == q->current_machine_id) {
                m = &machines[j];
                break;
            }
        }
        if (!m) continue;
        double elapsed = difftime(now, q->start_time);
        if (elapsed >= 10.0) { // Assume 10 seconds per machine
            strcpy(m->status, "Idle");
            m->current_pallet_id = 0;
            q->current_machine_id++;
            q->start_time = now;
            if (q->current_machine_id > MAX_MACHINES) {
                strcpy(pallet->status, "Packaged");
                for (int j = i; j < packaging_queue_count - 1; j++) {
                    packaging_queue[j] = packaging_queue[j + 1];
                }
                packaging_queue_count--;
                printf("Pallet %d completed packaging.\n", q->pallet_id);
                i--;
            } else {
                m = NULL;
                for (int j = 0; j < MAX_MACHINES; j++) {
                    if (machines[j].machine_id == q->current_machine_id) {
                        m = &machines[j];
                        break;
                    }
                }
                if (m) {
                    strcpy(m->status, "Processing");
                    m->current_pallet_id = q->pallet_id;
                    printf("Pallet %d moved to %s.\n", q->pallet_id, m->name);
                }
            }
            processed = 1;
        }
    }
    if (!processed) {
        printf("No pallets ready to advance in packaging.\n");
    }
    wait_for_enter();
}

void list_packaging_queue(void) {
    if (packaging_queue_count == 0) {
        printf("Packaging queue is empty.\n");
        wait_for_enter();
        return;
    }
    printf("\n%-8s %-8s %-10s\n", "Pallet ID", "Machine", "Time Elapsed");
    printf("%-8s %-8s %-10s\n", "--------", "-------", "----------");
    time_t now = time(NULL);
    for (int i = 0; i < packaging_queue_count; i++) {
        PackagingQueue *q = &packaging_queue[i];
        const char *machine_name = "Done";
        for (int j = 0; j < MAX_MACHINES; j++) {
            if (machines[j].machine_id == q->current_machine_id) {
                machine_name = machines[j].name;
                break;
            }
        }
        double elapsed = difftime(now, q->start_time);
        printf("%-8d %-8s %-10.1f\n", q->pallet_id, machine_name, elapsed);
    }
    wait_for_enter();
}
