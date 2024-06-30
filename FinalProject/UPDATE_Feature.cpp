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











void border(char c, int length = 60) {
    for (int i = 0; i < length; i++) cout << c;
    cout << "\n";
}

void displayCenteredLine(const string &line, int width = 60) {
    int padLen = (width - line.size()) / 2;
    cout << string(padLen, ' ') << line << string(width - padLen - line.size(), ' ') << endl;
}

void displayCenteredLine(const string &line, const string &color, int width = 60) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << string(width - padLen - line.size(), ' ') << RESET << endl;
}

void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
    system("cls");
    #else
    system("clear");
    #endif
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












/*---------------------------------------------------------------------------------*/
/*------------------------------- EXPENSE CLASS -----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Expense {
private:
    string date;
    string category;
    string account;
    double amount;
    string description;

public:
    Expense(string, double, string, string, string);

    string getDate() const;
    string getCategory() const;
    string getDescription() const;
    string getAccount() const;
    double getAmount() const;

    friend ostream& operator<<(ostream& os, const Expense& expense);
    friend istream& operator>>(istream& is, Expense& expense);
};



/* EXPENSE Class (Public): CONSTRUCTOR */
/*-------------------------------------*/
Expense :: Expense(string Date, double Amt, string Cat, string Acc, string Desc) :
                date(Date),
                amount(Amt),
                category(Cat),
                account(Acc),
                description(Desc) {}



/* EXPENSE Class MFs(Public): GETTERS */
/*------------------------------------*/

string Expense :: getDate() const { return date; }
double Expense :: getAmount() const { return amount; }
string Expense :: getCategory() const { return category; }
string Expense :: getAccount() const { return account; }
string Expense :: getDescription() const { return description; }



// WRITE Expenses to file
ostream& operator<<(ostream& os, const Expense& expense)
{
    size_t size;

    // WRITE date
    size = expense.date.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.date.c_str(), size);

    // WRITE amount
    os.write(reinterpret_cast<const char*>(&expense.amount), sizeof(expense.amount));

    // WRITE category
    size = expense.category.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.category.c_str(), size);

    // WRITE account
    size  = expense.account.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.account.c_str(), size);

    // WRITE description
    size = expense.description.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.description.c_str(), size);

    return os;
}



// READ Expenses from file
istream& operator>>(istream& is, Expense& expense) {

    size_t size;

    // READ date
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date size' from file");
    expense.date.resize(size);
    is.read(&expense.date[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date' from file");

    // READ amount
    is.read(reinterpret_cast<char*>(&expense.amount), sizeof(expense.amount));
    if (!is.good()) throw runtime_error("Error reading amount from file");

    // READ category
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'category size' from file");
    expense.category.resize(size);
    is.read(&expense.category[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'category' from file");

    // READ account
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'account size' from file");
    expense.account.resize(size);
    is.read(&expense.account[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'account' from file");

    // READ description
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description size' from file");
    expense.description.resize(size);
    is.read(&expense.description[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description' from file");

    return is;
}













/*---------------------------------------------------------------------------------*/
/*------------------------------- ALLOWANCE CLASS ---------------------------------*/
/*---------------------------------------------------------------------------------*/

class Allowance {
private:
    string date;
    double amount;
    string account;
    string description;

public:
    Allowance(string date, double amount, string account, string description)
    : date(date), amount(amount), account(account), description(description) {}

    string getDate() const;
    double getAmount() const;
    string getAccount() const;
    string getDescription() const;

    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, Allowance& allowance);
};



/* ALLOWANCE class MF(Public): GETTERS */
/*-------------------------------------*/

string Allowance :: getDate() const         { return date; }
double Allowance :: getAmount() const       { return amount; }
string Allowance :: getAccount() const      { return account; }
string Allowance :: getDescription() const  { return description; }




/* ALLOWANCE class Friends (Public): File Handling Operators */
/*-----------------------------------------------------------*/

// WRITE Allowance data
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

// READ Allowance data
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











/*---------------------------------------------------------------------------------*/
/*--------------------------------- BUDGET CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Budget {
private:
    double totalBudget;
    vector<Expense> expenses;
    double expenseLimit;
    double savingsGoal;
    double currentSavings;
    string expenseStartDate;
    string expenseDueDate;
    string savingsStartDate;
    string savingsDueDate;
    vector<Allowance> allowancesList;

    void calculateCurrentSavings() {
        currentSavings = 0.0;
        for (const auto& allowance : allowancesList) {
            currentSavings += allowance.getAmount();
        }
    }

public:
    Budget();

    void loadExpenses();
    void saveExpenses() const;

    void loadAllowances();
    void saveAllowances() const;

    const string& getExpenseStartDate() const;
    const string& getExpenseDueDate() const;
    const string& getSavingsStartDate() const;
    const string& getSavingsDueDate() const;

    void setExpenseLimit(double);
    void setSavingsGoal(double);
    void setTotalBudget(double);

    void addExpense(const Expense&);
    void addAllowance(const Allowance&);
    
    void removeAllowance(int index);

    void updateExpenseDateRange(const string &, const string &);
    void updateSavingsDateRange(const string &, const string &);

    void displayUpdateMenu();
    void run_SetLimitExpenses();
    void run_UpdateSavings();
    void run_UpdateAllowance(int page = 0);

    void saveState();

};



/* BUDGET class(Public): CONSTRUCTOR */
/*-----------------------------------*/

Budget :: Budget() :
            totalBudget(0.0),
            expenseLimit(0.0),
            savingsGoal(0.0),
            currentSavings(0.0) {}



/* BUDGET class MFs(Public): LOAD & SAVE Functions - Allowance and Expenses */
/*--------------------------------------------------------------------------*/

void Budget :: loadExpenses() {
    ifstream inFile(ExpensesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Expense expense("", 0, "", "", "");
            inFile >> expense;
            expenses.push_back(expense);
        }
        inFile.close();
    }
}

void Budget :: saveExpenses() const {
    ofstream outFile(ExpensesFILE, ios::binary);
    if (outFile.is_open()) {
        for (const auto& expense : expenses) {
            outFile << expense;
        }
        outFile.close();
    }
}

void Budget :: loadAllowances()
{
    ifstream inFile(AllowancesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Allowance allowance("", 0.0, "", "");
            inFile >> allowance;
            allowancesList.push_back(allowance);
        }
        inFile.close();
        calculateCurrentSavings();
    }
}

void Budget :: saveAllowances() const
{
    ofstream outFile(AllowancesFILE, ios::binary);
    if (outFile.is_open()) {
        for (const auto& allowance : allowancesList) {
            outFile << allowance;
        }
        outFile.close();
    }
}



/* BUDGET class MFs(Public): SETTERS */
/*-----------------------------------*/

const string& Budget :: getExpenseStartDate() const     { return expenseStartDate; }
const string& Budget :: getExpenseDueDate() const       { return expenseDueDate; }
const string& Budget :: getSavingsStartDate() const     { return savingsStartDate; }
const string& Budget :: getSavingsDueDate() const       { return savingsDueDate; }



/* BUDGET class MFs(Public): SETTERS */
/*-----------------------------------*/

void Budget :: setExpenseLimit(double limit) { expenseLimit = limit; }
void Budget :: setSavingsGoal(double goal) { savingsGoal = goal; }
void Budget :: setTotalBudget(double budget) { totalBudget = budget; }

void Budget :: addExpense(const Expense& expense) {
    expenses.push_back(expense);
    saveExpenses();
}

void Budget :: addAllowance(const Allowance& allowance) {
    allowancesList.push_back(allowance);
    saveAllowances();
    calculateCurrentSavings();
}





void Budget :: removeAllowance(int index) {
    if (index >= 0 && index < allowancesList.size()) {
        allowancesList.erase(allowancesList.begin() + index);
        saveAllowances();
        calculateCurrentSavings();
    } else {
        throw runtime_error("Invalid allowance index.");
    }
}




void Budget :: updateExpenseDateRange(const string &startDate, const string &dueDate) {
    if (validateDateFormat(startDate) && validateDateFormat(dueDate) &&
        validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
        expenseStartDate = startDate;
        expenseDueDate = dueDate;
    } else {
        throw runtime_error("Invalid date format or date range for expenses.");
    }
}

void Budget :: updateSavingsDateRange(const string &startDate, const string &dueDate) {
    if (validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
        savingsStartDate = startDate;
        savingsDueDate = dueDate;
    } else {
        throw runtime_error("Invalid date range for savings.");
    }
}




void Budget :: displayUpdateMenu() {
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

void Budget :: run_SetLimitExpenses() {
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

void Budget :: run_UpdateSavings() {
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

void Budget :: run_UpdateAllowance(int page = 0) {
    const int itemsPerPage = 10;
    while (true) {
        clearScreen();
        const int menuWidth = 150;

        // Display: UPDATE ALLOWANCE title
        border(205, menuWidth);
        displayCenteredLine("UPDATE: ALLOWANCE", BLUE, menuWidth);
        border(205, menuWidth);

        displayCenteredLine(">> DATA:", BOLDWHITE, menuWidth);
        cout << "\n";

        int start = page * itemsPerPage;
        int end = min(start + itemsPerPage, static_cast<int>(allowancesList.size()));

        // Display headers
        cout << setw(15) << left << "Date" << setw(15) << "Amount" << setw(20) << "Account" << setw(50) << "Description" << endl;
        cout << string(menuWidth, '-') << endl;

        for (int i = start; i < end; ++i) {
            const Allowance &allowance = allowancesList[i];
            cout << setw(15) << left << allowance.getDate() << setw(15) << allowance.getAmount()
                    << setw(20) << allowance.getAccount() << setw(50) << allowance.getDescription() << endl;
        }

        cout << string(menuWidth, '-') << endl;
        displayCenteredLine("Options", BOLDWHITE, menuWidth);
        displayCenteredLine("[1] Previous Page", BOLDWHITE, menuWidth);
        displayCenteredLine("[2] Next Page", BOLDWHITE, menuWidth);
        displayCenteredLine("[3] Edit (ADD ALLOWANCE)", BOLDWHITE, menuWidth);
        displayCenteredLine("[4] Edit (DELETE ALLOWANCE)", BOLDWHITE, menuWidth);
        displayCenteredLine("[R] Return", BOLDWHITE, menuWidth);
        displayCenteredLine("", menuWidth);
        displayCenteredLine(">> Enter number: ", CYAN, menuWidth);

        string input;
        cin >> input;

        if (input == "1") {
            if (page > 0) page--;
        } else if (input == "2") {
            if ((page + 1) * itemsPerPage < allowancesList.size()) page++;
        } else if (input == "3") {
            string date, account, description;
            double amount;
            cout << "Enter date (MM/DD/YYYY): ";
            cin >> date;
            if (!validateDateFormat(date)) {
                cout << "Invalid date format. Please try again." << endl;
                continue;
            }
            cout << "Enter amount: ";
            cin >> amount;
            cout << "Enter account: ";
            cin >> account;
            cout << "Enter description: ";
            cin.ignore();
            getline(cin, description);

            Allowance newAllowance(date, amount, account, description);
            addAllowance(newAllowance);
            cout << "New allowance added." << endl;
        } else if (input == "4") {
            int index;
            cout << "Enter index to delete (1-" << allowancesList.size() << "): ";
            cin >> index;
            index--;
            if (index >= 0 && index < allowancesList.size()) {
                removeAllowance(index);
                cout << "Allowance deleted." << endl;
            } else {
                cout << "Invalid index. Please try again." << endl;
            }
        } else if (input == "R" || input == "r") {
            saveState();
            return;
        } else {
            cout << "Invalid option, please try again." << endl;
        }
    }
}

void Budget :: saveState() {
    // Save any necessary state to file
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
                budget.run_SetLimitExpenses();
            } else if (input == "2") {
                budget.run_UpdateSavings();
            } else if (input == "3") {
                budget.run_UpdateAllowance();
            } else if (input == "4") {
                // Handle option 4
            } else {
                cout << "Invalid option, please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
