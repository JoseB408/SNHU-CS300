//============================================================================================
// Name: ProjectTwo.cpp
// Author: Jose Bojorquez Garcia
// Version: 1.0
// Class: CS 300, SNHU, Professor Cohen
//============================================================================================
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;

// Struct to hold course information
struct Course {
	string courseId;
	string courseName;
	vector<string> preList;
};

class BinarySearchTree {

private:
	// Define structures to hold courses
	struct Node {
		Course course;
		Node* right;
		Node* left;
		// default constructor
		Node() {
			left = nullptr;
			right = nullptr;
		}
		// initialize with a course
		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course aCourse);
	Course Search(string courseId);
	int Size();
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}

// Traverse the tree in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

// Insert a course
void BinarySearchTree::Insert(Course aCourse) {

	Node* currentNode = root;

	if (root == NULL) {
		root = new Node(aCourse);
	}
	else {
		while (currentNode != NULL) {
			if (aCourse.courseId < currentNode->course.courseId) {
				if (currentNode->left == nullptr) {
					currentNode->left = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->left;
				}
			}
			else {
				if (currentNode->right == nullptr) {
					currentNode->right = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->right;
				}
			}
		}
	}
	size++;
}

// Search for a course
Course BinarySearchTree::Search(string courseId) {
	Course aCourse;
	Node* currentNode = root;
	while (currentNode != NULL) {
		if (currentNode->course.courseId == courseId) {
			return currentNode->course;
		}
		else if (courseId < currentNode->course.courseId) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}


	return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
	if (node == NULL) {
		return;
	}
	inOrder(node->left);
	// print the node
	cout << node->course.courseId << ", " << node->course.courseName << endl;

	inOrder(node->right);
}

int BinarySearchTree::Size() {
	return size;
}

vector<string> Split(string lineFeed) {

	char delim = ',';

	lineFeed += delim; // includes a delimeter at the end so last word is also read
	vector<string> lineTokens;
	string temp = "";
	for (int i = 0; i < lineFeed.length(); i++) {
		if (lineFeed[i] == delim) {
			lineTokens.push_back(temp); // store words in token vector
			temp = "";
			i++;
		}
		temp += lineFeed[i];
	}
	return lineTokens;
}

// Function to load courses
void loadCourses(string csvPath, BinarySearchTree* courseList) {
	// Create a data structure and add to the collection of courses

	ifstream inFS; // instream to read file
	string line; // line feed
	vector<string> stringTokens;

	inFS.open(csvPath); // open the read file

	if (!inFS.is_open()) { //small error handler
		cout << "Could not open file. Please check inputs." << endl;
		return;
	}

	while (!inFS.eof()) {
		// Create a new struct for each "line"
		Course aCourse; 

		getline(inFS, line);
		stringTokens = Split(line); // Split the line into tokens via the delimeter
		// If there aren't 2 tokens, the line is misformatted
		if (stringTokens.size() < 2) { 
			cout << endl;
		}
		else {
			aCourse.courseId = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (unsigned int i = 2; i < stringTokens.size(); i++) {
				aCourse.preList.push_back(stringTokens.at(i));
			}
			// push this course to the end
			courseList->Insert(aCourse);
		}
	}
	inFS.close();
}

// Function that prints the courseId, courseName, prerequsites
void displayCourse(Course aCourse) {
	
	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";

	if (aCourse.preList.empty()) { // if the list is empty then there are no prerequisites
		cout << "none" << endl;
	}
	else {
		for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
			cout << aCourse.preList.at(i);
			if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) { // puts a comma for any elements greater than 1
				cout << ", ";
			}
		}
	}
	cout << endl;
}

// Function that forces the case of any string to uppercase, and passes the string by references and changes the case of any alpha to upper
void convertCase(string &toConvert) {
	for (unsigned int i = 0; i < toConvert.length(); i++) {
		if (isalpha(toConvert[i])) {
			toConvert[i] = toupper(toConvert[i]);
		}
	}
}

int main(int argc, char* argv[]) {
	// process command line arguments
	string csvPath, aCourseKey;

	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
	}
	// Define a table to hold all the courses
	BinarySearchTree* courseList = new BinarySearchTree();

	Course course;
	bool goodInput;
	int choice = 0;

	while (choice != 9) {
		cout << "Welcome to the course planner." << endl;
		cout << endl;
		cout << " 1. Load Data Structure." << endl;
		cout << " 2. Print Course List." << endl;
		cout << " 3. Print Course." << endl;
		cout << " 9. Exit" << endl;
		cout << "What would you like to do? ";

		aCourseKey = ""; // clear the string
		string anyKey = " "; // clear the string
		choice = 0; // clear the choice

		try {
			cin >> choice;

			if ((choice > 0 && choice < 5) || (choice == 9)) {
				goodInput = true;
			}
			else { // throw error for catch
				goodInput = false;
				throw 1;
			}

			switch (choice) {
			case 1:
				// Complete the method call to load the courses
				loadCourses(csvPath, courseList);
				cout << courseList->Size() << " courses read." << endl;

				Sleep(GLOBAL_SLEEP_TIME);

				break;
				
			case 2:

				cout << "\nHere is a sample schedule:" << endl;

				cout << endl;

				courseList->InOrder();

				cout << "\nEnter \'any key\' to continue..." << endl;

				cin >> anyKey;

				break;

			case 3:

				cout << "\nWhat course do you want to know about? " << endl;
				cin >> aCourseKey;

				convertCase(aCourseKey); // convert the case of user input

				course = courseList->Search(aCourseKey);

				if (!course.courseId.empty()) {
					displayCourse(course);
				}
				else {
					cout << "\nCourse ID " << aCourseKey << " not found." << endl;
				}
				Sleep(GLOBAL_SLEEP_TIME);

				break;

			case 9:
				exit;
				break;

			default:

				throw 2;
			}
		}

		catch (int err) {
			std::cout << choice << " is not a valid option." << endl;
			Sleep(GLOBAL_SLEEP_TIME);
		}
		// need to clear the cin operator of extra input or any errors generated by bad input
		cin.clear();
		cin.ignore();
	}
	
	cout << "Thank you for using the course planner!" << endl;

	Sleep(GLOBAL_SLEEP_TIME);

	return 0;
}