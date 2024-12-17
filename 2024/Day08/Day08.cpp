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


typedef std::vector<std::string> Map;

class Point
{
public:
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {
    }

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
    {
    }

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    Point operator+(const Point& other)
    {
        return Point(x_ + other.x_, y_ + other.y_);
    }

    Point& operator+=(const Point& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
};


class Antenna
{
public:
    Antenna(Point point, char type)
        : point_(point)
        , type_(type)
    {
    }

    Antenna()
        : point_(0, 0)
        , type_('?')
    {
    }

    std::string asString()
    {
        return asStringType() + ":" + point_.asString();
    }

    std::string asStringType()
    {
        return std::string(1, type_);
    }

    std::string asStringPoint()
    {
        return point_.asString();
    }

    Point point_;
    char type_;
};

typedef std::map<std::string, Antenna> Antennas;

typedef std::set<std::string> Visited;

typedef long long BigNumber;

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
                        map.emplace_back(line);
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


bool isWithinBounds(Map& map, Point p)
{
    if (p.x_ < 0 || p.y_ < 0 || p.x_ >= map[0].size() || p.y_ >= map.size())
    {
        return false;
    }
    return true;
}


void findAntennas(Map& map, Antennas& antennas)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (std::isalnum(map[y][x]))
            {
                Antenna antenna(Point(x, y), map[y][x]);
                antennas.emplace(antenna.asString(), antenna);
            }
        }
    }
}


void placeAntinodes(Map& map, Antenna& a1, Antenna& a2, Visited& visited, bool isPart2)
{
    int xDiff1 = a1.point_.x_ - a2.point_.x_;
    int yDiff1 = a1.point_.y_ - a2.point_.y_;

    int xDiff2 = a2.point_.x_ - a1.point_.x_;
    int yDiff2 = a2.point_.y_ - a1.point_.y_;

    Point antinode1(a1.point_.x_ + xDiff1, a1.point_.y_ + yDiff1);
    Point antinode2(a2.point_.x_ + xDiff2, a2.point_.y_ + yDiff2);

    if (isWithinBounds(map, antinode1))
    {
        visited.emplace(antinode1.asString());
    }

    if (isWithinBounds(map, antinode2))
    {
        visited.emplace(antinode2.asString());
    }

    if (isPart2)
    {
        Point antinodeH1(a1.point_.x_, a1.point_.y_);
        while(isWithinBounds(map, antinodeH1))
        {
            visited.emplace(antinodeH1.asString());
            antinodeH1.x_ += xDiff1;
            antinodeH1.y_ += yDiff1;
        }

        Point antinodeH2(a2.point_.x_, a2.point_.y_);
        while (isWithinBounds(map, antinodeH2))
        {
            visited.emplace(antinodeH2.asString());
            antinodeH2.x_ += xDiff2;
            antinodeH2.y_ += yDiff2;
        }
    }
}

void placeAntinodes(Map& map, std::vector<Antenna>& sameAntennas, Visited& visited, bool isPart2)
{
    // Iterate through all possible pairs
    for (int i = 0; i < sameAntennas.size() - 1; i++)
    {
        for (int j = i + 1; j < sameAntennas.size(); j++)
        {
            placeAntinodes(map, sameAntennas[i], sameAntennas[j], visited, isPart2);
        }
    }
}


void placeAntinodes(Map& map, Antennas& antennas, Visited& visited, bool isPart2)
{
    std::vector<Antenna> sameAntennas;
    for (auto& antenna : antennas)
    {
        if (sameAntennas.empty() || sameAntennas[0].asStringType() == antenna.second.asStringType())
        {
            sameAntennas.emplace_back(antenna.second);
        }
        else
        {
            if (sameAntennas.size() > 1)
            {
                placeAntinodes(map, sameAntennas, visited, isPart2);
            }

            // Time to start a new type
            sameAntennas.clear();
            sameAntennas.emplace_back(antenna.second);
        }
    }
    if (sameAntennas.size() > 1)
    {
        placeAntinodes(map, sameAntennas, visited, isPart2);
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    Antennas antennas;
    Visited visited1;
    Visited visited2;

    readInputFile(inputFileName, map);

    findAntennas(map, antennas);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            placeAntinodes(map, antennas, visited1, false);
            std::cout << "Total: " << visited1.size();
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            placeAntinodes(map, antennas, visited2, true);
            std::cout << "Total: " << visited2.size();
        }

        std::cout << std::endl;

    }
}
