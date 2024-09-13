#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

using namespace std;

class Product
{
private:
    string productID;
    string name;
    double price;
    int stockQuantity;
    string category;

public:
    // Constructor
    Product(string id, string name, double price, int stock, string cat)
        : productID(id), name(name), price(price), stockQuantity(stock), category(cat) {}

    // Getters
    string getProductID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }
    string getCategory() const { return category; }

    // Setters
    void setPrice(double newPrice) { price = newPrice; }
    void setStockQuantity(int newStock) { stockQuantity = newStock; }
    void setCategory(const string& newCategory) { category = newCategory; }

    void updateStock(int quantity) { stockQuantity += quantity; }
};

class ShoppingCart
{
private:
    int cartID;
    vector<pair<Product, int>> items;
    double totalPrice;

public:
    // Constructor
    ShoppingCart(int id) : cartID(id), totalPrice(0) {}

    // To add and remove products
    void addProduct(const Product& product, int quantity = 1)
    {
        bool found = false;
        for (auto& item : items) {
            if (item.first.getProductID() == product.getProductID()) {
                item.second += quantity;
                found = true;
                break;
            }
        }
        if (!found) {
            items.push_back({product, quantity});
        }
        totalPrice += product.getPrice() * quantity;
    }

    void removeProduct(const string& productID)
    {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].first.getProductID() == productID) {
                totalPrice -= items[i].first.getPrice() * items[i].second;
                items.erase(items.begin() + i);
                break;
            }
        }
    }

    double getTotalPrice() const { return totalPrice; }
    int getCartID() const { return cartID; }
    vector<pair<Product, int>> getItems() const { return items; }
};

class Customer
{
private:
    int customerID;
    string name;
    string email;
    string address;

public:
    // Constructor
    Customer(int id, string name, string email, string address)
        : customerID(id), name(name), email(email), address(address) {}

    // Getters
    int getCustomerID() const { return customerID; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }

    // Setters
    void setAddress(const string& newAddress) { address = newAddress; }

    // To interact with the cart
    void addToCart(Product product, ShoppingCart& cart, int quantity = 1)
    {
        cart.addProduct(product, quantity);
    }

    void placeOrder(const ShoppingCart& cart)
    {
        cout << "Order placed! Total Price: " << cart.getTotalPrice() << endl;
    }
};

class Order
{
private:
    int orderID;
    Customer customer;
    string orderDate;
    vector<pair<Product, int>> products;
    double totalAmount;

public:
    // Constructor
    Order(int id, const Customer& cust, const string& date, const vector<pair<Product, int>>& prodList)
        : orderID(id), customer(cust), orderDate(date), products(prodList), totalAmount(0)
    {
        for (const auto& p : products) {
            totalAmount += p.first.getPrice() * p.second;
        }
    }

    // To generate invoice
    void generateInvoice() const
    {
        cout << "=========================" << endl;
        cout << "        Invoice          " << endl;
        cout << "=========================" << endl;
        cout << "Order ID:  " << orderID << endl;
        cout << "Order Date: " << orderDate << endl;
        cout << "Order Details:" << endl;
        cout << left;
        cout << setw(12) << "Product ID" << setw(25) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << "Total" << endl;
        cout << "----------------------------------------------" << endl;

        for (const auto& p : products) {
            cout << setw(12) << p.first.getProductID()
                 << setw(25) << p.first.getName()
                 << setw(10) << fixed << setprecision(2) << p.first.getPrice()
                 << setw(10) << p.second
                 << setw(10) << fixed << setprecision(2) << p.first.getPrice() * p.second << endl;
        }
        cout << "----------------------------------------------" << endl;
        cout << "Total Amount: " << fixed << setprecision(2) << totalAmount << endl;
        cout << "=========================" << endl;
    }
};

void viewProducts(const vector<Product>& products)
{
    cout << "=========================" << endl;
    cout << "        Products          " << endl;
    cout << "=========================" << endl;

    cout << left << setw(12) << "Product ID" << setw(25) << "Name" << setw(10) << "Price" << setw(10) << "Stock" << endl;  
    cout << "----------------------------------------------" << endl;

    // Group products by category and print headers
    map<string, vector<Product>> productsByCategory;
    for (const auto& product : products) {
        productsByCategory[product.getCategory()].push_back(product);
    }

    for (const auto& categoryPair : productsByCategory) {
        cout << endl << categoryPair.first << endl;
        cout << "-------------------------" << endl;

        for (const auto& product : categoryPair.second) {
            cout << setw(12) << product.getProductID()
                 << setw(25) << product.getName()
                 << setw(10) << fixed << setprecision(2) << product.getPrice()
                 << setw(10) << product.getStockQuantity() << endl;
        }

        cout << "-------------------------" << endl;
    }
}

void addProductToCart(vector<Product>& productList, ShoppingCart& cart)
{
    string productID;

    while (true) 
    {
        // Display all products
        viewProducts(productList);

        cout << "Enter the ID of the product you want to add to the shopping cart (or enter -1 to cancel): ";
        cin >> productID;

        // Check if the user wants to cancel
        if (productID == "-1") 
        {
            cout << "Operation canceled." << endl;
            return;  // Exit the function without adding a product
        }

        // Find the product
        auto it = find_if(productList.begin(), productList.end(), [&](const Product& p) 
        {
            return p.getProductID() == productID;
        });

        if (it != productList.end())
        {
            cart.addProduct(*it, 1); // Add product with default quantity 1
            cout << "Product added successfully!" << endl;
            break;  // Exit the loop after successful addition
        }
        else
        {
            cout << "Product not found!" << endl;
            cout << "Would you like to try again? (Y/N): ";
            char choice;
            cin >> choice;

            if (choice == 'N' || choice == 'n') {
                cout << "Operation canceled." << endl;
                return;  // Exit the function without adding a product
            }
        }
    }
}

void viewShoppingCart(const ShoppingCart& cart)
{
    cout << "=========================" << endl;
    cout << "      Shopping Cart      " << endl;
    cout << "=========================" << endl;
    cout << left << setw(12) << "Product ID" << setw(25) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << "Total" << endl;
    cout << "----------------------------------------------" << endl;

    for (const auto& item : cart.getItems())
    {
        cout << setw(12) << item.first.getProductID()
             << setw(25) << item.first.getName()
             << setw(10) << fixed << setprecision(2) << item.first.getPrice()
             << setw(10) << item.second
             << setw(10) << fixed << setprecision(2) << item.first.getPrice() * item.second << endl;
    }

    cout << "----------------------------------------------" << endl;
    cout << "Total Price: " << fixed << setprecision(2) << cart.getTotalPrice() << endl;
    cout << "=========================" << endl;
}

int main()
{
    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    vector<Product> productList = {
        Product("E001", "iPhone 14 Pro Max", 89990.00, rand() % 100 + 1, "Electronics"),
        Product("E002", "Samsung Galaxy S23 Ultra", 74990.00, rand() % 100 + 1, "Electronics"),
        Product("E003", "Apple MacBook Pro M2", 99990.00, rand() % 100 + 1, "Electronics"),
        Product("E004", "Dell XPS 13", 69990.00, rand() % 100 + 1, "Electronics"),
        Product("E005", "PlayStation 5", 32990.00, rand() % 100 + 1, "Electronics"),
        Product("E006", "Xbox Series X", 31990.00, rand() % 100 + 1, "Electronics"),
        Product("E007", "Sony WH-1000XM5", 18990.00, rand() % 100 + 1, "Electronics"),
        Product("E008", "AirPods Max", 24990.00, rand() % 100 + 1, "Electronics"),
        Product("H001", "LG French Door Fridge", 39990.00, rand() % 100 + 1, "Home Appliances"),
        Product("H002", "Samsung SidexSide Fridge", 34990.00, rand() % 100 + 1, "Home Appliances"),
        Product("H003", "LG Front-Load Washer", 24990.00, rand() % 100 + 1, "Home Appliances"),
        Product("H004", "Carrier Air Conditioner", 16990.00, rand() % 100 + 1, "Home Appliances"),
        Product("F001", "Shirts", 2500.00, rand() % 100 + 1, "Fashion"),
        Product("F002", "Dresses", 5000.00, rand() % 100 + 1, "Fashion"),
        Product("F003", "Pants", 4000.00, rand() % 100 + 1, "Fashion"),
        Product("F004", "Tops", 2000.00, rand() % 100 + 1, "Fashion"),
        Product("F005", "Bottoms", 3000.00, rand() % 100 + 1, "Fashion"),
        Product("F006", "Shoes", 6000.00, rand() % 100 + 1, "Fashion"),
        Product("F007", "Bags", 10000.00, rand() % 100 + 1, "Fashion"),
        Product("F008", "Watches", 20000.00, rand() % 100 + 1, "Fashion"),
        Product("F009", "Jewelry", 5000.00, rand() % 100 + 1, "Fashion"),
        Product("B001", "Foundation", 2000.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B002", "Eyeshadow", 1500.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B003", "Lipstick", 1000.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B004", "Moisturizer", 2000.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B005", "Cleanser", 1000.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B006", "Sunscreen", 1500.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B007", "Shampoo", 800.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B008", "Conditioner", 800.00, rand() % 100 + 1, "Beauty and Personal Care"),
        Product("B009", "Hair Styling Products", 1500.00, rand() % 100 + 1, "Beauty and Personal Care")
    };

    ShoppingCart shoppingCart(1);
    Customer customer(1, "John Doe", "john.doe@example.com", "123 Main St");

    int choice;

    do {
        cout << "\nMain Menu\n";
        cout << "1. View Products\n";
        cout << "2. View Shopping Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addProductToCart(productList, shoppingCart);
            break;
        case 2:
            viewShoppingCart(shoppingCart);
            break;
        case 3:
            cout << "Order summary and details coming soon!" << endl;
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
        }
    } while (choice != 4);

    return 0;
}
