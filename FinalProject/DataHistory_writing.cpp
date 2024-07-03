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

void displayTxtByColumn(const string &, const string &, int);
void displayTxtByColumn_CENTERED(const string &, const string &, int);
void displayTxtByColumn_NB(const string &, const string &, int);
void displayTxtByColumn_CENTERED_NB(const string &, const string &, int);

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
    int dot = 0;
    if (str.empty()) return false;

    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            if (str[i] == '.') dot++;
            else return false;
        }
        if (dot > 1) return false;
    }

    if ((dot == 1) && (str.size() == 1)) return false;

    return true;
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
    string getParent() const;
    int getTotalBaby() const;
    vector<string> getBabies() const;

    // Category Public MFs
    void addBaby(string);

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

string Category :: getParent() const            { return categoryName_parent; }
int Category :: getTotalBaby() const          { return totalBabyCategories; }
vector<string> Category :: getBabies() const    { return categoryName_baby; }



/* CATEGORY CLASS: Public MFs */
/*----------------------------*/

void Category :: addBaby(string babyCatName)
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










/*_________________________________________________________________________________*/
/*------------------------------- BUDGET CLASS ------------------------------------*/
/*_________________________________________________________________________________*/














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
    displayTxtByColumn_CENTERED("DATE", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED_NB("DESCRIPTION", BOLDGREEN, COLUMN_WIDTH);
    cout << "\n";

    // Display all data
    for (const auto& expense : ExpenseData)
    {
        if (Date == expense.getDate()) {
            totalAmount += expense.getAmount();
            cout << line;
            displayTxtByColumn(expense.getDate(), BOLDWHITE, COLUMN_WIDTH);
            displayTxtByColumn(to_string(expense.getAmount()), WHITE, COLUMN_WIDTH);
            displayTxtByColumn(expense.getCategory(), WHITE, COLUMN_WIDTH);
            displayTxtByColumn_NB(expense.getDescription(), WHITE, 3);
            cout << "\n";
        }
    }

    if (totalAmount == 0) {
        for (int i = 0; i < 5; i++) {
            displayTxtByColumn("--/--/----", BOLDWHITE, COLUMN_WIDTH);
            displayTxtByColumn("0.00", WHITE, COLUMN_WIDTH);
            displayTxtByColumn("-----", WHITE, COLUMN_WIDTH);
            displayTxtByColumn_NB("-----", WHITE, 3);
        }
    }
    border('-', 65);

    cout << line;
    displayTxtByColumn_CENTERED("Total:", BOLDWHITE, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED_NB(to_string(totalAmount), BOLDWHITE, COLUMN_WIDTH);
}


//
void DataAndHistory :: filterExpenseData_ByCategory(const string Category)
{
    char line = 179;
    string expenseCat_Hol;
    double totalAmount = 0;

    // Display TITLE of each column
    cout << line;
    displayTxtByColumn_CENTERED("CATEGORY", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED("DATE", BOLDGREEN, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED_NB("DESCRIPTION", BOLDGREEN, COLUMN_WIDTH);
    cout << "\n";

    // Display expenses with said categpry
    for (const auto& expense : ExpenseData)
    {
        if (Category == expense.getCategory()) {
            cout << line;
            displayTxtByColumn(expense.getCategory(), BOLDWHITE, COLUMN_WIDTH);
            displayTxtByColumn(to_string(expense.getAmount()), WHITE, COLUMN_WIDTH);
            displayTxtByColumn(expense.getDate(), WHITE, COLUMN_WIDTH);
            displayTxtByColumn_NB(expense.getDescription(), WHITE, 3);
            cout << "\n";
        }
    }

    // If there is no such category, print empty slots as dummy
    if (totalAmount == 0) {
        for (int i = 0; i < 5; i++) {
            displayTxtByColumn("--/--/----", BOLDWHITE, COLUMN_WIDTH);
            displayTxtByColumn("0.00", WHITE, COLUMN_WIDTH);
            displayTxtByColumn("-----", WHITE, COLUMN_WIDTH);
            displayTxtByColumn_NB("-----", WHITE, 3);
        }
    }

    border('-', 65);

    cout << line;
    displayTxtByColumn_CENTERED("Total:", BOLDWHITE, COLUMN_WIDTH);
    displayTxtByColumn_CENTERED_NB(to_string(totalAmount), BOLDWHITE, COLUMN_WIDTH);
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

    dataHol.loadData_Expenses();
    string testDate = "06/20/2024";
    dataHol.filterExpenseData_ByDate(testDate);

    return 0;
}