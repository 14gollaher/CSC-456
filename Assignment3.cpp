#include <iostream>
#include <vector>
#include <string>

// Function prototypes
void memoryManager(int, int);
int allocate(int, int);
int deallocate(int);
int write(int, int);
int read(int, int);
void printMemory();

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


int main()
{
	// Always execute
	while (true)
	{
		// User input parsing
		std::string userCommand;
		std::getline(std::cin, userCommand);
		std::vector<std::string> userCommandArguments;
		for (size_t i = 0, j = 0; i != userCommand.npos; i = j)
		{
			userCommandArguments.push_back(userCommand.substr(i + (i != 0), (j = userCommand.find(' ', i + 1)) - i - (i != 0)));
		}

		// Handle user command
		switch (userCommandArguments[0][0])
		{
			case 'M': memoryManager(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
				break;
			case 'A': allocate(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
				break;
			case 'D': deallocate(std::atoi(userCommandArguments[1].c_str()));
				break;
			case 'W': write(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
				break;
			case 'R': read(std::atoi(userCommandArguments[1].c_str()), std::atoi(userCommandArguments[2].c_str()));
				break;
			case 'P': printMemory();
				break;
			case 'X': // Exit program
				return 0; 
		}
	}
}

void memoryManager(int memorySize, int frameSize)
{
	for (int i = 0; i < memorySize; i++)
	{
		physicalMemorySpace.push_back('0');
		freeFrames.push_back(true);
	}
}

int allocate(int allocationSize, int pId)
{

	if (allocationSize > physicalMemorySpace.size())
	{
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

int deallocate(int pId)
{
	for (size_t i = 0; i < processes.size(); i++)
	{
		if (processes[i].pId == pId)
		{
			for each (int page in processes[i].pageTable)
			{
				freeFrames[page] = true;
			}
			processes.erase(processes.begin() + i);
			return 1;
		}
	}
	return -1;
}

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
					physicalMemorySpace[logicalAddress] = 1;
					return 1;
				}
			}
		}
	}
	return -1;
}

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
					return physicalMemorySpace[logicalAddress];
				}
			}
		}
	}
	return -1;
}

void printMemory()
{
	std::cout << "Physical Memory Space:" << std::endl;
	for (size_t i = 0; i < physicalMemorySpace.size(); i++)
	{
		std::cout << physicalMemorySpace[i] << " ";
	}

	std::cout << std::endl << "Free Frame List" << std::endl;
	for (size_t i = 0; i < freeFrames.size(); i++)
	{
		std::cout << freeFrames[i] << " ";
	}

	std::cout << std::endl << "Process List" << std::endl;
	for (size_t i = 0; i < processes.size(); i++)
	{
		std::cout << processes[i].pId << " " << processes[i].pageTable.size() << std::endl;
	}
}