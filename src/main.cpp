// move implementation to taskmanager.cpp
// leave the header just for declaration

#include "TaskManager.h"

// this is not encryption, this is very weak obfuscation
std::string encrypt(std::string task) {
	for (int i{ 0 }; i < task.length(); i++)
		task[i] += 7;

	return task;
}

// same goes here
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

	// what if it fails to create the file? Yes it can.

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

	// in general you should never have to add/subtract characters to get to certain points in strings.
	// if you have to do it,
	// create a std::size_t strPtr; and use that instead. But even then, people will laugh at you.
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

	// nice!
	std::istringstream is(numbers);
	std::vector<int> delete_nums((std::istream_iterator<int>(is)), std::istream_iterator<int>());

	// this is very unclear!!! Name your variables with long, clear names,
	// i should be able to tell what a variable does, just by reading it's name, in the ideal world
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
			// have consistent style for variable names, either snake_case, or camelCase or PascalCase, but don't mix and match
			// for the same token type. It's okay if different tokens have different cases, for example functions can be snake_case
			// and variables can be camelCase.
			std::vector<std::string> tempBuffer;

			// optimization, here the size() check is being performed every single iteration of the loop
			// since you are re using it, consider calculating it once, and then use the variable instead
			// auto size = file_lines.size();
			for (int i = 0; i < size; i++) {
				bool keep = true;
				// auto is your best friend
				for (int j : delete_nums)
					if (i == j - 1) {
						keep = false;
						break;
					}

					// instead of creating a temp buffer, which is copying the strings, why not just
					// keep the indexes?
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

// never pass a std::string by copy!!!
// instead, either use a std::string& or std::string_view
void TaskManager::find_task(std::string keyword) {
	std::string line;
	std::cout << "\nLooking for a match with " << keyword << "\n--------------------------------------\n";

	// nice you used auto here, good job.
	// but i really don't like this find ' ' + n , lenght garbage.
	for (auto line : file_lines)
		if (line.substr(line.find(' ') + 1, line.length() - 1).find(encrypt(keyword)) != std::string::npos)
			std::cout << "Found: " << decrypt(line.substr(line.find(' ') + 1, line.length() - 1)) << '\n';

	std::cout << "--------------------------------------\n\n";
}

// same here about string copy
void TaskManager::delete_keyword(std::string keyword) {
	std::cout << "\n\nThe following task/s will be deleted:\n\n--------------------------------------\n";

	std::vector <std::string> tempbuff;

	// this for loop is doing something that looks very similar to what i saw above. you can probalby refactor this
	// loop to be its own function or smth.

	// create a find_keyword function

	for (int i{ 0 }; i < file_lines.size(); i++) {
		if (file_lines.at(i).find(encrypt(keyword)) != std::string::npos)
			std::cout << decrypt(file_lines.at(i).substr(file_lines.at(i).find(' ') + 1, file_lines.at(i).length() - 1)) << '\n';
		else
			tempbuff.push_back(file_lines.at(i));
	}

	std::cout << "--------------------------------------\n\nAre you sure you want to delete given task/s?\ny/n: ";
	// never use a char without intializing it, it could hold 0x88422, and fuck your life up.
	char yn {};
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
	int i = 0;

	// remember that '++' modifies the value. It's not like in python.
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

// this is fine
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
	/* This might have unforseen side effects. It may be more safe to not save the database. */
	TaskManager tm{ "tasks.txt" };
	tm.save_changes();
	exit(signum);
}

int main() {
	TaskManager tm{ "tasks.txt" };
	signal(SIGINT, signal_callback_handler);

	std::cout << "Welcome to the Task Manager application!\n";

	while (true) {
		std::cout << "Please, type your command (type help to get help).\n";

		std::string option;
		std::getline(std::cin, option);
		// std::cin has a lot of complexity, look up std::cin::fail(), std::cin::clear(), and how to
		// handle invalid input.

		if (option == "help")
			tm.print_helper();

		// You shouldn't have to substr here, there are much nicer ways of getting input from user.
		else if (option.substr(0, 3) == "add")
			tm.write_task();

		else if (option.substr(0, 4) == "list")
			tm.list_tasks();

		// same here, you should never have to manually clear spaces, because this won't work with
		// multiple spaces, and even if you think your program only cares about the first ' ', this
		// leaves a lot of room to exploit your program with bad input.
		else if (option.substr(0, option.find(' ')) == "find")
			tm.find_task(option.substr(option.find(' ') + 1, option.length() - 1));

		// you can probably streamline your function naming, for example consider:
		// delete_n_tasks, instead of delete_numbers.
		else if (option.substr(0, 6) == "delete" && option.length() == 6)
			tm.delete_numbers();

		// delete_task_by_keyword
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

		// else {
		// 	return 1;
		// }
	}

	// Your main function, on the last line should always return 0.
	return 1;
}











