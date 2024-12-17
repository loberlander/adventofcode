#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";


typedef long long BigNumber;

typedef std::vector<int> Map;

// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
                tokenPosition = line.find(delimiter);
            }
        }

        if (tokenPosition != std::string::npos)
        {
            result.emplace_back(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition);
        }
        else
        {
            // Must be last element in the line
            if (!line.empty())
            {
                result.emplace_back(line);
                line.clear();
            }
        }
    }

    return result;
}


void readInputFile(std::string fileName, Map& map)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Map
                    if (!line.empty())
                    {
                        int nextFileId = 0; // start with File Id zero
                        bool isFile = true; // start with file blocks

                        for (auto& ch : line)
                        {
                            int id = -1; // indicate space
                            if (isFile)
                            {
                                id = nextFileId;
                                nextFileId++;
                            }

                            int spaceCount = stoi(std::string(1, ch));

                            for (int i = 0; i < spaceCount; i++)
                            {
                                map.emplace_back(id);
                            }
                            isFile = !isFile;
                        }
                    }
                    else
                    {
                        
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

void defragmentPart1(Map& map)
{
    for (size_t i = 0; i < map.size(); i++)
    {
        if (map[i] == -1)
        {
            // Empty spot is available

            for (size_t j = map.size() - 1; j > 0; j--)
            {
                if (i >= j)
                {
                    break;
                }
                if (map[j] >= 0)
                {
                    map[i] = map[j];
                    map[j] = -1;
                    map.erase(map.begin() + j);
                    break;
                }
            }
        }
    }
}


int getFreeSpace(Map& map, size_t location)
{
    int size = 0;

    while (map[location] == -1)
    {
        size++;
        location++;
    }

    return size;
}

int getFileSize(Map& map, int fileId, size_t location)
{
    int size = 0;

    while (map[location] == fileId)
    {
        size++;
        if (location > 0)
        {
            location--;
        }
        else
        {
            break;
        }
    }

    return size;
}

void defragmentPart2(Map& map)
{
    for (size_t i = map.size() - 1; i > 0; i--)
    {
        if (map[i] >= 0)
        {
            int fileSize = getFileSize(map, map[i], i);

            for (size_t j = 0; j < i; j++)
            {
                if (map[j] == -1)
                {
                    int spaceAvailable = getFreeSpace(map, j);
                    if (spaceAvailable >= fileSize)
                    {
                        // move the file
                        for (int k = 0; k < fileSize; k++)
                        {
                            map[j + k] = map[i - fileSize + k + 1];
                            map[i - fileSize + k + 1] = -1;
                        }
                        break;
                    }
                    else
                    {
                        j = j + spaceAvailable - 1;  // -1 as the for() increments by one as well
                    }
                }
            }
            if (i >= fileSize + 1)
            {
                i = i - fileSize + 1; // +1 as the for() decrements by one as well
            }
            else
            {
                i = 1;
            }
        }
    }
}

BigNumber calculateChecksum(Map& map)
{
    BigNumber checksum = 0;

    for (int i = 0; i < map.size(); i++)
    {
        if (map[i] != -1)
        {
            checksum += i * map[i];
        }
    }

    return checksum;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            Map mapPart1 = map;
            defragmentPart1(mapPart1);
            BigNumber sum = calculateChecksum(mapPart1);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            Map mapPart2 = map;
            defragmentPart2(mapPart2);
            BigNumber sum = calculateChecksum(mapPart2);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
