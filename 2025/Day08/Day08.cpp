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
#include <cmath>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;

class Point
{
    public:
        Point()
            : x_(0)
            , y_(0)
            , z_(0)
        {
        }

        Point(BigNumber x, BigNumber y, BigNumber z)
            : x_(x)
            , y_(y)
            , z_(z)
        {
        }

        Point(const Point& point)
            : x_(point.x_)
            , y_(point.y_)
            , z_(point.z_)
            , groupId_(point.groupId_)
        {
        }

        bool operator==(const Point& other) const
        {
            return (x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_);
        }

        bool operator!=(const Point& other) const
        {
            return ! (*this == other);
        }

        Point operator+(const Point& other)
        {
            return Point(x_ + other.x_, y_ + other.y_, z_ + other.z_);
        }

        Point& operator+=(const Point& other)
        {
            x_ += other.x_;
            y_ += other.y_;
            z_ += other.z_;
            return *this;
        }

        double distance(const Point& other)
        {
            return std::sqrt(std::pow(x_ - other.x_, 2) + std::pow(y_ - other.y_, 2) + std::pow(z_ - other.z_, 2));
        }

        void setGroupId(std::string groupId)
        {
            groupId_ = groupId;
        }

        BigNumber x()
        {
            return x_;
        }

        BigNumber y()
        {
            return y_;
        }

        BigNumber z()
        {
            return z_;
        }

        std::string asString() const
        {
            return std::to_string(x_) + ":" + std::to_string(y_) + ":" + std::to_string(z_);
        }

    private:
        BigNumber x_;
        BigNumber y_;
        BigNumber z_;
        std::string groupId_;
};

typedef Point Direction;

typedef std::vector<Point> Points;

typedef std::map<double, Points> Distances;


struct AscendingPointComparator
{
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs.asString() < rhs.asString())
        {
            return true;
        }

        return false;
    }
};


typedef std::map<Point, BigNumber, AscendingPointComparator> Groups; // key: Point, value: Group Id

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


void readInputFile(std::string fileName, Points& points)
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
                    // Read ranges
                    if (!line.empty())
                    {
                        auto coordinates = split(line, { "," });
                        if (coordinates.size() == 3)
                        {
                            Point point(std::stoll(coordinates[0]), std::stoll(coordinates[1]), std::stoll(coordinates[2]));
                            points.emplace_back(point);
                        }
                    }
                    else
                    {
                        state++;
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

Groups groupPoints(Points& points, Distances& distances, BigNumber maxNumConnections, Points& lastNeighbors)
{
    Groups groups;

    // start with the first pair of points in the sorted distances that are the closest
    auto it = distances.begin();
    BigNumber groupId = 0;
    BigNumber numConnections = 0;

    while (numConnections < maxNumConnections && it != distances.end())
    {
        Points neighbors(it->second);
        
        auto foundIt0 = groups.find(neighbors[0]);
        auto foundIt1 = groups.find(neighbors[1]);

        if (foundIt0 != groups.end() && foundIt1 != groups.end() && foundIt0->second != foundIt1->second)
        {
            // Points were found in two distinct groups, they need to be merged
            // Connect the two groups together
            BigNumber groupIdNew = std::min(foundIt0->second, foundIt1->second);
            BigNumber groupIdOld = std::max(foundIt0->second, foundIt1->second);

            for (auto groupIt = groups.begin(); groupIt !=groups.end(); groupIt++)
            {
                if (groupIt->second == groupIdOld)
                {
                    groupIt->second = groupIdNew;
                }
            }
        }
        else if (foundIt0 != groups.end() && foundIt1 == groups.end())
        {
            // One of the points is already in a group
            // connect the other point to it as well
            groups.emplace(neighbors[1], foundIt0->second);
        }
        else if (foundIt0 == groups.end() && foundIt1 != groups.end())
        {
            // One of the points is already in a group
            // connect the other point to it as well
            groups.emplace(neighbors[0], foundIt1->second);
        }
        else if (foundIt0 == groups.end() && foundIt1 == groups.end())
        {
            // Neither of the points belong to a group yet
            // Create a new group for them
            groups.emplace(neighbors[0], groupId);
            groups.emplace(neighbors[1], groupId);
            ++groupId;
        }

        // Are we at the last connection?
        if (points.size() == groups.size())
        {
            BigNumber groupSpecial = groups.begin()->second;
            bool isSpecial = true;
            for (auto& group : groups)
            {
                if (group.second != groupSpecial)
                {
                    isSpecial = false;
                    break;
                }
            }
            if (isSpecial)
            {
                lastNeighbors = it->second; // make a copy for later
                return groups;
            }
        }

        // Move to the next set of points with the lowest distance 
        ++it;
        ++numConnections;
    }

    return groups;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Points points;
    Distances distances;

    readInputFile(inputFileName, points);

    if (points.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        // calculate distances among points and store them in a map (sorted by distances)
        for (auto& point1 : points)
        {
            for (auto& point2 : points)
            {
                if (point1 != point2)
                {
                    Points points({ point1, point2 });

                    auto it = distances.emplace(point1.distance(point2), points);
                }
            }
        }

        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            Points lastNeighbors;
            Points pointsP1 = points;

            Groups groups = groupPoints(pointsP1, distances, 1000, lastNeighbors);
            //Groups groups = groupPoints(pointsP1, distances, 10, lastNeighbors);

            std::map<BigNumber, BigNumber> groupsPopulation; // key: group Id: value: points in the group

            for (auto& group : groups)
            {
                groupsPopulation[group.second]++;
            }

            std::multiset<BigNumber> groupSizes; // Multiple instances of the same counts might exist
            for (auto& group : groupsPopulation)
            {
                groupSizes.emplace(group.second);
            }

            auto sizeIt = groupSizes.end();
            sizeIt--; // move to the last element
            result = (*sizeIt--);
            result *= (*sizeIt--);
            result *= (*sizeIt);

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            Points lastNeighbors;
            Points pointsP2 = points;

            Groups groups = groupPoints(pointsP2, distances, 100000000, lastNeighbors);

            result = lastNeighbors[0].x() * lastNeighbors[1].x();

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
