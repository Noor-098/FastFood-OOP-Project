#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;


template <typename T>                   //for generic display
void displayList(const vector<T>& list)
{
    for (const auto& item : list) {
        item.display();
    }
}

class Item //base class for food and employee
{
protected:
    int id;
    string name;

public:
    virtual void input()  {
        cout << "Enter ID: "; cin >> id;
        cin.ignore();
        cout << "Enter Name: "; getline(cin, name);
    }

    virtual void display() const {
        cout << "ID: " << id << ", Name: " << name << endl;
    }

    int getId() const { return id; }
    string getName() const { return name; }

    virtual ~Item() {}
};

class Food : public Item //1st child class
{
    float costPrice, sellingPrice;
    int stock, sold;

public:
    void input() override {
        Item::input();
        cout << "Enter Cost Price: "; cin >> costPrice;
        cout << "Enter Selling Price: "; cin >> sellingPrice;
        cout << "Enter Stock: "; cin >> stock;
        sold = 0;
    }

    void display() const override {
        cout << id << "\t" << name << "\t" << costPrice << "\t"
             << sellingPrice << "\t" << stock << "\t" << sold << endl;
    }

    bool recordSale(int qty) {
        if (stock >= qty) {
            stock -= qty;
            sold += qty;
            return true;
        }
        return false;
    }

    bool cancelSale(int qty) {
        if (sold >= qty) {
            sold -= qty;
            stock += qty;
            return true;
        }
        return false;
    }

    float getProfit() const {
        return sold * (sellingPrice - costPrice);
    }

    void saveToFile(ofstream& out) const {
        out << id << " " << name << "|" << costPrice << " "
            << sellingPrice << " " << stock << " " << sold << endl;
    }

    void loadFromFile(ifstream& in) {
        in >> id;
        in.ignore();
        getline(in, name, '|');
        in >> costPrice >> sellingPrice >> stock >> sold;
    }

    float getCostPrice() const { return costPrice; }
    float getSellingPrice() const { return sellingPrice; }
    int getStock() const { return stock; }
    int getSold() const { return sold; }
};

class Employee : public Item //2nd child class
{
    string position;
    float salary;

public:
    void input() override {
        Item::input();
        cout << "Enter Position: "; getline(cin, position);
        cout << "Enter Salary: "; cin >> salary;
    }

    void display() const override {
        cout << id << "\t" << name << "\t\t" << position << "\t\t" << salary << endl;
    }

    void saveToFile(ofstream& out) const {
        out << id << " " << name << "|" << position << "|" << salary << endl;
    }

    void loadFromFile(ifstream& in) {
        in >> id;
        in.ignore();
        getline(in, name, '|');
        getline(in, position, '|');
        in >> salary;
    }
};

vector<Food> foods;        //vectors
vector<Employee> employees;

void saveFoods() //file handling (starting )
{
    ofstream out("foods.txt");
    for (const auto& f : foods) f.saveToFile(out);
    out.close();
}

void saveEmployees() {
    ofstream out("employees.txt");
    for (const auto& e : employees) e.saveToFile(out);
    out.close();
}

void loadFoods() {
    ifstream in("foods.txt");
    bool dataLoaded = false;

    while (in) {
        Food f;
        f.loadFromFile(in);
        if (in) {
            foods.push_back(f);
            dataLoaded = true;
        }
    }
    in.close();

    if (!dataLoaded) {
        Food f;
        f.input(); foods.push_back(f);
    }
}

void loadEmployees() { //file handling (ending)
    ifstream in("employees.txt");
    while (in) {
        Employee e;
        e.loadFromFile(in);
        if (in) employees.push_back(e);
    }
    in.close();
}

void addFood() {
    Food f;
    f.input();
    foods.push_back(f);
    cout << "Food added.\n";
}

void updateFood() {
    int id;
    cout << "Enter Food ID to update: "; cin >> id;
    for (auto& f : foods) {
        if (f.getId() == id) {
            f.input();
            cout << "Food updated.\n";
            return;
        }
    }
    cout << "Food not found.\n";
}

void viewStock() {
    cout << "\n--- Stock Details ---\n";
    cout << left << setw(8) << "ID"
         << setw(16) << "Name"
         << setw(16) << "Cost Price"
         << setw(18) << "Selling Price"
         << setw(10) << "Stock"
         << setw(6)  << "Sold" << endl;

    cout << string(74, '-') << endl;

    for (const auto &food : foods) {
        cout << left << setw(8) << food.getId()
             << setw(16) << food.getName()
             << setw(16) << food.getCostPrice()
             << setw(18) << food.getSellingPrice()
             << setw(10) << food.getStock()
             << setw(6)  << food.getSold() << endl;
    }
}

void recordSale() {
    int id, qty;
    cout << "Enter Food ID: "; cin >> id;
    cout << "Enter Quantity: "; cin >> qty;
    for (auto& f : foods) {
        if (f.getId() == id) {
            if (f.recordSale(qty)) cout << "Sale recorded.\n";
            else cout << "Insufficient stock.\n";
            return;
        }
    }
    cout << "Food not found.\n";
}

void cancelSale() {
    int id, qty;
    cout << "Enter Food ID: "; cin >> id;
    cout << "Enter Quantity to Cancel: "; cin >> qty;
    for (auto& f : foods) {
        if (f.getId() == id) {
            if (f.cancelSale(qty)) cout << "Sale cancelled.\n";
            else cout << "Invalid quantity.\n";
            return;
        }
    }
    cout << "Food not found.\n";
}

void showProfit() {
    float total = 0;
    for (const auto& f : foods) total += f.getProfit();
    cout << "Total Profit: " << total << endl;
}

void addEmployee() {
    Employee e;
    e.input();
    employees.push_back(e);
    cout << "Employee added.\n";
}

void viewEmployees() {
    cout << "\n--- Employee Details ---\n";
    cout << left << setw(8) << "ID"
         << setw(16) << "Name"
         << setw(16) << "Position"
         << setw(16) << "Salary" << endl;

    cout << string(52, '-') << endl;

    for (const auto& e : employees) {
        e.display();
    }
}

int main() {
	cout<<"<--------Fast Food Management System------->"<<endl; //flow of program (function calling)
   loadFoods();       
    loadEmployees(); 

    int choice;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. View Stock\n";
        cout << "2. Add Food\n";
        cout << "3. Update Food\n";
        cout << "4. Sales\n";
        cout << "5. Cancel Sale\n";
        cout << "6. Show Profit\n";
        cout << "7. Add Employee\n";
        cout << "8. View Employees\n";
        cout << "9. Save & Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewStock(); break;
            case 2: addFood(); break;
            case 3: updateFood(); break;
            case 4: recordSale(); break;
            case 5: cancelSale(); break;
            case 6: showProfit(); break;
            case 7: addEmployee(); break;
            case 8: viewEmployees(); break;
            case 9: 
                saveFoods();
                saveEmployees();
                cout << "Data saved. Exiting...\n";
                break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 9);

    return 0;
}