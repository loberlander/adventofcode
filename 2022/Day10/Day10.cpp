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

enum Opcode
{
    noop,
    addx
};

class Instruction
{
public:
    Instruction(Opcode instruction, int amount)
        : opcode_(instruction)
        , amount_(amount)
    {}

    Opcode opcode_;
    int amount_;
};

typedef std::vector<Instruction> Instructions;

class Cpu
{
public:
    Cpu(Instructions& instructions)
        : instructions_(instructions)
        , isCycleDone_(true)
        , pc_(0)
        , x_(1)
        , cycle_(1)
        , signalStrengthsum_(0)
    {
        // Initialize the CRT
        for(int y = 0; y < 6; y++)
        {
            for(int x = 0; x < 40; x++)
            {
                crt_[y][x] = '.';
            }
        }
    }

    void execute()
    {
        while (pc_ < instructions_.size() )
        {
            execute(instructions_[pc_]);
        }
    }

    void execute(Instruction instruction)
    {
        int signalStrength = 0;

        switch (cycle_)
        {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
            {
                signalStrength = cycle_ * x_;
                signalStrengthsum_ += signalStrength;
                break;
            }
            case 219:
            {
                cycle_ = cycle_;
                break;
            }
        }

        int crtY = (cycle_ - 1) / 40;
        int crtX = (cycle_ - 1) % 40;
        if (crtX == x_)
        {
            crt_[crtY][crtX] = '#';
        }
        if (crtX == x_ - 1)
        {
            crt_[crtY][crtX] = '#';
        }
        if (crtX == x_ + 1)
        {
            crt_[crtY][crtX] = '#';
        }

        switch (instruction.opcode_)
        {
        case noop:
            {
                pc_++;
                break;
            }
        case addx:
            {
                if (isCycleDone_)
                {
                    isCycleDone_ = false;
                    cycleDone_ = cycle_ + 1;
                }
                else
                {
                    if (cycle_ == cycleDone_)
                    {
                        x_ += instruction.amount_;
                        pc_++;
                        isCycleDone_ = true;
                    }
                }
                break;
            }
        }

        cycle_++;
        
    }

    void display()
    {
        for(int y = 0; y < 6; y++)
        {
            for(int x = 0; x < 40; x++)
            {
                std::cout << crt_[y][x];
            }
            std::cout << std::endl;
        }
    }

    Instructions& instructions_;
    int pc_;
    int x_;
    int cycle_;
    int cycleDone_;
    bool isCycleDone_;
    int signalStrengthsum_;
    // std::vector<std::vector<char>> crt_;
    char crt_[6][40];
};

void parseLine(std::string& line, Instructions& instructions)
{
    while(line.size() > 0)
    {
        std::string inst = line.substr(0, 4);
        line.erase(0, 4);
        Opcode opcode = noop;
        int amount = 0;

        if (inst=="noop")
        {
            opcode = noop;
        }
        else
        {
            opcode = addx;
            amount = std::stoi(line);
        }
        instructions.emplace_back(opcode, amount);

        line.clear();
    }
}

void readInputFile(std::string fileName, Instructions& instructions)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                parseLine(line, instructions);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Instructions instructions;

    readInputFile(inputFileName, instructions);

    if (instructions.size() == 0)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        Cpu cpu(instructions);
        cpu.execute();
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        std::cout << "Signal strength: " << cpu.signalStrengthsum_;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        cpu.display();
        std::cout << std::endl;

    }
}
