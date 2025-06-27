#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <time.h>

#define MAX_WAREHOUSES 100
#define MAX_TRAILERS 1000
#define MAX_CUSTOMERS 100
#define MAX_PALLETS 1000
#define MAX_INVENTORY 1000
#define MAX_DRIVERS 100
#define MAX_ITEMS 1000
#define MAX_PACKAGING_QUEUE 100
#define MAX_DISPATCH_QUEUE 100
#define MAX_NAME_LEN 50
#define MAX_DESC_LEN 100
#define MAX_MACHINES 3

typedef struct {
    char id[4];
    char name[MAX_NAME_LEN];
    char location[MAX_NAME_LEN];
    int capacity;
    int current_trailers;
} Warehouse;

typedef struct {
    int id;
    char license_plate[20];
    char warehouse_id[4];
    char status[20];
    char customer_id[10];
} Trailer;

typedef struct {
    char id[10];
    char name[MAX_NAME_LEN];
    char contact[MAX_NAME_LEN];
    char address[MAX_NAME_LEN];
} Customer;

typedef struct {
    int id;
    char description[MAX_DESC_LEN];
    char customer_id[10];
    int quantity;
    int dimension_units;
    int weight_units;
    char status[20];
    int trailer_id;
} Pallet;

typedef struct {
    int id;
    char item_name[MAX_NAME_LEN];
    int quantity;
    char location[MAX_NAME_LEN];
    char warehouse_id[4];
} Inventory;

typedef struct {
    int id;
    char item_name[MAX_NAME_LEN];
    int quantity;
    char location[MAX_NAME_LEN];
    char warehouse_id[4];
    char status[20];
} Item;

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int assigned_trailer_id;
    float hours_worked;
    char status[20];
    int is_on_shift;
    time_t shift_start;
    time_t shift_end;
} Driver;

typedef struct {
    int machine_id;
    char name[20];
    char status[20];
    int current_pallet_id;
} Machine;

typedef struct {
    int item_id;
    int pallet_id;
    int current_machine_id;
    time_t start_time;
} PackagingQueue;

typedef struct {
    int item_id;
    int pallet_id;
    int current_machine_id;
    time_t start_time;
} PackingQueue;

typedef struct {
    int pallet_id;
    char destination[MAX_NAME_LEN];
    time_t start_time;
} DispatchQueue;

// Function prototypes
void clear_screen(void);
void wait_for_enter(void);
void save_data(void);
void load_data(void);
void list_warehouses(void);
void list_trailers(void);
void list_trailers_by_warehouse(void);
void reassign_trailer(void);
void view_trailer_details(int trailer_id);
void add_warehouse(void);
void edit_warehouse(void);
void delete_warehouse(void);
void add_trailer(void);
void edit_trailer(void);
void delete_trailer(void);
void add_customer(void);
void list_customers(void);
void edit_customer(void);
void delete_customer(void);
void add_pallet(void);
void list_pallets(void);
void edit_pallet(void);
void delete_pallet(void);
void add_inventory_item(void);
void list_inventory(void);
void edit_inventory_item(void);
void delete_inventory_item(void);
void add_driver(void);
void list_drivers(void);
void assign_pallet_to_trailer(void);
void assign_driver_to_trailer(void);
void update_driver_hours(void);
void start_driver_shift(void);
void end_driver_shift(void);
void update_driver_shift_hours(Driver *d);
void check_driver_shifts(void);
void unassign_driver_from_trailer(void);
void warehouse_menu(void);
void trailer_menu(void);
void customer_menu(void);
void pallet_menu(void);
void inventory_menu(void);
void driver_menu(void);
void main_menu(void);
void init_machines(void);
void packaging_menu(void);
void enqueue_pallet_for_packaging(void);
void process_packaging(void);
void list_packaging_queue(void);
void packing_menu(void);
void enqueue_item_for_packing(void);
void process_packing(void);
void list_packing_queue(void);
void dispatch_menu(void);
void enqueue_pallet_for_dispatch(void);
void process_dispatch(void);
void list_dispatch_queue(void);
int find_warehouse_by_id(const char *id);
int find_trailer_by_id(int id);
int find_customer_by_id(const char *id);
int find_pallet_by_id(int id);
int find_item_by_id(int id);
int find_inventory_by_id(int id);
int find_driver_by_id(int id);

// External variable declarations
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
extern PackagingQueue packaging_queue[MAX_PACKAGING_QUEUE];
extern int packaging_queue_count;
extern PackingQueue packing_queue[MAX_PACKAGING_QUEUE];
extern int packing_queue_count;
extern DispatchQueue dispatch_queue[MAX_DISPATCH_QUEUE];
extern int dispatch_queue_count;

#endif
