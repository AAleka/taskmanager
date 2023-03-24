// TaskManager.cpp : Defines the entry point for the application.
//

#include "TaskManager.h"

std::string encrypt(std::string task) {
	for (int i{ 0 }; i < task.length(); i++)
		task[i] += 7;

	return task;
}

std::string decrypt(std::string task) {
	for (int i{ 0 }; i < task.length(); i++)
		task[i] -= 7;

	return task;
}

TaskManager::TaskManager(const std::string path) : file_path{path} {
	std::ifstream file;
	file.open(path);
	if (file.fail()) {
		std::ofstream f;
		f.open("test.txt", std::fstream::out);
		f.close();
	}

	std::string line;

	while (std::getline(file, line))
		file_lines.push_back(line);

	file.close();
}

void TaskManager::write_task() {
	std::cout << "\nPlease, enter your task: ";

	std::string task;
	std::getline(std::cin, task);

	file_lines.push_back(uuid::generate_uuid_v4() + " " + encrypt(task));
	std::cout << '\n' << task << "\nSuccess!\n\n";
}

void TaskManager::list_tasks() {
	std::cout << "\n--------------------------------------\n";

	for (auto line : file_lines)
		std::cout << decrypt(line.substr(line.find(' ') + 1, line.length() - 1)) << '\n';

	std::cout << "--------------------------------------\n\n";
}

void TaskManager::delete_numbers() {
	std::cout << '\n';
	int i{ 0 };

	for (auto line : file_lines)
		std::cout << ++i << ") " << decrypt(line.substr(line.find(' ') + 1, line.length() + 1)) << '\n';

	std::cout << "\nEnter the number/s of a task you want to be deleted: ";

	std::string numbers;
	std::getline(std::cin, numbers);

	std::istringstream is(numbers);
	std::vector<int> delete_nums((std::istream_iterator<int>(is)), std::istream_iterator<int>());

	bool do_it = true;

	for (auto n : delete_nums)
		if (n - 1 >= file_lines.size()) {
			std::cout << "Please, enter number/s from the list.\n\n";
			do_it = false;
			break;
		}

	if (do_it) {
		std::cout << "Are you sure you want to delete these tasks? \n";

		for (auto n : delete_nums)
			std::cout << n << ") " << decrypt(file_lines[n - 1].substr(file_lines[n - 1].find(' ') + 1, file_lines[n - 1].length() + 1)) << '\n';

		std::cout << "y/n: ";
		char yn;
		std::cin >> yn;

		if (yn) {
			std::vector<std::string> tempBuffer;

			for (int i{ 0 }; i < file_lines.size(); i++) {
				bool keep = true;
				for (int j : delete_nums)
					if (i == j - 1) {
						keep = false;
						break;
					}

				if (keep)
					tempBuffer.push_back(file_lines.at(i));
			}


			file_lines = tempBuffer;
			std::cout << "\nSuccess!\n\n";
		}
		else
			std::cout << "\nCanceled.\n\n";
	}
}

void TaskManager::find_task(std::string keyword) {
	std::string line;
	std::cout << "\nLooking for a match with " << keyword << "\n--------------------------------------\n";

	for (auto line : file_lines)
		if (line.substr(line.find(' ') + 1, line.length() - 1).find(encrypt(keyword)) != std::string::npos)
			std::cout << "Found: " << decrypt(line.substr(line.find(' ') + 1, line.length() - 1)) << '\n';

	std::cout << "--------------------------------------\n\n";
}

void TaskManager::delete_keyword(std::string keyword) {
	std::cout << "\n\nThe following task/s will be deleted:\n\n--------------------------------------\n";

	std::vector <std::string> tempbuff;

	for (int i{ 0 }; i < file_lines.size(); i++) {
		if (file_lines.at(i).find(encrypt(keyword)) != std::string::npos)
			std::cout << decrypt(file_lines.at(i).substr(file_lines.at(i).find(' ') + 1, file_lines.at(i).length() - 1)) << '\n';
		else
			tempbuff.push_back(file_lines.at(i));
	}

	std::cout << "--------------------------------------\n\nAre you sure you want to delete given task/s?\ny/n: ";
	char yn;
	std::cin >> yn;

	if (yn == 'y') {
		file_lines = tempbuff;
		std::cout << "\nSuccess.\n\n";
	}

	else if (yn == 'n')
		std::cout << "\nCanceled.\n\n";

	else
		std::cout << "\nWrong argument.\n\n";
}

void TaskManager::edit_task() {
	std::cout << '\n';
	int i{ 0 };

	for (auto line : file_lines)
		std::cout << ++i << ") " << decrypt(line.substr(line.find(' ') + 1, line.length() + 1)) << '\n';

	std::cout << "\nEnter the number of a task you want to be edited: ";
	int n;
	std::cin >> n;

	if (n - 1 < file_lines.size()) {
		std::cout << "Enter your updated task: ";
		std::string updated_task;
		std::cin.ignore();
		std::getline(std::cin, updated_task);

		if (n - 1 < file_lines.size()) {
			file_lines[n - 1].replace(file_lines[n - 1].find(' ') + 1, file_lines[n - 1].length(), encrypt(updated_task));
			std::cout << "\nSuccess!\n\n";
		}
	}
	else
		std::cout << "Please, enter a number from the list.\n\n";
}

void TaskManager::save_changes() {
	std::ofstream file;
	file.open(file_path);
	file.close();

	std::ofstream f;
	f.open(file_path, std::ios::app);

	for (auto i{ 0 }; i < file_lines.size(); i++) {
		f << file_lines.at(i);
		if (i != file_lines.size() - 1)
			f << '\n';
	}

	f.close();
}

void TaskManager::print_helper() {
	std::cout << "\nThe available commands are:\n" <<
		"add: adds a new task.\n" <<
		"list: lists all saved tasks.\n" <<
		"find <keyword>: finds all tasks with given keyword.\n" <<
		"delete: deletes the given task.\n" <<
		"delete <keyword>: deletes all tasks with given keyword.\n" <<
		"export: exports your tasks to decrypted file.\n\n";
}

void TaskManager::export_to_file() {
	std::ofstream f;
	f.open("export.txt", std::fstream::out);

	for (auto line : file_lines)
		f << line.substr(0, line.find(' ') + 1) + decrypt(line.substr(line.find(' ') + 1, line.length())) + '\n';
	
	std::cout << "\nSuccess.\n\n";
	
	f.close();
}

void signal_callback_handler(int signum) {
	TaskManager tm{ "tasks.txt" };
	tm.save_changes();
	exit(signum);
}

int main()
{
	TaskManager tm{ "tasks.txt" };
	signal(SIGINT, signal_callback_handler);

	std::cout << "Welcome to the Task Manager application!\n";

	while (true) {
		std::cout << "Please, type your command (type help to get help).\n";

		std::string option;
		std::getline(std::cin, option);

		if (option == "help")
			tm.print_helper();

		else if (option.substr(0, 3) == "add")
			tm.write_task();

		else if (option.substr(0, 4) == "list")
			tm.list_tasks();

		else if (option.substr(0, option.find(' ')) == "find")
			tm.find_task(option.substr(option.find(' ') + 1, option.length() - 1));

		else if (option.substr(0, 6) == "delete" && option.length() == 6)
			tm.delete_numbers();

		else if (option.substr(0, option.find(' ')) == "delete" && option.length() > 6)
			tm.delete_keyword(option.substr(option.find(' ') + 1, option.length()));

		else if (option.substr(0, 4) == "edit")
			tm.edit_task();

		else if (option.substr(0, 6) == "export")
			tm.export_to_file();

		else if (option.substr(0, 4) == "exit") {
			tm.save_changes();
			return 0;
		}
	}

	return 1;
}












/*
int main()
{
	TaskManager tm;
	for (int i{ 0 }; ; i++) {
		if (i == 0)
			std::cout << "Welcome to the Task Manager application! What would you like to do? (type help to get help).\n";
		else
			std::cout << "Please, type your command (type help to get help).\n";
		
		std::string option;
		std::getline(std::cin, option);

		if (option == "help")
			tm.print_helper();

		else if (option == "add")
			tm.write_task();

		else if (option == "list")
			tm.list_tasks();

		else if (option.substr(0, option.find(' ')) == "find")
			tm.find_task(option.substr(option.find(' ') + 1, option.length() - 1));

		else if (option == "delete")
			tm.delete_numbers();

		else if (option == "edit")
			tm.edit_task();

		else if (option == "exit")
			return 0;

		else
			std::cout << "Wrong command type help to get help).\n";
	}
	
	return 0;
}
*/