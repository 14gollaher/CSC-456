// Matthew Gollaher
// CSC-456
// Assignment 3
// This program represents a paging-based memory management system.
// Users have options to several commands to utilize the paging-
// based memory management system. 

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

// Function prototypes
void memoryManager(int, int);
int allocate(int, int);
int deallocate(int);
int write(int, int);
int read(int, int);
void printMemory();
std::vector<std::string> getUserInput();
void printIntroduction();
bool checkArgumentCount(int, int);

// Structure to represent a process
struct process
{
	int pId;
	std::vector<int> pageTable;
};

//Phyiscal Memory Space
std::vector<unsigned char> physicalMemorySpace;

//Free Frame List
std::vector<bool> freeFrames;

//Processes list
std::vector<process> processes;

//Total allocation size
size_t totalAllocatedMemory = 0;

int main()
{
	// Print the introduction
	printIntroduction();

	// Continue program until user prompts to exit
	while (true)
	{
		std::vector<std::string> userCommandArguments = getUserInput();
		switch (userCommandArguments[0][0])
		{
		case 'M':
		case 'm':
			if (checkArgumentCount(3, userCommandArguments.size()))
			{
				memoryManager(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
			}
			break;
		case 'A':
		case 'a':
			if (checkArgumentCount(3, userCommandArguments.size()))
			{
				allocate(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
			}
			break;
		case 'D':
		case 'd':
			if (checkArgumentCount(2, userCommandArguments.size()))
			{
				deallocate(std::atoi(userCommandArguments[1].c_str()));
			}
			break;
		case 'W':
		case 'w':
			if (checkArgumentCount(3, userCommandArguments.size()))
			{
				write(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
			}
			break;
		case 'R':
		case 'r':
			if (checkArgumentCount(3, userCommandArguments.size()))
			{
				read(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
			}
			break;
		case 'P':
		case 'p':
			if (checkArgumentCount(1, userCommandArguments.size()))
			{
				printMemory();
			}
			break;
		case 'X':
		case 'x':
			if (checkArgumentCount(1, userCommandArguments.size()))
			{
				return 0;
			}
		}
	}
}

// Initialize a memory space with a given frame size - frame size is assumed to always be zero
void memoryManager(int memorySize, int frameSize)
{
	physicalMemorySpace.clear();
	for (int i = 0; i < memorySize; i++)
	{
		physicalMemorySpace.push_back('0');
		freeFrames.push_back(true);
	}
}

// Allocate a memory size to a  given process using random page assignment
int allocate(int allocationSize, int pId)
{
	totalAllocatedMemory += allocationSize;
	if (totalAllocatedMemory + allocationSize > physicalMemorySpace.size())
	{
		totalAllocatedMemory -= allocationSize;
		std::cout << "Allocate operation failed." << std::endl;
		return -1;
	}

	process process;
	process.pId = pId;

	int totalAllocated = 0;
	int index = 0;
	while (totalAllocated < allocationSize)
	{
		index = rand() % physicalMemorySpace.size();
		if (freeFrames[index])
		{
			process.pageTable.push_back(index);
			freeFrames[index] = false;
			totalAllocated++;
		}
	}
	processes.push_back(process);
	return 1;
}

// Function to deallocate all memory and frames used by a given process
int deallocate(int pId)
{
	for (size_t i = 0; i < processes.size(); i++)
	{
		if (processes[i].pId == pId)
		{
			for (size_t j = 0; j < processes[i].pageTable.size(); j++)
			{
				totalAllocatedMemory--;
				freeFrames[processes[i].pageTable[j]] = true;
				physicalMemorySpace[processes[i].pageTable[j]] = '0';
			}
			processes.erase(processes.begin() + i);
			return 1;
		}
	}
	std::cout << "Deallocate operation failed." << std::endl;
	return -1;
}

// Writes a 1 to a given address space of a given process id
int write(int pId, int logicalAddress)
{
	for (size_t i = 0; i < processes.size(); i++)
	{
		if (processes[i].pId == pId)
		{
			for (size_t j = 0; j < processes[i].pageTable.size(); j++)
			{
				if (processes[i].pageTable[j] == logicalAddress)
				{
					physicalMemorySpace[logicalAddress] = '1';
					return 1;
				}
			}
		}
	}
	std::cout << "Write operation failed." << std::endl;
	return -1;
}

// Function to read a logical address from a given process id
int read(int pId, int logicalAddress)
{
	for (size_t i = 0; i < processes.size(); i++)
	{
		if (processes[i].pId == pId)
		{
			for (size_t j = 0; j < processes[i].pageTable.size(); j++)
			{
				if (processes[i].pageTable[j] == logicalAddress)
				{
					std::cout << "Value in logical address " << logicalAddress << " is " << physicalMemorySpace[logicalAddress] << std::endl;
					return physicalMemorySpace[logicalAddress];
				}
			}
		}
	}
	std::cout << "Read operation failed." << std::endl;
	return -1;
}

// Function to print entities of the program to user
void printMemory()
{
	std::cout << "Physical Memory Space:" << std::endl;
	for (size_t i = 0; i < physicalMemorySpace.size(); i++)
	{
		std::cout << physicalMemorySpace[i] << " ";
	}

	std::cout << std::endl << "Free Frame List:" << std::endl;
	for (size_t i = 0; i < freeFrames.size(); i++)
	{
		if (freeFrames[i])
		{
			std::cout << i << " ";
		}
	}

	std::cout << std::endl << "Process List:" << std::endl;
	for (size_t i = 0; i < processes.size(); i++)
	{
		std::cout << processes[i].pId << " " << processes[i].pageTable.size() << std::endl;
	}
	std::cout << std::endl;
}

// Function that prompts user for next commnd
std::vector<std::string> getUserInput()
{
	std::cout << "> ";

	// User input parsing
	std::string userCommand;
	std::getline(std::cin, userCommand);
	std::vector<std::string> userCommandArguments;
	for (size_t i = 0, j = 0; i != userCommand.npos; i = j)
	{
		userCommandArguments.push_back(userCommand.substr(i + (i != 0), (j = userCommand.find(' ', i + 1)) - i - (i != 0)));
	}

	return userCommandArguments;
}

// Function to print introduction and instructions on how to use program to a user
void printIntroduction()
{
	std::cout << "This program represents a paging-based memory management system." << std::endl;
	std::cout << "Below are some command you may use to use this program." << std::endl << std::endl;
	std::cout << "M <memSize> <frameSize>" << std::endl;
	std::cout << "A <allocSize> <pid>" << std::endl;
	std::cout << "D <pid>" << std::endl;
	std::cout << "W <pid> <logical_address>" << std::endl;
	std::cout << "R <pid> <logical_address>" << std::endl;
	std::cout << "P" << std::endl;
	std::cout << "X <-- Command to exit program" << std::endl << std::endl;
}

// Function to compare two integers and return true if equal
bool checkArgumentCount(int expectedArgCount, int actualArgCount)
{
	if (expectedArgCount != actualArgCount)
	{
		std::cout << "Invalid command!" << std::endl;
		return false;
	}
	return true;
}