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

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

typedef long long BigNumber;

class Button
{
public:
    Button(std::string id, BigNumber x, BigNumber y)
        : id_(id)
        , x_(x)
        , y_(y)
    { }

    std::string id_;
    BigNumber x_;
    BigNumber y_;
};

typedef std::vector<Button> Buttons;

class Prize
{
public:
    Prize(BigNumber x, BigNumber y)
        : x_(x)
        , y_(y)
    {
    }

    BigNumber x_;
    BigNumber y_;
};

class Machine
{
public:
    Machine(Buttons buttons, Prize prize)
        : buttons_(buttons)
        , prize_(prize)
    { }

    Buttons buttons_;
    Prize prize_;
};

typedef std::vector<Machine> Machines;


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


void readInputFile(std::string fileName, Machines& machines)
{
    std::string line;
    std::ifstream myFile(fileName);
    std::vector<std::string> ButtonAString;
    std::vector<std::string> ButtonBString;
    std::vector<std::string> PrizeString;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Button A
                    if (!line.empty())
                    {
                        ButtonAString = split(line, {"Button ", ": X", ", Y"});
                        state++;
                    }
                    else
                    {
                        // state++;
                    }
                    break;
                }
                case 2:
                {
                    // Read Button B
                    if (!line.empty())
                    {
                        ButtonBString = split(line, { "Button ", ": X", ", Y" });
                        state++;
                    }
                    else
                    {
                        // state++;
                    }
                    break;
                }
                case 3:
                {
                    // Read Prize
                    if (!line.empty())
                    {
                        PrizeString = split(line, { "Prize: X=", ", Y=" });

                        if ((ButtonAString.size() == 3) && (ButtonBString.size() == 3) && (PrizeString.size() == 2))
                        {
                            machines.push_back({
                                { { ButtonAString[0], stoll(ButtonAString[1]), stoll(ButtonAString[2]) }, { ButtonBString[0], stoll(ButtonBString[1]), stoll(ButtonBString[2]) } },
                                { stoll(PrizeString[0]), stoll(PrizeString[1]) }
                                });
                        }

                        ButtonAString.clear();
                        ButtonBString.clear();
                        PrizeString.clear();
                        state = 1;
                    }
                    else
                    {
                        // state++;
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

bool isSolution(BigNumber a, BigNumber b, Machine& machine)
{
    if ((machine.buttons_[0].x_ * a + machine.buttons_[1].x_ * b == machine.prize_.x_) &&
        (machine.buttons_[0].y_ * a + machine.buttons_[1].y_ * b == machine.prize_.y_))
    {
        return true;
    }
    return false;
}


BigNumber findBest(Machine& machine)
{
    BigNumber result = 0;

    struct Solution
    {
        BigNumber xCount;
        BigNumber yCount;
        BigNumber tokens;
    };

    std::set<BigNumber> solutions;

    BigNumber maxAXCount = machine.prize_.x_ / machine.buttons_[0].x_;
    BigNumber maxAYCount = machine.prize_.y_ / machine.buttons_[0].y_;

    for (BigNumber a = 0; a < maxAXCount; a++)
    {
        BigNumber xALoc = a * machine.buttons_[0].x_;
        BigNumber xLeft = machine.prize_.x_ - xALoc;

        if (xLeft % machine.buttons_[1].x_ == 0)
        {
            BigNumber b = xLeft / machine.buttons_[1].x_;

            if (isSolution(a, b, machine))
            {
                solutions.emplace(a * 3 + b * 1);
            }
        }
    }

    if (!solutions.empty())
    {
        result = *solutions.begin();
    }

    return result;
}


// Linear equation system, solving for AA and BB
// Ax * AA + Bx * BB = X        // * By
// Ay * AA + By * BB = Y        // * Bx
//
// Ax * By * AA + Bx * By * BB = X * By
// Ay * Bx * AA + By * Bx * BB = Y * Bx     // subtract
//
// Ax * By * AA - Ay * Bx * AA = X * By - Y * Bx
//
// AA (Ax * By - Ay * Bx) = X * By - Y * Bx
// AA = (X * By - Y * Bx)/(Ax * By - Ay * Bx)
//
// BB = (X - Ax * AA) / Bx
//
BigNumber solve(Machine& machine)
{
    BigNumber result = 0;

    BigNumber Ax = machine.buttons_[0].x_;
    BigNumber Ay = machine.buttons_[0].y_;

    BigNumber Bx = machine.buttons_[1].x_;
    BigNumber By = machine.buttons_[1].y_;

    BigNumber X = machine.prize_.x_;
    BigNumber Y = machine.prize_.y_;

    BigNumber AA = (X * By - Y * Bx) / (Ax * By - Ay * Bx);
    BigNumber BB = (X - Ax * AA) / Bx;

    if (isSolution(AA, BB, machine))
    {
        result = (AA * 3 + BB * 1);
    }

    return result;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Machines machines;
    Machines machines2;

    readInputFile(inputFileName, machines);

    if (machines.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            for (auto& machine : machines)
            {
                //sum += findBest(machine);
                sum += solve(machine);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            machines2 = machines;
            for (auto& machine : machines2)
            {
                machine.prize_.x_ += 10000000000000;
                machine.prize_.y_ += 10000000000000;
            }

            BigNumber sum = 0;

            for (auto& machine : machines2)
            {
                //sum += findBest(machine);
                sum += solve(machine);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
