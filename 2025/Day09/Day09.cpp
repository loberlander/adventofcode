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


typedef long long BigNumber;

class Point
{
    public:
        Point()
            : x_(0)
            , y_(0)
        {
        }

        Point(BigNumber x, BigNumber y)
            : x_(x)
            , y_(y)
        {
        }

        Point(const Point& point)
            : x_(point.x_)
            , y_(point.y_)
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

        BigNumber x()
        {
            return x_;
        }

        BigNumber y()
        {
            return y_;
        }

        std::string asString()
        {
            return std::to_string(x_) + ":" + std::to_string(y_);
        }

    private:
        BigNumber x_;
        BigNumber y_;
};

typedef std::vector<Point> Points;

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
                        auto ids = split(line, { "," });
                        if (ids.size() == 2)
                        {
                            Point point(std::stoll(ids[0]), std::stoll(ids[1]));
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

// Inside/outside polygon determination using ray casting
bool isPointInside(Points& points, Point& p)
{
    bool inside = false;

    for (auto i = 0; i < points.size(); i++)
    {
        // Two points of the polygon segments in order
        // Per rules set, one of the x or y coordinates are not changing, so we can optimize
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];

        if ((p.x() == p1.x() && p.x() == p2.x() && std::min(p1.y(), p2.y()) <= p.y() && p.y() <= std::max(p1.y(), p2.y())) or
            (p.y() == p1.y() && p.y() == p2.y() && std::min(p1.x(), p2.x()) <= p.x() && p.x() <= std::max(p1.x(), p2.x())))
        {
            // Point is on the polygon segment
            return true;
        }

        if ( ((p1.y() > p.y()) != (p2.y() > p.y())) && (p.x() < (p2.x() - p1.x()) * (p.y() - p1.y()) / (p2.y() - p1.y()) + p1.x() ))
        {
            // crossing over, one has to be higher than the other
            inside = !inside;
        }
    }

    return inside; //  crossings % 2 != 0; // return true when odd number of crossings
}


bool isIntersect(Points& polyLine, Points& rectLine)
{
    BigNumber L1x1 = std::min(polyLine[0].x(), polyLine[1].x());
    BigNumber L1y1 = std::min(polyLine[0].y(), polyLine[1].y());

    BigNumber L1x2 = std::max(polyLine[0].x(), polyLine[1].x());
    BigNumber L1y2 = std::max(polyLine[0].y(), polyLine[1].y());

    BigNumber L2x1 = std::min(rectLine[0].x(), rectLine[1].x());
    BigNumber L2y1 = std::min(rectLine[0].y(), rectLine[1].y());

    BigNumber L2x2 = std::max(rectLine[0].x(), rectLine[1].x());
    BigNumber L2y2 = std::max(rectLine[0].y(), rectLine[1].y());

    if (L1y1 == L1y2)
    {
        if (L2y1 < L1y1 && L1y1 < L2y2)
        {
            if ((std::max(L1x1, L1x2) > L2x1) && (std::min(L1x1, L1x2) < L2x2))
            {
                return true;
            }
        }
    }
    else
    {
        if (L2x1 < L1x1 && L1x1 < L2x2)
        {
            if ((std::max(L1y1, L1y2) > L2y1) && (std::min(L1y1, L1y2) < L2y2))
            {
                return true;
            }
        }
    }
    return false;
/*
    BigNumber intersection_low_x = std::max(L1x1, L2x1);
    BigNumber intersection_high_x = std::min(L1x2, L2x2);

    BigNumber intersection_low_y = std::max(L1y1, L2y1);
    BigNumber intersection_high_y = std::min(L1y2, L2y2);

    return intersection_low_x <= intersection_high_x && intersection_low_y <= intersection_high_y;
*/
}


// For a rectangle to be valid
// - All 4 corners of the rectangle must fall inside of the polygon (or be on the polygon)
// - No polygon edge passes through the rectangle interior (no polygon edges intersect the rectangle edges)

bool isRectagleValid(Points& points, Point& p1, Point& p2)
{
    // This is an overly zelous implementation to check each And every point of the rectangle to be inside the polygon
    /*
    for (auto x = std::min(p1.x(), p2.x()); x <= std::max(p1.x(), p2.x()); x++)
    {
        for (auto y = std::min(p1.y(), p2.y()); y <= std::max(p1.y(), p2.y()); y++)
        {
            Point testPoint(x, y);
            if (!isPointInside(points, testPoint))
            {
                return false;
            }
        }
    }
    */

    BigNumber x1 = std::min(p1.x(), p2.x());
    BigNumber y1 = std::min(p1.y(), p2.y());

    BigNumber x2 = std::max(p1.x(), p2.x());
    BigNumber y2 = std::max(p1.y(), p2.y());

    Points rectangleVertices{ {x1, y1}, {x1, y2}, {x2, y1}, {x2, y2} };

    for (auto& p : rectangleVertices)
    {
        if (!isPointInside(points, p))
        {
            return false;
        }
    }

    for (auto i = 0; i < points.size(); i++)
    {
        // Two points of the polygon segments in order
        // Per rules set, one of the x or y coordinates are not changing, so we can optimize
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        Points polyLine{ p1, p2 };

        for (auto i = 0; i < rectangleVertices.size(); i++)
        {
            Points rectLine{ rectangleVertices[i], rectangleVertices[(i + 1) % rectangleVertices.size()]};
            if (isIntersect(polyLine, rectLine))
            {
                return false;
            }
        }
    }

    return true;
}



int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Points points;

    readInputFile(inputFileName, points);

    if (points.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            // Check all points against every other points
            for (auto i = 0; i < points.size(); i++)
            {
                for (auto j = i + 1; j < points.size(); j++)
                {
                    BigNumber area = (std::abs(points[i].x() - points[j].x()) + 1) * (std::abs(points[i].y() - points[j].y()) + 1);
                    if (area > result)
                    {
                        result = area;
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            // Check all points against every other points
            for (auto i = 0; i < points.size(); i++)
            {
                for (auto j = i + 1; j < points.size(); j++)
                {
                    if (isRectagleValid(points, points[i], points[j]))
                    {
                        BigNumber area = (std::abs(points[i].x() - points[j].x()) + 1) * (std::abs(points[i].y() - points[j].y()) + 1);
                        if (area > result)
                        {
                            result = area;
                        }
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
