#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

using namespace std;

class StudyPlanner {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

public:
    StudyPlanner() {
        // Establish connection to MySQL
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "your_username", "your_password");
        con->setSchema("StudyPlanner");
    }

    ~StudyPlanner() {
        delete con;
    }

    void addUser(string name, string preferredStudyTime, int breakInterval) {
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO User (Name, PreferredStudyTime, BreakInterval) VALUES (?, ?, ?)");
        pstmt->setString(1, name);
        pstmt->setString(2, preferredStudyTime);
        pstmt->setInt(3, breakInterval);
        pstmt->executeUpdate();
        delete pstmt;
        cout << "User added successfully.\n";
    }

    void addTask(int userId, string taskName, string deadline, int estimatedTime, int difficultyLevel) {
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO Task (UserID, TaskName, Deadline, EstimatedTime, DifficultyLevel) VALUES (?, ?, ?, ?, ?)");
        pstmt->setInt(1, userId);
        pstmt->setString(2, taskName);
        pstmt->setString(3, deadline);
        pstmt->setInt(4, estimatedTime);
        pstmt->setInt(5, difficultyLevel);
        pstmt->executeUpdate();
        delete pstmt;
        cout << "Task added successfully.\n";
    }

    void viewSchedule(int userId) {
        sql::PreparedStatement *pstmt = con->prepareStatement("SELECT TaskName, StartTime, EndTime, IsCompleted FROM Schedule INNER JOIN Task ON Schedule.TaskID = Task.TaskID WHERE Schedule.UserID = ?");
        pstmt->setInt(1, userId);
        sql::ResultSet *res = pstmt->executeQuery();
        
        cout << "Schedule for User ID " << userId << ":\n";
        while (res->next()) {
            cout << "Task: " << res->getString("TaskName") 
                 << ", Start Time: " << res->getString("StartTime") 
                 << ", End Time: " << res->getString("EndTime") 
                 << ", Completed: " << (res->getBoolean("IsCompleted") ? "Yes" : "No") << endl;
        }
        
        delete res;
        delete pstmt;
    }

    void trackProductivity(int userId, int taskId, int productivityScore, int timeSpent, string date) {
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO Productivity (UserID, TaskID, ProductivityScore, TimeSpent, Date) VALUES (?, ?, ?, ?, ?)");
        pstmt->setInt(1, userId);
        pstmt->setInt(2, taskId);
        pstmt->setInt(3, productivityScore);
        pstmt->setInt(4, timeSpent);
        pstmt->setString(5, date);
        pstmt->executeUpdate();
        delete pstmt;
        cout << "Productivity tracked successfully.\n";
    }
};

int main() {
    StudyPlanner planner;
    
    int choice;
    do {
        cout << "\n1. Add New User\n2. Add New Task\n3. View Schedule\n4. Track Productivity\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, preferredStudyTime;
                int breakInterval;
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Preferred Study Time: ";
                cin >> preferredStudyTime;
                cout << "Enter Break Interval (in minutes): ";
                cin >> breakInterval;
                planner.addUser(name, preferredStudyTime, breakInterval);
                break;
            }
            case 2: {
                int userId, estimatedTime, difficultyLevel;
                string taskName, deadline;
                cout << "Enter User ID: ";
                cin >> userId;
                cout << "Enter Task Name: ";
                cin >> taskName;
                cout << "Enter Deadline (YYYY-MM-DD): ";
                cin >> deadline;
                cout << "Enter Estimated Time (in hours): ";
                cin >> estimatedTime;
                cout << "Enter Difficulty Level (1-5): ";
                cin >> difficultyLevel;
                planner.addTask(userId, taskName, deadline, estimatedTime, difficultyLevel);
                break;
            }
            case 3: {
                int userId;
                cout << "Enter User ID: ";
                cin >> userId;
                planner.viewSchedule(userId);
                break;
            }
            case 4: {
                int userId, taskId, productivityScore, timeSpent;
                string date;
                cout << "Enter User ID: ";
                cin >> userId;
                cout << "Enter Task ID: ";
                cin >> taskId;
                cout << "Enter Productivity Score (1-5): ";
                cin >> productivityScore;
                cout << "Enter Time Spent (in hours): ";
                cin >> timeSpent;
                cout << "Enter Date (YYYY-MM-DD): ";
                cin >> date;
                planner.trackProductivity(userId, taskId, productivityScore, timeSpent, date);
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
