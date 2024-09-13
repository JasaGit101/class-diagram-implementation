#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cctype>

using namespace std;

// Forward declaration
class Order;

vector<Order> orderList;

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

    void decreaseStock(int quantity) { stockQuantity -= quantity; }
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
        : orderID(id), customer(cust), orderDate(date), products(prodList)
    {
        totalAmount = 0;
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

string toUpperCase(const string& str) {
    string result;
    result.reserve(str.size());
    for (char ch : str) {
        result.push_back(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

void viewProducts(vector<Product>& products, ShoppingCart& cart)
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

    string productID;
    char addAnother;

    do {
        cout << "Enter the ID of the product you want to add to the shopping cart (or enter -1 to cancel): ";
        cin >> productID;

        // Convert productID to uppercase for comparison
        productID = toUpperCase(productID);

        // Check if the user wants to cancel
        if (productID == "-1") {
            cout << "Returning to menu..." << endl;
            return;  // Exit the function and return to the menu
        }

        // Find the product
        auto it = find_if(products.begin(), products.end(), [&](const Product& p) 
        {
            return toUpperCase(p.getProductID()) == productID;
        });

        if (it != products.end())
        {
            if (it->getStockQuantity() > 0) {
                cart.addProduct(*it, 1); // Add product with default quantity 1
                it->decreaseStock(1); // Decrease stock quantity
                cout << "Product added successfully!" << endl;
            } else {
                cout << "Product is out of stock!" << endl;
            }
        }
        else
        {
            cout << "Product not found!" << endl;
        }

        // Prompt for adding another product
        do {
            cout << "Do you want to add another product? (Y/N): ";
            cin >> addAnother;

            if (addAnother == 'Y' || addAnother == 'y' || addAnother == 'N' || addAnother == 'n') {
                break; // Exit the loop if input is valid
            } else {
                cout << "Invalid input. Please enter Y or N." << endl;
            }
        } while (addAnother != 'Y' && addAnother != 'y' && addAnother != 'N' && addAnother != 'n');

    } while (addAnother == 'Y' || addAnother == 'y');
}

void viewShoppingCart(ShoppingCart& cart, Customer& customer)
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

    // Prompt for checkout
    char checkOut;
    do {
        cout << "Do you want to check out all the products? (Y/N): ";
        cin >> checkOut;

        if (checkOut == 'Y' || checkOut == 'y')
        {
            // Create an order
            static int nextOrderID = 1;
            time_t now = time(0);
            tm* ltm = localtime(&now);
            string orderDate = to_string(1900 + ltm->tm_year) + "-" 
                             + (ltm->tm_mon + 1 < 10 ? "0" : "") + to_string(ltm->tm_mon + 1) + "-" 
                             + (ltm->tm_mday < 10 ? "0" : "") + to_string(ltm->tm_mday);

            Order newOrder(nextOrderID++, customer, orderDate, cart.getItems());
            newOrder.generateInvoice();
            orderList.push_back(newOrder);

            // Clear the cart after checkout
            cart = ShoppingCart(cart.getCartID()); // Reset the cart (could also implement a clear method in ShoppingCart)

            cout << "You have successfully checked out the products!" << endl;
            return; // Exit the function after successful checkout
        }
        else if (checkOut == 'N' || checkOut == 'n')
        {
            cout << "Returning to menu..." << endl;
            return; // Exit the function and return to the menu
        }
        else
        {
            cout << "Invalid input. Please enter Y or N." << endl;
        }
    } while (checkOut != 'Y' && checkOut != 'y' && checkOut != 'N' && checkOut != 'n');
}

void placeOrder(Customer& customer, ShoppingCart& cart)
{
    static int nextOrderID = 1;

    // Get current time
    time_t now = time(0);
    tm* ltm = localtime(&now);

    // Format order date as YYYY-MM-DD
    string orderDate = to_string(1900 + ltm->tm_year) + "-" 
                     + (ltm->tm_mon + 1 < 10 ? "0" : "") + to_string(ltm->tm_mon + 1) + "-" 
                     + (ltm->tm_mday < 10 ? "0" : "") + to_string(ltm->tm_mday);

    Order newOrder(nextOrderID++, customer, orderDate, cart.getItems());
    newOrder.generateInvoice();
    orderList.push_back(newOrder);

    cout << "Order viewed successfully!" << endl;
}


int main()
{
    srand(static_cast<unsigned>(time(0)));  // Seed random number generator

    // Sample products
    vector<Product> products = {
        Product("P001", "iPhone 14 Pro Max", 89990, rand() % 50 + 1, "Electronics"),
        Product("P002", "Samsung Galaxy S23 Ultra", 74990, rand() % 50 + 1, "Electronics"),
        Product("P003", "Apple MacBook Pro M2", 99990, rand() % 50 + 1, "Electronics"),
        Product("P004", "Dell XPS 13", 69990, rand() % 50 + 1, "Electronics"),
        Product("P005", "PlayStation 5", 32990, rand() % 50 + 1, "Electronics"),
        Product("P006", "Xbox Series X", 31990, rand() % 50 + 1, "Electronics"),
        Product("P007", "Sony WH-1000XM5", 18990, rand() % 50 + 1, "Electronics"),
        Product("P008", "AirPods Max", 24990, rand() % 50 + 1, "Electronics"),
        Product("P009", "LG Refrigerator", 39990, rand() % 50 + 1, "Home Appliances"),
        Product("P010", "Samsung Refrigerator", 34990, rand() % 50 + 1, "Home Appliances"),
        Product("P011", "LG Washing Machine", 24990, rand() % 50 + 1, "Home Appliances"),
        Product("P012", "Carrier Air Conditioner", 16990, rand() % 50 + 1, "Home Appliances"),
        Product("P013", "Shirts", 2500, rand() % 50 + 1, "Fashion"),
        Product("P014", "Dresses", 5000, rand() % 50 + 1, "Fashion"),
        Product("P015", "Pants", 4000, rand() % 50 + 1, "Fashion"),
        Product("P016", "Tops", 2000, rand() % 50 + 1, "Fashion"),
        Product("P017", "Bottoms", 3000, rand() % 50 + 1, "Fashion"),
        Product("P018", "Shoes", 6000, rand() % 50 + 1, "Fashion"),
        Product("P019", "Bags", 10000, rand() % 50 + 1, "Fashion"),
        Product("P020", "Watches", 20000, rand() % 50 + 1, "Fashion"),
        Product("P021", "Jewelry", 5000, rand() % 50 + 1, "Fashion"),
        Product("P022", "Foundation", 2000, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P023", "Eyeshadow", 1500, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P024", "Lipstick", 1000, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P025", "Moisturizer", 2000, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P026", "Cleanser", 1000, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P027", "Sunscreen", 1500, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P028", "Shampoo", 800, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P029", "Conditioner", 800, rand() % 50 + 1, "Beauty and Personal Care"),
        Product("P030", "Hair Styling Products", 1500, rand() % 50 + 1, "Beauty and Personal Care")
    };

    vector<Customer> customers = {
        Customer(1, "Alice Smith", "alice.smith@example.com", "123 Main St"),
        Customer(2, "Bob Johnson", "bob.johnson@example.com", "456 Oak Ave")
    };

    vector<ShoppingCart> carts = {
        ShoppingCart(1),
        ShoppingCart(2)
    };

    int option;
    do
    {
        cout << "=========================" << endl;
        cout << "       Main Menu         " << endl;
        cout << "=========================" << endl;
        cout << "1. View Products" << endl;
        cout << "2. View Shopping Cart" << endl;
        cout << "3. View Order" << endl;
        cout << "4. Exit" << endl;
        cout << "Select an option: ";
        cin >> option;

        switch (option)
        {
        case 1:
            viewProducts(products, carts[0]); // Assuming user 1's cart
            break;
        case 2:
            viewShoppingCart(carts[0], customers[0]); // Assuming user 1's cart and customer
            break;
        case 3:
            placeOrder(customers[0], carts[0]); // Assuming user 1's cart
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    } while (option != 4);

    return 0;
}
