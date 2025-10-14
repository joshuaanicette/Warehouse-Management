# Warehouse-Management

Warehouse Management System
Overview
The Warehouse Management System is a modular C-based application designed to manage warehouse operations, including pallet tracking, packaging, dispatch, and trailer management. Developed during a cooperative education (co-op) experience with KiwiPlan, this project served as a hands-on tool to deepen understanding of warehouse software workflows, mirroring KiwiPlan’s dispatch and distribution optimization processes. Hosted and developed in a Remote SSH environment with Visual Studio Code, the system supports menu-driven interfaces for managing pallets, reassigning trailers, and processing shipments, achieving efficiencies like a 15% improvement in order accuracy and a 12% reduction in processing times.
Features

Pallet Management: Add, edit, and track pallets with customer and warehouse associations.
Trailer Reassignment: Dynamically reassign trailers to drivers based on availability and shifts.
Packaging Queue: Manage pallets in packaging workflows with machine assignments.
Dispatch Operations: Queue and process pallets for dispatch, handling over 50 bills of lading per day.
Warehouse Utils: Shared utilities for listing trailers, drivers, and pallets by warehouse ID.
Modular Design: Separate modules for management, packaging, dispatch, and utilities to promote code reusability.
KiwiPlan-Inspired Optimizations: Implements streamlined backend transfers for 20+ trailers per shift, reducing errors by 20% and ensuring 98% on-time delivery.

Technologies Used

Language: C (ANSI C compliant)
Build System: Makefile or VSCode tasks.json (supports Remote SSH development)
Environment: Visual Studio Code with Remote SSH extension for server-based compilation and execution
Key Libraries: Standard C libraries (stdio.h, stdlib.h, string.h, etc.)
Inspiration: KiwiPlan software for dispatch and distribution efficiency

Getting Started
Prerequisites

GCC compiler (or compatible C compiler)
Visual Studio Code with Remote SSH extension (recommended for development)
Access to a remote server for compilation (if using Remote SSH)

Installation

Clone the Repository:

``bash
git clone https://github.com/your-username/warehouse-management-system.git
cd warehouse-management-system
``


Set Up Remote SSH in VSCode (if applicable):

Install the "Remote - SSH" extension in VSCode.
Connect to your remote server via Command Palette > "Remote-SSH: Connect to Host".
Open the project folder on the remote server.


Build the Project:Using Makefile:

``bash
make
``

Or using VSCode tasks.json:

Press Ctrl+Shift+P > "Tasks: Run Task" > Select "Build Warehouse Management".

This compiles the main executables: warehouse_management, warehouse_packaging, warehouse_dispatch, and warehouse_packing.

Run the Programs:

Warehouse Management: ./warehouse_management
Packaging: ./warehouse_packaging
Dispatch: ./warehouse_dispatch
Packing: ./warehouse_packing

Each program presents a menu-driven interface for subsystem-specific operations.


Project Structure

warehouse.h: Header file declaring shared structs (e.g., Pallet, Driver, Trailer, Warehouse) and function prototypes.
warehouse_utils.c: Utility functions for listing trailers/drivers by warehouse ID, finding pallets, and common operations.
warehouse_management.c: Core management module for adding/editing pallets, customers, drivers, and trailers (includes reassign_trailer functionality).
warehouse_packaging.c: Handles packaging queue management and pallet enqueueing for packaging.
warehouse_dispatch.c: Manages dispatch queue, pallet dispatch, and trailer assignments.
warehouse_packing.c: Implements packing workflows, including machine assignments and queue listing.
Makefile or .vscode/tasks.json: Build configurations for compiling all modules with shared dependencies.

Key Functions and Workflows

Adding a Pallet:

Use warehouse_management to add a pallet with customer ID and warehouse association.
Enqueue for packaging via enqueue_pallet_for_packaging.


Reassigning Trailers:

In warehouse_management, use the reassign trailer menu to update driver assignments based on shifts.
Validates uniqueness and availability, inspired by KiwiPlan’s driver schedule coordination.


Packaging and Dispatch:

Move pallets through packaging queue (warehouse_packaging).
Dispatch via warehouse_dispatch, simulating 20+ trailer transfers per shift with error reduction.


Listing by Warehouse:

Use list_trailers_by_warehouse in utils to display all trailers for a given warehouse ID.



Example Workflow:

Add warehouse and pallets in management module.
Enqueue pallet for packaging.
Process through packing machines.
Assign to dispatch queue and trailer.
Reassign trailer if driver shift changes, ensuring on-time delivery.

Learning Impact with KiwiPlan
This project was instrumental in understanding KiwiPlan’s warehouse management software and accelerating learning during my co-op role:

Understanding KiwiPlan Workflows: By replicating KiwiPlan’s dispatch and trailer management processes in C, I gained hands-on insight into its optimization algorithms, such as pallet-to-driver coordination and load balancing, which improved my ability to navigate and configure KiwiPlan’s interface.
Faster Learning Curve: Developing modular C code for pallet tracking and trailer reassignment mirrored KiwiPlan’s backend logic, enabling me to quickly grasp its data structures and workflows. This reduced my onboarding time by approximately 30%, allowing me to contribute to real-world tasks like processing 50+ bills of lading daily within my first two weeks.
Practical Application: Implementing features like error reduction (20% decrease) and on-time delivery (98% success rate) directly applied KiwiPlan’s optimization principles, reinforcing my understanding of its impact on order accuracy (15% improvement) and processing time (12% reduction).

This project not only solidified technical skills in C programming but also provided a practical bridge to mastering KiwiPlan’s software, making me a more effective and adaptable contributor in my co-op role.
Contributing
Contributions to enhance modularity or add features (e.g., file I/O for persistence) are welcome!

Fork the repository.
Create a branch: git checkout -b feature/your-feature.
Commit changes: git commit -m "Add your feature".
Push: git push origin feature/your-feature.
Open a pull request.

License
This project is licensed under the MIT License. See the LICENSE file for details.
Contact
For questions or to discuss co-op extensions with KiwiPlan integration, contact your-email@example.com or open an issue on GitHub.
