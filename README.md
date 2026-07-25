# Stock and Inventory Management System (SIMS) 📦

[![Language: C](https://img.shields.io/badge/Language-C-00599C.svg?style=flat&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: Mac & Windows](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%2011-blue.svg)](https://github.com/labib-0/SIMS_C)
[![Algorithm: Binary Search](https://img.shields.io/badge/Search-Binary%20Search%20O(log%20N)-brightgreen.svg)](https://github.com/labib-0/SIMS_C)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A high-performance, modular, menu-driven command-line **Stock and Inventory Management System (SIMS)** written in pure **C**. Developed as part of the **Software Development Capstone Project (CEP2)** at Daffodil International University.

---

## 🚀 Key Features

- **🔐 Role-Based Access Control (RBAC)**:
  - **Administrator (`A`)**: Full system access, User management, Product CRUD, Stock management, Data backup/restore, Audit log analysis.
  - **Store Manager (`M`)**: Product search/viewing, Stock arrival updates, Low-stock alerts, Restock approvals, Transaction history, Business reports.
  - **Sales Staff (`S`)**: Sales transaction processing, Product search, Low-stock alerts, Restock requests, Personal transaction history.

- **⚡ High Performance & Binary Search**:
  - Implements $O(\log N)$ **Binary Search** algorithms across sorted memory arrays for instant User ID and Product ID lookups.

- **🛒 Interactive Sales Processing**:
  - Direct catalog product listing table and quick ID selection.
  - Instant stock deduction upon sale completion.
  - Automatic timestamped recording in `transactions.csv`.
  - Automatic low-stock warnings when inventory drops below `MinStock`.

- **📦 Product & Stock Management**:
  - Simple, clean sequential ID generation (`P001`, `P002`, `P003`...).
  - Integrated stock replenishment requests and approval workflows.
  - Unified **Low Stock & Restock Alerts** report.

- **📊 Reports & Business Analytics**:
  - **Inventory Report**: Product counts, total stock, and financial valuation.
  - **Sales Summary Report**: Total revenue and units sold for a date range (`DD/MM/YYYY`).
  - **Performance Report**: Top-selling products ranked by sales volume.
  - **File Export**: 1-click export of any report to `.txt` files (`inventory_report.txt`, `sales_report.txt`, `performance_report.txt`).

- **🛡️ Security & Audit Logging**:
  - Standard password hashing algorithm for credential storage.
  - In-app password change for logged-in users.
  - Password recovery mechanism (`Forgot Password`) via User ID and Date of Birth verification.
  - Comprehensive, real-time audit trail in `audit_log.txt`.

- **💾 Data Backup & Restoration**:
  - 1-click backup of all CSV databases and logs into `backups/`.
  - Restoration capability to recover system databases from backup files.

---

## 🔑 Default Administrator Credentials

Upon initial launch, if no users exist in `users.csv`, the system automatically initializes a default Admin account:

| Role | User ID | Password | Date of Birth |
|---|---|---|---|
| **Administrator** | `A100` | `Admin123` | `01/01/2000` |

---

## 🏗️ Project Architecture & File Structure

The project follows a clean, decoupled, multi-module architecture:

```text
SIMS_C/
├── main.c           # Entry point, database initializations, and role menu dispatcher
├── utility.c        # Centralized utilities, date/time formatting, & Binary Search algorithms
├── auth.c           # Password hashing, validation, login, and password recovery
├── admin.c          # Administrator Dashboard & User Management features
├── manager.c        # Store Manager Dashboard
├── staff.c          # Sales Staff Dashboard
├── inventory.c      # Product CRUD, stock arrival, alerts & restock workflows
├── transaction.c    # Interactive sales processing & transaction history
├── reports.c        # Inventory, Sales, Performance reporting & audit log viewer
│
├── users.csv        # Registered users database
├── products.csv     # Product inventory database
├── transactions.csv # Sales transaction log
├── audit_log.txt    # System audit trail
└── SIMS_Architecture_Roadmap_Bangla.pdf # Architecture guide & roadmap (Bangla)
```

---

## 💻 Building and Running the System

### Prerequisites
- Any standard C compiler (**GCC** or **Clang**).
- Compatible with **macOS** and **Windows 11** / Linux.

### 1. Compilation
Compile all modular source files using `gcc`:

```bash
gcc -Wall main.c utility.c auth.c inventory.c transaction.c reports.c admin.c manager.c staff.c -o sims
```

### 2. Execution
Run the compiled executable:

```bash
./sims
```

On Windows (Command Prompt / PowerShell):
```cmd
sims.exe
```

---

## 🎓 Academic & Team Information

- **Course Name**: Software Development Capstone Project (CEP2)
- **Project Name**: Stock and Inventory Management System
- **Semester**: Summer 2026
- **Batch**: 45 | **Section**: C1 | **Group**: 01 (`temp8`)
- **Institution**: Daffodil International University
- **Course Teacher**: Jafrin Iqbal Chowdhury (Lecturer)

### Team Members
| Sl. | Student Name | Student ID |
|---|---|---|
| 1. | **Labib Ul Hasan** | `252-35-031` |
| 2. | **Masuk Al Aff** | `252-35-099` |
| 3. | **Rim Akter** | `252-35-167` |

---

## 📜 License

This project is open source and available under the [MIT License](LICENSE).
