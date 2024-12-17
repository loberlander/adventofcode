#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const char TREE = '#';
static const int slopeX1 = 3;
static const int slopeY1 = 1;

static const int slopeX2 = 1;
static const int slopeY2 = 1;

static const int slopeX3 = 5;
static const int slopeY3 = 1;

static const int slopeX4 = 7;
static const int slopeY4 = 1;

static const int slopeX5 = 1;
static const int slopeY5 = 2;

// Format of input:  ".#..........#...#...#..#......."

std::vector<bool> parseLine(std::string line)
{
    std::vector<bool> trees;

    for (auto tree : line)
    {
        trees.emplace_back(tree == TREE);
    }

    return trees;
}

std::vector<std::vector<bool>> readInputFile(std::string fileName)
{
    std::vector<std::vector<bool>> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.push_back(parseLine(line));
        }
        myFile.close();
    }

    return data;
}

int treesCrossed(std::vector<std::vector<bool>> & data, int slopeX, int slopeY)
{
    int treeCount = 0;

    auto sizeY = data.size();
    auto sizeX = data[0].size();
    int x = 0;
    for (int y = 0; y < sizeY ; y += slopeY)
    //for (auto trees : data)
    {
        auto trees = data[y];
        if (trees[x])
        {
            ++treeCount;
        }
        x = (x + slopeX) % (sizeX);
    }

    return treeCount;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<std::vector<bool>> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;

        long long int treesCrossed0 = treesCrossed(data, slopeX1, slopeY1);

        std::cout << "Trees crossed: " << treesCrossed0 << std::endl;

        std::cout << "Part 2:" << std::endl;

        long long int treesCrossed1 = treesCrossed(data, slopeX1, slopeY1);
        std::cout << "Trees crossed #1: " << treesCrossed1 << std::endl;

        long long int treesCrossed2 = treesCrossed(data, slopeX2, slopeY2);
        std::cout << "Trees crossed #2: " << treesCrossed2 << std::endl;

        long long int treesCrossed3 = treesCrossed(data, slopeX3, slopeY3);
        std::cout << "Trees crossed #3: " << treesCrossed3 << std::endl;

        long long int treesCrossed4 = treesCrossed(data, slopeX4, slopeY4);
        std::cout << "Trees crossed #4: " << treesCrossed4 << std::endl;

        long long int treesCrossed5 = treesCrossed(data, slopeX5, slopeY5);
        std::cout << "Trees crossed #5: " << treesCrossed5 << std::endl;

        // Must use long long int to get a result that would not overflow!
        std::cout << "Product: " << treesCrossed1 * treesCrossed2 * treesCrossed3 * treesCrossed4 * treesCrossed5 << std::endl;
    }
}
