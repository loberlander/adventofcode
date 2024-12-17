#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>

static const char inputFileName[] = "input.txt";

class Turn1
{
public:
    Turn1(std::string play)
        : myMove_(play.at(2))
        , theirMove_(play.at(0))
    {
        if (myMove_ == 'X') // Rock
        {
            myMove_ = 'A';
            score_ = 1;
        }
        if (myMove_ == 'Y') // Paper
        {
            myMove_ = 'B';
            score_ = 2;
        }
        if (myMove_ == 'Z') // Scossors
        {
            myMove_ = 'C';
            score_ = 3;
        }
        if (myMove_ == theirMove_) // Draw
        {
            score_ += 3;
        }
        else if ((myMove_ == 'A' && theirMove_ == 'C') // wins
            || (myMove_ == 'B' && theirMove_ == 'A')
            || (myMove_ == 'C' && theirMove_ == 'B'))
        {

            score_ += 6;
        }
        else // loses
        {

        }
    }

    char myMove_;
    char theirMove_;
    int score_;
};

class Turn2
{
public:
    Turn2(std::string play)
        : myMove_(play.at(2))
        , theirMove_(play.at(0))
    {
        if (myMove_ == 'X') // Must lose
        {
            switch (theirMove_)
            {
                case 'A':
                    myMove_ = 'C';
                    break;
                case 'B':
                    myMove_ = 'A';
                    break;
                case 'C':
                    myMove_ = 'B';
                    break;
            }
        }
        if (myMove_ == 'Y') // Draw
        {
            myMove_ = theirMove_;
        }
        if (myMove_ == 'Z') // Must win
        {
            switch (theirMove_)
            {
                case 'A':
                    myMove_ = 'B';
                    break;
                case 'B':
                    myMove_ = 'C';
                    break;
                case 'C':
                    myMove_ = 'A';
                    break;
            }
        }

        switch(myMove_)
        {
            case 'A':
                score_ = 1;
                break;
            case 'B':
                score_ = 2;
                break;
            case 'C':
                score_ = 3;
                break;
        }

        if (myMove_ == theirMove_) // Draw
        {
            score_ += 3;
        }
        else if ((myMove_ == 'A' && theirMove_ == 'C') // wins
            || (myMove_ == 'B' && theirMove_ == 'A')
            || (myMove_ == 'C' && theirMove_ == 'B'))
        {

            score_ += 6;
        }
        else // loses
        {

        }
    }

    char myMove_;
    char theirMove_;
    int score_;
};

typedef std::vector<Turn1> Turns1;
typedef std::vector<Turn2> Turns2;

void readInputFile(std::string fileName, Turns1& data1, Turns2& data2)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                // Add new elf's data
                data1.emplace_back(line);
                data2.emplace_back(line);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Turns1 data1;
    Turns2 data2;
    readInputFile(inputFileName, data1, data2);

    if (data1.empty() || data2.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int sum1 = 0;
        for(auto turn: data1)
        {
            sum1 += turn.score_;
        }
        std::cout << "Sum is:" << sum1 << std::endl;

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;

        int sum2 = 0;
        for(auto turn: data2)
        {
            sum2 += turn.score_;
        }
        std::cout << "Sum is:" << sum2 << std::endl;

    }
}
