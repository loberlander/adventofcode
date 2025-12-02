#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <cmath>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";
//static const char inputFileName[] = "input_test6.txt";
//static const char inputFileName[] = "input_test7.txt";


enum Instructions
{
    adv = 0x00,
    bxl = 0x01,
    bst = 0x02,
    jnz = 0x03,
    bxc = 0x04,
    out = 0x05,
    bdv = 0x06,
    cdv = 0x07
};


typedef long long BigNumber;

class Machine
{
public:
    Machine()
        : registers_(3, 0)
        , insPointer_(0)
    { }

    Machine(BigNumber a, BigNumber b, BigNumber c)
        : registers_(3, 0)
        , insPointer_(0)
    {
        registers_[0] = a;
        registers_[1] = b;
        registers_[2] = c;
    }

    void setRegister(int index, BigNumber value)
    {
        registers_[index] = value;
    }

    void setRegister(std::string name, BigNumber value)
    {
        int index = 0;
        if (name == "A")
        {
            index = 0;
        }
        else if (name == "B")
        {
            index = 1;
        }
        else if (name == "C")
        {
            index = 2;
        }

        registers_[index] = value;
    }

    void setInstructions(std::vector<int> instructions)
    {
        instructions_ = instructions;
    }

    bool execute(size_t tillOutCount)
    {
        bool done = false;
        while (!done)
        {
            if (insPointer_ >= instructions_.size())
            {
                done = true;
                break;
            }

            Instructions ins = (Instructions) instructions_[insPointer_];
            insPointer_++;

            int operand = instructions_[insPointer_];
            insPointer_++;

            BigNumber comboOperand = 0;

            switch (operand)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                {
                    comboOperand = operand;
                    break;
                }
                case 4:
                case 5:
                case 6:
                {
                    comboOperand = registers_[operand - 4];
                }
                case 7:
                {
                    
                }
            }

            switch(ins)
            {
                case Instructions::adv:
                {
                    registers_[0] = registers_[0] / std::pow(2, comboOperand);
                    break;
                }
                case Instructions::bxl:
                {
                    registers_[1] = registers_[1] ^ operand; // bitwise XOR
                    break;
                }
                case Instructions::bst:
                {
                    registers_[1] = comboOperand % 8;
                    break;
                }
                case Instructions::jnz:
                {
                    if (registers_[0] != 0)
                    {
                        insPointer_ = operand;
                    }
                    break;
                }
                case Instructions::bxc:
                {
                    registers_[1] = registers_[1] ^ registers_[2]; // bitwise XOR
                    break;
                }
                case Instructions::out:
                {
                    if (!output_.empty())
                    {
                        output_ += ",";
                    }
                    output_ += std::to_string(comboOperand % 8);
                    if (tillOutCount != 0 && output_.size() >= tillOutCount)
                    {

                    }
                    break;
                }
                case Instructions::bdv:
                {
                    registers_[1] = registers_[0] / std::pow(2, comboOperand);
                    break;
                }
                case Instructions::cdv:
                {
                    registers_[2] = registers_[0] / std::pow(2, comboOperand);
                    break;
                }
            }
        }
        return done;
    }

    std::string output()
    {
        return output_;
    }

    std::vector<BigNumber> registers_;
    std::vector<int> instructions_;
    int insPointer_;
    std::string output_;
};


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

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
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

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    BigNumber x_;
    BigNumber y_;
};

typedef std::vector<Point> Points;
typedef Point Direction;
//typedef std::vector<Direction> Directions;

std::map<char, Direction> Directions =
{
    {'>', Direction(1, 0)},
    {'v', Direction(0, 1)},
    {'<', Direction(-1, 0)},
    {'^', Direction(0, -1)}
};


class Node
{
public:
    Node(BigNumber score, Point point, char direction, std::string path)
        : score_(score)
        , point_(point)
        , direction_(direction)
        , path_(path)
    {
    }

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    void nextDirection(bool isClockwise)
    {
        static const std::vector<char> DirectionOrder = { '>', 'v', '<', '^' };

        for (int i = 0; i < DirectionOrder.size(); i++)
        {
            if (DirectionOrder[i] == direction_)
            {
                direction_ = DirectionOrder[((isClockwise ? 1 : 3) +  i) % DirectionOrder.size()];
                return;
            }
        }
    }

    BigNumber score_;
    Point point_;
    char direction_;
    std::string path_;
};


struct NodeComparator
{
    //bool operator()(const Hand& lhs, const Hand& rhs) const;
    bool operator()(const Node& lhs, const Node& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs.score_ > rhs.score_)
        {
            return true;
        }

        return false;
    }
};


typedef std::map<std::string, Node> Visited;
typedef std::set<std::string> Seen;


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


void readInputFile(std::string fileName, Machine& machine, std::string& program)
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
                    // Read map
                    if (!line.empty())
                    {
                        auto registerString = split(line, { " ", ": " });

                        if (registerString.size() == 3)
                        {
                            machine.setRegister(registerString[1], stoll(registerString[2]));
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
                    if (!line.empty())
                    {
                        auto instructionString = split(line, { ": ", "," });

                        std::vector<int> instructions;

                        for (auto instruction : instructionString)
                        {
                            if (instruction != "Program")
                            {
                                instructions.emplace_back(stoi(instruction));
                                if (!program.empty())
                                {
                                    program += ",";
                                }
                                program += instruction;
                            }
                        }

                        machine.setInstructions(instructions);
                    }
                    else
                    {

                    }
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

    std::string program;
    Machine machine;

    readInputFile(inputFileName, machine, program);

    Machine machine1 = machine;
    Machine machine2 = machine;

    if (false)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;
            machine1.execute(0);

            std::cout << "Total: " << machine1.output();
            std::cout << std::endl;
            std::cout << "A:" << machine1.registers_[0] << " B:" << machine1.registers_[1] << " C:" << machine1.registers_[2];
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            auto outputSize = machine.instructions_.size();

            BigNumber i = 0;
            //BigNumber i = 330550451571142;
            int matchPtr = 0;
            bool done = false;
            Seen seen;

            /*
            Part 2:
            Searching for : 2, 4, 1, 1, 7, 5, 1, 5, 4, 0, 5, 5, 0, 3, 3, 0
                4 : 4 : 0
                37 : 45 : 3, 0
                298 : 452 : 3, 3, 0
                2390 : 4526 : 0, 3, 3, 0
                19124 : 45264 : 5, 0, 3, 3, 0
                152996 : 452644 : 5, 5, 0, 3, 3, 0
                1223974 : 4526446 : 0, 5, 5, 0, 3, 3, 0
                9791792 : 45264460 : 4, 0, 5, 5, 0, 3, 3, 0
                78334343 : 452644607 : 5, 4, 0, 5, 5, 0, 3, 3, 0

                Conclusions:
                - Octal representation is used in the code, so we are looking for octal numbers
                - The beginning (most significant bits) of Register A determine the end
                - For every outputted (octal) digit, there seems to be an octal value in Register A
                - To guide the brute force search, a good guess is that Register A needs to start with digits found to solve the end and must be the same octal length as what we are looking for
                - So find the last half of the program first and then move those values up where they need to be place value wise, and continue the search from there to find the rest of the digits
            */

            auto programLen = split(program, { "," }).size();

            std::cout << "Searching for: " << program;
            std::cout << std::endl;
            while (!done)
            {
                machine2 = machine;
                machine2.registers_[0] = i;
                machine2.execute(0);

                std::string result = machine2.output();

                if (program.ends_with(result))
                {
                    if (seen.find(result) == seen.end())
                    {
                        seen.emplace(result);
                        std::cout << std::dec << i << " : " << std::oct << i << " : " << result << std::dec;

                        std::cout << std::endl;

                        auto resultLength = split(result, { "," }).size();

                        if (resultLength == programLen / 2)
                        {
                            std::cout << "Adjusting number of digits from: " << std::dec << i << " : " << std::oct << i << std::dec << std::endl;
                            // It is time continue the search from the following value
                            i = i << (3 * (programLen - resultLength));
                            std::cout << "Adjusting number of digits to  : " << std::dec << i << " : " << std::oct << i << std::dec << std::endl;
                        }
                    }
                }

                if (program == result)
                {
                    break;
                }


                i++;
            }

            std::cout << std::dec << std::endl;
            std::cout << "Register A: " << i;
        }

        std::cout << std::endl;
    }
}
