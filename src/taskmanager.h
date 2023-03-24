// TaskManager.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <signal.h>
#include<windows.h>

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
            ss << dis(gen);

        ss << "-";
        for (i = 0; i < 4; i++)
            ss << dis(gen);

        ss << "-4";
        for (i = 0; i < 3; i++)
            ss << dis(gen);

        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++)
            ss << dis(gen);

        ss << "-";
        for (i = 0; i < 12; i++)
            ss << dis(gen);

        return ss.str();
    }
}

class TaskManager {
public:
    std::vector <std::string> file_lines;
    std::string file_path;

    TaskManager(const std::string path);

    ~TaskManager() { }

    void write_task(); 

    void list_tasks();

    void find_task(std::string keyword);

    void delete_numbers();

    void delete_keyword(std::string keyword);

    void edit_task();

    void save_changes();

    void print_helper();

    void export_to_file();
};








/*
class TaskManager {
public:
    void write_task() {
        std::ofstream fout;
        fout.open("tasks.txt", std::ios::app);

        std::cout << "\nPlease, enter your task: ";

        std::string task;
        std::getline(std::cin, task);

        fout << '\n' + uuid::generate_uuid_v4() + " " + task;
        std::cout << '\n' << task << "\nSuccess!\n\n";

        fout.close();
    }

    void list_tasks() {
        std::ifstream fin;

        fin.open("tasks.txt");

        std::string line;

        std::cout << "\n--------------------------------------\n";

        while (std::getline(fin, line))
            std::cout << line.substr(line.find(' ') + 1, line.length() - 1) << '\n';

        std::cout << "--------------------------------------\n\n";

        fin.close();
    }

    void find_task(std::string keyword) {
        std::ifstream fin;

        fin.open("tasks.txt");

        std::string line;
        std::cout << "\nLooking for a match with " << keyword << "\n--------------------------------------\n";

        while (std::getline(fin, line))
            if (line.substr(line.find(' ') + 1, line.length() - 1).find(keyword) != std::string::npos)
                std::cout << "Found: " << line.substr(line.find(' ') + 1, line.length() - 1) << '\n';

        std::cout << "--------------------------------------\n\n";

        fin.close();
    }

    void delete_task() {
        std::ifstream fin;

        fin.open("tasks.txt");
        
        std::string line;
        std::vector <std::string> lines;

        std::cout << '\n';
        int i{ 0 };
        
        while (std::getline(fin, line)) {
            std::cout << ++i << ") " << line.substr(line.find(' ') + 1, line.length() + 1) << '\n';
            lines.push_back(line);
        }
        
        fin.close();

        std::cout << "\nEnter the number of a task you want to be deleted: ";
        int n;
        std::cin >> n;

        std::ofstream fout;
        fout.open("tasks.txt");

        for (int i{ 0 }; i < lines.size(); i++)
            if (i != n-1)
                fout << lines.at(i) + '\n';
        
        std::cout << "\nSuccess!\n\n";

        fout.close();
    }

    void edit_task() {
        std::ifstream fin;

        fin.open("tasks.txt");

        std::string line;
        std::vector <std::string> lines;

        std::cout << '\n';

        int i{ 0 };
        while (std::getline(fin, line)) {
            std::cout << ++i << ") " << line.substr(line.find(' ') + 1, line.length() + 1) << '\n';
            lines.push_back(line);
        }

        fin.close();

        std::cout << "\nEnter the number of a task you want to be deleted: ";
        int n;
        std::cin >> n;

        std::ofstream fout;
        fout.open("tasks.txt");

        for (int i{ 0 }; i < lines.size(); i++)
            if (i != n - 1)
                fout << lines.at(i) + '\n';

        std::cout << "\nSuccess!\n\n";

        fout.close();
    }

    void print_helper() {
        std::cout << "\nThe available commands are:\n" <<
            "add: adds a new task.\n" <<
            "list: lists all saved tasks.\n" <<
            "find <keyword>: finds all tasks with given keyword.\n" <<
            "delete <task>: deletes the given task.\n\n";
    }
};
*/

// TODO: Reference additional headers your program requires here.
