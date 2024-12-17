#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

static const std::string delimiterMulStart = "mul(";
static const std::string delimiterOpSeparator = ",";
static const std::string delimiterEnd = ")";
static const std::string delimiterDo = "do()";
static const std::string delimiterDont = "don't()";

enum Token
{
    INV,
    MUL,
    DO,
    DONT
};

class Instruction
{
    public:
        Instruction()
            : token_(INV)
            , op1_(0)
            , op2_(0)
        { }

        Instruction(Token token, int op1, int op2)
            : token_(token)
            , op1_(op1)
            , op2_(op2)
        { }

        Token token_;
        int op1_;
        int op2_;
};

typedef std::vector<Instruction> Instructions;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
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


void readInputFile(std::string fileName, Instructions& instructions)
{
    std::string newLine;
    std::string line;
    std::ifstream myFile(fileName);
    Instruction instruction;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, newLine))
        {
            line += newLine;
        }

        while (!line.empty())
        {
            switch (state)
            {
                case 1:
                {
                    // Find beginning of Instructions
                    if (!line.empty())
                    {
                        size_t tokenPositionMul = 0;
                        tokenPositionMul = line.find(delimiterMulStart, 0);

                        size_t tokenPositionDo = 0;
                        tokenPositionDo = line.find(delimiterDo, 0);

                        size_t tokenPositionDont = 0;
                        tokenPositionDont = line.find(delimiterDont, 0);

                        if (tokenPositionDo != std::string::npos && tokenPositionDo < tokenPositionMul && tokenPositionDo < tokenPositionDont)
                        {
                            instruction.token_ = DO;
                            instruction.op1_ = 0;
                            instruction.op2_ = 0;
                            instructions.emplace_back(instruction);

                            line.erase(0, tokenPositionDo + delimiterDo.length());

                            break;
                        }

                        if (tokenPositionDont != std::string::npos && tokenPositionDont < tokenPositionMul && tokenPositionDont < tokenPositionDo)
                        {
                            instruction.token_ = DONT;
                            instruction.op1_ = 0;
                            instruction.op2_ = 0;
                            instructions.emplace_back(instruction);
                            line.erase(0, tokenPositionDont + delimiterDo.length());

                            break;
                        }

                        if (tokenPositionMul != std::string::npos)
                        {
                            instruction.token_ = MUL;
                            line.erase(0, tokenPositionMul + delimiterMulStart.length());

                            state++;
                        }
                        else
                        {
                            // No more valid token starts
                            instruction.token_ = INV;
                            line.clear();
                        }
                        
                        break;
                    }
                    else
                    {
                        // no more characters to process, we are done
                    }
                    break;
                }
                case 2:
                {
                    // Find operand #1
                    if (!line.empty())
                    {
                        size_t tokenPosition = 0;

                        tokenPosition = line.find(delimiterOpSeparator, 0);
                        if (tokenPosition != std::string::npos && tokenPosition <= 3)
                        {
                            instruction.op1_ = stoi(line.substr(0, tokenPosition));
                            line.erase(0, tokenPosition + delimiterOpSeparator.length());

                            state++;
                        }
                        else
                        {
                            state = 1; // Restart search
                        }

                        break;
                    }
                    else
                    {
                        // no more characters to process, we are done
                    }
                    break;
                }
                case 3:
                {
                    // Find operand #2
                    if (!line.empty())
                    {
                        size_t tokenPosition = 0;

                        tokenPosition = line.find(delimiterEnd, 0);
                        if (tokenPosition != std::string::npos && tokenPosition <= 3)
                        {
                            instruction.op2_ = stoi(line.substr(0, tokenPosition));
                            instructions.emplace_back(instruction);

                            line.erase(0, tokenPosition + delimiterEnd.length());

                            state = 1; // Restart search
                        }
                        else
                        {
                            state = 1; // Restart search
                        }

                        break;
                    }
                    else
                    {
                        // no more characters to process, we are done
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
    Instructions instructions;

    readInputFile(inputFileName, instructions);

    if (instructions.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;

            for (auto & inst : instructions)
            {
                switch (inst.token_)
                {
                    case MUL:
                    {
                        sum += inst.op1_ * inst.op2_;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;
            bool isEnabled = true;

            for (auto& inst : instructions)
            {
                switch (inst.token_)
                {
                    case DO:
                    {
                        isEnabled = true;
                        break;
                    }
                    case DONT:
                    {
                        isEnabled = false;
                        break;
                    }
                    case MUL:
                    {
                        if (isEnabled)
                        {
                            sum += inst.op1_ * inst.op2_;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}