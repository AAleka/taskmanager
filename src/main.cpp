// TaskManager.cpp : Defines the entry point for the application.
//

#include "TaskManager.h"

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

		else if (option.substr(0, 6) == "delete")
			tm.delete_numbers();

		else if (option.substr(0, option.find(' ')) == "delete" && option.length() > 7)
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