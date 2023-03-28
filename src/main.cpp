// move implementation to taskmanager.cpp (CHECK)
// leave the header just for declaration (CHECK)

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

	// what if it fails to create the file? Yes it can. (CHECK)
	if (file.fail()) {
		try {
			std::ofstream f;
			f.open("test.txt", std::fstream::out);
			f.close();
		}
		catch(std::ifstream& f){
			std::cout << "Unable to open or create a file.\n";
			exit(EXIT_FAILURE);
		}
	}

	std::string line;

	while (std::getline(file, line))
		file_lines.push_back(line);

	file.close();

	// writing to a file in temp directory (CHECK)
	CHAR czTempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, czTempPath);

	std::string temp_path = czTempPath;
	temp_path += "temp.txt";

	std::ofstream temp;
	temp.open(temp_path, std::fstream::out);

	for (auto line : file_lines)
		temp << line + '\n';

	temp.close();
}

void backup(const std::vector <std::string> lines) {
	CHAR czTempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, czTempPath);

	std::string temp_path = czTempPath;
	temp_path += "temp.txt";

	std::ofstream temp;
	temp.open(temp_path, std::fstream::out);

	for (auto line : lines)
		temp << line + '\n';

	temp.close();
}

std::vector <int> find_keyword(const std::vector <std::string> lines, const std::string* keyword) {
	size_t size{ lines.size() };
	std::vector <int> keyword_indices;

	for (auto i{ 0 }; i < size; i++)
		if (lines.at(i).substr(lines.at(i).find(' ') + 1, lines.at(i).length() - 1).find(encrypt(*keyword)) != std::string::npos)
			keyword_indices.push_back(i);

	return keyword_indices;
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

	// nice! (CHECK)
	std::istringstream is(numbers);
	std::vector<int> delete_nums((std::istream_iterator<int>(is)), std::istream_iterator<int>());

	// this is very unclear!!! Name your variables with long, clear names, (CHECK)
	// i should be able to tell what a variable does, just by reading it's name, in the ideal world (CHECK for ideal world where you can tell what a variable does)
	bool to_delete = true;

	for (auto n : delete_nums)
		if (n - 1 >= file_lines.size()) {
			std::cout << "Please, enter number/s from the list.\n\n";
			to_delete = false;
			break;
		}

	if (to_delete) {
		std::cout << "Are you sure you want to delete these tasks? \n";

		for (auto n : delete_nums)
			std::cout << n << ") " << decrypt(file_lines[n - 1].substr(file_lines[n - 1].find(' ') + 1, file_lines[n - 1].length() + 1)) << '\n';

		std::cout << "y/n: ";
		char validation{};
		std::cin >> validation;

		if (validation) {
			// have consistent style for variable names, either snake_case, or camelCase or PascalCase, but don't mix and match
			// for the same token type. It's okay if different tokens have different cases, for example functions can be snake_case
			// and variables can be camelCase. (CHECK)
			std::vector<std::string> tempBuffer;
			size_t size{ file_lines.size() };

			// optimization, here the size() check is being performed every single iteration of the loop
			// since you are re using it, consider calculating it once, and then use the variable instead
			// auto size = file_lines.size(); (CHECK)
			for (auto i = 0; i < size; i++) {
				bool keep = true;
				// auto is your best friend (CHECK)
				for (auto j : delete_nums)
					if (i == j - 1) {
						keep = false;
						break;
					}

				// instead of creating a temp buffer, which is copying the strings, why not just
				// keep the indexes? (How do I push it back to original vector?)
				if (keep)
					tempBuffer.push_back(file_lines.at(i));
			}


			file_lines = tempBuffer;
			std::cout << "\nSuccess!\n\n";
		}
		else if (validation == 'n')
			std::cout << "\nCanceled.\n\n";
		else
			std::cout << "\nInvalid selection.\n\n";
	}
}

// never pass a std::string by copy!!! (CHECK)
// instead, either use a std::string& or std::string_view (CHECK)
void TaskManager::find_task(const std::string* keyword) {
	std::string line;
	std::cout << "\nLooking for a match with " << *keyword << "\n--------------------------------------\n";

	// nice you used auto here, good job. (CHECK)
	// but i really don't like this find ' ' + n , lenght garbage. (CHECK you should try it once, your life will change.)
	std::vector <int> keyword_indices = find_keyword(file_lines, keyword);
	
	for (auto index : keyword_indices)
		std::cout << "Found: " << decrypt(file_lines.at(index).substr(file_lines.at(index).find(' ') + 1, file_lines.at(index).length() - 1)) << '\n';

	std::cout << "--------------------------------------\n\n";
}

// same here about string copy (CHECK)
void TaskManager::delete_keyword(const std::string* keyword) {
	size_t size{ file_lines.size() };
	std::cout << "\n\nThe following task/s will be deleted:\n\n--------------------------------------\n";

	// this for loop is doing something that looks very similar to what i saw above. you can probalby refactor this
	// loop to be its own function or smth. (CHECK)

	// create a find_keyword function (CHECK)
	/*std::vector <int> keyword_indices = find_keyword(file_lines, keyword);

	for (auto index : keyword_indices)
		std::cout << decrypt(file_lines.at(index).substr(file_lines.at(index).find(' ') + 1, file_lines.at(index).length() - 1)) << '\n';*/

	std::vector <std::string> tempbuff;

	for (int i{ 0 }; i < file_lines.size(); i++) {
		if (file_lines.at(i).find(encrypt(*keyword)) != std::string::npos)
			std::cout << decrypt(file_lines.at(i).substr(file_lines.at(i).find(' ') + 1, file_lines.at(i).length() - 1)) << '\n';
		else
			tempbuff.push_back(file_lines.at(i));
	}


	std::cout << "--------------------------------------\n\nAre you sure you want to delete given task/s?\ny/n: ";
	// never use a char without intializing it, it could hold 0x88422, and fuck your life up. (CHECK)
	char validation {};
	std::cin >> validation;

	if (validation == 'y') {
		/*std::vector <std::string> tempbuff;
		int keep_index{ 0 };
		for (auto delete_index : keyword_indices) {
			if (keep_index != delete_index)
				tempbuff.push_back(file_lines.at(keep_index));
			keep_index++;
		}*/

		file_lines = tempbuff;
		std::cout << "\nSuccess.\n\n";
	}

	else if (validation == 'n')
		std::cout << "\nCanceled.\n\n";

	else
		std::cout << "\nWrong argument.\n\n";
}

void TaskManager::edit_task() {
	std::cout << '\n';
	int i = 0;

	// remember that '++' modifies the value. It's not like in python. (CHECK)
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

// this is fine (CHECK)
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

	// writing to a file in temp directory (CHECK)
	backup(file_lines);
}

void signal_callback_handler(int signum) {
	/* This might have unforseen side effects. It may be more safe to not save the database. (CHECK) */
	TaskManager tm{ "tasks.txt" };
	//tm.save_changes();
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

		if (std::cin.fail()) {
			std::cin.clear();
			std::cout << "Invalid input\n";
		}
		else {
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
			else if (option.substr(0, option.find(' ')) == "find") {
				std::string keyword{ option.substr(option.find(' ') + 1, option.length() - 1) };
				tm.find_task(&keyword);
			}

			// you can probably streamline your function naming, for example consider:
			// delete_n_tasks, instead of delete_numbers.
			else if (option.substr(0, 6) == "delete" && option.length() == 6)
				tm.delete_numbers();

			// delete_task_by_keyword
			else if (option.substr(0, option.find(' ')) == "delete" && option.length() > 6) {
				std::string keyword{ option.substr(option.find(' ') + 1, option.length() - 1) };
				tm.delete_keyword(&keyword);
			}

			else if (option.substr(0, 4) == "edit")
				tm.edit_task();

			else if (option.substr(0, 6) == "export")
				tm.export_to_file();

			else if (option.substr(0, 4) == "exit") {
				tm.save_changes();
				return 0;
			}

			else
				std::cout << "Invalid command\n";
		}

		// else {
		// 	return 1;
		// } (CHECK)
	}

	// Your main function, on the last line should always return 0. (CHECK)
	return 0;
}











