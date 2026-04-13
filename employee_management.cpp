#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// ============================================================
//  Employee Class
// ============================================================
class Employee {
private:
    int    employeeID;
    string name;
    double salary;
    string department;

public:
    // Constructor
    Employee(int id, const string& name, double salary, const string& dept)
        : employeeID(id), name(name), salary(salary), department(dept) {}

    // ---------- Getters ----------
    int    getEmployeeID()  const { return employeeID;  }
    string getName()        const { return name;        }
    double getSalary()      const { return salary;      }
    string getDepartment()  const { return department;  }

    // ---------- Setters ----------
    void setEmployeeID (int id)           { employeeID  = id;   }
    void setName       (const string& n)  { name        = n;    }
    void setSalary     (double s)         { salary      = s;    }
    void setDepartment (const string& d)  { department  = d;    }

    // ---------- Salary after tax ----------
    double getSalaryAfterTax(double taxRate = 0.10) const {
        return salary - (salary * taxRate);
    }

    // ---------- Display ----------
    void display() const {
        cout << left
             << setw(8)  << employeeID
             << setw(20) << name
             << setw(15) << department
             << "$" << fixed << setprecision(2) << setw(12) << salary
             << "$" << fixed << setprecision(2) << getSalaryAfterTax()
             << "\n";
    }
};

// ============================================================
//  Employee Management System
// ============================================================
class EmployeeManagementSystem {
private:
    vector<Employee> employees;

    // Helper: find index by ID (-1 if not found)
    int findIndex(int id) const {
        for (int i = 0; i < (int)employees.size(); i++)
            if (employees[i].getEmployeeID() == id)
                return i;
        return -1;
    }

    // Helper: check if ID already exists
    bool idExists(int id) const {
        return findIndex(id) != -1;
    }

    // Helper: safe integer input
    int readInt(const string& prompt) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) { cin.ignore(); return val; }
            cout << "  [!] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Helper: safe double input
    double readDouble(const string& prompt) {
        double val;
        while (true) {
            cout << prompt;
            if (cin >> val && val >= 0) { cin.ignore(); return val; }
            cout << "  [!] Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void printHeader() const {
        cout << "\n"
             << left
             << setw(8)  << "ID"
             << setw(20) << "Name"
             << setw(15) << "Department"
             << setw(13) << "Gross Salary"
             << "Net Salary (after 10% tax)\n";
        cout << string(75, '-') << "\n";
    }

public:
    // ----------------------------------------------------------
    //  1. Add Employee
    // ----------------------------------------------------------
    void addEmployee() {
        cout << "\n===== Add New Employee =====\n";

        int id = readInt("  Enter Employee ID   : ");
        if (idExists(id)) {
            cout << "  [!] Employee with ID " << id << " already exists.\n";
            return;
        }

        cin.clear();
        cout << "  Enter Name           : ";
        string name; getline(cin, name);

        cout << "  Enter Department     : ";
        string dept; getline(cin, dept);

        double salary = readDouble("  Enter Salary ($)    : ");

        employees.emplace_back(id, name, salary, dept);
        cout << "  [+] Employee \"" << name << "\" added successfully!\n";
    }

    // ----------------------------------------------------------
    //  2. View All Employees
    // ----------------------------------------------------------
    void viewEmployees() const {
        cout << "\n===== All Employees =====\n";
        if (employees.empty()) {
            cout << "  No employees found.\n";
            return;
        }
        printHeader();
        for (const auto& emp : employees)
            emp.display();
        cout << string(75, '-') << "\n";
        cout << "  Total employees: " << employees.size() << "\n";
    }

    // ----------------------------------------------------------
    //  3. Update Employee (salary or department)
    // ----------------------------------------------------------
    void updateEmployee() {
        cout << "\n===== Update Employee =====\n";
        int id = readInt("  Enter Employee ID to update: ");
        int idx = findIndex(id);
        if (idx == -1) {
            cout << "  [!] Employee with ID " << id << " not found.\n";
            return;
        }

        Employee& emp = employees[idx];
        cout << "  Employee found: " << emp.getName() << "\n";
        cout << "  What would you like to update?\n";
        cout << "    1. Salary\n";
        cout << "    2. Department\n";
        cout << "    3. Both\n";

        int choice = readInt("  Your choice: ");
        switch (choice) {
            case 1: {
                double newSalary = readDouble("  Enter new Salary ($): ");
                emp.setSalary(newSalary);
                cout << "  [✓] Salary updated successfully!\n";
                break;
            }
            case 2: {
                cout << "  Enter new Department: ";
                string newDept; getline(cin, newDept);
                emp.setDepartment(newDept);
                cout << "  [✓] Department updated successfully!\n";
                break;
            }
            case 3: {
                double newSalary = readDouble("  Enter new Salary ($): ");
                emp.setSalary(newSalary);
                cout << "  Enter new Department: ";
                string newDept; getline(cin, newDept);
                emp.setDepartment(newDept);
                cout << "  [✓] Salary and Department updated successfully!\n";
                break;
            }
            default:
                cout << "  [!] Invalid choice. No changes made.\n";
        }
    }

    // ----------------------------------------------------------
    //  4. Delete Employee
    // ----------------------------------------------------------
    void deleteEmployee() {
        cout << "\n===== Delete Employee =====\n";
        int id = readInt("  Enter Employee ID to delete: ");
        int idx = findIndex(id);
        if (idx == -1) {
            cout << "  [!] Employee with ID " << id << " not found.\n";
            return;
        }

        cout << "  Are you sure you want to delete \"" << employees[idx].getName() << "\"? (y/n): ";
        char confirm; cin >> confirm; cin.ignore();
        if (confirm == 'y' || confirm == 'Y') {
            string name = employees[idx].getName();
            employees.erase(employees.begin() + idx);
            cout << "  [-] Employee \"" << name << "\" deleted successfully!\n";
        } else {
            cout << "  Deletion cancelled.\n";
        }
    }

    // ----------------------------------------------------------
    //  5. Salary Details for a specific employee
    // ----------------------------------------------------------
    void viewSalaryDetails() const {
        cout << "\n===== Salary Details =====\n";
        int id;
        // inline read to keep const
        cout << "  Enter Employee ID: ";
        cin >> id; cin.ignore();

        int idx = findIndex(id);
        if (idx == -1) {
            cout << "  [!] Employee with ID " << id << " not found.\n";
            return;
        }
        const Employee& emp = employees[idx];
        cout << "\n  Name         : " << emp.getName()       << "\n";
        cout << "  Department   : " << emp.getDepartment()  << "\n";
        cout << fixed << setprecision(2);
        cout << "  Gross Salary : $" << emp.getSalary()          << "\n";
        cout << "  Tax (10%)    : $" << emp.getSalary() * 0.10   << "\n";
        cout << "  Net Salary   : $" << emp.getSalaryAfterTax()  << "\n";
    }

    // ----------------------------------------------------------
    //  Main Menu
    // ----------------------------------------------------------
    void run() {
        cout << "\n";
        cout << "  ╔══════════════════════════════════════╗\n";
        cout << "  ║   Employee Management System         ║\n";
        cout << "  ╚══════════════════════════════════════╝\n";

        int choice = 0;
        do {
            cout << "\n  ┌─────────────────────────┐\n";
            cout << "  │         MAIN MENU        │\n";
            cout << "  ├─────────────────────────┤\n";
            cout << "  │  1. Add Employee         │\n";
            cout << "  │  2. View Employees       │\n";
            cout << "  │  3. Update Employee      │\n";
            cout << "  │  4. Delete Employee      │\n";
            cout << "  │  5. View Salary Details  │\n";
            cout << "  │  6. Exit                 │\n";
            cout << "  └─────────────────────────┘\n";
            cout << "  Enter your choice (1-6): ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  [!] Invalid input.\n";
                continue;
            }
            cin.ignore();

            switch (choice) {
                case 1: addEmployee();      break;
                case 2: viewEmployees();    break;
                case 3: updateEmployee();   break;
                case 4: deleteEmployee();   break;
                case 5: viewSalaryDetails(); break;
                case 6: cout << "\n  Goodbye! Exiting system...\n\n"; break;
                default: cout << "  [!] Invalid choice. Please enter 1-6.\n";
            }
        } while (choice != 6);
    }
};

// ============================================================
//  Main Entry Point
// ============================================================
int main() {
    EmployeeManagementSystem ems;
    ems.run();
    return 0;
}