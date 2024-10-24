// ====================
// James Lightner
// 10/16/2024
// CS-300: Project Two
//======================

# include <iostream>
# include <iomanip>
# include <string>
# include <vector>
# include <sstream>
# include <fstream>

using namespace std;

// Creating Course Struct
 
struct Course {
    string courseNum;
    string courseName;

    vector<string> PreReqs;

    //empty constructor
    Course() {};
    //constructor with 2 values
    Course(string aCourseNum, string aCourseName) {
        courseNum = aCourseNum;
        courseName = aCourseName;
    }
    //constructor with prereqs
    Course(string aCourseNum, string aCourseName, vector<string> prereqs) {
        courseNum = aCourseNum;
        courseName = aCourseName;
        vector<string> PreReqs = prereqs;
    }
};


// Creating Node Struct

struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course acourse) : Node() {
        this->course = acourse;
    }
};


// BST Class

class BST {
private:
    Node* root;
    void addNode(Course acourse, Node* node);
    void inOrder(Node* node);
    void printCourse(Node* node, string courseNum);

public:
    BST();
    void InOrder();
    void PrintCourse(string courseNum);
    bool loadData();
    bool validatePreReqs(Course course);
    void insert(Course course);
};

//default constructor for bst
BST::BST() {
    root = nullptr;
}

//public in order (passes root to private in order)
void BST::InOrder() {
    this->inOrder(root);
}

//passes root node to print the course
void BST::PrintCourse(string courseNum) {
    this->printCourse(root, courseNum);
}

/// Inserting the New Node

void BST::insert(Course course) {
    //if the root is empty
    if (root == nullptr) {
        root = new Node(course);
    }
    //tree has been started, call addnode, pass root
    else {
        this->addNode(course, root);
    }
}


// Adding the Node to the Tree

void BST::addNode(Course acourse, Node* node) {

    //if current node is larger than passed courseNum
    if (node->course.courseNum.compare(acourse.courseNum) > 0) {
        //if there isn't a left node
        if (node->left == nullptr) {
            node->left = new Node(acourse);
        }
        //recurse down the left side
        else {
            this->addNode(acourse, node->left);
        }
    }
    //if current node is smaller than the passed courseNum
    else {
        //if there isn't a right node
        if (node->right == nullptr) {
            node->right = new Node(acourse);
        }
        //recurse down the right side
        else {
            this->addNode(acourse, node->right);
        }
    }
}

// Print Courses In Order: private

void BST::inOrder(Node* node) {

    if (node != nullptr) {
        //recurse down the left side
        inOrder(node->left);
        //print the course information at the current node
        cout << "Course Number: " << node->course.courseNum;
        cout << " Course Name: " << node->course.courseName;
        cout << " Prerequisite(s): ";
        //iterate through course's prerequisite vector to print
        for (int i = 0; i < node->course.PreReqs.size(); ++i) {
            cout << node->course.PreReqs[i] << " ";
        }
        if (node->course.PreReqs.size() == 0) {
            cout << "None";
        }
        cout << "\n";

        //recurse down the right side
        inOrder(node->right);
    }
    return;
}

// Printing Individual Course: private

void BST::printCourse(Node* node, string courseNum) {

    while (node != nullptr) {
        //if there is a match
        if (node->course.courseNum.compare(courseNum) == 0) {
            cout << "Course Number: " << node->course.courseNum;
            cout << "  Course Name: " << node->course.courseName;
            cout << "  Prerequisite(s): ";
            //iterate through prerequisite vector to print
            for (int i = 0; i < node->course.PreReqs.size(); ++i) {
                cout << node->course.PreReqs[i] << " ";
            }
            //if there isn't prerequisites
            if (node->course.PreReqs.size() == 0) {
                cout << "None";
            }
            cout << "\n";
            return;
        }
        //if current course number is smaller than the one passed 
        else if (node->course.courseNum.compare(courseNum) > 0 && node->left != nullptr) {
            node = node->left;
        }
        //if current course number is larger than the one passed 
        else if (node->course.courseNum.compare(courseNum) < 0 && node->right != nullptr) {
            node = node->right;
        }
        else {
            cout << "Course not found. Please try again. \n";
            return;
        }
    }
    //error if the tree cannot load
    if (root == nullptr) {
        cout << "Error loading data. =[ \n";
        return;
    }
}

// Load Data (from file into bst)

bool BST::loadData() {

    //use ifstream to open file
    ifstream file;
    file.open("ProjectCourseFile.txt");

    if (file.is_open()) {
        //while end of file is not reached
        while (!file.eof()) {
            //create a vector to hold line items
            vector<string> courseLine;
            //to parse line into vector
            string line;

            //get line from file and assign to 'line'
            getline(file, line);
            //while line is not empty
            while (line.length() > 0) {
                unsigned int delim = line.find(',');
                //limits data size to 99 or less characters
                if (delim < 100) {
                    //add item to courseLine, then delete
                    courseLine.push_back(line.substr(0, delim));
                    line.erase(0, delim + 1);
                }
                //add item following last comma, then clear line
                else {
                    courseLine.push_back(line.substr(0, line.length()));
                    line = "";
                }
            }

            //local course object (to be filled and passed to insert)
            Course course;
            //assign course number and name

            course.courseNum = courseLine[0];
            course.courseName = courseLine[1];
            //push items 3+ into prereq vector
            for (unsigned int i = 2; i < courseLine.size(); i++) {
                course.PreReqs.push_back(courseLine[i]);
            }
            //insert the course into bst
            insert(course);
        }
        return true;
    }
    if (!file) {
        cout << "Could not open file. Please try again.  \n";
        return false;
    }
    file.close();
}

bool BST::validatePreReqs(Course course) {
    //create local node and set to root
    Node* current = root;
    if (current->course.PreReqs.size() > 0) {
        //iterate through prerequisite vector
        for (int i = 0; i < course.PreReqs.size(); i++) {
            while (current != nullptr) {
                //if match is found
                if (course.PreReqs[i] == current->course.courseNum) {
                    return true;
                }
                //if course number is smaller than prerequisite
                else if (course.PreReqs[i].compare(current->course.courseNum) > 0 && current->left != nullptr) {
                    current = current->left;
                }
                //if course number is larger than prerequisite
                else if (course.PreReqs[i].compare(current->course.courseNum) < 0 && current->right != nullptr) {
                    current = current->right;
                }
                //if corresponding course number is not found
                else {
                    cout << "Prerequisite not found. \n";
                    return false;
                }
            }
        }
    }
    else {
        //no prerequisites
        return true;
    }
}

int main()
{
    //create bst 
    BST* bst = new BST();
    //create a string variable to hold input (for case 3)
    string courseNum;
    //set choice to 0
    int choice = 0;

    cout << "Welcome to ABCU course scheduler \n\n";
    //Menu loop that will iterate until user enters 4
    while (choice != 4) {
        cout << "**_____________________________**\n";
        cout << "||            MENU             ||\n";
        cout << "||-----------------------------||\n";
        cout << "||[1] Load Data Structure      ||\n";
        cout << "||[2] Print Course List        ||\n";
        cout << "||[3] Print Course             ||\n";
        cout << "||[4] Exit                     ||\n";
        cout << "||_____________________________||\n";
        cout << "Please enter your option ";
        cin >> choice;

        //menu selections
        switch (choice) {
            //load data from file
        case 1:
            bst->loadData();
            break;
            //print sorted course information
        case 2:
            cout << "\nHere is a sample schedule: \n";
            cout << "---------------------------- \n";
            bst->InOrder();
            cout << "\n";
            break;
            //search for a specific course to display
        case 3:
            cout << "Please enter the course number you would like to search: ";
            cin >> courseNum;
            bst->PrintCourse(courseNum);
            cout << "\n";
            break;
            //say goodbye, the user is leaving now
        case 4:
            cout << "\nThank you for using the course planner!\n";
            break;
            //for invalid input
        default:
            cout << choice << " is not a valid option. Please enter a number 1 - 4.\n";
        }
    }
    return 0;
}