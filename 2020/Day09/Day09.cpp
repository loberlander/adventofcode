#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const int windowSize = 25;

std::vector<long long int> readInputFile(std::string fileName)
{
    std::vector<long long int> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.push_back(std::stoll(line));
        }
        myFile.close();
    }

    return data;
}

// Search in the data that preceeds the index to qualify data at index
bool isPreamble(std::vector<long long int>& data, int index)
{
    bool foundIt = false;
    long long int magicNumber = data[index];

    for (int i = index - windowSize; i < index; i++)
    {
        for (int j = i; j < index; j++)
        {
            if ((data[i] != data[j]) && (data[i] + data[j] == magicNumber))
            {
                // index is certainly not it!
                return foundIt;
            }
        }
    }

    // If we get this far that means we have found it!
    foundIt = true;

    return foundIt;
}

bool findSum(std::vector<long long int>& data, int size, long long int magicNumber, long long int & magicSum)
{
    bool foundIt = false;
    size_t dataSize = data.size();
    for (int i = 0; i < dataSize - size; i++)
    {
        long long int sum = 0;
        long long int minimumValue = LLONG_MAX;
        long long int maximumValue = 0;
        for (int j = i; j < i + size; j++)
        {
            if (data[j] > maximumValue)
            {
                maximumValue = data[j];
            }
            if (data[j] < minimumValue)
            {
                minimumValue = data[j];
            }
            sum += data[j];
            if (sum > magicNumber)
            {
                return foundIt;
            }
            else if (sum == magicNumber)
            {
                magicSum = minimumValue + maximumValue;
                foundIt = true;
                return foundIt;
            }
        }
    }

    return foundIt;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<long long int> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;

        long long int preamble = 0;
        size_t dataSize = data.size();
        for (int i = windowSize; i < dataSize; i++)
        {
            bool foundIt = isPreamble(data, i);
            if (foundIt)
            {
                preamble = data[i];
                std::cout << "Today's winning number is "<< preamble << std::endl;
                break;
            }
        }

        std::cout << "Part 2:" << std::endl;
        long long int magicSum = 0;
        for (int i = 2; i < dataSize ; i++)
        {
            bool foundIt = findSum(data, i, preamble, magicSum);
            if (foundIt)
            {
                std::cout << "Today's magic sum is " << magicSum << std::endl;
                break;
            }
        }

    }
}
