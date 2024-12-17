#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";

//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiterComma = ",";

struct Point
{
    float x;
    float y;
    float z;
};

struct BoundingBox
{
    Point min;
    Point max;
};

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator < (const Point& lhs, const Point& rhs)
{
    std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y) + ":" + std::to_string(lhs.z));
    std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y) + ":" + std::to_string(rhs.z));

    return lhsString < rhsString;
}


struct cmpPoints
{
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y) + ":" + std::to_string(lhs.z));
        std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y) + ":" + std::to_string(rhs.z));

        return lhsString < rhsString;
    }
};

class Cube
{
public:
    Cube()
        : connections_(0)
    {}
    
    Cube(float x, float y, float z)
        : connections_(0)
        , x_(x)
        , y_(y)
        , z_(z)
    {}

    bool checkNeighbors(Cube& other)
    {
        if (x_ == other.x_ && y_ == other.y_&& z_ == other.z_)
        {
            // comparing agains ourselves, ignore
            return false;
        }
        if (
               (x_ == other.x_ && y_ == other.y_ && abs(z_ - other.z_) == 1)
            || (x_ == other.x_ && z_ == other.z_ && abs(y_ - other.y_) == 1)
            || (y_ == other.y_ && z_ == other.z_ && abs(x_ - other.x_) == 1)
            )
        {
            connections_++;
            return true;
        }

        return false;
    }

    float x_;
    float y_;
    float z_;

    int connections_;
};


typedef std::vector<Cube> Cubes;


void readInputFile(std::string fileName, Cubes& cubes)
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
                        int x = 0;
                        int y = 0;
                        int z = 0;
                        size_t tokenPositionCommaX = line.find(delimiterComma, 0);
                        if (tokenPositionCommaX != std::string::npos)
                        {
                            x = std::stoi(line.substr(0, tokenPositionCommaX));
                            line.erase(0, tokenPositionCommaX + delimiterComma.length());

                            size_t tokenPositionCommaY = line.find(delimiterComma, 0);
                            if (tokenPositionCommaY != std::string::npos)
                            {
                                y = std::stoi(line.substr(0, tokenPositionCommaY));
                                line.erase(0, tokenPositionCommaY + delimiterComma.length());
                            }

                            if (!line.empty())
                            {
                                z = std::stoi(line);
                                cubes.emplace_back(x, y, z);
                            }

                        }
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

// Represent the faces of a cube by the middle point of each face
std::vector < Point > faceOffsets = { {0.0, 0.0, 0.5}, {0.0, 0.5, 0}, {0.5, 0.0, 0.0}, {0.0, 0.0, -0.5}, {0.0, -0.5, 0}, {-0.5, 0.0, 0.0} };

typedef std::map<Point, float, cmpPoints> Faces;


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Cubes cubes;

    Faces faces1;
    Faces faces2;


    readInputFile(inputFileName, cubes);


    if (cubes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            for (auto& cube : cubes)
            {
                for (auto& offset : faceOffsets)
                {
                    Point point;

                    point.x = cube.x_ + offset.x;
                    point.y = cube.y_ + offset.y;
                    point.z = cube.z_ + offset.z;
                    if (faces1.find(point) == faces1.end())
                    {
                        faces1[point] = 0;
                    }
                    faces1[point] += 1; // one more reference to this face
                }
            }

            size_t total = 0;
            for (auto& face : faces1)
            {
                // count of bigger than 1 (i.e. 2) means two boxe faces are touching
                if (face.second == 1)
                {
                    total++;
                }
            }



            std::cout << "Total surface area: " << total;
            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            BoundingBox boundingBox;
            boundingBox.max.x = std::numeric_limits<float>::min();
            boundingBox.max.y = std::numeric_limits<float>::min();
            boundingBox.max.z = std::numeric_limits<float>::min();

            boundingBox.min.x = std::numeric_limits<float>::max();
            boundingBox.min.y = std::numeric_limits<float>::max();
            boundingBox.min.z = std::numeric_limits<float>::max();

            std::set<Point> droplet;

            for (auto& cube : cubes)
            {
                Point point1;
                point1.x = cube.x_;
                point1.y = cube.y_;
                point1.z = cube.z_;

                // Contain all the points of the droplet
                droplet.emplace(point1);

                // Update bounding box dimensions
                boundingBox.max.x = std::max(cube.x_, boundingBox.max.x);
                boundingBox.max.y = std::max(cube.y_, boundingBox.max.y);
                boundingBox.max.z = std::max(cube.z_, boundingBox.max.z);

                boundingBox.min.x = std::min(cube.x_, boundingBox.min.x);
                boundingBox.min.y = std::min(cube.y_, boundingBox.min.y);
                boundingBox.min.z = std::min(cube.z_, boundingBox.min.z);

                for (auto& offset : faceOffsets)
                {
                    Point point;

                    point.x = cube.x_ + offset.x;
                    point.y = cube.y_ + offset.y;
                    point.z = cube.z_ + offset.z;
                    if (faces2.find(point) == faces2.end())
                    {
                        faces2[point] = 0;
                    }
                    faces2[point] += 1;
                }
            }

            // Make sure the bounding box is bigger than what it needs to contain the droplet
            boundingBox.max.x += 1;
            boundingBox.max.y += 1;
            boundingBox.max.z += 1;

            boundingBox.min.x -= 1;
            boundingBox.min.y -= 1;
            boundingBox.min.z -= 1;

            std::set<Point> air; // Keep track of all the cells with air around the droplet
            std::deque<Point> q; // To contain Breadth-first search algorithm

            // Start searching in the bounding box, but outside of the droplet
            // Therefore a good starting position is the minimum location (minimum of the bounding box)
            q.emplace_back(boundingBox.min);
            air.emplace(boundingBox.min);

            while (!q.empty())
            {
                Point point = q.front();
                q.pop_front();

                // Look at all the neighbors of our current location...
                for (auto& offset : faceOffsets)
                {
                    Point n;

                    n.x = point.x + 2 * offset.x; // 2x because we had the offsets to the faces, but we need to get to the neighbor that is at 2x distance
                    n.y = point.y + 2 * offset.y;
                    n.z = point.z + 2 * offset.z;

                    // Not inside the bounding box, no need to search
                    if (!((boundingBox.min.x <= n.x && n.x <= boundingBox.max.x)
                        && (boundingBox.min.y <= n.y && n.y <= boundingBox.max.y)
                        && (boundingBox.min.z <= n.z && n.z <= boundingBox.max.z)))
                    {
                        continue;
                    }
                    // Not part of the droplet, no need to search
                    if (droplet.find(n) != droplet.end())
                    {
                        continue;
                    }
                    // already found it
                    if (air.find(n) != air.end())
                    {
                        continue;
                    }
                    air.emplace(n);
                    q.push_back(n);
                }

            }

            // Create a set containing all the faces of the cells around the droplet
            std::set<Point> freeFaces;
            for (auto& airCell : air)
            {
                for (auto& offset : faceOffsets)
                {
                    Point air1;
                    air1.x = airCell.x + offset.x;
                    air1.y = airCell.y + offset.y;
                    air1.z = airCell.z + offset.z;

                    freeFaces.emplace(air1);
                }
            }

            int total = 0;
            // Count the number of free faces that are also part of the original faces of the droplet
            for (auto& face : faces2)
            {
                Point f;
                f.x = face.first.x;
                f.y = face.first.y;
                f.z = face.first.z;
                if (freeFaces.find(f) != freeFaces.end())
                {
                    total += 1;
                }
            }

            std::cout << "Total surface area: " << total;

            std::cout << std::endl;
        }
    }
}
