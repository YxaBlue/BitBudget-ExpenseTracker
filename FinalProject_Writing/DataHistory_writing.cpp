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
string getDate(int);
string getCurrentTime();

tm parseDate(const string&);
tm stringToTime(const string*);

bool isDateInRange(const string&, const string&, const string&);
bool isTimeNow(const string&);
bool isCurrentTimeInRange(const string&, const string&);


























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








/*-----------------------------------------------------------------------------------------*/
/*--------------------------------- DataAndHistory CLASS ----------------------------------*/
/*-----------------------------------------------------------------------------------------*/

class DATAandHISTORY {
private:
    double totalBudget;
    double totalExpenses;
    double totalAllowance;

    vector<Expense> expensesList;
    vector<Allowance> allowancesList;

    vector<SavingsAndExpenseLim> savingsList;
    vector<SavingsAndExpenseLim> expenseLimitsList;

    vector<Category> CategoryList;
    vector<string> AccountList;




protected:
    // DATA GETTERS
    auto getAllowanceData_ByDate(string);
    auto getExpenseData_ByDate(string);
    auto getAllowanceData_ByAcc(string);
    auto getExpenseData_ByAcc(string);
    auto getExpenseData_ByCat(string);


    // PROTECTED CALCULATORS
    void calculateTotalExpenses();
    void calculateTotalAllowance();
    double calculateTotal_GivenExpenses(vector<Expense>);
    double calculateTotal_GivenAllowance(vector<Allowance>);


    // LOAD/SAVE DATA
    void loadExpenses();
    void loadAllowances();
    void loadCategoryList();
    void loadAccountList();




    // DISPLAY DATA [ALLOWANCES / EXPENSES / CATEGORIES / ETC.]
    void displayAllowancesData(vector<Allowance>, int);
    void displayExpensesData(vector<Expense>, int);
    void displayCategoryList_parent();
    void displayCategoryList_bbys(int);
    void displayAccountList();



    // DATA AND HISTORY FEATURE: DATA
    void run_YesterdayVSToday();



    // DATA AND HISTORY FEATURE: DATA
    void run_AllowanceHistory();
    void run_ExpenseHistory();
    void run_FilterByCategory();
    void run_FilterByDateRange();
    void run_FilterByDate();






public:
    // CONSTRUCTORS
    DATAandHISTORY();

    // PUBLIC CALCULATORS
    void calculateTotalBudget();

    // DATA AND HISTORY: Run DATA AND HISTORY FEATURE
    void run_DATA();
    void run_HISTORY();
    void runBB_DataAndHistory();

    // LOAD/SAVE ALL DATA
    void loadData();
};





/*-----------------------------------------------------------------------------------------*/
/*------------------------ DATAandHISTORY CLASS: FUNCTION MEMBERS -------------------------*/
/*-----------------------------------------------------------------------------------------*/

// DATAandHISTORY constructor
DATAandHISTORY :: DATAandHISTORY() :
    totalBudget(0), totalAllowance(0), totalExpenses(0)
    {}







/*--------------------------------------------*/
/*    DATAandHISTORY : Load data from file    */
/*--------------------------------------------*/
// DATAandHISTORY function member to load all expenses data
void DATAandHISTORY :: loadExpenses()
{
    ifstream inFile(ExpensesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Expense expense("", "", 0, "", "", "", "");
            inFile >> expense;
            expensesList.push_back(expense);
        }

        inFile.close();
    }
}

// DATAandHISTORY function member to load all allowances data
void DATAandHISTORY :: loadAllowances()
{
    ifstream inFile(AllowancesFILE, ios::binary);
    if (inFile.is_open()) {
        while (inFile.peek() != EOF) {
            Allowance allowance("", "", 0.0, "", "");
            inFile >> allowance;

            allowancesList.push_back(allowance);
        }
        inFile.close();
    }
}

// DATAandHISTORY function member to load all of Category class
void DATAandHISTORY :: loadCategoryList()
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

// DATAandHISTORY function member to load of all accounts
void DATAandHISTORY :: loadAccountList()
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

// DATAandHISTORY function member to load all necessary data
void DATAandHISTORY :: loadData()
{
    loadExpenses();
    loadAllowances();
    loadCategoryList();
    loadAccountList();
}






/*--------------------------------------------*/
/*        DATAandHISTORY : Calculators        */
/*--------------------------------------------*/
// DATAandHISTORY function member to calculate total allowance, expenses, and budget
void DATAandHISTORY :: calculateTotalBudget()
{
    calculateTotalAllowance();
    calculateTotalExpenses();
    
    totalBudget = totalAllowance - totalExpenses;
}

// DATAandHISTORY function member to calculate overall expenses created
void DATAandHISTORY :: calculateTotalExpenses()
{
    totalExpenses = 0.00;
    for (const auto& expense : expensesList)
    {
        totalExpenses += expense.getAmount();
    }
}

// DATAandHISTORY function member to calculate overall allowances created
void DATAandHISTORY :: calculateTotalAllowance()
{
    totalAllowance = 0.00;
    for (const auto& allowance : allowancesList)
    {
        totalAllowance += allowance.getAmount();
    }    
}

// DATAandHISTORY function member to calculate and return the total expenses of a given vector
double DATAandHISTORY :: calculateTotal_GivenExpenses(vector<Expense> expenseData)
{
    double totalExpense = 0.00;
    for (const auto& expense : expenseData)
    {
            totalExpense += expense.getAmount();
    }
    return totalExpense;
}

// DATAandHISTORY function member to calculate and return the total allowances of a given vector
double DATAandHISTORY :: calculateTotal_GivenAllowance(vector<Allowance> allowanceData)
{
    double totalExpense = 0.00;
    for (const auto& allowance : allowanceData)
    {
            totalExpense += allowance.getAmount();
    }
    return totalExpense;
}






/*--------------------------------------------*/
/*        DATAandHISTORY : Calculators        */
/*--------------------------------------------*/
// DATAandHISTORY function member to get allowance data created at given date and return a vector(Allowance class)
auto DATAandHISTORY :: getAllowanceData_ByDate(string date)
{
    vector<Allowance> AllowanceData;
    for(const auto& allowance : allowancesList)
    {
        if (allowance.getDate() == date) {
            AllowanceData.push_back(allowance);
        }
    }
    return AllowanceData;
}

// DATAandHISTORY function member to get expense data created by account and return a vector(Allowance class)
auto DATAandHISTORY :: getAllowanceData_ByAcc(string account)
{
    vector<Allowance> AllowanceData;
    for(const auto& allowance : allowancesList)
    {
        if (allowance.getAccount() == account) {
            AllowanceData.push_back(allowance);
        }
    }
    return AllowanceData;
}

// DATAandHISTORY function member to get expense data created at given date and return a vector(Expense class)
auto DATAandHISTORY :: getExpenseData_ByDate(string date)
{
    vector<Expense> ExpenseData;
    for(const auto& expense : expensesList)
    {
        if (expense.getDate() == date) {
            ExpenseData.push_back(expense);
        }
    }
    return ExpenseData;
}

// DATAandHISTORY function member to get expense data created by account and return a vector(Expense class)
auto DATAandHISTORY :: getExpenseData_ByAcc(string account)
{
    vector<Expense> ExpenseData;
    for(const auto& expense : expensesList)
    {
        if (expense.getAccount() == account) {
            ExpenseData.push_back(expense);
        }
    }
    return ExpenseData;
}

// DATAandHISTORY function member to get expense data by category and return a vector(Expense class)
auto DATAandHISTORY :: getExpenseData_ByCat(string category)
{
    vector<Expense> ExpenseData;
    for(const auto& expense : expensesList)
    {
        if (expense.getCategory() == category) {
            ExpenseData.push_back(expense);
        }
    }
    return ExpenseData;
}





/*--------------------------------------------*/
/*       DATAandHISTORY : Display Data        */
/*--------------------------------------------*/
// DATAandHISTORY function member to display allowance data table by page
void DATAandHISTORY :: displayAllowancesData(vector<Allowance> allowanceData, int page)
{
    int i;
    char border = 179;
    int vSize = allowanceData.size();

    // Display: Headers
    cout << string(10, ' ') << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 60);


    // Display: Allowances data per page
    if (vSize > 0)
    {
        int dataPerPage = 10;
        int maxPages = vSize / dataPerPage;
        if ((vSize % dataPerPage) != 0) maxPages ++;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, vSize);
        int items = end - start + 1;

        for (; start < end; start++)
        {
            Allowance allowance = allowanceData[start];

            // Convert: Amount to string with 2 decimal places
            double amt_db = allowance.getAmount();
            stringstream stream;
            stream << fixed << setprecision(2) << amt_db;
            string amt_str = stream.str();

            cout << "\n" << string(10, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(start + 1), WHITE, 7);
            displayTxtByColumn(allowance.getDate(), WHITE, COLUMNWIDTH);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getAccount(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getDescription(), WHITE, 60);
        }

        // Display remaining empty slots
        if (items < dataPerPage) {
            int vacant = dataPerPage - items;

            for (i = 0; i <= vacant; i++) {
                cout << "\n" << string(10, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(++start), WHITE, 7);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("----- ----- -----", WHITE, 60);
            }
        }

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: " + to_string(page) + " out of " + to_string(maxPages), GRAY);
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 10; i++) {
            cout << "\n" << string(10, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i + 1), WHITE, 7);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----- ----- -----", WHITE, 60);
        }

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: 1 out of 1 ", GRAY);
    }
}

void DATAandHISTORY :: displayExpensesData(vector<Expense> expenseData, int page)
{
    int i;
    char border = 179;
    int vSize = expenseData.size();

    // Display: Headers
    cout << border;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("DATE", BOLDWHITE, COLUMNWIDTH-2);
    displayTxtByColumn_CENTERED("AMOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("SUBCATEGORY", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("ACCOUNT", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("DESCRIPTION", BOLDWHITE, 54);


    // Display: Expenses Data by page
    if (vSize > 0)
    {
        int dataPerPage = 5;
        int maxPages = vSize / dataPerPage;

        if ((vSize % dataPerPage) != 0) maxPages ++;
        if (page > maxPages) page = maxPages;
        if (page < 0) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, vSize);
        int items = end - start + 1;

        for (; start < end; start++)
        {
            Expense expense = expenseData[start];

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
        if (items < dataPerPage) {
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
        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: " + to_string(page) + " out of " + to_string(maxPages), GRAY);
    }
    else {
        // Display: Dummy data
        for (i = 0; i < 10; i++) {
            cout << "\n" << border;
            displayTxtByColumn_CENTERED(to_string(i+1), WHITE, 7);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH-2);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----- ----- -----", WHITE, 54);
        }

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: 1 out of 1 ", GRAY);
    }
}





/*----------------------------------------------*/
/*       DATAandHISTORY FEATURE: HISTORY        */
/*----------------------------------------------*/
void DATAandHISTORY :: run_AllowanceHistory()
{
    
}




















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
void clearScreen()
{
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
void clearFile(string FILENAME)
{
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
bool isNumeric(string str)
{
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            return false;
        }
    }

    return true;
}

// Function to check if string is a positive double value
bool isDouble(string str)
{
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
bool validateDateFormat(const string &date)
{
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

// Function to validate date if it is present or later
bool validateDate(const string &date)
{
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
bool validateSecondDate(const string &firstDate, const string &secondDate)
{
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
    string day = to_string(ltm->tm_mday);

    if (month.size() == 1) month = "0" + month;
    if (day.size() == 1) day = "0" + day;

    string dateToday = month + "/" + day + "/" + year;

    return dateToday; 
}

// Function to get date from inputted date to days from start
string getDate_FromDateInput(string startDate, int daysFromStart)
{
    tm start = parseDate(startDate);
    time_t start_time = mktime(&start);

    start_time += daysFromStart * 24 * 60 * 60;
    tm *newTime = localtime(&start_time);

    ostringstream oss;
    oss << put_time(newTime, "%m/%d/%Y");

    return oss.str();
}

// Function to parse date in MM/DD/YYYY format. Returns tm class
tm parseDate(const std::string& date)
{
    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%m/%d/%Y");
    return tm;
}

// Function to compare two dates
bool isDateInRange(const string& dateStr, const string& startStr, const string& endStr)
{
    tm date = parseDate(dateStr);
    tm startDate = parseDate(startStr);
    tm endDate = parseDate(endStr);

    time_t dateT = mktime(&date);
    time_t startT = mktime(&startDate);
    time_t endT = mktime(&endDate);

    return (dateT >= startT && dateT <= endT);
}

// Function to get current time (Format: HH:MM AM/PM)
string getCurrentTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    ostringstream oss;
    oss << put_time(ltm, "%I:%M %p");
    return oss.str();
}

// Function to check if current time matches given time
bool isTimeNow(const string& targetTime)
{
    string currTime = getCurrentTime();
    return currTime == targetTime;
}

// Function to convert time string (Format: HH:MM AM/PM) to tm structure
tm stringToTime(const string& timeStr)
{
    tm timeStruct = {};
    istringstream iss(timeStr);
    iss >> get_time(&timeStruct, "%I:%M %p");
    return timeStruct;
}

// Function to check if the current time is within a given time range
bool isCurrentTimeInRange(const string& startTime, const string& endTime)
{
    string currentTimeStr = getCurrentTime();
    tm currentTime = stringToTime(currentTimeStr);
    tm startTm = stringToTime(startTime);
    tm endTm = stringToTime(endTime);

    // Convert times to seconds from midnight for comparison
    auto toSeconds = [](const tm& t) {
        return t.tm_hour * 3600 + t.tm_min * 60;
    };

    int currentSeconds = toSeconds(currentTime);
    int startSeconds = toSeconds(startTm);
    int endSeconds = toSeconds(endTm);

    // Check if current time is within range
    if (startSeconds < endSeconds) {
        return currentSeconds >= startSeconds && currentSeconds <= endSeconds;
    } else {
        // For ranges that span midnight
        return currentSeconds >= startSeconds || currentSeconds <= endSeconds;
    }
}






/* ------------------- JURO'S FUNCTIONS ------------------- */

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

string getDate_Yesterday()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string((5 + ltm->tm_mday)-1);

    if (month.size() == 1) month = "0" + month;

    string dateToday = month + "/" + day + "/" + year;

    return dateToday; 
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


























/*-----------------------------------------------------------*/
/*                    UPDATE: MAIN FUNCTION                  */
/*-----------------------------------------------------------*/

int main() {

    return 0;
}