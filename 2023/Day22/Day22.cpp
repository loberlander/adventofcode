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

#include "helper.h"

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";


static const std::string delimiterParts = "~";
static const std::string delimiterCordinates = ",";

typedef std::map<int, std::vector<int>> SupportedBy;
typedef std::map<int, std::vector<int>> Supports;

typedef long long BigNumber;

class Point
{
public:
    Point()
        : x_(0)
        , y_(0)
        , extra_(0)
    {}

    Point(int x, int y, int extra = 0)
        : x_(x)
        , y_(y)
        , extra_(extra)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(position.extra_)
    {}

    Point(const Point& position, int extra)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(extra)
    {}

    bool inRange(const int minX, const int maxX, const int minY, const int maxY) const
    {
        return (minX <= x_) && (x_ <= maxX) && (minY <= y_) && (y_ <= maxY);
    }

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    Point& operator+=(const Point& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    Point operator+(const Point& other) const
    {
        return Point(x_ + other.x_, y_ + other.y_);
    }

    Point operator-(const Point& other) const
    {
        return Point(x_ - other.x_, y_ - other.y_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    std::string asStringWithExtra()
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
    int extra_;
};

class Point3D
{
public:
    Point3D()
        : x_(0)
        , y_(0)
        , z_(0)
        , extra_(0)
    {}

    Point3D(int x, int y, int z, int extra = 0)
        : x_(x)
        , y_(y)
        , z_(z)
        , extra_(extra)
    {}

    Point3D(const Point3D& position)
        : x_(position.x_)
        , y_(position.y_)
        , z_(position.z_)
        , extra_(position.extra_)
    {}

    Point3D(const Point3D& position, int extra)
        : x_(position.x_)
        , y_(position.y_)
        , z_(position.z_)
        , extra_(extra)
    {}

    bool inRange(const int minX, const int maxX, const int minY, const int maxY, const int minZ, const int maxZ) const
    {
        return (minX <= x_) && (x_ <= maxX) && (minY <= y_) && (y_ <= maxY) && (minZ <= z_) && (z_ <= maxZ);
    }

    bool operator==(const Point3D& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_);
    }

    Point3D& operator+=(const Point3D& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }

    Point3D& operator-=(const Point3D& other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this;
    }

    Point3D operator+(const Point3D& other) const
    {
        return Point3D(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Point3D operator-(const Point3D& other) const
    {
        return Point3D(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_) + ":" + std::to_string(z_);
    }

    std::string asStringWithExtra()
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_) + ":" + std::to_string(z_);
    }

    int x_;
    int y_;
    int z_;
    int extra_;
};


class Shape
{
public:
    Shape()
    {}

    Shape(int id, Point3D location, Point3D start, Point3D end)
        : id_(id)
        , start_(start)
        , end_(end)
        , location_(location)
    {}

    Shape(int id, Point3D start, Point3D end)
        : id_(id)
        , start_(start)
        , end_(end)
        , location_(0, 0, 0)
    {}

    bool isBetween(const int a, const int b, const int c) const
    {
        return (a <= b && b <= c) || (c <= b && b <= a);
    }

    bool doSegmentsIntersect1(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4) const
    {
        // Check if segments share a common point
        if (p1.x_ == p3.x_ && p1.y_ == p3.y_ && p1.z_ == p3.z_) return true;
        if (p1.x_ == p4.x_ && p1.y_ == p4.y_ && p1.z_ == p4.z_) return true;
        if (p2.x_ == p3.x_ && p2.y_ == p3.y_ && p2.z_ == p3.z_) return true;
        if (p2.x_ == p4.x_ && p2.y_ == p4.y_ && p2.z_ == p4.z_) return true;

        int d1343 = (p1.x_ - p3.x_) * (p4.x_ - p3.x_) + (p1.y_ - p3.y_) * (p4.y_ - p3.y_) + (p1.z_ - p3.z_) * (p4.z_ - p3.z_);
        int d4321 = (p4.x_ - p3.x_) * (p2.x_ - p1.x_) + (p4.y_ - p3.y_) * (p2.y_ - p1.y_) + (p4.z_ - p3.z_) * (p2.z_ - p1.z_);
        int d1321 = (p1.x_ - p3.x_) * (p2.x_ - p1.x_) + (p1.y_ - p3.y_) * (p2.y_ - p1.y_) + (p1.z_ - p3.z_) * (p2.z_ - p1.z_);
        int d4343 = (p4.x_ - p3.x_) * (p4.x_ - p3.x_) + (p4.y_ - p3.y_) * (p4.y_ - p3.y_) + (p4.z_ - p3.z_) * (p4.z_ - p3.z_);
        int d2121 = (p2.x_ - p1.x_) * (p2.x_ - p1.x_) + (p2.y_ - p1.y_) * (p2.y_ - p1.y_) + (p2.z_ - p1.z_) * (p2.z_ - p1.z_);

        int denom = d2121 * d4343 - d4321 * d4321;
        if (denom == 0) return false;

        double numer = d1343 * d4321 - d1321 * d4343;
        double mua = numer / denom;
        double mub = (d1343 + d4321 * (mua)) / d4343;

        if ((0 <= mua && mua <= 1) && (0 <= mub && mub <= 1)) {
            return true; // Segments intersect
        }
        return false; // Segments don't intersect
    }

    bool doSegmentsIntersect(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4) const
    {
        int d = ((p2.x_ - p1.x_) * ((p4.y_ - p3.y_) * (p3.z_ - p1.z_) - (p4.z_ - p3.z_) * (p3.y_ - p1.y_))) -
            ((p2.y_ - p1.y_) * ((p4.x_ - p3.x_) * (p3.z_ - p1.z_) - (p4.z_ - p3.z_) * (p3.x_ - p1.x_))) +
            ((p2.z_ - p1.z_) * ((p4.x_ - p3.x_) * (p3.y_ - p1.y_) - (p4.y_ - p3.y_) * (p3.x_ - p1.x_)));

        int da = ((p4.x_ - p3.x_) * ((p1.y_ - p3.y_) * (p2.z_ - p1.z_) - (p1.z_ - p3.z_) * (p2.y_ - p1.y_))) -
            ((p4.y_ - p3.y_) * ((p1.x_ - p3.x_) * (p2.z_ - p1.z_) - (p1.z_ - p3.z_) * (p2.x_ - p1.x_))) +
            ((p4.z_ - p3.z_) * ((p1.x_ - p3.x_) * (p2.y_ - p1.y_) - (p1.y_ - p3.y_) * (p2.x_ - p1.x_)));

        if (d == 0 && da == 0) {
            if (isBetween(p1.x_, p3.x_, p2.x_) || isBetween(p1.x_, p4.x_, p2.x_) ||
                isBetween(p3.x_, p1.x_, p4.x_) || isBetween(p3.x_, p2.x_, p4.x_)) {
                if (isBetween(p1.y_, p3.y_, p2.y_) || isBetween(p1.y_, p4.y_, p2.y_) ||
                    isBetween(p3.y_, p1.y_, p4.y_) || isBetween(p3.y_, p2.y_, p4.y_)) {
                    if (isBetween(p1.z_, p3.z_, p2.z_) || isBetween(p1.z_, p4.z_, p2.z_) ||
                        isBetween(p3.z_, p1.z_, p4.z_) || isBetween(p3.z_, p2.z_, p4.z_)) {
                        return true; // Segments overlap or share a common point
                    }
                }
            }
        }

        return false; // Segments don't intersect
    }

    bool intersect(const Shape& other) const
    {
/*
        int maxX = std::max(start_.x_, other.start_.x_);
        int minX = std::min(end_.x_, other.end_.x_);
        
        int maxY = std::max(start_.y_, other.start_.y_);
        int minY = std::min(end_.y_, other.end_.y_);

        return (maxX <= minX) && (maxY <= minY);
*/

        return doSegmentsIntersect(start_, end_, other.start_, other.end_);
    }

    bool intersect2(const Shape& other) const
    {
        int maxX = std::max(start_.x_, other.start_.x_);
        int minX = std::min(end_.x_, other.end_.x_);

        int maxY = std::max(start_.y_, other.start_.y_);
        int minY = std::min(end_.y_, other.end_.y_);

        return (maxX <= minX) && (maxY <= minY);
    }

    int id_;
    Point3D start_;
    Point3D end_;
    Point3D location_;
};

typedef std::vector<Shape> Shapes;

// Directions
typedef Point Dir;
typedef std::vector<Dir> Dirs;
typedef Point3D Dir3D;
typedef std::vector<Dir3D> Dirs3D;

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

BigNumber lcm(std::vector<BigNumber> values)
{
    // Requires C++17 that provides std::gcd()
    // calculate the LCM of values in a vector
    // lcm(a, b) = (a*b)/gcd(a,b)
    BigNumber lcmTmp = values.back();
    values.pop_back();

    for (auto value : values)
    {
        lcmTmp = lcmTmp * value / std::gcd(lcmTmp, value);
    }

    return lcmTmp;
}


void readInputFile(std::string fileName, Shapes& shapes)
{
    std::string line;
    std::ifstream myFile(fileName);

    int id = 0;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Workflows
                    if (!line.empty())
                    {
                        std::vector<std::string> parts = split(line, delimiterParts);
                        std::vector<std::string> coords0 = split(parts[0], delimiterCordinates);
                        std::vector<std::string> coords1 = split(parts[1], delimiterCordinates);

                        shapes.emplace_back(
                            id,
                            Point3D(std::stoi(coords0[0]), std::stoi(coords0[1]), std::stoi(coords0[2])),
                            Point3D(std::stoi(coords1[0]), std::stoi(coords1[1]), std::stoi(coords1[2]))
                        );
                        id++;
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }

            }
        }
        myFile.close();
    }
}

void stack1(Shapes& shapes, SupportedBy& supportedBy, Supports& supports)
{
    for (auto itOuter = shapes.begin(); itOuter < shapes.end(); itOuter++)
    {
        auto testShape = *itOuter;
        bool done = false;

        int loopCount = 0;
        while (!done)
        {
            // change z to force intersection for checking
            testShape.start_.z_--;
            testShape.end_.z_--;
            loopCount++;

            if (itOuter == shapes.begin())
            {
                if (!(testShape.end_.z_ > 0 && testShape.start_.z_ > 0))
                {
                    done = true;
                }
            }
            else
            {
                // Check for collisions with already settled shapes
                for (auto itInner = shapes.begin(); itInner < itOuter; itInner++)
                {
                    if (itInner->intersect(testShape))
                    {
                        done = true;
                        auto supportedByIt = supportedBy.find(testShape.id_);
                        if (supportedByIt == supportedBy.end())
                        {
                            supportedBy.emplace(testShape.id_, std::vector({ itInner->id_ }));
                        }
                        else
                        {
                            supportedByIt->second.emplace_back(itInner->id_);
                        }

                        auto supportsIt = supports.find(itInner->id_);
                        if (supportsIt == supports.end())
                        {
                            supports.emplace(itInner->id_, std::vector({ testShape.id_ }));
                        }
                        else
                        {
                            supportsIt->second.emplace_back(testShape.id_);
                        }
                    }
                    if (!(testShape.end_.z_ > 0 && testShape.start_.z_ > 0))
                    {
                        done = true;
                    }
                }
            }
        }
        if (loopCount > 1)
        {
            testShape.start_.z_++;
            testShape.end_.z_++;
            *itOuter = testShape;
        }
    }
}

void stack2(Shapes& shapes, SupportedBy& supportedBy, Supports& supports)
{
    for (auto itOuter = shapes.begin(); itOuter < shapes.end(); itOuter++)
    {
        int maxZ = 1;
        // Check for collisions with already settled shapes
        for (auto itInner = shapes.begin(); itInner < itOuter; itInner++)
        {
            if (itInner->intersect2(*itOuter))
            {
                maxZ = std::max(maxZ, itInner->end_.z_ + 1);
            }
        }
        itOuter->end_.z_ -= itOuter->start_.z_ - maxZ;
        itOuter->start_.z_ = maxZ;
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Shapes shapes;

    SupportedBy supportedBy;
    Supports supports;

    readInputFile(inputFileName, shapes);

    struct
    {
        bool operator()(Shape a, Shape b) const { return a.start_.z_ < b.start_.z_; }
    } customLess;

    // sorting is needed for the order to be right
    // Some shapes might already be too low by the time they are processed
    std::sort(shapes.begin(), shapes.end(), customLess);

    if (shapes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        stack1(shapes, supportedBy, supports);

        //std::cout << "Supports1: " << supports << std::endl;
        //std::cout << "SupporedBy1: " << supportedBy << std::endl;

        //for (auto& item : supports)
        //{
        //    std::cout <<
        //}
        //std::cout << std::endl;

        SupportedBy supportedBy2;
        Supports supports2;

        for (auto itUpper = shapes.begin(); itUpper < shapes.end(); itUpper++)
        {
            auto testShape = *itUpper;
            testShape.start_.z_--;
            testShape.end_.z_--;
            for (auto itLower = shapes.begin(); itLower < itUpper; itLower++)
            {
                if (itLower->intersect(testShape))
                {
                    auto supportedByIt = supportedBy2.find(testShape.id_);
                    if (supportedByIt == supportedBy2.end())
                    {
                        supportedBy2.emplace(testShape.id_, std::vector({ itLower->id_ }));
                    }
                    else
                    {
                        supportedByIt->second.emplace_back(itLower->id_);
                    }

                    auto supportsIt = supports2.find(itLower->id_);
                    if (supportsIt == supports2.end())
                    {
                        supports2.emplace(itLower->id_, std::vector({ testShape.id_ }));
                    }
                    else
                    {
                        supportsIt->second.emplace_back(testShape.id_);
                    }
                }
            }
        }

        //std::cout << "Supports2: " << supports2 << std::endl;
        //std::cout << "SupporedBy2: " << supportedBy2 << std::endl;
/*
            if (supports != supports2)
            {
                std::cout << "Supports2 is different: " << std::endl;
            }

            if (supportedBy != supportedBy2)
            {
                std::cout << "SupportedBy2 is different: " << std::endl;
            }
*/

        std::cout << "Part 1:" << std::endl;
        {
            int total = 0;

            std::set<int> seen;

            for (auto& shape : shapes)
            {
                // Find all shapes that are supported by another shape
                auto supportIt = supports2.find(shape.id_);
                if (supportIt != supports2.end())
                {
                    bool hasAllWithMultipleSupport = true;
                    for (auto& item : supportIt->second)
                    {
                        // Find all supporters of the shape and see if there is more than 1
                        auto supportedByIt = supportedBy2.find(item);
                        if (supportedByIt != supportedBy2.end())
                        {
                            if (supportedByIt->second.size() > 1)
                            {
                                
                            }
                            else
                            {
                                hasAllWithMultipleSupport = false;
                            }
                        }
                    }
                    if (hasAllWithMultipleSupport)
                    {
                        seen.emplace(shape.id_);
                        //total++;
                    }
                }
                else
                {
                    //total++;
                    seen.emplace(shape.id_);
                }
            }

            //std::cout << "Seen: " << seen << std::endl;
            total = seen.size();

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            int total = 0;

            for (auto& shape : shapes)
            {
                std::deque<int> q;
                std::set<int> seen;

                auto supportIt = supports2.find(shape.id_);
                if (supportIt != supports2.end())
                {
                    for (auto& item : supportIt->second)
                    {
                        // Find all supporters of the shape and see if there is only one
                        auto supportedByIt = supportedBy2.find(item);
                        if (supportedByIt != supportedBy2.end())
                        {
                            if (supportedByIt->second.size() == 1)
                            {
                                q.emplace_back(item);
                                seen.emplace(item);
                            }
                        }
                    }
                }
                
                seen.emplace(shape.id_); // Since this is the id we are actively disintegrating to start with

                while (!q.empty())
                {
                    int i = q.front();
                    q.pop_front();

                    // No,
                    auto supportIt = supports2.find(i);
                    if (supportIt != supports2.end())
                    {
                        for (auto& item : supportIt->second)
                        {
                            bool allDetroyingAlready = true;
                            // Are we destroying this already? Have we seen this already?
                            if (seen.find(item) == seen.end())
                            {
                                // Not yet
                                // Find all supporters of the shape and see if there is only one
                                auto supportedByIt = supportedBy2.find(item);
                                if (supportedByIt != supportedBy2.end())
                                {
                                    // Check if all supporters are destroyed
                                    for (auto& supporters : supportedByIt->second)
                                    {
                                        if (seen.find(supporters) == seen.end())
                                        {
                                            allDetroyingAlready = false;
                                        }
                                    }
                                }
                            }
                            if (allDetroyingAlready)
                            {
                                q.emplace_back(item);
                                seen.emplace(item);
                            }
                        }
                    }
                }

                total += seen.size() - 1;
            }
            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
    }
}
