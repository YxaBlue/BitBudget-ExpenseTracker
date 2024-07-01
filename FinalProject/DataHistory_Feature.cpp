#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <list> 
#include <fstream>
using namespace std;



#define COLUMN_WIDTH 10
#define SCREENWIDTH 150



/*_________________________________________________________________________________*/
/*------------------------------- COLOR DEFINITIONS -------------------------------*/
/*_________________________________________________________________________________*/

#define RESET   "\033[0m"
#define BLACK   "\033[30m"                  /* Black */
#define RED     "\033[31m"                  /* Red */
#define GREEN   "\033[32m"                  /* Green */
#define YELLOW  "\033[33m"                  /* Yellow */
#define BLUE    "\033[34m"                  /* Blue */
#define MAGENTA "\033[35m"                  /* Magenta */
#define CYAN    "\033[36m"                  /* Cyan */
#define WHITE   "\033[37m"                  /* White */
#define GRAY    "\033[2m\033[37m"           /* Gray */

#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */

#define BOLDDARKGREEN   "\033[1m\033[2m\033[32m"       /* Bold Dark Green */








/*_________________________________________________________________________________*/
/*------------ FILENAMES: UPDATE, INBOX, and DATA & HISTORY FEATURE ---------------*/
/*_________________________________________________________________________________*/

const string ExpensesFILE = "EXPENSEHistory.bin";
const string AllowancesFILE = "ALLOWANCEHistory.bin";

const string UserStatusFILE = "UserSTATUS.bin";
const string LimitSavingsFILE = "LimitAndSavings.bin";

const string CategoryListFILE = "CATEGORYList.bin";
const string InboxFILE = "Inbox.bin";








/*_________________________________________________________________________________*/
/*-------------------------------- FUNCTIONS LIST: --------------------------------*/
/*_________________________________________________________________________________*/

void border(char, int);
void displayCenteredLine_Colored(const string &, const string &, int);
void displayCenteredLine_NoColor(const string &, int);
void displayCenteredLine_NoNewLine(const string &, const string &, int);
void clearScreen();

void displayTxtByColumn(const string &, const string &, int, int);
void displayTxtByColumn_CENTERED(const string &, const string &, int, int);
void displayTxtByColumn_NB(const string &, const string &, int, int);
void displayTxtByColumn_CENTERED_NB(const string &, const string &, int, int);


bool IsNumeric(string);


bool validateDateFormat(const string &);
bool validateDate(const string &);
bool validateSecondDate(const string &, const string &);

string getDate_Today();
string getMonth(const string&);
string getDay(const string&);
string getYear(const string&);


void displayUpdateMenu();









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



// Function to display lines by column
void displayTxtByColumn(const string &str, const string &color, int columns = 2, int width = 150)
{
    int columnWidth = width / columns;
    int spaceLen = columnWidth - str.size() - 1;
    char border = 179;
    cout << color << " " << str << string(spaceLen, ' ') << RESET << border;
}

// Function to display centered lines, 
void displayTxtByColumn_CENTERED(const string &txt, const string &color, int columns = 2, int width = 150)
{
    int columnWidth = width / columns;
    int totalPadding = columnWidth - txt.size() - 1;
    int spaceLen_Left = totalPadding / 2;
    int spaceLen_Right = totalPadding - spaceLen_Left;
    char border = 179;

    if (spaceLen_Left < 0) spaceLen_Left = 0;
    if (spaceLen_Right < 0) spaceLen_Right = 0;

    cout << color << " " << string(spaceLen_Left, ' ') << txt << string(spaceLen_Right, ' ') << RESET << border;
}

// Function to display lines by column
void displayTxtByColumn_NB(const string &str, const string &color, int columns = 2, int width = 150)
{
    int columnWidth = width / columns;
    int spaceLen = columnWidth - str.size();
    cout << color << " " << str << string(spaceLen, ' ') << RESET;
}

// Function to display centered lines, 
void displayTxtByColumn_CENTERED_NB(const string &txt, const string &color, int columns = 2, int width = 150)
{
    int columnWidth = width / columns;
    int totalPadding = columnWidth - txt.size();
    int spaceLen_Left = totalPadding / 2;
    int spaceLen_Right = totalPadding - spaceLen_Left;

    if (spaceLen_Left < 0) spaceLen_Left = 0;
    if (spaceLen_Right < 0) spaceLen_Right = 0;

    cout << color << " " << string(spaceLen_Left, ' ') << txt << string(spaceLen_Right, ' ') << RESET;
}



// Function to clear the console screen
void clearScreen()
{
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}







/*_________________________________________________________________________________*/
/*------------------------------- EXTRA FUNCTIONS ---------------------------------*/
/*_________________________________________________________________________________*/

// check if string is NUMERIC
bool IsNumeric(string str) {
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            return false;
        }
    }

    return true;
}



// validate Date Format
bool validateDateFormat(const string &date) {
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

// validate date to be present or future date
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

// validate second date based on first date
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

// Get today's date
string getDate_Today()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string(5 + ltm->tm_mday);

    if (month.size() == 1) month = "0" + month;
    if (day.size() == 1) day = "0" + day;

    string dateToday = month + "/" + day + "/" + year;

    return dateToday; 
}

string getMonth(const string& date)
{
    if (validateDateFormat(date)) {
        string month_Str = date.substr(0, 2);
        return month_Str;
    }

    return "";
}


string getYear(const string& date)
{
    if (validateDateFormat(date)) {
        string year_Str = date.substr(6, 4);
        return year_Str;
    }

    return "";
}


string getDay(const string& date)
{
    if (validateDateFormat(date)) {
        string day_Str = date.substr(3, 2);
        return day_Str;
    }

    return "";
}











/*_________________________________________________________________________________*/
/*------------------------------- CATEGORY CLASS ----------------------------------*/
/*_________________________________________________________________________________*/

class Category {
private:
    string categoryName_parent;
    int totalBabyCategories;
    vector<string> categoryName_baby;

public:
    // Constructors
    Category(string, int);


    // Category getters
    string getParentCategory() const;
    int getTotalBabyCategories() const;
    vector<string> getBabyCategories() const;


    // Category Public MFs
    void add_BabyCategory(string);

    void displayParentCategories(int);
    void displayBabyCategories(string);


    // Friend Functions
    friend ostream& operator<<(ostream& outFILE, const Category &categoryHol);
    friend istream& operator>>(istream& inFILE, Category &categoryHol);
    
};


// CATEGORY CLASS: Constructors
Category :: Category(string catName_parent, int babyCat_Total) :
                categoryName_parent(catName_parent),
                totalBabyCategories(babyCat_Total) {}



// CATEGORY CLASS: Getters
string Category :: getParentCategory() const            { return categoryName_parent; }
int Category :: getTotalBabyCategories() const          { return totalBabyCategories; }
vector<string> Category :: getBabyCategories() const    { return categoryName_baby; }



// CATEGORY CLASS: Public MFs
void Category :: add_BabyCategory(string babyCatName)
{
    categoryName_baby.push_back(babyCatName);
    totalBabyCategories = categoryName_baby.size();
}






// WRITE category/ies
ostream& operator<<(ostream& outFILE, const Category &categoryHol)
{
    size_t size;


    // WRITE parent category name
    size = categoryHol.categoryName_parent.size();
    outFILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outFILE.write(categoryHol.categoryName_parent.c_str(), size);

    // WRITE total baby categories
    outFILE.write(reinterpret_cast<const char*>(&categoryHol.totalBabyCategories), sizeof(categoryHol.totalBabyCategories));

    // WRITE all baby categories
    for(const auto& babyCategory : categoryHol.categoryName_baby)
    {
        size = babyCategory.size();
        outFILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFILE.write(babyCategory.c_str(), size);
    }

    return outFILE;
}


// READ category/ies
istream& operator>>(istream& inFILE, Category &categoryHol)
{
    size_t size;
    string catNameHol;

    // READ Parent Category Name
    inFILE.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!inFILE.good()) throw runtime_error(">> ERROR: Unable to read 'parent category name size' from file");
    categoryHol.categoryName_parent.resize(size);
    inFILE.read(&categoryHol.categoryName_parent[0], size);
    if (!inFILE.good()) throw runtime_error(">> ERROR: Unable to read 'parent category name' from file");


    // READ Total Baby Categories
    inFILE.read(reinterpret_cast<char*>(&categoryHol.totalBabyCategories), sizeof(categoryHol.totalBabyCategories));
    if(!inFILE.good()) throw runtime_error(">> ERROR: Unable to read 'Total baby category/ies name' from file");


    // READ Baby Category Name/s
    for (int i = 0; i < categoryHol.totalBabyCategories; i++)
    {
        inFILE.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (!inFILE.good()) throw runtime_error(">> ERROR: Unable to read 'baby category name size' from file");
        catNameHol.resize(size);
        inFILE.read(&catNameHol[0], size);
        if (!inFILE.good()) throw runtime_error(">> ERROR: Unable to read 'baby category name' from file");
        categoryHol.categoryName_baby.push_back(catNameHol); 

    }

    return inFILE;
}










/*_________________________________________________________________________________*/
/*------------------------------- EXPENSE CLASS -----------------------------------*/
/*_________________________________________________________________________________*/

class Expense {
private:
    string date;
    string category;
    string account;
    double amount;
    string description;

public:
    Expense(string, double, string, string, string);

    // Expense data member GETTERS
    string getDate() const;
    string getCategory() const;
    string getDescription() const;
    string getAccount() const;
    double getAmount() const;

    // Expense data member SETTERS
    void setDate(string) const;
    void setCategory(string) const;
    void setDescription(string) const;
    void setAmount(double) const;
    void setAccount(string) const;

    // Friend Functions
    friend ostream& operator<<(ostream& os, const Expense& expense);
    friend istream& operator>>(istream& is, Expense& expense);
};



// EXPENSE CLASS: Constructors
Expense :: Expense(string Date, double Amt, string Cat, string Acc, string Desc) :
                date(Date),
                amount(Amt),
                category(Cat),
                account(Acc),
                description(Desc) {}


// EXPENSE CLASS: Data Member Getters
string Expense :: getDate() const               { return date; }
double Expense :: getAmount() const             { return amount; }
string Expense :: getCategory() const           { return category; }
string Expense :: getAccount() const            { return account; }
string Expense :: getDescription() const        { return description; }




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
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'amount' from file");


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











/*_________________________________________________________________________________*/
/*------------------------------- ALLOWANCE CLASS ---------------------------------*/
/*_________________________________________________________________________________*/

class Allowance {
private:
    string date;
    double amount;
    string account;
    string description;


public:
    // Allowance CONSTRUCTOR
    Allowance(string date, double amount, string account, string description)
    : date(date), amount(amount), account(account), description(description) {}

    // Allowance data GETTERS
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
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date size' from file");
    allowance.date.resize(dateSize);
    is.read(&allowance.date[0], dateSize);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date' from file");

    is.read(reinterpret_cast<char*>(&accountSize), sizeof(accountSize));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'account size' from file");
    allowance.account.resize(accountSize);
    is.read(&allowance.account[0], accountSize);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'account' from file");

    is.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description' size from file");
    allowance.description.resize(descSize);
    is.read(&allowance.description[0], descSize);
    if (!is.good()) throw runtime_error("Error reading description from file");

    is.read(reinterpret_cast<char*>(&allowance.amount), sizeof(allowance.amount));
    if (!is.good()) throw runtime_error("Error reading amount from file");
    return is;
}








/*_________________________________________________________________________________*/
/*------------------------------- BUDGET CLASS ------------------------------------*/
/*_________________________________________________________________________________*/

class Budget {
private:
    double totalBudget;
    vector<Expense> expenses;
    double expenseLimit;
    double savingsGoal;
    double totalSavings;
    string expenseStartDate;
    string expenseDueDate;
    string savingsStartDate;
    string savingsDueDate;
    vector<Allowance> allowancesList;

    void calculateCurrentSavings();

public:
    // Constructor
    Budget();

    // READ/WRITE Expenses & Allowances
    void loadExpenses();
    void saveExpenses() const;
    void loadAllowances();
    void saveAllowances() const;
    void saveState();

    // Setters
    void setExpenseLimit(double limit);
    void setSavingsGoal(double goal);
    void setTotalBudget(double budget);

    // Adders (Expenses/Allowance)
    void addExpense(const Expense& expense);
    void addAllowance(const Allowance& allowance);

    void removeAllowance(int index);

    // Date Updaters
    void updateExpenseDateRange(const string &startDate, const string &dueDate);
    void updateSavingsDateRange(const string &startDate, const string &dueDate);

    // Display Menu
    void displayUpdateMenu();
    void run_SetLimitExpenses();
    void run_UpdateSavings();
    void run_UpdateAllowance(int);

    // Getters
    const string& getExpenseStartDate() const;
    const string& getExpenseDueDate() const;
    const string& getSavingsStartDate() const;
    const string& getSavingsDueDate() const;
};




/* --------------------------------------------------------------------------------------- */
/* ------------------------------ BUDGET CLASS: PRIVATE MFs ------------------------------ */
/* --------------------------------------------------------------------------------------- */

void Budget :: calculateCurrentSavings()
{
    totalSavings = 0.0;
    for (const auto& allowance : allowancesList) {
        totalSavings += allowance.getAmount();
    }
}





/* --------------------------------------------------------------------------------------- */
/* ----------------------- BUDGET CLASS: PUBLIC MFs and CONSTRUCTORS --------------------- */
/* --------------------------------------------------------------------------------------- */

// Budget Constructor
Budget :: Budget() :
                    totalBudget(0.0),
                    expenseLimit(0.0),
                    savingsGoal(0.0),
                    totalSavings(0.0) {}


void Budget :: loadExpenses()
{
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


void Budget :: saveExpenses() const
{
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


void Budget :: setExpenseLimit(double limit)
{
    expenseLimit = limit;
}


void Budget :: setSavingsGoal(double goal)
{
    savingsGoal = goal;
}


void Budget :: addExpense(const Expense& expense)
{
    expenses.push_back(expense);
    saveExpenses();
}


void Budget :: addAllowance(const Allowance& allowance)
{
    allowancesList.push_back(allowance);
    saveAllowances();
    calculateCurrentSavings();
}


void Budget :: removeAllowance(int index)
{
    if (index >= 0 && index < allowancesList.size()) {
        allowancesList.erase(allowancesList.begin() + index);
        saveAllowances();
        calculateCurrentSavings();
    } else {
        throw runtime_error("Invalid allowance index.");
    }
}


void Budget :: updateExpenseDateRange(const string &startDate, const string &dueDate)
{
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


void Budget :: setTotalBudget(double budget)
{
    totalBudget = budget;
}


void Budget :: displayUpdateMenu()
{
    clearScreen();
    const int menuWidth = 150;

    border(205, menuWidth);
    displayCenteredLine_Colored("UPDATE", BLUE, menuWidth);
    border(205, menuWidth);
    displayCenteredLine_Colored("Options", BOLDWHITE, menuWidth);
    displayCenteredLine_NoColor("", menuWidth);
    displayCenteredLine_Colored("[ 1 ] Set LIMIT OF EXPENSES", WHITE, menuWidth);
    displayCenteredLine_Colored("[ 2 ] Set SAVINGS", WHITE, menuWidth);
    displayCenteredLine_NoColor("", menuWidth);
    displayCenteredLine_Colored("[ 3 ] Update ALLOWANCE", WHITE, menuWidth);
    displayCenteredLine_Colored("[ 4 ] Update TRANSACTION", WHITE, menuWidth);
    displayCenteredLine_NoColor("", menuWidth);
    displayCenteredLine_Colored("[ R ] Return", WHITE, menuWidth);
    displayCenteredLine_NoColor("", menuWidth);
    border(205, menuWidth);
    displayCenteredLine_Colored(">> Enter number: ", CYAN, menuWidth);
}


void Budget :: run_SetLimitExpenses() {
    while (true) {
        clearScreen();
        const int menuWidth = 150;

        border(205, menuWidth);
        displayCenteredLine_Colored("UPDATE: Set LIMIT OF EXPENSES", BLUE, menuWidth);
        border(205, menuWidth);
        displayCenteredLine_Colored(">> EXPENSES Limit", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("*Date: [" + getExpenseStartDate() + " - " + getExpenseDueDate() + "]", WHITE, menuWidth);
        displayCenteredLine_Colored("*LIMIT GOAL:                         P" + to_string(expenseLimit), WHITE, menuWidth);
        displayCenteredLine_Colored("*Current Expenses:                   P", WHITE, menuWidth);  
        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        border(205, menuWidth);

        displayCenteredLine_Colored("Options", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[1] SET/REWRITE GOAL", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[2] SET/REWRITE DATE", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[R] Return", BOLDWHITE, menuWidth);

        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_Colored(">> Enter number: ", CYAN, menuWidth);

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
        displayCenteredLine_Colored("UPDATE: SAVINGS", BLUE, menuWidth);
        border(205, menuWidth);
        displayCenteredLine_Colored(">> SAVINGS", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("*Date: [" + getSavingsStartDate() + " - " + getSavingsDueDate() + "]", WHITE, menuWidth);
        displayCenteredLine_Colored("*Savings GOAL:                       P" + to_string(savingsGoal), WHITE, menuWidth);
        displayCenteredLine_Colored("*Current Savings:                    P" + to_string(totalSavings) + "/" + to_string(savingsGoal), WHITE, menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        border(205, menuWidth);

        displayCenteredLine_Colored("Options", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[1] SET/REWRITE GOAL", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[2] SET/REWRITE DATE", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[R] Return", BOLDWHITE, menuWidth);

        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_Colored(">> Enter number: ", CYAN, menuWidth);

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

        border(205, menuWidth);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT)", BLUE, menuWidth);
        border(205, menuWidth);
        displayCenteredLine_Colored(">> DATA:", BOLDWHITE, menuWidth);
        displayCenteredLine_NoColor("", menuWidth);

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
        displayCenteredLine_Colored("Options", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[1] Previous Page", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[2] Next Page", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[3] Edit (ADD ALLOWANCE)", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[4] Edit (DELETE ALLOWANCE)", BOLDWHITE, menuWidth);
        displayCenteredLine_Colored("[R] Return", BOLDWHITE, menuWidth);
        displayCenteredLine_NoColor("", menuWidth);
        displayCenteredLine_Colored(">> Enter number: ", CYAN, menuWidth);

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

const string& Budget :: getExpenseStartDate() const {
    return expenseStartDate;
}

const string& Budget :: getExpenseDueDate() const {
    return expenseDueDate;
}

const string& Budget :: getSavingsStartDate() const {
    return savingsStartDate;
}

const string& Budget :: getSavingsDueDate() const {
    return savingsDueDate;
}












/* --------------------------------------------------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------------------------------------------------- */
//
class DataAndHistory {
private:
    vector<Allowance> AllowanceData;
    vector<Expense> ExpenseData;
    vector<Category> CategoryList;


public:
    void loadData_Expenses();
    void loadData_Allowance();
    void load_CategoryList();

    void filterExpenseData_ByDate(const string);
    void filterExpenseData_ByCategory(const string);
};




/* --------------------------------------------------------------------------------------- */
/* -------------------------- DataAndHistory CLASS : PUBLIC MFs -------------------------- */
/* --------------------------------------------------------------------------------------- */

//
void DataAndHistory :: loadData_Expenses()
{ 
    ifstream inFILE(ExpensesFILE, ios::binary);

    if (inFILE.is_open())
    {
        // READ Expenses
        while (inFILE.peek() != EOF) {
            Expense expenseHol("", 0, "", "", "");
            inFILE >> expenseHol;
            if (inFILE) { ExpenseData.push_back(expenseHol); }
        }
        inFILE.close();
    }
    else {
        // Handle file open error
        cerr << ">> ERROR: Unable to open Expenses file" << endl;
    }
}


//
void DataAndHistory :: loadData_Allowance()
{
    // READ Allowances
    ifstream inFILE(AllowancesFILE, ios::binary);

    if (inFILE.is_open())
    {
        while (inFILE.peek() != EOF) {
            Allowance allowanceHol("", 0, "", "");
            inFILE >> allowanceHol;
            if (inFILE) { AllowanceData.push_back(allowanceHol); }
        }
        inFILE.close();
    }
    else {
        // Handle file open error
        cerr << ">> ERROR: Unable to open Allowance file" << endl;
    }
}


//
void DataAndHistory :: load_CategoryList()
{
    ifstream inFILE(CategoryListFILE, ios::binary);

    if (inFILE.is_open())
    {
        while (inFILE.peek() != EOF) {
            Category categoryHol("", 0);
            inFILE >> categoryHol;
            if (inFILE) { CategoryList.push_back(categoryHol); }
        }
        inFILE.close();
    }
    else {
        // Handle file open error
        cerr << ">> ERROR: Unable to open Category List file" << endl;
    }
}




//
void DataAndHistory :: filterExpenseData_ByDate(const string Date)
{
    char line = 179;
    string expenseDate_Hol;
    double totalAmount = 0;

    // Display TITLE of each column
    cout << line;
    displayTxtByColumn_CENTERED("AMOUNT", BOLDGREEN, COLUMN_AMT);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDGREEN, COLUMN_AMT);
    displayTxtByColumn_CENTERED_NB("DESCRIPTION", BOLDGREEN, COLUMN_AMT);
    cout << "\n";

    // Display expenses with said date
    for (const auto& expense : ExpenseData)
    {
        if (Date == expense.getDate()) {
            totalAmount += expense.getAmount();
            cout << line;
            displayTxtByColumn(to_string(expense.getAmount()), WHITE, COLUMN_AMT);
            displayTxtByColumn(expense.getCategory(), WHITE, COLUMN_AMT);
            displayTxtByColumn_NB(expense.getDescription(), WHITE, 3);
            cout << "\n";
        }
    }

    // If there is no such date, print empty slots as dummy
    if (totalAmount == 0) {
        for (int i = 0; i < 5; i++) {
            displayTxtByColumn("0.00", WHITE, COLUMN_AMT);
            displayTxtByColumn("-----", WHITE, COLUMN_AMT);
            displayTxtByColumn_NB("-----", WHITE, 3);
        }
    }
    border('-', 65);

    // Display total amount
    cout << line;
    displayTxtByColumn_CENTERED("Total:", BOLDWHITE, COLUMN_AMT);
    displayTxtByColumn_CENTERED_NB(to_string(totalAmount), BOLDWHITE, COLUMN_AMT);
}


// 
void DataAndHistory :: filterExpenseData_ByCategory(const string Category)
{
    char line = 179;
    string expenseCat_Hol;
    double totalAmount = 0;

    // Display TITLE of each column
    cout << line;
    displayTxtByColumn_CENTERED("AMOUNT", BOLDGREEN, COLUMN_AMT);
    displayTxtByColumn_CENTERED("DATE", BOLDGREEN, COLUMN_AMT);
    displayTxtByColumn_CENTERED_NB("DESCRIPTION", BOLDGREEN, COLUMN_AMT);
    cout << "\n";

    // Display expenses with said category
    for (const auto& expense : ExpenseData)
    {
        if (Category == expense.getCategory()) {
            cout << line;
            displayTxtByColumn(to_string(expense.getAmount()), WHITE, COLUMN_AMT);
            displayTxtByColumn(expense.getDate(), WHITE, COLUMN_AMT);
            displayTxtByColumn_NB(expense.getDescription(), WHITE, 3);
            cout << "\n";
        }
    }

    // If there is no such category, print empty slots as dummy
    if (totalAmount == 0) {
        for (int i = 0; i < 5; i++) {
            displayTxtByColumn("0.00", WHITE, COLUMN_AMT);
            displayTxtByColumn("--/--/----", BOLDWHITE, COLUMN_AMT);
            displayTxtByColumn_NB("-----", WHITE, 3);
        }
    }

    border('-', 65);

    // Display total amount
    cout << line;
    displayTxtByColumn_CENTERED("Total:", BOLDWHITE, COLUMN_AMT);
    displayTxtByColumn_CENTERED_NB(to_string(totalAmount), BOLDWHITE, COLUMN_AMT);
}























/* --------------------------------------------------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------------------------------------------------- */

int main(void)
{
    DataAndHistory dataHol;
    // Expense expense1("Date", 12.5, "Category", "Account", "Description");
    // Expense expense2("06/20/2024", 34.0, "Food", "Cash", "Icecream and bread");
    // Expense expense3("06/20/2024", 12.5, "Food", "Cash", "Candies");
    // Expense expense4("06/20/2024", 5.0, "Food", "Cash", "Gummy bears");

    // ofstream outFILE(ExpensesFILE, ios::binary | ios::trunc);

    // if (outFILE.is_open()) {
    //     outFILE << expense1;
    //     outFILE << expense2;
    //     outFILE << expense3;
    //     outFILE << expense4;
    //     outFILE.close();

    //     cout << "File handling SUCCESSFUL";

    // }
    // else {
    //     cerr << "ERROR";
    // }

    return 0;
}