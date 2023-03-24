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

class TaskManager {
public:
    std::vector <std::string> file_lines;
    std::string file_path;

    TaskManager(const std::string path) : file_path{ path } {
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

    ~TaskManager() { }

    void write_task() {
        std::cout << "\nPlease, enter your task: ";

        std::string task;
        std::getline(std::cin, task);

        file_lines.push_back(uuid::generate_uuid_v4() + " " + encrypt(task));
        std::cout << '\n' << task << "\nSuccess!\n\n";
    }

    void list_tasks() {
        std::cout << "\n--------------------------------------\n";

        for (auto line : file_lines)
            std::cout << decrypt(line.substr(line.find(' ') + 1, line.length() - 1)) << '\n';

        std::cout << "--------------------------------------\n\n";
    }

    void find_task(std::string keyword) {
        std::string line;
        std::cout << "\nLooking for a match with " << keyword << "\n--------------------------------------\n";

        for (auto line : file_lines)
            if (line.substr(line.find(' ') + 1, line.length() - 1).find(encrypt(keyword)) != std::string::npos)
                std::cout << "Found: " << decrypt(line.substr(line.find(' ') + 1, line.length() - 1)) << '\n';

        std::cout << "--------------------------------------\n\n";
    }

    void delete_numbers() {
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

    void delete_keyword(std::string keyword) {
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

    void edit_task() {
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

    void save_changes() {
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

    void print_helper() {
        std::cout << "\nThe available commands are:\n" <<
            "add: adds a new task.\n" <<
            "list: lists all saved tasks.\n" <<
            "find <keyword>: finds all tasks with given keyword.\n" <<
            "delete: deletes the given task.\n" <<
            "delete <keyword>: deletes all tasks with given keyword.\n" <<
            "export: exports your tasks to decrypted file.\n\n";
    }

    void export_to_file() {
        std::cout << "Enter the name of the file you want your decrypted tasks to be exported to (e.g. tasks2.txt): ";
        std::string export_name;
        std::getline(std::cin, export_name);
        
        std::ofstream f;
        f.open(export_name, std::fstream::out);

        for (auto line : file_lines)
            f << line.substr(0, line.find(' ') + 1) + decrypt(line.substr(line.find(' ') + 1, line.length())) + '\n';

        f.close();
    }
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
