#include <iostream> 
#include <vector>  
#include <memory> 
#include <string> 


class MenuItem {
protected:
    std::string name; 
    double price; 
public:
    MenuItem(std::string n, double p) : name(n), price(p) {} 
    virtual void displayItem() const { 
        std::cout << name << " - $" << price << std::endl;
    }
    virtual double calculatePrice() const { return price; } 
    virtual ~MenuItem() {} 
};


class Coffee : public MenuItem {
    std::string coffeeType; 
public:
    Coffee(std::string n, double p, std::string type) : MenuItem(n, p), coffeeType(type) {} 
    void displayItem() const override { 
        std::cout << coffeeType << " Coffee: " << name << " - EUR" << price << std::endl;
    }
};


class Alcohol : public MenuItem {
    std::string alcoholType; 
public:
    Alcohol(std::string n, double p, std::string type) : MenuItem(n, p), alcoholType(type) {} 
    void displayItem() const override { 
        std::cout << alcoholType << " Alcohol: " << name << " - EUR" << price << std::endl;
    }
};


class Order {
    std::vector<std::shared_ptr<MenuItem>> items; 
    double totalPrice; 
public:
    Order() : totalPrice(0) {} 
    void addItem(std::shared_ptr<MenuItem> item) { 
        items.push_back(item);
        totalPrice += item->calculatePrice(); 
    }
    void displayOrder() const { 
        std::cout << "Order Summary:" << std::endl;
        for (const auto& item : items) { 
            item->displayItem();
        }
        std::cout << "Total: EUR" << totalPrice << std::endl; 
    }
    double getTotalPrice() const { return totalPrice; } 
};


class Payment {
public:
    virtual void processPayment(double amount) = 0; 
    virtual ~Payment() {} 
};

class CashPayment : public Payment {
public:
    void processPayment(double amount) override { 
        std::cout << "Paid EUR" << amount << " in cash." << std::endl;
    }
};

class CardPayment : public Payment {
public:
    void processPayment(double amount) override { 
        std::cout << "Paid EUR" << amount << " using a credit/debit card." << std::endl;
    }
};

class Customer {
    std::string name; 
    std::vector<Order> orderHistory; 
public:
    Customer(std::string n) : name(n) {} 
    void placeOrder(const Order& order) {
        orderHistory.push_back(order); 
        std::cout << name << " placed an order." << std::endl;
        order.displayOrder(); 
    }
    void viewOrderHistory() const { 
        std::cout << "Order history for " << name << ":" << std::endl;
        for (const auto& order : orderHistory) { 
            order.displayOrder();
        }
    }
};

int main() {
    std::string customerName;
    std::cout << "Enter your name: ";
    std::getline(std::cin, customerName); 
    Customer customer(customerName); 

    Order order; 


    std::vector<std::shared_ptr<MenuItem>> menu = {
        
        std::make_shared<Coffee>("Latte", 3.5, "Espresso"),
        std::make_shared<Coffee>("Cappuccino", 4.0, "Espresso"),
        std::make_shared<Alcohol>("Whiskey", 10.0, "Bourbon"),
        std::make_shared<Alcohol>("Beer", 5.0, "Lager")
    };

    std::cout << "Menu:" << std::endl;
    for (size_t i = 0; i < menu.size(); ++i) {
        std::cout << i + 1 << ". ";
        menu[i]->displayItem();
    }

    int choice;
    while (true) {
        std::cout << "Enter item number to add to your order (0 to finish): ";
        std::cin >> choice;
        if (choice == 0) break;
        if (choice > 0 && choice <= menu.size()) {
            order.addItem(menu[choice - 1]);
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    customer.placeOrder(order); 

    std::unique_ptr<Payment> payment;
    int paymentChoice;
    std::cout << "Choose payment method (1 - Cash, 2 - Card): ";
    std::cin >> paymentChoice;

    if (paymentChoice == 1) {
        payment = std::make_unique<CashPayment>();
    }
    else {
        payment = std::make_unique<CardPayment>();
    }

    payment->processPayment(order.getTotalPrice()); 

    customer.viewOrderHistory(); 
    return 0;
}
