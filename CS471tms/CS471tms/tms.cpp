/*
	AUTHOR: Caleb Hellickson
	PURPOSE: CS471 TO-DO LIST PROJECT
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // for std::sort
#include <time.h>  // to keep track of the elapsed time 

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

//NEED TO PUT THIS IN TASK CLASS LATER
//******************************
vector<Task> _loadedTasks;
bool tasksLoaded = false;
vector <Task> TaskList;
//******************************

//for use in std::sort when sorting tasks in a vector
//by priority
struct ComparePriority
{
	bool operator()(Task & a, Task & b)
	{
		return a.priority < b.priority;
	}
};

void updateWorkingTasks(int elapsedTime)
{
	for (unsigned int w = 0; w < _loadedTasks.size(); ++w)
	{
		if (_loadedTasks[w].working == true && _loadedTasks[w].archived == false)
		{
			_loadedTasks[w].talloc += elapsedTime;
		}
	}
}

void displayHelpMenu()
{
	cout << "------------------------------------------------------------" << endl << endl;
	cout << "Here are the commands you can enter to interact with the TMS:" << endl << endl;
	cout << "------------------------------------------------------------" << endl << endl;

	cout << "help: displays this page" << endl << endl;
	cout << "loadtasklist: loads the tasks that are in your current task list into memory" << endl << endl;
	cout << "displaytasklist: displays your current task list sorted by priority" << endl << endl;
	cout << "displaytasklista: displays the tasks that you have previously archived" << endl << endl;
	cout << "createTask: creates a task list" << endl << endl;
	cout << "modifyTask: modifies a task in your existing task list" << endl << endl;
	cout << "deleteTask: deletes a task in your existing task list" << endl << endl;
}

void rewrite()
{
	//open tasklist.txt for rewriting the task list
	ofstream taskFile("tasklist.txt");
	//This is to destinguish whether to append to the file and use ENDNAME
	//instead of name for the remaining tasks after the first task has been
	//written to the file
	bool firstTaskWritten = false;
	
	if (firstTaskWritten == false)
	{
		taskFile << "NAME:" << _loadedTasks[0].taskName << endl;
		taskFile << "DESCRIPTION:" << _loadedTasks[0].description << endl;
		taskFile << "PRIORITY:" << _loadedTasks[0].priority << endl;
		taskFile << "DEPENDENCE:" << _loadedTasks[0].dependance << endl;
		taskFile << "DUEDATE:" << _loadedTasks[0].dueDate << endl;
		taskFile << "WORKINGON:" << _loadedTasks[0].working << endl;
		taskFile << "TIMEALLOC:" << _loadedTasks[0].talloc << endl;
		taskFile << "ARCHIVED:" << _loadedTasks[0].archived << endl;
		taskFile.close();
		firstTaskWritten = true;
	}

		//append the rest of the tasks to the file
		ofstream appendTask("tasklist.txt", std::ios_base::app);

		for (unsigned int k = 1; k < _loadedTasks.size(); ++k)
		{
			appendTask << "ENDNAME:" << _loadedTasks[k].taskName << endl;
			appendTask << "DESCRIPTION:" << _loadedTasks[k].description << endl;
			appendTask << "PRIORITY:" << _loadedTasks[k].priority << endl;
			appendTask << "DEPENDENCE:" << _loadedTasks[k].dependance << endl;
			appendTask << "DUEDATE:" << _loadedTasks[k].dueDate << endl;
			appendTask << "WORKINGON:" << _loadedTasks[k].working << endl;
			appendTask << "TIMEALLOC:" << _loadedTasks[k].talloc << endl;
			appendTask << "ARCHIVED:" << _loadedTasks[k].archived << endl;
		}
		//Write End to the end of the file to stop parsing when
		//loading tasks
		appendTask << "END";
		appendTask.close();
}


vector<Task>::iterator findTask(const string & taskName)
{
	for (auto i = _loadedTasks.begin(); i != _loadedTasks.end();i++)
	{
		if (i->taskName == taskName)
		{
			return i;
		}
	}
	cout << "The name of the task you are trying to modify does not exist" << endl;
	return _loadedTasks.end();
}

void deleteTask(string & taskName)
{
	auto taskToDelete = findTask(taskName);
	if (taskToDelete == _loadedTasks.end())
	{
		return;
	}
	if (taskToDelete->talloc == 0)
	{
		_loadedTasks.erase(taskToDelete);
	}
	else
	{
		cout << endl << "You cannot delete a task that you have time allocated to" << endl;
	}
}


void modifyTask(string taskName)
{
	string attributeToChange;
	string line;
	int number;
	auto taskToModify = findTask(taskName);
	if (taskToModify == _loadedTasks.end())
	{
		return;
	}
	cout << "What attribute do you want to modify?: ";
	getline(cin, attributeToChange);
	if (attributeToChange == "name")
	{
		cout << endl << "NAME: ";
		getline(cin, line);
		taskToModify->taskName = line;
	}
	else if (attributeToChange == "description")
	{
		cout << endl << "DESCRIPTION: ";
		getline(cin, line);
		taskToModify->description = line;
	}
	else if (attributeToChange == "priority")
	{
		cout << endl << "PRIORITY: ";
		cin >> number;
		taskToModify->priority = number;
	}
	else if (attributeToChange == "dependence")
	{
		cout << endl << "DEPENDENCE: ";
		getline(cin, line);
		taskToModify->dependance = line;
	}
	else if (attributeToChange == "due")
	{
		cout << endl << "DUEDATE: ";
		cin >> number;
		taskToModify->dueDate = number;
	}
	else if (attributeToChange == "working")
	{
		cout << endl << "WORKINGON: ";
		cin >> line;
		if (line == "t")
		{
			taskToModify->working = true;
		}
		else
		{
			taskToModify->working = false;
		}
	}
	else if (attributeToChange == "time")
	{
		cout << endl << "TIMEALLOC: ";
		cin >> number;
		taskToModify->talloc = number;
	}
	else if (attributeToChange == "archived")
	{
		cout << endl << "ARCHIVED: ";
		cin >> line;
		if (line == "t")
		{
			taskToModify->archived = true;
		}
		else
		{
			taskToModify->archived = false;
		}
		
	}
	
}

//Function that clears any information
//in the task. This is used when parsing
//the file for tasks
Task & clearTask(Task & task)
{
	task.taskName = "";
	task.description = "";
	task.priority = 0;
	task.dependance = "";
	task.dueDate = 0;
	task.working = false;
	task.talloc = 0;
	task.archived = false;
	return task;
}

void loadTasks()
{
	//open tasklist.txt and go though it to populate the vector of current tasks
	Task tempTask;
	ifstream taskFile("tasklist.txt");
	//If a tasklist has not yet been created
	//there will be not tasks to load
	if (!taskFile)
	{
		cout << endl <<  "There is no current task list to load.  Please create a task list." << endl;
		return;
	}
	_loadedTasks.clear();
	string line;
	string attributeIdentifier;
	
	//Get a line from the file 
	while (getline(taskFile, line) && line != "END")
	{
		attributeIdentifier.clear();
		attributeIdentifier += line[0];
		attributeIdentifier += line[1];
		attributeIdentifier += line[2];
		attributeIdentifier += line[3];

		//cout << attributeIdentifier;
		//return;

		if (attributeIdentifier == "NAME")
		{
			line.erase(line.begin(), line.begin() + 5); //Name: is 5 characters long save the rest of the line for Task object
			tempTask.taskName = line;
		}
		else if (attributeIdentifier == "ENDN") //This is for the case when the task parsed is not the last task in the file
		{
			line.erase(line.begin(), line.begin() + 8); //ENDNAME: is 8 characters long
			tempTask.taskName = line;
		}
		else if (attributeIdentifier == "DESC")
		{
			line.erase(line.begin(), line.begin() + 12); //DESCRIPTION: is 12 characters long
			tempTask.description = line;
		}
		else if (attributeIdentifier == "PRIO")
		{
			line.erase(line.begin(), line.begin() + 9); // PRIORITY: is 9 characters long
			tempTask.priority = atoi(line.c_str());
		}
		else if (attributeIdentifier == "DEPE")
		{
			line.erase(line.begin(), line.begin() + 11);  // DEPENDENCE: is 11 characters long
			tempTask.dependance = line;
		}
		else if (attributeIdentifier == "DUED")
		{
			line.erase(line.begin(), line.begin() + 8); //DUEDATE: is 8 characters long
			tempTask.dueDate = atoi(line.c_str());
		}
		else if (attributeIdentifier == "WORK")
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
		else if (attributeIdentifier == "TIME")
		{
			line.erase(line.begin(), line.begin() + 10 ); //TIMEALLOC: is 10 characters long
			tempTask.talloc = atoi(line.c_str());
		}
		else if (attributeIdentifier == "ARCH")
		{
			line.erase(line.begin(), line.begin() + 9); //ARCHIVED: is 9 characters long
			if (line == "t")
			{
				tempTask.archived = true;
				//Put the loaded task into the vector
				_loadedTasks.push_back(tempTask);
				clearTask(tempTask);
			}
			else
			{
				tempTask.archived = false;
				//Put the loaded task into the vector
				_loadedTasks.push_back(tempTask);
				clearTask(tempTask);
			}
		}
		
	}

	//Sort the task list by priority for display
	sort(_loadedTasks.begin(), _loadedTasks.end(), ComparePriority());

	//print task list
	for (unsigned int i = 0; i<_loadedTasks.size();++i)
	{
		cout << "NAME:" << _loadedTasks[i].taskName << endl;
		cout << "DESCRIPTION:" << _loadedTasks[i].description << endl;
		cout << "PRIORITY:" << _loadedTasks[i].priority << endl;
		cout << "DEPENDENCE:" << _loadedTasks[i].dependance << endl;
		cout << "DUEDATE:" << _loadedTasks[i].dueDate << endl;
		cout << "WORKINGON:" << _loadedTasks[i].working<< endl;
		cout << "TIMEALLOC:" << _loadedTasks[i].talloc << endl;
		cout << "ARCHIVED:" << _loadedTasks[i].archived << endl << endl;
	}
	
}

//Method that displays the task list in the order in which the tasks were added
//They are not sorted by any means when put into the file
void displayTaskList()
{
	//Sort the task list by priority for display
	sort(_loadedTasks.begin(), _loadedTasks.end(), ComparePriority());

	for (auto i : _loadedTasks)
	{
		cout << "NAME: " << i.taskName << endl;
		cout << "DESCRIPTION: " << i.description << endl;
		cout << "PRIORITY: " << i.priority << endl;
		cout << "DEPENDENCE: " << i.dependance << endl;
		cout << "DUEDATE: " << i.dueDate << endl;
		cout << "WORKINGON: " << i.working << endl;
		cout << "TIMEALLOC: " << i.talloc << endl;
		cout << "ARCHIVED: " << i.archived << endl << endl;
	}
}

void displayArchivedTasks()
{
	for (auto x : _loadedTasks)
	{
		if (x.archived == true)
		{
			cout << "NAME: " << x.taskName << endl;
			cout << "DESCRIPTION: " << x.description << endl;
			cout << "PRIORITY: " << x.priority << endl;
			cout << "DEPENDENCE: " << x.dependance << endl;
			cout << "DUEDATE: " << x.dueDate << endl;
			cout << "WORKINGON: " << x.working << endl;
			cout << "TIMEALLOC: " << x.talloc << endl;
			cout << "ARCHIVED: " << x.archived << endl << endl;
		}
	}
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
		//searchForEnd();
		cout << "Current task list file opened sucessfully" << endl << endl;
		ofstream existingTaskFile("tasklist.txt", std::ios_base::app);
		Task NewTask;

		//Write task attributes to file
		cout << "NAME: ";
		cin.ignore();
		getline(cin, NewTask.taskName);
		existingTaskFile << "NAME:" + NewTask.taskName << "\n";

		cout << endl << "DESCRIPTION: ";
		getline(cin, NewTask.description);
		existingTaskFile << "DESCRIPTION:" + NewTask.description << "\n";

		cout << endl << "PRIORITY(e.g: 1): ";
		cin >> NewTask.priority;
		existingTaskFile << "PRIORITY:" << NewTask.priority << "\n";

		cout << endl << "DEPENDENCE: ";
		cin.ignore();
		getline(cin, NewTask.dependance);
		existingTaskFile << "DEPENDENCE:" << NewTask.dependance << "\n";

		cout << endl << "DUEDATE(e.g 11132013): ";
		cin >> NewTask.dueDate;
		existingTaskFile << "DUEDATE:" << NewTask.dueDate << "\n";


		cout << endl << "CURRENTLY WORKING ON?(t or f): ";
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

		cout << endl << "TIME ALLOCATED IN MINUTES : ";
		cin >> NewTask.talloc;
		existingTaskFile << "TIMEALLOC:" << NewTask.talloc << "\n";

		cout << endl << "ARCHIVED?(t or f): ";
		cin.ignore();
		getline(cin, NewTask.archivedAnswer);
		cout << endl << endl;
		if (NewTask.archivedAnswer == "t")
		{
			NewTask.archived = true;
			existingTaskFile << "ARCHIVED:" << "t" << "\n";
		}
		else
		{
			NewTask.archived = false;
			existingTaskFile << "ARCHIVED:" << "f" << "\n" << "END";
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
			getline(cin, FirstTask.description);
			newTaskFile << "DESCRIPTION:" + FirstTask.description << "\n";

			cout << endl << "PRIORITY(e.g: 1): ";
			cin  >> FirstTask.priority;
			newTaskFile << "PRIORITY:" << FirstTask.priority << "\n";

			cout << endl << "DEPENDENCE: ";
			cin.ignore();
			getline(cin, FirstTask.dependance);
			newTaskFile << "DEPENDENCE:" << FirstTask.dependance << "\n";

			cout << endl << "DUEDATE(e.g 11132013): ";
			cin  >> FirstTask.dueDate;
			newTaskFile << "DUEDATE:" << FirstTask.dueDate << "\n";


			cout << endl << "CURRENTLY WORKING ON?(t or f): ";
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
			
			cout << endl << "TIME ALLOCATED IN MINUTES : ";
			cin  >> FirstTask.talloc;
			newTaskFile << "TIMEALLOC:" << FirstTask.talloc << "\n";

			cout << endl << "ARCHIVED?(t or f): ";
			cin.ignore();
			getline(cin, FirstTask.archivedAnswer);
			cout << endl << endl;
			if (FirstTask.archivedAnswer == "t")
			{
				FirstTask.archived = true;
				newTaskFile << "ARCHIVED:" << "t" << "\n";
			}
			else
			{
				FirstTask.archived = false;
				newTaskFile << "ARCHIVED:" << "f" << "\n" << "END";
			}

			newTaskFile.close();

			//Add new task to vector of current tasks
			TaskList.push_back(FirstTask);
			autoBackup();
		}
	}
	
}

void displayMainMenu()
{
	cout << "------------------------------------------" << endl << endl;
	cout << "Welcome to Task Management Software (TMS)" << endl << endl;
	cout << "------------------------------------------" << endl;
	cout << "What would you like to do?(type the command help if unsure): ";
	string userAnswer;

	cin >> userAnswer;
	cout << endl << endl;
	if (userAnswer == "help")
	{
		displayHelpMenu();
		displayMainMenu();
	}
	else if (userAnswer == "modifyTask")
	{
		string modifyAnswer;
		cin.ignore();
		cout << "Which task would you like to modify?: ";
		getline(cin, modifyAnswer);
		modifyTask(modifyAnswer);
		rewrite();
		autoBackup();
		displayMainMenu();
	}
	else if (userAnswer == "deleteTask")
	{
		string deleteAnswer;
		cin.ignore();
		cout << "Which task would you like to delete?: ";
		getline(cin, deleteAnswer);
		deleteTask(deleteAnswer);
		rewrite();
		autoBackup();
		displayMainMenu();
	}
	else if (userAnswer == "createTask")
	{
		createTask();
		loadTasks();
		displayMainMenu();
	}
	else if (userAnswer == "displaytasklist")
	{
		displayTaskList();
		displayMainMenu();
	}
	else if (userAnswer == "displaytasklista")
	{
		displayArchivedTasks();
		displayMainMenu();
	}
	else if (userAnswer == "exit")
	{
		return;
	}
	else if (userAnswer == "loadtasklist" && tasksLoaded == false)
	{
		tasksLoaded = true;
		loadTasks();
		displayMainMenu();
	}
	else if (userAnswer == "loadtasklist" && tasksLoaded == true)
	{
		cout << "The task list has already been loaded into memory." << endl;
		cout << "To look at your task list please type displaytasklist" << endl << endl;
		displayMainMenu();
	}
	else
	{
		cout << "You entered an invalid command. Please try again, type help if your lost!: " << endl;
		displayMainMenu();
	}
}

int main()
{
	time_t start = clock();
	displayMainMenu();
	time_t end = clock();
	int elapsed((int)end - (int)start);
	int seconds = elapsed / CLOCKS_PER_SEC;
	int minutes = seconds / 60;

	//Try to open task list for reading
	//if no task list exists then we don't need to update anything and can exit
	ifstream openTask("tasklist.txt");
	if (openTask.is_open())
	{
		updateWorkingTasks(minutes);
		rewrite();
	}
	cout << endl << endl << minutes << " minutes have elapsed." << endl << endl;
	system("pause");
	return 0;
}
