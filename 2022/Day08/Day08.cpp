#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";



class Tree
{
public:
    Tree(int x, int y, int h)
        : x_(x)
        , y_(y)
        , h_(h)
        , scenicScore_(0)
        , visible_(false)
    {
    }

    int x_;
    int y_;
    int h_;
    size_t scenicScore_;
    bool visible_;
};

typedef std::vector<std::vector<Tree>> Trees;

bool checkVisibilityXY(Trees& trees, size_t xx, size_t yy)
{
    bool visible = false;
    size_t sizeX = trees[0].size();
    size_t sizeY = trees.size();
    size_t h = trees[yy][xx].h_;

    // edge trees are always visible
    if (xx == 0 || yy == 0 || xx == sizeX - 1 || yy == sizeY - 1)
    {
        return true;
    }

    visible = true;
    for(size_t y = 0; y < yy; y++)
    {
        if(trees[y][xx].h_ >= h)
        {
            visible = false;
            break;
        }
    }
    if (visible) return true;

    visible = true;
    for(size_t y = yy + 1; y < sizeY; y++)
    {
        if(trees[y][xx].h_ >= h)
        {
            visible = false;
            break;
        }
    }
    if (visible) return true;

    visible = true;
    for(size_t x = 0; x < xx; x++)
    {
        if(trees[yy][x].h_ >= h)
        {
            visible = false;
            break;
        }
    }
    if (visible) return true;

    visible = true;
    for(size_t x = xx + 1; x < sizeX; x++)
    {
        if(trees[yy][x].h_ >= h)
        {
            visible = false;
            break;
        }
    }

    return visible;
}

size_t calcScenicScoreXY(Trees& trees, size_t xx, size_t yy)
{
    bool visible = false;
    size_t scenicScore1 = 0;
    size_t scenicScore2 = 0;
    size_t scenicScore3 = 0;
    size_t scenicScore4 = 0;
    size_t sizeX = trees[0].size();
    size_t sizeY = trees.size();
    size_t h = trees[yy][xx].h_;

    // edge trees will always get a zero score
    if (xx == 0 || yy == 0 || xx == sizeX - 1 || yy == sizeY - 1)
    {
        return 0;
    }

    {
        int y = yy + 1;
        do
        {
            scenicScore1++;
            if (trees[y][xx].h_ >= h)
            {
                break;
            }
            y++;
        } while(y < sizeY);
        if (scenicScore1 == 0) return 0;
    }

    {
        int y = yy - 1;
        do
        {
            scenicScore2++;
            if (trees[y][xx].h_ >= h)
            {
                break;
            }
            y--;
        } while(y >= 0);
        if (scenicScore2 == 0) return 0;
    }

    {
        int x = xx + 1;
        do
        {
            scenicScore3++;
            if (trees[yy][x].h_ >= h)
            {
                break;
            }
            x++;
        } while(x < sizeX);
        if (scenicScore3 == 0) return 0;
    }

    {
        int x = xx - 1;
        do
        {
            scenicScore4++;
            if (trees[yy][x].h_ >= h)
            {
                break;
            }
            x--;
        } while(x >= 0);
        if (scenicScore4 == 0) return 0;
    }

    return scenicScore1 * scenicScore2 * scenicScore3 * scenicScore4;
}

void calcScores(Trees& trees)
{
    size_t sizeX = trees[0].size();
    size_t sizeY = trees.size();
    
    for(size_t y = 0; y < sizeY; y++)
    {
        for(size_t x = 0; x < sizeX; x++ )
        {
            trees[y][x].visible_ = checkVisibilityXY(trees, x, y);
            trees[y][x].scenicScore_ = calcScenicScoreXY(trees, x, y);
        }
    }
}

size_t countVisible(Trees& trees)
{
    size_t totalVisible = 0;
    size_t sizeX = trees[0].size();
    size_t sizeY = trees.size();
    
    for(size_t y = 0; y < sizeY; y++)
    {
        for(size_t x = 0; x < sizeX; x++ )
        {
            if (trees[y][x].visible_)
            {
                totalVisible++;
            }
        }
    }
    return totalVisible;
}

size_t findHighestScenicScore(Trees& trees)
{
    size_t scenicScoreMax = 0;
    size_t sizeX = trees[0].size();
    size_t sizeY = trees.size();
    
    for(size_t y = 0; y < sizeY; y++)
    {
        for(size_t x = 0; x < sizeX; x++ )
        {
            if (trees[y][x].scenicScore_ > scenicScoreMax)
            {
                scenicScoreMax = trees[y][x].scenicScore_;
            }
        }
    }
    return scenicScoreMax;
}

void parseLine(std::string& line, Trees& trees, int y)
{
    std::vector<Tree> row;
    int x = 0;
    int h = 0;

    while(line.size() > 0)
    {
        std::string height = line.substr(0, 1);
        line.erase(0, 1);
        h = std::stoi(height);
        row.emplace_back(x, y, h);
        x++;
    }

    trees.emplace_back(row);
}

void readInputFile(std::string fileName, Trees& trees)
{
    std::string line;
    std::ifstream myFile(fileName);
    int y = 0;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                parseLine(line, trees, y);
                y++;
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Trees trees;

    readInputFile(inputFileName, trees);
    calcScores(trees);

    if (trees.size() == 0)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        size_t total = countVisible(trees);

        std::cout << "total: " << total;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        size_t highestScenicScore = findHighestScenicScore(trees);
        std::cout << "highest scenic score: " << highestScenicScore;
        std::cout << std::endl;

    }
}
