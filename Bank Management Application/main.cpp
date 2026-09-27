#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const string FILE_NAME = "accounts.txt";

// ===============================
// Bank Account Class
// ===============================
class BankAccount {
private:
    int accountNumber;
    string name;
    string phone;
    double balance;

public:
    // Default constructor
    BankAccount() {
        accountNumber = 0;
        name = "";
        phone = "";
        balance = 0.0;
    }

    // Parameterized constructor
    BankAccount(int accNo, string accName, string accPhone, double accBalance) {
        accountNumber = accNo;
        name = accName;
        phone = accPhone;
        balance = accBalance;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    string getName() const {
        return name;
    }

    string getPhone() const {
        return phone;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\nAmount deposited successfully!\n";
            cout << "New Balance: Rs. " << fixed << setprecision(2)
                 << balance << endl;
        } else {
            cout << "\nInvalid deposit amount!\n";
        }
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\nInvalid withdrawal amount!\n";
            return false;
        }

        if (amount > balance) {
            cout << "\nInsufficient balance!\n";
            return false;
        }

        balance -= amount;

        cout << "\nAmount withdrawn successfully!\n";
        cout << "Remaining Balance: Rs. "
             << fixed << setprecision(2)
             << balance << endl;

        return true;
    }

    void display() const {
        cout << "\n-----------------------------------\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Name           : " << name << endl;
        cout << "Phone          : " << phone << endl;
        cout << "Balance        : Rs. "
             << fixed << setprecision(2)
             << balance << endl;
        cout << "-----------------------------------\n";
    }

    // Save account to file
    void saveToFile(ofstream &file) const {
        file << accountNumber << '\n';
        file << name << '\n';
        file << phone << '\n';
        file << balance << '\n';
    }

    // Load account from file
    bool loadFromFile(ifstream &file) {
        if (!(file >> accountNumber)) {
            return false;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(file, name);
        getline(file, phone);

        file >> balance;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        return true;
    }
};

// ===============================
// Utility Functions
// ===============================

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Check whether account already exists
bool accountExists(int accountNumber) {
    ifstream file(FILE_NAME);

    if (!file) {
        return false;
    }

    BankAccount account;

    while (account.loadFromFile(file)) {
        if (account.getAccountNumber() == accountNumber) {
            return true;
        }
    }

    return false;
}

// Find account and display it
bool findAccount(int accountNumber, BankAccount &result) {
    ifstream file(FILE_NAME);

    if (!file) {
        return false;
    }

    BankAccount account;

    while (account.loadFromFile(file)) {
        if (account.getAccountNumber() == accountNumber) {
            result = account;
            return true;
        }
    }

    return false;
}

// Rewrite all accounts into file
void saveAllAccounts(BankAccount updatedAccount) {
    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");

    if (!tempFile) {
        cout << "Error creating temporary file!\n";
        return;
    }

    BankAccount account;

    while (file && account.loadFromFile(file)) {

        if (account.getAccountNumber() ==
            updatedAccount.getAccountNumber()) {

            updatedAccount.saveToFile(tempFile);
        }
        else {
            account.saveToFile(tempFile);
        }
    }

    file.close();
    tempFile.close();

    remove(FILE_NAME.c_str());
    rename("temp.txt", FILE_NAME.c_str());
}

// ===============================
// Create New Account
// ===============================
void createAccount() {

    int accountNumber;
    string name;
    string phone;
    double initialDeposit;

    cout << "\n====================================\n";
    cout << "          CREATE ACCOUNT\n";
    cout << "====================================\n";

    cout << "Enter Account Number: ";

    while (!(cin >> accountNumber)) {
        cout << "Invalid input. Enter a number: ";
        clearInput();
    }

    if (accountExists(accountNumber)) {
        cout << "\nAccount already exists!\n";
        return;
    }

    cin.ignore();

    cout << "Enter Customer Name: ";
    getline(cin, name);

    cout << "Enter Phone Number: ";
    getline(cin, phone);

    cout << "Enter Initial Deposit: Rs. ";

    while (!(cin >> initialDeposit)) {
        cout << "Invalid amount. Enter again: ";
        clearInput();
    }

    if (initialDeposit < 0) {
        cout << "\nInitial deposit cannot be negative!\n";
        return;
    }

    BankAccount account(
        accountNumber,
        name,
        phone,
        initialDeposit
    );

    ofstream file(FILE_NAME, ios::app);

    if (!file) {
        cout << "\nError opening account file!\n";
        return;
    }

    account.saveToFile(file);

    file.close();

    cout << "\nAccount created successfully!\n";
    account.display();
}

// ===============================
// Deposit Money
// ===============================
void depositMoney() {

    int accountNumber;
    double amount;

    cout << "\n====================================\n";
    cout << "             DEPOSIT\n";
    cout << "====================================\n";

    cout << "Enter Account Number: ";

    while (!(cin >> accountNumber)) {
        cout << "Invalid input. Enter account number: ";
        clearInput();
    }

    BankAccount account;

    if (!findAccount(accountNumber, account)) {
        cout << "\nAccount not found!\n";
        return;
    }

    cout << "Enter Deposit Amount: Rs. ";

    while (!(cin >> amount)) {
        cout << "Invalid amount. Enter again: ";
        clearInput();
    }

    account.deposit(amount);

    saveAllAccounts(account);
}

// ===============================
// Withdraw Money
// ===============================
void withdrawMoney() {

    int accountNumber;
    double amount;

    cout << "\n====================================\n";
    cout << "            WITHDRAW\n";
    cout << "====================================\n";

    cout << "Enter Account Number: ";

    while (!(cin >> accountNumber)) {
        cout << "Invalid input. Enter account number: ";
        clearInput();
    }

    BankAccount account;

    if (!findAccount(accountNumber, account)) {
        cout << "\nAccount not found!\n";
        return;
    }

    cout << "Current Balance: Rs. "
         << fixed << setprecision(2)
         << account.getBalance() << endl;

    cout << "Enter Withdrawal Amount: Rs. ";

    while (!(cin >> amount)) {
        cout << "Invalid amount. Enter again: ";
        clearInput();
    }

    if (account.withdraw(amount)) {
        saveAllAccounts(account);
    }
}

// ===============================
// Check Balance
// ===============================
void checkBalance() {

    int accountNumber;

    cout << "\n====================================\n";
    cout << "          BALANCE INQUIRY\n";
    cout << "====================================\n";

    cout << "Enter Account Number: ";

    while (!(cin >> accountNumber)) {
        cout << "Invalid input. Enter account number: ";
        clearInput();
    }

    BankAccount account;

    if (findAccount(accountNumber, account)) {
        cout << "\nAccount Details\n";
        account.display();
    }
    else {
        cout << "\nAccount not found!\n";
    }
}

// ===============================
// Display All Accounts
// ===============================
void displayAllAccounts() {

    ifstream file(FILE_NAME);

    if (!file) {
        cout << "\nNo accounts found!\n";
        return;
    }

    BankAccount account;
    bool found = false;

    cout << "\n====================================\n";
    cout << "        ALL BANK ACCOUNTS\n";
    cout << "====================================\n";

    while (account.loadFromFile(file)) {

        account.display();

        found = true;
    }

    if (!found) {
        cout << "No account records available.\n";
    }

    file.close();
}

// ===============================
// Main Function
// ===============================
int main() {

    int choice;

    do {

        cout << "\n\n";
        cout << "============================================\n";
        cout << "          BANK MANAGEMENT SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "============================================\n";

        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid choice. Enter a number: ";
            clearInput();
        }

        switch (choice) {

            case 1:
                createAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                checkBalance();
                break;

            case 5:
                displayAllAccounts();
                break;

            case 6:
                cout << "\nThank you for using Bank Management System!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}