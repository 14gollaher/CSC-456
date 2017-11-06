// Matthew Gollaher
// Operating Systems
// Programming Assignment 2
// To compile: g++ -pthread Assignment2.cpp
// To run (after compile): ./a.out or a.out

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <pthread.h>

// Global variables
pthread_mutex_t mutex;
std::vector<std::string> BufferA;
std::vector<std::string> BufferB;

// Function Prototypes
void* reader(void*);
void* converter(void*);
void* writer(void*);

int main(void)
{
    // Clear screen for new program
    system("clear");

   // Output instructions to user
    std::cout << "For 3 threads - Enter a string, and all spaces will be converted to %s!" << std::endl << "Enter 'exit' to exit the current reader thread." << std::endl << std::endl;

   // Number of types of threads to be used in program (3 * 3 = 9)
    int threadCount = 3;

   // Declare threads used in program
    pthread_t readerThread[threadCount];
    pthread_t writerThread[threadCount];
    pthread_t converterThread[threadCount];

   // Loop to create the threads
    for(int i = 0; i < threadCount; i++)
    {
        pthread_create(&readerThread[i], NULL, reader, NULL);
        pthread_join(readerThread[i], NULL);
        pthread_create(&converterThread[i], NULL, converter, NULL);
        pthread_join(converterThread[i], NULL);
        pthread_create(&writerThread[i], NULL, writer, NULL);
        pthread_join(writerThread[i], NULL);
    }

   return 0;
}

// Reader thread to read user input 
void* reader(void *) 
{
    // Lock the thread
    pthread_mutex_lock(&mutex);
    std::string inputMessage = "";
    int i = 0;

   // While input message is not exit keyword
    while (inputMessage != "exit")
    {
        inputMessage = "";
        std::cout << "Enter string " << i << ": ";
        getline(std::cin, inputMessage);
        BufferA.push_back(inputMessage);
        i++;
    }
    // Unlock the thread
    pthread_mutex_unlock(&mutex);
}

// Converter thread to convert all input from ' ' to '%'
void* converter(void *)
{
    // Lock the thread
    pthread_mutex_lock(&mutex);

   // While the input buffer is not empty
    while (!BufferA.empty())
    {
        // Evaluate each string, and add to output buffer if it has a space
        std::string originalString = BufferA[BufferA.size() - 1];
        std::string evaluatedString = "";
        bool stringEvaluated = false;
        for (int i = 0; i < originalString.size(); i++)
        {
            if (originalString[i] == ' ')
            {
                stringEvaluated = true;
                originalString[i] = '%';
            }
            evaluatedString.push_back(originalString[i]);
        }
        if (stringEvaluated)
        {
            BufferB.push_back(evaluatedString);
        }
        BufferA.pop_back();
    }
    // Unlock the thread
    pthread_mutex_unlock(&mutex);
}

// Writer thread to output all output to screen
void* writer(void *count)
{
    // Lock the thread
    pthread_mutex_lock(&mutex);
    int i = 1;
    // While the output buffer is not empty
    while (!BufferB.empty())
    {
        std::string outputString = BufferB[BufferB.size() - 1];
        BufferB.pop_back();
        std::cout << "Converted String " << i << ": " << outputString << std::endl;
        i++;
    }
    std::cout << std::endl;
    
   // Unlock the thread
    pthread_mutex_unlock(&mutex);
}