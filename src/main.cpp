#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include "sqlite3.h"

// This macro is supplied by CMakeLists.txt.
// If it is not supplied, use the current folder as fallback.
#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR "."
#endif

bool executeSql(sqlite3* db, const std::string& sql)
{
    char* errorMessage = nullptr;

    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK)
    {
        std::cout << "SQL error: "
            << (errorMessage ? errorMessage : "Unknown SQL error")
            << std::endl;

        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

void createProductsTable(sqlite3* db)
{
    std::string sql =
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "price REAL NOT NULL"
        ");";

    if (executeSql(db, sql))
    {
        std::cout << "Products table is ready." << std::endl;
    }
}

void displayMenu()
{
    std::cout << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "      Inventory Management System" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "1. Add Product" << std::endl;
    std::cout << "2. Display All Products" << std::endl;
    std::cout << "3. Search Product by Name" << std::endl;
    std::cout << "4. Update Product" << std::endl;
    std::cout << "5. Delete Product" << std::endl;
    std::cout << "6. Add Stock" << std::endl;
    std::cout << "7. Reduce Stock" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "Please choose an option: ";
}

void addProduct(sqlite3* db)
{
    std::string name;
    std::string category;
    int quantity;
    double price;

    std::cout << "Enter product name: ";
    std::getline(std::cin, name);

    std::cout << "Enter category: ";
    std::getline(std::cin, category);

    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    std::cout << "Enter price: ";
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (quantity < 0 || price < 0)
    {
        std::cout << "Quantity and price cannot be negative." << std::endl;
        return;
    }

    std::string sql =
        "INSERT INTO products (name, category, quantity, price) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare insert statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, price);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        std::cout << "Product added successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to add product: "
            << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void displayAllProducts(sqlite3* db)
{
    std::string sql =
        "SELECT id, name, category, quantity, price "
        "FROM products ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare select statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    bool found = false;

    std::cout << std::endl;
    std::cout << "========== Product List ==========" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;

        int id = sqlite3_column_int(stmt, 0);
        std::string name =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string category =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int quantity = sqlite3_column_int(stmt, 3);
        double price = sqlite3_column_double(stmt, 4);

        std::cout << "Product ID: " << id << std::endl;
        std::cout << "Name      : " << name << std::endl;
        std::cout << "Category  : " << category << std::endl;
        std::cout << "Quantity  : " << quantity << std::endl;
        std::cout << "Price     : "
            << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "----------------------------------" << std::endl;
    }

    if (!found)
    {
        std::cout << "No products found." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void searchProductByName(sqlite3* db)
{
    std::string keyword;

    std::cout << "Enter product name or keyword: ";
    std::getline(std::cin, keyword);

    std::string searchText = "%" + keyword + "%";

    std::string sql =
        "SELECT id, name, category, quantity, price "
        "FROM products WHERE name LIKE ? ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare search statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, searchText.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;

    std::cout << std::endl;
    std::cout << "========== Search Results ==========" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;

        int id = sqlite3_column_int(stmt, 0);
        std::string name =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string category =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int quantity = sqlite3_column_int(stmt, 3);
        double price = sqlite3_column_double(stmt, 4);

        std::cout << "Product ID: " << id << std::endl;
        std::cout << "Name      : " << name << std::endl;
        std::cout << "Category  : " << category << std::endl;
        std::cout << "Quantity  : " << quantity << std::endl;
        std::cout << "Price     : "
            << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "----------------------------------" << std::endl;
    }

    if (!found)
    {
        std::cout << "No matching products found." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void updateProduct(sqlite3* db)
{
    int id;
    std::string name;
    std::string category;
    int quantity;
    double price;

    std::cout << "Enter product ID to update: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter new product name: ";
    std::getline(std::cin, name);

    std::cout << "Enter new category: ";
    std::getline(std::cin, category);

    std::cout << "Enter new quantity: ";
    std::cin >> quantity;

    std::cout << "Enter new price: ";
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (quantity < 0 || price < 0)
    {
        std::cout << "Quantity and price cannot be negative." << std::endl;
        return;
    }

    std::string sql =
        "UPDATE products "
        "SET name = ?, category = ?, quantity = ?, price = ? "
        "WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare update statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, price);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
        {
            std::cout << "Product updated successfully." << std::endl;
        }
        else
        {
            std::cout << "No product found with that ID." << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to update product: "
            << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void deleteProduct(sqlite3* db)
{
    int id;

    std::cout << "Enter product ID to delete: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string sql = "DELETE FROM products WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare delete statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
        {
            std::cout << "Product deleted successfully." << std::endl;
        }
        else
        {
            std::cout << "No product found with that ID." << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to delete product: "
            << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void addStock(sqlite3* db)
{
    int id;
    int amount;

    std::cout << "Enter product ID: ";
    std::cin >> id;

    std::cout << "Enter quantity to add: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (amount <= 0)
    {
        std::cout << "Quantity to add must be greater than zero." << std::endl;
        return;
    }

    std::string sql =
        "UPDATE products SET quantity = quantity + ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare stock update statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
        {
            std::cout << "Stock added successfully." << std::endl;
        }
        else
        {
            std::cout << "No product found with that ID." << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to add stock: "
            << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void reduceStock(sqlite3* db)
{
    int id;
    int amount;

    std::cout << "Enter product ID: ";
    std::cin >> id;

    std::cout << "Enter quantity to reduce: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (amount <= 0)
    {
        std::cout << "Quantity to reduce must be greater than zero." << std::endl;
        return;
    }

    std::string sql =
        "UPDATE products "
        "SET quantity = quantity - ? "
        "WHERE id = ? AND quantity >= ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to prepare stock reduction statement: "
            << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, amount);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
        {
            std::cout << "Stock reduced successfully." << std::endl;
        }
        else
        {
            std::cout << "Product not found or insufficient stock." << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to reduce stock: "
            << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

int main()
{
    sqlite3* db = nullptr;

    std::string dbPath =
        std::string(PROJECT_SOURCE_DIR) + "/data/inventory.db";

    std::cout << "Opening database: " << dbPath << std::endl;

    int result = sqlite3_open(dbPath.c_str(), &db);

    if (result != SQLITE_OK)
    {
        std::cout << "Error opening database: "
            << sqlite3_errmsg(db) << std::endl;

        sqlite3_close(db);
        return 1;
    }

    createProductsTable(db);

    int choice;

    while (true)
    {
        displayMenu();

        if (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            addProduct(db);
            break;

        case 2:
            displayAllProducts(db);
            break;

        case 3:
            searchProductByName(db);
            break;

        case 4:
            updateProduct(db);
            break;

        case 5:
            deleteProduct(db);
            break;

        case 6:
            addStock(db);
            break;

        case 7:
            reduceStock(db);
            break;

        case 8:
            sqlite3_close(db);
            std::cout << "Thank you for using the Inventory Management System."
                << std::endl;
            return 0;

        default:
            std::cout << "Invalid option. Please choose again." << std::endl;
        }
    }
}