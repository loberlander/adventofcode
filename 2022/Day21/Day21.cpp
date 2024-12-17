#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <algorithm>


/*

for part 2:
Take the output from part 2, (edit it if desired) give it to an online solver like this one:
https://quickmath.com/


((((((((3*(((((((2*(4+(3*(3*3))))+18)+13)+((2*((3+(17*3))-(((5*2)+5)+2)))-10))*2)/2)*3))*((((((((((4*(((1+(4+2))+20)+(5*2)))+((((5+(13*2))*4)+(((7*2)/2)*((3*3)*3)))*((2*((3+4)+3))-3)))/3)-(11*((11*3)-(4*2))))/2)+(19*((4*2)+3)))*(((((2*17)*(19-2))/2)+((2*(2*(2+(((2*5)*3)-3))))*2))*2))+((5*((((5*((2+5)*(5*5)))+(((((2*(((2+(2*4))+1)*2))+((2*3)+((4*2)+3)))+(((6*((2*3)+3))+9)+((7+(6+((9*3)+3)))+(4*11))))*3)*2))*2)+((3*(2+5))*((3*(3+6))+4))))*(7*3)))*((5*((2*(4+9))+11))+(2*(((3+(5*2))*5)+((2*(20+3))*14)))))+((((7*3)+(2*5))*((((9*(12+(13+(6*4))))/(2+5))+(2*((((9+(19+((2*4)+3)))+4)*2)+((((3*5)+2)+2)+(2*(((((11+2)*5)+5)/5)-3))))))*((9*((((16+(9*4))/2)*((6*7)+4))+(((3*3)*5)*((((1+(((7+(3+3))*2)/2))+9)+20)*3))))-((((((5+4)*2)*11)+(2*13))+(((7*(1+6))*(2+(1+(20+8))))+(((((((2*((18+5)+(((2*(2*3))+5)*2)))+((3+8)*4))*2)/4)*3)*2)+(2*((((2+((1+((2*4)*4))/3))+(2*((5+(2*3))*3)))*5)+((9*((2*4)+3))+(((4*(((4*4)+(7*5))/3))-(((6*6)/2)-4))+(((3*3)*3)*5))))))))*4))))+((10+((2*((((7*2)*2)+10)/2))/2))*(((2+(3*7))+((7*5)+1))*(((3*(((5+(3*(7-1)))*2)+(9*13)))*(3*8))+((7*5)*(5*(10+13)))))))))+(((17+6)*(((((7*(2*3))+((14+9)+14))*18)+((((((3+19)+(3*((4+(2+9))+(5*2))))+10)*(11+12))+((3+(2*17))*(2*(((2*((2*5)+(3*3)))*2)-(15+(2*4))))))-(((2*((((16+5)*5)+(((2*6)-1)*2))+((((((((4+19)+(1+(((4+3)*2)+(3*5))))*5)*2)/5)/2)-12)*2)))+(2*((2*(((2*(2*((3+20)+(8-2))))-(((5+4)*2)+5))+10))+(5*((4*(1+(2*5)))+(5*7))))))/2)))+((((2*3)*(3*(11+12)))+(6*(5*3)))*(((((2*(5+3))-5)*2)+(2*3))*((11+7)-1)))))*((((2*(3*7))-11)*4)*(2*((((2*(2*(((((16+(2*5))+11)+(12*2))*2)+(((2*4)+(7*5))*3))))-(((11*5)*2)/2))+((2*(5*(((10*6)-13)+14)))+(5*(((4*2)*2)+3))))+(((18*6)+(2*(13*17)))+((((17*2)-11)*3)+(2*(((17*5)+11)+1)))))))))*(((5+2)*3)+(3+4)))-(4*(((((((2*((2*(((13*20)+((((((3*4)+5)+((3*2)+4))+(4*8))*7)+((2*9)*(((7*7)/(5+2))-1))))+(((((6+(5+2))+12)*((6+5)+(13*2)))+((4*(((((3*3)*(18+5))+(((((4*(((17*19)*2)+((((((4*2)*(((((((((5*17)+((4*2)*(14/2)))/3)+((3*3)*3))+(17+((6+(8*2))+14)))*(4+3))+((((((2*(4+(5+2)))*11)+(((5*(2*5))/2)*5))+(8*(2*(3+5))))+((4*((2+4)*2))+(4+((5*3)*5))))+((((((((((((11*2)+9)+(((((2*((2*(7+4))+15))*2)*(2*7))/7)/2))*2)/2)+(4*(((((3*5)-5)+((5*3)-2))*2)-3)))+((2*((2*((3*(((4*2)*2)+3))+((((11*2)*2)*2)+(((((2*(((3*2)+1)*5))/2)+18)+(3+(10*2)))-(2*5)))))/2))+(((19-6)*5)*3)))+((2*((2*((((((2*((((((12*(((((((((6*(11+(14*3)))*(((4*(3*2))*((4*2)*4))+((x-((2*((2*(((((((2*(3*19))+(5*((2*(20+(((4+3)*2)+9)))/2)))/(4+3))*2)/2)*2)/2))-((1+6)*3)))*6))/9)))+((7+6)*(5*5)))/(4+7))-((2*17)*((5*(4+3))-(4*2))))*2)+(((((7*2)+10)-3)*(2*(20+(2+(7*5)))))/3))/2)-(((17*17)*2)+(((5*2)+((((3*((2*3)*2))+(1+(2*3)))+(10+14))+((5+5)*(16+3))))+(6*10)))))-((4*2)*((3*3)+(5*17))))/2)+((((2*(7+4))*2)*5)+(((2*3)*(2+4))*13)))*2)+(((3*2)*9)*13)))-((((3*3)*2)+16)*((3*2)+(3+4))))/2)+(((2*(((((3*14)+17)*2)/2)*2))-(9+((2*4)+(2*13))))*3))/4)-((5*5)+(2*((((17*4)-17)+2)+(2*(11+14)))))))-((((5*2)*5)+(5*9))+(((7*((1+(3*2))*2))/7)*3))))+((1+(((2*3)+(10+((8-1)+6)))*3))+15)))/(3*3))+((5*5)*2))*2)-(3*13))))/(3*2))+(2*((10*(2*4))+17))))-(2*(4+(((4*((2*3)+1))/4)*7))))/2)-(3*(((((((2*(7*3))+5)-(3*2))-(3*4))*2)/2)-(2*3))))/2)))+(((17+(10*3))-4)+((((8*3)+5)+(3*((4*8)-1)))+((8*7)+((3*(5*3))+((2*4)*6))))))/2)-((7*((14+((4*2)+(9+2)))+((13+4)*2)))*2))*(5+2)))/(2*3))-6))-(5*((((2*9)+5)+1)+((((((4*11)-3)*5)/5)*3)+(8*4))))))/2)))+(3*(((12*(1+5))+((((3*5)+4)*4)+10))+(5*(7*3))))))-(((15*(2+(((3*2)*4)/2)))+(5*(((4+3)+(18*5))+((2*3)*4))))+((9+(5*((9*(3*2))/2)))/3)))/3)-(3*(((3*(5+(4*4)))+15)+(4*(((8-1)+17)+7)))))+(17+(2*(((3+(4*2))+12)*4))))/2)+8)))/4)+((2+4)*(2*((2*(2+(3*3)))+((((((11+((20*4)-(((4*(8*3))/3)-10)))-18)-(2*5))-12)+6)+2)))))*2)
=
((((((((14+(((5+2)*7)-(6*2)))*(13*2))-((((2*(5*5))+(3*8))+(12-1))+((7*5)+((((2*19)*4)/2)+12))))+((((2*4)*((19+(5+(3*2)))+16))+((2*(((3*(((((2*5)+9)+(5*2))*4)+(17+(((4*(3+(2*7)))*8)/4))))-(5*7))-(3*((((3*(4+3))-2)*2)+5))))+((5*9)*19)))+(3*(((19*2)-1)*2))))+((10+(6+(4+2)))*(((4*((7*(5+6))/7))*2)+((8*(((4+(6+1))*5)+(4*4)))+(3*12)))))*(((13*2)*5)+(7*(((3*(((3*(4*2))+(3+19))/2))/3)+8))))+((((2*(((((2*((((3*9)-8)+((((2*(((4*10)/2)+((11+2)+((3*((3*15)-11))/3))))/2)+((17*((4+2)+1))+(3*((1+8)*6))))/4))/2))*(((11+6)+6)+1))+(((((6+(6+(((((3*3)+(4*2))*(5*2))/2)/5)))*3)+(((4+(3+(4*4)))+(((5*5)*((16+4)-4))/(2*4)))*2))+((6+4)*((3*4)+(8+((11+2)*3)))))*(2+9)))*2)+(13*((((((((11*((5*5)+12))+(6*(2*((2+(2*(5*5)))+(5*((((4*4)*3)-16)-9))))))+((5+9)*(3*(3*3))))*4)+(((11+(6*3))*(12*2))+(((2*(3*((3*9)+10)))+((((((5*3)+16)*3)-(5*3))+(1+10))*2))+(3*((((5*((7*13)/(4+3)))*(3+(4*2)))+((3*(2*4))+(((8+3)*2)+(3+4))))+((((5*((5*11)+(2*((13*9)+(4*(7*3))))))+((5+4)*(9*3)))/2)+(((6*((14+2)-5))/6)*((8+2)+(13+(3*(11*2)))))))))))/5)-((((18*((1+(5+5))*2))/2)-5)*7))/2))))*4)+(((((8*3)+11)*((2+9)*3))+((((((4+2)+(((5*(3*(1+(5+1))))/5)+2))+4)*2)*(5+(3*(7*3))))+((2*(((5*((2*(5+2))/2))+((3*3)*2))+(10*5)))*(2*5))))*19))+((((3*(11+(3*4)))+(((2*((((2*3)+1)*(3+4))+((2*((2*((((2+9)+20)*2)/2))/2))*2)))-((6+(6+(4+3)))*5))*2))*2)*((1+(3+(1+(1+5))))*3))))*(((3+((3*3)+2))/2)*((((((10+(7*(1+6)))*11)*(2*(((2*(((((8+5)*4)+12)+(6+1))*2))/2)/2)))+(((3*(2+5))*3)*(3*(((((2+11)*2)+(3*7))*2)/2))))+((3*(((((3*(((((9*3)+20)*3)*3)-(2*13)))+(13*(2*((((1+7)+1)*18)+((17+6)*((1+5)+17))))))*2)/2)*2))-((((2*4)*4)/2)*(((11+(2*(3*13)))*11)+(2*(4*((5+((4*2)*2))+(2*(13+(3*2))))))))))*5)))


the answer is 3342154812537

*/

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";
static const std::string delimiterComma = ",";
static const std::string delimiterColon = ": ";
static const std::string delimiterSpace = " ";


static const std::string idRoot = "root";
static const std::string idHuman = "humn";

class Instruction
{
public:
    Instruction()
        : valueKnown_(false)
        , value_(0)
        , operation_(' ')
    {

    }

    void setValue(long long value)
    {
        value_ = value;
        valueString_ = std::to_string(value);
        valueKnown_ = true;
    }

    void setValue(std::string value)
    {
        valueString_ = value;
        valueKnown_ = true;
    }

    std::string id_;
    bool valueKnown_;
    long long value_;
    char operation_;
    std::string op1_;
    std::string op2_;
    std::string valueString_;
};

typedef std::map<std::string, Instruction> Instructions;

bool parseLine(std::string& line, Instruction& instruction)
{
    // Set defaults
    instruction.value_ = 0;
    instruction.operation_ = ' ';

    size_t tokenPositionColon = line.find(delimiterColon, 0);
    if (tokenPositionColon != std::string::npos)
    {
        instruction.id_ = line.substr(0, tokenPositionColon);
        line.erase(0, tokenPositionColon + delimiterColon.length());

        size_t tokenPositionSpace = line.find(delimiterSpace, 0);
        if (tokenPositionSpace != std::string::npos)
        {
            // Instructions found
            instruction.op1_ = line.substr(0, tokenPositionSpace);
            line.erase(0, tokenPositionSpace + delimiterSpace.length());

            tokenPositionSpace = line.find(delimiterSpace, 0);
            if (tokenPositionSpace != std::string::npos)
            {
                instruction.operation_ = line.substr(0, tokenPositionSpace)[0];
                line.erase(0, tokenPositionSpace + delimiterSpace.length());

                instruction.op2_ = line;
            }
        }
        else
        {

            instruction.setValue(std::stoi(line));
        }
    }
    

    return true;
}

void readInputFile(std::string fileName, Instructions& instructions)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                switch (state)
                {
                    case 1:
                    {
                        Instruction instruction;
                        if (parseLine(line, instruction))
                        {
                            instructions.emplace(instruction.id_, instruction);
                        }
                        break;
                    }
                }
            }
            else
            {
                state = 1;
            }
        }
        myFile.close();
    }
}

long long evaluate1(Instructions& instructions, Instruction& currentInst )
{
    if (currentInst.valueKnown_)
    {
        return currentInst.value_;
    }

    switch(currentInst.operation_)
    {
        case '+':
        {
            Instruction& op1Inst = instructions[currentInst.op1_];
            Instruction& op2Inst = instructions[currentInst.op2_];
            currentInst.setValue(evaluate1(instructions, op1Inst) + evaluate1(instructions, op2Inst));
            break;
        }
        case '-':
        {
            Instruction& op1Inst = instructions[currentInst.op1_];
            Instruction& op2Inst = instructions[currentInst.op2_];
            currentInst.setValue(evaluate1(instructions, op1Inst) - evaluate1(instructions, op2Inst));
            break;
        }
        case '*':
        {
            Instruction& op1Inst = instructions[currentInst.op1_];
            Instruction& op2Inst = instructions[currentInst.op2_];
            currentInst.setValue(evaluate1(instructions, op1Inst) * evaluate1(instructions, op2Inst));
            break;
        }
        case '/':
        {
            Instruction& op1Inst = instructions[currentInst.op1_];
            Instruction& op2Inst = instructions[currentInst.op2_];
            currentInst.setValue(evaluate1(instructions, op1Inst) / evaluate1(instructions, op2Inst));
            break;
        }
        default:
            // should never get here
            int test = 0;
            break;
    }

    return currentInst.value_;
}


std::string evaluate2(Instructions& instructions, Instruction& currentInst)
{
    if (currentInst.id_ == idHuman)
    {
        return "x";
        //return currentInst.id_;
    }

    if (currentInst.valueKnown_)
    {
        return currentInst.valueString_;
    }

    switch (currentInst.operation_)
    {
    case '+':
    {
        Instruction& op1Inst = instructions[currentInst.op1_];
        Instruction& op2Inst = instructions[currentInst.op2_];
        currentInst.setValue("(" + evaluate2(instructions, op1Inst) + "+" + evaluate2(instructions, op2Inst) + ")");
        //currentInst.value_ = evaluate2(instructions, op1Inst) + evaluate2(instructions, op2Inst);
        //currentInst.valueKnown_ = true;
        break;
    }
    case '-':
    {
        Instruction& op1Inst = instructions[currentInst.op1_];
        Instruction& op2Inst = instructions[currentInst.op2_];
        currentInst.setValue("(" + evaluate2(instructions, op1Inst) + "-" + evaluate2(instructions, op2Inst) + ")");
        //currentInst.value_ = evaluate2(instructions, op1Inst) - evaluate2(instructions, op2Inst);
        //currentInst.valueKnown_ = true;
        break;
    }
    case '*':
    {
        Instruction& op1Inst = instructions[currentInst.op1_];
        Instruction& op2Inst = instructions[currentInst.op2_];
        currentInst.setValue("(" + evaluate2(instructions, op1Inst) + "*" + evaluate2(instructions, op2Inst) + ")");
        //currentInst.value_ = evaluate2(instructions, op1Inst) * evaluate2(instructions, op2Inst);
        //currentInst.valueKnown_ = true;
        break;
    }
    case '/':
    {
        Instruction& op1Inst = instructions[currentInst.op1_];
        Instruction& op2Inst = instructions[currentInst.op2_];
        currentInst.setValue("(" + evaluate2(instructions, op1Inst) + "/" + evaluate2(instructions, op2Inst) + ")");
        //currentInst.value_ = evaluate2(instructions, op1Inst) / evaluate2(instructions, op2Inst);
        //currentInst.valueKnown_ = true;
        break;
    }
    default:
        // should never get here
        int test = 0;
        break;
    }

    return currentInst.valueString_;
}



int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Instructions instructions1;

    readInputFile(inputFileName, instructions1);

    Instructions instructions2 = instructions1;

    if (instructions1.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;
            std::cout << "Value of " << idRoot << ": " << evaluate1(instructions1, instructions1[idRoot]) << std::endl;;
            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            std::cout << "1:" << std::endl;
            std::cout << evaluate2(instructions2, instructions2[instructions2[idRoot].op1_]);
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "2:" << std::endl;
            std::cout << evaluate2(instructions2, instructions2[instructions2[idRoot].op2_]);
            std::cout << std::endl;
        }
    }
}
