cmake_minimum_required(VERSION 3.20)

project(TaskManager LANGUAGES CXX)

set(SOURCE_FILES
	"src/main.cpp"
	"src/taskmanager.h"
)

add_executable(taskmanager ${SOURCE_FILES})