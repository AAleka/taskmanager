#pragma once

#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <signal.h>
#include<windows.h>
#include <filesystem>

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

    void find_task(const std::string* keyword);

    void delete_numbers();

    void delete_keyword(const std::string* keyword);

    void edit_task();

    void save_changes();

    void print_helper();

    void export_to_file();
};