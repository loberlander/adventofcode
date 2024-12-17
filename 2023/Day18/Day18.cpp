#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

static const std::string delimiter0 = " ";

typedef long long BigNumber;

class Element
{
public:
    Element(char direction, int distance, std::string color)
        : direction_(direction)
        , distance_(distance)
        , color_(color)
    {
        static const std::vector<char> dirMap2 = {'R', 'D', 'L', 'U'};

        int dirIndx2 = std::stoi(color_.substr(7, 1));
        distance2_ = std::stol(color_.substr(2, 5), nullptr, 16);
        direction2_ = dirMap2[dirIndx2];
    }

    char direction_;
    int distance_;
    std::string color_;
    int distance2_;
    char direction2_;
};

class Point
{
public:
    Point(BigNumber x, BigNumber y)
        : x_(x)
        , y_(y)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
    {}

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    BigNumber x_;
    BigNumber y_;
};

typedef std::vector<Element> Elements;

typedef std::vector<Point> Points;

typedef std::vector<std::string> Map;

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

void readInputFile(std::string fileName, Elements& elements)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                auto elementAsString = split(line, delimiter0);

                elements.emplace_back(elementAsString[0][0], std::stoi(elementAsString[1]), elementAsString[2]);
            }
        }
        myFile.close();
    }
}

void drawMap(Elements& elements, Points& points, BigNumber& n)
{
    BigNumber x = 0;
    BigNumber y = 0;
    points.emplace_back(x, y);

    n = 0;

    for (auto& element : elements)
    {
        BigNumber dx = 0;
        BigNumber dy = 0;
        switch (element.direction_)
        {
            case 'U':
            {
                dx = 0;
                dy = -1;
                break;
            }
            case 'D':
            {
                dx = 0;
                dy = 1;
                break;
            }
            case 'L':
            {
                dx = -1;
                dy = 0;
                break;
            }
            case 'R':
            {
                dx = 1;
                dy = 0;
                break;
            }
            default:
            {
                break;
            }
        }
        x += dx * element.distance_;
        y += dy * element.distance_;
        n += element.distance_;
        points.emplace_back(x, y);
    }
}

void drawMap2(Elements& elements, Points& points, BigNumber& n)
{
    BigNumber x = 0;
    BigNumber y = 0;
    points.emplace_back(x, y);

    n = 0;

    for (auto& element : elements)
    {
        BigNumber dx = 0;
        BigNumber dy = 0;
        switch (element.direction2_)
        {
        case 'U':
        {
            dx = 0;
            dy = -1;
            break;
        }
        case 'D':
        {
            dx = 0;
            dy = 1;
            break;
        }
        case 'L':
        {
            dx = -1;
            dy = 0;
            break;
        }
        case 'R':
        {
            dx = 1;
            dy = 0;
            break;
        }
        default:
        {
            break;
        }
        }
        x += dx * element.distance2_;
        y += dy * element.distance2_;
        n += element.distance2_;
        points.emplace_back(x, y);
    }
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Elements elements;

    readInputFile(inputFileName, elements);

    if (elements.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;
            BigNumber total = 0;

            Points points;
            BigNumber boundaryPoints;
            drawMap(elements, points, boundaryPoints);

            size_t numPoints = points.size();
            BigNumber A = 0;

            // https://en.wikipedia.org/wiki/Shoelace_formula (under Other formulas):
            for(int i = 0; i < numPoints; i++)
            {
                A += points[i].y_ * (points[(i - 1) % numPoints].x_ - points[(i + 1) % numPoints].x_);
            }
            A = std::abs(A) / 2;

            // Number of interior points, based on boundary points
            // https://en.wikipedia.org/wiki/Pick%27s_theorem

            total = A - boundaryPoints / 2 + 1;
            total += boundaryPoints;

            std::cout << "total: " << total;
        }
        std::cout << std::endl;

        {
            std::cout << "Part 2:" << std::endl;
            BigNumber total = 0;

            Points points;
            BigNumber boundaryPoints;
            drawMap2(elements, points, boundaryPoints);

            size_t numPoints = points.size();
            BigNumber A = 0;

            // https://en.wikipedia.org/wiki/Shoelace_formula (under Other formulas):
            for (int i = 0; i < numPoints; i++)
            {
                A += points[i].y_ * (points[(i - 1) % numPoints].x_ - points[(i + 1) % numPoints].x_);
            }
            A = std::abs(A) / 2;

            // Number of interior points, based on boundary points
            // https://en.wikipedia.org/wiki/Pick%27s_theorem

            total = A - (boundaryPoints / 2) + 1;
            total += boundaryPoints;

            std::cout << "total: " << total;
        }
        std::cout << std::endl;

    }
}
