#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Class representing a bank account
class BankAccount {
private:
    int accountNumber;
    string customerName;
    double balance;

public:
    // Constructor
    BankAccount(int accNo = 0, string name = "", double bal = 0.0) {
        accountNumber = accNo;
        customerName = name;
        balance = bal;
    }

    // Getter for account number
    int getAccountNumber() const {
        return accountNumber;
    }

    // Getter for customer name
    string getCustomerName() const {
        return customerName;
    }

    // Getter for balance
    double getBalance() const {
        return balance;
    }

    // Deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Amount deposited successfully.\n";
            cout << "New Balance: Rs. " << fixed << setprecision(2)
                 << balance << endl;
        } else {
            cout << "Invalid amount. Deposit must be greater than 0.\n";
        }
    }

    // Withdraw money
    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount. Withdrawal must be greater than 0.\n";
        }
        else if (amount > balance) {
            cout << "Insufficient balance.\n";
        }
        else {
            balance -= amount;
            cout << "Amount withdrawn successfully.\n";
            cout << "Remaining Balance: Rs. " << fixed << setprecision(2)
                 << balance << endl;
        }
    }

    // Display account details
    void displayAccount() const {
        cout << "\n----- Account Details -----\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Customer Name  : " << customerName << endl;
        cout << "Balance        : Rs. " << fixed << setprecision(2)
             << balance << endl;
        cout << "---------------------------\n";
    }
};


// Class responsible for managing the bank
class BankManager {
private:
    const string fileName = "accounts.txt";

public:

    // Check whether an account already exists
    bool accountExists(int accountNumber) {
        ifstream file(fileName);

        int accNo;
        string name;
        double balance;

        while (file >> accNo >> ws && getline(file, name, '|')
               && file >> balance) {

            if (accNo == accountNumber) {
                file.close();
                return true;
            }

            file.ignore();
        }

        file.close();
        return false;
    }


    // Create a new account
    void createAccount() {
        int accountNumber;
        string name;
        double initialDeposit;

        cout << "\n===== Create New Account =====\n";

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        if (accountExists(accountNumber)) {
            cout << "An account with this number already exists.\n";
            return;
        }

        cin.ignore();

        cout << "Enter Customer Name: ";
        getline(cin, name);

        cout << "Enter Initial Deposit: Rs. ";
        cin >> initialDeposit;

        if (initialDeposit < 0) {
            cout << "Initial deposit cannot be negative.\n";
            return;
        }

        ofstream file(fileName, ios::app);

        if (!file) {
            cout << "Error opening file.\n";
            return;
        }

        // Store account information in the file
        file << accountNumber << "|" << name << "|"
             << fixed << setprecision(2) << initialDeposit << endl;

        file.close();

        cout << "\nAccount created successfully!\n";
        cout << "Account Number: " << accountNumber << endl;
    }


    // Find and display an account
    void checkBalance() {
        int accountNumber;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        ifstream file(fileName);

        int accNo;
        string name;
        double balance;
        bool found = false;

        while (file >> accNo >> ws && getline(file, name, '|')
               && file >> balance) {

            if (accNo == accountNumber) {
                BankAccount account(accNo, name, balance);
                account.displayAccount();
                found = true;
                break;
            }

            file.ignore();
        }

        file.close();

        if (!found) {
            cout << "Account not found.\n";
        }
    }


    // Deposit money into an account
    void depositMoney() {
        int accountNumber;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Amount to Deposit: Rs. ";
        cin >> amount;

        updateBalance(accountNumber, amount, true);
    }


    // Withdraw money from an account
    void withdrawMoney() {
        int accountNumber;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Amount to Withdraw: Rs. ";
        cin >> amount;

        updateBalance(accountNumber, amount, false);
    }


    // Update balance in the file
    void updateBalance(int accountNumber, double amount, bool isDeposit) {

        if (amount <= 0) {
            cout << "Amount must be greater than 0.\n";
            return;
        }

        ifstream file(fileName);

        if (!file) {
            cout << "No account records found.\n";
            return;
        }

        // Temporary file used to store updated records
        ofstream tempFile("temp.txt");

        int accNo;
        string name;
        double balance;
        bool found = false;
        bool successful = false;

        while (file >> accNo >> ws && getline(file, name, '|')
               && file >> balance) {

            if (accNo == accountNumber) {

                found = true;

                BankAccount account(accNo, name, balance);

                if (isDeposit) {
                    account.deposit(amount);
                    successful = true;
                }
                else {
                    if (amount <= balance) {
                        account.withdraw(amount);
                        successful = true;
                    }
                    else {
                        account.withdraw(amount);
                    }
                }

                balance = account.getBalance();
            }

            // Write record to temporary file
            tempFile << accNo << "|" << name << "|"
                     << fixed << setprecision(2)
                     << balance << endl;

            file.ignore();
        }

        file.close();
        tempFile.close();

        if (!found) {
            cout << "Account not found.\n";
            remove("temp.txt");
            return;
        }

        // Replace old file with updated file
        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        if (successful) {
            cout << "Transaction completed successfully.\n";
        }
    }


    // Display all accounts
    void displayAllAccounts() {

        ifstream file(fileName);

        if (!file) {
            cout << "\nNo account records found.\n";
            return;
        }

        int accNo;
        string name;
        double balance;
        bool found = false;

        cout << "\n========== All Accounts ==========\n";

        while (file >> accNo >> ws && getline(file, name, '|')
               && file >> balance) {

            found = true;

            cout << "\nAccount Number : " << accNo << endl;
            cout << "Customer Name  : " << name << endl;
            cout << "Balance        : Rs. "
                 << fixed << setprecision(2) << balance << endl;

            file.ignore();
        }

        file.close();

        if (!found) {
            cout << "No accounts available.\n";
        }

        cout << "==================================\n";
    }
};


// Main function
int main() {

    BankManager bank;

    int choice;

    cout << "====================================\n";
    cout << "       BANK MANAGEMENT SYSTEM       \n";
    cout << "====================================\n";

    do {
        cout << "\n------------- MENU -------------\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "--------------------------------\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                bank.createAccount();
                break;

            case 2:
                bank.depositMoney();
                break;

            case 3:
                bank.withdrawMoney();
                break;

            case 4:
                bank.checkBalance();
                break;

            case 5:
                bank.displayAllAccounts();
                break;

            case 6:
                cout << "\nThank you for using the Bank Management System.\n";
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}