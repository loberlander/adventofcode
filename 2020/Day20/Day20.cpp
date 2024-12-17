#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "inputTest.txt";

static const std::string delimiterTileIdStart = "Tile ";
static const std::string delimiterTileIdEnd = ":";

class Tile
{
public:
    Tile(int id, std::string& top, std::string& bottom, std::string& left, std::string& right, std::vector<std::string> image)
        : id_(id)
        , top_(top)
        , bottom_(bottom)
        , left_(left)
        , right_(right)
        , idTop_(-1)
        , idBottom_(-1)
        , idLeft_(-1)
        , idRight_(-1)
        , neighbors_(4)
        , image_(image)
        , transformRotate_(0)
        , transformFlip_(0)
    {}

    void rotate()
    {
        // rotate 90 degrees clockwise
        {
            // rotate the edge pieces
            std::swap(top_, right_);
            std::swap(bottom_, left_);
            std::swap(bottom_, top_);
            std::reverse(top_.begin(), top_.end());
            std::reverse(bottom_.begin(), bottom_.end());
        }

        transformRotate_++;
    }

    void flip()
    {
        // flip the edge pieces (top<->bottom)
        {
            std::swap(top_, bottom_);
            std::reverse(right_.begin(), right_.end());
            std::reverse(left_.begin(), left_.end());
        }

        transformFlip_++;
    }

    int id_;
    int idTop_;
    int idBottom_;
    int idLeft_;
    int idRight_;
    int neighbors_;
    std::string top_;
    std::string bottom_;
    std::string left_;
    std::string right_;
    std::vector<std::string> image_;
    int transformRotate_;
    int transformFlip_;
};

typedef std::map<int, Tile> Tiles;
typedef std::multimap<std::string, Tile> SortedTiles;
typedef std::vector<std::vector<Tile>> TilesGrid;

void readInputFile(std::string fileName, Tiles& tiles)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int i = 0;
        int id = 0;
        std::string top;
        std::string bottom;
        std::string left;
        std::string right;
        std::vector<std::string> image;

        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                switch(i)
                {
                    case 0:
                    {
                        size_t tokenPosition = 0;

                        tokenPosition = line.find(delimiterTileIdStart, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterTileIdStart.length());

                            tokenPosition = line.find(delimiterTileIdEnd, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                id = std::stoi(line.substr(0, tokenPosition));
                            }
                            top.clear();
                            bottom.clear();
                            left.clear();
                            right.clear();
                            image.clear();
                            i = 1;
                        }
                        break;
                    }
                    case 1:
                    {
                        top = line;
                        left += line[0];
                        right += line[line.size()-1];
                        i = 2;
                        break;
                    }
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    {
                        image.emplace_back(line.substr(1, line.size()-2));
                        left += line[0];
                        right += line[line.size()-1];
                        i++;
                        break;
                    }
                    case 10:
                    {
                        bottom = line;
                        left += line[0];
                        right += line[line.size()-1];

                        tiles.emplace(id, Tile(id, top, bottom, left, right, image));
                        i = 0;
                        break;
                    }
                }
            }
        }
        myFile.close();
    }
}

int sqrtInt(unsigned int x)
{
    // Based on (n + 1)^2 = n^2 + 2n +1
    unsigned long a = 1;
    unsigned long delta = 3;
    while (a <= x)
    {
        a += delta; // (a + 1)^2
        delta += 2;
    }
    return delta / 2 - 1;
}


Tiles findCorners(Tiles& tiles)
{
    Tiles corners;
    for(auto& tileIt: tiles)
    {
        Tile &tile = tileIt.second;
        if (tile.neighbors_ == 2)
        {
            corners.emplace(tile.id_, tile);
        }
    }

    return corners;
}

void sortTiles(Tiles& tiles, SortedTiles& sortedTiles)
{
    // Preprocess #1
    // Place (Sort) the tiles by matching sides (including the reverse of each side)
    for(auto& tileIt: tiles)
    {
        Tile& tile = tileIt.second; 

        std::string top = tile.top_;
        std::string bottom = tile.bottom_;
        std::string left = tile.left_;
        std::string right = tile.right_;

        sortedTiles.emplace(top, tile);
        std::reverse(top.begin(), top.end());
        sortedTiles.emplace(top, tile);

        sortedTiles.emplace(bottom, tile);
        std::reverse(bottom.begin(), bottom.end());
        sortedTiles.emplace(bottom, tile);

        sortedTiles.emplace(left, tile);
        std::reverse(left.begin(), left.end());
        sortedTiles.emplace(left, tile);

        sortedTiles.emplace(right, tile);
        std::reverse(right.begin(), right.end());
        sortedTiles.emplace(right, tile);
    }
}

void markNeighbors(Tiles& tiles, SortedTiles& sortedTiles)
{
    // Preprocess #2
    // Find out how many neighbors a tile might have
    for(auto& tileIt: tiles)
    {
        Tile &tile = tileIt.second;
        tile.neighbors_ = 4;

        {
            auto range = sortedTiles.equal_range(tile.top_);
            auto d = std::distance(range.first, range.second);
            if ( d == 1)
            {
                // Found an edge piece, update it
                tile.idTop_ = 0;
                tile.neighbors_--;
            }
        }
        {
            auto range = sortedTiles.equal_range(tile.bottom_);
            auto d = std::distance(range.first, range.second);
            if (d == 1)
            {
                // Found an edge piece, update it
                tile.idBottom_ = 0;
                tile.neighbors_--;
            }
        }
        {
            auto range = sortedTiles.equal_range(tile.left_);
            auto d = std::distance(range.first, range.second);
            if (d == 1)
            {
                // Found an edge piece, update it
                tile.idLeft_ = 0;
                tile.neighbors_--;
            }
        }
        {
            auto range = sortedTiles.equal_range(tile.right_);
            auto d = std::distance(range.first, range.second);
            if (d == 1)
            {
                // Found an edge piece, update it
                tile.idRight_ = 0;
                tile.neighbors_--;
            }
        }
    }
}


unsigned long long solvePuzzle1(Tiles& tiles, SortedTiles& sortedTiles)
{
    // Preprocess #1
    sortTiles(tiles, sortedTiles);
    
    // Preprocess #2
    markNeighbors(tiles, sortedTiles);

    // Find the corner pieces
    Tiles corners = findCorners(tiles);

    // Calculate the product of the corners
    if (corners.size() == 4)
    {
        unsigned long long product = 1;
        for (auto& tileIt: corners)
        {
            product *= tileIt.second.id_;
        }
        return product;
    }

    return 0;
}


int solvePuzzle2(Tiles& tiles, SortedTiles& sortedTiles, TilesGrid& puzzle)
{
    // Assumes solvePuzzle1() was already executed

    // Find the corner pieces
    Tiles corners = findCorners(tiles);

    //std::vector<Tile> row;

    // Find the best corner to start with
    int id = 0;

    if (corners.size() == 4)
    {
        for (auto& tileIt: corners)
        {
            Tile &tile = tileIt.second;
            id = tile.id_;
            if (tile.idTop_ == 0 && tile.idLeft_ == 0)
            {
                // Found an ideal top left corner
                break;
            }
        }
    }

    auto tileIt = tiles.find(id);
    tileIt->second.rotate();
    /*
    while (tiles[id].idTop_ != 0 || tiles[id].idLeft_ != 0)
    {
        // Found an ideal top left corner
        // Let's rotate/flip the corner we got until it becomes the top left...
        tiles[id].rotate();
    }
    */
    //row.emplace_back()

    return 0;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Tiles tiles;
    SortedTiles sortedTiles;
    TilesGrid puzzle;

    readInputFile(inputFileName, tiles);

    if (tiles.size() == 0)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        unsigned long long result1 = solvePuzzle1(tiles, sortedTiles);
        std::cout << "Product of corners: " << result1;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        int result2 = solvePuzzle2(tiles, sortedTiles, puzzle);
        std::cout << "Water roughness: " << result2;
        std::cout << std::endl;

    }
}
