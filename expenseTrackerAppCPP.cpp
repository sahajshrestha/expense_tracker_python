#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Expense Struct
 * Attributes inlcude
 *   date
 *   amount 
 *   category
 *   description
*/
struct Expense {
    string date;
    double amount;
    string category;
    string description;
};

/**
 * addExpense
 * Function to add expenses
*/
void addExpense(vector<Expense> &expenses) {
    Expense newExpense;
    cout << "Enter date (in the format YYYY-MM-DD): ";
    cin >> newExpense.date;
    cout << "Enter amount of expense in number $: ";
    cin >> newExpense.amount;
    cin.ignore();
    cout << "Enter category of the expense: ";
    getline(cin, newExpense.category);
    cout << "Enter description of the expense: ";
    getline(cin, newExpense.description);

    expenses.push_back(newExpense);
    cout << "Expense added successfully!\n";
}

/**
 * viewExpenses
 * Function to display the expenses
*/
void viewExpenses(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    cout << "\nRecorded Expenses Are:\n";
    cout << setw(12) << "Date" << setw(10) << "Amount" << setw(15) << "Category" << setw(20) << "Description\n";
    cout << "--------------------------------------------------------------\n";
    
    for (const auto &exp : expenses) {
        cout << setw(12) << exp.date << setw(10) << exp.amount << setw(15) << exp.category << setw(20) << exp.description << "\n";
    }
}

/**
 * filterByCategory
 * Function to filter the expenses based on category string
*/
void filterByCategory(const vector<Expense> &expenses, const string &category) {
    cout << "\nExpenses in Category: " << category << "\n";
    bool found = false;
    for (const auto &exp : expenses) {
        if (exp.category == category) {
            cout << exp.date << " | $" << exp.amount << " | " << exp.description << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No expenses found in this category.\n";
    }
}

/**
 * filterByDateRange
 * Function to filter the expenses based on a given date range
*/
void filterByDateRange(const vector<Expense> &expenses, const string &startDate, const string &endDate) {
    cout << "\nExpenses from " << startDate << " to " << endDate << ":\n";
    bool found = false;
    for (const auto &exp : expenses) {
        if (exp.date >= startDate && exp.date <= endDate) {
            cout << exp.date << " | $" << exp.amount << " | " << exp.category << " | " << exp.description << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No expenses found in this date range.\n";
    }
}

/**
 * totalByCategory
 * Function to calcuate the total expense on a given category
*/
void totalByCategory(const vector<Expense> &expenses) {
    map<string, double> categoryTotals;
    for (const auto &exp : expenses) {
        categoryTotals[exp.category] += exp.amount;
    }

    cout << "\nTotal Expenses by Category:\n";
    for (const auto &pair : categoryTotals) {
        cout << pair.first << ": $" << pair.second << "\n";
    }
}

/**
 * totalExpenses
 * Function to calculate the total expense recorded so far
*/
void totalExpenses(const vector<Expense> &expenses) {
    double total = 0;
    for (const auto &exp : expenses) {
        total += exp.amount;
    }
    cout << "\nTotal Expenses: $" << total << "\n";
}

/**
 * The Main program
*/
int main() {
    vector<Expense> expenses;
    int choice;

    do {
        cout << "\nExpense Tracker Menu (Please choose via number) \n";
        cout << "1. Add Expense\n";
        cout << "2. View All Expenses\n";
        cout << "3. Filter by Category\n";
        cout << "4. Filter by Date Range\n";
        cout << "5. Total Expenses by Category\n";
        cout << "6. Total Overall Expenses\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addExpense(expenses);
                break;
            case 2:
                viewExpenses(expenses);
                break;
            case 3: {
                cout << "Enter category to filter: ";
                string category;
                cin.ignore();
                getline(cin, category);
                filterByCategory(expenses, category);
                break;
            }
            case 4: {
                string startDate, endDate;
                cout << "Enter start date (YYYY-MM-DD): ";
                cin >> startDate;
                cout << "Enter end date (YYYY-MM-DD): ";
                cin >> endDate;
                filterByDateRange(expenses, startDate, endDate);
                break;
            }
            case 5:
                totalByCategory(expenses);
                break;
            case 6:
                totalExpenses(expenses);
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
