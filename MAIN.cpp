#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_PRODUCTS = 100;

// Product class
class Product {
public:
    string name;
    int productNo;
    int quantity;
    double price;

    Product() : name(""), productNo(0), quantity(0), price(0.0) {}

    Product(string name, int productNo, int quantity, double price)
        : name(name), productNo(productNo), quantity(quantity), price(price) {}

    void display() const {
        cout << setw(15) << name
             << setw(15) << productNo
             << setw(15) << quantity
             << setw(15) << fixed << setprecision(2) << price << endl;
    }
};

// Inventory class
class Inventory {
private:
    Product products[MAX_PRODUCTS];
    int productCount = 0;
    string filename = "products.csv";

    void loadProductsFromFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string line, name;
        int productNo, quantity;
        double price;

        productCount = 0;

        while (getline(file, line) && productCount < MAX_PRODUCTS) {
            stringstream ss(line);
            getline(ss, name, ',');
            ss >> productNo;
            ss.ignore(); // Ignore the comma
            ss >> quantity;
            ss.ignore(); // Ignore the comma
            ss >> price;

            products[productCount++] = Product(name, productNo, quantity, price);
        }

        file.close();
    }

    // Function to save products to the CSV file
    void saveProductsToFile() const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        for (int i = 0; i < productCount; ++i) {
            file << products[i].name << ","
                 << products[i].productNo << ","
                 << products[i].quantity << ","
                 << products[i].price << endl;
        }

        file.close();
    }

public:
    Inventory() {
        loadProductsFromFile();
    }

    void addProduct(const Product& product) {
        if (productCount >= MAX_PRODUCTS) {
            cout << "Inventory full. Cannot add more products." << endl;
            return;
        }

        for (int i = 0; i < productCount; ++i) {
            if (products[i].productNo == product.productNo) {
                cout << "Product number already exists. Use update to change details." << endl;
                return;
            }
        }

        products[productCount++] = product;
        saveProductsToFile();
        cout << "Product added successfully." << endl;
    }

    void updateProduct(int productNo, const string& name, int quantity, double price) {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].productNo == productNo) {
                products[i].name = name;
                products[i].quantity = quantity;
                products[i].price = price;
                saveProductsToFile();
                cout << "Product updated successfully." << endl;
                return;
            }
        }

        cout << "Product not found. Cannot update non-existent product." << endl;
    }

    void deleteProduct(int productNo) {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].productNo == productNo) {
                for (int j = i; j < productCount - 1; ++j) {
                    products[j] = products[j + 1];
                }
                productCount--;
                saveProductsToFile();
                cout << "Product deleted successfully." << endl;
                return;
            }
        }

        cout << "Product not found. Cannot delete non-existent product." << endl;
    }

    void displayProducts() const {
        if (productCount == 0) {
            cout << "No products available." << endl;
            return;
        }

        cout << setw(15) << "Product Name"
             << setw(15) << "Product No"
             << setw(15) << "Quantity"
             << setw(15) << "Price" << endl;
        cout << "---------------------------------------------------------------" << endl;

        for (int i = 0; i < productCount; ++i) {
            products[i].display();
        }

        cout << endl << endl << endl;
    }

    Product* searchProductByNumber(int productNo) {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].productNo == productNo) {
                return &products[i];
            }
        }
        cout << "Product not found." << endl;
        return nullptr;
    }

    Product* searchProductByName(const string& name) {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].name == name) {
                return &products[i];
            }
        }
        cout << "Product not found." << endl;
        return nullptr;
    }

    const Product* searchProductByNumber(int productNo) const {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].productNo == productNo) {
                return &products[i];
            }
        }
        cout << "Product not found." << endl;
        return nullptr;
    }

    const Product* searchProductByName(const string& name) const {
        for (int i = 0; i < productCount; ++i) {
            if (products[i].name == name) {
                return &products[i];
            }
        }
        cout << "Product not found." << endl;
        return nullptr;
    }
};

// Cart class
class Cart {
private:
    Product cartItems[MAX_PRODUCTS];
    int cartCount = 0;

public:
    void addToCart(Inventory& inventory, int productNo, int quantity) {
        Product* product = inventory.searchProductByNumber(productNo);
        if (product && product->quantity >= quantity) {
            for (int i = 0; i < cartCount; ++i) {
                if (cartItems[i].productNo == productNo) {
                    cartItems[i].quantity += quantity;
                    cout << "Added to cart: " << product->name << " (Quantity: " << quantity << ")" << endl;
                    return;
                }
            }

            cartItems[cartCount] = *product;
            cartItems[cartCount].quantity = quantity;
            cartCount++;
            cout << "Added to cart: " << product->name << " (Quantity: " << quantity << ")" << endl;
        } else {
            cout << "Insufficient stock or product not available." << endl;
        }
    }

    void viewCart(const Inventory& inventory) const {
        if (cartCount == 0) {
            cout << "Cart is empty." << endl;
            return;
        }

        double total = 0.0;
        cout << setw(15) << "Product Name"
             << setw(15) << "Product No"
             << setw(15) << "Quantity"
             << setw(15) << "Price" << endl;
        cout << "---------------------------------------------------------------" << endl;

        for (int i = 0; i < cartCount; ++i) {
            const Product* product = inventory.searchProductByNumber(cartItems[i].productNo);
            if (product) {
                double price = product->price * cartItems[i].quantity;
                total += price;
                cout << setw(15) << product->name
                     << setw(15) << product->productNo
                     << setw(15) << cartItems[i].quantity
                     << setw(15) << fixed << setprecision(2) << price << endl;
            }
        }
        cout << "---------------------------------------------------------------" << endl;
        cout << "Total Amount: " << fixed << setprecision(2) << total << endl;
    }

    void checkout(Inventory& inventory) {
        if (cartCount == 0) {
            cout << "Cart is empty." << endl;
            return;
        }

        string name, email, address;
        cout << "Enter your name: ";
        cin.ignore();  // To ignore newline character left in the buffer
        getline(cin, name);
        cout << "Enter your email: ";
        getline(cin, email);
        cout << "Enter your address: ";
        getline(cin, address);
system("cls");
        cout << "Processing checkout..." << endl;

        double total = 0.0;
        for (int i = 0; i < cartCount; ++i) {
            Product* product = inventory.searchProductByNumber(cartItems[i].productNo);
            if (product && product->quantity >= cartItems[i].quantity) {
                double price = product->price * cartItems[i].quantity;
                total += price;
                product->quantity -= cartItems[i].quantity;
            } else {
                cout << "Checkout failed for product: " << cartItems[i].productNo << endl;
            }
        }

        cartCount = 0;

        cout << "Order Summary" << endl;
        cout << "---------------------------------------" << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Address: " << address << endl;
        cout << "Total Amount: Rs" << fixed << setprecision(2) << total << endl;
        cout << "Checkout completed successfully." << endl;
    }
    
};


class User {
protected:
    string username;
    string password;

public:
    User(const string& username, const string& password)
        : username(username), password(password) {}
};

// Admin class
class Admin : public User, public Inventory {
public:
    Admin(const string& username, const string& password)
        : User(username, password) {}

    bool login() {
        string enteredUsername, enteredPassword;
        cout << "Admin Login" << endl;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;
        return (enteredUsername == username && enteredPassword == password);
        system("cls");
    }

    void panel(Inventory& inventory) {
        if (!login()) {
            cout << "Invalid login credentials." << endl;
            return;
        }

        int choice;
        do { 
        
            cout << "Admin Panel" << endl;
            cout << "1. Add Product" << endl;
            cout << "2. Update Product" << endl;
            cout << "3. Delete Product" << endl;
            cout << "4. Display Products" << endl;
            cout << "5. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                addProduct(inventory);
                break;
            case 2:
                updateProduct(inventory);
                break;
            case 3:
                deleteProduct(inventory);
                break;
            case 4:
                inventory.displayProducts();
                break;
            case 5:
                cout << "Returning to main menu..." << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

private:
    void addProduct(Inventory& inventory) {
        string name;
        int productNo, quantity;
        double price;

        cout << "Enter product name: ";
        cin.ignore(); // Ignore newline character from the buffer
        getline(cin, name);
        cout << "Enter product number: ";
        cin >> productNo;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter price: ";
        cin >> price;

        Product product(name, productNo, quantity, price);
        inventory.addProduct(product);
    }

    void updateProduct(Inventory& inventory) {
        int productNo;
        string name;
        int quantity;
        double price;

        cout << "Enter product number to update: ";
        cin >> productNo;

        cout << "Enter new name: ";
        cin.ignore(); // Ignore newline character from the buffer
        getline(cin, name);
        cout << "Enter new quantity: ";
        cin >> quantity;
        cout << "Enter new price: ";
        cin >> price;

        inventory.updateProduct(productNo, name, quantity, price);
    }

    void deleteProduct(Inventory& inventory) {
        int productNo;
        cout << "Enter product number to delete: ";
        cin >> productNo;
        inventory.deleteProduct(productNo);
    }
};

// Customer class
class Customer : public User {
public:
    Customer(const string& username, const string& password)
        : User(username, password) {}

    void panel(Inventory& inventory, Cart& cart) {
        int choice;
        do {
            cout << "Customer Panel" << endl;
            cout << "1. View Products" << endl;
            cout << "2. Search Product" << endl;
            cout << "3. View Cart" << endl;
            cout << "4. Checkout" << endl;
            cout << "5. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                inventory.displayProducts();
                break;
            case 2:
                searchProduct(inventory, cart);
                break;
            case 3:
                cart.viewCart(inventory);
                break;
            case 4:
                cart.checkout(inventory);
                break;
            case 5:
                cout << "Returning to main menu..." << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

private:
    void searchProduct(Inventory& inventory, Cart& cart) {
        int searchType;
        cout << "Search by: \n 1. Product Number \n 2. Product Name" << endl;
        cout << "Enter choice: ";
        cin >> searchType;

        Product* product = nullptr;
        if (searchType == 1) {
            int productNo;
            cout << "Enter product number to search: ";
            cin >> productNo;
            product = inventory.searchProductByNumber(productNo);
        } else if (searchType == 2) {
            string productName;
            cout << "Enter product name to search: ";
            cin.ignore();
            getline(cin, productName);
            product = inventory.searchProductByName(productName);
        } else {
            cout << "Invalid search type." << endl;
            return;
        }

        if (product) {
            cout << "Product Found: " << endl;
            product->display();

            int choice, quantity;
            cout << "1. Add to Cart" << endl;
            cout << "2. Buy Now" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "Enter quantity to add to cart: ";
                cin >> quantity;
                cart.addToCart(inventory, product->productNo, quantity);
            } else if (choice == 2) {
                cout << "Enter quantity to purchase: ";
                cin >> quantity;
                cart.addToCart(inventory, product->productNo, quantity);
                cart.checkout(inventory);
            } else {
                cout << "Invalid choice." << endl;
            }
        }
    }
};

// Main function
int main() {
    Inventory inventory;
    Cart cart;

    Admin admin("admin", "admin123");
    Customer customer("customer", "cust123");

    int userType;
    do {
        cout << "\t\t\t\tWelcome to the E-commerce System\n" << endl;
        cout << "\t\t1. Admin Login" << endl;
        cout << "\t\t2. Customer Login" << endl;
        cout << "\t\t3. Exit" << endl;
        cout << "\tEnter user type (1, 2, or 3): ";
        cin >> userType;
system("cls");
        switch (userType) {
        case 1:
            admin.panel(inventory);
            break;
        case 2:
            customer.panel(inventory, cart);
            break;
        case 3:
            cout << "Exiting the system..." << endl;
            break;
        default:
            cout << "Invalid user type selected. Please try again." << endl;
        }
    } while (userType != 3);

    return 0;
}

