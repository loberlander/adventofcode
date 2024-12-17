#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const std::string delimiter = " ";
static const std::string tokenAcc = "acc";
static const std::string tokenJmp = "jmp";
static const std::string tokenNop = "nop";

enum InstructionType {acc, jmp, nop};


// Format of input:
// acc +49
// jmp +274
// acc -49
// acc +49
// jmp +476
// jmp +409
struct Instruction
{
    InstructionType instruction;
    int operand;
    bool firstTime;
};

struct Instruction parseLine(std::string line)
{
    Instruction instruction;

    // Parse instruction token
    size_t tokenPosition = line.find(delimiter, 0);
    if (tokenPosition != std::string::npos)
    {
        std::string inst = line.substr(0, tokenPosition);
        if (inst == tokenAcc)
        {
            instruction.instruction = acc;
        }
        else if (inst == tokenJmp)
        {
            instruction.instruction = jmp;
        }
        else if (inst == tokenNop)
        {
            instruction.instruction = nop;
        }
        else
        {
            instruction.instruction = nop;
        }
        
        line.erase(0, tokenPosition + delimiter.length());

        instruction.operand = std::stoi(line.substr(0, line.length()));
    }

    instruction.firstTime = true;

    return instruction;
}

std::vector<Instruction> readInputFile(std::string fileName)
{
    std::vector<Instruction> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.push_back(parseLine(line));
        }
        myFile.close();
    }

    return data;
}

int execute(std::vector<Instruction> & data, bool & endReached)
{
    int accumulator = 0;
    int location = 0;
    size_t lastLocation = data.size() - 1;
    bool done = false;
    endReached = false;

    while (!done)
    {
        if ((location <= lastLocation) && (data[location].firstTime))
        {
            data[location].firstTime = false;
            switch (data[location].instruction)
            {
            case acc:
                accumulator += data[location].operand;
                ++location;
                break;
            case jmp:
                location += data[location].operand;
                break;
            case nop:
                ++location;
            default:
                break;
            }
        }
        else
        {
            done = true;
        }
    }

    if (location > lastLocation)
    {
        endReached = true;
    }

    return accumulator;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<Instruction> data = readInputFile(inputFileName);
    std::vector<Instruction> program = data;
    bool endReached = false;

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        std::cout << "Accumulator = " << execute(program, endReached) << std::endl;

        std::cout << "Part 2:" << std::endl;
        int accumulator = 0;
        int lastPoke = -1;
        size_t lastLocation = data.size();
        while (!endReached)
        {
            program = data; // Copy original program

            for (int i = lastPoke + 1; i < lastLocation; i++)
            {
                if (program[i].instruction == nop)
                {
                    program[i].instruction = jmp;
                    lastPoke = i;
                    break;
                }
                else if (program[i].instruction == jmp)
                {
                    program[i].instruction = nop;
                    lastPoke = i;
                    break;
                }
            }

            accumulator = execute(program, endReached);
        }

        std::cout << "Accumulator = " << accumulator << std::endl;
    }
}
