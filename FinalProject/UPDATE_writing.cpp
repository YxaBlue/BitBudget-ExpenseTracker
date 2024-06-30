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
    string getParentCategory() const;
    int getTotalBabyCategories() const;
    vector<string> getBabyCategories() const;

    // Category Public MFs
    void add_BabyCategory(string);

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
    double currentAmt;
    string description;

public:
    SavingsAndExpenseLim(string, string, double, double, string);

    string get_startDate() const;
    string get_dueDate() const;
    double get_goal() const;
    double get_currentAmt() const;
    string get_desc() const;

    void set_startDate(string);
    void set_dueDate(string);
    void set_goal(double);
    void set_currentAmt(double);
    void set_desc(string);

    friend ostream& operator<<(ostream& os, const SavingsAndExpenseLim& savingsHol);
    friend istream& operator>>(istream& is, SavingsAndExpenseLim& savingsHol);
};



/* SavingsAndExpenseLim Class (Public): CONSTRUCTORS */
/*--------------------------------------*/

SavingsAndExpenseLim :: SavingsAndExpenseLim(string start, string due, double amt, double currAmt, string desc) :
    startDate(start), 
    dueDate(due),
    goal(amt),
    currentAmt(currAmt),
    description(desc) {}



/* SavingsAndExpenseLim Class (Public): GETTERS */
/*----------------------------------------------*/

string SavingsAndExpenseLim :: get_startDate() const        { return startDate; }
string SavingsAndExpenseLim :: get_dueDate() const          { return dueDate; }
double SavingsAndExpenseLim :: get_goal() const             { return goal; }
double SavingsAndExpenseLim :: get_currentAmt() const   { return currentAmt; }
string SavingsAndExpenseLim :: get_desc() const             { return description; }




/* SavingsAndExpenseLim Class (Public): SETTERS */
/*----------------------------------------------*/

void SavingsAndExpenseLim :: set_startDate(string newDate)              { startDate = newDate; }
void SavingsAndExpenseLim :: set_dueDate(string newDate)                { dueDate = newDate; }
void SavingsAndExpenseLim :: set_goal(double newGoal)                   { goal = newGoal; }
void SavingsAndExpenseLim :: set_currentAmt(double newCurrSavings)  { currentAmt = newCurrSavings; }
void SavingsAndExpenseLim :: set_desc(string newDesc)                   { description = newDesc; }





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

    // WRITE currentAmt
    os.write(reinterpret_cast<const char*>(&savingsHol.currentAmt), sizeof(savingsHol.currentAmt));
    return os;

    // WRITE description
    size = savingsHol.description.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(savingsHol.description.c_str(), size);
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

    // READ currentAmt
    is.read(reinterpret_cast<char*>(&savingsHol.currentAmt), sizeof(savingsHol.currentAmt));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'current amount' from file");

    //READ description
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description size' from file");
    savingsHol.description.resize(size);
    is.read(&savingsHol.description[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'description' from file");

    return is;
}











/*---------------------------------------------------------------------------------*/
/*------------------------------- EXPENSE CLASS -----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Expense {
private:
    string dateCreated;

    string date;
    double amount;
    string account;

    string category;
    string babyCategory;
    string description;


public:
    Expense(string, string, double, string, string, string, string);

    string getDateCreated() const;
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

Expense :: Expense(string DateCreated, string Date,
                   double Amt, string Cat, string bbyCat,
                   string Acc, string Desc) :
            dateCreated(DateCreated), 
            date(Date),
            amount(Amt),
            category(Cat),
            babyCategory(bbyCat),
            account(Acc),
            description(Desc) {}



/* EXPENSE Class MFs(Public): GETTERS */
/*------------------------------------*/

string Expense :: getDateCreated() const    { return dateCreated; }
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

    // WRITE dateCreated
    size = expense.dateCreated.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(expense.dateCreated.c_str(), size);

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

    // READ dateCreated
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date created size' from file");
    expense.dateCreated.resize(size);
    is.read(&expense.dateCreated[0], size);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date created' from file");

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
    string dateCreated;
    string date;
    double amount;
    string account;
    string description;


public:
    Allowance(string Date, double Amount, string Account, string Description)
    : dateCreated(getDate_Today()), date(date), amount(amount), account(account), description(description) {}

    string getDateCreated() const;
    string getDate() const;
    double getAmount() const;
    string getAccount() const;
    string getDescription() const;

    void setDate(string);
    void setAmount(double);
    void setAccount(string);
    void setDescription(string);
    void setDateCreated();

    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, Allowance& allowance);
};



/* ALLOWANCE class MF(Public): GETTERS */
/*-------------------------------------*/

string Allowance :: getDateCreated() const  { return dateCreated; }
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
void Allowance :: setDateCreated()                { dateCreated = getDate_Today(); }



/* ALLOWANCE class Friends (Public): File Handling Operators */
/*-----------------------------------------------------------*/

// WRITE Allowance data
ostream& operator<<(ostream& os, const Allowance& allowance) {
    size_t dateSize = allowance.date.size();
    size_t accountSize = allowance.account.size();
    size_t descSize = allowance.description.size();
    size_t dateCreatedSize = allowance.dateCreated.size();

    // WRITE dateCreated
    os.write(reinterpret_cast<const char*>(&dateCreatedSize), sizeof(dateCreatedSize));
    os.write(allowance.dateCreated.c_str(), dateCreatedSize);

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
    size_t dateSize, accountSize, descSize, dateCreatedSize;

    is.read(reinterpret_cast<char*>(&dateCreatedSize), sizeof(dateCreatedSize));
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date created size' from file");
    allowance.date.resize(dateCreatedSize);
    is.read(&allowance.dateCreated[0], dateCreatedSize);
    if (!is.good()) throw runtime_error(">> ERROR: Unable to read 'date created' from file");

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

    double totalExpenses;
    double totalAllowance;
    double totalExpenses_Today;
    double totalAllowance_Today;

    vector<Expense> expensesList;
    vector<Allowance> allowancesList;
    vector<Expense> expensesList_Today;
    vector<Allowance> allowancesList_Today;

    vector<SavingsAndExpenseLim> savingsList;
    vector<SavingsAndExpenseLim> expenseLimitsList;

    vector<Category> CategoryList;




protected:
    void calculateCurrentSavings(); // DONE
    void calculateTotalExpenses(); // DONE
    void calculateTotalAllowance(); // DONE
    void calculateTotalExpenses_Today(); // DONE
    void calculateTotalAllowance_Today(); // DONE
    void calculateTotalBudget(); // DONE

    void loadExpenses();
    void loadAllowances();
    void loadSavings();
    void loadExpenseLimits();
    void loadCategoryList();

    void saveExpenses() const;
    void saveAllowances() const;
    void saveSavings() const;
    void saveExpenseLimits() const;
    void saveCategoryList() const;




public:
    Budget();

    void setTotalBudget(double);
    void setAside_Savings(int, double);

    void addExpense(const Expense&);
    void addAllowance(const Allowance&);
    void addSavings(const SavingsAndExpenseLim&);
    void addExpenseLim(const SavingsAndExpenseLim&);
    void addCategory(const Category&);
    
    void removeAllowance(int);
    void removeExpense(int);
    void removeSavings(int);
    void removeExpenseLimit(int);

    void changeAllowance(int, int, string, double);
    void changeExpenses(int, int, string, string, double);
    void changeExpenseGoal(int, int, string, double);
    void changeSavingsGoal(int, int, string, double);
    void updateExpenseDateRange(const string&, const string&, int);
    void updateSavingsDateRange(const string&, const string&, int);


    void notifyDue_Savings(); // working~
    void notifyDue_ExpenseGoal(); // working~


    void displayAllowancesList(int);
    void displayExpensesList(int);
    void displaySavingsList();
    void displayExpenseLimitList();
    void displayCategoryList_parent();
    void displayCategoryList_bbys(int);

    void displayUpdateMenu(); // DONE

    void run_UpdateLimitExpenses(); // working~
    void run_LE_SetNewGoal(); // DONE
    void run_LE_EditGoal(); // 
    void run_LE_DeleteGoal();

    void run_UpdateSavings(); // working~
    void run_S_SetNewGoal();
    void run_S_EditGoal();
    void run_S_DeleteGoal();
    void run_SetAsideSavings();

    void run_UpdateAllowance(); // working~
    void run_AddAllowance();
    void run_DeleteAllowance();
    void run_EditAllowance();

    void run_UpdateExpense();
    void run_AddExpenses();
    void run_DeleteExpenses();
    void run_EditExpenses();

    void loadData();
    void saveState();

};














/*--------------------------------------------------------*/
/*           BUDGET class(Private): Calculator            */
/*--------------------------------------------------------*/

void Budget :: calculateCurrentSavings() {
    currentSavings = 0.0;
    for (const auto& savings : savingsList)
        currentSavings += savings.get_currentAmt();
}

void Budget :: calculateTotalAllowance() {
    totalAllowance = 0;
    for (const auto& allowance : allowancesList)
        totalAllowance += allowance.getAmount();

    calculateTotalAllowance_Today();
        totalAllowance += totalAllowance_Today;
}

void Budget :: calculateTotalExpenses() {
    totalExpenses = 0;
    for (const auto& expense : expensesList)
        totalExpenses += expense.getAmount();

    calculateTotalExpenses_Today();
    totalExpenses += totalExpenses_Today;
}

void Budget :: calculateTotalAllowance_Today() {
    totalAllowance_Today = 0;
    for (const auto& allowance : allowancesList_Today)
        totalAllowance_Today += allowance.getAmount();
}

void Budget :: calculateTotalExpenses_Today() {
    totalExpenses_Today = 0;
    for (const auto& expense : expensesList_Today)
        totalExpenses_Today += expense.getAmount();
}

void Budget :: calculateTotalBudget() {
    totalBudget = 0;
    calculateTotalAllowance();
    calculateTotalExpenses();
    calculateCurrentSavings();

    totalBudget = totalAllowance - totalExpenses - currentSavings;
}













/*--------------------------------------------------------*/
/*           BUDGET class(Public): CONSTRUCTOR            */
/*--------------------------------------------------------*/
Budget :: Budget() :
            totalBudget(0.0),
            currentSavings(0.0) {}














/*--------------------------------------------------------------------------------------*/
/*       BUDGET class MFs(Public): LOAD & SAVE Functions - Allowance and Expenses       */
/*--------------------------------------------------------------------------------------*/

// LOAD: Expenses list from file
void Budget :: loadExpenses() {
    ifstream inFile(ExpensesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Expense expense("", "", 0, "", "", "", "");
            inFile >> expense;

            if (expense.getDateCreated() == getDate_Today()) {
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

            if (allowance.getDateCreated() == getDate_Today()) {
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
            SavingsAndExpenseLim savingsData("", "", 0.0, 0.0, "");
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
            SavingsAndExpenseLim expenseLimitsData("", "", 0.0, 0.0, "");
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

// LOAD: Category List from file
void Budget :: loadCategoryList()
{
    ifstream inFILE(CategoryListFILE, ios::binary);
    if (inFILE.is_open()) {
        while(inFILE.peek() != EOF)
        {
            Category categoryHol("", 0);
            inFILE >> categoryHol;
            CategoryList.push_back(categoryHol);
        }
        inFILE.close();
    }
}

// SAVE: Category List to file
void Budget :: saveCategoryList() const
{
    ofstream outFILE(CategoryListFILE, ios::binary | ios::trunc);
    if (outFILE.is_open()) {
        for (const auto& cat : CategoryList)
        {
            outFILE << cat;
        }
        outFILE.close();
    }
}












/*----------------------------------------------------------*/
/*          BUDGET class MFs(Public): DISPLAY LISTS         */
/*----------------------------------------------------------*/

void Budget :: displayAllowancesList(int page = 1)
{
    int i;
    char border = 179;

    // Display: Headers
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH + 2);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);

    int AllowanceSize = allowancesList_Today.size();

    // Display: Allowances data per page
    if (AllowanceSize > 0)
    {
        int dataPerPage = 5;
        int maxPages = AllowanceSize / dataPerPage;
        if ((AllowanceSize % dataPerPage) != 0) maxPages ++;

        if (page > maxPages) page = maxPages;
        if (page < 0) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, AllowanceSize);

        int items = end - start + 1;

        for (; start < end; start++)
        {
            Allowance allowance = allowancesList_Today[start];

            // Convert: Amount to string with 2 decimal places
            double amt_db = allowance.getAmount();
            stringstream stream;
            stream << fixed << setprecision(2) << amt_db;
            string amt_str = stream.str();

            cout << "\n" << string(13, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(start + 1), WHITE, COLUMNWIDTH);
            displayTxtByColumn(allowance.getDate(), WHITE, COLUMNWIDTH);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH + 2);
            displayTxtByColumn(allowance.getAccount(), WHITE, COLUMNWIDTH);
            displayTxtByColumn(allowance.getDescription(), WHITE, 60);
        }

        // Display remaining empty slots
        if (items < 5) {
            int vacant = dataPerPage - items;

            for (i = 0; i < vacant; i++) {
                cout << "\n" << string(13, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(++start), WHITE, COLUMNWIDTH);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH + 2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("----- ----- -----", WHITE, 60);
            }
        }
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 5; i++) {
            cout << "\n" << string(13, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i + 1), WHITE, COLUMNWIDTH);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH + 2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("----- ----- -----", WHITE, 60);
        }
    }
}

void Budget :: displayExpensesList(int page = 1)
{
    int i;
    char border = 179;

    // Display: Headers
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH + 2);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED_NB("DESCRIPTION", BOLDWHITE, COLUMNWIDTH);

    int ExpensesSize = expensesList_Today.size();

    // Display: Expenses Data by page
    if (ExpensesSize > 0)
    {
        int dataPerPage = 5;
        int maxPages = ExpensesSize / dataPerPage;
        if ((ExpensesSize % dataPerPage) != 0) maxPages ++;

        if (page > maxPages) page = maxPages;
        if (page < 0) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, ExpensesSize);

        int items = end - start + 1;

        for (; start < end; start++)
        {
            Expense expense = expensesList_Today[start];

            // Convert: Amount to string with 2 decimal places
            double amt_db = expense.getAmount();
            stringstream stream;
            stream << fixed << setprecision(2) << amt_db;
            string amt_str = stream.str();

            cout << "\n" << string(13, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(start + 1), WHITE, COLUMNWIDTH);
            displayTxtByColumn(expense.getDate(), WHITE, COLUMNWIDTH);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH + 2);
            displayTxtByColumn(expense.getAccount(), WHITE, COLUMNWIDTH);
            displayTxtByColumn(expense.getDescription(), WHITE, 60);
        }

        // Display remaining empty slots
        if (items < 5) {
            int vacant = dataPerPage - items;

            for (i = 0; i < vacant; i++) {
                cout << "\n" << string(13, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(++start), WHITE, COLUMNWIDTH);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH + 2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("----- ----- -----", WHITE, 60);
            }
        }
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 5; i++) {
            cout << "\n" << string(13, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i + 1), WHITE, COLUMNWIDTH);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH + 2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("----- ----- -----", WHITE, 60);
        }
    }
}

void Budget :: displaySavingsList()
{
    int i = 1;
    int items = savingsList.size();

    // Display: Headers
    cout << string(5, ' ');
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE (Start-Due)", BOLDWHITE, 25);
    displayTxtByColumn_CENTERED("SAVINGS GOAL", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);
    displayTxtByColumn_CENTERED("CURRENT SAVINGS", BOLDWHITE, COLUMNWIDTH*2);

    // Display: Savings data
    for(const auto& savingsData : savingsList)
    {
        stringstream stream1, stream2;
        stream1 << fixed << setprecision(2) << savingsData.get_goal();
        stream2 << fixed << setprecision << savingsData.get_currentAmt();

        string amt_str1 = stream1.str();
        string amt_str2 = stream2.str();

        cout << "\n" << string(5, ' ');
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(savingsData.get_startDate() + " - " + savingsData.get_dueDate(), WHITE, 25);
        displayTxtByColumn( "P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(savingsData.get_desc(), WHITE, 60);
        displayTxtByColumn( "P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
    }

    // Display: Dummy data
    if (items < 4) {
        cout << "\n" << string(5, ' ');
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn("--/--/---- - --/--/----", WHITE, 25);
        displayTxtByColumn( "P 0.00", WHITE, COLUMNWIDTH+2);
        displayTxtByColumn("----------", WHITE, 60);
        displayTxtByColumn("P 0.00  /  P 0.00", MAGENTA, COLUMNWIDTH*2);
    }
}

void Budget :: displayExpenseLimitList()
{
    int i = 1;
    char border = 179;
    int items = expenseLimitsList.size();

    // Display: Headers
    cout << string(5, ' ') << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("DATE (Start-Due)", BOLDWHITE, 25);
    displayTxtByColumn_CENTERED("EXPENSE GOAL", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);
    displayTxtByColumn_CENTERED("CURRENT EXPENSES", BOLDWHITE, COLUMNWIDTH*2);

    // Display: All expense limit data
    for(const auto& expenseLimData : expenseLimitsList)
    {
        stringstream stream1, stream2;
        stream1 << fixed << setprecision(2) << expenseLimData.get_goal();
        stream2 << fixed << setprecision(2) << expenseLimData.get_currentAmt();

        string amt_str1 = stream1.str();
        string amt_str2 = stream2.str();

        cout << "\n" << string(5, ' ') << border;
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
        displayTxtByColumn(expenseLimData.get_startDate() + " - " + expenseLimData.get_dueDate(), WHITE, 25);
        displayTxtByColumn("P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(expenseLimData.get_desc(), WHITE, 60);
        displayTxtByColumn("P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
    }

    // Display: Dummy data
    if (items < 4) {
        for(int iter = items; iter < 4; iter++) {
            cout << "\n" << string(5, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i++), WHITE, COLUMNWIDTH);
            displayTxtByColumn("--/--/---- - --/--/----", WHITE, 25);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----------", WHITE, 60);
            displayTxtByColumn("P 0.00  /  P 0.00", MAGENTA, COLUMNWIDTH*2);
        }
    }
}

void Budget :: displayCategoryList_parent() {
    char border = 179;
    vector<string> ParentCategories;
    
    // Load all parent categories
    for (const auto& catHol : CategoryList) {
        ParentCategories.push_back(catHol.getParentCategory());
    }

    // Fill in vacant slots
    while (ParentCategories.size() < 10) ParentCategories.push_back("----------");

    // Display ParentCategories by 2 columns
    for (int i = 0; i < ParentCategories.size() / 2; i++) {
        cout << string(50, ' ') << border;
        displayTxtByColumn(to_string(i+1) + " " + ParentCategories[i], WHITE, 20);
        displayTxtByColumn(to_string(i+6) + " " + ParentCategories[i + 6], WHITE, 20);
        cout << "\n";
    }
}

void Budget :: displayCategoryList_bbys(int index_Parent) {
    if ((index_Parent <= 0) || (index_Parent > CategoryList.size())) return;

    char border = 179;
    vector<string> BbyCategories = CategoryList[index_Parent - 1].getBabyCategories();

    // Fill in vacant slots
    while (BbyCategories.size() < 10) BbyCategories.push_back("----------");

    // Display BbyCategories by 2 columns
    for (int i = 0; i < BbyCategories.size() / 2; i++) {
        cout << string(50, ' ') << border;
        displayTxtByColumn(to_string(i+1) + " " + BbyCategories[i], WHITE, 20);
        displayTxtByColumn(to_string(i+6) + " " + BbyCategories[i + 6], WHITE, 20);
        cout << "\n";
    }
}












/*------------------------------------------------------------------------*/
/*      BUDGET class MFs(Public): SETTERS, ADDERS, REMOVERS, EDITORS      */
/*------------------------------------------------------------------------*/

void Budget :: setTotalBudget(double budget) {
    totalBudget = budget;
}

void Budget :: setAside_Savings(int index, double saveAmt) {
    if (totalBudget < saveAmt)
        throw runtime_error(">> WARNING: The amount to set aside exceeds your budget.");

    if ((index <= 0) || (index > savingsList.size()))
        throw runtime_error(">> ERROR: There is no such Savings Goal set at said index.");

    SavingsAndExpenseLim& savingsHol = savingsList[index - 1];
    savingsHol.set_currentAmt(saveAmt);
}

void Budget :: addExpense(const Expense& expense) {
    expensesList_Today.push_back(expense);
}

void Budget :: addAllowance(const Allowance& allowance) {
    allowancesList_Today.push_back(allowance);
}

void Budget :: addExpenseLim(const SavingsAndExpenseLim& expenseLim) {
    if (expenseLimitsList.size() == 4)
        throw runtime_error(">> WARNING: There can only be 4 Expense Limits to set at max.");

    expenseLimitsList.push_back(expenseLim);
}

void Budget :: addSavings(const SavingsAndExpenseLim& savings) {
    if (savingsList.size() == 4)
        throw runtime_error(">> WARNING: There can only be 4 Savings to set at max.");
    
    savingsList.push_back(savings);
}

void Budget :: addCategory(const Category& categoryHol) {
    if (CategoryList.size() == 10)
        throw runtime_error(">> WARNING: There can only be 10 Categories at max.");

    CategoryList.push_back(categoryHol);
}

void Budget :: removeAllowance(int index) {
    if ((index >= 0) && (index < allowancesList_Today.size())) {
        allowancesList_Today.erase(allowancesList_Today.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid allowance index.");
    }
}

void Budget :: removeExpense(int index) {
    if ((index >= 0) && (index < expensesList_Today.size())) {
        expensesList_Today.erase(expensesList_Today.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid expense index.");
    }
}

void Budget :: removeExpenseLimit(int index) {
    if ((index >= 0) && (index < expenseLimitsList.size())) {
        expenseLimitsList.erase(expenseLimitsList.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid expense limit goal index.");
    }
}

void Budget :: removeSavings(int index) {
    if ((index >= 0) && (index < savingsList.size())) {
        savingsList.erase(savingsList.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid savings goal index.");
    }
}

// CHANGE: Allowance data created during present day [1-Date, 2-Amount, 3-Acc, 4-Desc]
void Budget :: changeAllowance(int index, int mode, string input_Str = "", double input_Db = 0)
{
    if ((index <= 0) || (index > allowancesList_Today.size())) 
        throw runtime_error(">> WARNING: Invalid allowance index.");

    Allowance& allowanceHol = allowancesList_Today[index - 1];

    switch (mode) {
        case 1:
            // Edit Date
            allowanceHol.setDate(input_Str);
            break;
        
        case 2:
            // Edit Amount
            allowanceHol.setAmount(input_Db);
            break;
        
        case 3:
            // Edit Account
            allowanceHol.setAccount(input_Str);
            break;
        
        case 4:
            // Edit Description
            allowanceHol.setDescription(input_Str);
            break;

        default:
            break;
    }
}

// CHANGE: Expenses data created during present day [1-Date, 2-Amount, 3-Category, 4-Acc, 5-Desc]
void Budget :: changeExpenses(int index, int mode, string input_Str = "", string input_Str2 = "", double input_Db = 0)
{
    if ((index <= 0) || (index > expensesList_Today.size())) return;
        throw runtime_error(">> WARNING: Invalid expense index.");
    
    Expense& expenseHol = expensesList_Today[index - 1];

    switch (mode) {
        case 1:
            // Change Date
            expenseHol.setDate(input_Str);
            break;
        
        case 2:
            // Change Amount
            expenseHol.setAmount(input_Db);
            break;

        case 3:
            // Change Category Parent Baby
            expenseHol.setCategory(input_Str);
            expenseHol.setBabyCategory(input_Str2);
            break;
        
        case 4:
            // Change Account
            expenseHol.setAccount(input_Str);
            break;
        
        case 5:
            // Change Description
            expenseHol.setDescription(input_Str);
        
        default:
            break;
    }
}

// CHANGE: Expense goal data [1-Goal, 2-Desc]
void Budget :: changeExpenseGoal(int index, int mode, string input1, double amt)
{
    if ((index <= 0) || (index > expenseLimitsList.size())) return;
        throw runtime_error(">> WARNING: Invalid expense goal index.");

    SavingsAndExpenseLim& expenseGoal = expenseLimitsList[index - 1];

    switch (mode) {
        case 1:
            // Change Expense Limit Goal
            expenseGoal.set_goal(amt);
            break;
        
        case 2:
            // Change Description
            expenseGoal.set_desc(input1);
            break;
            
        default:
            break;
    }
}

// CHANGE: Savings goal data [1-Goal, 2-Desc]
void Budget :: changeSavingsGoal(int index, int mode, string input1, double amt)
{
    if ((index <= 0) || (index > savingsList.size())) return;
        throw runtime_error(">> WARNING: Invalid savings goal index.");

    SavingsAndExpenseLim& savingsGoal = savingsList[index - 1];

    switch (mode) {
        case 1:
            // Change Savings Goal
            savingsGoal.set_goal(amt);
            break;
        
        case 2:
            // Change Description
            savingsGoal.set_desc(input1);
            break;
            
        default:
            break;
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














/* ------------------------------------------------------ */ 
/*               UPDATE: UPDATE MAIN MENU                 */ 
/* ------------------------------------------------------ */ 
void Budget :: displayUpdateMenu() {
    clearScreen();

    // Display: UPDATE title
    border(205);
    displayCenteredLine_Colored("UPDATE", BLUE);
    border(205);

    // Display: Options
    displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
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











/* ------------------------------------------------------ */ 
/*               UPDATE: LIMIT OF EXPENSES                */ 
/* ------------------------------------------------------ */ 
void Budget :: run_UpdateLimitExpenses() {
    string input;
    while (true) {
        clearScreen();
        /* ------------------- SET LIMIT EXPENSES MENU ------------------- */

        // Display: UPDATE(Limit Of Expenses) title
        border(205);
        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES", BLUE);
        border(205);

        // Display: Expenses Total and New Limit
        cout << BOLDWHITE << "  >> TOTAL EXPENSES(Today):    P " << totalExpenses_Today << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Expense Limit: EXPENSES:" << RESET << endl;
        displayExpenseLimitList();
        cout << "\n";
        cout << "\n";
        border(196);

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[ 1 ] SET NEW GOAL");
        displayCenteredLine_NoColor("[ 2 ] EDIT GOAL   ");
        displayCenteredLine_NoColor("[ 3 ] DELETE GOAL ");
        cout << "\n";
        displayCenteredLine_NoColor("[ R ] Return      ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input);


        /* ---------------------------------------------------------------- */
        if (input == "1")   // Perform: SET NEW GOAL
        {
            run_LE_SetNewGoal();
        }

        else if (input == "2") {
            run_LE_EditGoal();
        }
        else if (input == "R" || input == "r") {
            return; 
        }
    }
}

// ExpenseLimit/Savings: Set NEW GOAL [S, EL]
void Budget :: run_LE_SetNewGoal() {
    int inputflow = 1;
    string input_SNG;
    double input_db;

    // Input holders
    double newGoal = 0;
    string date1 = "MM/DD/YYYY";
    string date2 = "MM/DD/YYYY";
    string description = "----------";

    while (true)
    {
        clearScreen();
        // Display: UPDATE(TITLE)
        border(205);
        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES(Set New Goal)", BLUE);
        border(205);

        // Display: Expenses Total and New Limit
        cout << BOLDWHITE << "  >> SETTING NEW GOAL:" << RESET << endl;
        cout << string(5, ' ') << "* New Goal Amount:    " << GREEN << "P " << newGoal << RESET << endl;
        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  date1 << RESET << endl;
        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  date2 << RESET << endl;
        cout << string(5, ' ') << "* Description:        " << BLUE <<  description << RESET << endl;
        cout << "\n";
        border(196);


        // Display: Notice if expense limit reached max slot
        if (expenseLimitsList.size() >= 4) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored(">> You can set up to 4 GOALS at most!", YELLOW);
            displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
            getchar();
            return; // Return to MENU
        }

        else {
            switch (inputflow) {
                case 1: /* ----- Input: NEW GOAL ----- */
                    displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                    getline(cin, input_SNG);

                    if (isDouble(input_SNG))
                    {
                        input_db = stod(input_SNG);

                        if (input_db > 0) {
                            newGoal = input_db;
                            inputflow++; // Proceed to input START DATE
                        }
                        else {
                            // Display: WARNING for invalid input
                            border(196);
                            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                            displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                            displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                            getchar();
                        }
                    }
                    else if ((input_SNG == "R") || (input_SNG == "r")) {
                        // Return to menu
                        return;
                    }
                    else {
                        // Display: WARNING for invalid input
                        border(196);
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                        displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                        getchar();
                    }
                    break;


                case 2: /* ----- Input: START DATE ----- */
                    displayCenteredLine_NoNewLine(">> Enter START DATE: ", CYAN);
                    getline(cin, input_SNG);

                    if (validateDate(input_SNG)) {
                        date1 = input_SNG;
                        inputflow++; // Proceed to input DUE DATE
                    }
                    else if ((input_SNG == "R") || (input_SNG == "r")) {
                        // Return to menu
                        return;
                    }
                    else {
                        // Display: WARNING for invalid input
                        border(196);
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                        displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                        getchar();
                    }
                    break;


                case 3: /* ----- Input: DUE DATE ----- */
                    displayCenteredLine_NoNewLine(">> Enter DUE DATE: ", CYAN);
                    getline(cin, input_SNG);

                    if (validateSecondDate(date1, input_SNG)) {
                        date2 = input_SNG;
                        inputflow++; // Proceed to input DESCRIPTION
                    }
                    else if ((input_SNG == "R") || (input_SNG == "r")) {
                        // Return menu
                        return;
                    }
                    else {
                        // Display: WARNING for invalid input
                        border(196);
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                        displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                        getchar();
                    }
                    break;
                

                case 4: /* ----- Input: DESCRIPTION ----- */
                    cout << CYAN << "\t>> Enter DESCRIPTION(max 50 chars): " << RESET;
                    getline(cin, input_SNG);

                    if (input_SNG.size() <= 50) {
                        description = input_SNG;
                        inputflow++; // Proceed to notify user SET NEW GOAL SUCCESSFUL
                    }
                    else if ((input_SNG == "R") || (input_SNG == "r")) {
                        // Return menu
                        return;
                    }
                    else {
                        // Display: WARNING for invalid input
                        border(196);
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter description of 50 characters at most.", YELLOW);
                        displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                        getchar();
                    }
                    break;
                

                default:
                    // Add: New goal to Expense Limit List
                    SavingsAndExpenseLim goal(date1, date2, newGoal, 0, description);
                    addExpenseLim(goal);

                    // Display: NOTIFICATION for successfully adding new goal
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Setting new goal successful!", YELLOW);
                    displayCenteredLine_Colored(">> Press enter to continue...  ", YELLOW);
                    getchar();
                    return;
                    break;
            }
        }
    }
}









/*-----------------------------------------------------------*/
/*                      UPDATE: SAVINGS                      */
/*-----------------------------------------------------------*/

void Budget :: run_UpdateSavings() {
    string input;

    while (true) {
        clearScreen();
        // Display: UPDATE(SAVINGS) Title
        border(205);
        displayCenteredLine_Colored("UPDATE: SAVINGS", BLUE);
        border(205);

        // Display: List of SAVINGS schedules
        displaySavingsList();
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
void Budget :: run_UpdateAllowance() {
    string input;
    int page = 1;

    int maxPages = allowancesList_Today.size() / 5;
    if ((allowancesList_Today.size() % 5) != 0) maxPages++;

    while (true) {
        clearScreen();

        // Display: UPDATE ALLOWANCE title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE", BLUE);
        border(205);

        // Display: Allowances today
        cout << BOLDWHITE << "  >> DATA:" << RESET << endl;
        cout << "\n";
        displayAllowancesList(page);

        // Display: Options
        border(196);
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        displayCenteredLine_NoColor("[1] Previous Page           [4] DELETE Allowance");
        displayCenteredLine_NoColor("[2] Next Page               [5] EDIT Allowance  ");
        displayCenteredLine_NoColor("[3] ADD Allowance           [R] RETURN          ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Get user input
        getline(cin, input);

        if (input == "1") {
            // Display: PREVIOUS PAGE
            if (page > 1) page--;
        }
        else if (input == "2") {
            // Display: NEXT PAGE
            if (page < maxPages) page++;
        }
        else if (input == "3") {
            // Perform: ADD Allowance
            int inputFlow = 1;
            string date = "--/--/----";
            double amount = 0;
            string account = "-----";
            string desc = "-----";

            while (true) {
                clearScreen();

                // Display: UPDATE ALLOWANCE title
                border(205);
                displayCenteredLine_Colored("UPDATE: ALLOWANCE(ADD)", BLUE);
                border(205);
                
                // Display: Inputted data
                cout << BOLDWHITE << ">> NEW ALLOWANCE:\n" << RESET << endl;
                cout << "\t" << "* DATE:      " << date << endl;
                cout << "\t" << "* AMOUNT:    " << setprecision(2) << amount << endl;

                // Ask user for input
                switch (inputFlow) {
                    case 1:
                        displayCenteredLine_NoNewLine(">> Enter DATE(MM/DD/YYYY): ");
                        inputFlow++;
                        break;
                }
            }

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


/*-----------------------------------------------------------*/
/*                      UPDATE: SAVINGS                      */
/*-----------------------------------------------------------*/














/*-----------------------------------------------------------*/
/*                UPDATE: LOAD AND SAVE DATA                 */
/*-----------------------------------------------------------*/

void Budget :: loadData() {
    loadAllowances();
    loadExpenses();
    loadExpenseLimits();
    loadSavings();
    loadCategoryList();

    calculateTotalAllowance();
    calculateTotalExpenses();
    calculateTotalBudget();
}

void Budget :: saveState() {
    saveAllowances();
    saveExpenses();
    saveExpenseLimits();
    saveSavings();
    saveCategoryList();
}

















/*-----------------------------------------------------------*/
/*                    UPDATE: MAIN FUNCTION                  */
/*-----------------------------------------------------------*/

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
                budget.run_UpdateLimitExpenses();
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
