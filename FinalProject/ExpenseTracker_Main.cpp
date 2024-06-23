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

using namespace std;








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

/* DISPLAY FUNCTIONS */
//
void border(char, int);
void displayCenteredLine_NoColor(const string, int);
void displayCenteredLine_Colored(const string, const string, int);
void displayCenteredLine_NoNewLine(const string, const string, int);
void clearScreen();


/* MAIN MENU FUNCTIONS */
// 
void print_BitBudgetMM(bool&, bool&);
void print_QuitMenu();


/* INBOX FUNCTIONS */
// 
void inbox_Options(int);
void inbox_Title();
void run_INBOX();

void clearFile(string);
bool strIsNumeric(string);


/* DATE-RELATED FUNCTIONS */
// 
bool validateDateFormat(const string &);
bool validateDate(const string &);
bool validateSecondDate(const string &, const string &);

string getDate_Today();





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







/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*------------------------------------------------------------- BITBUDGET: UPDATE FUNCTIONS ----------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/

/*________________________________________________________________________________*/
/*----------------------- CLASS DEF'N FOR EXPENSE & BUDGET -----------------------*/
/*________________________________________________________________________________*/
















/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*----------------------------------------------------------- BITBUDGET MAIN MENU: FUNCTIONS ---------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/

// print BITBUDGET main menu
void print_BitBudgetMM(bool& alert_Notif, bool& alert_LogIn) {
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

// print QUIT Menu
void print_QuitMenu() {
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










/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*------------------------------------------------------------- BITBUDGET: INBOX FUNCTIONS -----------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/


/* ---------------- NOTIFICATIONS: CLASS DEFINITION AND FUNCTIONS ------------------ */
/*-----------------------------------------------------------------------------------*/

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
            inFILE.read(&notif[0], sizeof(notif));
            NotificationList.push_back(notif);
        }

        inFILE.close();
    }
    else {
        displayCenteredLine_Colored(">> ERROR: Unable to read Inbox File", YELLOW);
        // displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue... ", YELLOW);
        // getchar();
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
            outFILE.write(notifs.c_str(), sizeof(notifs));
        }

        outFILE.close();
    }
    else {
        displayCenteredLine_Colored(">> ERROR: Unable to read Inbox File", YELLOW);
        // displayCenteredLine_NoNewLine(">> Press 'ENTER' to continue... ", YELLOW);
        // getchar();
    }
}





// DISPLAY notifications by page
void Inbox :: displayNotifications(int page = 1, int msgPerPage = 4)
{
    if (NotificationList.size() > 0)
    {
        int total_Notifs = NotificationList.size();
        int total_Pages = NotificationList.size() / msgPerPage;

        if ((total_Notifs % msgPerPage) != 0) total_Pages++;
        if (page < 1) page = 1;

        int msgIndex = (page - 1) * msgPerPage;
        int msg_LastIndex = min(msgIndex + msgPerPage, total_Notifs);

        for (int i = msgIndex; i < msg_LastIndex; i++)
        {
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
    string index_Str;
    int index_Int;

    int page = 1;
    int maxPages = NotificationList.size()/4;
    if ((NotificationList.size() % 4) != 0) maxPages++;

    loadNotifications();


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
            displayCenteredLine_Colored("OPTIONS\n", BOLDWHITE);
            displayCenteredLine_NoColor("[ 1 ]  Previous Page       [ 3 ]  DELETE");
            displayCenteredLine_NoColor("[ 2 ]  Next Page           [ 4 ]  CLEAR ");
            displayCenteredLine_NoColor("             [ R ]  Return              ");
            cout << "\n";
            displayCenteredLine_NoNewLine(">> Enter choice: ", CYAN);

            // Ask for input
            getline(cin, choice_Str);

            if ((choice_Str == "R") || (choice_Str == "r")) {
                saveNotifications();
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
                    displayCenteredLine_Colored("OPTIONS: DELETE\n", BOLDWHITE);
                    displayCenteredLine_NoColor(">> Enter a number of the message you want to delete.");
                    displayCenteredLine_NoColor(">> Enter 'R' to go back.                            ");
                    cout << "\n";
                    displayCenteredLine_NoNewLine(">> Enter valid input: ", CYAN);

                    // Get input from user
                    getline(cin, index_Str);

                    // Return to INBOX Menu
                    if ((index_Str == "R") || (index_Str == "r")) break;

                    // Perform DELETE
                    else if (strIsNumeric(index_Str))
                    {
                        index_Int = stoi(index_Str);

                        if ((index_Int > 0) && (index_Int < NotificationList.size()))
                        {
                            // Perform: DELETE Notification
                            deleteNotif(index_Int);
                            clearScreen();

                            // Display: INBOX TITLE
                            border(205);
                            displayCenteredLine_Colored("BITBUDGET: INBOX", BOLDGREEN);
                            border(205);

                            // Display: Notifications
                            displayNotifications(page);

                            // Notify user, Notification successfully deleted
                            displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
                            displayCenteredLine_Colored(">> Message deleted SUCCESSFULLY!", YELLOW);
                            displayCenteredLine_Colored(">> Press 'ENTER' to continue... ", YELLOW);
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
                            displayCenteredLine_Colored(">> Message not found...         ", YELLOW);
                            displayCenteredLine_Colored(">> Press 'ENTER' to continue... ", YELLOW);
                        }
                    }
                }
                break;

            case 4:
                break;

            default:
                break;

        }
    }
}










/*-----------------------------------------------------------------------------------*/
/* ---------------- NOTIFICATIONS: CLASS DEFINITION AND FUNCTIONS ------------------ */
/*-----------------------------------------------------------------------------------*/

/* CLASS definition for NOTIFICATIONS */
class Notification {
private:
    string reminder =               "   (OvO)/  Spent anything today? Update your tracker now! \n    Go to UPDATE and record today's transaction\n";
    string warning =                "   (O-O)!  Uh oh... You have exceeded your expense limit!\n    Better tone down the spending when possible!\n";
    string progressReport_Pos =     "   (>u<)/  Your progress is looking great!\n    Take a look at DATA & HISTORY and see how you did.\n";
    string progressReport_Neg  =    "   (O-O)!  Oh no... Your progress is not looking great... \n    Take a look at DATA & HISTORY and see why. \n   Try to limit your expenses whenever possible (;-;)\n";
    string loginNotif =             "   (OwO)/ Hello! Welcome to BitBudget: Expense Tracker!\n    Get started by UPDATEing XD\n";


public:
    bool createNotification(int);
};


// Notification MF: Create NEW NOTIFICATION and write in file: 1 - Reminder, 2 - Warning, 3 - Progress report: Positive, 4 - Progress report, 5 - Log in notification
bool Notification:: createNotification(int mode)
{
    string current_Time = __TIME__;

    // Create new notification based on mode
    string NewNotif = BOLDWHITE + getDate_Today() + "; " + current_Time + RESET + "\n";
    switch (mode) {
        case 1:
            NewNotif += reminder;
            break;
        case 2:
            NewNotif += warning;
            break;
        case 3:
            NewNotif += progressReport_Pos;
            break;            
        case 4:
            NewNotif += progressReport_Neg;
            break;
        case 5:
            NewNotif += loginNotif;
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
    else {
        // Notify User if file failed to open
        // ERROR [1]: Notify User that file failed to be opened
            displayCenteredLine_Colored("ERROR [1]", BOLDRED);
            cout << "\n";

            displayCenteredLine_Colored(">> Unable to open file.", YELLOW);
            displayCenteredLine_NoNewLine("Press 'ENTER' to continue...   ", YELLOW);
            getchar();
        return false;
    }


    return false;
}












/*___________________________________________________________________________________*/
/*--------------------------- DISPLAY FUNCTIONS FOR INBOX ---------------------------*/
/*___________________________________________________________________________________*/

//         case 2:
//             /* INBOX feature #4 */
//             displayCenteredLine_Colored(">> Do you want to CLEAR your inbox?\n", YELLOW);
//             cout << "\n\n\n\n\n";
//             border(196);
//             displayCenteredLine_Colored("OPTIONS: DELETE\n", BOLDWHITE);
//             displayCenteredLine_NoColor("[ Y ]  YES        ");
//             displayCenteredLine_NoColor("[ N ]  NO, Return\n");
//             displayCenteredLine_NoNewLine(">> Enter choice: ", CYAN);
//             break;














/*___________________________________________________________________________________*/
/*---------------------------------- INBOX FEATURE ----------------------------------*/
/*___________________________________________________________________________________*/
//             case 4:
//                 /* CLEAR INBOX */
//                 while (true) {
//                     // Display CLEAR INBOX menu
//                     clearScreen();
//                     inbox_Title();
//                     inbox_Options(2);

//                     // Ask user for confirmation
//                     getline(cin, input_Str);

//                     // CLEAR file if confirmed Y/y
//                     if ((input_Str == "Y") || (input_Str == "y")) {
//                         clearFile(InboxFILE);

//                         if (notifs_Handler.getTotalNotifs() == 0) {
//                             // Notify user that action CLEAR INBOX was successful
//                             clearScreen();
//                             inbox_Title();

//                             displayCenteredLine_Colored("NOTICE", BOLDYELLOW);
//                             cout << "\n";

//                             displayCenteredLine_Colored("INBOX successfully cleared.", YELLOW);
//                             displayCenteredLine_NoNewLine("Press 'ENTER' to return to INBOX menu...   ", YELLOW);
//                             getchar();
//                             break;
//                         }
//                         else {
//                             // ERROR [2]: Notify User that file failed to opened
//                             clearScreen();
//                             inbox_Title();

//                             displayCenteredLine_Colored("ERROR [3]", BOLDRED);
//                             cout << "\n";

//                             displayCenteredLine_Colored("Unable to open file & delete notifications.", YELLOW);
//                             displayCenteredLine_NoNewLine("Press 'ENTER' to return to INBOX menu...   ", YELLOW);
//                             getchar();
//                             break;
//                         }
//                     }
//                     // Return to INBOX menu if input is N/n
//                     else if ((input_Str == "N") || (input_Str == "n")) {
//                         break;
//                     }
//                 }
//                 break;
            

//             default:
//                 /* Perform nothing */
//                 break;
//         }
//     }
// }










/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*--------------------------------------------------------- BITBUDGET: DATA & HISTORY FUNCTIONS ------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________________________________________________________*/

// 











/*________________________________________________________________________________________________________________________________________________________________*/
/*------------------------------------------------------------------ EXTRA FUNCTIONS -----------------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/
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
bool strIsNumeric(string str) {
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9')) {
            return false;
        }
    }

    return true;
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











/*________________________________________________________________________________________________________________________________________________________________*/
/*---------------------------------------------------------- BITBUDGET MAIN MENU: MAIN FUNCTION ------------------------------------------------------------------*/
/*________________________________________________________________________________________________________________________________________________________________*/

int main()
{
    Notification MM_NotifsHandler;
    Inbox inboxHandler;

    bool alert_NewNotif = true;
    bool alert_NewLogIn = MM_NotifsHandler.createNotification(5);

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