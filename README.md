# Chapter08_InventorySQLite

An inventory management system with SQLite database integration written in C.

## 📋 Overview

This is an inventory management system developed in C as part of Chapter 8 learning exercises. The project demonstrates database programming, SQL operations, and file management through a practical inventory tracking system using SQLite.

## 🛠️ Technology Stack

- **Language**: C
- **Database**: SQLite3
- **Build System**: CMake (recommended)

## 🚀 Getting Started

### Prerequisites

- C compiler (GCC or Clang)
- SQLite3 development libraries
- CMake 3.10 or higher (optional, but recommended)

### Installation

#### On Ubuntu/Debian:
```bash
sudo apt-get install libsqlite3-dev
```

#### On macOS:
```bash
brew install sqlite3
```

#### On Windows:
Download SQLite3 from [sqlite.org](https://www.sqlite.org/download.html)

### Building the Project

#### Using CMake (Recommended)

```bash
# Clone the repository
git clone https://github.com/liewvk/Chapter08_InventorySQLite.git
cd Chapter08_InventorySQLite

# Create a build directory
mkdir build
cd build

# Generate build files and compile
cmake ..
make
```

#### Using Direct Compilation

```bash
# Navigate to the project directory
cd Chapter08_InventorySQLite

# Compile all source files with SQLite3
gcc -std=c99 -o InventorySQLite src/*.c -lsqlite3
```

### Running the Application

```bash
./InventorySQLite
```

## 📁 Project Structure

The project is organized as follows:

```
Chapter08_InventorySQLite/
├── CMakeLists.txt          # CMake configuration file
├── src/                    # Source implementation files (.c)
├── include/                # Header files (.h)
├── database/               # SQLite database files
└── README.md               # This file
```

## ✨ Features

- **Product Management**: Add, update, and delete products
- **Inventory Tracking**: Monitor stock levels in real-time
- **Stock Alerts**: Generate low stock warnings
- **Database Operations**: CRUD operations using SQLite
- **Transaction History**: Track inventory changes over time
- **Search and Filter**: Find products by category or name
- **Reports**: Generate inventory and sales reports
- **Data Backup**: Export and import inventory data
- **User Authentication**: Secure access control (optional)

## 📝 Usage

### Basic Operations

The application supports the following operations:

1. **Add Product**: Add new items to the inventory
2. **Update Stock**: Adjust quantity on hand
3. **Delete Product**: Remove products from inventory
4. **Search Products**: Find items by name or category
5. **View Inventory**: Display all products and stock levels
6. **Check Low Stock**: Identify items below threshold
7. **Generate Reports**: Create inventory analysis reports
8. **Transaction History**: View inventory change history
9. **Export Data**: Backup inventory to file

## 📚 Key Concepts Demonstrated

- SQLite Database Operations (CREATE, SELECT, INSERT, UPDATE, DELETE)
- SQL Queries and Joins
- Prepared Statements for Security
- Database Connection Management
- Error Handling and Recovery
- Data Validation
- File Operations in C
- String Manipulation
- Dynamic Memory Management

## 🎯 Learning Objectives

By working through this project, you will learn:

- How to design and implement database schemas
- How to perform CRUD operations with SQLite
- SQL query optimization and indexing
- How to handle database errors and transactions
- Techniques for data validation and sanitization
- Methods for generating reports from database data
- How to structure C programs for database access

## 💡 Example Workflow

```
1. Start the application
2. Create product database
3. Add products to inventory
4. Update stock levels
5. Search for products
6. Check low stock items
7. Generate inventory report
8. Export data to file
9. Close application
```

## 🔧 Database Schema

The inventory system uses the following tables:

- **products**: Product information (ID, name, category, price)
- **inventory**: Stock levels and locations
- **transactions**: History of inventory changes
- **suppliers**: Supplier information (optional)

## ⚠️ Important Notes

- SQLite3 must be installed before building
- Database files are created automatically on first run
- Ensure proper file permissions for database access
- Use prepared statements to prevent SQL injection

## 📝 License

This project is currently unlicensed. Please refer to the repository for any licensing information.

## 👤 Author

[liewvk](https://github.com/liewvk)

## 🤝 Contributing

Contributions are welcome! Feel free to fork this repository and submit pull requests with improvements or additional features.

---

**Last Updated**: July 6, 2026
