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

void border(char, int);
void displayCenteredLine_NoColor(const string&, int);
void displayCenteredLine_Colored(const string&, const string&, int);
void displayCenteredLine_NoNewLine(const string&, const string&, int);

void displayTxtByColumn(const string &, const string &, int);
void displayTxtByColumn_CENTERED(const string &, const string &, int);
void displayTxtByColumn_NB(const string &, const string &, int);

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
string getCurrentTime();

tm parseDate(const string&);
tm stringToTime(const string*);

bool isDateInRange(const string&, const string&, const string&);
bool isTimeNow(const string&);
bool isCurrentTimeInRange(const string&, const string&);
string convertToComparableDate(const string&);

string makePercentBar(double, int);


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
    // PROTECTED CALCULATORS
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


    // DISPLAY DATA [ALLOWANCES / EXPENSES / CATEGORIES / ETC.]
    void displayAllowancesList_today(int);
    void displayExpensesList_today(int);
    void displaySavingsList();
    void displayExpenseLimitList();
    void displayCategoryList_parent();
    void displayCategoryList_bbys(int);
    void displayAccountList();
    void displayUpdateMenu();


    // UPDATE: LIMIT OF EXPENSES [Features]
    void displayMenu_UpdateLE();
    void run_LE_SetNewGoal();
    void run_LE_EditGoal();
    void run_LE_DeleteGoal();

    // UPDATE: SAVINGS [Features]
    void displayMenu_UpdateSavings();
    void run_S_SetNewGoal();
    void run_S_EditGoal();
    void run_S_DeleteGoal();
    void run_SetAsideSavings();

    // UPDATE: ALLOWANCE [Features]
    void run_AddAllowance();
    void run_EditAllowance();

    // UPDATE: EXPENSES [Features]
    void displayMenu_UpdateExpense(int);
    void run_AddExpenses();
    void run_DeleteExpenses(); 
    void run_EditExpenses();

    // UPDATE: ALLOWANCE/EXPENSE [Menu to add Accounts, Category, Subcategory]
    void run_AddAccount(string);
    void run_AddCategory(string);
    void run_AddSubcategory(int, string);


    // UPDATE: ALL MAIN FEATURES OF UPDATE TO RUN IN MAIN FUNCTION
    void run_UpdateLimitExpenses();
    void run_UpdateSavings();
    void run_UpdateAllowance();
    void run_UpdateExpense();




public:
    // CONSTRUCTORS
    Budget();

    // PUBLIC CALCULATORS
    void calculateTotalBudget();
    void setTotalBudget(double);

    // UPDATE: Run UPDATE FEATURE
    void run_BBUpdate();

    // Savings and Expense limit TRACKER
    void trackExpenses();
    void trackDue_SavingsGoal(bool&);
    void trackDue_ExpensesLimit(bool&);
    void autoDelete_SavingsGoal(bool&);
    void autoDelete_ELGoal(bool&);

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
    void createNotif_SavingsGoal(SavingsAndExpenseLim&, int);
    void createNotif_ExpenseLim(SavingsAndExpenseLim&, int);
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







/*---------------------------------------------------------------------------------*/
/*---------------------------- DATA & HISTORY CLASS -------------------------------*/
/*---------------------------------------------------------------------------------*/

class DATAandHISTORY {
private:
    double totalBudget;
    double totalExpenses;
    double totalAllowance;

    vector<Expense> expensesList;
    vector<Allowance> allowancesList;

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


    // LIST SORTER
    void sortExpensesByDate_Ascending(vector<Expense>&);
    void sortExpensesByDate_Descending(vector<Expense>&);
    void sortAllowanceByDate_Ascending(vector<Allowance>&);
    void sortAllowanceByDate_Descending(vector<Allowance>&);


    // DISPLAY DATA [ALLOWANCES / EXPENSES / CATEGORIES / ETC.]
    void displayAllowancesData(vector<Allowance>, int, int);
    void displayExpensesData(vector<Expense>, int, int);
    void displayCategoryList_parent();
    void displayAccountList();



    // DATA AND HISTORY FEATURE
    void displayMenu_DataAndHistory();



    // TRANSACTION HISTORY FEATURE
    string run_GetAccount();
    string run_GetCategory();
    void runDH_TAHistory();


    // EXPENSE DATA FEATURE
    void displayTitle_DataAndHistory(string);
    void displayTable_YESTERDAYvsTODAY(vector<Expense>, vector<Expense>, int);
    void displayTable_RankByCategory();
    void runDH_ExpenseData();




public:
    // CONSTRUCTORS
    DATAandHISTORY();

    // PUBLIC CALCULATORS
    void calculateTotalBudget();

    // DATA AND HISTORY: Run DATA AND HISTORY FEATURE
    void runBB_DataAndHistory();

    // LOAD/SAVE ALL DATA
    void loadData();
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
bool validateDateFormat(const string &date) {
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

// Helper function to convert MM/DD/YYYY to YYYYMMDD for easy comparison
string convertToComparableDate(const string& date)
{
    string month = date.substr(0, 2);
    string day = date.substr(3, 2);
    string year = date.substr(6, 4);
    return year + month + day;
}

// Function to create a percent bar
string makePercentBar(double percentage, int barWidth = 50)
{
    // Clamp the percentage between 0 and 100
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    // Create bar
    int filledWidth = static_cast<int>(barWidth * (percentage / 100.0f));
    string bar(barWidth, '-');
    for(int i = 0; i < filledWidth; ++i) bar[i] = '#';

    // Convert double to string, displaying 2 decimal places
    stringstream ss;
    ss << fixed << setprecision(0) << percentage;

    // Create string
    string percentBar = "[" + bar + "]    " + ss.str() + "%";

    return percentBar;
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
    allowance.dateCreated.resize(dateCreatedSize);
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
    if (totalBudget < 0) totalBudget = 0;
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
            else {
                allowancesList.push_back(allowance);
            }
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
Budget :: Budget() :
            totalBudget(0.0),
            totalSavings(0.0) {}






/*--------------------------------------------------------------------------------------*/
/*                        BUDGET class MFs(Protected): DISPLAY LISTS                    */
/*--------------------------------------------------------------------------------------*/
// Budget function to display allowances created at present day
void Budget :: displayAllowancesList_today(int page = 1)
{
    int i;
    char border = 179;

    // Display: Headers
    cout << string(10, ' ') << border;
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
        int maxPages = (AllowanceSize + dataPerPage - 1) / dataPerPage;

        if (page > maxPages) page = maxPages;
        if (page < 1) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, AllowanceSize);

        int items = end - start;

        for (int idx = start; idx < end; idx++)
        {
            Allowance allowance = allowancesList_Today[start];

            // Convert: Amount to string with 2 decimal places
            double amt_db = allowance.getAmount();
            stringstream stream;
            stream << fixed << setprecision(2) << amt_db;
            string amt_str = stream.str();

            cout << "\n" << string(10, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(idx + 1), WHITE, 7);
            displayTxtByColumn(allowance.getDate(), WHITE, COLUMNWIDTH);
            displayTxtByColumn("P " + amt_str, WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getAccount(), WHITE, COLUMNWIDTH+2);
            displayTxtByColumn(allowance.getDescription(), WHITE, 60);
        }

        // Display remaining empty slots
        if (items < dataPerPage) {
            int vacant = dataPerPage - items;

            for (i = 0; i < vacant; i++) {
                cout << "\n" << string(10, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(end + i + 1), WHITE, 7);
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
        for (i = 0; i < 5; i++) {
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
        int maxPages = (ExpensesSize + dataPerPage - 1) / dataPerPage;

        if (page > maxPages) page = maxPages;
        if (page < 1) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, ExpensesSize);

        int items = end - start;

        for (int idx = start; idx < end; idx++)
        {
            Expense expense = expensesList_Today[start];

            // Convert: Amount to string with 2 decimal places
            double amt_db = expense.getAmount();
            stringstream stream;
            stream << fixed << setprecision(2) << amt_db;
            string amt_str = stream.str();

            cout << "\n" << border;
            displayTxtByColumn_CENTERED(to_string(idx+1), WHITE, 7);
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

            for (i = 0; i < vacant; i++) {
                cout << "\n" << border;
                displayTxtByColumn_CENTERED(to_string(end + i + 1), WHITE, 7);
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

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: 1 out of 1 ", GRAY);
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

        string dayAfterDue = getDate_FromDateInput(savingsData.get_dueDate(), 1);
        string dateToday = getDate_Today();

        cout << "\n" << string(3, ' ') << border;
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
        displayTxtByColumn(savingsData.get_startDate() + " - " + savingsData.get_dueDate(), WHITE, 25);
        displayTxtByColumn( "P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(savingsData.get_desc(), WHITE, 60);

        if (dateToday == dayAfterDue)
            displayTxtByColumn_CENTERED( "COMPLETED!", MAGENTA, COLUMNWIDTH*2);
        else
            displayTxtByColumn( "P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
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

        string dateToday = getDate_Today();
        string dayAfterDue = getDate_FromDateInput(expenseLimData.get_dueDate(), 1);

        cout << "\n" << string(3, ' ') << border;
        displayTxtByColumn_CENTERED(to_string(i++), WHITE, 7);
        displayTxtByColumn(expenseLimData.get_startDate() + " - " + expenseLimData.get_dueDate(), WHITE, 25);
        displayTxtByColumn("P " + amt_str1, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn(expenseLimData.get_desc(), WHITE, 60);

        if (dateToday == dayAfterDue)
            displayTxtByColumn_CENTERED("COMPLETED!", MAGENTA, COLUMNWIDTH*2);
        else
            displayTxtByColumn("P " + amt_str2 + "  /  " + "P " + amt_str1, MAGENTA, COLUMNWIDTH*2);
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
    vector<string> ParentCategories;
    
    // Load all parent categories
    for (const auto& catHol : CategoryList) {
        ParentCategories.push_back(catHol.getParent());
    }

    // Fill in vacant slots
    while (ParentCategories.size() <= 10) ParentCategories.push_back("----------");

    // Display ParentCategories by 2 columns
    for (int i = 0; i < ParentCategories.size() / 2; i++) {
        cout << string(50, ' ');
        displayTxtByColumn("[ " + to_string(i+1) + " ] " + ParentCategories[i], WHITE, 25);
        displayTxtByColumn("[ " + to_string(i+6) + " ] " + ParentCategories[i + 5], WHITE, 25);
        cout << "\n";
    }
}

// Budget function to display subcategories of a category (index_Parent must be +1)
void Budget :: displayCategoryList_bbys(int index_Parent)
{
    if ((index_Parent <= 0) || (index_Parent > CategoryList.size())) return;

    vector<string> BbyCategories = CategoryList[index_Parent - 1].getBabies();

    // Fill in vacant slots
    while (BbyCategories.size() <= 10) BbyCategories.push_back("----------");

    // Display BbyCategories by 2 columns
    for (int i = 0; i < BbyCategories.size() / 2; i++) {
        cout << string(50, ' ');
        displayTxtByColumn_NB("[ " + to_string(i+1) + " ] " + BbyCategories[i], WHITE, 25);
        displayTxtByColumn_NB("[ " + to_string(i+6) + " ] " + BbyCategories[i + 5], WHITE, 25);
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
        cout << string(50, ' ');
        displayTxtByColumn_NB("[ " + to_string(i+1) + " ] " + Accounts[i], WHITE, 25);
        displayTxtByColumn_NB("[ " + to_string(i+6) + " ] " + Accounts[i + 5], WHITE, 25);
        cout << "\n";
    }
}









/*------------------------------------------------------------------------*/
/*    BUDGET class MFs(Protected): SETTERS, ADDERS, REMOVERS, EDITORS     */
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





/*-----------------------------------------------------------------------------------------------------*/
/*    BUDGET class MFs(Protected): UPDATE ALLOWANCE/EXPENSE [Add Accounts, Category, Subcategory]      */
/*-----------------------------------------------------------------------------------------------------*/

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

        // Warn and restrict user from adding new account
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










/* -------------------------------------------------------------------------- */ 
/*                          UPDATE: UPDATE MAIN MENU                          */ 
/* -------------------------------------------------------------------------- */ 
// Budget function member to display main menu of UPDATE feature
void Budget :: displayUpdateMenu()
{
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
// Budget function member to display Update LIMIT OF EXPENSES feature's main menu
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

// Budget function member to run Update LIMIT OF EXPENSES main menu
void Budget :: run_UpdateLimitExpenses()
{
    trackExpenses();
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

// Budget function member to run Update LIMIT OF EXPENSES special feature: Set new goal (4 maximum goals)
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
                    SavingsAndExpenseLim goal(date1, date2, newGoal, 0.0, description);
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

// Budget function member to run Update LIMIT OF EXPENSES special feature: Delete goal and return savings back to allowance
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

// Budget function member to run Update LIMIT OF EXPENSES special feature: Edit details of the goal (restricted when past due date)
void Budget :: run_LE_EditGoal()
{
    string input_str;
    int input_int;


    // Expense Limit Edit Goal MENU 1: Ask user for index of savings goal to edit
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

        if ((input_str == "R") || (input_str == "r")) {
            // End function
            return;
        }

        else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
            input_int = stoi(input_str);

            // Proceed to next menu
            if ((input_int > 0) && (input_int <= expenseLimitsList.size()))
            break;
        }
    }


    // Get goal and create reference
    SavingsAndExpenseLim& goal = expenseLimitsList[input_int - 1];


    // Notify and restrict user from editing the goal if Expense limit Goal is past due date
    string dateToday = getDate_Today();
    if (!isDateInRange(dateToday, goal.get_startDate(), goal.get_dueDate())) {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
        displayCenteredLine_Colored(">> Goal is already past due! You cannot edit this!           ", YELLOW);
        displayCenteredLine_Colored(">> This goal will deleted automatically by afternoon/evening.", YELLOW);
        displayCenteredLine_Colored(">> You can also manually delete the goal if you wish!        ", YELLOW);
        displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
        getchar();
        return;
    }


    // Expense Limit Goal MENU 2: Ask what Expense limit data to edit
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

        if ((input_str == "R") || (input_str == "r")) {
            // End function
            return;
        }
        
        if ((input_str == "1") || (input_str == "2") || (input_str == "3")) {
            // proceed to next menu
            input_int = stoi(input_str);
            break;
        }
    }




    // Temporary placeholders for data to be edited
    string newDate1 = goal.get_startDate();
    string newDate2 = goal.get_dueDate();





    // Expense Limit Goal MENU 3: Editing menu
    int inputflow = 1;
    switch (input_int)
    {
        case 1:


            while (true)
            {
                clearScreen();

                // Display: UPDATE(Limit Of Expenses) title
                border(205);
                displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                border(205);

                // Display original data
                cout << BOLDWHITE << "  >> EDITING START & DUE DATE:" << RESET << endl;
                cout << "\n";
                cout << string(5, ' ') << "* Start Date:         " << YELLOW <<  newDate1 << RESET << endl;
                cout << string(5, ' ') << "* Due Date:           " << YELLOW <<  newDate2 << RESET << endl;
                cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
                cout << string(5, ' ') << "* Description:        " << BLUE <<  goal.get_desc() << RESET << endl;
                cout << "\n";
                cout << "\n";
                border(196);

                switch (inputflow)
                {
                    case 1: // Input START DATE
                        displayCenteredLine_NoNewLine("Enter Start Date(MM/DD/YYYY): ", CYAN);
                        getline(cin , input_str);

                        if ((input_str == "R") || (input_str == "r")) {
                            // End function
                            return;
                        }

                        else if (validateDate(input_str)) {
                            // Set new start date
                            newDate1 = input_str;
                            inputflow++;
                        }
                        break;


                    case 2: // Input DUE DATE
                        displayCenteredLine_NoNewLine("Enter Due Date(MM/DD/YYYY): ", CYAN);
                        getline(cin , input_str);

                        if ((input_str == "R") || (input_str == "r")) {
                            return;
                        }

                        else if (validateSecondDate(goal.get_startDate(), input_str)) {
                            newDate2 = input_str;
                            inputflow++;
                        }
                        break;


                    default: // Notify user EDIT SUCCESSFUL
                        goal.set_startDate(newDate1);
                        goal.set_dueDate(newDate2);
                        
                        displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                        getchar();
                        return;
                        break;
                }
            }
            break;




        case 2: // EDIT GOAL AMOUNT
            while (true) {
                clearScreen();

                // Display: UPDATE(Limit Of Expenses) title
                border(205);
                displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                border(205);

                // Display original data
                cout << BOLDWHITE << "  >> EDITING GOAL AMOUNT:" << RESET << endl;
                cout << string(5, ' ') << "* Start Date:         " << BLUE <<  goal.get_startDate() << RESET << endl;
                cout << string(5, ' ') << "* Due Date:           " << BLUE <<  goal.get_dueDate() << RESET << endl;
                cout << string(5, ' ') << "* Goal Amount:        " << YELLOW <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
                cout << string(5, ' ') << "* Description:        " << BLUE <<  goal.get_desc() << RESET << endl;
                cout << "\n";
                cout << "\n";
                border(196);

                

                switch (inputflow)
                {
                    case 1:
                        displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                        getline(cin, input_str);

                        if ((input_str == "R") || (input_str == "r")) {
                            return;
                        }

                        else if ((isDouble(input_str)) && (input_str.size() > 0)) {
                            double newGoal = stod(input_str);

                            if (newGoal > 0) {
                                goal.set_goal(newGoal);
                                inputflow++;
                            }
                        }
                        break;


                    default:
                        displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                        getchar();
                        return;
                        break;
                }
            }
            break;






        case 3: // EDIT DESCRIPTION
            while (true) {
                clearScreen();

                // Display: UPDATE(Limit Of Expenses) title
                border(205);
                displayCenteredLine_Colored("UPDATE: LIMIT OF EXPENSES (EDIT GOAL)", BLUE);
                border(205);

                // Display original data
                cout << BOLDWHITE << "  >> EDITING DESCRIPTION:" << RESET << endl;
                cout << string(5, ' ') << "* Start Date:         " << BLUE <<  goal.get_startDate() << RESET << endl;
                cout << string(5, ' ') << "* Due Date:           " << BLUE <<  goal.get_dueDate() << RESET << endl;
                cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
                cout << string(5, ' ') << "* Description:        " << YELLOW <<  goal.get_desc() << RESET << endl;
                cout << "\n";
                cout << "\n";
                border(196);

                switch (inputflow)
                {
                    case 1:
                        displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                        getline(cin, input_str);

                        if ((input_str == "R") || (input_str == "r")) {
                            return;
                        }

                        else if (input_str.size() <= 50) {
                            goal.set_desc(input_str);
                            inputflow++;
                        }
                        break;


                    default:
                        displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Goal edited successfully!                            ", YELLOW);
                        displayCenteredLine_NoNewLine(">> Return to Limit of Expenses Menu... (Press 'ENTER')  ", WHITE);
                        getchar();
                        return;
                        break;
                }
            }
            break;
        




        default:
            // do nothing lolll
            break;
    
    }
}



/*---------------------------------------------------------------------------*/
/*                             UPDATE: SAVINGS                               */
/*---------------------------------------------------------------------------*/
// Budget function member to display Update SAVINGS main menu
void Budget :: displayMenu_UpdateSavings()
{
    clearScreen();

    // Display: UPDATE(Savings) title
    border(205);
    displayCenteredLine_Colored("UPDATE: SAVINGS", BLUE);
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

// Budget function member to run Update SAVINGS feature
void Budget :: run_UpdateSavings()
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

// Budget function member to run Update SAVINGS special feature: Set New Goal (4 maximum goals)
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
                    SavingsAndExpenseLim goal(date1, date2, newGoal, 0.0, description);
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

// Budget function member to run Update SAVINGS special feature: Edit details of the goal (restricted when past due date)
void Budget :: run_S_EditGoal()
{
    string input_str;
    int input_int;

    // Savings Edit Goal MENU 1: Ask user for index of savings goal to edit
    while (true) {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter any index number to EDIT");
        displayCenteredLine_NoColor(">> Enter 'R' to return           ");
        cout << "\n";
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);

        // Ask user for input
        getline(cin, input_str);

        if ((input_str == "R") || (input_str == "r")) {
            // End function
            return;
        }

        else if ((isNumeric(input_str)) && (input_str.size() > 0))
        {
            input_int = stoi(input_str);

            // Proceed to next Edit Savings Goal Menu
            if ((input_int > 0) && (input_int <= savingsList.size()))
                break;
        }
    }


    // Get and Make REFERENCE for Savings Goal
    SavingsAndExpenseLim& goal = savingsList[input_int - 1];


    // Notify and restrict user from editing the goal if Savings Goal is past due date
    string dateToday = getDate_Today();
    if (!isDateInRange(dateToday, goal.get_startDate(), goal.get_dueDate())) {
        displayMenu_UpdateSavings();

        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
        displayCenteredLine_Colored(">> Goal is already past due! You cannot edit this!           ", YELLOW);
        displayCenteredLine_Colored(">> This goal will deleted automatically by afternoon/evening.", YELLOW);
        displayCenteredLine_Colored(">> You can also manually delete the goal if you wish!        ", YELLOW);
        displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
        getchar();
        return;
    }


    // Savings Edit Goal MENU 2: Ask user what data of the goal to edit
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

        // End Edit Goal function
        if ((input_str == "R") || (input_str == "r")) {
            return;
        }

        // Proceed to next menu
        else if (((input_str == "1") || (input_str == "2") || (input_str == "3")) && (input_str.size() > 0)) {
            input_int = stoi(input_str);
            break;
        }
    }




    // Temporary placeholder for data to be edited
    string newDate1 = goal.get_startDate();
    string newDate2 = goal.get_dueDate();



    // Savings Edit Goal MENU 3: Editing data Menu
    int inputflow = 1;
    switch (input_int)
    {
        case 1: // EDIT DATE RANGE
        while (true)
        {
            clearScreen();

            // Display: UPDATE(Savings) title
            border(205);
            displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
            border(205);

            // Display original data
            cout << BOLDWHITE << "  >> EDITING START & DUE DATE:" << RESET << endl;
            cout << "\n";
            cout << string(5, ' ') << "* Start Date:         " << YELLOW <<  newDate1 << RESET << endl;
            cout << string(5, ' ') << "* Due Date:           " << YELLOW <<  newDate2 << RESET << endl;
            cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
            cout << string(5, ' ') << "* Description:        " << BLUE <<  goal.get_desc() << RESET << endl;
            cout << "\n";
            cout << "\n";
            border(196);

            switch (inputflow)
            {
                case 1: // Input START DATE
                    displayCenteredLine_NoNewLine("Enter Start Date(MM/DD/YYYY): ", CYAN);
                    getline(cin , input_str);

                    // End function
                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }

                    // If input is valid, set new START DATE
                    else if (validateDate(input_str)) {
                        newDate1 = input_str;
                        inputflow++;
                    }
                    break;


                case 2: // Input DUE DATE
                    displayCenteredLine_NoNewLine("Enter Due Date(MM/DD/YYYY): ", CYAN);
                    getline(cin , input_str);

                    // End function
                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }

                    // If input is valid, set new START DATE
                    else if (validateSecondDate(newDate1, input_str)) {
                        newDate2 = input_str;
                        inputflow++;
                    }
                    break;


                default: // Notify user EDIT SUCCESSFUL
                    goal.set_startDate(newDate1);
                    goal.set_dueDate(newDate2);

                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                    getchar();
                    return;
                    break;
            }
        }
        break;







        case 2: // EDIT AMOUNT GOAL
        while (true)
        {
            clearScreen();

            // Display: UPDATE(Savings) title
            border(205);
            displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
            border(205);

            // Display original data
            cout << BOLDWHITE << "  >> EDITING GOAL AMOUNT:" << RESET << endl;
            cout << string(5, ' ') << "* Start Date:         " << BLUE <<  goal.get_startDate() << RESET << endl;
            cout << string(5, ' ') << "* Due Date:           " << BLUE <<  goal.get_dueDate() << RESET << endl;
            cout << string(5, ' ') << "* Goal Amount:        " << YELLOW <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
            cout << string(5, ' ') << "* Description:        " << BLUE <<  goal.get_desc() << RESET << endl;
            cout << "\n";
            cout << "\n";
            border(196);

            
            switch (inputflow)
            {
                case 1: // Input NEW GOAL AMOUNT
                    displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }

                    else if ((isDouble(input_str)) && (input_str.size() > 0)) {
                        double newGoal = stod(input_str);

                        // If new goal is the same or greater than original goal
                        if (newGoal >= goal.get_goal()) {
                            goal.set_goal(newGoal);
                            inputflow++;
                        }
                        // If new goal is less than original goal and current savings, return excess amount saved to budget
                        else if ((newGoal > 0) && (newGoal < goal.get_currentAmt())) {
                            double excess = goal.get_currentAmt() - newGoal;
                            double newSavingsAmt = goal.get_currentAmt() - excess;

                            goal.set_currentAmt(newSavingsAmt);
                            goal.set_goal(newGoal);
                            inputflow++;
                        }
                    }
                    break;


                default: // Notify user EDIT SUCCESSFUL
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                    getchar();
                    return;
                    break;
            }
        }
        break;






        case 3: // EDIT DESCRIPTION
        while (true)
        {
            clearScreen();

            // Display: UPDATE(Savings) title
            border(205);
            displayCenteredLine_Colored("UPDATE: SAVINGS (EDIT GOAL)", BLUE);
            border(205);

            // Display original data
            cout << BOLDWHITE << "  >> EDITING DESCRIPTION:" << RESET << endl;
            cout << string(5, ' ') << "* Start Date:         " << BLUE <<  goal.get_startDate() << RESET << endl;
            cout << string(5, ' ') << "* Due Date:           " << BLUE <<  goal.get_dueDate() << RESET << endl;
            cout << string(5, ' ') << "* Goal Amount:        " << BLUE <<  fixed << setprecision(2) << goal.get_goal() << RESET << endl;
            cout << string(5, ' ') << "* Description:        " << YELLOW <<  goal.get_desc() << RESET << endl;
            cout << "\n";
            cout << "\n";
            border(196);

            switch (inputflow)
            {
                case 1: // Input NEW DESCRIPTION
                    displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }

                    else if (input_str.size() <= 50) {
                        goal.set_desc(input_str);
                        inputflow++;
                    }
                    break;


                default: // Notify user EDIT SUCCESSFUL
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Goal edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Return to Savings Menu... (Press 'ENTER')  ", WHITE);
                    getchar();
                    return;
                    break;
            }
            break;
        }
    }

    calculateTotalBudget();
}

// Budget function member to run Update SAVINGS special feature: Delete Goal and returns savings back to allowance
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

// Budget function member to run Update SAVINGS special feature: Set aside savings by taking away allowance
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




/*------------------------------------------------------------------------------------------*/
/*                                   UPDATE: ALLOWANCE                                      */
/*------------------------------------------------------------------------------------------*/
// Budget function member to run Update ALLOWANCE feature
void Budget::run_UpdateAllowance()
{
    string input;
    int page = 1;
    int maxPages = allowancesList_Today.size() / 5;
    if ((allowancesList_Today.size() / 5) != 0) maxPages++;


    while (true) {
        clearScreen();
        // Display: UPDATE(Allowance) title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE", BLUE);
        border(205);

        // Display: Total Allowance
        cout << BOLDWHITE << "  >> Total Budget:       P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
        cout << BOLDWHITE << "  >> Total Savings:      P " << fixed << setprecision(2) << totalSavings << "\n" << RESET << endl;
        border(196);
        cout << BOLDWHITE << "  >> Allowance List (Created Today):\n" << RESET << endl;
        displayAllowancesList_today(page);
        border(196);

        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[ 1 ] Previous Page       [ 3 ] ADD NEW ALLOWANCE");
        displayCenteredLine_NoColor("[ 2 ] Next Page           [ 4 ] EDIT ALLOWANCE   ");
        cout << "\n";
        displayCenteredLine_NoColor("[ R ] Return");
        cout << "\n";

        // Ask user for input
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
        getline(cin, input);

        if (input == "1") {
            if (page > 1) page--;
        }
        else if (input == "2") {
            if (page < maxPages) page++;
        }
        else if (input == "3")          run_AddAllowance();
        else if (input == "4")          run_EditAllowance();
        else if ((input == "R") || (input == "r")) return;
    }
}

// Budget function member to run Update ALLOWANCE special feature: Add Allowance
void Budget::run_AddAllowance()
{
    string input_str;
    int input_int;
    double input_db;

    int inputflow = 1;

    // Input holders
    Allowance allowance(getDate_Today(), "MM/DD/YYYY", 0, "----------", "----------");

    while (true) {
        clearScreen();
        // Display: UPDATE(Allowance) title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE (ADD NEW ALLOWANCE)", BLUE);
        border(205);

        // Display: New Allowance details
        if (inputflow == 4) {
            // Display Account List
            cout << BOLDWHITE << "  >> ACCOUNT LIST: " << RESET << endl;
            displayAccountList();
            cout << "\n";
            displayCenteredLine_NoColor("[ A ] ADD New Account");
            cout << "\n";
            cout << BOLDWHITE << "  >> Enter the number of the Account you a want to input. Enter (A) if you want to add a new Account." << RESET << endl;
        }
        else {
            cout << BOLDWHITE << "  >> ADDING NEW ALLOWANCE:\n" << RESET << endl;
            cout << string(5, ' ') << "* Date:                  " << BLUE <<  allowance.getDate() << RESET << endl;
            cout << string(5, ' ') << "* Allowance Amount:      " << GREEN << "P " << fixed << setprecision(2) << allowance.getAmount() << RESET << endl;
            cout << string(5, ' ') << "* Account:               " << BLUE <<  allowance.getAccount() << RESET << endl;
            cout << string(5, ' ') << "* Description:           " << BLUE << allowance.getDescription() << RESET << endl;
            cout << "\n";
        }
        border(196);


        switch (inputflow) {
            case 1: // Input: Date
                displayCenteredLine_NoNewLine(">> Enter DATE (MM/DD/YYYY): ", CYAN);
                getline(cin, input_str);

                if (validateDateFormat(input_str)) {
                    allowance.setDate(input_str);
                    inputflow++;
                }
                else if (input_str == "R" || input_str == "r") return;
                else {
                    border(196);
                    displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Please enter a valid input.", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press enter to continue... ", WHITE);
                    getchar();
                }
                break;




            case 2: // Input: New Allowance Amount
                displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                getline(cin, input_str);

                if ((isDouble(input_str)) && (!input_str.empty())) {
                    input_db = stod(input_str);

                    if (input_db > 0) {
                        allowance.setAmount(input_db);
                        inputflow++;
                    } else {
                        border(196);
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored  (">> Please enter a valid input.", YELLOW);
                        displayCenteredLine_NoNewLine(">> Press enter to continue... ", WHITE);
                        getchar();
                    }
                }
                else if (input_str == "R" || input_str == "r") return;
                else {
                    border(196);
                    displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Please enter a valid input.", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press enter to continue... ", WHITE);
                    getchar();
                }
                break;



            case 3: // Input: Confirm allowance
                displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                cout << fixed << setprecision(2) << allowance.getAmount() << RESET << endl;
                border(196);
                displayCenteredLine_Colored  (">> Is this amount final? This cannot be changed if confirmed...", YELLOW);
                displayCenteredLine_NoNewLine(">> Enter (Y) to confirm, (ENTER) if else: ", CYAN);
                getline(cin, input_str);

                if ((input_str == "Y") || (input_str == "y")) {
                    // Proceed to input account
                    inputflow++;
                }
                else {
                    // Go back to input new amount
                    inputflow--;
                    allowance.setAmount(0);
                }
                break;




            case 4: // Input: Account
                displayCenteredLine_NoNewLine(">> Enter ACCOUNT(index num): ", CYAN);
                getline(cin , input_str);
                
                if ((isNumeric(input_str)) &&  (input_str.size() > 0)) {
                    input_int = stoi(input_str); 
                    if ((input_int > 0) && (input_int <= AccountList.size())) {
                        allowance.setAccount(AccountList[input_int-1]);
                        inputflow++;
                    }
                }

                else if ((input_str == "A") || (input_str == "a")) run_AddAccount("UPDATE: ALLOWANCE (ADD NEW ALLOWANCE)");
                else if ((input_str == "R") || (input_str == "r")) return;
                break;




            case 5: // Input: Description
                cout << CYAN << "\t>> Enter DESCRIPTION (max 50 chars): " << RESET;
                getline(cin, input_str);

                if (input_str.size() <= 50) {
                    allowance.setDescription(input_str);
                    inputflow++;
                }
                else if (input_str == "R" || input_str == "r") return;
                else {
                    border(196);
                    displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Please enter a description of up to 50 characters.", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press enter to continue...                        ", WHITE);
                    getchar();
                }
                break;



            default: // Add new allowance to list
                allowance.setDateCreated();
                addAllowance(allowance);
                calculateTotalBudget();

                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored(">> New allowance added successfully!", YELLOW);
                displayCenteredLine_NoNewLine(">> Press enter to continue...       ", WHITE);
                getchar();
                return;
        }
    }
}

/* 
    Budget function member to run UPDATE ALLOWANCE special feature: Edit Allowance data.
    Can only edit: Date, Account, and Description of Allowances created at present day
*/
void Budget::run_EditAllowance()
{
    int page = 1;
    int maxPages = expensesList_Today.size() / 5;
    if ((expensesList_Today.size() % 5) != 0) maxPages++;

    string input_str;
    int input_int;
    int inputFlow = 1;


    /* EDIT ALLOWANCE: Get Index of Allowance to edit (Menu 1) */
    while (true) {
        clearScreen();
        // Display: UPDATE(Allowance) title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT)", BLUE);
        border(205);

        // Display: Total Allowance
        cout << BOLDWHITE << "  >> Allowance List (Created Today):\n" << RESET << endl;
        displayAllowancesList_today(page);
        border(196);

        // Display: Options
        displayCenteredLine_Colored("OPTIONS (EDIT)", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[P] Previous Page                 [N] Next Page");
        cout << "\n";
        displayCenteredLine_NoColor(">> Enter index number of Allowance data to edit");
        cout << "\n";


        // Ask user for input
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
        getline(cin, input_str);


        // Display previous page
        if ((input_str == "P") || (input_str == "p")) {
            if (page > 1) page--;
        }

        // Display next page
        else if ((input_str == "N") || (input_str == "n")) {
            if (page < maxPages) page++;
        }

        // End function
        else if ((input_str == "R") || (input_str == "r")) {
            return;
        }

        // If input is valid, break and proceed to next menu
        else if ((isNumeric(input_str)) && (input_str.size() > 0))
        {
            input_int = stoi(input_str);
            if ((input_int > 0) && (input_int <= allowancesList_Today.size())) break;
        }
    }

    // Get expense and make reference
    Allowance& allowance = allowancesList_Today[input_int - 1];




    /* EDIT ALLOWANCE: Ask user what data to edit(Menu 2) */
    while (true)
    {
        clearScreen();

        // Display: UPDATE(Allowance) title
        border(205);
        displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT)", BLUE);
        border(205);

        // Display: Total Allowance
        cout << BOLDWHITE << "  >> Allowance List (Created Today):\n" << RESET << endl;
        displayAllowancesList_today(page);
        border(196);

        // Display: Options to edit
        displayCenteredLine_Colored("OPTIONS (EDIT)", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_NoColor("[ 1 ] DATE                      [ 3 ] DESCRIPTION");
        displayCenteredLine_NoColor("[ 2 ] ACCOUNT                   [ R ] RETURN     ");
        cout << "\n";


        // Get input from user
        displayCenteredLine_NoNewLine(">> Enter choice: ", CYAN);
        getline(cin, input_str);


        // End function if input is (R/r)
        if ((input_str == "R") || ((input_str == "r"))) {
            return;
        }
        
        else if ((isNumeric(input_str)) && (input_str.size() > 0))
        {
            input_int = stoi(input_str);

            // Edit if input is numeric and is in between range of choices
            if ((input_int >= 1) && (input_int <= 3)) break;
        }
    }




    /* EDIT ALLOWANCE: EDITING MENU (Menu 3) */
    switch (input_int)
    {
        case 1: /* EDIT MENU: DATE */
        while (true) {
            clearScreen();

            // Display: UPDATE(Expense) Title
            border(205);
            displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT DATE)", BLUE);
            border(205);

            // Display: Expense Details to be edited
            cout << BOLDWHITE << "  >> EDITING DATE: " << RESET << endl;
            cout << string(5, ' ') << "* DATE:             " << YELLOW << allowance.getDate() << RESET << endl;
            cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << allowance.getAmount() << RESET << endl;
            cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << allowance.getAccount() << RESET << endl;
            cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << allowance.getDescription() << RESET << endl;
            border(196);

            switch (inputFlow) {
                case 1: // Get New Date
                    displayCenteredLine_NoNewLine(">> Enter NEW DATE: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }
                    else if (validateDateFormat(input_str)) {
                        allowance.setDate(input_str);
                        inputFlow++;
                    }
                    break;

                default: // Display: Notice that date edited successfully
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Allowance Date edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...       ", WHITE);
                    getchar();
                    return;
                    break;
            }
        }
        break;


        case 2: /* EDIT MENU: ACCOUNT */
        while (true) {
            clearScreen();

            // Display: UPDATE(Allowance) Title
            border(205);
            displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT ACCOUNT)", BLUE);
            border(205);

            // Display: Expense Details to be edited
            cout << BOLDWHITE << "  >> EDITING DATE: " << RESET << endl;
            cout << string(5, ' ') << "* DATE:             " << GREEN << allowance.getDate() << RESET << endl;
            cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << allowance.getAmount() << RESET << endl;
            cout << string(5, ' ') << "* ACCOUNT:          " << YELLOW << allowance.getAccount() << RESET << endl;
            cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << allowance.getDescription() << RESET << endl;
            border(196);

            // Display: Account List
            cout << BOLDWHITE << "  >> ACCOUNT LIST: " << RESET << endl;
            displayAccountList();
            cout << "\n";
            displayCenteredLine_NoColor("[ A ] ADD New Account");
            cout << "\n";
            border(196);
            
            switch (inputFlow)
            {
                case 1: // Get New Account
                    displayCenteredLine_NoNewLine(">> Enter NEW ACCOUNT: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }

                    else if ((input_str =="A") || (input_str == "a")) {
                        run_AddAccount("UPDATE: ALLOWANCE (EDIT ACCOUNT)");
                    }

                    else if ((isNumeric(input_str)) && (input_str.size() > 0)) {
                        input_int = stoi(input_str);

                        // Set Account if input is valid
                        if ((input_int > 0) && (input_int <= AccountList.size())) {
                            string acc = AccountList[input_int-1];
                            allowance.setAccount(acc);
                            inputFlow++;
                        }
                    }
                    break;

                default: // Display: Notice that Allowance account edited successfully
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored(">> Allowance Account edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...          ", WHITE);
                    getchar();
                    return;
                    break;
            }
        }
        break;


        case 3: /* EDIT MENU: DESCRIPTION */
        while (true) {
            clearScreen();

            // Display: UPDATE(Allowance) Title
            border(205);
            displayCenteredLine_Colored("UPDATE: ALLOWANCE (EDIT DESCRIPTION)", BLUE);
            border(205);

            // Display: Expense Details to be edited
            cout << BOLDWHITE << "  >> EDITING DATE: " << RESET << endl;
            cout << string(5, ' ') << "* DATE:             " << YELLOW << allowance.getDate() << RESET << endl;
            cout << string(5, ' ') << "* AMOUNT:           " << GREEN << "P " << fixed << setprecision(2) << allowance.getAmount() << RESET << endl;
            cout << string(5, ' ') << "* ACCOUNT:          " << GREEN << allowance.getAccount() << RESET << endl;
            cout << string(5, ' ') << "* DESCRIPTION:      " << GREEN << allowance.getDescription() << RESET << endl;
            border(196);

            switch (inputFlow)
            {
                case 1: // Get New Description
                    displayCenteredLine_NoNewLine(">> Enter NEW DESCRIPTION: ", CYAN);
                    getline(cin, input_str);

                    if ((input_str == "R") || (input_str == "r")) {
                        return;
                    }
                    
                    else if ((input_str =="A") || (input_str == "a")) {
                        run_AddAccount("UPDATE: EXPENSES (EDIT)");
                    }
                    
                    else if (input_str.size() <= 50) {
                        allowance.setDescription(input_str);
                        inputFlow++;
                    }
                    break;

                default: // Display: Notice that Allowance account edited successfully
                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                    displayCenteredLine_Colored  (">> Allowance Description edited successfully!", YELLOW);
                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...              ", WHITE);
                    getchar();
                    return;
                    break;
            }
        }
        break;


        default: // Do Nothing Lol
            break;
    }
}





/*-----------------------------------------------------------------------------------------*/
/*                                   UPDATE: EXPENSES                                      */
/*-----------------------------------------------------------------------------------------*/
// Budget function member to display EXPENSES main menu
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
    border(196);
}

// Budget function member to run Update EXPENSES feature
void Budget :: run_UpdateExpense()
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

// Budget function member to run Update EXPENSES special feature: Add Expenses
void Budget :: run_AddExpenses()
{
    string input_str;
    int input_int, index;
    int inputFlow = 1;
    bool takenSavings = false;

    double amt;
    vector<string> bbys;
    Expense newExpense(getDate_Today(), "MM/DD/YYYY", 0.0, "----------", "----------", "----------", "----------");
    double temporaryBudget = 0;

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
                // Get NEW AMOUNT from user
                displayCenteredLine_NoNewLine(">> Enter AMOUNT: ", CYAN);
                getline(cin , input_str);

                if ((input_str == "R") || (input_str == "r")) return;

                else if ((isDouble(input_str)) && (input_str.size() > 0))
                {
                    amt = stod(input_str);

                    if (totalBudget <= 0) {
                        // Get money from savings if budget is empty
                        if (totalSavings > 0) {
                            if ((amt > 0) && (amt <= totalSavings))
                            {
                                for (auto& savings : savingsList) {
                                    temporaryBudget += savings.get_currentAmt();

                                    // If amount taken from savings reached the amount to be edited, return excess and end loop
                                    if (temporaryBudget >= amt) {
                                        temporaryBudget -= amt;
                                        savings.set_currentAmt(temporaryBudget); // Store excess back to savings
                                        break;
                                    }

                                    savings.set_currentAmt(0.0);
                                }

                                // After taking away the savings, SET THE NEW EXPENSE DATA
                                newExpense.setAmount(amt);
                                inputFlow++;
                                takenSavings = true;
                            }
                            else {
                                // Notify if budget is empty and savings are lacking
                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget is empty and savings are lacking.", YELLOW);
                                displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...                         ", YELLOW);
                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                                getchar();
                                return;
                            }
                        }
                        else {
                            // Notify if budget and savings are empty
                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                            displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget and savings are empty.", YELLOW);
                            displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...              ", YELLOW);
                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                            getchar();
                            return;
                        }
                    }
                    else {
                        // If amount is within budget, set the new amount
                        if ((amt > 0) && (amt <= totalBudget)) {
                            newExpense.setAmount(amt);
                            inputFlow++;
                        }
                        else {
                            // If budget is lacking, take from savings
                            if (totalSavings > 0) {
                                temporaryBudget = totalBudget + totalSavings;

                                // If amount is within totalBudget and totalSavings, take away money from budget and savings and set amount
                                if ((amt > 0) && (amt <= temporaryBudget)) {
                                    temporaryBudget = totalBudget;

                                    for (auto& savings :  savingsList) {
                                        temporaryBudget += savings.get_currentAmt();

                                        // If amount taken from savings reached the amount to be edited, return excess and end loop
                                        if (temporaryBudget >= amt) {
                                            temporaryBudget -= amt;
                                            savings.set_currentAmt(temporaryBudget); // Store excess back to savings
                                            break;
                                        }

                                        savings.set_currentAmt(0.0);
                                    }

                                    totalBudget = 0;
                                    
                                    newExpense.setAmount(amt);
                                    inputFlow++; // proceed to notify: expense edited successfully
                                    takenSavings = true;
                                }
                                else {
                                    // Notify if budget and savings are lacking
                                    displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                    displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget and savings are lacking.", YELLOW);
                                    displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...                ", YELLOW);
                                    displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                                    getchar();
                                    return;
                                }
                            }
                        }

                    }
                    
                }
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
                calculateTotalBudget();
                // add function to track expense limit

                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                displayCenteredLine_Colored(">> New Expense added successfully!", YELLOW);
                if (takenSavings) displayCenteredLine_Colored(">> Savings has been taken to make up the lacking budget.", YELLOW);
                displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...   ", WHITE);
                getchar();
                return; // Return to MENU
                break;
        }
    }
}

// Budget function member to run Update EXPENSES special feature: Delete Expenses created at present day
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
                calculateTotalBudget();

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

// Budget function member to run Update EXPENSES special feature: Edit Expense data created at present day
void Budget :: run_EditExpenses()
{
    int page = 1;
    int maxPages = expensesList_Today.size() / 5;
    if ((expensesList_Today.size() % 5) != 0) maxPages++;

    string input_str;
    int input_int, i = 0, subCat_size;
    int inputFlow = 1;
    bool takenSavings = false;
    double temporaryBudget = 0;

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
                                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...     ", WHITE);
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

                                    // Display: Budget and Savings status
                                    cout << BOLDWHITE << "  >> Current Budget:           P " << fixed << setprecision(2) << totalBudget << "\n" << RESET << endl;
                                    cout << BOLDWHITE << "  >> Current Savings:          P " << fixed << setprecision(2) << totalSavings << "\n\n" << RESET << endl;

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
                                        case 1: /* GET NEW AMOUNT */
                                        displayCenteredLine_NoNewLine(">> Enter NEW AMOUNT: ", CYAN);
                                        getline(cin, input_str);

                                        if ((input_str == "R") || (input_str == "r")) return;

                                        else if ((isDouble(input_str)) && (input_str.size() > 0)) {
                                            double amt = stod(input_str);

                                            // If new amount is less than original, set the new amount. Return excess as budget.
                                            if ((amt > 0) && (amt < expense.getAmount())) {
                                                expense.setAmount(amt);
                                                calculateTotalBudget();
                                                inputFlow++;
                                            }

                                            // Else if new amount is greater than original, check if it can be edited
                                            else {
                                                if (totalBudget <= 0)
                                                {
                                                    // Get money from savings if budget is empty
                                                    if (totalSavings > 0) {
                                                        if ((amt > 0) && (amt <= totalSavings))
                                                        {
                                                            for (auto& savings : savingsList) {
                                                                temporaryBudget += savings.get_currentAmt();

                                                                // If amount taken from savings reached the amount to be edited, return excess and end loop
                                                                if (temporaryBudget >= amt) {
                                                                    temporaryBudget -= amt;
                                                                    savings.set_currentAmt(temporaryBudget); // Store excess back to savings
                                                                    break;
                                                                }

                                                                savings.set_currentAmt(0.0);
                                                            }

                                                            // After taking away the savings, SET THE NEW EXPENSE DATA
                                                            expense.setAmount(amt);
                                                            calculateTotalBudget();
                                                            inputFlow++;
                                                            takenSavings = true;
                                                        }
                                                        else {
                                                            // Notify if budget is empty and savings are lacking
                                                            border(196);
                                                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                            displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget is empty and savings are lacking.", YELLOW);
                                                            displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...                         ", YELLOW);
                                                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                                                            getchar();
                                                            return;
                                                        }
                                                    }

                                                    else {
                                                        // Notify if budget and savings are empty
                                                        border(196);
                                                        displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                        displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget and savings are empty.", YELLOW);
                                                        displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...              ", YELLOW);
                                                        displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                                                        getchar();
                                                        return;
                                                    }

                                                }

                                                else {
                                                    // If amount is within budget, set the new amount
                                                    if ((amt > 0) && (amt <= totalBudget)) {
                                                        expense.setAmount(amt);
                                                        calculateTotalBudget();
                                                        inputFlow++;
                                                    }
                                                    else {
                                                        // If budget is lacking, take from savings
                                                        if (totalSavings > 0) {
                                                            temporaryBudget = totalBudget + totalSavings;

                                                            // If amount is within totalBudget and totalSavings, take away money from budget and savings and set amount
                                                            if ((amt > 0) && (amt <= temporaryBudget)) {
                                                                temporaryBudget = totalBudget;

                                                                for (auto& savings :  savingsList) {
                                                                    temporaryBudget += savings.get_currentAmt();

                                                                    // If amount taken from savings reached the amount to be edited, return excess and end loop
                                                                    if (temporaryBudget >= amt) {
                                                                        temporaryBudget -= amt;
                                                                        savings.set_currentAmt(temporaryBudget); // Store excess back to savings
                                                                        break;
                                                                    }

                                                                    savings.set_currentAmt(0.0);
                                                                }

                                                                totalBudget = 0;
                                                                expense.setAmount(amt);
                                                                calculateTotalBudget();
                                                                inputFlow++; // proceed to notify: expense edited successfully
                                                                takenSavings = true;
                                                            }

                                                            else {
                                                                // Notify if budget and savings are lacking
                                                                border(196);
                                                                displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                                                displayCenteredLine_Colored(">> Cannot edit Expense amount! Your budget and savings are lacking.", YELLOW);
                                                                displayCenteredLine_Colored(">> Please ADD to your ALLOWANCE or/and try again...                ", YELLOW);
                                                                displayCenteredLine_NoNewLine(">> Press 'ENTER' to return to Expense menu and continue...   ", WHITE);
                                                                getchar();
                                                                return;
                                                            }
                                                        }
                                                    }

                                                }
                                            }
                                            
                                            
                                        }
                                        break;


                                        default: /* DISPLAY: NOTIFICATION */
                                            calculateTotalBudget();

                                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                                            displayCenteredLine_Colored(">> Expense Amount edited successfully!", YELLOW);
                                            if (takenSavings) displayCenteredLine_Colored(">> Savings were taken due to lack of budget... ", YELLOW);
                                            displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue...   ", WHITE);
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
/*          UPDATE: EXPENSE LIMIT AND SAVINGS TRACKER AND NOTIFIER          */
/*--------------------------------------------------------------------------*/
//Budget function member to track expenses and update the expense limit goal of the amount spent
void Budget :: trackExpenses()
{
    double amt;
    string date;

    for (auto& goal : expenseLimitsList) {
        amt = 0;

        // Iterate over all expenses created today
        for (auto& expense : expensesList_Today) {
            if (isDateInRange(expense.getDate(), goal.get_startDate(), goal.get_dueDate()))
            {
                amt += expense.getAmount();
            }
        }

        // Iterate over all other expenses
        for (auto& expense : expensesList) {
            if (isDateInRange(expense.getDate(), goal.get_startDate(), goal.get_dueDate()))
            {
                amt += expense.getAmount();
            }
        }

        goal.set_currentAmt(amt);
    }
}

// Budget function member to track all savings goal when its already due by the previous day, delete goal, and send notification
void Budget :: trackDue_SavingsGoal(bool& newNotif)
{
    Notification notifHandler;
    int index = 0;

    for (auto& goal : savingsList)
    {
        string afterDueDate = getDate_FromDateInput(goal.get_dueDate(), 1);

        if (getDate_Today() == afterDueDate) {
            notifHandler.createNotif_SavingsGoal(goal, index+1);
            removeSavings(index);
            newNotif = true;
            index--;
        }
        index++;
    }
}

// Budget function member to track all expense limit goal when its already due by the previous day, delete goal, and send notification
void Budget :: trackDue_ExpensesLimit(bool& newNotif)
{
    Notification notifHandler;
    int index = 0;

    for (auto& expenseLim : expenseLimitsList)
    {
        string afterDueDate = getDate_FromDateInput(expenseLim.get_dueDate(), 1);

        if (getDate_Today() == afterDueDate) {
            notifHandler.createNotif_ExpenseLim(expenseLim, index+1);
            removeExpenseLimit(index);
            expenseLim.set_currentAmt(0.0);
            newNotif = true;
            index--;
        }
        index++;
    }
}

// Budget function to remove savings goal at least 2 day after due date if not removed and send notification
void Budget :: autoDelete_SavingsGoal(bool& newNotif)
{
    Notification notifHandler;
    int index = 1;

    auto goal = savingsList.begin();
    while (goal != savingsList.end())
    {
        string goal_DueDelete = getDate_FromDateInput(goal->get_dueDate(), 1);

        // Delete goal if date today is not within start date and date after due
        if (!isDateInRange(getDate_Today(), goal->get_startDate(), goal->get_dueDate())) {
            notifHandler.createNotif_SavingsGoal(*goal, index);
            goal = savingsList.erase(goal);
            newNotif = true;
        }
        else {
            ++goal;
        }
        ++index;
    }
}

// Budget function to remove expense limit goal at least 2 days after due date if not removed and send notification
void Budget :: autoDelete_ELGoal(bool& newNotif)
{
    Notification notifHandler;
    int index = 1;

    auto goal = expenseLimitsList.begin();
    while (goal != expenseLimitsList.end())
    {
        string goal_DueDelete = getDate_FromDateInput(goal->get_dueDate(), 1);

        // Delete goal if date today is not within start date and date after due
        if (!isDateInRange(getDate_Today(), goal->get_startDate(), goal_DueDelete)) {
            notifHandler.createNotif_ExpenseLim(*goal, index);
            goal = expenseLimitsList.erase(goal);
            newNotif = true;
        }
        else {
            ++goal;
        }
        ++index;
    }
}





/*---------------------------------------------------------------------------*/
/*                        UPDATE: LOAD AND SAVE DATA                         */
/*---------------------------------------------------------------------------*/
// Budget function member to load all necessary data
void Budget :: loadData()
{
    allowancesList.clear();
    allowancesList_Today.clear();
    expensesList.clear();
    expensesList_Today.clear();
    savingsList.clear();
    expenseLimitsList.clear();
    CategoryList.clear();
    AccountList.clear();

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
}





/*---------------------------------------------------------------------------*/
/*                        UPDATE: RUN UPDATE FUNCTION                        */
/*---------------------------------------------------------------------------*/
// Budget function member to run the entire UPDATE feature
void Budget :: run_BBUpdate()
{
    string input;
    while (true) {
        trackExpenses();
        calculateTotalBudget();
        displayUpdateMenu();
        getline(cin, input);

        if (input == "R" || input == "r") {
            saveState();
            break;
        }

        try {
            if (input == "1") {
                run_UpdateLimitExpenses();
            }
            else if (input == "2") {
                run_UpdateSavings();
            }
            else if (input == "3") {
                run_UpdateAllowance();
            }
            else if (input == "4") {
                run_UpdateExpense();
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

/*-----------------------------------------------------------------------------------------*/
/*----------------------------- INBOX CLASS: FUNCTION MEMBERS -----------------------------*/
/*-----------------------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------------------*/
/*-------------------------- NOTIFICATION CLASS: FUNCTION MEMBERS -------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*  Notification function member that creates a custom notification, is stored unto file, and returns bool value

    >> Mode: 
    [1] Reminder                            [4] Savings Report
    [2] Expense Limit Report, negative      [5] Log in notif
    [3] Expense Limit Report, positive      [6] Custom notification
*/
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
            NewNotif += ExpenseLimReport_Pos;
            break;
        case 4:
            NewNotif += savingsReport;
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

// Notification function member to create notification for savings goal
void Notification :: createNotif_SavingsGoal(SavingsAndExpenseLim& savings, int iter)
{
    string notifTxt;
    double currentAmt = savings.get_currentAmt();
    double goal = savings.get_goal();
    
    if (currentAmt == goal) {
        notifTxt = "\t(>v<)/ Savings Goal #" + to_string(iter) + " has reached its goal! GOOD JOB! KEEP AT IT!\n";
        notifTxt += "\tThe amount saved up will be added back to your budget, and the goal will be deleted automatically.\n";
    } else if (currentAmt > 0 && currentAmt < goal) {
        stringstream stream1;
        stream1 << fixed << setprecision(2) << currentAmt;
        string amt = stream1.str();

        notifTxt = "\t(OwO) Savings Goal #" + to_string(iter) + " did not reach the goal, but you saved up " + BOLDGREEN + "P " + amt + RESET + "\n";
        notifTxt += "\tThat's okay! You still did great! Better budget next time! XD\n";
        notifTxt += "\tThe amount saved up will be added back to your budget, and the goal will be deleted automatically.\n";
    } else if (currentAmt == 0) {
        notifTxt = "\t(T^T) Savings Goal #" + to_string(iter) + " is already due, and you didn't save anything...\n";
        notifTxt += "\tThat's okay! But better budget well next time! XD. The goal will be deleted automatically.\n";
    }

    createNotification(6, notifTxt);
}

// Notification function member to create notification for expense limit goal
void Notification :: createNotif_ExpenseLim(SavingsAndExpenseLim& expenseLim, int iter)
{
    string notifTxt;
    double currentAmt = expenseLim.get_currentAmt();
    double goal = expenseLim.get_goal();
    
    if (currentAmt > goal) {
        stringstream stream1;
        stream1 << fixed << setprecision(2) << currentAmt;
        string amt = stream1.str();

        notifTxt = "\t(T^T) Expense Limit Goal #" + to_string(iter) + " has exceeded its limit!\n" + "You spent " + BOLDGREEN + "P " + amt + RESET + " ... That's okay!\n";
        notifTxt += "\tBetter budget next time! The goal will be deleted automatically.\n";
    }

    else if ((currentAmt > 0) && (currentAmt <= goal)) {
        stringstream stream1;
        stream1 << fixed << setprecision(2) << currentAmt;
        string amt = stream1.str();

        notifTxt = "\t(OwO) In Expense Limit Goal #" + to_string(iter) + " you spent up to " + BOLDGREEN + "P " + amt + RESET + "\n";
        notifTxt += "\tWOAH! You did great! Better budget again next time! XD. The goal will be deleted automatically.\n";
    }

    else if (currentAmt == 0) {
        notifTxt = "\t(O-O)! Expense Limit Goal #" + to_string(iter) + " is already due, and you didn't spend anything?! AMAZING!!!\n";
        notifTxt += "\tHow thrifty! Keep that up! The goal will be deleted automatically.\n";
    }

    createNotification(6, notifTxt);
}


















/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
    // Empty vectors
    allowancesList.clear();
    expensesList.clear();
    AccountList.clear();
    CategoryList.clear();

    // Load all data
    loadExpenses();
    loadAllowances();
    loadCategoryList();
    loadAccountList();

    calculateTotalBudget();
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
/*       DATAandHISTORY : Vector Maker        */
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
/*         DATAandHISTORY : Sort Data         */
/*--------------------------------------------*/
// DATAandHISTORY function member to sort expense list by date, ascending order
void DATAandHISTORY :: sortExpensesByDate_Ascending(vector<Expense>& expenseData)
{
    sort(expenseData.begin(), expenseData.end(), [](const Expense& a, const Expense& b) {
        return convertToComparableDate(a.getDate()) < convertToComparableDate(b.getDate());
    });
}

// DATAandHISTORY function member to sort expense vector by date, descending order
void DATAandHISTORY :: sortExpensesByDate_Descending(vector<Expense>& expenseData)
{
    sort(expenseData.begin(), expenseData.end(), [](const Expense& a, const Expense& b) {
        return convertToComparableDate(a.getDate()) > convertToComparableDate(b.getDate());
    });
}

// DATAandHISTORY function member to sort allowance vector by date, ascending order
void DATAandHISTORY :: sortAllowanceByDate_Ascending(vector<Allowance>& allowanceData)
{
    sort(allowanceData.begin(), allowanceData.end(), [](const Allowance& a, const Allowance& b) {
        return convertToComparableDate(a.getDate()) < convertToComparableDate(b.getDate());
    });
}

// DATAandHISTORY function member to sort allowance vector by date, descending order
void DATAandHISTORY :: sortAllowanceByDate_Descending(vector<Allowance>& allowanceData)
{
    sort(allowanceData.begin(), allowanceData.end(), [](const Allowance& a, const Allowance& b) {
        return convertToComparableDate(a.getDate()) > convertToComparableDate(b.getDate());
    });
}






/*--------------------------------------------*/
/*       DATAandHISTORY : Display Data        */
/*--------------------------------------------*/
// DATAandHISTORY function member to display allowance data table by page
void DATAandHISTORY :: displayAllowancesData(vector<Allowance> allowanceData, int page = 1, int dataPerPage = 5)
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
        int maxPages = vSize / dataPerPage;

        if ((vSize % dataPerPage) != 0) maxPages++;
        if (page > maxPages) page = maxPages;
        if (page < 1) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, vSize);
        int items = end - start;

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

            for (i = 0; i < vacant; i++) {
                cout << "\n" << string(10, ' ') << border;
                displayTxtByColumn_CENTERED(to_string(start+1), WHITE, 7);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("----- ----- -----", WHITE, 60);
                start++;
            }
        }

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: " + to_string(page) + " out of " + to_string(maxPages), GRAY);
    }
    else {
        // Display: Dummy data
        for (i = 0; i < dataPerPage; i++) {
            cout << "\n" << string(10, ' ') << border;
            displayTxtByColumn_CENTERED(to_string(i + 1), WHITE, 7);
            displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("----- ----- -----", WHITE, 60);
        }

        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: 1 out of 1", GRAY);
    }
}

// DATAandHISTORY function member to display expense data table by page
void DATAandHISTORY :: displayExpensesData(vector<Expense> expenseData, int page = 1, int dataPerPage = 5)
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
        int maxPages = vSize / dataPerPage;

        if ((vSize % dataPerPage) != 0) maxPages++;
        if (page > maxPages) page = maxPages;
        if (page < 1) page = 1;

        int start = (page - 1) * dataPerPage;
        int end =  min(start + dataPerPage, vSize);
        int items = end - start;

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

            for (i = 0; i < vacant; i++) {
                cout << "\n" << border;
                displayTxtByColumn_CENTERED(to_string(start+1), WHITE, 7);
                displayTxtByColumn("--/--/----", WHITE, COLUMNWIDTH-2);
                displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("-----", WHITE, COLUMNWIDTH+2);
                displayTxtByColumn("----- ----- -----", WHITE, 54);
                start++;
            }
        }
        cout << "\n\n";
        displayCenteredLine_Colored("Displaying Page: " + to_string(page) + " out of " + to_string(maxPages), GRAY);
    }
    else {
        // Display: Dummy data
        for (i = 0; i < dataPerPage; i++) {
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

// DATAandHISTORY function member to display a list of accounts
void DATAandHISTORY :: displayAccountList()
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
        displayTxtByColumn_NB("[ " + to_string(i+1) + " ] " + Accounts[i], WHITE, 25);
        displayTxtByColumn_NB("[ " + to_string(i+6) + " ] " + Accounts[i + 5], WHITE, 25);
        cout << "\n";
    }
}

// DATAandHISTORY function member to display a list of Category Parent
void DATAandHISTORY :: displayCategoryList_parent()
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




/*---------------------------------------------------*/
/*       DATAandHISTORY FEATURE: EXPENSE DATA        */
/*---------------------------------------------------*/

// DATAandHISTORY function member to display title + custom subtitle of DATA and HISTORY
void DATAandHISTORY :: displayTitle_DataAndHistory(string subTitle = "")
{
    clearScreen();

    border(205);
    displayCenteredLine_Colored("DATA & HISTORY" + subTitle, BLUE);
    border(205);
}

// DATAandHISTORY function member to display table for Expense Data today and yesterday
void DATAandHISTORY :: displayTable_YESTERDAYvsTODAY(vector<Expense> expensesData_Today, vector<Expense> expensesData_Yesterday, int page = 1)
{
    // Get date TODAY and YESTERDAY
    string dateYesterday = getDate_FromDateInput(getDate_Today(), -1);
    string dateToday = getDate_Today();

    // Calculate total expenses TODAY and YESTERDAY
    double totalExp_Yesterday = calculateTotal_GivenExpenses(expensesData_Yesterday);
    double totalExp_Today = calculateTotal_GivenExpenses(expensesData_Today);
    stringstream expYesterday, expToday;
    expYesterday << fixed << setprecision(2) << totalExp_Yesterday;
    expToday << fixed << setprecision(2) << totalExp_Today;



    // Display: EXPENSE DATA TODAY
    displayCenteredLine_Colored("EXPENSES TODAY: " + dateToday, BOLDWHITE);
    cout << "\n";
    displayExpensesData(expensesData_Today, page);
    cout << "\n";
    border('-');

    // Display: EXPENSE DATA YESTERDAY
    displayCenteredLine_Colored("EXPENSES YESTERDAY: " + dateYesterday, BOLDWHITE);
    cout << "\n";
    displayExpensesData(expensesData_Yesterday, page);
    cout << "\n";
    border(196);



    // Display: TOTAL EXPENSES EACH DAY
    if (totalExp_Today > totalExp_Yesterday) {
        cout << BOLDWHITE << ">> Total Expenses(TODAY):         " << RESET << GREEN << "P " << expToday.str() << RESET << endl;
        cout << BOLDWHITE << ">> Total Expenses(YESTERDAY):     " << RESET << RED   << "P " << expYesterday.str() << RESET << endl;
    }
    else if (totalExp_Today < totalExp_Yesterday) {
        cout << BOLDWHITE << ">> Total Expenses(TODAY):         " << RESET << RED   << "P " << expToday.str() << RESET << endl;
        cout << BOLDWHITE << ">> Total Expenses(YESTERDAY):     " << RESET << GREEN << "P " << expYesterday.str() << RESET << endl;
    }
    else {
        cout << BOLDWHITE << ">> Total Expenses(TODAY):         " << RESET << GREEN << "P " << expToday.str() << RESET << endl;
        cout << BOLDWHITE << ">> Total Expenses(YESTERDAY):     " << RESET << GREEN << "P " << expYesterday.str() << RESET << endl;
    }
    border(196);
}

// DATAandHISTORY function member to display a list of categories and percentage of total expenses each category
void DATAandHISTORY :: displayTable_RankByCategory()
{
    int iter = 1;
    char c = 179;

    // Display Headers
    cout << string(19, ' ') << c;
    displayTxtByColumn_CENTERED("INDEX", BOLDWHITE, 7);
    displayTxtByColumn_CENTERED("CATEGORY", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("TOTAL COST", BOLDWHITE, COLUMNWIDTH+2);
    displayTxtByColumn_CENTERED("PERCENTAGE", BOLDWHITE, 70);
    cout << "\n";


    for (const auto& category : CategoryList)
    {
        string catName = category.getParent();
        double totalAmt = 0;

        // Get total percentage of expenses in each category
        for (const auto& expenses : expensesList)
        {
            if (expenses.getCategory() == catName)
            totalAmt += expenses.getAmount();
        }
        double rate = (totalAmt / totalExpenses) * 100;
        stringstream ss;
        ss << fixed << setprecision(2) << totalAmt;

        // Display data by table
        string percentBar = makePercentBar(rate);
        cout << string(19, ' ') << c;
        displayTxtByColumn_CENTERED(to_string(iter), WHITE, 7);
        displayTxtByColumn(catName, WHITE, COLUMNWIDTH+2);
        displayTxtByColumn("P " + ss.str(), WHITE, COLUMNWIDTH+2);
        displayTxtByColumn_CENTERED(percentBar, WHITE, 70);
        cout << "\n";
        iter++;
    }

    // Display vacant slots
    if (CategoryList.size() < 10) {
        int vacant = 10 - CategoryList.size();

        for (int i = 0; i < vacant; i++) {
            cout << string(19, ' ') << c;
            displayTxtByColumn_CENTERED(to_string(iter), WHITE, 7);
            displayTxtByColumn("----------", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn("P 0.00", WHITE, COLUMNWIDTH+2);
            displayTxtByColumn_CENTERED(makePercentBar(0), WHITE, 70);
            cout << "\n";
            iter++;
        }
    }
    cout << "\n";
    border(196);
}

// DATAandHISTORY function member to run Expense Data Feature
void DATAandHISTORY :: runDH_ExpenseData()
{
    // USER INPUTS
    string optionDisplay = "1";
    string choice;
    

    // Expense Vectors (Today & Yesterday)
    string dateToday = getDate_Today();
    string dateYesterday = getDate_FromDateInput(dateToday, -1);
    auto expData_Today = getExpenseData_ByDate(dateToday);
    auto expData_Yesterday = getExpenseData_ByDate(dateYesterday);


    // For displaying expense data
    int page = 1;
    int dataPerPage = 10;
    int expDataSize_1 = expData_Today.size();
    int expDataSize_2 = expData_Yesterday.size();

    int maxPages1 = expDataSize_1 / dataPerPage;
    int maxPages2 = expDataSize_2 / dataPerPage;
    int maxPages_Highest;
    if ((expDataSize_1 % dataPerPage) != 0) maxPages1++;
    if ((expDataSize_2 % dataPerPage) != 0) maxPages2++;

    if (maxPages1 > maxPages2) maxPages_Highest = maxPages1;
    else maxPages_Highest = maxPages2;


    while (true) {
        displayTitle_DataAndHistory(": EXPENSE DATA");

        // DISPLAY PT. 1 of the MENU FOR YESTERDAY VS TODAY
        if (optionDisplay == "1") {
            // Display: YESTERDAY vs TODAY Table
            displayCenteredLine_Colored("YESTERDAY vs TODAY", BOLDYELLOW);
            border('-');
            displayTable_YESTERDAYvsTODAY(expData_Today, expData_Yesterday, page);


            // Display: Expense DATA Options
            displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
            cout << "\n";
            displayCenteredLine_Colored("[ 1 ] YESTERDAY vs TODAY               [ P ] Previous Page", WHITE);
            displayCenteredLine_Colored("[ 2 ] RANK EXPENSES by CATEGORY        [ N ] Next Page    ", WHITE);
        }

        // DISPLAY PT. 1 of the MENU FOR RANK BY CATEGORY
        else if (optionDisplay == "2") {
            // Display: RANK BY CATEGORY Feature
            displayCenteredLine_Colored("RANK BY CATEGORY", BOLDYELLOW);
            border('-');
            cout << "\n";
            displayTable_RankByCategory();

            // Display: Expense DATA Options
            displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
            cout << "\n";
            displayCenteredLine_Colored("[ 1 ] YESTERDAY vs TODAY", WHITE);
            displayCenteredLine_Colored("[ 2 ] RANK by CATEGORY  ", WHITE);
        }

        // DISPLAY PT. 2 of the MENU FOR EXPENSE DATA
        cout << "\n";
        displayCenteredLine_Colored("[ R ] RETURN", WHITE);
        cout << "\n";
        border(205);


        // GET USER INPUT
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
        getline(cin, choice);


        // Display: chosen option by user
        if ((choice == "R") || (choice == "r")) {
            return; // End function
        }

        else if (choice == "1") {
            if (optionDisplay != "1")
                optionDisplay = choice; // DISPLAY: YESTERDAY vs TODAY table
        }

        else if (choice == "2") {
            if (optionDisplay != "2")
                optionDisplay = choice; // DISPLAY: RANK EXPENSES by CATEGORY
        }

        else if ((choice == "P") || (choice == "p")) {
            if ((optionDisplay == "1") && (page > 1)) page--;
            
        }

        else if ((choice == "N") || (choice == "n")) {
            if ((optionDisplay == "1") && (page < maxPages_Highest)) page++;
        }
    }
}





/*------------------------------------------------------------------------*/
/*       DATAandHISTORY FEATURE: TRANSACTION AND ALLOWANCE HISTORY        */
/*------------------------------------------------------------------------*/
// DATAandHISTORY function member to run Filter By Category option, exclusive to Expense Class only
string DATAandHISTORY :: run_GetCategory()
{
    string input;

    while (true)
    {
        // Display Menu to get category
        displayTitle_DataAndHistory(": TRANSACTION HISTORY");
        displayCenteredLine_Colored("CATEGORY LIST", YELLOW);
        displayCategoryList_parent();
        border('-');
        cout << "   >> Enter the index number of the Category you wish to pick." << endl;
        border(196);

        // Get user input
        displayCenteredLine_NoNewLine(">> Enter NUMBER: ", CYAN);
        getline(cin, input);

        if ((input == "R") || (input == "r")) {
            return "";
        }
        else if ((isNumeric(input)) && (!input.empty())) {
            int input_int = stoi(input);
            if ((input_int > 0) && (input_int <= CategoryList.size())) {
                // Return Category Parent name
                return CategoryList[input_int-1].getParent();
            }
        }
    }
}

// DATAandHISTORY function member to run Filter by Account option. This feature is scratched due to lack of time
string DATAandHISTORY :: run_GetAccount()
{
    string input;

    while (true)
    {
        // Display Menu to get category
        displayTitle_DataAndHistory(": TRANSACTION HISTORY");
        displayCenteredLine_Colored("ACCOUNT LIST", YELLOW);
        displayAccountList();
        border('-');
        cout << "   >> Enter the index number of the Account you wish to pick." << endl;
        border(196);

        // Get user input
        displayCenteredLine_NoNewLine(">> Enter NUMBER: ", CYAN);
        getline(cin, input);

        if ((input == "R") || (input == "r")) {
            return "";
        }
        else if ((isNumeric(input)) && (!input.empty())) {
            int input_int = stoi(input);
            if ((input_int > 0) && (input_int <= AccountList.size())) {
                // Return Account name
                return AccountList[input_int-1];
            }
        }
    }
}

// DATAandHISTORY function member to run Transaction History Feature
void DATAandHISTORY :: runDH_TAHistory()
{
    // USER INPUTS
    string optionDisplay = "E";
    string choice, input;
    int filterDisplay = 6;
    int choice_int;


    // For displaying expense & allowance data
    int page = 1;
    int dataPerPage = 10;
    int expDataSize = expensesList.size();
    int allowanceDataSize = allowancesList.size();

    int maxPages1 = expDataSize / dataPerPage;
    int maxPages2 = allowanceDataSize / dataPerPage;
    int maxPages_Highest;
    if ((expDataSize % dataPerPage) != 0) maxPages1++;
    if ((allowanceDataSize % dataPerPage) != 0) maxPages2++;

    if (maxPages1 > maxPages2) maxPages_Highest = maxPages1;
    else maxPages_Highest = maxPages2;

    // For displaying filtered expense & allowance data
    vector<Allowance> filteredAllowances;
    vector<Expense> filteredExpenses;
    string filter;


    while (true) {
        displayTitle_DataAndHistory(": TRANSACTION HISTORY");

        // DISPLAY: Allowance History
        if ((optionDisplay == "A") || (optionDisplay == "a")) {
            // If OPTION is SHOW ALL
            if (filterDisplay == 6)
            {
                displayCenteredLine_Colored("ALLOWANCE HISTORY", BOLDWHITE);
                cout << "\n";
                border(196);
                displayAllowancesData(allowancesList, page, 10);
                border(196);
            }

            // IF OPTION IS FILTERED
            else {
                displayCenteredLine_Colored("FILTERED ALLOWANCE HISTORY: " + filter, BOLDWHITE);
                cout << "\n";
                border(196);
                displayAllowancesData(filteredAllowances, page, 10);
                border(196);
            }
            
        }

        // DISPLAY: Expense History
        else if ((optionDisplay == "E") || (optionDisplay == "e")) {
            // If OPTION is SHOW ALL
            if (filterDisplay == 6)
            {
                displayCenteredLine_Colored("TRANSACTION HISTORY", BOLDWHITE);
                cout << "\n";
                border(196);
                displayExpensesData(expensesList, page, 10);
                border(196);
            }

            // IF OPTION IS FILTERED
            else {
                displayCenteredLine_Colored("FILTERED TRANSACTION HISTORY: " + filter, BOLDWHITE);
                cout << "\n";
                border(196);
                displayExpensesData(filteredExpenses, page, 10);
                border(196);
            }
            
        }
        

        
        // Display: Expense DATA Options
        displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
        cout << "\n";
        displayCenteredLine_Colored("[ A ] ALLOWANCE HISTORY            [ E ] EXPENSE HISTORY     ", WHITE);
        displayCenteredLine_Colored("[ P ] Previous Page                [ N ] Next Page           ", WHITE);
        cout << "\n";
        displayCenteredLine_Colored("[ 1 ] Sort by DATE (Ascending)     [ 4 ] Filter by DATE      ", WHITE);
        displayCenteredLine_Colored("[ 2 ] Sort by DATE (Descending)    [ 5 ] Filter by DATE RANGE", WHITE);
        displayCenteredLine_Colored("[ 3 ] Filter by CATEGORY           [ 6 ] Show ALL            ", WHITE);
        cout << "\n";
        displayCenteredLine_Colored("[ R ] RETURN", WHITE);
        cout << "\n";
        border(205);


        // GET USER INPUT
        displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
        getline(cin, choice);



        // End Function
        if ((choice == "R") || (choice == "r")) {
            return; // End function
        }

        // Display Allowance
        else if ((choice == "A") || (choice == "a")) {
            if ((optionDisplay != "A") || (optionDisplay != "a"))
                page = 1;

            optionDisplay = choice;
            filterDisplay = 6; // Show All Data
        }

        // Display Expenses
        else if ((choice == "E") || (choice == "e")) {
            if ((optionDisplay != "E") || (optionDisplay != "e"))
                page = 1;

            optionDisplay = choice;
            filterDisplay = 6; // Show All Data
        }

        // Display Previous page
        else if ((choice == "P") || (choice == "p")) {
            if (page > 1) page--;
        }

        // Display Next page
        else if ((choice == "N") || (choice == "n")) {
            if ((optionDisplay == "A") || (optionDisplay == "a")) {
                if (page < maxPages2) page++;
                else page = maxPages2;
            }
            else if ((optionDisplay == "E") || (optionDisplay == "e")) {
                if (page < maxPages1) page++;
                else page = maxPages1;
            }
        }

        // Sorting
        else if ((isNumeric(choice)) && (!choice.empty())) {
            choice_int = stoi(choice);

            switch (choice_int)
            {
                case 1: // SORT BY DATE (ASCENDING)
                    if (filterDisplay == 6) {
                        sortExpensesByDate_Ascending(expensesList);
                        sortAllowanceByDate_Ascending(allowancesList);
                    }
                    else {
                        sortExpensesByDate_Ascending(filteredExpenses);
                        sortAllowanceByDate_Ascending(filteredAllowances);
                    }
                    break;



                case 2: // SORT BY DATE (DESCENDING)
                    if (filterDisplay == 6) {
                        sortExpensesByDate_Descending(expensesList);
                        sortAllowanceByDate_Descending(allowancesList);
                    }
                    else {
                        sortExpensesByDate_Descending(filteredExpenses);
                        sortAllowanceByDate_Descending(filteredAllowances);
                    }
                    break;



                case 3: // FILTER BY CATEGORY
                    if ((optionDisplay == "E") || (optionDisplay == "e")) {
                        input = run_GetCategory();

                        // If Filtering expenses
                        if (!input.empty()) {
                            // Empty vector
                            filteredExpenses.clear();

                            // Get filtered vector
                            vector<Expense> expenses_TempList = getExpenseData_ByCat(input);

                            // Copy elements from temporary vector
                            filteredExpenses.insert(filteredExpenses.end(), expenses_TempList.begin(), expenses_TempList.end());

                            // Display filtered data
                            filterDisplay = 3;
                            filter = input;
                        }
                    }
                    break;
                


                case 4: // FILTER BY DATE
                    border(196);
                    displayCenteredLine_NoNewLine(">> Enter DATE (MM/DD/YYYY): ", CYAN);
                    getline(cin, input);

                    // Stop filtering data if input invalid
                    if (!validateDateFormat(input)) {
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Enter valid date format: MM/DD/YYYY", YELLOW);
                        getchar();
                        break;
                    }

                    // If Filtering expenses
                    if ((optionDisplay == "E") || (optionDisplay == "e")) {
                        // Empty vector
                        filteredExpenses.clear();

                        // Get filtered vector
                        vector<Expense> expenses_TempList = getExpenseData_ByDate(input);

                        // Copy elements from temporary vector
                        filteredExpenses.insert(filteredExpenses.end(), expenses_TempList.begin(), expenses_TempList.end());

                        // Display filtered data
                        filterDisplay = 4;
                        filter = input;
                    }

                    // If Filtering allowances
                    else if ((optionDisplay == "A") || (optionDisplay == "a")) {
                        // Empty vector
                        filteredAllowances.clear();

                        // Get filtered vector
                        vector<Allowance> allowances_TempList = getAllowanceData_ByDate(input);

                        // Copy elements from temporary vector
                        filteredAllowances.insert(filteredAllowances.end(), allowances_TempList.begin(), allowances_TempList.end());

                        // Display filtered data
                        filterDisplay = 4;
                        filter = input;
                    }
                    break;
                


                case 5: // FILTER BY DATE RANGE
                    border(196);
                    displayCenteredLine_NoNewLine(">> Enter FIRST DATE (MM/DD/YYYY): ", CYAN);
                    getline(cin, input);

                    // Stop filtering data if input invalid
                    if (!validateDateFormat(input)) {
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                        getchar();
                        break;
                    }

                    border(196);
                    displayCenteredLine_NoNewLine(">> Enter SECOND DATE (MM/DD/YYYY): ", CYAN);
                    getline(cin, choice);

                    // Stop filtering data if input invalid
                    if (!validateSecondDate(input, choice)) {
                        displayCenteredLine_Colored("WARNING", BOLDYELLOW);
                        displayCenteredLine_Colored(">> Please enter valid input.", YELLOW);
                        getchar();
                        break;
                    }

                    // If Filtering expenses
                    if ((optionDisplay == "E") || (optionDisplay == "e")) {
                        // Empty vector
                        filteredExpenses.clear();

                        // Filter and insert to vector
                        string date = input;
                        while (isDateInRange(date, input, choice))
                        {
                            for (const auto& expense : expensesList)
                            {
                                if (expense.getDate() == date)
                                filteredExpenses.push_back(expense);
                            }
                            date = getDate_FromDateInput(date, 1); // Increment date
                        }

                        
                        // Display filtered data
                        filterDisplay = 5;
                        filter = choice + " - " + input;
                    }

                    // If Filtering allowances
                    else if ((optionDisplay == "A") || (optionDisplay == "a")) {
                        // Empty vector
                        filteredAllowances.clear();

                        // Filter and insert to vector
                        string date = input;
                        while (isDateInRange(date, input, choice))
                        {
                            for (const auto& allowance : allowancesList)
                            {
                                if (allowance.getDate() == date)
                                filteredAllowances.push_back(allowance);
                            }
                            date = getDate_FromDateInput(date, 1); // Increment date
                        }

                        
                        // Display filtered data
                        filterDisplay = 5;
                        filter = choice + " - " + input;
                    }
                    break;



                case 6:
                    filterDisplay = 6;
                    break;



                default:
                    break;
            }
        }

    }
}






/*----------------------------------------------*/
/*            DATAandHISTORY FEATURE            */
/*----------------------------------------------*/

void DATAandHISTORY :: displayMenu_DataAndHistory()
{
    clearScreen();

    border(205); 
    displayCenteredLine_Colored("DATA & HISTORY", BLUE);
    border(205);

    cout << BOLDWHITE << "    >> EXPENSES DATA" << RESET << endl;
    cout <<              "          - Displays basic data of your expenses!\n" << endl;
    cout <<              "          - You can compare your expenses TODAY and YESTERDAY!" << endl;
    cout <<              "        See how and where you spend your money!\n\n" << endl;

    cout << BOLDWHITE << "    >> TRANSACTION ans ALLOWANCE HISTORY" << RESET << endl;
    cout <<              "          - Displays all the transactions created and allowances made!\n" << endl;
    cout <<              "          - You can take a look a your transaction and allowance history," << endl;
    cout <<              "        view them by sorting either by Category or by Date!\n" << endl;


    border(196);
    displayCenteredLine_Colored("OPTIONS", BOLDWHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ 1 ] EXPENSES DATA      ", WHITE);
    displayCenteredLine_Colored("[ 2 ] TRANSACTION HISTORY", WHITE);
    cout << "\n";
    displayCenteredLine_Colored("[ R ] RETURN", WHITE);
    cout << "\n";
    border(205);
    displayCenteredLine_NoNewLine(">> Enter number: ", CYAN);
}

void DATAandHISTORY :: runBB_DataAndHistory()
{
    string option;
    loadData();

    while(true) {
        // Display: DATA and HISTORY menu
        displayMenu_DataAndHistory();

        getline(cin, option);

        if ((option == "R") || (option == "r")) {
            return;
        }
        else if ((isNumeric(option)) && (!option.empty())) {
            int option_int = stoi(option);

            switch (option_int)
            {
                case 1: // RUN EXPENSES DATA
                    runDH_ExpenseData();
                    break;

                case 2: // RUN TRANSACTION and ALLOWANCE HISTORY
                    runDH_TAHistory();
                    break;

                default:
                    break;
            }
        }
    }
}











/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// BitBudget function to print BitBudget main menu
void print_BitBudgetMM(bool& alert_Notif, bool& alert_LogIn)
{
    string presentDate = getDate_Today();

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
    // Inbox handler
    Inbox inboxHandler;

    // Load necessary data
    Budget budgetHandler;
    DATAandHISTORY DH_Handler;
    budgetHandler.calculateTotalBudget();
    budgetHandler.loadData();

    // Notification Handlers
    Notification MM_NotifsHandler;
    bool alert_NewNotif = true;
    bool SandEL_trackerNotif = false;
    bool alert_NewLogIn = MM_NotifsHandler.createNotification(5, "");

    // User Inputs
    string choice_Str;
    string quitConfirmation;
    int choice_Int;


    while (true)
    {
        // Notification Creators: Savings and Expenses Limit 
        if ((isCurrentTimeInRange("12:00 PM", "11:59 PM"))) {
            budgetHandler.trackDue_SavingsGoal(alert_NewNotif);
            budgetHandler.trackDue_ExpensesLimit(alert_NewNotif);
        }

        // Notification Creators: Reminders(Can be done when online and user happens to be in Main Menu at exactly 10 AM/PM)
        if ((getCurrentTime() == "10:00 AM") || (getCurrentTime() == "10:00 PM")) {
            MM_NotifsHandler.createNotification(1, "");
            alert_NewNotif = true;
        }

        // AUTO GOAL-DELETERS: deletes goals that are long past due date
        budgetHandler.autoDelete_ELGoal(alert_NewNotif);
        budgetHandler.autoDelete_SavingsGoal(alert_NewNotif);
        


        /* ----- Display main menu and ask user which feature to run ------ */
        do {
            // DISPLAY Main Menu
            clearScreen();
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

                // SAVE data and END program if User confirms to quit
                if ((quitConfirmation == "Y") || (quitConfirmation == "y")) {
                    budgetHandler.saveState();
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
                budgetHandler.run_BBUpdate();
                break;
            
            case 2:
                /* INBOX FEATURE */
                inboxHandler.run_Inbox();
                break;
            
            case 3:
                /* DATA & HISTORY FEATURE */
                DH_Handler.runBB_DataAndHistory();
                break;
            
            default:
                break;
        }
    }

    return 0;
}