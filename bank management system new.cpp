#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>


using namespace std;


#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"n 


char name_[100], address_[100], password_[20];
int phone;
double balance = 0.0;
bool loggedIn = false;


void saveAccount() {
    ofstream file;
	file.open("ACCOUNT.txt",ios::app);
    file << name_ << endl
    << phone << endl
    << password_ << endl
    << address_ << endl
    << balance << endl;
    file.close();
}


void saveHistory(string type, double amount) {
    ofstream file("HISTORY.txt", ios::app);
    file << type << " : " << amount << " Birr" << endl;
    file.close();
}


bool loadAccount(int p, char pass[]) {
    ifstream file("ACCOUNT.txt");
    if (!file) return false;

    char tempName[100], tempAdd[100], tempPass[20];
    int tempPhone;
    double tempBalance;

    file.getline(tempName, 100);
    file >> tempPhone;
    file.ignore();
    file.getline(tempPass, 20);
    file.getline(tempAdd, 100);
    file >> tempBalance;

    if (tempPhone == p && strcmp(tempPass, pass) == 0) {
        strcpy(name_, tempName);
        strcpy(address_, tempAdd);
        strcpy(password_, tempPass);
        phone = tempPhone;
        balance = tempBalance;
        loggedIn = true;
        file.close();
        return true;
    }

    file.close();
    return false;
}


void createAccount() {
    cin.ignore();
    cout << CYAN << "Enter full name: " << RESET;
    cin.getline(name_, 100);

    cout << CYAN << "Enter phone number: " << RESET;
    cin >> phone;
    cin.ignore();

    cout << CYAN << "Set password: " << RESET;
    cin.getline(password_, 20);

    cout << CYAN << "Enter address: " << RESET;
    cin.getline(address_, 100);

    balance = 0;
    loggedIn = true;
    saveAccount();

    cout << GREEN << "Account created successfully!\n" << RESET;
}


void login() {
    int p;
    char pass[20];

    cout << CYAN << "Phone number: " << RESET;
    cin >> p;
    cin.ignore();

    cout << CYAN << "Password: " << RESET;
    cin.getline(pass, 20);

    if (loadAccount(p, pass))
        cout << GREEN << "Login successful. Welcome " << name_ << "!\n" << RESET;
    else
        cout << RED << "Invalid phone or password.\n" << RESET;
}


void deposit() {
    if (!loggedIn) {
        cout << RED << "Login required.\n" << RESET;
        return;
    }

    double amt;
    cout << CYAN << "Deposit amount: " << RESET;
    cin >> amt;

    if (amt > 0) {
        balance += amt;
        saveAccount();
        saveHistory("Deposit", amt);
        cout << GREEN << "Deposit successful.\n" << RESET;
    } else {
        cout << RED << "Invalid amount.\n" << RESET;
    }
}


void withdraw() {
    if (!loggedIn) {
        cout << RED << "Login required.\n" << RESET;
        return;
    }

    double amt;
    cout << CYAN << "Withdraw amount: " << RESET;
    cin >> amt;

    if (amt > 0 && amt <= balance) {
        balance -= amt;
        saveAccount();
        saveHistory("Withdraw", amt);
        cout << GREEN << "Withdrawal successful.\n" << RESET;
    } else {
        cout << RED << "Invalid or insufficient balance.\n" << RESET;
    }
}


void transferMoney() {
    if (!loggedIn) {
        cout << RED << "Login required.\n" << RESET;
        return;
    }

    int acc;
    double amt;

    cout << CYAN << "Receiver phone: " << RESET;
    cin >> acc;
    cout << CYAN << "Amount: " << RESET;
    cin >> amt;

    if (acc != phone && amt > 0 && amt <= balance) {
        balance -= amt;
        saveAccount();
        saveHistory("Transfer", amt);
        cout << GREEN << "Transfer successful.\n" << RESET;
    } else {
        cout << RED << "Transfer failed.\n" << RESET;
    }
}


void showHistory() {
    if (!loggedIn) {
        cout << RED << "Login required.\n" << RESET;
        return;
    }

    ifstream file("HISTORY.txt");
    cout << YELLOW << "\n--- Transaction History ---\n" << RESET;

    string line;
    while (getline(file, line))
        cout << line << endl;

    file.close();
}


void showBalance() {
    cout << BLUE << "Balance: " << fixed << setprecision(2)
         << balance << " Birr\n" << RESET;
}


int main() {
    int choice;

    do {
        cout << YELLOW << "\n===== BANK SYSTEM =====\n" << RESET;
        cout << "1. Create account\n";
        cout << "2. Login\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Show balance\n";
        cout << "7. Transaction history\n";
        cout << "8. Exit\n";

        cout << CYAN << "Choose: " << RESET;
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: transferMoney(); break;
            case 6: showBalance(); break;
            case 7: showHistory(); break;
            case 8: cout << GREEN << "Goodbye!\n" << RESET; break;
            default: cout << RED << "Invalid option.\n" << RESET;
        }
         cin.clear();
		 fflush(stdin);
    } while (choice != 8);

    return 0;
}