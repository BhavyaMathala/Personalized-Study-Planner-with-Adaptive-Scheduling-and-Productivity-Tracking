#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <string>

using namespace std;

class StudyPlanner {
public:
    StudyPlanner();
    void showMenu();

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    void setupDatabase();
    void addUser();
    void addTask();
    void viewSchedule();
};

StudyPlanner::StudyPlanner() {
    // Establish connection to MySQL
    setupDatabase();
}

void StudyPlanner::setupDatabase() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "your_username", "your_password");
    con->setSchema("StudyPlanner");
}

void StudyPlanner::addUser() {
    string name;
    string preferredStudyTime;
    int breakInterval;

    cout << "Enter User Name: ";
    cin >> name;
    cout << "Enter Preferred Study Time: ";
    cin >> preferredStudyTime;
    cout << "Enter Break Interval (min): ";
    cin >> breakInterval;

    sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO User (Name, PreferredStudyTime, BreakInterval) VALUES (?, ?, ?)");
    pstmt->setString(1, name);
    pstmt->setString(2, preferredStudyTime);
    pstmt->setInt(3, breakInterval);
    pstmt->executeUpdate();
    delete pstmt;

    cout << "User added successfully." << endl;
}

void StudyPlanner::addTask() {
    int userId;
    string taskName;
    string deadline;
    int estimatedTime;
    int difficultyLevel;

    cout << "Enter User ID: ";
    cin >> userId;
    cout << "Enter Task Name: ";
    cin >> taskName;
    cout << "Enter Deadline (YYYY-MM-DD): ";
    cin >> deadline;
    cout << "Enter Estimated Time (hrs): ";
    cin >> estimatedTime;
    cout << "Enter Difficulty Level (1-5): ";
    cin >> difficultyLevel;

    sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO Task (UserID, TaskName, Deadline, EstimatedTime, DifficultyLevel) VALUES (?, ?, ?, ?, ?)");
    pstmt->setInt(1, userId);
    pstmt->setString(2, taskName);
    pstmt->setString(3, deadline);
    pstmt->setInt(4, estimatedTime);
    pstmt->setInt(5, difficultyLevel);
    pstmt->executeUpdate();
    delete pstmt;

    cout << "Task added successfully." << endl;
}

void StudyPlanner::viewSchedule() {
    // Fetch and display the schedule from the database
    sql::PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM Task");
    sql::ResultSet *res = pstmt->executeQuery();
    
    cout << "Current Schedule:" << endl;
    while (res->next()) {
        cout << "UserID: " << res->getInt("UserID")
             << ", Task: " << res->getString("TaskName")
             << ", Deadline: " << res->getString("Deadline")
             << ", Estimated Time: " << res->getInt("EstimatedTime")
             << " hrs, Difficulty: " << res->getInt("DifficultyLevel") << endl;
    }
    delete res;
    delete pstmt;
}

void StudyPlanner::showMenu() {
    int choice;
    do {
        cout << "\n--- Personalized Study Planner ---" << endl;
        cout << "1. Add User" << endl;
        cout << "2. Add Task" << endl;
        cout << "3. View Schedule" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                addTask();
                break;
            case 3:
                viewSchedule();
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);
}

int main() {
    StudyPlanner planner;
    planner.showMenu();
    return 0;
}
