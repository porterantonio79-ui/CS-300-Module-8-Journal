// CS-300 Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Course class to store course information
class Course {
private:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

public:
    // Default constructor
    Course() {}

    // Parameterized constructor
    Course(string number, string title) {
        courseNumber = number;
        courseTitle = title;
    }

    // Getters
    string getCourseNumber() const { return courseNumber; }
    string getCourseTitle() const { return courseTitle; }
    vector<string> getPrerequisites() const { return prerequisites; }

    // Add prerequisite
    void addPrerequisite(string prereq) {
        prerequisites.push_back(prereq);
    }

    // Display course information
    void displayCourseInfo(const vector<Course>& allCourses) const {
        cout << courseNumber << ", " << courseTitle << endl;
        if (!prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < prerequisites.size(); ++i) {
                string prereqNumber = prerequisites[i];

                // Find and display prerequisite title
                bool found = false;
                for (const auto& course : allCourses) {
                    if (course.getCourseNumber() == prereqNumber) {
                        cout << prereqNumber << ", " << course.getCourseTitle();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << prereqNumber << ", Title not found";
                }

                if (i < prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        else {
            cout << "No prerequisites" << endl;
        }
    }
};

// Function to trim whitespace from string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == string::npos || end == string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Function to load data from file into data structure
vector<Course> loadDataStructure(const string& filename) {
    vector<Course> courses;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return courses;
    }

    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        lineCount++;

        // Skip empty lines
        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split line by commas
        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() >= 2) {
            // Create new course with number and title
            Course course(tokens[0], tokens[1]);

            // Add prerequisites if any
            for (size_t i = 2; i < tokens.size(); ++i) {
                if (!tokens[i].empty()) {
                    course.addPrerequisite(tokens[i]);
                }
            }

            courses.push_back(course);
        }
        else {
            cout << "Warning: Invalid format on line " << lineCount << ": " << line << endl;
        }
    }

    file.close();

    if (courses.empty()) {
        cout << "No valid course data found in file." << endl;
    }
    else {
        cout << "Data loaded successfully. " << courses.size() << " courses loaded." << endl;
    }

    return courses;
}

// Function to compare courses for sorting (case-insensitive)
bool compareCourses(const Course& a, const Course& b) {
    string aNum = a.getCourseNumber();
    string bNum = b.getCourseNumber();

    // Convert to uppercase for case-insensitive comparison
    transform(aNum.begin(), aNum.end(), aNum.begin(), ::toupper);
    transform(bNum.begin(), bNum.end(), bNum.begin(), ::toupper);

    return aNum < bNum;
}

// Function to print course list in alphanumeric order
void printCourseList(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses loaded. Please load data first (Option 1)." << endl;
        return;
    }

    // Sort courses alphanumerically
    sort(courses.begin(), courses.end(), compareCourses);

    cout << "\nHere is a sample schedule:\n" << endl;
    for (const auto& course : courses) {
        cout << course.getCourseNumber() << ", " << course.getCourseTitle() << endl;
    }
    cout << endl;
}

// Function to print course information
void printCourseInformation(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses loaded. Please load data first (Option 1)." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    // Convert to uppercase for case-insensitive comparison
    string courseNumberUpper = courseNumber;
    transform(courseNumberUpper.begin(), courseNumberUpper.end(), courseNumberUpper.begin(), ::toupper);

    bool found = false;
    for (const auto& course : courses) {
        string currentCourseNumber = course.getCourseNumber();
        transform(currentCourseNumber.begin(), currentCourseNumber.end(), currentCourseNumber.begin(), ::toupper);

        if (currentCourseNumber == courseNumberUpper) {
            cout << endl;
            course.displayCourseInfo(courses);
            cout << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Course '" << courseNumber << "' not found." << endl;
    }
}

// Function to display menu
void displayMenu() {
    cout << "\nWelcome to the course planner." << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    vector<Course> courses;
    string filename;
    int choice = 0;

    cout << "ABCU Computer Science Department Course Planner" << endl;
    cout << "==============================================" << endl;

    while (choice != 9) {
        displayMenu();
        cin >> choice;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number (1, 2, 3, or 9)." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter filename: ";
            cin >> filename;
            courses = loadDataStructure(filename);
            break;

        case 2:
            printCourseList(courses);
            break;

        case 3:
            printCourseInformation(courses);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            cout << "Please choose 1, 2, 3, or 9." << endl;
            break;
        }
    }

    return 0;
}