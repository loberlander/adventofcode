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

#include <Eigen/Core>
#include <Eigen/Dense>

//#define TEST

#ifndef TEST
static const char inputFileName[] = "input.txt";
#else
static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";
#endif


static const std::string delimiterParts = " @ ";
static const std::string delimiterCordinates = ", ";


typedef long long BigNumber;

class Point
{
public:
    Point()
        : x_(0)
        , y_(0)
        , extra_(0)
    {}

    Point(BigNumber x, BigNumber y, BigNumber extra = 0)
        : x_(x)
        , y_(y)
        , extra_(extra)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(position.extra_)
    {}

    Point(const Point& position, BigNumber extra)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(extra)
    {}

    bool inRange(const BigNumber minX, const BigNumber maxX, const BigNumber minY, const BigNumber maxY) const
    {
        return (minX <= x_) && (x_ <= maxX) && (minY <= y_) && (y_ <= maxY);
    }

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    bool operator!=(const Point& other) const
    {
        return !operator==(other);
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

    std::string asString() const
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    std::string asStringWithExtra() const
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_);
    }

    bool operator<(const Point& other) const
    {
        return asString() < other.asString();
    }

    BigNumber x_;
    BigNumber y_;
    BigNumber extra_;
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

    Point3D(BigNumber x, BigNumber y, BigNumber z, BigNumber extra = 0)
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

    Point3D(const Point3D& position, BigNumber extra)
        : x_(position.x_)
        , y_(position.y_)
        , z_(position.z_)
        , extra_(extra)
    {}

    bool inRange(const BigNumber minX, const BigNumber maxX, const BigNumber minY, const BigNumber maxY, const BigNumber minZ, const BigNumber maxZ) const
    {
        return (minX <= x_) && (x_ <= maxX) && (minY <= y_) && (y_ <= maxY) && (minZ <= z_) && (z_ <= maxZ);
    }

    bool operator==(const Point3D& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_);
    }

    bool operator!=(const Point3D& other) const
    {
        return !operator==(other);
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

    std::string asString() const
    {
        return std::to_string(x_) + ":" + std::to_string(y_) + ":" + std::to_string(z_);
    }

    std::string asStringWithExtra() const
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_) + ":" + std::to_string(z_);
    }

    bool operator<(const Point3D& other) const
    {
        return asString() < other.asString();
    }

    BigNumber x_;
    BigNumber y_;
    BigNumber z_;
    BigNumber extra_;
};

// Directions
typedef Point Dir;
typedef std::vector<Dir> Dirs;
typedef Point3D Dir3D;
typedef std::vector<Dir3D> Dirs3D;

class Shape
{
public:
    Shape()
    {}

    Shape(BigNumber id, Point3D location, Dir3D velocity)
        : id_(id)
        , location_(location)
        , velocity_(velocity)

    {
        // We want to store the line equation in the general/standard form (ax + by = c) for easy calculations
        // Using ChatGPT answers "convert parametric line equation to standard form" with:
        // Parametric form:
        //  x = x1 + a * t
        // y = y1 + b * t
        //
        // Standard form:
        // A * x + B * y = C
        //
        // A = b
        // B = -a
        // C = b * x1 - a * y1

        A = velocity_.y_;
        B = -velocity_.x_;
        C = velocity_.y_ * location_.x_ - velocity_.x_ * location_.y_;
    }

    bool isBetween(const BigNumber a, const BigNumber b, const BigNumber c) const
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

        BigNumber d1343 = (p1.x_ - p3.x_) * (p4.x_ - p3.x_) + (p1.y_ - p3.y_) * (p4.y_ - p3.y_) + (p1.z_ - p3.z_) * (p4.z_ - p3.z_);
        BigNumber d4321 = (p4.x_ - p3.x_) * (p2.x_ - p1.x_) + (p4.y_ - p3.y_) * (p2.y_ - p1.y_) + (p4.z_ - p3.z_) * (p2.z_ - p1.z_);
        BigNumber d1321 = (p1.x_ - p3.x_) * (p2.x_ - p1.x_) + (p1.y_ - p3.y_) * (p2.y_ - p1.y_) + (p1.z_ - p3.z_) * (p2.z_ - p1.z_);
        BigNumber d4343 = (p4.x_ - p3.x_) * (p4.x_ - p3.x_) + (p4.y_ - p3.y_) * (p4.y_ - p3.y_) + (p4.z_ - p3.z_) * (p4.z_ - p3.z_);
        BigNumber d2121 = (p2.x_ - p1.x_) * (p2.x_ - p1.x_) + (p2.y_ - p1.y_) * (p2.y_ - p1.y_) + (p2.z_ - p1.z_) * (p2.z_ - p1.z_);

        BigNumber denom = d2121 * d4343 - d4321 * d4321;
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
        BigNumber d = ((p2.x_ - p1.x_) * ((p4.y_ - p3.y_) * (p3.z_ - p1.z_) - (p4.z_ - p3.z_) * (p3.y_ - p1.y_))) -
            ((p2.y_ - p1.y_) * ((p4.x_ - p3.x_) * (p3.z_ - p1.z_) - (p4.z_ - p3.z_) * (p3.x_ - p1.x_))) +
            ((p2.z_ - p1.z_) * ((p4.x_ - p3.x_) * (p3.y_ - p1.y_) - (p4.y_ - p3.y_) * (p3.x_ - p1.x_)));

        BigNumber da = ((p4.x_ - p3.x_) * ((p1.y_ - p3.y_) * (p2.z_ - p1.z_) - (p1.z_ - p3.z_) * (p2.y_ - p1.y_))) -
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
        BigNumber maxX = std::max(start_.x_, other.start_.x_);
        BigNumber minX = std::min(end_.x_, other.end_.x_);
        
        BigNumber maxY = std::max(start_.y_, other.start_.y_);
        BigNumber minY = std::min(end_.y_, other.end_.y_);

        return (maxX <= minX) && (maxY <= minY);
*/

        return doSegmentsIntersect(start_, end_, other.start_, other.end_);
    }

    bool intersect2(const Shape& other) const
    {
        BigNumber maxX = std::max(start_.x_, other.start_.x_);
        BigNumber minX = std::min(end_.x_, other.end_.x_);

        BigNumber maxY = std::max(start_.y_, other.start_.y_);
        BigNumber minY = std::min(end_.y_, other.end_.y_);

        return (maxX <= minX) && (maxY <= minY);
    }

    BigNumber id_;
    Point3D location_;
    Point3D start_;
    Point3D end_;
    Dir3D velocity_;

    double A;
    double B;
    double C;
};

typedef std::vector<Shape> Shapes;

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

    BigNumber id = 0;

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
                        std::vector<std::string> location = split(parts[0], delimiterCordinates);
                        std::vector<std::string> velocity = split(parts[1], delimiterCordinates);

                        shapes.emplace_back(
                            id,
                            Point3D(std::stoll(location[0]), std::stoll(location[1]), std::stoll(location[2])),
                            Point3D(std::stoll(velocity[0]), std::stoll(velocity[1]), std::stoll(velocity[2]))
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


Eigen::Matrix3d crossMatrix(Eigen::Vector3d v)
{
    Eigen::Matrix3d result;
    result << 0, -v[2],  v[1],
           v[2],     0, -v[0],
          -v[1],  v[0],     0;
    return result;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Shapes shapes;

    readInputFile(inputFileName, shapes);

    /*
    struct
    {
        bool operator()(Shape a, Shape b) const { return a.start_.z_ < b.start_.z_; }
    } customLess;

    // sorting is needed for the order to be right
    // Some shapes might already be too low by the time they are processed
    std::sort(shapes.begin(), shapes.end(), customLess);
    */

    if (shapes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;

            auto numShapes = shapes.size();

#ifdef TEST
            BigNumber minRange = 7;
            BigNumber maxRange = 27;
#else
            BigNumber minRange = 200000000000000;
            BigNumber maxRange = 400000000000000;
#endif

            // Check each shape against all other ones
            for (auto it1 = shapes.begin(); it1 < shapes.end(); it1++)
            {
                for (auto it2 = std::next(it1); it2 < shapes.end(); it2++)
                {
                    // Check if parallel
                    // ChatGPT answers "when are two lines in standard form parallel ?" with
                    // A1 / B1 == A2 / B2 or cross multiply (Bs might be zero)
                    // A1 * B2 == B1 * A2
                    auto s1 = it1->A * it2->B;
                    auto s2 = it1->B * it2->A;
                    if (s1 == s2)
                    {
                        // parallel lines do not intersect!
                        continue;
                    }
                    // Calculate intersection
                    // ChatGPT answer to "formula for intersection point of two lines in standard form" 
                    auto x = (it1->C * it2->B - it2->C * it1->B) / (s1 - s2);
                    auto y = (it2->C * it1->A - it1->C * it2->A) / (s1 - s2);

                    // Check if the intersection happened in the past
                    // It happened in the past if the sign of the velocity vector and the actual movement of the shape do not match
                    // If differently signed values are multiplied together the result will be negative
                    // Check for both shapes and both x and y components of the coordinates
                    if (minRange <= x && x <= maxRange && minRange <= y && y <= maxRange)
                    {
                        if (((x - it1->location_.x_) * it1->velocity_.x_ >= 0) &&
                            ((y - it1->location_.y_) * it1->velocity_.y_ >= 0) &&
                            ((x - it2->location_.x_) * it2->velocity_.x_ >= 0) &&
                            ((y - it2->location_.y_) * it2->velocity_.y_ >= 0))
                        {
                            total++;
                            //std::cout << "x: " << x << " y: " << y << std::endl;
                        }
                    }
                }
            }

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            // Must find a solution where rock
            // X = Xr + t * Vrx
            // Y = Yr + t * Vry
            // Z = Zr + t * Vrz
            //
            // intercepts a hail
            // X = Xh + t * Vhx
            // Y = Yh + t * Vhy
            // Z = Zh + t * Vhz
            //
            // There are 6 unknowns to figure out:
            // Xr, Yr, Zr, Vrx, Vry, Vrz
            // 
            // This requires 6 independent equations...
            // 
            // t = (Xr - Xh) / (Vhx -Vrx) = (Yr - Yh) / (Vhy -Vry) = (Zr - Zh) / (Vhz -Vrz)
            // 
            // Most likely there is no need to check agains ALL hails as hopefully 2, 3, 4 will provide only a single solution

            // Based on solution posted at
            // https://www.reddit.com/r/adventofcode/comments/18pnycy/comment/kepu26z/
            // https://pastebin.com/NmR6ZDXL
            struct Hailstone
            {
                Eigen::Vector3d p;
                Eigen::Vector3d v;
            };

            std::vector<Hailstone> hailstones;

            // Pick almost any 3 shapes from our data
            int sIndx = 0; //start index
            for (int i = 0; i < 3; i++)
            {
                Hailstone hailstone;
                hailstone.p[0] = shapes[sIndx + i].location_.x_;
                hailstone.p[1] = shapes[sIndx + i].location_.y_;
                hailstone.p[2] = shapes[sIndx + i].location_.z_;
                hailstone.v[0] = shapes[sIndx + i].velocity_.x_;
                hailstone.v[1] = shapes[sIndx + i].velocity_.y_;
                hailstone.v[2] = shapes[sIndx + i].velocity_.z_;
                hailstones.emplace_back(hailstone);
            }

            Eigen::MatrixXd M(6, 6);
            Eigen::VectorXd rhs(6);
            rhs.segment<3>(0) = -hailstones[0].p.cross(hailstones[0].v) + hailstones[1].p.cross(hailstones[1].v);
            rhs.segment<3>(3) = -hailstones[0].p.cross(hailstones[0].v) + hailstones[2].p.cross(hailstones[2].v);
            M.block<3, 3>(0, 0) = crossMatrix(hailstones[0].v) - crossMatrix(hailstones[1].v);
            M.block<3, 3>(3, 0) = crossMatrix(hailstones[0].v) - crossMatrix(hailstones[2].v);
            M.block<3, 3>(0, 3) = -crossMatrix(hailstones[0].p) + crossMatrix(hailstones[1].p);
            M.block<3, 3>(3, 3) = -crossMatrix(hailstones[0].p) + crossMatrix(hailstones[2].p);
            Eigen::VectorXd result = M.inverse() * rhs;
            std::cout << std::fixed << "pos: " << std::endl << result.segment<3>(0) << std::endl << "vel: " << std::endl << result.segment<3>(3) << std::endl;
            BigNumber sum = 0;
            double sumD = 0.0;
            for (int i = 0; i < 3; i++)
            {
                // Round the results before summing them
                sum += BigNumber(std::round(result[i]));
                sumD += result[i];
            }
            std::cout << "The answer is " << sum << std::endl;

/*
            static const std::string Xr("A");
            static const std::string Yr("B");
            static const std::string Zr("C");

            static const std::string Vrx("D");
            static const std::string Vry("E");
            static const std::string Vrz("F");

            int i = 0;
            for (auto it = shapes.begin(); it < shapes.end(); it++)
            {
                std::cout << "( " << Xr << " - " << it->location_.x_ << " ) / ( " << it->velocity_.x_ << " - " << Vrx << " )" << " = " << "( " << Yr << " - " << it->location_.y_ << " ) / ( " << it->velocity_.y_ << " - " << Vry << " )";
                std::cout << std::endl;
                std::cout << "( " << Yr << " - " << it->location_.y_ << " ) / ( " << it->velocity_.y_ << " - " << Vry << " )" << " = " << "( " << Zr << " - " << it->location_.z_ << " ) / ( " << it->velocity_.z_ << " - " << Vrz << " )";
                std::cout << std::endl;
                i++;
                if (i > 3)
                {
                    break;
                }
            }
            std::cout << "The total is: " << total;
*/
        }

        std::cout << std::endl;
    }
}
