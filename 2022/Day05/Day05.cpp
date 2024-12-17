#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiter0 = " ";
static const std::string delimiter1 = "move ";
static const std::string delimiter2 = "from ";
static const std::string delimiter3 = "to ";

class Move
{
public:
    Move(int src, int dst, int cnt)
        : src_(src)
        , dst_(dst)
        , cnt_(cnt)
    {

    }

    int cnt_;
    int src_;
    int dst_;
};

typedef std::vector<std::deque<char>> Stacks;
typedef std::vector<Move> Moves;

void readInputFile(std::string fileName, Stacks& stacks, Moves& moves)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                    // Read stacks
                    while (!line.empty())
                    {
                        int  lineLength = line.length();
                        char ch = ' ';
                        if (lineLength > 0 && (line.find('[', 0) != std::string::npos))
                        {
                            for (int i = 1; i < lineLength;i += 4)
                            {
                                if (stacks.size() < 1 + i/4)
                                {
                                    stacks.emplace_back();
                                }
                                if (line[i] != ' ')
                                {
                                    stacks[i/4].push_back(line[i]);
                                }
                            }
                        }
                        getline(myFile, line);
                    }
                    break;

                case 2:
                    size_t tokenPosition = 0;
                    // Read moves
                    while (!line.empty())
                    {
                        int src = 0;
                        int dst = 0;
                        int cnt = 0;
                        tokenPosition = line.find(delimiter1, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiter1.length());
                            tokenPosition = line.find(delimiter0, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                std::string count = line.substr(0, tokenPosition);
                                line.erase(0, tokenPosition + delimiter0.length());
                                cnt =std::stoi(count);
                            }
                        }

                        tokenPosition = line.find(delimiter2, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiter2.length());
                            tokenPosition = line.find(delimiter0, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                std::string source = line.substr(0, tokenPosition);
                                line.erase(0, tokenPosition + delimiter0.length());
                                src =std::stoi(source);
                            }
                        }

                        tokenPosition = line.find(delimiter3, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiter3.length());
//                            tokenPosition = line.find(delimiter0, 0);
//                            if (tokenPosition != std::string::npos)
                            {
                                std::string destination = line; // line.substr(0, tokenPosition);
//                                line.erase(0, tokenPosition + delimiter0.length());
                                dst =std::stoi(destination);
                            }
                        }

                        moves.emplace_back(src, dst, cnt);

                        line = "";
                        getline(myFile, line);
                    }
                    break;
            }
            state++;
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Stacks stacks1;
    Stacks stacks2;
    Moves moves;

    readInputFile(inputFileName, stacks1, moves);

    if (stacks1.empty() || moves.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        for(auto stack: stacks1)
        {
            stacks2.emplace_back(stack);
        }

        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        for(auto& move : moves)
        {
            for(int i=0; i < move.cnt_; i++)
            {
                char item = stacks1[move.src_ - 1].front();
                stacks1[move.src_ - 1].pop_front();
                stacks1[move.dst_ - 1].push_front(item);
            }
        }
        for(auto& stack: stacks1)
        {
            std::cout << stack.front();
        }
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        for (auto& move : moves)
        {
            std::deque<char> items;
            for (int i = 0; i < move.cnt_; i++)
            {
                char item = stacks2[move.src_ - 1].front();
                stacks2[move.src_ - 1].pop_front();
                items.emplace_back(item);
            }
            for (int i = 0; i < move.cnt_; i++)
            {
                char item = items.back();
                items.pop_back();
                stacks2[move.dst_ - 1].push_front(item);
            }
        }
        for (auto& stack : stacks2)
        {
            std::cout << stack.front();
        }
        std::cout << std::endl;

    }
}
