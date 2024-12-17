#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
#define LINE_OF_INTEREST 2000000
#define SEARCH_SIZE 4000000

//static const char inputFileName[] = "inputTest.txt";
//#define LINE_OF_INTEREST 10
//#define SEARCH_SIZE 20


static const std::string delimiterSensor = "Sensor at ";
static const std::string delimiterBeacon = "closest beacon is at ";
static const std::string delimiterColon = ":";
static const std::string delimiterComma = ",";
static const std::string delimiterX = "x=";
static const std::string delimiterY = "y=";

class Point
{
public:
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {

    }

    int x_;
    int y_;
};

class Position
{
public:
    Position(Point sensor, Point beacon)
        : sensor_(sensor)
        , beacon_(beacon)
        , distance_(abs(sensor.x_ - beacon.x_) + abs(sensor.y_ - beacon.y_))
    {

    }

    Point sensor_;
    Point beacon_;
    int distance_;
};

typedef std::vector<Position> Positions;

struct Range1
{
    int min;
    int max;
};

class Range
{
public:
    Range()
    {
        // Construct without any elements
    }

    Range(int xMin, int xMax)
    {
        // Construct with a new element
        // after sorting them in order
        range_.push_back({ std::min(xMin, xMax), std::max(xMin, xMax) });
    }

    void add(int xMin, int xMax)
    {
        int min = std::min(xMin, xMax);
        int max = std::max(xMin, xMax);

        for (auto it = range_.begin(); it != range_.end(); it++)
        {
            if (min < it->min && max < it->min)
            {
                // not overlapping ranges
                continue;
            }
            if (it->max < min && it->max < max)
            {
                // not overlapping ranges
                continue;
            }

            // There is some overlap with the range we are currently checking against, must combine ranges
            int newMin = std::min(it->min, min);
            int newMax = std::max(it->max, max);

            range_.erase(it); // delete the one we are merging with
            add(newMin, newMax); // add a new range and see if there are any new overlaps
            return; // must return
        }

        // if we get this far, it must be a brand new range that needs to be added
        range_.push_back({ min, max });
    }

    size_t distance()
    {
        int dist = 0;

        for (auto& element : range_)
        {
            dist += element.max - element.min + 1;
        }

        return dist;
    }

    int findGap()
    {
        int current = -1;

        for (auto& element : range_)
        {
            if (current < element.min)
            {
                current = element.min - 1;
            }
                
            if (element.max < current)
            {
                current = element.max + 1;
            }
            if (current != - 1)
            {
                return current;
            }
        }
        return -1;
    }

    std::vector<Range1> range_; //<min, max> non overlapping pairs of coordinates
};


Point parsePoint(std::string line)
{
    int x = 0;
    int y = 0;

    size_t tokenPositionX = line.find(delimiterX, 0);
    if (tokenPositionX != std::string::npos)
    {
        line.erase(0, tokenPositionX + delimiterX.length());

        size_t tokenPositionComma = line.find(delimiterComma, 0);
        x = std::stoi(line.substr(0, tokenPositionComma));

        size_t tokenPositionY = line.find(delimiterY, 0);
        if (tokenPositionY != std::string::npos)
        {
            line.erase(0, tokenPositionY + delimiterX.length());
            y = std::stoi(line);
        }
    }

    return Point(x, y);
}

void readInputFile(std::string fileName, Positions& positions)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                switch (state)
                {
                    case 1:
                    {
                        std::string sensorCoordinates;
                        std::string beaconCoordinates;

                        size_t tokenPositionSensor = line.find(delimiterSensor, 0);
                        if (tokenPositionSensor != std::string::npos)
                        {
                            line.erase(0, tokenPositionSensor + delimiterSensor.length());

                            size_t tokenPositionColon = line.find(delimiterColon, 0);
                            if (tokenPositionColon != std::string::npos)
                            {
                                sensorCoordinates = line.substr(0, tokenPositionColon);
                            }

                            size_t tokenPositionBeacon = line.find(delimiterBeacon, 0);
                            if (tokenPositionBeacon != std::string::npos)
                            {
                                line.erase(0, tokenPositionBeacon + delimiterBeacon.length());
                            }
                            beaconCoordinates = line;
                        }

                        positions.emplace_back(parsePoint(sensorCoordinates), parsePoint(beaconCoordinates));
                        break;
                    }
                }
            }
            else
            {
                state = 1;
            }
        }
        myFile.close();
    }
}


size_t getCoverage(Positions& positions, int y, int xMinLimit, int xMaxLimit, bool includeBeacons, int &x)
{
    size_t coverage = 0;
    bool isWithinRange = false;
    int x1 = 0;
    int x2 = 0;
    Range range;
    std::set<int> beaconsAtY;
    size_t numBeaconsAtY = 0;


    for (auto& position : positions)
    {
        if (position.beacon_.y_ == y)
        {
            beaconsAtY.emplace(position.beacon_.x_);
        }

        // Is y within range of a sensor and its beacon distance
        if (abs(position.sensor_.y_ - y) <= position.distance_)
        {
            int dist = position.distance_ - abs(position.sensor_.y_ - y);
            int xx1 = position.sensor_.x_ - dist;
            int xx2 = position.sensor_.x_ + dist;

            if (xx1 < xMinLimit)
            {
                xx1 = xMinLimit;
            }
            if (xx2 > xMaxLimit)
            {
                xx2 = xMaxLimit;
            }

            range.add(xx1, xx2);
        }
    }

    coverage = range.distance();

    if (!includeBeacons)
    {
        numBeaconsAtY = beaconsAtY.size();
    }
    else
    {
        if (coverage < SEARCH_SIZE + 1)
        {
            x = range.findGap();
        }
    }

    return coverage - numBeaconsAtY;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Positions positions;
    Range range;
    int x = -1;

    readInputFile(inputFileName, positions);


    if (positions.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;
            std::cout << "Coverage: " << getCoverage(positions, LINE_OF_INTEREST, INT_MIN, INT_MAX, false, x);

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            for (int y = 0; y <= SEARCH_SIZE; y++)
            {
                x = -1;
                size_t coverage = getCoverage(positions, y, 0, SEARCH_SIZE, true, x);
                if (coverage < SEARCH_SIZE + 1)
                {
                    std::cout << "Possible row: " << y << " at " << x << " with tuning frequency: " << (unsigned long long) x * 4000000 + y << std::endl;
                }
            }

            std::cout << std::endl;
        }
    }
}
