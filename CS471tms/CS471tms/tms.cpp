#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // for std::sort

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;
using std::vector;

void displayMainMenu();

class Task
{
public:
	string taskName;
	string description;
	int priority;
	string dependance;
	int dueDate;
	bool working;
	string workingAnswer;
	int talloc;
	bool archived;
	string archivedAnswer;
};

//for use in std::sort when sorting tasks in a vector
//by priority
struct ComparePriority
{
	bool operator()(Task & a, Task & b)
	{
		return a.priority < b.priority;
	}
};

vector <Task> TaskList;

void loadTasks()
{
	//open tasklist.txt and go though it to populate the vector of current tasks
	Task tempTask;
	vector<Task> loadedTasks;
	ifstream taskFile("tasklist.txt");
	string line;
	string attributeIdentifier;
	
	//Get a line from the file 
	while (getline(taskFile, line))
	{
		attributeIdentifier.clear();
		attributeIdentifier += line[0];
		attributeIdentifier += line[1];
		attributeIdentifier += line[2];
		if (attributeIdentifier == "NAM")
		{
			line.erase(line.begin(), line.begin() + 5); //Name: is 5 characters long save the rest of the line for Task object
			tempTask.taskName = line;
		}
		else if (attributeIdentifier == "DES")
		{
			line.erase(line.begin(), line.begin() + 11); //DESCRIPTION: is 11 characters long
			tempTask.description = line;
		}
		else if (attributeIdentifier == "PRI")
		{
			line.erase(line.begin(), line.begin() + 9); // PRIORITY: is 9 characters long
			tempTask.priority = atoi(line.c_str());
		}
		else if (attributeIdentifier == "DEP")
		{
			line.erase(line.begin(), line.begin() + 11);  // DEPENDENCE: is 11 characters long
			tempTask.dependance = line;
		}
		else if (attributeIdentifier == "DUE")
		{
			line.erase(line.begin(), line.begin() + 8); //DUEDATE: is 8 characters long
			tempTask.dueDate = atoi(line.c_str());
		}
		else if (attributeIdentifier == "WOR")
		{
			line.erase(line.begin(), line.begin() + 10); //WORKINGON: is 10 characters long
			if (line == "t")
			{
				tempTask.working = true;
			}
			else
			{
				tempTask.working = false;
			}
		}
		else if (attributeIdentifier == "TIM")
		{
			line.erase(line.begin(), line.begin() + 10 ); //TIMEALLOC: is 10 characters long
			tempTask.talloc = atoi(line.c_str());
		}
		else if (attributeIdentifier == "ARC")
		{
			line.erase(line.begin(), line.begin() + 9); //ARCHIVED: is 9 characters long
			if (line == "t")
			{
				tempTask.archived = true;
			}
			else
			{
				tempTask.archived = false;
			}
		}
		else
		{
			cout << endl << "Something went wrong.  This message should never be seen!";
		}

		//Put the loaded task into the vector
		loadedTasks.push_back(tempTask);
	}
	//Sort the task list by priority for display
	sort(loadedTasks.begin(), loadedTasks.end(), ComparePriority());
}


void displayTaskList()
{
	ifstream taskList("tasklist.txt");
	string line;
	while (getline(taskList, line))
	{
		cout << line << endl;
	}
	cout << endl;
	displayMainMenu();
}

void autoBackup()
{
	bool backupExists = false;
	string line;
	ifstream openBackupFile("autobackup.txt");
	if (openBackupFile.is_open())
	{
		backupExists = true;
		//open tasklist.txt and copy contents to backup file
		ofstream openedBackupFile("autobackup.txt");
		ifstream existingTaskListFile("tasklist.txt");

		//copy contents of file
		while (getline(existingTaskListFile, line))
		{
			openedBackupFile << line << "\n";
		}
		
	}
	else
	{
		//create new backup file
		ofstream newAutoBackupFile ("autobackup.txt");
		//open existing tasklist.cpp file
		ifstream existingTaskListFile("tasklist.txt");

		//copy contents of file
		while (getline(existingTaskListFile, line))
		{
			newAutoBackupFile << line << "\n";
		}

	}
	backupExists = true;
}

void createTask()
{
	bool firstTaskList;
	ifstream taskFile("tasklist.txt");
	if (taskFile.is_open())
	{
		firstTaskList = false;
		cout << "Current task list file opened sucessfully" << endl << endl;
		ofstream existingTaskFile("tasklist.txt", std::ios_base::app);
		Task NewTask;

		//Write task attributes to file
		cout << "NAME: ";
		cin.ignore();
		getline(cin, NewTask.taskName);
		existingTaskFile << "NAME:" + NewTask.taskName << "\n";

		cout << endl << "DESCRIPTION: ";
		cin.ignore();
		getline(cin, NewTask.description);
		existingTaskFile << "DESCRIPTION:" + NewTask.description << "\n";

		cout << endl << "PRIORITY(e.g: 1): ";
		cin >> NewTask.priority;
		existingTaskFile << "PRIORITY:" << NewTask.priority << "\n";

		cout << endl << "DEPENDENCE: ";
		cin.ignore();
		getline(cin, NewTask.dependance);
		existingTaskFile << "DEPENDENCE:" << NewTask.dependance << "\n";

		cout << "DUEDATE(e.g 11132013): ";
		cin >> NewTask.dueDate;
		existingTaskFile << "DUEDATE:" << NewTask.dueDate << "\n";


		cout << "CURRENTLY WORKING ON?(t or f): ";
		cin.ignore();
		getline(cin, NewTask.workingAnswer);
		if (NewTask.workingAnswer == "t")
		{
			NewTask.working = true;
			existingTaskFile << "WORKINGON:" << "t" << "\n";

		}
		else
		{
			NewTask.working = false;
			existingTaskFile << "WORKINGON:" << "f" << "\n";
		}

		cout << "TIME ALLOCATED IN MINUTES : ";
		cin >> NewTask.talloc;
		existingTaskFile << "TIMEALLOC:" << NewTask.talloc << "\n";

		cout << "ARCHIVED?(t or f): ";
		cin.ignore();
		getline(cin, NewTask.archivedAnswer);
		if (NewTask.archivedAnswer == "t")
		{
			NewTask.archived = true;
			existingTaskFile << "ARCHIVED:" << "t" << "\n";
		}
		else
		{
			NewTask.archived = false;
			existingTaskFile << "ARCHIVED:" << "f" << "\n" << "\n";
		}

		existingTaskFile.close();
		TaskList.push_back(NewTask);
		autoBackup();

	}
	else
	{
		//First tasklist file 
		cout << "*********************************" << endl;
		cout << "Lets create your first task list!" << endl;
		cout << "*********************************" << endl << endl;

		ofstream newTaskFile("tasklist.txt");
		if (newTaskFile.is_open())
		{
			//Create FirstTask for new task file
			Task FirstTask;

			//Write task attributes to file
			cout << "NAME: ";
			cin.ignore();
			getline(cin, FirstTask.taskName);
			newTaskFile << "NAME:" + FirstTask.taskName << "\n";

			cout << endl << "DESCRIPTION: ";
			cin.ignore();
			getline(cin, FirstTask.description);
			newTaskFile << "DESCRIPTION:" + FirstTask.description << "\n";

			cout << endl << "PRIORITY(e.g: 1): ";
			cin  >> FirstTask.priority;
			newTaskFile << "PRIORITY:" << FirstTask.priority << "\n";

			cout << endl << "DEPENDENCE: ";
			cin.ignore();
			getline(cin, FirstTask.dependance);
			newTaskFile << "DEPENDENCE:" << FirstTask.dependance << "\n";

			cout << "DUEDATE(e.g 11132013): ";
			cin  >> FirstTask.dueDate;
			newTaskFile << "DUEDATE:" << FirstTask.dueDate << "\n";


			cout << "CURRENTLY WORKING ON?(t or f): ";
			cin.ignore();
			getline(cin, FirstTask.workingAnswer);
			if (FirstTask.workingAnswer == "t")
			{
				FirstTask.working = true;
				newTaskFile << "WORKINGON:" << "t" << "\n";

			}
			else
			{
				FirstTask.working = false;
				newTaskFile << "WORKINGON:" << "f" << "\n";
			}
			
			cout << "TIME ALLOCATED IN MINUTES : ";
			cin  >> FirstTask.talloc;
			newTaskFile << "TIMEALLOC:" << FirstTask.talloc << "\n";

			cout << "ARCHIVED?(t or f): ";
			cin.ignore();
			getline(cin, FirstTask.archivedAnswer);
			if (FirstTask.archivedAnswer == "t")
			{
				FirstTask.archived = true;
				newTaskFile << "ARCHIVED:" << "t" << "\n";
			}
			else
			{
				FirstTask.archived = false;
				newTaskFile << "ARCHIVED:" << "f" << "\n" << "\n";
			}

			newTaskFile.close();

			//Add new task to vector of current tasks
			TaskList.push_back(FirstTask);
			autoBackup();
		}
	}
	displayMainMenu();
}

void displayMainMenu()
{
	cout << "------------------------------------------" << endl << endl;
	cout << "Welcome to Task Mangement Software (TMS)" << endl << endl;
	cout << "------------------------------------------" << endl;
	cout << "What would you like to do?(type the command help if unsure): ";
	string userAnswer;

	cin >> userAnswer;
	cout << endl << endl;
	if (userAnswer == "help")
	{
		cout << "------------------------------------------------------------" << endl << endl;
		cout << "Here are the commands you can enter to interact with the TMS:" << endl << endl;
		cout << "------------------------------------------------------------" << endl << endl;

		cout << "help: displays this page" << endl << endl;
		cout << "displaytasklist: displays the task list by priority currently in your task list" << endl << endl;
		cout << "createTask: creates a task list if you dont already have one or creates a new task onto your existing task list" << endl << endl;
		cout << "modifyTask modifies a task in your existing task list" << endl << endl;
		cout << "deleteTask deletes a task in your existing task list" << endl << endl;
		cout << "menu: type this command after a completed task to return to the main menu" << endl << endl;

		string secondAnswer;
		cout << "What do you want to do?: ";
		cin >> secondAnswer;
		if (secondAnswer == "createTask")
		{
			createTask();
		}
		else if (secondAnswer == "displaytasklist")
		{
			displayTaskList();
		}
	}
	else if (userAnswer == "createTask")
	{
		createTask();
	}
	else if (userAnswer == "displaytasklist")
	{
		displayTaskList();
	}
	else if (userAnswer == "exit")
	{
		return;
	}
	else
	{
		cout << "You entered an invalid command. Please try again, type help if your lost!: " << endl;
		displayMainMenu();
	}
}

int main()
{
	displayMainMenu();
	system("pause");
	return 0;
}
