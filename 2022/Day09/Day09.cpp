#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiter = " ";

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

class Move
{
public:
    Move(Direction d, int amount)
        : d_(d)
        , amount_(amount)
    {}

    Direction d_;
    int amount_;
};

class Position
{
public:
    Position(int xx, int yy)
        : x(xx)
        , y(yy)
    {}
    int x;
    int y;
};


class Rope
{
public:
    Rope(int segments)
        : head_({0, 0})
        , tail_({ 0, 0})
        , segments_(segments)
    {
        for (int i = 0; i < segments_; i++)
        {
            tails_.emplace_back(0, 0);
        }
        track(tails_[segments_ - 1]);
    }

    Position distance(Position& head, Position& tail)
    {
        int dx = head.x - tail.x;
        int dy = head.y - tail.y;

        return Position({dx, dy});
    }

    void moveTail(Position& head, Position& tail)
    {
        // Move tail
        Position d = distance(head, tail);
        if (abs(d.x) > 1 || abs(d.y) > 1)
        {
            // Tail must move
            if (d.x > 1)
            {
                tail.x++;
                if (d.y >= 1)
                {
                    tail.y++;
                }
                else if (d.y <= -1)
                {
                    tail.y--;
                }
            }
            else if (d.x < -1)
            {
                tail.x--;
                if (d.y >= 1)
                {
                    tail.y++;
                }
                else if (d.y <= -1)
                {
                    tail.y--;
                }
            }
            else if (d.y > 1)
            {
                tail.y++;
                if (d.x >= 1)
                {
                    tail.x++;
                }
                else if (d.x <= -1)
                {
                    tail.x--;
                }
            }
            else if (d.y < -1)
            {
                tail.y--;
                if (d.x >= 1)
                {
                    tail.x++;
                }
                else if (d.x <= -1)
                {
                    tail.x--;
                }
            }
        }
    }

    void moveHead(Position& head, Move& move)
    {
        switch (move.d_)
        {
            case Up:
            {
                head_.y++;
                break;
            }
            case Down:
            {
                head_.y--;
                break;
            }
            case Left:
            {
                head_.x--;
                break;
            }
            case Right:
            {
                head_.x++;
                break;
            }
        }
    }

    void move(Move move)
    {
        for (int i = 0; i < move.amount_; i++)
        {
            //Move hesd
            moveHead(head_, move);

            // Move tail
            Position head = head_;
            for (int i = 0; i < segments_; i++)
            {
                moveTail(head, tails_[i]);
                head = tails_[i];
            }

            // Track the last segment only
            track(tails_[segments_ - 1]);
        }
    }

    void track(Position p)
    {
        int count = 0;
        std::string element = std::to_string(p.x) + ":" + std::to_string(p.y);
        auto it = tracker_.find(element);
        if (it != tracker_.end())
        {
            // Already visited...
            // Not part of the puzzle, but count the number of times it is visited
            count = it->second;
            count++;
            tracker_.erase(it);
        }

        tracker_.emplace(element, count);
    }

    size_t visited()
    {
        return tracker_.size();
    }

    Position head_;
    Position tail_;
    std::vector<Position> tails_;
    std::map<std::string, int> tracker_;
    int segments_;
};

typedef std::vector<Move> Moves;

void parseLine(std::string& line, Moves& moves)
{
    while(line.size() > 0)
    {
        size_t tokenPosition = 0;
        tokenPosition = line.find(delimiter, 0);
        if (tokenPosition != std::string::npos)
        {
            std::string direction = line.substr(0, tokenPosition);
            line.erase(0, tokenPosition + delimiter.length());
            int amount = std::stoi(line);
            line.clear();

            Direction d;
            if (direction == "U")
            {
                d = Up;
            }
            else if (direction == "D")
            {
                d = Down;
            }
            else if (direction == "L")
            {
                d = Left;
            }
            else if (direction == "R")
            {
                d = Right;
            }
            moves.emplace_back(d, amount);
        }
    }
}

void readInputFile(std::string fileName, Moves& moves)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                parseLine(line, moves);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Moves moves;
    Rope rope1(1);
    Rope rope9(9);

    readInputFile(inputFileName, moves);

    if (moves.size() == 0)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        for (auto& move : moves)
        {
            rope1.move(move);
        }
        std::cout << "Tail visited: " << rope1.visited();
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;

        for (auto& move : moves)
        {
            rope9.move(move);
        }
        std::cout << "Tail visited: " << rope9.visited();
        std::cout << std::endl;

    }
}
