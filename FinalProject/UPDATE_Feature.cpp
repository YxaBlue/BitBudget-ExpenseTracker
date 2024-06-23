#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <cstring>

using namespace std;

const string ExpensesFILE = "EXPENSEHistory.bin";
const string AllowancesFILE = "ALLOWANCEHistory.bin";

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLDWHITE "\033[1m\033[37m"

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GRAY    "\033[2m\033[37m"           /* Gray */

#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */

#define BOLDDARKGREEN   "\033[1m\033[2m\033[32m"       /* Bold Dark Green */








/*_________________________________________________________________________________*/
/*------------ FILENAMES: UPDATE, INBOX, and DATA & HISTORY FEATURE ---------------*/
/*_________________________________________________________________________________*/
const string UserStatusFILE = "UserSTATUS.bin";
const string LimitSavingsFILE = "LimitAndSavings.bin";

const string CategoryListFILE = "CATEGORYList.bin";
const string InboxFILE = "Inbox.bin";





/*_________________________________________________________________________________*/
/*-------------------------------- FUNCTIONS LIST: --------------------------------*/
/*_________________________________________________________________________________*/

void border(char, int);
void displayCenteredLine_NoColor(const string, int);
void displayCenteredLine_Colored(const string, const string, int);
void displayCenteredLine_NoNewLine(const string, const string, int);
void clearScreen();

void clearFile(string);
bool strIsNumeric(string);

bool validateDate(const string &);
bool validateDateFormat(const string &);







/*_________________________________________________________________________________*/
/*------------------------ DISPLAY TEXT-STYLE UI FUNCTIONS ------------------------*/
/*_________________________________________________________________________________*/
// Function to print a border with a given character
void border(char c, int length = 150) {
    for (int i = 0; i < length; i++) cout << c;
    cout << endl;
}

// Function to display centered lines
void displayCenteredLine_NoColor(const string &line, int width = 150) {
    int padLen = (width - line.size()) / 2;
    cout << string(padLen, ' ') << line << endl;
}

// Function to display centered lines with a color
void displayCenteredLine_Colored(const string &line, const string &color, int width = 150) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << RESET << endl;
}

// Function to display centered lines with color (no '\n')
void displayCenteredLine_NoNewLine(const string &line, const string &color, int width = 150) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << RESET;
}

// Function to clear the console screen
void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void displayCenteredLine(const string &line, int width = 60) {
    int padLen = (width - line.size()) / 2;
    cout << string(padLen, ' ') << line << string(width - padLen - line.size(), ' ') << endl;
}

void displayCenteredLine(const string &line, const string &color, int width = 60) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << string(width - padLen - line.size(), ' ') << RESET << endl;
}

bool validateDateFormat(const string &date) {
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

bool validateDate(const string &date) {
    if (!validateDateFormat(date)) {
        return false;
    }
    int month, day, year;
    sscanf(date.c_str(), "%d/%d/%d", &month, &day, &year);

    // Ensure month is from 1 to 12 and day is from 1 to 31
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    time_t t = time(nullptr);
    tm now = *localtime(&t);
    int currentYear = now.tm_year + 1900;
    int currentMonth = now.tm_mon + 1;
    int currentDay = now.tm_mday;

    if (year > currentYear || (year == currentYear && month > currentMonth) ||
        (year == currentYear && month == currentMonth && day >= currentDay)) {
        return true;
    }
    return false;
}

bool validateSecondDate(const string &firstDate, const string &secondDate) {
    if (!validateDateFormat(secondDate)) {
        return false;
    }
    int firstMonth, firstDay, firstYear;
    sscanf(firstDate.c_str(), "%d/%d/%d", &firstMonth, &firstDay, &firstYear);
    int secondMonth, secondDay, secondYear;
    sscanf(secondDate.c_str(), "%d/%d/%d", &secondMonth, &secondDay, &secondYear);

    // Ensure month is from 1 to 12 and day is from 1 to 31
    if (secondMonth < 1 || secondMonth > 12 || secondDay < 1 || secondDay > 31) {
        return false;
    }

    if (secondYear > firstYear || (secondYear == firstYear && secondMonth > firstMonth) ||
        (secondYear == firstYear && secondMonth == firstMonth && secondDay > firstDay)) {
        return true;
    }
    return false;
}

void displayUpdateMenu(); 

class Expense {
private:
    string category;
    string description;
    double amount;

public:
    Expense(string cat, string desc, double amt) : category(cat), description(desc), amount(amt) {}
    double getAmount() const { return amount; }
    string getDescription() const { return description; }

    friend ostream& operator<<(ostream& os, const Expense& expense);
    friend istream& operator>>(istream& is, Expense& expense);
};

ostream& operator<<(ostream& os, const Expense& expense) {
    size_t catSize = expense.category.size();
    size_t descSize = expense.description.size();
    os.write(reinterpret_cast<const char*>(&catSize), sizeof(catSize));
    os.write(expense.category.c_str(), catSize);
    os.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    os.write(expense.description.c_str(), descSize);
    os.write(reinterpret_cast<const char*>(&expense.amount), sizeof(expense.amount));
    return os;
}

istream& operator>>(istream& is, Expense& expense) {
    size_t catSize, descSize;
    is.read(reinterpret_cast<char*>(&catSize), sizeof(catSize));
    if (!is.good()) throw runtime_error("Error reading category size from file");
    expense.category.resize(catSize);
    is.read(&expense.category[0], catSize);
    if (!is.good()) throw runtime_error("Error reading category from file");

    is.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    if (!is.good()) throw runtime_error("Error reading description size from file");
    expense.description.resize(descSize);
    is.read(&expense.description[0], descSize);
    if (!is.good()) throw runtime_error("Error reading description from file");

    is.read(reinterpret_cast<char*>(&expense.amount), sizeof(expense.amount));
    if (!is.good()) throw runtime_error("Error reading amount from file");
    return is;
}

class Allowance {
private:
    string date;
    double amount;
    string account;
    string description;

public:
    Allowance(string date, double amount, string account, string description)
    : date(date), amount(amount), account(account), description(description) {}

    string getDate() const { return date; }
    double getAmount() const { return amount; }
    string getAccount() const { return account; }
    string getDescription() const { return description; }

    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, Allowance& allowance);
};

ostream& operator<<(ostream& os, const Allowance& allowance) {
    size_t dateSize = allowance.date.size();
    size_t accountSize = allowance.account.size();
    size_t descSize = allowance.description.size();
    os.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
    os.write(allowance.date.c_str(), dateSize);
    os.write(reinterpret_cast<const char*>(&accountSize), sizeof(accountSize));
    os.write(allowance.account.c_str(), accountSize);
    os.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    os.write(allowance.description.c_str(), descSize);
    os.write(reinterpret_cast<const char*>(&allowance.amount), sizeof(allowance.amount));
    return os;
}

istream& operator>>(istream& is, Allowance& allowance) {
    size_t dateSize, accountSize, descSize;
    is.read(reinterpret_cast<char*>(&dateSize), sizeof(dateSize));
    if (!is.good()) throw runtime_error("Error reading date size from file");
    allowance.date.resize(dateSize);
    is.read(&allowance.date[0], dateSize);
    if (!is.good()) throw runtime_error("Error reading date from file");

    is.read(reinterpret_cast<char*>(&accountSize), sizeof(accountSize));
    if (!is.good()) throw runtime_error("Error reading account size from file");
    allowance.account.resize(accountSize);
    is.read(&allowance.account[0], accountSize);
    if (!is.good()) throw runtime_error("Error reading account from file");

    is.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    if (!is.good()) throw runtime_error("Error reading description size from file");
    allowance.description.resize(descSize);
    is.read(&allowance.description[0], descSize);
    if (!is.good()) throw runtime_error("Error reading description from file");

    is.read(reinterpret_cast<char*>(&allowance.amount), sizeof(allowance.amount));
    if (!is.good()) throw runtime_error("Error reading amount from file");
    return is;
}

class Budget {
private:
    double totalBudget;
    double expenseLimit;

    double savingsGoal;
    double currentSavings;

    string expenseStartDate;
    string expenseDueDate;

    string savingsStartDate;
    string savingsDueDate;

    vector<Allowance> allowances;
    vector<Expense> expenses;

    void calculateCurrentSavings() {
        currentSavings = 0.0;
        for (const auto& allowance : allowances) {
            currentSavings += allowance.getAmount();
        }
    }

public:
    Budget() : totalBudget(0.0), expenseLimit(0.0), savingsGoal(0.0), currentSavings(0.0) {}

    void loadExpenses() {
        ifstream inFile(ExpensesFILE, ios::binary);
        if (inFile.is_open()) {
            while (inFile.peek() != EOF) {
                Expense expense("", "", 0.0);
                inFile >> expense;
                expenses.push_back(expense);
            }
            inFile.close();
        }
    }

    void saveExpenses() const {
        ofstream outFile(ExpensesFILE, ios::binary);
        if (outFile.is_open()) {
            for (const auto& expense : expenses) {
                outFile << expense;
            }
            outFile.close();
        }
    }

    void loadAllowances() {
        ifstream inFile(AllowancesFILE, ios::binary);
        if (inFile.is_open()) {
            while (inFile.peek() != EOF) {
                Allowance allowance("", 0.0, "", "");
                inFile >> allowance;
                allowances.push_back(allowance);
            }
            inFile.close();
            calculateCurrentSavings();
        }
    }

    void saveAllowances() const {
        ofstream outFile(AllowancesFILE, ios::binary);
        if (outFile.is_open()) {
            for (const auto& allowance : allowances) {
                outFile << allowance;
            }
            outFile.close();
        }
    }

    void setExpenseLimit(double limit) {
        expenseLimit = limit;
    }

    void setSavingsGoal(double goal) {
        savingsGoal = goal;
    }

    void addExpense(const Expense& expense) {
        expenses.push_back(expense);
        saveExpenses();
    }

    void addAllowance(const Allowance& allowance) {
        allowances.push_back(allowance);
        saveAllowances();
        calculateCurrentSavings();
    }

    void removeAllowance(int index) {
        if (index >= 0 && index < allowances.size()) {
            allowances.erase(allowances.begin() + index);
            saveAllowances();
            calculateCurrentSavings();
        } else {
            throw runtime_error("Invalid allowance index.");
        }
    }

    void updateExpenseDateRange(const string &startDate, const string &dueDate) {
        if (validateDateFormat(startDate) && validateDateFormat(dueDate) &&
            validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
            expenseStartDate = startDate;
            expenseDueDate = dueDate;
        } else {
            throw runtime_error("Invalid date format or date range for expenses.");
        }
    }

    void updateSavingsDateRange(const string &startDate, const string &dueDate) {
        if (validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
            savingsStartDate = startDate;
            savingsDueDate = dueDate;
        } else {
            throw runtime_error("Invalid date range for savings.");
        }
    }

    void setTotalBudget(double budget) {
        totalBudget = budget;
    }

    void displayUpdateMenu() {
        clearScreen();
        const int menuWidth = 150;

        border(205, menuWidth);
        displayCenteredLine("UPDATE", BLUE, menuWidth);
        border(205, menuWidth);
        displayCenteredLine("Options", BOLDWHITE, menuWidth);
        displayCenteredLine("", menuWidth);
        displayCenteredLine("[ 1 ] Set LIMIT OF EXPENSES", WHITE, menuWidth);
        displayCenteredLine("[ 2 ] Set SAVINGS", WHITE, menuWidth);
        displayCenteredLine("", menuWidth);
        displayCenteredLine("[ 3 ] Update ALLOWANCE", WHITE, menuWidth);
        displayCenteredLine("[ 4 ] Update TRANSACTION", WHITE, menuWidth);
        displayCenteredLine("", menuWidth);
        displayCenteredLine("[ R ] Return", WHITE, menuWidth);
        displayCenteredLine("", menuWidth);
        border(205, menuWidth);
        displayCenteredLine(">> Enter number: ", CYAN, menuWidth);
    }

    void displaySetLimitExpensesMenu() {
        while (true) {
            clearScreen();
            const int menuWidth = 150;

            border(205, menuWidth);
            displayCenteredLine("UPDATE: Set LIMIT OF EXPENSES", BLUE, menuWidth);
            border(205, menuWidth);
            displayCenteredLine(">> EXPENSES Limit", BOLDWHITE, menuWidth);
            displayCenteredLine("*Date: [" + getExpenseStartDate() + " - " + getExpenseDueDate() + "]", WHITE, menuWidth);
            displayCenteredLine("*LIMIT GOAL:                         P" + to_string(expenseLimit), WHITE, menuWidth);
            displayCenteredLine("*Current Expenses:                   P", WHITE, menuWidth);  
            displayCenteredLine("", menuWidth);
            displayCenteredLine("", menuWidth);
            displayCenteredLine("", menuWidth);
            border(205, menuWidth);

            displayCenteredLine("Options", BOLDWHITE, menuWidth);
            displayCenteredLine("[1] SET/REWRITE GOAL", BOLDWHITE, menuWidth);
            displayCenteredLine("[2] SET/REWRITE DATE", BOLDWHITE, menuWidth);
            displayCenteredLine("[R] Return", BOLDWHITE, menuWidth);

            displayCenteredLine("", menuWidth);
            displayCenteredLine(">> Enter number: ", CYAN, menuWidth);

            string input;
            cin >> input;

            if (input == "1") {
                double newGoal;
                cout << "Enter new expense limit goal: ";
                cin >> newGoal;
                setExpenseLimit(newGoal);
                cout << "New expense limit goal set to P" << newGoal << endl;
            } else if (input == "2") {
                string startDate, dueDate;
                cout << "Enter start date (MM/DD/YYYY): ";
                cin >> startDate;
                if (startDate == "R" || startDate == "r") {
                    continue; 
                }

                cout << "Enter due date (MM/DD/YYYY): ";
                cin >> dueDate;
                if (dueDate == "R" || dueDate == "r") {
                    continue; 
                }

                // Validate and update date range
                try {
                    updateExpenseDateRange(startDate, dueDate);
                    cout << "Expense date range set from " << startDate << " to " << dueDate << endl;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            } else if (input == "R" || input == "r") {
                saveState();
                return; 
            } else {
                cout << "Invalid option, please try again." << endl;
            }
        }
    }

    void displaySetSavingsMenu() {
        while (true) {
            clearScreen();
            const int menuWidth = 150;

            border(205, menuWidth);
            displayCenteredLine("UPDATE: SAVINGS", BLUE, menuWidth);
            border(205, menuWidth);
            displayCenteredLine(">> SAVINGS", BOLDWHITE, menuWidth);
            displayCenteredLine("*Date: [" + getSavingsStartDate() + " - " + getSavingsDueDate() + "]", WHITE, menuWidth);
            displayCenteredLine("*Savings GOAL:                       P" + to_string(savingsGoal), WHITE, menuWidth);
            displayCenteredLine("*Current Savings:                    P" + to_string(currentSavings) + "/" + to_string(savingsGoal), WHITE, menuWidth);
            displayCenteredLine("", menuWidth);
            displayCenteredLine("", menuWidth);
            displayCenteredLine("", menuWidth);
            border(205, menuWidth);

            displayCenteredLine("Options", BOLDWHITE, menuWidth);
            displayCenteredLine("[1] SET/REWRITE GOAL", BOLDWHITE, menuWidth);
            displayCenteredLine("[2] SET/REWRITE DATE", BOLDWHITE, menuWidth);
            displayCenteredLine("[R] Return", BOLDWHITE, menuWidth);

            displayCenteredLine("", menuWidth);
            displayCenteredLine(">> Enter number: ", CYAN, menuWidth);

            string input;
            cin >> input;

            if (input == "1") {
                double newGoal;
                cout << "Enter new savings goal: ";
                cin >> newGoal;
                setSavingsGoal(newGoal);
                cout << "New savings goal set to P" << newGoal << endl;
            } else if (input == "2") {
                string startDate, dueDate;
                cout << "Enter start date (MM/DD/YYYY): ";
                cin >> startDate;
                if (startDate == "R" || startDate == "r") {
                    continue;
                }

                cout << "Enter due date (MM/DD/YYYY): ";
                cin >> dueDate;
                if (dueDate == "R" || dueDate == "r") {
                    continue; 
                }

                // Validate and update date range
                try {
                    updateSavingsDateRange(startDate, dueDate);
                    cout << "Savings date range set from " << startDate << " to " << dueDate << endl;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            } else if (input == "R" || input == "r") {
                saveState();
                return; 
            } else {
                cout << "Invalid option, please try again." << endl;
            }
        }
    }

    void displayUpdateAllowanceMenu(int page = 0) {
        const int itemsPerPage = 10;
        while (true) {
            clearScreen();
            const int menuWidth = 150;

            border(205, menuWidth);
            displayCenteredLine("UPDATE: ALLOWANCE (EDIT)", BLUE, menuWidth);
            border(205, menuWidth);
            displayCenteredLine(">> DATA:", BOLDWHITE, menuWidth);
            displayCenteredLine("", menuWidth);

            int start = page * itemsPerPage;
            int end = min(start + itemsPerPage, static_cast<int>(allowances.size()));

            // Display headers
            cout << setw(15) << left << "Date" << setw(15) << "Amount" << setw(20) << "Account" << setw(50) << "Description" << endl;
            cout << string(menuWidth, '-') << endl;

            for (int i = start; i < end; ++i) {
                const Allowance &allowance = allowances[i];
                cout << setw(15) << left << allowance.getDate() << setw(15) << allowance.getAmount()
                     << setw(20) << allowance.getAccount() << setw(50) << allowance.getDescription() << endl;
            }

            cout << string(menuWidth, '-') << endl;
            displayCenteredLine("Options", BOLDWHITE, menuWidth);
            displayCenteredLine("[ 1 ] Previous Page          [ 3 ] Edit (ADD ALLOWANCE)   ", BOLDWHITE, menuWidth);
            displayCenteredLine("[ 2 ] Next Page              [ 4 ] Edit (DELETE ALLOWANCE)", BOLDWHITE, menuWidth);
            displayCenteredLine("[ R ] Return", BOLDWHITE, menuWidth);
            displayCenteredLine("", menuWidth);
            displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

            string input;
            cin >> input;

            if (input == "1") {
                if (page > 0) page--;
            }
            
            else if (input == "2") {
                if ((page + 1) * itemsPerPage < allowances.size()) page++;
            }
            
            else if (input == "3") {
                string date, account, description;
                double amount;
                cout << "Enter date (MM/DD/YYYY): ";
                cin >> date;
                if (!validateDateFormat(date)) {
                    cout << "Invalid date format. Please try again." << endl;
                    continue;
                }
                cout << "Enter amount:            ";
                cin >> amount;
                cout << "Enter account:           ";
                cin >> account;
                cout << "Enter description:       ";
                cin.ignore();
                getline(cin, description);

                Allowance newAllowance(date, amount, account, description);
                addAllowance(newAllowance);

                cout << "New allowance added." << endl;
                getchar();
            }
            
            else if (input == "4") {
                int index;
                cout << "Enter index to delete (1-" << allowances.size() << "): ";
                cin >> index;
                index--;
                if (index >= 0 && index < allowances.size()) {
                    removeAllowance(index);
                    cout << "Allowance deleted." << endl;
                    getchar();

                } else {
                    cout << "Invalid index. Please try again." << endl;
                }
            }
            
            else if (input == "R" || input == "r") {
                saveState();
                return;
            }
            
            else {
                cout << "Invalid option, please try again." << endl;
                getchar();
            }
        }
    }

    void saveState() {
        // Save any necessary state to file
    }

    const string& getExpenseStartDate() const {
        return expenseStartDate;
    }

    const string& getExpenseDueDate() const {
        return expenseDueDate;
    }

    const string& getSavingsStartDate() const {
        return savingsStartDate;
    }

    const string& getSavingsDueDate() const {
        return savingsDueDate;
    }
};


/*---------------------------------------------------------------------*/
class Transaction
{
private:
    vector<Allowance> Allowance_History;
    vector<Expense> Expense_History;
    vector<Budget> UserStatus;



public:
    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, const Allowance& allowance);

    
    
};

void displayTitle_Transaction();
void displayMainMenu_Transaction();
void displayOptions_Transaction(int);
void run_TRANSACTION();


void displayTitle_Transaction() {
    border(205);
    displayCenteredLine_Colored("UPDATE: TRANSACTION", BLUE);
    border(205);
}

// TRANSACTION: Display OPTIONS. (1-Transaction Menu,, 2-OptionDelete)
void displayOptions_Transaction(int option) {
    switch (option) {
        case 1:
            /* TRANSACTION MENU Options */
            displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
            displayCenteredLine_NoColor("[ 1 ]  ADD   ");
            displayCenteredLine_NoColor("[ 2 ]  DELETE");
            displayCenteredLine_NoColor("[ R ]  RETURN");
            cout << "\n\n";
            border(196);
            displayCenteredLine_NoNewLine(">> Enter choice:  ", CYAN);
            break;

        case 2:
            /* DELETE TRANSACTION Options */
            displayCenteredLine_Colored("OPTIONS: DELETE", BOLDWHITE);
            displayCenteredLine_NoColor(">> Enter a TRANSACTION NUMBER to DELETE      ");
            displayCenteredLine_NoColor(">> Enter [ R ] to RETURN to Transaction Menu.");
            cout << "\n\n";
            displayCenteredLine_NoNewLine(">> Enter number/choice: ", CYAN);
            break;

        default:
            break;
    }
}

void run_TRANSACTION() {
    string choice_Str;
    int choice_Int;

    while (true) {
        do {
            /* ------------- TRANSACTION: MAIN MENU ------------- */
            clearScreen();
            displayTitle_Transaction();

            // display STATUS: Current allowance, List of transactions of today
            
            cout << "\n\n";
            border(196);
            displayOptions_Transaction(1);
            getline(cin, choice_Str);

            if ((choice_Str == "R") || (choice_Str == "r")) {
                // End TRANSACTION feature, return to MAIN MENU
                return;
            }
        } while ((choice_Str != "1") && (choice_Str != "2") && (choice_Str != "3"));
        
        choice_Int = stoi(choice_Str);

        switch (choice_Int) {
            case 1:
                /* ---------- ADD Transaction Feature ---------- */
                break;

            case 2:
                /* ---------- DELETE Transaction Feature ---------- */
                break;
            
            case 3:
                /* ---------- EDIT Transaction Feature ---------- */
                break;

            default:
                break;
        }
    }
}










int main() {
    Budget budget;
    budget.loadExpenses();
    budget.loadAllowances();
    budget.setTotalBudget(0.0);

    string input;
    while (true) {
        budget.displayUpdateMenu();
        cin >> input;

        if (input == "R" || input == "r") break;

        try {
            if (input == "1") {
                budget.displaySetLimitExpensesMenu();

            } else if (input == "2") {
                budget.displaySetSavingsMenu();
            } else if (input == "3") {
                budget.displayUpdateAllowanceMenu();
            } else if (input == "4") {
                run_TRANSACTION();
            } else {
                cout << "Invalid option, please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}