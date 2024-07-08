#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

class Expense {
public:
    Expense(const std::string& date, double amount)
        : date(date), amount(amount) {}

    std::string getDate() const { return date; }
    double getAmount() const { return amount; }

private:
    std::string date;
    double amount;
};

class Goal {
public:
    Goal(const std::string& startDate, const std::string& dueDate)
        : startDate(startDate), dueDate(dueDate), currentAmt(0) {}

    std::string get_startDate() const { return startDate; }
    std::string get_dueDate() const { return dueDate; }
    double get_currentAmt() const { return currentAmt; }

    void set_currentAmt(double amt) { currentAmt = amt; }

private:
    std::string startDate;
    std::string dueDate;
    double currentAmt;
};

class Budget {
public:
    void addExpenseToday(const Expense& expense) { expensesList_Today.push_back(expense); }
    void addExpense(const Expense& expense) { expensesList.push_back(expense); }
    void addGoal(const Goal& goal) { expenseLimitsList.push_back(goal); }

    void trackExpenses();

private:
    tm parseDate(const std::string& date);
    bool isDateInRange(const std::string& dateStr, const std::string& startStr, const std::string& endStr);

    std::vector<Expense> expensesList_Today;
    std::vector<Expense> expensesList;
    std::vector<Goal> expenseLimitsList;
};

// Function to parse date in MM/DD/YYYY format. Returns tm class
tm Budget::parseDate(const std::string& date) {
    tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%m/%d/%Y");
    return tm;
}

// Function to compare two dates
bool Budget::isDateInRange(const std::string& dateStr, const std::string& startStr, const std::string& endStr) {
    tm date = parseDate(dateStr);
    tm startDate = parseDate(startStr);
    tm endDate = parseDate(endStr);

    time_t dateT = mktime(&date);
    time_t startT = mktime(&startDate);
    time_t endT = mktime(&endDate);

    return (dateT >= startT && dateT <= endT);
}

void Budget::trackExpenses() {
    double amt;

    for (auto& goal : expenseLimitsList) {
        amt = 0;

        // Iterate over today's expenses
        for (auto& expense : expensesList_Today) {
            if (isDateInRange(expense.getDate(), goal.get_startDate(), goal.get_dueDate())) {
                amt += expense.getAmount();
            }
        }

        // Iterate over all expenses
        for (auto& expense : expensesList) {
            if (isDateInRange(expense.getDate(), goal.get_startDate(), goal.get_dueDate())) {
                amt += expense.getAmount();
            }
        }

        // Update the current amount spent in the goal
        goal.set_currentAmt(amt);

        // Debug statement to check goal and amount
        std::cout << "Goal from " << goal.get_startDate() << " to " << goal.get_dueDate()
                  << " updated with amount: " << amt << std::endl;
    }
}

int main() {
    Budget myBudget;

    // Add some goals
    myBudget.addGoal(Goal("01/01/2024", "01/31/2024"));
    myBudget.addGoal(Goal("02/01/2024", "02/28/2024"));

    // Add some expenses
    myBudget.addExpenseToday(Expense("01/05/2024", 50.0));
    myBudget.addExpenseToday(Expense("01/15/2024", 75.0));
    myBudget.addExpense(Expense("01/20/2024", 100.0));
    myBudget.addExpense(Expense("02/10/2024", 150.0));

    // Track expenses and update goals
    myBudget.trackExpenses();

    return 0;
}
