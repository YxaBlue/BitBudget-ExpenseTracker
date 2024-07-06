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
const string AccountListFILE = "ACCOUNTList.bin";
const string InboxFILE = "Inbox.bin";







/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*_________________________________________________________________________________*/
/*-------------------------------- FUNCTIONS LIST: --------------------------------*/
/*_________________________________________________________________________________*/

/* DISPLAY FUNCTIONS */
//
void border(char, int);
void displayCenteredLine_NoColor(const string&, int);
void displayCenteredLine_Colored(const string&, const string&, int);
void displayCenteredLine_NoNewLine(const string&, const string&, int);

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
tm parseDate(const string&);
bool isDateInRange(const string&, const string&, const string&);
string getCurrentTime();


/* BITBUDGET FUNCTIONS */
//
void print_BitBudgetMM(bool&, bool&);
void print_QuitMenu();











/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
/*------------------------------- CATEGORY CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Category {
private:
    string parentName;
    int babiesTotal;
    vector<string> babyNames;

public:
    // CONSTRUCTOR
    Category(string, int);

    // CATEGORY GETTERS
    string getParent() const;
    int getTotalBaby() const;
    vector<string> getBabies() const;

    // CATEGORY SETTERS
    void addBaby(string);
    void setParent(string);
    void setBaby(int, string);

    // Friend Functions
    friend ostream& operator<<(ostream& outFILE, const Category &categoryHol);
    friend istream& operator>>(istream& inFILE, Category &categoryHol);
};






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

    // GETTERS
    string get_startDate() const;
    string get_dueDate() const;
    double get_goal() const;
    double get_currentAmt() const;
    string get_desc() const;

    // SETTERS
    void set_startDate(string);
    void set_dueDate(string);
    void set_goal(double);
    void set_currentAmt(double);
    void set_desc(string);

    // SAVE & LOAD TO FILE
    friend ostream& operator<<(ostream& os, const SavingsAndExpenseLim& savingsHol);
    friend istream& operator>>(istream& is, SavingsAndExpenseLim& savingsHol);
};









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
    // CONSTRUCTORS
    Expense(string, string, double, string, string, string, string);

    // GETTERS
    string getDateCreated() const;
    string getDate() const;
    string getCategory() const;
    string getBabyCategory() const;
    string getDescription() const;
    string getAccount() const;
    double getAmount() const;

    // SETTERS
    void setDate(string);
    void setCategory(string);
    void setBabyCategory(string);
    void setDescription(string);
    void setAccount(string);
    void setAmount(double);

    // SAVE & LOAD TO FILE
    friend ostream& operator<<(ostream& os, const Expense& expense);
    friend istream& operator>>(istream& is, Expense& expense);
};






/*---------------------------------------------------------------------------------*/
/*-------------------------------- ALLOWANCE CLASS --------------------------------*/
/*---------------------------------------------------------------------------------*/

class Allowance {
private:
    string dateCreated;
    string date;
    double amount;
    string account;
    string description;


public:
    // CONSTRUCTORS
    Allowance(string DateCreated, string Date, double Amount, string Account, string Description)
    : dateCreated(DateCreated), date(Date), amount(Amount), account(Account), description(Description) {}

    // GETTERS
    string getDateCreated() const;
    string getDate() const;
    double getAmount() const;
    string getAccount() const;
    string getDescription() const;

    // SETTERS
    void setDate(string);
    void setAmount(double);
    void setAccount(string);
    void setDescription(string);
    void setDateCreated();

    // SAVE & LOAD TO FILE
    friend ostream& operator<<(ostream& os, const Allowance& allowance);
    friend istream& operator>>(istream& is, Allowance& allowance);
};






/*---------------------------------------------------------------------------------*/
/*--------------------------------- BUDGET CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Budget {
private:
    double totalBudget;
    double totalSavings;
    double totalExpenses_LOE;

    double totalGoal_Savings;
    double totalGoal_LOE;

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
    vector<string> AccountList;




protected:
    // CALCULATORS
    void calculateCurrentSavings();
    void calculateTotalGoal_Savings();
    void calculateTotalGoal_ExpenseLim();
    void calculateTotalExpenses();
    void calculateTotalAllowance();
    void calculateTotalExpenses_Today();
    void calculateTotalAllowance_Today();


    // LOAD/SAVE DATA
    void loadExpenses();
    void loadAllowances();
    void loadSavings();
    void loadExpenseLimits();
    void loadCategoryList();
    void loadAccountList();
    void saveExpenses() const;
    void saveAllowances() const;
    void saveSavings() const;
    void saveExpenseLimits() const;
    void saveCategoryList() const;
    void saveAccountList() const;


    // UPDATE: LIMIT OF EXPENSES [Features] ~DONE, need to deal with notifications
    void displayMenu_UpdateLE();
    void run_LE_SetNewGoal();
    void run_LE_EditGoal();
    void run_LE_DeleteGoal();

    // UPDATE: SAVINGS [Features] ~DONE, need to deal with notifications
    void displayMenu_UpdateSavings();
    void run_S_SetNewGoal();
    void run_S_EditGoal();
    void run_S_DeleteGoal();
    void run_SetAsideSavings();
    bool track_SavingsGoal(); // working~



    // UPDATE: ALLOWANCE [Features]
    void run_AddAllowance(); // working~
    void run_DeleteAllowance(); // working~
    void run_EditAllowance(); // working~

    // UPDATE: EXPENSES [Features]
    void displayMenu_UpdateExpense(int);
    void run_AddExpenses(); // working~
    void run_DeleteExpenses(); 
    void run_EditExpenses(); // working~
    void track_ExpensesLimit(bool&); // working~

    // UPDATE: ALLOWANCE/EXPENSE [Menu to add Accounts, Category, Subcategory]
    void run_AddAccount(string);
    void run_AddCategory(string);
    void run_AddSubcategory(int, string);



    // CLASS ADDERS
    void addExpense(const Expense&);
    void addAllowance(const Allowance&);
    void addSavings(const SavingsAndExpenseLim&);
    void addExpenseLim(const SavingsAndExpenseLim&);
    void addCategory(const Category&);
    void addAccount(const string&);


    // ALLOWANCE/EXPENSE/GOAL REMOVERS
    void removeAllowance(int);
    void removeExpense(int);
    void removeSavings(int);
    void removeExpenseLimit(int);


    // DATE UPDATERS
    void updateExpenseDateRange(const string&, const string&, int);
    void updateSavingsDateRange(const string&, const string&, int);





public:
    // CONSTRUCTORS
    Budget();


    // CALCULATORS
    void calculateTotalBudget();
    void setTotalBudget(double);


    // NOTIFIERS
    void notifyDue_Savings(); // working~
    void notifyDue_ExpenseGoal(); // working~


    // DISPLAY DATA [ALLOWANCES / EXPENSES / CATEGORIES / ETC.]
    void displayAllowancesList_today(int);
    void displayExpensesList_today(int);
    void displaySavingsList();
    void displayExpenseLimitList();
    void displayCategoryList_parent();
    void displayCategoryList_bbys(int);
    void displayAccountList();

    void displayUpdateMenu();


    // UPDATE: ALL MAIN FEATURES TO RUN IN MAIN FUNCTION
    void run_UpdateLimitExpenses(bool&); // DONE, need to work on how to add up the expenses.
    void run_UpdateSavings(bool&); // DONE, need to work on the notification part
    void run_UpdateAllowance(); // working
    void run_UpdateExpense(bool&); // working~

    // UPDATE: Run UPDATE FEATURE
    void run_BBUpdate(bool&);

    // LOAD/SAVE ALL DATA
    void loadData();
    void saveState();
};







/*---------------------------------------------------------------------------------*/
/*------------------------------- NOTIFICATION CLASS ------------------------------*/
/*---------------------------------------------------------------------------------*/

class Notification {
private:
    string reminder =               "   (OvO)/  Spent anything today? Update your tracker now! \n    Go to UPDATE and record today's transaction\n";
    string ExpenseLimReport_Neg =   "   (O-O)!  Uh oh... You have exceeded your expense limit!\n    Better tone down the spending when possible!\n";
    string savingsReport =          "   (>u<)/  Your savings has now been added back to your allowance!\n    Take a look at UPDATE: SAVINGS\n";
    string ExpenseLimReport_Pos  =  "   (O-O)!  Hey! An expense goal has reached its due!\n    You spent your money pretty well! GREAT JOB!\n";
    string loginNotif =             "   (OwO)/ Hello! Welcome to BitBudget: Expense Tracker!\n    Get started by UPDATEing, or maybe take a look at your DATA & HISTORY XD\n";


public:
    bool createNotification(int, string);
};










/*---------------------------------------------------------------------------------*/
/*---------------------------------- INBOX CLASS ----------------------------------*/
/*---------------------------------------------------------------------------------*/

class Inbox {
private:
    vector<string> NotificationList;

    void loadNotifications();
    void saveNotifications();

    void displayNotifications(int, int);
    void deleteNotif(int);
    void clearInbox();

public:
    void run_Inbox();
};


















/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*_________________________________________________________________________________*/
/*------------------------ DISPLAY TEXT-STYLE UI FUNCTIONS ------------------------*/
/*_________________________________________________________________________________*/
// Function to print a border with a given character
void border(char c, int length = SCREENWIDTH)
{
    for (int i = 0; i < length; i++) cout << c;
    cout << endl;
}

// Function to display centered lines
void displayCenteredLine_NoColor(const string &line, int width = SCREENWIDTH)
{
    int padLen = (width - line.size()) / 2;
    cout << string(padLen, ' ') << line << endl;
}

// Function to display centered lines with a color
void displayCenteredLine_Colored(const string &line, const string &color, int width = SCREENWIDTH)
{
    int padLen = (width - line.size()) / 2;
    cout << color << string(padLen, ' ') << line << RESET << endl;
}

// Function to display centered lines with color (no '\n')
void displayCenteredLine_NoNewLine(const string &line, const string &color, int width = SCREENWIDTH)
{
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

// Function to display lines by column, centered
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

// Function to display lines by column, without border
void displayTxtByColumn_NB(const string &str, const string &color, int width = COLUMNWIDTH)
{
    int totalSpaces = width - str.size() - 1;
    cout << color << " " << str << string(totalSpaces, ' ') << RESET;
}

// Function to display centered lines by column, centered and without border
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
// Function to clear all contents of the file
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

// Function to check if string is a positive integer
bool isNumeric(string str) {
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            return false;
        }
    }

    return true;
}

// Function to check if string is a positive double value
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

// Function to validate date format in MM/DD/YYYY
bool validateDateFormat(const string &date) {
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

// Function to validate date if it is present or later
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

// Function to validate the second date if it is later than or the same date
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

// Function to get date today in MM/DD/YYYY format
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

// Function to parse date in MM/DD/YYYY format
tm parseDate(const std::string& date) {
    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%m/%d/%Y");
    return tm;
}

// Function to compare two dates
bool isDateInRange(const string& dateStr, const string& startStr, const string& endStr) {
    tm date = parseDate(dateStr);
    tm startDate = parseDate(startStr);
    tm endDate = parseDate(endStr);

    time_t dateT = mktime(&date);
    time_t startT = mktime(&startDate);
    time_t endT = mktime(&endDate);

    return (dateT >= startT && dateT <= endT);
}

// Function to get current time (Format: HH:MM AM/PM)
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    ostringstream oss;
    oss << put_time(ltm, "%I:%M %p");
    return oss.str();
}

// Function to check if current time matches given time
bool isTimeNow(const string& targetTime) {
    string currTime = getCurrentTime();
    return currTime == targetTime;
}










/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


/* CATEGORY CLASS: Constructors */
Category :: Category(string catName_parent, int babyCat_Total) :
                parentName(catName_parent),
                babiesTotal(babyCat_Total) {}

/* CATEGORY CLASS: Constructors */
//
string Category :: getParent() const            { return parentName; }
int Category :: getTotalBaby() const            { return babiesTotal; }
vector<string> Category :: getBabies() const    { return babyNames; }

/* CATEGORY CLASS: Public MFs */
//
void Category :: addBaby(string babyCatName)
{
    babyNames.push_back(babyCatName);
    babiesTotal = babyNames.size();
}
void Category :: setParent(string newParent)  { parentName = newParent; }
void Category :: setBaby(int index, string newBby) {
    if ((index >= 0) && (index < babyNames.size()))
    babyNames[index] = newBby;
}

// WRITE category/ies
ostream& operator<<(ostream& outFILE, const Category &categoryHol)
{
    size_t size;

    // WRITE parent category name
    size = categoryHol.parentName.size();
    outFILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outFILE.write(categoryHol.parentName.c_str(), size);

    // WRITE total baby categories
    outFILE.write(reinterpret_cast<const char*>(&categoryHol.babiesTotal), sizeof(categoryHol.babiesTotal));

    // WRITE all baby categories
    for(const auto& babyCategory : categoryHol.babyNames)
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
    if (!inFILE.good()) throw runtime_error(">> ERROR[Category]: Unable to read 'parent category name size' from file");
    categoryHol.parentName.resize(size);
    inFILE.read(&categoryHol.parentName[0], size);
    if (!inFILE.good()) throw runtime_error(">> ERROR[Category]: Unable to read 'parent category name' from file");

    // READ Total Baby Categories
    inFILE.read(reinterpret_cast<char*>(&categoryHol.babiesTotal), sizeof(categoryHol.babiesTotal));
    if(!inFILE.good()) throw runtime_error(">> ERROR[Category]: Unable to read 'Total baby category/ies name' from file");

    // READ Baby Category Name/s
    categoryHol.babyNames.clear();
    for (int i = 0; i < categoryHol.babiesTotal; i++)
    {
        inFILE.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (!inFILE.good()) throw runtime_error(">> ERROR[Category]: Unable to read 'baby category name size' from file");
        catNameHol.resize(size);
        inFILE.read(&catNameHol[0], size);
        if (!inFILE.good()) throw runtime_error(">> ERROR[Category]: Unable to read 'baby category name' from file");
        categoryHol.babyNames.push_back(catNameHol); 

    }
    return inFILE;
}







/* SavingsAndExpenseLim Class (Public): CONSTRUCTORS */
SavingsAndExpenseLim :: SavingsAndExpenseLim(string start, string due, double amt, double currAmt, string desc) :
    startDate(start), 
    dueDate(due),
    goal(amt),
    currentAmt(currAmt),
    description(desc) {}

/* SavingsAndExpenseLim Class (Public): GETTERS */
string SavingsAndExpenseLim :: get_startDate() const        { return startDate; }
string SavingsAndExpenseLim :: get_dueDate() const          { return dueDate; }
double SavingsAndExpenseLim :: get_goal() const             { return goal; }
double SavingsAndExpenseLim :: get_currentAmt() const       { return currentAmt; }
string SavingsAndExpenseLim :: get_desc() const             { return description; }

/* SavingsAndExpenseLim Class (Public): SETTERS */
void SavingsAndExpenseLim :: set_startDate(string newDate)              { startDate = newDate; }
void SavingsAndExpenseLim :: set_dueDate(string newDate)                { dueDate = newDate; }
void SavingsAndExpenseLim :: set_goal(double newGoal)                   { goal = newGoal; }
void SavingsAndExpenseLim :: set_currentAmt(double newCurrSavings)      { currentAmt = newCurrSavings; }
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

    // WRITE description
    size = savingsHol.description.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(savingsHol.description.c_str(), size);

    return os;
}

// READ SavingsAndExpenseLim data
istream& operator>>(istream& is, SavingsAndExpenseLim& savingsHol)
{
    size_t size;

    //READ start Date
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'startDate size' from file");
    savingsHol.startDate.resize(size);
    is.read(&savingsHol.startDate[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'startDate' from file");

    //READ dueDate
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'dueDate size' from file");
    savingsHol.dueDate.resize(size);
    is.read(&savingsHol.dueDate[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'dueDate' from file");

    // READ goal
    is.read(reinterpret_cast<char*>(&savingsHol.goal), sizeof(savingsHol.goal));
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'goal' from file");

    // READ currentAmt
    is.read(reinterpret_cast<char*>(&savingsHol.currentAmt), sizeof(savingsHol.currentAmt));
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'current amount' from file");

    //READ description
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'description size' from file");
    savingsHol.description.resize(size);
    is.read(&savingsHol.description[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Savings/LOE]: Unable to read 'description' from file");

    return is;
}







/* EXPENSE Class (Public): CONSTRUCTOR */
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
string Expense :: getDateCreated() const    { return dateCreated; }
string Expense :: getDate() const           { return date; }
double Expense :: getAmount() const         { return amount; }
string Expense :: getCategory() const       { return category; }
string Expense :: getBabyCategory() const   { return babyCategory; }
string Expense :: getAccount() const        { return account; }
string Expense :: getDescription() const    { return description; }

/* EXPENSE Class MFs(Public): SETTERS */
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
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'date created size' from file");
    expense.dateCreated.resize(size);
    is.read(&expense.dateCreated[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'date created' from file");

    // READ date
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'date size' from file");
    expense.date.resize(size);
    is.read(&expense.date[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'date' from file");

    // READ amount
    is.read(reinterpret_cast<char*>(&expense.amount), sizeof(expense.amount));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'amount' from file");

    // READ category
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'category size' from file");
    expense.category.resize(size);
    is.read(&expense.category[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'category' from file");

    // READ baby category
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'baby category size' from file");
    expense.babyCategory.resize(size);
    is.read(&expense.babyCategory[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'baby category' from file");

    // READ account
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'account size' from file");
    expense.account.resize(size);
    is.read(&expense.account[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'account' from file");

    // READ description
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'description size' from file");
    expense.description.resize(size);
    is.read(&expense.description[0], size);
    if (!is.good()) throw runtime_error(">> ERROR[Expenses]: Unable to read 'description' from file");

    return is;
}







/* ALLOWANCE class MF(Public): GETTERS */
string Allowance :: getDateCreated() const  { return dateCreated; }
string Allowance :: getDate() const         { return date; }
double Allowance :: getAmount() const       { return amount; }
string Allowance :: getAccount() const      { return account; }
string Allowance :: getDescription() const  { return description; }

/* ALLOWANCE class MF(Public): SETTERS */
void Allowance :: setDate(string newDate)         { date = newDate; }
void Allowance :: setAmount(double newAmt)        { amount = newAmt; }
void Allowance :: setAccount(string newAcc)       { account = newAcc; }
void Allowance :: setDescription(string newDesc)  { description = newDesc; }
void Allowance :: setDateCreated()                { dateCreated = getDate_Today(); }

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

/*--------------------------------------------------------------------------------------*/
/*                          BUDGET class(Protected): CALCULATOR                         */
/*--------------------------------------------------------------------------------------*/

void Budget :: calculateCurrentSavings()
{
    totalSavings = 0.0;
    for (const auto& savings : savingsList)
        totalSavings += savings.get_currentAmt();
}

void Budget :: calculateTotalGoal_Savings()
{
    totalGoal_Savings = 0.0;
    for (const auto& savings : savingsList) {
        totalGoal_Savings += savings.get_goal();
    }
}

void Budget :: calculateTotalGoal_ExpenseLim()
{
    totalGoal_LOE = 0.0;
    for (const auto& LOE : expenseLimitsList) {
        totalGoal_LOE += LOE.get_goal();
    }
}

void Budget :: calculateTotalAllowance()
{
    totalAllowance = 0;
    for (const auto& allowance : allowancesList)
        totalAllowance += allowance.getAmount();

    calculateTotalAllowance_Today();
        totalAllowance += totalAllowance_Today;
}

void Budget :: calculateTotalExpenses()
{
    totalExpenses = 0;
    for (const auto& expense : expensesList)
        totalExpenses += expense.getAmount();

    calculateTotalExpenses_Today();
    totalExpenses += totalExpenses_Today;
}

void Budget :: calculateTotalAllowance_Today()
{
    totalAllowance_Today = 0;
    for (const auto& allowance : allowancesList_Today) {
        if (allowance.getDate() == getDate_Today())
        totalAllowance_Today += allowance.getAmount();

        else totalAllowance += allowance.getAmount();
    }
}

void Budget :: calculateTotalExpenses_Today()
{
    totalExpenses_Today = 0;
    for (const auto& expense : expensesList_Today) {
        if (expense.getDate() == getDate_Today())
            totalExpenses_Today += expense.getAmount();

        else totalExpenses += expense.getAmount();
    }
        
}

void Budget :: calculateTotalBudget()
{
    totalBudget = 0;
    calculateTotalAllowance();
    calculateTotalExpenses();
    calculateCurrentSavings();

    totalBudget = totalAllowance - totalExpenses - totalSavings;
}






/*--------------------------------------------------------------------------------------------------*/
/*     BUDGET class MFs(Protected): LOAD & SAVE Functions - Allowance/Expenses/Categories/ETC.      */
/*--------------------------------------------------------------------------------------------------*/

// Budget function member to load expenses from file to ExpensesList & ExpensesList_Today
void Budget :: loadExpenses()
{
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

// Budget function member to save expenses to file
void Budget :: saveExpenses() const
{
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

// Budget function member to load allowances from file AllowanceList & AllowanceList_Today
void Budget :: loadAllowances()
{
    ifstream inFile(AllowancesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Allowance allowance("", "", 0.0, "", "");
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

// Budget function member to save allowances to file
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

// Budget function member to load savings goals from file to SavingsList
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

// Budget function member to save savings goals to file
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

// Budget function member to load expense limit goals from file to ExpenseLimList
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

// Budget function member to save expense limit goals to file
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

// Budget function member to load categories from file to CategoryList
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

// Budget function member to save categories to file
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

// Budget function member to load accounts from file to AccountList
void Budget :: loadAccountList()
{
    size_t size;
    string acc;

    ifstream inFILE(AccountListFILE, ios::binary);
    if (inFILE.is_open()) {
        while(inFILE.read(reinterpret_cast<char*>(&size), sizeof(size)))
        {
            acc.resize(size);
            inFILE.read(&acc[0], size);
            AccountList.push_back(acc);
        }
        inFILE.close();
    }
}

// Budget function member to save accounts to file
void Budget :: saveAccountList() const
{
    size_t size;

    ofstream outFILE(AccountListFILE, ios::binary | ios::trunc);
    if (outFILE.is_open()) {
        for (const auto& acc : AccountList) {
            size = acc.size();
            outFILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
            outFILE.write(acc.c_str(), size);
        }
        outFILE.close();
    }
}








/*--------------------------------------------------------------------------------------*/
/*                          BUDGET class(Public): CONSTRUCTOR                           */
/*--------------------------------------------------------------------------------------*/
// 
Budget :: Budget() :
            totalBudget(0.0),
            totalSavings(0.0) {}





/*--------------------------------------------------------------------------------------*/
/*                        BUDGET class MFs(Public): DISPLAY LISTS                       */
/*--------------------------------------------------------------------------------------*/
// Budget function to display allowances created at present day
void Budget :: displayAllowancesList_today(int page = 1)
{
    int i;
    char border = 179;

    // Display: Headers
    cout << string(3, ' ') << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH+2);
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

            cout << "\n" << string(3, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(start + 1), WHITE, 7);
            displayTxtByColumn(allowance.getDate(), WHITE, COLUMNWIDTH);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getAccount(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getDescription(), WHITE, 60);
        }

        // Display remaining empty slots
        if (items < 5) {
            int vacant = dataPerPage - items;

            for (i = 0; i <= vacant; i++) {
                cout << "\n" << string(3, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(++start), WHITE, 7);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("----- ----- -----", WHITE, 60);
            }
        }
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 5; i++) {
            cout << "\n" << string(3, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i + 1), WHITE, 7);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----- ----- -----", WHITE, 60);
        }
    }
}

// Budget function to display expenses created at present day
void Budget :: displayExpensesList_today(int page = 1)
{
    int i;
    char border = 179;

    // Display: Headers
    cout << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH-2);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("SUBCATEGORY", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 54);

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

            cout << "\n" << border;
            displayTxtByColumn_CENTERED(to_string(start+1), WHITE, 7);
            displayTxtByColumn(expense.getDate(), WHITE, COLUMNWIDTH-2);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(expense.getCategory(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(expense.getBabyCategory(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(expense.getAccount(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(expense.getDescription(), WHITE, 54);
        }

        // Display remaining empty slots
        if (items < 5) {
            int vacant = dataPerPage - items;

            for (i = 0; i <= vacant; i++) {
                cout << "\n" << border;
                displayTxtByColumn_CENTERED(to_string(++start), WHITE, 7);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH-2);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("----- ----- -----", WHITE, 54);
            }
        }
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 5; i++) {
            cout << "\n" << border;
            displayTxtByColumn_CENTERED(to_string(i+1), WHITE, 7);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH-2);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----- ----- -----", WHITE, 54);
        }
    }
}

// Budget function to display savings goals
void Budget :: displaySavingsList()
{
    int i = 1;
    char border = 179;
    int items = savingsList.size();

    // Display: Headers
    cout << string(3, ' ') << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE (Start-Due)", BOLDWHITE, 25);
    displayTxtByColumn_CENTERED("SAVINGS GOAL", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);
    displayTxtByColumn_CENTERED("STATUS", BOLDWHITE, COLUMNWIDTH*2);

    // Display: Savings data
    for(const auto& savingsData : savingsList)
    {
        stringstream stream1, stream2;
        stream1 << fixed << setprecision(2) << savingsData.get_goal();
        stream2 << fixed << setprecision(2) << savingsData.get_currentAmt();

        string amt_str1 = stream1.str();
        string amt_str2 = stream2.str();

        cout << "\n" << string(3, ' ') << border;
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
        displayTxtByColumn(savingsData.get_startDate() + " - " + savingsData.get_dueDate(), WHITE, 25);
        displayTxtByColumn( "P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(savingsData.get_desc(), WHITE, 60);
        if (getDate_Today() == savingsData.get_dueDate()) {
            displayTxtByColumn( "COMPLETED!", MAGENTA, COLUMNWIDTH*2);
        }
        else displayTxtByColumn( "P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
    }

    // Display: Dummy data
    if (items < 4) {
        for (int iter = items; iter < 4; iter++) {
            cout << "\n" << string(3, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
            displayTxtByColumn("--/--/---- - --/--/----", WHITE, 25);
            displayTxtByColumn( "P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----------", WHITE, 60);
            displayTxtByColumn("P 0.00  /  P 0.00", MAGENTA, COLUMNWIDTH*2);
        }
    }
}

// Budget function to display expense limit goals
void Budget :: displayExpenseLimitList()
{
    int i = 1;
    char border = 179;
    int items = expenseLimitsList.size();

    // Display: Headers
    cout << string(3, ' ') << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE (Start-Due)", BOLDWHITE, 25);
    displayTxtByColumn_CENTERED("EXPENSE GOAL", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);
    displayTxtByColumn_CENTERED("STATUS", BOLDWHITE, COLUMNWIDTH*2);

    // Display: All expense limit data
    for(const auto& expenseLimData : expenseLimitsList)
    {
        stringstream stream1, stream2;
        stream1 << fixed << setprecision(2) << expenseLimData.get_goal();
        stream2 << fixed << setprecision(2) << expenseLimData.get_currentAmt();

        string amt_str1 = stream1.str();
        string amt_str2 = stream2.str();

        cout << "\n" << string(3, ' ') << border;
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
        displayTxtByColumn(expenseLimData.get_startDate() + " - " + expenseLimData.get_dueDate(), WHITE, 25);
        displayTxtByColumn("P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(expenseLimData.get_desc(), WHITE, 60);

        if (getDate_Today() == expenseLimData.get_dueDate()) {
            displayTxtByColumn_CENTERED("COMPLETED!", MAGENTA, COLUMNWIDTH*2);
        }
        else displayTxtByColumn("P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
    }

    // Display: Dummy data
    if (items < 4) {
        for(int iter = items; iter < 4; iter++) {
            cout << "\n" << string(3, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
            displayTxtByColumn("--/--/---- - --/--/----", WHITE, 25);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----------", WHITE, 60);
            displayTxtByColumn("P 0.00  /  P 0.00", MAGENTA, COLUMNWIDTH*2);
        }
    }
}

// Budget function to display categories
void Budget :: displayCategoryList_parent()
{
    char border = 179;
    vector<string> ParentCategories;
    
    // Load all parent categories
    for (const auto& catHol : CategoryList) {
        ParentCategories.push_back(catHol.getParent());
    }

    // Fill in vacant slots
    while (ParentCategories.size() <= 10) ParentCategories.push_back("----------");

    // Display ParentCategories by 2 columns
    for (int i = 0; i < ParentCategories.size() / 2; i++) {
        cout << string(50, ' ') << border;
        displayTxtByColumn("[ " + to_string(i+1) + " ] " + ParentCategories[i], WHITE, 25);
        displayTxtByColumn("[ " + to_string(i+6) + " ] " + ParentCategories[i + 5], WHITE, 25);
        cout << "\n";
    }
}

// Budget function to display subcategories of a category (index_Parent must be +1)
void Budget :: displayCategoryList_bbys(int index_Parent)
{
    if ((index_Parent <= 0) || (index_Parent > CategoryList.size())) return;

    char border = 179;
    vector<string> BbyCategories = CategoryList[index_Parent - 1].getBabies();

    // Fill in vacant slots
    while (BbyCategories.size() <= 10) BbyCategories.push_back("----------");

    // Display BbyCategories by 2 columns
    for (int i = 0; i < BbyCategories.size() / 2; i++) {
        cout << string(50, ' ') << border;
        displayTxtByColumn("[ " + to_string(i+1) + " ] " + BbyCategories[i], WHITE, 25);
        displayTxtByColumn("[ " + to_string(i+6) + " ] " + BbyCategories[i + 5], WHITE, 25);
        cout << "\n";
    }
}

// Budget function to display accounts
void Budget :: displayAccountList()
{
    char border = 179;
    vector<string> Accounts;
    
    // Load all parent categories
    for (const auto& acc : AccountList) {
        Accounts.push_back(acc);
    }

    // Fill in vacant slots
    while (Accounts.size() <= 10) Accounts.push_back("----------");

    // Display ParentCategories by 2 columns
    for (int i = 0; i < Accounts.size() / 2; i++) {
        cout << string(50, ' ') << border;
        displayTxtByColumn("[ " + to_string(i+1) + " ] " + Accounts[i], WHITE, 25);
        displayTxtByColumn("[ " + to_string(i+6) + " ] " + Accounts[i + 5], WHITE, 25);
        cout << "\n";
    }
}







/*------------------------------------------------------------------------*/
/*      BUDGET class MFs(Public): SETTERS, ADDERS, REMOVERS, EDITORS      */
/*------------------------------------------------------------------------*/

void Budget :: setTotalBudget(double budget)
{
    totalBudget = budget;
}


void Budget :: addExpense(const Expense& expense)
{
    expensesList_Today.push_back(expense);
}

void Budget :: addAllowance(const Allowance& allowance)
{
    allowancesList_Today.push_back(allowance);
}

void Budget :: addExpenseLim(const SavingsAndExpenseLim& expenseLim)
{
    if (expenseLimitsList.size() == 4)
        throw runtime_error(">> WARNING: There can only be 4 Expense Limits to set at max.");

    expenseLimitsList.push_back(expenseLim);
}

void Budget :: addSavings(const SavingsAndExpenseLim& savings)
{
    if (savingsList.size() == 4)
        throw runtime_error(">> WARNING: There can only be 4 Savings to set at max.");
    
    savingsList.push_back(savings);
}

void Budget :: addCategory(const Category& categoryHol)
{
    if (CategoryList.size() == 10)
        throw runtime_error(">> WARNING: There can only be 10 Categories at max.");

    CategoryList.push_back(categoryHol);
}

void Budget :: addAccount(const string& acc)
{
    if (AccountList.size() == 10)
        throw runtime_error(">> WARNING: There can only be 10 Accounts at max.");

    AccountList.push_back(acc);
}


void Budget :: removeAllowance(int index)
{
    if ((index >= 0) && (index < allowancesList_Today.size())) {
        allowancesList_Today.erase(allowancesList_Today.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid allowance index.");
    }
}

void Budget :: removeExpense(int index)
{
    if ((index >= 0) && (index < expensesList_Today.size())) {
        expensesList_Today.erase(expensesList_Today.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid expense index.");
    }
}

void Budget :: removeExpenseLimit(int index)
{
    if ((index >= 0) && (index < expenseLimitsList.size())) {
        expenseLimitsList.erase(expenseLimitsList.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid expense limit goal index.");
    }
}

void Budget :: removeSavings(int index)
{
    if ((index >= 0) && (index < savingsList.size())) {
        savingsList.erase(savingsList.begin() + index);
        calculateTotalBudget();
    }
    else {
        throw runtime_error(">> WARNING: Invalid savings goal index.");
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
            SavingsAndExpenseLim& expenseLimitData = expenseLimitsList[index];
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
    if ((index < 0) || (index >= savingsList.size())) {
        throw runtime_error("Index not found in expensesList.");
    }

    // Updata savings date range
    if (validateDate(startDate) && validateSecondDate(startDate, dueDate)) {
            SavingsAndExpenseLim& savingsData = savingsList[index];
            savingsData.set_startDate(startDate);
            savingsData.set_dueDate(dueDate);
    }
    else {
        throw runtime_error("Invalid date range for savings.");
    }
}


















/* -------------------------------------------------------------------------- */ 
/*                          UPDATE: UPDATE MAIN MENU                          */ 
/* -------------------------------------------------------------------------- */ 

void Budget :: displayUpdateMenu() {
    clearScreen();

    // Display: UPDATE title
    border(205);
    displayCenteredLine_Colored("UPDATE", BLUE);
    border(205);

    cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Total Expenses(Today):    P " << fixed << setprecision(2) << totalExpenses_Today << "\n\n\n" << RESET << endl;
    border(196);

    // Display: Options
    displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ 1 ] LIMIT OF EXPENSES ", WHITE);
    displayCenteredLine_Colored("[ 2 ] SAVINGS           ", WHITE);
    displayCenteredLine_Colored("[ 3 ] Update ALLOWANCE  ", WHITE);
    displayCenteredLine_Colored("[ 4 ] Update TRANSACTION", WHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ R ] Return", WHITE);
    cout << "\n";

    border(196);
    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
}






/* ------------------------------------------------------------------------- */ 
/*                        UPDATE: LIMIT OF EXPENSES                          */ 
/* ------------------------------------------------------------------------- */

void Budget :: displayMenu_UpdateLE()
{
        clearScreen();
        
        // Display: UPDATE(Limit Of Expenses) title
        border(205);
        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES", BLUE);
        border(205);

        // Display: Expenses Total and New Limit
        cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Total Expenses(Today):    P " << fixed << setprecision(2) << totalExpenses_Today << "\n" << RESET << endl;
        border(196);
        cout << BOLDWHITE << "  >> Expense Limit:\n" << RESET << endl;
        displayExpenseLimitList();
        cout << "\n";
        cout << "\n";
        border(196);
}

void Budget :: run_UpdateLimitExpenses(bool& newNotif)
{
    string input;
    while (true) {
        displayMenu_UpdateLE();

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

        if (input == "1")       run_LE_SetNewGoal();
        else if(input == "2")   run_LE_EditGoal();
        else if (input == "3")  run_LE_DeleteGoal();
        
        else if (input == "R" || input == "r") return; 
    }
}

void Budget :: run_LE_SetNewGoal()
{
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
        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (SET NEW GOAL)", BLUE);
        border(205);

        // Display: Expenses Total and New Limit
        cout << BOLDWHITE << "  >> SETTING NEW GOAL:\n" << RESET << endl;
        cout << string(5, ' ') << "* New Goal Amount:    " << GREEN << "P " << newGoal << RESET << endl;
        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  date1 << RESET << endl;
        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  date2 << RESET << endl;
        cout << string(5, ' ') << "* Description:        " << BLUE <<  description << RESET << endl;
        cout << "\n";
        border(196);


        // Display: Notice if expense limit reached max slot
        if (expenseLimitsList.size() >= 4) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored  (">> You can set up to 4 GOALS at most!                      ", YELLOW);
            displayCenteredLine_NoNewLine(">> Returning to Limit of Expenses Menu... (Press 'ENTER')  ", YELLOW);
            getchar();
            return; // Return to MENU
        }

        else {
            switch (inputflow) {
                case 1: /* ----- Input: NEW GOAL ----- */
                    displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                    getline(cin, input_SNG);

                    if ((isDouble(input_SNG)) && (input_SNG.size() > 0))
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

void Budget :: run_LE_DeleteGoal()
{
    string input_str;
    int input_int;
    
    while (true) 
    {
        displayMenu_UpdateLE();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to DELETE");
        displayCenteredLine_NoColor(">> Enter 'R' to return             ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input_str);

        if (isNumeric(input_str) && (input_str.size() > 0)) {
            input_int = stoi(input_str);
            
            if ((input_int > 0) && (input_int <= expenseLimitsList.size())) {
                removeExpenseLimit(input_int - 1);

                // Display: Notice that Expense Limit goal successfully deleted
                border(196);
                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored  (">> Deleted successfully!                                   ", YELLOW);
                displayCenteredLine_NoNewLine(">> Returning to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                getchar();
                return;
            }
        }
        else if ((input_str == "R") || (input_str == "r")) {
            // End function and return to Limit of Expenses Menu
            return;
        }
    }
}

void Budget :: run_LE_EditGoal()
{
    string input_str;
    int input_int;

    while (true)
    {
        displayMenu_UpdateLE();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to EDIT");
        displayCenteredLine_NoColor(">> Enter 'R' to return           ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input_str);

        if ((isNumeric(input_str)) && (input_str.size() > 0)) {
            input_int = stoi(input_str);

            if ((input_int > 0) && (input_int <= expenseLimitsList.size()))
            {
                while (true) {
                    displayMenu_UpdateLE();

                    displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
                    cout << "\n";
                    displayCenteredLine_NoColor("[ 1 ] START & DUE DATE");
                    displayCenteredLine_NoColor("[ 2 ] GOAL            ");
                    displayCenteredLine_NoColor("[ 3 ] DESCRIPTION     ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

                    // Get user input
                    getline(cin, input_str);

                    if (((input_str == "R") || (input_str == "r") ||
                        (input_str == "1") || (input_str == "2") ||
                        (input_str == "3")) && (input_str.size() > 0))
                    break;
                }




                // End function and return to Limit Expenses Menu
                if ((input_str == "R") || (input_str == "r"))
                return;




                // Edit START & DUE GOAL
                else if (input_str == "1") {
                    SavingsAndExpenseLim expenseLim = expenseLimitsList[input_int - 1];
                    string date1 = expenseLim.get_startDate();
                    string date2 = expenseLim.get_dueDate();

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Limit Of Expenses) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING START & DUE DATE:" << RESET << endl;
                        cout << "\n";
                        cout << string(5, ' ') << "* Start Date:         " << YELLOW <<  date1 << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << YELLOW <<  date2 << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << expenseLim.get_goal() << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << BLUE <<  expenseLim.get_desc() << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        switch (inputflow) {
                            case 1: // Input START DATE
                                displayCenteredLine_NoNewLine("Enter Start Date(MM/DD/YYYY): ", CYAN);
                                getline(cin , input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (validateDate(input_str)) {
                                    date1 = input_str;
                                    inputflow++;
                                }
                                break;


                            case 2: // Input DUE DATE
                                displayCenteredLine_NoNewLine("Enter Due Date(MM/DD/YYYY): ", CYAN);
                                getline(cin , input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (validateSecondDate(date1, input_str)) {
                                    date2 = input_str;
                                    inputflow++;
                                }
                                break;


                            default: // Notify user EDIT SUCCESSFUL
                                updateExpenseDateRange(date1, date2, input_int - 1);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }



                // Edit AMOUNT
                else if (input_str == "2") {
                    SavingsAndExpenseLim& expenseLim = expenseLimitsList[input_int - 1];
                    double og_Goal = expenseLim.get_goal();
                    double goalHol;

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Limit Of Expenses) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING GOAL AMOUNT:" << RESET << endl;
                        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  expenseLim.get_startDate() << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  expenseLim.get_dueDate() << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << YELLOW <<  fixed << setprecision(2) << og_Goal << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << BLUE <<  expenseLim.get_desc() << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        

                        switch (inputflow) {
                            case 1:
                                displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                                getline(cin, input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if ((isDouble(input_str)) && (input_str.size() > 0)) {
                                    goalHol = stod(input_str);

                                    if (goalHol > 0) {
                                        og_Goal = goalHol;
                                        inputflow++;
                                    }
                                }
                                break;


                            default:
                                expenseLim.set_goal(og_Goal);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }



                // Edit DESCRIPTION
                else if (input_str == "3") {
                    SavingsAndExpenseLim& expenseLim = expenseLimitsList[input_int - 1];
                    string og_desc = expenseLim.get_desc();

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Limit Of Expenses) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING DESCRIPTION:" << RESET << endl;
                        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  expenseLim.get_startDate() << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  expenseLim.get_dueDate() << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << expenseLim.get_goal() << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << YELLOW <<  og_desc << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        switch (inputflow) {
                            case 1:
                                displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                                getline(cin, input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (input_str.size() <= 50) {
                                    og_desc = input_str;
                                    inputflow++;
                                }
                                break;


                            default:
                                expenseLim.set_desc(og_desc);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }
            }
            
        }
        else if ((input_str == "R") || (input_str == "r")) return;
    }
}







/*---------------------------------------------------------------------------*/
/*                             UPDATE: SAVINGS                               */
/*---------------------------------------------------------------------------*/

void Budget :: displayMenu_UpdateSavings()
{
    clearScreen();

    // Display: UPDATE(Savings) title
    border(205);
    displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
    border(205);

    // Display: List of SAVINGS schedules
    cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Savings Goals:\n" << RESET << endl;
    displaySavingsList();
    cout << "\n";
    cout << "\n";
    border(196);
}

void Budget :: run_UpdateSavings(bool& newNotif)
{
    string input;

    while (true) {
        displayMenu_UpdateSavings();

        // Display: OPTIONS
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[ 1 ] SET NEW GOAL     ");
        displayCenteredLine_NoColor("[ 2 ] EDIT GOAL        ");
        displayCenteredLine_NoColor("[ 3 ] DELETE GOAL      ");
        displayCenteredLine_NoColor("[ 4 ] SET ASIDE SAVINGS");
        cout << "\n";
        displayCenteredLine_NoColor("[ R ] Return           ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        getline(cin, input);


        if(input == "1")        run_S_SetNewGoal();
        else if(input == "2")   run_S_EditGoal();
        else if(input == "3")   run_S_DeleteGoal();
        else if(input == "4")   run_SetAsideSavings();

        else if (input == "R" || input == "r") return; 
        
    }
}

void Budget :: run_S_SetNewGoal()
{
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
        displayCenteredLine_Colored("UPDATE: SAVINGS (SET NEW GOAL)", BLUE);
        border(205);

        // Display: New Goal data
        cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
        border(196);
        cout << BOLDWHITE << "  >> SETTING NEW GOAL:" << RESET << endl;
        cout << string(5, ' ') << "* New Goal Amount:    " << GREEN << "P " << newGoal << RESET << endl;
        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  date1 << RESET << endl;
        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  date2 << RESET << endl;
        cout << string(5, ' ') << "* Description:        " << BLUE <<  description << RESET << endl;
        cout << "\n";
        cout << "\n";
        cout << "\n";
        border(196);


        // Display: Notice if Savings List reached max slot
        if (savingsList.size() >= 4) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored(">> You can set up to 4 GOALS at most!            ", YELLOW);
            displayCenteredLine_NoNewLine(">> Returning to Savings Menu... (Press 'ENTER')  ", YELLOW);
            getchar();
            return; // Return to MENU
        }

        else {
            switch (inputflow) {
                case 1: /* ----- Input: NEW GOAL ----- */
                    displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                    getline(cin, input_SNG);

                    if ((isDouble(input_SNG)) && (input_SNG.size() > 0))
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
                            displayCenteredLine_Colored(">> Please enter valid input.   ", YELLOW);
                            displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
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
                        displayCenteredLine_Colored(">> Please enter valid input.   ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
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
                        displayCenteredLine_Colored(">> Please enter valid input.   ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
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
                        displayCenteredLine_Colored(">> Please enter valid input.   ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
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
                        displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
                        getchar();
                    }
                    break;
                

                default:
                    // Add: New goal to Savings List
                    SavingsAndExpenseLim goal(date1, date2, newGoal, 0, description);
                    addSavings(goal);

                    // Display: NOTIFICATION for successfully adding new goal
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Setting new goal successful!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press enter to continue...  ", YELLOW);
                    getchar();
                    return;
                    break;
            }
        }
    }
}

void Budget :: run_S_EditGoal()
{
string input_str;
    int input_int;

    while (true)
    {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to EDIT");
        displayCenteredLine_NoColor(">> Enter 'R' to return           ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input_str);

        if ((isNumeric(input_str)) && (input_str.size() > 0)) {
            input_int = stoi(input_str);

            if ((input_int > 0) && (input_int <= savingsList.size()))
            {
                while (true) {
                    displayMenu_UpdateSavings();

                    displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
                    cout << "\n";
                    displayCenteredLine_NoColor("[ 1 ] START & DUE DATE");
                    displayCenteredLine_NoColor("[ 2 ] GOAL            ");
                    displayCenteredLine_NoColor("[ 3 ] DESCRIPTION     ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

                    // Get user input
                    getline(cin, input_str);

                    if (((input_str == "R") || (input_str == "r") ||
                         (input_str == "1") || (input_str == "2") ||
                         (input_str == "3")) && (input_str.size() > 0))
                    break;
                }




                // End function and return to Limit Expenses Menu
                if ((input_str == "R") || (input_str == "r"))
                return;




                // Edit START & DUE GOAL
                else if (input_str == "1") {
                    SavingsAndExpenseLim savings = savingsList[input_int - 1];
                    string date1 = savings.get_startDate();
                    string date2 = savings.get_dueDate();

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Savings) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING START & DUE DATE:" << RESET << endl;
                        cout << "\n";
                        cout << string(5, ' ') << "* Start Date:         " << YELLOW <<  date1 << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << YELLOW <<  date2 << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << savings.get_goal() << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << BLUE <<  savings.get_desc() << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        switch (inputflow) {
                            case 1: // Input START DATE
                                displayCenteredLine_NoNewLine("Enter Start Date(MM/DD/YYYY): ", CYAN);
                                getline(cin , input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (validateDate(input_str)) {
                                    date1 = input_str;
                                    inputflow++;
                                }
                                break;


                            case 2: // Input DUE DATE
                                displayCenteredLine_NoNewLine("Enter Due Date(MM/DD/YYYY): ", CYAN);
                                getline(cin , input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (validateSecondDate(date1, input_str)) {
                                    date2 = input_str;
                                    inputflow++;
                                }
                                break;


                            default: // Notify user EDIT SUCCESSFUL
                                updateSavingsDateRange(date1, date2, input_int - 1);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }



                // Edit AMOUNT
                else if (input_str == "2") {
                    SavingsAndExpenseLim& savings = savingsList[input_int - 1];
                    double og_Goal = savings.get_goal();
                    double goalHol;

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Savings) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING GOAL AMOUNT:" << RESET << endl;
                        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  savings.get_startDate() << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  savings.get_dueDate() << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << YELLOW <<  fixed << setprecision(2) << og_Goal << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << BLUE <<  savings.get_desc() << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        

                        switch (inputflow) {
                            case 1:
                                displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                                getline(cin, input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if ((isDouble(input_str)) && (input_str.size() > 0)) {
                                    goalHol = stod(input_str);

                                    if (goalHol > 0) {
                                        og_Goal = goalHol;
                                        inputflow++;
                                    }
                                }
                                break;


                            default:
                                savings.set_goal(og_Goal);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }



                // Edit DESCRIPTION
                else if (input_str == "3") {
                    SavingsAndExpenseLim& savings = savingsList[input_int - 1];
                    string og_desc = savings.get_desc();

                    int inputflow = 1;

                    while (true) {
                        clearScreen();

                        // Display: UPDATE(Savings) title
                        border(205);
                        displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
                        border(205);

                        // Display original data
                        cout << BOLDWHITE << "  >> EDITING DESCRIPTION:" << RESET << endl;
                        cout << string(5, ' ') << "* Start Date:         " << BLUE <<  savings.get_startDate() << RESET << endl;
                        cout << string(5, ' ') << "* Due Date:           " << BLUE <<  savings.get_dueDate() << RESET << endl;
                        cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << savings.get_goal() << RESET << endl;
                        cout << string(5, ' ') << "* Description:        " << YELLOW <<  og_desc << RESET << endl;
                        cout << "\n";
                        cout << "\n";
                        border(196);

                        switch (inputflow) {
                            case 1:
                                displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                                getline(cin, input_str);

                                if ((input_str == "R") || (input_str == "r")) return;

                                if (input_str.size() <= 50) {
                                    og_desc = input_str;
                                    inputflow++;
                                }
                                break;


                            default:
                                savings.set_desc(og_desc);

                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                                displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                                getchar();
                                return;
                                break;
                        }
                    }
                }
            }
            
        }
        else if ((input_str == "R") || (input_str == "r")) return;
    }
}

void Budget :: run_S_DeleteGoal()
{
    string input_str;
    int input_int;
    
    while (true) 
    {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to DELETE");
        displayCenteredLine_NoColor(">> Enter 'R' to return             ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input_str);

        if ((isNumeric(input_str)) && (input_str.size() > 0)) {
            input_int = stoi(input_str);
            
            if ((input_int > 0) && (input_int <= savingsList.size())) {
                removeSavings(input_int - 1);

                // Display: Notice that Savings goal successfully deleted
                border(196);
                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored(">> Deleted successfully!", YELLOW);
                displayCenteredLine_NoNewLine(">> Returning to Savings Menu... (Press 'ENTER')  ", WHITE);
                getchar();
                return;
            }
        }
        else if ((input_str == "R") || (input_str == "r")) {
            // End function and return to Savings Menu
            return;
        }
    }
}

void Budget :: run_SetAsideSavings()
{
    string input_str;
    int input_int;
    
    while (true) 
    {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to SET ASIDE SAVINGS");
        displayCenteredLine_NoColor(">> Enter 'R' to return                        ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input [index in savingsList]
        getline(cin, input_str);

        if ((input_str == "R") || (input_str == "r")) return;

        else if ((isNumeric(input_str)) && (input_str.size() > 0))
        {
            input_int = stoi(input_str);

            if ((input_int > 0) && (input_int <= savingsList.size()))
            {
                SavingsAndExpenseLim& savings = savingsList[input_int - 1]; 

                // Input valid amount to set aside
                while (true) {
                    clearScreen();
                
                    // Display: UPDATE(Savings) title
                    border(205);
                    displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
                    border(205);

                    cout << BOLDWHITE << "  >> Total Budget:      P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;

                    // Display original data
                    cout << BOLDWHITE << "  >> EDITING DESCRIPTION:" << RESET << endl;
                    cout << string(5, ' ') << "* Start Date:         " << BLUE << savings.get_startDate() << RESET << endl;
                    cout << string(5, ' ') << "* Due Date:           " << BLUE << savings.get_dueDate() << RESET << endl;
                    cout << string(5, ' ') << "* Description:        " << BLUE << savings.get_desc() << RESET << endl;
                    cout << "\n";
                    cout << string(5, ' ') << "* Current Amount:     " << BLUE << "P " << fixed << setprecision(2) << savings.get_currentAmt() << " / P " << savings.get_goal() << RESET << endl;
                    cout << "\n";
                    cout << "\n";
                    border(196);

                    displayCenteredLine_NoNewLine(">> Enter AMOUNT to save: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) return;

                    else if ((isDouble(input_str)) && (input_str.size() > 0)) {
                        double amt = stod(input_str);
                        double currentAmt = savings.get_currentAmt();
                        double maxAmt = savings.get_goal() - currentAmt;

                        if ((amt > 0) && (amt <= maxAmt) && (amt <= totalBudget)) {
                            savings.set_currentAmt(currentAmt + amt);
                            calculateTotalBudget();

                            // Display: Notice that Amount Saved Successfully
                            border(196);
                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                            displayCenteredLine_Colored(">> Amount is added to your Savings Goal!", YELLOW);
                            displayCenteredLine_NoNewLine(">> Returning to Savings Menu... (Press 'ENTER')  ", WHITE);
                            getchar();
                            return;
                        }
                    }

                }
            }

        }
    }
    
}






/*-----------------------------------------------------------------------------------------*/
/*             UPDATE: ALLOWANCE/EXPENSE [Add Accounts, Category, Subcategory]             */
/*-----------------------------------------------------------------------------------------*/
void Budget :: run_AddAccount(string TITLE)
{
    string input_str, confirm;
    int inputFlow = 1;

    while (true) {
        clearScreen();

        // Display: UPDATE(Expense) Title
        border(205);
        displayCenteredLine_Colored(TITLE, BLUE);
        border(205);

        cout << BOLDWHITE << "  >> ACCOUNT LIST:\n" << RESET << endl;
        displayAccountList();
        cout << "\n";
        border(196);

        if (AccountList.size() >= 10) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored(">> You can create up to 10 Accounts!", YELLOW);
            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..      ", WHITE);
            getchar();
            return; // Return to MENU
        }
        else {
            switch (inputFlow) {
                case 1: // INPUT NEW ACCOUNT
                    displayCenteredLine_NoNewLine(">> Enter NEW ACCOUNT(15 chars max): ", CYAN);
                    getline(cin, input_str);
                    
                    if ((input_str == "R") || (input_str == "r")) return;

                    else if ((input_str.size() > 0) && (input_str.size() <= 15)) {
                        border(196);
                        displayCenteredLine_Colored(">> Is this final? This cannot be changed after confirming...", BOLDYELLOW);
                        displayCenteredLine_NoNewLine(">> Enter (Y) to confirm. (ENTER) if otherwise: ", CYAN);
                        getline(cin , confirm);

                        if ((confirm == "Y") || (confirm == "y")) {
                            addAccount(input_str);
                            inputFlow++;
                        }
                    }
                    break;
                

                case 2: // NOTIFY AND END FUNCTION
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Account added successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..", WHITE);
                    getchar();
                    return; // Return to MENU
                    break;


                default:
                    break;
            }
        }

    }
}

void Budget :: run_AddCategory(string TITLE)
{
    string input_str, confirm;
    Category cat("", 0);
    int inputFlow = 1;

    while (true) {
        clearScreen();

        // Display: Title
        border(205);
        displayCenteredLine_Colored(TITLE, BLUE);
        border(205);

        cout << BOLDWHITE << "  >> CATEGORY LIST:\n" << RESET << endl;
        displayCategoryList_parent();
        cout << "\n";
        border(196);

        if (CategoryList.size() >= 10) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored(">> You can create up to 10 Categories!", YELLOW);
            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..      ", WHITE);
            getchar();
            return; // Return to MENU
        }
        else {
            switch (inputFlow) {
                case 1: // INPUT NEW CATEGORY
                    displayCenteredLine_NoNewLine(">> Enter NEW CATEGORY(15 chars max): ", CYAN);
                    getline(cin, input_str);
                    
                    if ((input_str == "R") || (input_str == "r")) return;

                    else if ((input_str.size() > 0) && (input_str.size() <= 15)) {
                        border(196);
                        displayCenteredLine_Colored(">> Is this final? This cannot be changed after confirming...", BOLDYELLOW);
                        displayCenteredLine_NoNewLine(">> Enter (Y) to confirm. (ENTER) if otherwise: ", CYAN);
                        getline(cin , confirm);

                        if ((confirm == "Y") || (confirm == "y")) {
                            cat.setParent(input_str);
                            addCategory(cat);
                            inputFlow++;
                        }
                    }
                    break;
                

                case 2: // NOTIFY AND END FUNCTION
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Category added successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..", WHITE);
                    getchar();
                    return; // Return to MENU
                    break;


                default:
                    break;
            }
        }

    }
}

void Budget :: run_AddSubcategory(int index, string TITLE)
{
    if ((index <= 0) || (index > CategoryList.size())) return;

    string input_str, confirm;
    Category& cat = CategoryList[index - 1];
    int inputFlow = 1;

    while (true) {
        clearScreen();

        // Display: Title
        border(205);
        displayCenteredLine_Colored(TITLE, BLUE);
        border(205);

        cout << BOLDWHITE << "  >> SUBCATEGORY LIST (" + cat.getParent() + "):" << RESET << endl;
        displayCategoryList_bbys(index);
        cout << "\n";
        border(196);

        if (cat.getTotalBaby() >= 10) {
            displayCenteredLine_Colored("WARNING", BOLDYELLOW);
            displayCenteredLine_Colored(">> You can create up to 10 Subcategories!", YELLOW);
            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..      ", WHITE);
            getchar();
            return; // Return to MENU
        }
        else {
            switch (inputFlow) {
                case 1: // INPUT NEW SUBCATEGORY
                    displayCenteredLine_NoNewLine(">> Enter NEW SUBCATEGORY(15 chars max): ", CYAN);
                    getline(cin, input_str);
                    
                    if ((input_str == "R") || (input_str == "r")) return;

                    else if ((input_str.size() > 0) && (input_str.size() <= 15)) {
                        border(196);
                        displayCenteredLine_Colored(">> Is this final? This cannot be changed after confirming...", BOLDYELLOW);
                        displayCenteredLine_NoNewLine(">> Enter (Y) to confirm. Press (ENTER) if otherwise: ", CYAN);
                        getline(cin , confirm);

                        if ((confirm == "Y") || (confirm == "y")) {
                            inputFlow++;
                            cat.addBaby(input_str);
                        }
                    }
                    break;
                

                case 2: // NOTIFY AND END FUNCTION
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Subcategory added successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue..", WHITE);
                    getchar();
                    return; // Return to MENU
                    break;


                default:
                    break;
            }
        }

    }
}







/*-----------------------------------------------------------------------------------------*/
/*                                   UPDATE: EXPENSES                                      */
/*-----------------------------------------------------------------------------------------*/

void Budget :: displayMenu_UpdateExpense(int page = 1)
{
    clearScreen();
    
    // Display: UPDATE(Expense) Title
    border(205);
    displayCenteredLine_Colored("UPDATE: EXPENSES", BLUE);
    border(205);

    // Display: Total Allowance and Expenses(today)
    cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
    cout << BOLDWHITE << "  >> Total Expenses(Today):    P " << fixed << setprecision(2) << totalExpenses_Today << "\n\n" << RESET << endl;
    border(196);

    // Display: Expenses Data
    cout << BOLDWHITE << "  >> Expenses (Today):\n" << RESET << endl;
    displayExpensesList_today(page);
    cout << "\n";
    cout << "\n";
    border(196);
}

void Budget :: run_UpdateExpense(bool& newNotif)
{
    int page = 1;
    int maxPages = expensesList_Today.size() / 5;
    if ((expensesList_Today.size() % 5) != 0) maxPages++;

    string input_str;

    while (true) {
        displayMenu_UpdateExpense(page);

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[1] Previous Page           [4] DELETE Expenses");
        displayCenteredLine_NoColor("[2] Next Page               [5] EDIT Expenses  ");
        displayCenteredLine_NoColor("[3] ADD Expenses            [R] RETURN         ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        getline(cin, input_str);

        if (input_str == "1") {
            // Display previous page
            if (page > 1) page--;
        }

        else if (input_str == "2") {
            // Display next page
            if (page < maxPages) page++;
        }

        else if (input_str == "3") run_AddExpenses();
        else if (input_str == "4") run_DeleteExpenses();
        else if (input_str == "5") run_EditExpenses();
        else if ((input_str == "R") || (input_str == "r")) return;
    }
}

void Budget :: run_AddExpenses()
{
    string input_str;
    int input_int, index;
    int inputFlow = 1;

    double amt;
    vector<string> bbys;
    Expense newExpense(getDate_Today(), "MM/DD/YYYY", 0.0, "----------", "----------", "----------", "----------");

    while (true) {
        clearScreen();
        
        // Display: UPDATE(Expense) Title
        border(205);
        displayCenteredLine_Colored("UPDATE: EXPENSES (ADD)", BLUE);
        border(205);

        // Display: Total Allowance and Expenses(today)
        cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Total Expenses(Today):      P " << fixed << setprecision(2) << totalExpenses_Today << "\n" << RESET << endl;
        border(196);

        // Display: New Expense Data
        if (inputFlow == 3) {
            // Display: Category List
            cout << BOLDWHITE << "  >> CATEGORY LIST: " << RESET << endl;
            displayCategoryList_parent();
            cout << "\n";
            displayCenteredLine_NoColor("[ A ] ADD New Category");
            cout << "\n";
            cout << BOLDWHITE << "  >> Enter the number of the Category you a want to input. Enter (A) if you want to add a new Category." << RESET << endl;
        }
        else if (inputFlow == 4) {
            // Display: Subcategory List
            cout << BOLDWHITE << "  >> SUBCATEGORY LIST: " << RESET << endl;
            displayCategoryList_bbys(input_int);
            cout << "\n";
            displayCenteredLine_NoColor("[ A ] ADD New Subcategory");
            cout << "\n";
            cout << BOLDWHITE << "  >> Enter the number of the Subcategory you a want to input. Enter (A) if you want to add a new Subcategory." << RESET << endl;
            cout << "\n";
        }
        else if (inputFlow == 5) {
            // Display Account List
            cout << BOLDWHITE << "  >> ACCOUNT LIST: " << RESET << endl;
            displayAccountList();
            cout << "\n";
            displayCenteredLine_NoColor("[ A ] ADD New Account");
            cout << "\n";
            cout << BOLDWHITE << "  >> Enter the number of the Account you a want to input. Enter (A) if you want to add a new Account." << RESET << endl;
            cout << "\n";
        }
        else {
            // Display: Input of new expense
            cout << BOLDWHITE << "  >> ADDING NEW EXPENSE: " << RESET << endl;
            cout << string(5, ' ') << "* DATE:             " << GREEN << newExpense.getDate() << RESET << endl;
            cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << newExpense.getAmount() << RESET << endl;
            cout << string(5, ' ') << "* CATEGORY:         " << GREEN << newExpense.getCategory() << RESET << endl;
            cout << string(5, ' ') << "* SUBCATEGORY:      " << GREEN << newExpense.getBabyCategory() << RESET << endl;
            cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << newExpense.getAccount() << RESET << endl;
            cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << newExpense.getDescription() << RESET << endl;
            cout << "\n";
            cout << "\n";
        }
        border(196);

        switch (inputFlow) {
            case 1: // Input DATE
                displayCenteredLine_NoNewLine(">> Enter DATE: ", CYAN);
                getline(cin , input_str);

                if (validateDateFormat(input_str)) {
                    newExpense.setDate(input_str);
                    inputFlow++;
                }
                else if ((input_str == "R") || (input_str == "r")) return;
                break;


            case 2: // Input AMOUNT
                displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                getline(cin , input_str);

                if ((isDouble(input_str)) && (input_str.size() > 0)) {
                    amt = stod(input_str);

                    if (totalBudget > 0) {
                        if ((amt > 0) && (amt <= totalBudget))
                        {
                            newExpense.setAmount(amt);
                            inputFlow++;
                        }
                    }
                    else {
                        if ((amt > 0) && (amt <= totalSavings))
                        {
                            // take away from savings
                        }
                    }
                    
                }
                else if ((input_str == "R") || (input_str == "r")) return;
                break;


            case 3: // Input PARENT CATEGORY
                displayCenteredLine_NoNewLine(">> Enter CATEGORY(index num): ", CYAN);
                getline(cin , input_str);
                
                if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                    input_int = stoi(input_str);

                    if ((input_int > 0) && (input_int <= CategoryList.size())) {
                        index = input_int;
                        Category cat = CategoryList[index - 1];

                        newExpense.setCategory(cat.getParent());
                        inputFlow++;
                    }
                }
                else if ((input_str == "A") || (input_str == "a")) run_AddCategory("UPDATE: EXPENSES (ADD)");
                else if ((input_str == "R") || (input_str == "r")) return;
                break;


            case 4: // Input BABY CATEGORY
                displayCenteredLine_NoNewLine(">> Enter SUBCATEGORY(index num.): ", CYAN);
                getline(cin , input_str);
                
                if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                    input_int = stoi(input_str);
                    Category cat = CategoryList[index - 1];
                    bbys = cat.getBabies();

                    if ((input_int > 0) && (input_int <= cat.getTotalBaby())) {
                        newExpense.setBabyCategory(bbys[input_int-1]);
                        inputFlow++;
                    }
                }
                else if ((input_str == "A") || (input_str == "a")) run_AddSubcategory(index, "UPDATE: EXPENSES (ADD)");
                else if ((input_str == "R") || (input_str == "r")) return;
                break;


            case 5: // Input ACCOUNT
                displayCenteredLine_NoNewLine(">> Enter ACCOUNT(index num): ", CYAN);
                getline(cin , input_str);
                
                if ((isNumeric(input_str)) &&  (input_str.size() > 0)) {
                    input_int = stoi(input_str); 
                    if ((input_int > 0) && (input_int <= AccountList.size())) {
                        newExpense.setAccount(AccountList[input_int - 1]);
                        inputFlow++;
                    }
                }

                else if ((input_str == "A") || (input_str == "a")) run_AddAccount("UPDATE: EXPENSES (ADD)");
                else if ((input_str == "R") || (input_str == "r")) return;

                break;


            case 6: // Input DESCRIPTION
                cout << CYAN << "\t>> Enter DESCRIPTION (50 chars max): " << RESET;
                getline(cin , input_str);

                if (input_str.size() <= 50) {
                    newExpense.setDescription(input_str);
                    inputFlow++;
                }
                else if ((input_str == "R") || (input_str == "r")) return;
                break;


            default: // NOTIFY: New Expense Added.
                addExpense(newExpense);

                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored  (">> New Expense added successfully!", YELLOW);
                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...   ", WHITE);
                getchar();
                return; // Return to MENU
                break;
        }
    }
}

void Budget :: run_DeleteExpenses()
{
    int page = 1;
    int maxPages = expensesList_Today.size() / 5;
    if ((expensesList_Today.size() % 5) != 0) maxPages++;

    string input_str;
    int input_int;
    vector<string> subcategories;

    while (true) {
        displayMenu_UpdateExpense(page);

        displayCenteredLine_Colored("OPTIONS (DELETE)", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[P] Previous Page           [N] Next Page");
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter index number of Expense data to delete");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        getline(cin, input_str);

        if ((input_str == "P") || (input_str == "p")) {
            // Display previous page
            if (page > 1) page--;
        }

        else if ((input_str == "N") || (input_str == "n")) {
            // Display next page
            if (page < maxPages) page++;
        }

        else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
            input_int = stoi(input_str);

            if ((input_int > 0) && (input_int <= expensesList_Today.size())) {
                removeExpense(input_int - 1);

                // Notify: Expense Deleted Successfully
                border(196);
                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored(">> Expense data deleted successfully!", YELLOW);
                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                getchar();
                return;
            }
        }

    }
}

void Budget :: run_EditExpenses()
{
    int page = 1;
    int maxPages = expensesList_Today.size() / 5;
    if ((expensesList_Today.size() % 5) != 0) maxPages++;

    string input_str;
    int input_int, i = 0, subCat_size;
    int inputFlow = 1;

    while (true) {
        displayMenu_UpdateExpense(page);

        // Display: Options
        displayCenteredLine_Colored("OPTIONS (EDIT)", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[P] Previous Page           [N] Next Page");
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter index number of Expense data to edit");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Get user input
        getline(cin, input_str);


        if ((input_str == "P") || (input_str == "p")) {
            // Display previous page
            if (page > 1) page--;
        }

        else if ((input_str == "N") || (input_str == "n")) {
            // Display next page
            if (page < maxPages) page++;
        }

        else if ((input_str == "R") || (input_str == "r")) {
            // End function
            return;
        }

        else if ((isNumeric(input_str)) && (input_str.size() > 0))
        {
            input_int = stoi(input_str);

            if ((input_int > 0) && (input_int <= expensesList_Today.size()))
            {
                // Get expense and make reference
                Expense& expense = expensesList_Today[input_int - 1];

                // PERFORM: EDIT
                while (true) {
                    displayMenu_UpdateExpense(page);

                    // Display: Options to edit
                    displayCenteredLine_Colored("OPTIONS (EDIT)", BOLDWHITE);
                    cout << "\n";
                    displayCenteredLine_NoColor("[ 1 ] DATE                      [ 4 ] ACCOUNT    ");
                    displayCenteredLine_NoColor("[ 2 ] AMOUNT                    [ 5 ] DESCRIPTION");
                    displayCenteredLine_NoColor("[ 3 ] CATEGORY & SUBCATEGORY    [ R ] Return     ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

                    // Get input from user
                    getline(cin, input_str);

                    if ((input_str == "R") || ((input_str == "r"))) return;
                    if ((isNumeric(input_str)) && (input_str.size() > 0))
                    {
                        input_int = stoi(input_str);

                        if ((input_int >= 1) && (input_int <= 6))
                        {
                            switch (input_int)
                            {
                                case 1: /* EDIT DATE */
                                    while (true) {
                                        clearScreen();

                                        // Display: UPDATE(Expense) Title
                                        border(205);
                                        displayCenteredLine_Colored("UPDATE: EXPENSES (EDIT)", BLUE);
                                        border(205);

                                        // Display: Expense Details to be edited
                                        cout << BOLDWHITE << "  >> EDITING DATE: " << RESET << endl;
                                        cout << string(5, ' ') << "* DATE:             " << YELLOW << expense.getDate() << RESET << endl;
                                        cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << expense.getAmount() << RESET << endl;
                                        cout << string(5, ' ') << "* CATEGORY:         " << GREEN << expense.getCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* SUBCATEGORY:      " << GREEN << expense.getBabyCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << expense.getAccount() << RESET << endl;
                                        cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << expense.getDescription() << RESET << endl;
                                        border(196);

                                        switch (inputFlow) {
                                            case 1:
                                                displayCenteredLine_NoNewLine(">> Enter NEW DATE: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;
                                                else if (validateDate(input_str)) {
                                                    expense.setDate(input_str);
                                                    inputFlow++;
                                                }
                                                break;

                                            default:
                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                displayCenteredLine_Colored(">> Expense Date edited successfully!", YELLOW);
                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                                                getchar();
                                                return;
                                                break;
                                        }
                                    }
                                    break;
                                

                                case 2: /* EDIT AMOUNT */
                                    while (true) {
                                        clearScreen();

                                        // Display: UPDATE(Expense) Title
                                        border(205);
                                        displayCenteredLine_Colored("UPDATE: EXPENSES (EDIT)", BLUE);
                                        border(205);

                                        // Display: Expense Details to be edited
                                        cout << BOLDWHITE << "  >> EDITING AMOUNT: " << RESET << endl;
                                        cout << string(5, ' ') << "* DATE:             " << GREEN << expense.getDate() << RESET << endl;
                                        cout << string(5, ' ') << "* AMOUNT:           " << YELLOW << "P " << fixed << setprecision(2) << expense.getAmount() << RESET << endl;
                                        cout << string(5, ' ') << "* CATEGORY:         " << GREEN << expense.getCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* SUBCATEGORY:      " << GREEN << expense.getBabyCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << expense.getAccount() << RESET << endl;
                                        cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << expense.getDescription() << RESET << endl;
                                        border(196);

                                        switch (inputFlow) {
                                            case 1:
                                                displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;
                                                else if ((isDouble(input_str)) && (input_str.size() > 0)) {
                                                    double amt = stod(input_str);
                                                    if (amt > 0) {
                                                        expense.setAmount(amt);
                                                        inputFlow++;
                                                    }
                                                }
                                                break;

                                            default:
                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                displayCenteredLine_Colored(">> Expense Amount edited successfully!", YELLOW);
                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                                                getchar();
                                                return;
                                                break;
                                        }
                                    }
                                    break;

                                
                                case 3: /* EDIT CATEGORY */
                                    while (true) {
                                        clearScreen();

                                        // Display: UPDATE(Expense) Title
                                        border(205);
                                        displayCenteredLine_Colored("UPDATE: EXPENSES (EDIT)", BLUE);
                                        border(205);

                                        // Display: Expense Details to be edited
                                        cout << BOLDWHITE << "  >> EDITING CATEGORY & SUBCATEGORY: " << RESET << endl;
                                        cout << string(5, ' ') << "* DATE:             " << GREEN << expense.getDate() << RESET << endl;
                                        cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << expense.getAmount() << RESET << endl;
                                        cout << string(5, ' ') << "* CATEGORY:         " << YELLOW << expense.getCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* SUBCATEGORY:      " << YELLOW << expense.getBabyCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << expense.getAccount() << RESET << endl;
                                        cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << expense.getDescription() << RESET << endl;
                                        border(196);

                                        // Display: Category & Subcategory List
                                        if (inputFlow == 1) {
                                            cout << BOLDWHITE << "  >> CATEGORY LIST: " << RESET << endl;
                                            displayCategoryList_parent();
                                            cout << "\n";
                                            displayCenteredLine_NoColor("[ A ] ADD New Category");
                                            cout << "\n";
                                            border(196);
                                        }
                                        else if (inputFlow == 2) {
                                            cout << BOLDWHITE << "  >> SUBCATEGORY LIST: " << RESET << endl;
                                            displayCategoryList_bbys(i);
                                            cout << "\n";
                                            displayCenteredLine_NoColor("[ A ] ADD New Subcategory");
                                            cout << "\n";
                                            border(196);
                                        }
                                        
                                        
                                        switch (inputFlow) {
                                            case 1: // Get user input
                                                displayCenteredLine_NoNewLine(">> Enter NEW CATEGORY: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;

                                                else if ((input_str =="A") || (input_str == "a")) run_AddCategory("UPDATE: EXPENSES (EDIT)");

                                                else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                                                    input_int = i = stoi(input_str);

                                                    if ((input_int > 0) && (input_int <= CategoryList.size())) {
                                                        Category cat = CategoryList[input_int - 1];
                                                        expense.setCategory(cat.getParent());
                                                        inputFlow++;
                                                    }
                                                }
                                                break;
                                            
                                            case 2:
                                                displayCenteredLine_NoNewLine(">> Enter NEW SUBCATEGORY: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;

                                                else if ((input_str =="A") || (input_str == "a")) run_AddSubcategory(i, "UPDATE: EXPENSES (EDIT)");

                                                else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                                                    input_int = stoi(input_str);

                                                    if ((input_int > 0) && (input_int <= subCat_size)) {
                                                        Category cat = CategoryList[i-1];
                                                        vector<string> subcats = cat.getBabies();
                                                        string newSubcat = subcats[input_int-1];

                                                        expense.setBabyCategory(newSubcat);
                                                        inputFlow++;
                                                    }
                                                }
                                                break;

                                            default: // Display: Notice edit successful
                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                displayCenteredLine_Colored(">> Expense Category & Subcategory edited successfully!", YELLOW);
                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                                                getchar();
                                                return;
                                                break;
                                        }
                                    }
                                    break;



                                case 4: /* EDIT ACOUNT */
                                    while (true) {
                                        clearScreen();

                                        // Display: UPDATE(Expense) Title
                                        border(205);
                                        displayCenteredLine_Colored("UPDATE: EXPENSES (ADD)", BLUE);
                                        border(205);

                                        // Display: Expense Details to be edited
                                        cout << BOLDWHITE << "  >> EDITING ACCOUNT: " << RESET << endl;
                                        cout << string(5, ' ') << "* DATE:             " << GREEN << expense.getDate() << RESET << endl;
                                        cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << expense.getAmount() << RESET << endl;
                                        cout << string(5, ' ') << "* CATEGORY:         " << GREEN << expense.getCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* SUBCATEGORY:      " << GREEN << expense.getBabyCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* ACCOUNT:          " << YELLOW << expense.getAccount() << RESET << endl;
                                        cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << expense.getDescription() << RESET << endl;
                                        border(196);

                                        // Display: Account List
                                        cout << BOLDWHITE << "  >> ACCOUNT LIST: " << RESET << endl;
                                        displayAccountList();
                                        cout << "\n";
                                        displayCenteredLine_NoColor("[ A ] ADD New Account");
                                        cout << "\n";
                                        border(196);
                                        
                                        switch (inputFlow) {
                                            case 1:
                                                displayCenteredLine_NoNewLine(">> Enter NEW ACCOUNT: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;

                                                else if ((input_str =="A") || (input_str == "a")) run_AddAccount("UPDATE: EXPENSES (EDIT)");

                                                else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                                                    input_int = stoi(input_str);

                                                    if ((input_int > 0) && (input_int <= AccountList.size())) {
                                                        string acc = AccountList[input_int - 1];
                                                        expense.setAccount(acc);
                                                        inputFlow++;
                                                    }
                                                    
                                                }
                                                break;

                                            default:
                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                displayCenteredLine_Colored(">> Expense Account edited successfully!", YELLOW);
                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                                                getchar();
                                                return;
                                                break;
                                        }
                                    }
                                    break;


                                case 5: /* EDIT DESCRIPTION */
                                    while (true) {
                                        clearScreen();

                                        // Display: UPDATE(Expense) Title
                                        border(205);
                                        displayCenteredLine_Colored("UPDATE: EXPENSES (ADD)", BLUE);
                                        border(205);

                                        // Display: Expense Details to be edited
                                        cout << BOLDWHITE << "  >> EDITING DESCRIPTION: " << RESET << endl;
                                        cout << string(5, ' ') << "* DATE:             " << GREEN << expense.getDate() << RESET << endl;
                                        cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << expense.getAmount() << RESET << endl;
                                        cout << string(5, ' ') << "* CATEGORY:         " << GREEN << expense.getCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* SUBCATEGORY:      " << GREEN << expense.getBabyCategory() << RESET << endl;
                                        cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << expense.getAccount() << RESET << endl;
                                        cout << string(5, ' ') << "* DESCRIPTION:      " << YELLOW << expense.getDescription() << RESET << endl;
                                        border(196);

                                        switch (inputFlow) {
                                            case 1:
                                                displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                                                getline(cin, input_str);

                                                if ((input_str == "R") || (input_str == "r")) return;

                                                else if ((input_str =="A") || (input_str == "a")) run_AddAccount("UPDATE: EXPENSES (EDIT)");

                                                else if (input_str.size() <= 50) {
                                                    expense.setDescription(input_str);
                                                    inputFlow++;
                                                }
                                                break;

                                            default:
                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                displayCenteredLine_Colored(">> Expense Description edited successfully!", YELLOW);
                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...      ", WHITE);
                                                getchar();
                                                return;
                                                break;
                                        }
                                    }
                                    break;

                                
                                default:
                                    break;
                            }
                        }
                    }
                }

            }

        }

    }
}






/*--------------------------------------------------------------------------*/
/*                UPDATE: EXPENSE LIMIT AND SAVINGS TRACKER                 */
/*--------------------------------------------------------------------------*/
// Budget function member to track all savings goal and send notification
bool Budget :: track_SavingsGoal() {
    Notification notifHandler;
    bool savingsDue = false;
    int iter = 1;

    for (auto& savings : savingsList)
    {
        if (getDate_Today() == savings.get_dueDate()) {
            // Create notification that says t
            if ((savings.get_currentAmt() > 0) && (savings.get_currentAmt() < savings.get_goal())) {
                totalAllowance += savings.get_currentAmt();
                savings.set_currentAmt(0.0);
                savingsDue = true;
            }
            else if (savings.get_currentAmt() == 0) {

            }
            totalAllowance += savings.get_currentAmt();
            savings.set_currentAmt(0.0);
            savingsDue = true;
        }

        iter++;
    }

    return savingsDue;
}

// Budget function to remove savings goal the after due date








/*---------------------------------------------------------------------------*/
/*                        UPDATE: LOAD AND SAVE DATA                         */
/*---------------------------------------------------------------------------*/
// Budget function member to load all necessary data
void Budget :: loadData()
{
    loadAllowances();
    loadExpenses();
    loadExpenseLimits();
    loadSavings();
    loadCategoryList();
    loadAccountList();

    calculateTotalAllowance();
    calculateTotalExpenses();
    calculateTotalBudget();
}

// Budget function member to save latest state of data to file
void Budget :: saveState()
{
    saveAllowances();
    saveExpenses();
    saveExpenseLimits();
    saveSavings();
    saveCategoryList();
    saveAccountList();

    allowancesList.clear();
    allowancesList_Today.clear();
    expensesList.clear();
    expensesList_Today.clear();
    savingsList.clear();
    expenseLimitsList.clear();
    CategoryList.clear();
    AccountList.clear();
}






/*---------------------------------------------------------------------------*/
/*                        UPDATE: RUN UPDATE FUNCTION                        */
/*---------------------------------------------------------------------------*/

void Budget :: run_BBUpdate(bool& newNotif) {
    loadData();

    string input;
    while (true) {
        calculateTotalBudget();
        displayUpdateMenu();
        getline(cin, input);

        if (input == "R" || input == "r") {
            saveState();
            break;
        }

        try {
            if (input == "1") {
                run_UpdateLimitExpenses(newNotif);
            }
            else if (input == "2") {
                run_UpdateSavings(newNotif);
            }
            else if (input == "3") {
                //run_UpdateAllowance();
            }
            else if (input == "4") {
                run_UpdateExpense(newNotif);
            }
            else {
                cout << "Invalid option, please try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

















/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// READ Notifications from file
void Inbox :: loadNotifications()
{
    size_t size;
    string notif;

    ifstream inFILE(InboxFILE, ios::binary);
    if (inFILE.is_open())
    {
        while (inFILE.read(reinterpret_cast<char*>(&size), sizeof(size)))
        {
            notif.resize(size);
            inFILE.read(&notif[0], size);
            NotificationList.push_back(notif);
        }

        inFILE.close();
    }
}

// WRITE Notifications to file
void Inbox :: saveNotifications()
{
    if (NotificationList.size() == 0) return;

    size_t size;
    ofstream outFILE(InboxFILE, ios::binary | ios::trunc);
    if (outFILE.is_open())
    {
        for (const auto& notifs : NotificationList)
        {
            size = notifs.size();
            outFILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
            outFILE.write(notifs.c_str(), size);
        }

        outFILE.close();
    }
}

// DISPLAY notifications by page
void Inbox :: displayNotifications(int page = 1, int msgPerPage = 4)
{
    int maxPages = NotificationList.size() / msgPerPage;
    if ((NotificationList.size() % msgPerPage) != 0) maxPages++;

    if (page > maxPages) page = maxPages;
    else if (page < 0) page = 1;

    if (NotificationList.size() > 0)
    {
        int total_Notifs = NotificationList.size();
        int total_Pages = NotificationList.size() / msgPerPage;

        if ((total_Notifs % msgPerPage) != 0) total_Pages++;
        if (page < 1) page = 1;

        int msgIndex = (page - 1) * msgPerPage;
        int msg_LastIndex = min(msgIndex + msgPerPage, total_Notifs);

        for (int i = msgIndex; i < msg_LastIndex; i++) {
            cout << BOLDWHITE << "[" << (i + 1) << "]: " << RESET << NotificationList[i] << endl;
        }

        cout << "\n\n";
        displayCenteredLine_Colored(to_string(page) + " out of " + to_string(total_Pages) + " pages ", GRAY);
        border(196);
        return;
    }
    else {
        // Notify User that INBOX is empty
        displayCenteredLine_Colored("Your inbox do be looking desolate here...", YELLOW);
        cout << "\n";
        displayCenteredLine_Colored("Chile, anyways.", YELLOW);
        displayCenteredLine_Colored("Be on your merry way and UPDATE your tracker XD", YELLOW);
        
        cout << "\n\n\n\n\n\n";
        border(196);
        return;
    }
}

// DELETE notification by index
void Inbox :: deleteNotif(int index)
{
    if ((index > 0) && (index <= NotificationList.size()))
    NotificationList.erase(NotificationList.begin() + index - 1);
}

// CLEAR inbox file and Notification list
void Inbox :: clearInbox()
{
    clearFile(InboxFILE);
    NotificationList.clear();
}

// RUN INBOX FEATURE
void Inbox :: run_Inbox()
{
    string choice_Str;
    int choice_Int;
    string input_Str;
    int input_Int;

    loadNotifications();

    int page = 1;
    int maxPages = NotificationList.size()/4;
    if ((NotificationList.size() % 4) != 0) maxPages++;

    while (true) {        
        // Display: INBOX MENU
        do {
            clearScreen();

            // Display: INBOX TITLE
            border(205);
            displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
            border(205);

            // Display: Notifications
            displayNotifications(page);

            // Display: Options
            displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
            cout << "\n";
            displayCenteredLine_NoColor("[ 1 ]  Previous Page       [ 3 ]  DELETE");
            displayCenteredLine_NoColor("[ 2 ]  Next Page           [ 4 ]  CLEAR ");
            displayCenteredLine_NoColor("             [ R ]  Return              ");
            cout << "\n";
            displayCenteredLine_NoNewLine(">> Enter choice: ", CYAN);

            // Ask for input
            getline(cin, choice_Str);

            if ((choice_Str == "R") || (choice_Str == "r")) {
                saveNotifications();
                NotificationList.clear();
                return;
            }
        } while ((choice_Str != "1") && (choice_Str != "2") && (choice_Str != "3") && (choice_Str != "4"));
        
        choice_Int = stoi(choice_Str);

        switch (choice_Int) {
            case 1:
                // Display: Previous page
                if (page > 1) page--;
                break;

            case 2:
                // Display: Next Page
                if (page < maxPages) page++;
                break;
            
            case 3:
                if (NotificationList.size() == 0) break;

                // Run DELETE FEATURE
                while (true) {
                    clearScreen();

                    // Display: INBOX TITLE
                    border(205);
                    displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                    border(205);

                    // Display: Notifications
                    displayNotifications(page);

                    // Display: Options
                    displayCenteredLine_Colored("OPTIONS: DELETE", BOLDWHITE);
                    cout << "\n";
                    displayCenteredLine_NoColor(">> Enter a number of the message you want to delete.");
                    displayCenteredLine_NoColor(">> Enter 'R' to go back.                            ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter valid input: ", CYAN);

                    // Get input from user
                    getline(cin, input_Str);

                    // Return to INBOX Menu
                    if ((input_Str == "R") || (input_Str == "r")) break;

                    // Perform DELETE
                    else if (isNumeric(input_Str) && (input_Str.size() != 0))
                    {
                        input_Int = stoi(input_Str);

                        if ((input_Int > 0) && (input_Int <= NotificationList.size()))
                        {
                            // Perform: DELETE Notification
                            deleteNotif(input_Int);
                            clearScreen();

                            // Display: INBOX TITLE
                            border(205);
                            displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                            border(205);

                            // Display: Notifications
                            displayNotifications(page);

                            // Notify user, Notification successfully deleted
                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                            cout << "\n";
                            displayCenteredLine_Colored(">> Message deleted SUCCESSFULLY!", YELLOW);
                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue... ", YELLOW);
                            getchar();
                        }

                        else {
                            clearScreen();

                            // Display: INBOX TITLE
                            border(205);
                            displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                            border(205);

                            // Display: Notifications
                            displayNotifications(page);

                            // Notify user, Notification successfully deleted
                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                            cout << "\n";
                            displayCenteredLine_Colored(">> Message not found...         ", YELLOW);
                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue... ", YELLOW);

                            getchar();
                        }
                    }
                }
                break;

            case 4:
                if (NotificationList.size() == 0) break;
                while (true)
                {
                    clearScreen();

                    // Display: INBOX TITLE
                    border(205);
                    displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                    border(205);

                    // Display: Notifications
                    displayNotifications(page);

                    // Display: Options
                    displayCenteredLine_Colored("OPTIONS: CLEAR", BOLDWHITE);
                    cout << "\n";
                    displayCenteredLine_Colored(">> Do you want to CLEAR your inbox?", YELLOW);
                    cout << "\n\n";
                    displayCenteredLine_NoColor("[ Y ]  YES        ");
                    displayCenteredLine_NoColor("[ N ]  NO. Return ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter valid input: ", CYAN);

                    // Get input from user
                    getline(cin, input_Str);

                    // Return to INBOX Menu
                    if ((input_Str == "N") || (input_Str == "n")) break;

                    //
                    else if ((input_Str == "Y") || (input_Str == "y")) {
                        // Perform CLEAR
                        clearInbox();
                        clearScreen();

                        // Display: INBOX TITLE
                        border(205);
                        displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                        border(205);

                        // Display: Notifications
                        displayNotifications(page);

                        // Notify user, Notification successfully deleted
                        displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Inbox cleared SUCCESSFULLY!  ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue... ", YELLOW);
                        getchar();
                        break;
                    }

                }
                break;

            default:
                break;

        }
    }
}
















/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Notification function member that creates a custom notification, is stored unto file, and returns bool value
bool Notification:: createNotification(int mode, string customNotif)
{
    string current_Time = __TIME__;

    // Create new notification based on mode
    string NewNotif = BOLDWHITE + getDate_Today() + "; " + current_Time + RESET + "\n";
    switch (mode) {
        case 1:
            NewNotif += reminder;
            break;
        case 2:
            NewNotif += ExpenseLimReport_Neg;
            break;
        case 3:
            NewNotif += savingsReport;
            break;            
        case 4:
            NewNotif += ExpenseLimReport_Pos;
            break;
        case 5:
            NewNotif += loginNotif;
            break;
        
        case 6:
            NewNotif += customNotif;
            break;

        default:
            break;
    }


    // Write new notification in Inbox.bin file
    ofstream outFILE(InboxFILE, ios::binary | ios::app);
    if (outFILE.is_open()) {
        size_t notifSize = NewNotif.size();
        outFILE.write(reinterpret_cast<char*>(&notifSize), sizeof(notifSize));
        outFILE.write(NewNotif.c_str(), notifSize);

        //Closing file
        outFILE.close();
        return true;
    }

    return false;
}













/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// BitBudget function to print BitBudget main menu
void print_BitBudgetMM(bool& alert_Notif, bool& alert_LogIn)
{
    string presentDate = __DATE__;

    /* ------- TITLE + Present Date ------- */
    // Display TITLE + Present Date
    border(205);
    displayCenteredLine_Colored("BITBUDGET: EXPENSE TRACKER", BOLDGREEN);
    border(205);
    cout << BOLDDARKGREEN << "Today is: " << presentDate << RESET << endl;
    border(196);


    /* -------- NOTIFICATION AREA --------- */
    // Greet the user if newly logged in
    if (alert_LogIn) {
        cout << YELLOW << ">> " << BOLDYELLOW << "WELCOME! " << RESET << YELLOW << "Nice to have you here! (UwU)\n" << RESET << endl;
        alert_LogIn = false;
    }
    else {
        cout << YELLOW << ">> Great day today, isn't it?\n" << RESET << endl;
    }
    
    // Alert new user in Main Menu for new notification
    if (alert_Notif) {
        cout << YELLOW << ">> A " << BOLDYELLOW << "NEW NOTIFICATION" << RESET << YELLOW << " just got in to your " << BOLDYELLOW << "INBOX! "<< RESET << YELLOW << "Why not check it out? <(OvO)>\n" << RESET << endl;
        alert_Notif = false;
    }
    else {
        cout << YELLOW << ">> There is no new notification at the moment. (OwO)\n" << RESET << endl;
    }
    border(196);

    // Display short explanation about the features
    displayCenteredLine_Colored("ABOUT", BOLDWHITE);
    cout << BOLDWHITE << ">> UPDATE" << RESET << endl;
    cout << "\t- You can document your transactions." << endl;
    cout << "\t- Set LIMIT to your EXPENSES." << endl;
    cout << "\t- Set SAVINGS GOAL.\n" << endl;

    cout << BOLDWHITE << ">> INBOX" << RESET << endl;
    cout << "\t- Check out new messages BITBUDGET has in store for you\n\n" << endl;

    cout << BOLDWHITE << ">> DATA & HISTORY" << RESET << endl;
    cout << "\t- You can look through your old transactions." << endl;
    cout << "\t- You can take a look at your progress\n" << endl;

    border(205);
    displayCenteredLine_Colored("OPTIONS\n", BOLDWHITE);
    displayCenteredLine_Colored("[ 1 ]  UPDATE         [ 3 ]  DATA & HISTORY", WHITE);
    displayCenteredLine_Colored("[ 2 ]  INBOX          [ E ]  EXIT         \n\n", WHITE);
    displayCenteredLine_NoNewLine(">> Enter choice:  ", CYAN);
}

// BitBudget function to print menu for when user wishes to quit the program
void print_QuitMenu()
{
        clearScreen();

        // Print program TITLE
        border(205);
        displayCenteredLine_Colored("BITBUDGET: EXPENSE TRACKER", BOLDGREEN);
        border(205);

        // Ask user to CONFIRM
        displayCenteredLine_Colored(">> Are you sure you wanna leave? (T-T)", YELLOW);
        cout << "\n\n";

        // Print OPTIONS and PROMPT user to choose option
        border(45);
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        displayCenteredLine_NoColor("[ Y ]  YES             [ N ]  CANCEL");
        displayCenteredLine_NoNewLine(">> Enter choice:  ", CYAN);
}









/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main() {
    Notification MM_NotifsHandler;
    Inbox inboxHandler;
    Budget budgetHandler;

    bool alert_NewNotif = true;
    bool alert_NewLogIn = MM_NotifsHandler.createNotification(5, "");

    string choice_Str;
    string quitConfirmation;

    int choice_Int;


    while (true)
    {
        /* Display main menu and ask user which feature to run */
        do {
            clearScreen();

            // DISPLAY Main Menu
            print_BitBudgetMM(alert_NewNotif, alert_NewLogIn);
            getline(cin, choice_Str);

            // User chooses to EXIT PROGRAM
            if ((choice_Str == "E") || (choice_Str == "e"))
            {
                // Print Quit Menu and ask User for confirmation
                do {
                    clearScreen();

                    // ASK User for confirmation
                    print_QuitMenu();
                    getline(cin, quitConfirmation);
                } while ((quitConfirmation != "Y") && (quitConfirmation != "y") && (quitConfirmation != "N") && (quitConfirmation != "n"));

                // END program if User confirms to quit
                if ((quitConfirmation == "Y") || (quitConfirmation == "y")) {
                    return 1;
                }
            }
        } while ((choice_Str != "1") && (choice_Str != "2") && (choice_Str != "3"));


        // Convert choice_Str to Int
        choice_Int = stoi(choice_Str);

        
        // Run chosen feature
        switch (choice_Int) {
            case 1:
                /* UPDATE FEATURE */
                budgetHandler.run_BBUpdate(alert_NewNotif);
                break;
            
            case 2:
                /* INBOX FEATURE */
                inboxHandler.run_Inbox();
                break;
            
            case 3:
                /* DATA & HISTORY FEATURE */
                break;
            
            default:
                break;
        }
    }

    return 0;
}