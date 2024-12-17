#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiterStart = "Game ";
static const std::string delimiterGameNumber = ": ";
static const std::string delimiterGame = "; ";
static const std::string delimiterColor = " ";
static const std::string delimiterRed = "red";
static const std::string delimiterGreen = "green";
static const std::string delimiterBlue = "blue";

class Set
{
public:
    Set(int id, int red, int green, int blue, bool possible)
        : id_(id)
        , red_(red)
        , green_(green)
        , blue_(blue)
        , possible_(possible)
    {

    }

    int id_;
    int red_;
    int green_;
    int blue_;
    bool possible_;
};

class Game
{
public:
    Game(int id, std::vector<Set> sets, bool possible, int power)
        : id_(id)
        , sets_(sets)
        , possible_(possible)
        , power_(power)
    {

    }

    int id_;
    std::vector<Set> sets_;
    bool possible_;
    int power_;
};

typedef std::vector<Game> Games;

void processSet(std::string& game, int& red, int& green, int& blue)
{
    while (!game.empty())
    {
        size_t tokenPosition = 0;
        tokenPosition = game.find(delimiterColor, 0);
        std::string countAsString = game.substr(0, tokenPosition);
        game.erase(0, tokenPosition + delimiterColor.length());
        int count = std::stoi(countAsString);

        size_t tokenPositionRed = 0;
        size_t tokenPositionGreen = 0;
        size_t tokenPositionBlue = 0;

        std::string color;

        color = game.substr(0, delimiterRed.length());
        if (color == delimiterRed)
        {
            red = count;
            game.erase(0, delimiterRed.length());
        }

        color = game.substr(0, delimiterGreen.length());
        if (color == delimiterGreen)
        {
            green = count;
            game.erase(0, delimiterGreen.length());
        }

        color = game.substr(0, delimiterBlue.length());
        if (color == delimiterBlue)
        {
            blue = count;
            game.erase(0, delimiterBlue.length());
        }

        if (!game.empty() && game[0] == ',')
        {
            game.erase(0, 1);
        }

        if (!game.empty() && game[0] == ' ')
        {
            game.erase(0, 1);
        }
    }
}

void readInputFile(std::string fileName, Games& games, const int maxRed, const int maxGreen, const int maxBlue)
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
                {
                    // Read Game #
                    size_t tokenPosition = 0;
                    int id = 0;

                    tokenPosition = line.find(delimiterStart, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterStart.length());
                        tokenPosition = line.find(delimiterGameNumber, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            std::string idAsString = line.substr(0, tokenPosition);
                            line.erase(0, tokenPosition + delimiterGameNumber.length());
                            id = std::stoi(idAsString);
                        }
                    }

                    std::vector<Set> sets;
                    bool possibleGame = true;

                    // Read sets in the game
                    while (!line.empty())
                    {
                        int red = 0;
                        int green = 0;
                        int blue = 0;
                        bool possible = true;

                        std::string setAsString;

                        tokenPosition = line.find(delimiterGame, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            setAsString = line.substr(0, tokenPosition);
                            line.erase(0, tokenPosition + delimiterGame.length());
                        }
                        else
                        {
                            setAsString = line;
                            line.clear();
                        }
                        processSet(setAsString, red, green, blue);

                        if (red > maxRed)
                        {
                            possible = false;
                        }
                        if (green > maxGreen)
                        {
                            possible = false;
                        }
                        if (blue > maxBlue)
                        {
                            possible = false;
                        }

                        if (!possible)
                        {
                            possibleGame = false;
                        }

                        sets.emplace_back(id, red, green, blue, possible);
                    }

                    int redMax = 0;
                    int greenMax = 0;
                    int blueMax = 0;
                    for (auto& set : sets)
                    {
                        if (set.red_ > redMax)
                        {
                            redMax = set.red_;
                        }
                        if (set.green_ > greenMax)
                        {
                            greenMax = set.green_;
                        }
                        if (set.blue_ > blueMax)
                        {
                            blueMax = set.blue_;
                        }
                    }
                    games.emplace_back(id, sets, possibleGame, redMax * greenMax * blueMax);

                    break;
                }
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Games games;

    readInputFile(inputFileName, games, 12, 13, 14);

    if (games.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int sum1 = 0;
        for(auto& game : games)
        {
            if (game.possible_)
            {
                sum1 += game.id_;
            }
        }

        std::cout << "Possible game sum: " << sum1;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        int sum2 = 0;
        for (auto& game : games)
        {
            sum2 += game.power_;
        }

        std::cout << "Sum of game powers: " << sum2;
        std::cout << std::endl;

    }
}
