#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";

static const char SPACE = '.';

typedef long long BigNumber;

typedef std::vector<std::string> Shape;

class Present
{
    public:
        Present(int id, Shape& shape)
            : id_(id)
            , shape_(shape)
            , area_(0)
        {
            for (int y = 0; y < shape_.size(); y++)
            {
                for (int x = 0; x < shape_[0].size(); x++)
                {
                    if (shape_[y][x] != SPACE)
                    {
                        ++area_;
                    }
                }
            }
        }

        int id()
        {
            return id_;
        }

        Shape shape()
        {
            return shape_;
        }

        int area()
        {
            return area_;
        }

    private:
        int id_;
        Shape shape_;
        int area_;
};

typedef std::vector<Present> Presents;

typedef std::vector<int> ShapeCounts;

class Region
{
    public:
        Region(int sizeX, int sizeY, ShapeCounts& shapeCounts)
            : sizeX_(sizeX)
            , sizeY_(sizeY)
            , shapeCounts_(shapeCounts)
            , area_(sizeX * sizeY)
        {}

        int sizeX()
        {
            return sizeX_;
        }
        int sizeY()
        {
            return sizeY_;
        }
        ShapeCounts shapeCounts()
        {
            return shapeCounts_;
        }
        int shapeCount(int index)
        {
            return shapeCounts_[index];
        }
        int area()
        {
            return area_;
        }
        int operator[](int index)
        {
            return shapeCounts_[index];
        }

    private:
        int sizeX_;
        int sizeY_;
        ShapeCounts shapeCounts_;
        int area_;
};

typedef std::vector<Region> Regions;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::vector<std::string> delimiters)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;
    if (delimiters.empty())
    {
        return result;
    }

    int delimIndex = 0;

    while (!line.empty())
    {
        std::string delimiter = delimiters[delimIndex];

        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());

                delimIndex = delimIndex < delimiters.size() - 1 ? delimIndex + 1 : delimIndex;
                delimiter = delimiters[delimIndex];
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


std::string trimLeft(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the first non-whitespace character
    size_t pos = str.find_first_not_of(pattern);

    // Extract the substring from the first non-whitespace to the end
    return (pos == std::string::npos) ? "" : str.substr(pos);
}


std::string trimRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the last non-whitespace character
    size_t pos = str.find_last_not_of(pattern);

    // Extract the substring from the beginning to the last non-whitespace 
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}


std::string trimLeftRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    return trimLeft(trimRight(str, pattern), pattern);
}


void readInputFile(std::string fileName, Presents& presents, Regions& regions)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read shapes and regions
                    if (!line.empty())
                    {
                        auto id = split(line, { ":" });

                        switch (id.size())
                        {
                            default:
                            case 0:
                            {
                                // This is an error
                                break;
                            }
                            case 1:
                            {
                                Shape shape;
                                // This is the beginning of a shape
                                while (getline(myFile, line))
                                {
                                    if (line.empty())
                                    {
                                        presents.emplace_back(std::stoi(id[0]), shape);
                                        break;
                                    }
                                    else
                                    {
                                        shape.emplace_back(line);
                                    }
                                }
                                break;
                            }
                            case 2:
                            {
                                // this is the beginning of a region
                                auto region = split(id[0], { "x" });

                                auto counts = split(id[1], { " " });
                                ShapeCounts shapeCounts;
                                for (auto& count : counts)
                                {
                                    shapeCounts.emplace_back(std::stoi(count));
                                }

                                regions.emplace_back(std::stoi(region[0]), std::stoi(region[1]), shapeCounts);
                                break;
                            }
                        }
                    }
                    break;
                }
                case 2:
                {
                    break;
                }
            }
        }
        myFile.close();
    }
}

bool isAFit(Presents& presents, Region& region)
{
    int presentsAreaNeeded = 0;;
    for (int i = 0; i < region.shapeCounts().size(); i++)
    {
        presentsAreaNeeded += region[i] * presents[i].area();
    }
    if (region.area() < presentsAreaNeeded)
    {
        // Guaranteed to not enough space to contain all the gifts in a region
        return false;
    }

    return true;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Presents presents;
    Regions regions;

    readInputFile(inputFileName, presents, regions);

    if (presents.empty() || regions.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;
            for (auto& region : regions)
            {
                result += (isAFit(presents, region) ? 1: 0);
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
