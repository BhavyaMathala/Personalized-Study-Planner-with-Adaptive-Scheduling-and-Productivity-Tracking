-- Create the database
CREATE DATABASE StudyPlanner;

-- Use the created database
USE StudyPlanner;

-- Create the User table
CREATE TABLE User (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    PreferredStudyTime VARCHAR(50) NOT NULL,
    BreakInterval INT NOT NULL
);

-- Create the Task table
CREATE TABLE Task (
    TaskID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT,
    TaskName VARCHAR(100) NOT NULL,
    Deadline DATE NOT NULL,
    EstimatedTime INT NOT NULL,
    DifficultyLevel INT CHECK (DifficultyLevel BETWEEN 1 AND 5),
    FOREIGN KEY (UserID) REFERENCES User(UserID) ON DELETE CASCADE
);

-- Create the Schedule table
CREATE TABLE Schedule (
    ScheduleID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT,
    TaskID INT,
    StartTime TIME NOT NULL,
    EndTime TIME NOT NULL,
    IsCompleted BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (UserID) REFERENCES User(UserID) ON DELETE CASCADE,
    FOREIGN KEY (TaskID) REFERENCES Task(TaskID) ON DELETE CASCADE
);

-- Create the Productivity table
CREATE TABLE Productivity (
    ProductivityID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT,
    TaskID INT,
    ProductivityScore INT CHECK (ProductivityScore BETWEEN 1 AND 5),
    TimeSpent INT NOT NULL,
    Date DATE NOT NULL,
    FOREIGN KEY (UserID) REFERENCES User(UserID) ON DELETE CASCADE,
    FOREIGN KEY (TaskID) REFERENCES Task(TaskID) ON DELETE CASCADE
);
