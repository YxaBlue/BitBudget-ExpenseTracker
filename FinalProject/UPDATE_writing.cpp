#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <list> 
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <cstring>
#include <sstream>

using namespace std;

#define COLUMN_AMT 10
#define COLUMNWIDTH 15
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
const string ExpenseLimitFILE = "ExpenseLimits.bin";
const string SavingsFILE = "Savings.bin";

const string CategoryListFILE = "CATEGORYList.bin";
const string InboxFILE = "Inbox.bin";







/*_________________________________________________________________________________*/
/*-------------------------------- FUNCTIONS LIST: --------------------------------*/
/*_________________________________________________________________________________*/

/* DISPLAY FUNCTIONS */
//
void border(char, int);
void displayCenteredLine_NoColor(const string, int);
void displayCenteredLine_Colored(const string, const string, int);
void displayCenteredLine_NoNewLine(const string, const string, int);

void displayTxtByColumn(const string &, const string &, int, int);
void displayTxtByColumn_CENTERED(const string &, const string &, int, int);
void displayTxtByColumn_NB(const string &, const string &, int, int);
void displayTxtByColumn_CENTERED_NB(const string &, const string &, int, int);

void clearScreen();


/* EXTRA FUNCTIONS */
//
void clearFile(string);
bool isNumeric(string);
bool isDouble(string);

bool validateDateFormat(const string &);
bool validateDate(const string &);
bool validateSecondDate(const string &, const string &);

string getDate_Today();









/*_________________________________________________________________________________*/
/*------------------------ DISPLAY TEXT-STYLE UI FUNCTIONS ------------------------*/
/*_________________________________________________________________________________*/
// Function to print a border with a given character
void border(char c, int length = SCREENWIDTH) {
    for (int i = 0; i < length; i++) cout << c;
    cout << endl;
}

// Function to display centered lines
void displayCenteredLine_NoColor(const string &line, int width = SCREENWIDTH) {
    int padLen = (width - line.size()) / 2;
    cout << string(padLen, ' ') << line << endl;
}

// Function to display centered lines with a color
void displayCenteredLine_Colored(const string &line, const string &color, int width = SCREENWIDTH) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << RESET << endl;
}

// Function to display centered lines with color (no '\n')
void displayCenteredLine_NoNewLine(const string &line, const string &color, int width = SCREENWIDTH) {
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << RESET;
}



// Function to display lines by column
void displayTxtByColumn(const string &str, const string &color, int width = COLUMNWIDTH)
{
    int totalSpaces = width - str.size() - 1;
    char border = 179;
    cout << color << " " << str << string(totalSpaces, ' ') << RESET << border;
}

// Function to display centered lines, 
void displayTxtByColumn_CENTERED(const string &txt, const string &color, int width = COLUMNWIDTH)
{
    int totalSpaces = width - txt.size() - 1;
    int spaceLen_Left = totalSpaces / 2;
    int spaceLen_Right = totalSpaces - spaceLen_Left;

    char border = 179;

    if (spaceLen_Left < 0) spaceLen_Left = 0;
    if (spaceLen_Right < 0) spaceLen_Right = 0;

    cout << color << " " << string(spaceLen_Left, ' ') << txt << string(spaceLen_Right, ' ') << RESET << border;
}

// Function to display lines by column
void displayTxtByColumn_NB(const string &str, const string &color, int width = COLUMNWIDTH)
{
    int totalSpaces = width - str.size() - 1;
    cout << color << " " << str << string(totalSpaces, ' ') << RESET;
}

// Function to display centered lines, 
void displayTxtByColumn_CENTERED_NB(const string &txt, const string &color, int width = COLUMNWIDTH)
{
    int totalSpaces = width - txt.size() - 1;
    int spaceLen_Left = totalSpaces / 2;
    int spaceLen_Right = totalSpaces - spaceLen_Left;

    if (spaceLen_Left < 0) spaceLen_Left = 0;
    if (spaceLen_Right < 0) spaceLen_Right = 0;

    cout << color << " " << string(spaceLen_Left, ' ') << txt << string(spaceLen_Right, ' ') << RESET;
}



// Function to clear the console screen
void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}






/*_________________________________________________________________________________*/
/*-------------------------------- EXTRA FUNCTIONS --------------------------------*/
/*_________________________________________________________________________________*/
// CLEAR all contents of file
void clearFile(string FILENAME) {
    ofstream outFILE(InboxFILE, ios::binary | ios::trunc);

    if (!outFILE.is_open()) {
        // ERROR [3]: Notify User that file failed to be opened
        displayCenteredLine_Colored("ERROR [4]", BOLDRED);
        cout << "\n";

        displayCenteredLine_Colored("Unable to open file.", YELLOW);
        displayCenteredLine_NoNewLine("Press 'ENTER' to continue...   ", YELLOW);
        getchar();
    }

    outFILE.close();
}

// check if string is NUMERIC
bool isNumeric(string str) {
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            return false;
        }
    }

    return true;
}

bool isDouble(string str) {
    int dot;
    for (int i = 0; i < str.size(); i++) {
        
    }
}


//
bool validateDateFormat(const string &date) {
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

//
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

//
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

//
string getDate_Today()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string(5 + ltm->tm_mday);

    if (month.size() == 1) month = "0" + month;

    string dateToday = month + "/" + day + "/" + year;

    return dateToday; 
}










/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/











/*---------------------------------------------------------------------------------*/
/*------------------------------- CATEGORY CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

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

    void display_BabyCategories();

    // Friend Functions
    friend ostream& operator<<(ostream& outFILE, const Category &categoryHol);
    friend istream& operator>>(istream& inFILE, Category &categoryHol);
    
};


/* CATEGORY CLASS: Constructors */
/*------------------------------*/

Category :: Category(string catName_parent, int babyCat_Total) :
                categoryName_parent(catName_parent),
                totalBabyCategories(babyCat_Total) {}



/* CATEGORY CLASS: Constructors */
/*------------------------------*/

string Category :: getParentCategory() const            { return categoryName_parent; }
int Category :: getTotalBabyCategories() const          { return totalBabyCategories; }
vector<string> Category :: getBabyCategories() const    { return categoryName_baby; }



/* CATEGORY CLASS: Public MFs */
/*----------------------------*/

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
    categoryHol.categoryName_baby.clear();
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










/*---------------------------------------------------------------------------------*/
/*-------------------------- SavingsAndExpenseLim CLASS ---------------------------*/
/*---------------------------------------------------------------------------------*/

class SavingsAndExpenseLim {
private:
    string startDate;
    string dueDate;
    double goal;

public:
    SavingsAndExpenseLim(string, string, double);

    string get_startDate() const;
    string get_dueDate() const;
    double get_goal() const;

    void set_startDate(string);
    void set_dueDate(string);
    double set_goal(double);

    friend ostream& operator<<(ostream& os, const SavingsAndExpenseLim& savingsHol);
    friend istream& operator>>(istream& is, SavingsAndExpenseLim& savingsHol);
};



/* SavingsAndExpenseLim Class (Public): CONSTRUCTORS */
/*--------------------------------------*/

SavingsAndExpenseLim :: SavingsAndExpenseLim(string start, string due, double amt) :
    startDate(start), 
    dueDate(due),
    goal(amt) {}



/* SavingsAndExpenseLim Class (Public): GETTERS */
/*----------------------------------------------*/

string SavingsAndExpenseLim :: get_startDate() const   { return startDate; }
string SavingsAndExpenseLim :: get_dueDate() const     { return dueDate; }
double SavingsAndExpenseLim :: get_goal() const        { return goal; }




/* SavingsAndExpenseLim Class (Public): GETTERS */
/*----------------------------------------------*/

void SavingsAndExpenseLim :: set_startDate(string newDate)   { startDate = newDate; }
void SavingsAndExpenseLim :: set_dueDate(string newDate)     { dueDate = newDate; }
double SavingsAndExpenseLim :: set_goal(double newGoal)      { goal = newGoal; }





// WRITE SavingsAndExpenseLimit data
ostream& operator<<(ostream& os, const SavingsAndExpenseLim& savingsHol)
{
    string::size_type size;

    // WRITE startDate
    size = savingsHol.startDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(savingsHol.startDate.c_str(), size);

    // WRITE dueDate
    size = savingsHol.dueDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(savingsHol.dueDate.c_str(), size);

    // WRITE goal
    os.write(reinterpret_cast<const char*>(&savingsHol.goal), sizeof(savingsHol.goal));

    return os;
}

// READ SavingsAndExpenseLim data
istream& operator>>(istream& is, SavingsAndExpenseLim& savingsHol)
{
    size_t size;

    //READ startDate
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'startDate size' from file");
    savingsHol.startDate.resize(size);
    is.read(&savingsHol.startDate[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'startDate' from file");

    //READ dueDate
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'dueDate size' from file");
    savingsHol.dueDate.resize(size);
    is.read(&savingsHol.dueDate[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'dueDate' from file");

    // READ goal
    is.read(reinterpret_cast<char*>(&savingsHol.goal), sizeof(savingsHol.goal));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'goal' from file");

    return is;
}











/*---------------------------------------------------------------------------------*/
/*------------------------------- EXPENSE CLASS -----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Expense {
private:
    string date;
    double amount;
    string account;

    string category;
    string babyCategory;
    string description;


public:
    Expense(string, double, string, string, string, string);

    string getDate() const;
    string getCategory() const;
    string getBabyCategory() const;
    
    string getDescription() const;
    string getAccount() const;
    double getAmount() const;


    void setDate(string);
    void setCategory(string);
    void setBabyCategory(string);

    void setDescription(string);
    void setAccount(string);
    void setAmount(double);


    friend ostream& operator<<(ostream& os, const Expense& expense);
    friend istream& operator>>(istream& is, Expense& expense);
};


/* EXPENSE Class (Public): CONSTRUCTOR */
/*-------------------------------------*/

Expense :: Expense(string Date, double Amt,
                   string Cat, string bbyCat,
                   string Acc, string Desc) :
            date(Date),
            amount(Amt),
            category(Cat),
            babyCategory(bbyCat),
            account(Acc),
            description(Desc) {}



/* EXPENSE Class MFs(Public): GETTERS */
/*------------------------------------*/

string Expense :: getDate() const           { return date; }
double Expense :: getAmount() const         { return amount; }
string Expense :: getCategory() const       { return category; }
string Expense :: getBabyCategory() const   { return babyCategory; }
string Expense :: getAccount() const        { return account; }
string Expense :: getDescription() const    { return description; }



/* EXPENSE Class MFs(Public): SETTERS */
/*------------------------------------*/

void Expense :: setDate(string newDate)             { date = newDate; }
void Expense :: setAmount(double newAmt)            { amount = newAmt; }
void Expense :: setCategory(string newCat)          { category = newCat; }
void Expense :: setBabyCategory(string newBbyCat)   { babyCategory = newBbyCat; }
void Expense :: setAccount(string newAcc)           { account = newAcc; }
void Expense :: setDescription(string newDesc)      { description = newDesc; }




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

    // WRITE baby category
    size = expense.babyCategory.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.babyCategory.c_str(), size);

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
    if (!is.good()) throw runtime_error("Error reading 'amount' from file");

    // READ category
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'category size' from file");
    expense.category.resize(size);
    is.read(&expense.category[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'category' from file");

    // READ baby category
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'baby category size' from file");
    expense.babyCategory.resize(size);
    is.read(&expense.babyCategory[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'baby category' from file");

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

    void setDate(string);
    void setAmount(double);
    void setAccount(string);
    void setDescription(string);

    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, Allowance& allowance);
};



/* ALLOWANCE class MF(Public): GETTERS */
/*-------------------------------------*/

string Allowance :: getDate() const         { return date; }
double Allowance :: getAmount() const       { return amount; }
string Allowance :: getAccount() const      { return account; }
string Allowance :: getDescription() const  { return description; }




/* ALLOWANCE class MF(Public): SETTERS */
/*-------------------------------------*/

void Allowance :: setDate(string newDate)         { date = newDate; }
void Allowance :: setAmount(double newAmt)        { amount = newAmt; }
void Allowance :: setAccount(string newAcc)       { account = newAcc; }
void Allowance :: setDescription(string newDesc)  { description = newDesc; }




/* ALLOWANCE class Friends (Public): File Handling Operators */
/*-----------------------------------------------------------*/

// WRITE Allowance data
ostream& operator<<(ostream& os, const Allowance& allowance) {
    size_t dateSize = allowance.date.size();
    size_t accountSize = allowance.account.size();
    size_t descSize = allowance.description.size();

    // WRITE date
    os.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
    os.write(allowance.date.c_str(), dateSize);

    // WRITE account
    os.write(reinterpret_cast<const char*>(&accountSize), sizeof(accountSize));
    os.write(allowance.account.c_str(), accountSize);
    
    // WRITE description
    os.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    os.write(allowance.description.c_str(), descSize);

    // WRITE amount
    os.write(reinterpret_cast<const char*>(&allowance.amount), sizeof(allowance.amount));

    return os;
}

// READ Allowance data
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
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description size' from file");
    allowance.description.resize(descSize);
    is.read(&allowance.description[0], descSize);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description' from file");

    is.read(reinterpret_cast<char*>(&allowance.amount), sizeof(allowance.amount));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'amount' from file");
    return is;
}












/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/










/*---------------------------------------------------------------------------------*/
/*--------------------------------- BUDGET CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Budget {
private:
    double totalBudget;
    double currentSavings;

    vector<Expense> expensesList;
    vector<Allowance> allowancesList;
    vector<Expense> expensesList_Today;
    vector<Allowance> allowancesList_Today;

    vector<SavingsAndExpenseLim> savingsList;
    vector<SavingsAndExpenseLim> expenseLimitsList;

    void calculateCurrentSavings();

public:
    Budget();

    void loadExpenses();
    void saveExpenses() const;
    void loadAllowances();
    void saveAllowances() const;
    void loadSavings();
    void saveSavings() const;
    void loadExpenseLimits();
    void saveExpenseLimits() const;


    void displayAllowancesList(int);
    void displayExpensesList(int);
    void displaySavingsList(int);
    void displayExpenseLimitList(int);

    void setTotalBudget(double);

    void addExpense(const Expense&);
    void addAllowance(const Allowance&);
    
    void removeAllowance(int index);
    void removeExpense(int index);

    void updateExpenseDateRange(const string&, const string&, int);
    void updateSavingsDateRange(const string&, const string&, int);

    void displayUpdateMenu();
    void displaySetLimitExpensesMenu();
    void displaySetSavingsMenu();
    void displayUpdateAllowanceMenu(int);

    void saveState();

};



/* BUDGET class(Private): Calculator */
/*-----------------------------------*/
void Budget :: calculateCurrentSavings() {
    currentSavings = 0.0;
    for (const auto& allowance : allowancesList) {
        currentSavings += allowance.getAmount();
    }

    for (const auto& allowance : allowancesList_Today) {
        currentSavings += allowance.getAmount();
    }
}




/* BUDGET class(Public): CONSTRUCTOR */
/*-----------------------------------*/

Budget :: Budget() :
            totalBudget(0.0),
            currentSavings(0.0) {}



/* BUDGET class MFs(Public): LOAD & SAVE Functions - Allowance and Expenses */
/*--------------------------------------------------------------------------*/
// LOAD: Expenses list from file
void Budget :: loadExpenses() {
    ifstream inFile(ExpensesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Expense expense("", 0, "", "", "", "");
            inFile >> expense;

            if (expense.getDate() == getDate_Today()) {
                expensesList_Today.push_back(expense);
            }
            else expensesList.push_back(expense);
        }

        inFile.close();
    }
}

// SAVE: Expenses list to file
void Budget :: saveExpenses() const {
    ofstream outFile(ExpensesFILE, ios::binary | ios::trunc);
    if (outFile.is_open()) {
        for (const auto& expense : expensesList) {
            outFile << expense;
        }
        for (const auto& expense : expensesList_Today) {
            outFile << expense;
        }

        outFile.close();
    }
}

// LOAD: Allowances list from file
void Budget :: loadAllowances()
{
    ifstream inFile(AllowancesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Allowance allowance("", 0.0, "", "");
            inFile >> allowance;

            if (allowance.getDate() == getDate_Today()) {
                allowancesList_Today.push_back(allowance);
            }
            else allowancesList.push_back(allowance);
        }
        inFile.close();
        calculateCurrentSavings();
    }
}

// SAVE: Allowances list to file
void Budget :: saveAllowances() const
{
    ofstream outFile(AllowancesFILE, ios::binary | ios::trunc);
    if (outFile.is_open()) {
        for (const auto& allowance : allowancesList) {
            outFile << allowance;
        }
        for (const auto& allowance : allowancesList_Today) {
            outFile << allowance;
        }

        outFile.close();
    }
}

// LOAD: Savings list from file
void Budget :: loadSavings()
{
    ifstream inFILE(SavingsFILE, ios::binary);
    if(inFILE.is_open()) {
        while(inFILE.peek() != EOF)
        {
            SavingsAndExpenseLim savingsData("", "", 0);
            inFILE >> savingsData;
            savingsList.push_back(savingsData);
        }
        inFILE.close();
    }
}

// SAVE: Savings list to file
void Budget :: saveSavings() const
{
    ofstream outFILE(SavingsFILE, ios::binary | ios::trunc);
    if (outFILE.is_open()) {
        for (const auto& savingsData : savingsList)
        {
            outFILE << savingsData;
        }
        outFILE.close();
    }
}

// LOAD: Expense Limits list from file
void Budget :: loadExpenseLimits()
{
    ifstream inFILE(ExpenseLimitFILE, ios::binary);
    if(inFILE.is_open()) {
        while(inFILE.peek() != EOF)
        {
            SavingsAndExpenseLim expenseLimitsData("", "", 0);
            inFILE >> expenseLimitsData;
            expenseLimitsList.push_back(expenseLimitsData);
        }

        inFILE.close();
    }
}

// SAVE: Expense Limit list to file
void Budget :: saveExpenseLimits() const
{
    ofstream outFILE(ExpenseLimitFILE, ios::binary | ios::trunc);
    if (outFILE.is_open()) {
        for (const auto& expenseLimitData : expenseLimitsList)
        {
            outFILE << expenseLimitData;
        }
        outFILE.close();
    }
}




/* BUDGET class MFs(Public): Display Lists */
/*-----------------------------------------*/

void Budget :: displayAllowancesList(int page = 1)
{
    int i = 1;
    // Display: Headers
    displayTxtByColumn_CENTERED("INDEX", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("ACCOUNT", WHITE, COLUMNWIDTH);
    displayTxtByColumn_NB("DESCRIPTION", WHITE);

    for(const auto& allowance : allowancesList_Today)
    {
        double amt_db = allowance.getAmount();
        stringstream stream;
        stream << fixed << setprecision(2) << amt_db;
        string amt_str = stream.str();

        cout << "\n";
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(allowance.getDate(), WHITE, COLUMNWIDTH);
        displayTxtByColumn(amt_str, WHITE, COLUMN_AMT);
        displayTxtByColumn(allowance.getAccount(), WHITE, COLUMNWIDTH);
        displayTxtByColumn_NB(allowance.getDescription(), WHITE, 60);
    }

}

void Budget :: displayExpensesList(int page = 1)
{
    int i = 1;
    // Display: Headers
    displayTxtByColumn_CENTERED("INDEX", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("CATEGORY", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("ACCOUNT", WHITE, COLUMNWIDTH);
    displayTxtByColumn_NB("DESCRIPTION", WHITE);

    for(const auto& expenses : expensesList_Today)
    {
        double amt_db = expenses.getAmount();
        stringstream stream;
        stream << fixed << setprecision(2) << amt_db;
        string amt_str = stream.str();

        cout << "\n";
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(expenses.getDate(), WHITE, COLUMNWIDTH);
        displayTxtByColumn(amt_str, WHITE, COLUMNWIDTH);
        displayTxtByColumn(expenses.getCategory(), WHITE, COLUMNWIDTH);
        displayTxtByColumn(expenses.getAccount(), WHITE, COLUMNWIDTH);
        displayTxtByColumn_NB(expenses.getDescription(), WHITE, 60);
    }
}

void Budget :: displaySavingsList(int page = 1)
{
    int i = 1;
    string presentDate = getDate_Today();

    // Display: Headers
    cout << string(5, ' ');
    displayTxtByColumn_CENTERED("INDEX", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE(Start - Due)", WHITE, 25);
    displayTxtByColumn_CENTERED("SAVINGS GOAL", WHITE, COLUMNWIDTH);

    for(const auto& savingsData : savingsList)
    {
        double amt_db = savingsData.get_goal();
        stringstream stream;
        stream << fixed << setprecision(2) << amt_db;
        string amt_str = stream.str();

        cout << "\n";
        cout << string(5, ' ');
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(savingsData.get_startDate() + " - " + savingsData.get_dueDate(), WHITE, 25);
        displayTxtByColumn_NB( "P" + amt_str, WHITE, COLUMNWIDTH);
    }
}

void Budget :: displayExpenseLimitList(int page = 1)
{
    int i = 1;
    string presentDate = getDate_Today();

    // Display: Headers
    cout << string(5, ' ');
    displayTxtByColumn_CENTERED("INDEX", WHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE(Start - Due)", WHITE, 25);
    displayTxtByColumn_CENTERED("SAVINGS GOAL", WHITE, COLUMNWIDTH);

    for(const auto& expenseLimData : expenseLimitsList)
    {
        double amt_db = expenseLimData.get_goal();
        stringstream stream;
        stream << fixed << setprecision(2) << amt_db;
        string amt_str = stream.str();

        cout << "\n";
        cout << string(5, ' ');
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(expenseLimData.get_startDate() + " - " + expenseLimData.get_dueDate(), WHITE, 25);
        displayTxtByColumn_NB(amt_str, WHITE, COLUMNWIDTH);
    }
}





/* BUDGET class MFs(Public): SETTERS, ADDERS, REMOVERS */
/*-----------------------------------*/

void Budget :: setTotalBudget(double budget)            { totalBudget = budget; }

void Budget :: addExpense(const Expense& expense)       { expensesList_Today.push_back(expense); }
void Budget :: addAllowance(const Allowance& allowance) {
    allowancesList_Today.push_back(allowance);
    calculateCurrentSavings();
}

void Budget :: removeAllowance(int index) {
    if (index >= 0 && index < allowancesList_Today.size()) {
        allowancesList_Today.erase(allowancesList_Today.begin() + index);
        calculateCurrentSavings();
    } else {
        throw runtime_error(">> WARNING: Invalid allowance index.");
    }
}




void Budget :: updateExpenseDateRange(const string& startDate, const string& dueDate, int index)
{
    // Check if index is out of bounds
    if ((index < 0) || (index >= savingsList.size())) {
        throw runtime_error(">> ERROR: Index not found in expensesList.");
    }

    // Update expense date range
    if (validateDateFormat(startDate) && validateDateFormat(dueDate) &&
        validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
            SavingsAndExpenseLim& expenseLimitData = expenseLimitsList[index-1];
            expenseLimitData.set_startDate(startDate);
            expenseLimitData.set_dueDate(dueDate);
    }
    else {
        throw runtime_error("Invalid date format or date range for expensesList.");
    }
}

void Budget :: updateSavingsDateRange(const string& startDate, const string& dueDate, int index)
{
    // Check if index is out of bounds
    if ((index <= 0) || (index > savingsList.size())) {
        throw runtime_error("Index not found in expensesList.");
    }

    // Updata savings date range
    if (validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
            SavingsAndExpenseLim& savingsData = savingsList[index-1];
            savingsData.set_startDate(startDate);
            savingsData.set_dueDate(dueDate);
    }
    else {
        throw runtime_error("Invalid date range for savings.");
    }
}




void Budget :: displayUpdateMenu() {
    clearScreen();

    // Display: UPDATE title
    border(205);
    displayCenteredLine_Colored("UPDATE", BLUE);
    border(205);

    // Display: Options
    displayCenteredLine_Colored("Options", BOLDWHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ 1 ] Set LIMIT OF EXPENSES", WHITE);
    displayCenteredLine_Colored("[ 2 ] Set SAVINGS          ", WHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ 3 ] Update ALLOWANCE     ", WHITE);
    displayCenteredLine_Colored("[ 4 ] Update TRANSACTION   ", WHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ R ] Return               ", WHITE);
    cout << "\n";

    border(196);
    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
}


void Budget :: displaySetLimitExpensesMenu() {
    string input;
    while (true) {
        clearScreen();

        // Display: UPDATE(Limit Of Expenses) title
        border(205);
        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES", BLUE);
        border(205);

        // Display: Expenses Total and New Limit
        cout << BOLDWHITE << "  >> TOTAL EXPENSES:    P " << "<display total>" << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Expense Limit: EXPENSES:" << RESET << endl;
        displayExpenseLimitList();
        cout << "\n";
        cout << "\n";
        border(196);

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[1] SET NEW GOAL");
        displayCenteredLine_NoColor("[2] EDIT        ");
        cout << "\n";
        displayCenteredLine_NoColor("[R] Return      ");
        cout << "\n";
        displayCenteredLine_Colored(">> Enter number: ", CYAN);

        getline(cin, input);

        if (input == "1")   // Perform: SET NEW GOAL
        {
            bool input1 = true, input2 = false, input3 = false;

            double newGoal = 0;
            string date1 = "MM/DD/YYYY";
            string date2 = "MM/DD/YYYY";

            while (true)
            {
                clearScreen();
                // Display: UPDATE(Limit Of Expenses) title
                border(205);
                displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES", BLUE);
                border(205);

                // Display: Expenses Total and New Limit
                cout << BOLDWHITE << "  >> TOTAL EXPENSES:    P " << "<display total>" << "\n" << RESET << endl;

                cout << BOLDWHITE << "  >> NEW Expense Limit:" << RESET << endl;
                cout << string(5, ' ');
                cout << "* NEW GOAL:       " << GREEN << "P " << newGoal << RESET << endl;
                cout << "* START DATE:     " << BLUE <<  date1 << RESET << endl;
                cout << "* DUE DATE:       " << BLUE <<  date2 << RESET << endl;
                cout << "\n";
                border(196);

                // Display: Notice if expense limit reached max slot
                if (expenseLimitsList.size() == 5) {
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> You can set Expense Limit Goals up to 5 times!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
                    getchar();
                    break;
                }

                // Display: Prompts for user to input NEW GOAL
                if (input1) {
                    if (true) {
                        input1 = false;
                        input2 = true;
                    }
                }

                else if (input2) {
                    if (validateDate(date1)) {
                        input2 = false;
                        input3 = true;
                    }
                }

                else if (input3) {
                    input3 = false;
                    if (validateSecondDate(date1, date2)) {
                        input2 = false;
                        input3 = true;
                    }
                }

            }
        }

        else if (input == "2") {
            string startDate, dueDate;
            int index;

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
                updateExpenseDateRange(startDate, dueDate, index);
                cout << "Expense date range set from " << startDate << " to " << dueDate << endl;
            } catch (const exception& e) {
                cout << ">> ERROR: " << e.what() << endl;
            }
        }
        else if (input == "R" || input == "r") {
            saveState();
            return; 
        }
    }
}

// UPDATE: SET SAVINGS
void Budget :: displaySetSavingsMenu() {
    string input;

    while (true) {
        clearScreen();
        // Display: UPDATE(SAVINGS) Title
        border(205);
        displayCenteredLine_Colored("UPDATE: SAVINGS", BLUE);
        border(205);

        // Display: List of SAVINGS schedules
        displayCenteredLine_Colored(">> SAVINGS", BOLDWHITE);
        displayCenteredLine_NoColor("*Date: [" + getSavingsStartDate() + " - " + getSavingsDueDate() + "]");
        displayCenteredLine_NoColor("*Savings GOAL:                       P" + to_string(savingsGoal));
        displayCenteredLine_NoColor("*Current Savings:                    P" + to_string(currentSavings) + "/" + to_string(savingsGoal));
        cout << "\n";
        cout << "\n";
        cout << "\n";
        border(205);

        // Display: OPTIONS
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_Colored("[1] SET/REWRITE GOAL", BOLDWHITE);
        displayCenteredLine_Colored("[2] SET/REWRITE DATE", BOLDWHITE);
        displayCenteredLine_Colored("[R] Return", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        getline(cin, input);

        if (input == "1") {
            double newGoal;
            cout << "Enter new savings goal: ";
            cin >> newGoal;
            setSavingsGoal(newGoal);
            cout << "New savings goal set to P" << newGoal << endl;
        }
        else if (input == "2") {
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
        }
        else if (input == "R" || input == "r") {
            saveState();
            return; 
        }
    }
}

// UPDATE: ALLOWANCE
void Budget :: displayUpdateAllowanceMenu(int page = 0) {
    const int itemsPerPage = 10;
    string input;

    while (true) {
        clearScreen();
        int start = page * itemsPerPage;
        int end = min(start + itemsPerPage, static_cast<int>(allowancesList.size()));

        // Display: UPDATE ALLOWANCE title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE", BLUE);
        border(205);

        cout << BOLDWHITE << "  >> DATA:" << RESET << endl;
        cout << "\n";

        // Display headers
        cout << setw(15) << left << "Date" << setw(15) << "Amount" << setw(20) << "Account" << setw(50) << "Description" << endl;
        cout << string(150, '-') << endl;

        for (int i = start; i < end; ++i) {
            const Allowance &allowance = allowancesList[i];
            cout << setw(15) << left << allowance.getDate() << setw(15) << allowance.getAmount()
                    << setw(20) << allowance.getAccount() << setw(50) << allowance.getDescription() << endl;
        }

        border(196);
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        displayCenteredLine_NoColor("[1] Previous Page      [3] Edit (ADD ALLOWANCE)   ");
        displayCenteredLine_NoColor("[2] Next Page          [4] Edit (DELETE ALLOWANCE)");
        displayCenteredLine_NoColor("                   [R] Return                     ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        getline(cin, input);

        if (input == "1") {
            if (page > 0) page--;
        }
        else if (input == "2") {
            if ((page + 1) * itemsPerPage < allowancesList.size()) page++;
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
        }
        else if (input == "4") {
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
        }
        else if (input == "R" || input == "r") {
            saveState();
            return;
        }
        else {
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
        getline(cin, input);

        if (input == "R" || input == "r") break;

        try {
            if (input == "1") {
                budget.displaySetLimitExpensesMenu();
            } else if (input == "2") {
                budget.displaySetSavingsMenu();
            } else if (input == "3") {
                budget.displayUpdateAllowanceMenu();
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
