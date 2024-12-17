#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";

static const std::string delimiterNodeId = " = (";
static const std::string delimiterSides = ", ";
static const std::string delimiterEnd = ")";

typedef unsigned long long BigNumber;

typedef std::string Instructions;

class Node
{
public:
    Node(std::string key, std::string stepLeft, std::string stepRight)
        : key_(key)
        , stepLeft_(stepLeft)
        , stepRight_(stepRight)
    {

    }
    std::string key_;
    std::string stepLeft_;
    std::string stepRight_;
};

typedef std::map<std::string, Node> Nodes;

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


void readInputFile(std::string fileName, Instructions& instructions, Nodes& nodes)
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
                    // Read Instructions
                    if (!line.empty())
                    {
                        instructions = line;
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    // Read Nodes
                    size_t tokenPosition = 0;

                    std::string nodeId;
                    std::string left;
                    std::string right;

                    tokenPosition = line.find(delimiterNodeId, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        nodeId = line.substr(0, tokenPosition);
                        line.erase(0, tokenPosition + delimiterNodeId.length());

                        tokenPosition = line.find(delimiterSides, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            left = line.substr(0, tokenPosition);
                            line.erase(0, tokenPosition + delimiterSides.length());

                            tokenPosition = line.find(delimiterEnd, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                right = line.substr(0, tokenPosition);
                                line.erase(0, tokenPosition + delimiterEnd.length());

                                nodes.emplace(nodeId, Node(nodeId, left, right));
                            }
                        }

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
    Nodes nodes;

    readInputFile(inputFileName, instructions, nodes);

    if (instructions.empty() || nodes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
{
            BigNumber stepCount = 0;

            auto currentNodeIt = nodes.find("AAA");
            auto endNodeIt = nodes.find("ZZZ");
            size_t instructionsSize = instructions.size();

            while(currentNodeIt != endNodeIt)
            {
                char currentInstruction = instructions[stepCount % instructionsSize];
                if (currentInstruction == 'L')
                {
                    currentNodeIt = nodes.find(currentNodeIt->second.stepLeft_);
                }
                else
                {
                    currentNodeIt = nodes.find(currentNodeIt->second.stepRight_);
                }
                stepCount++;
            };

            std::cout << "steps taken: " << stepCount;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Find all nodes that end with 'A'
            std::vector<Node> startNodes;
            for (auto& node : nodes)
            {
                if (node.first[2] == 'A')
                {
                    startNodes.emplace_back(node.second.key_, node.second.stepLeft_, node.second.stepRight_);
                }
            }

            BigNumber stepCount = 0;
            size_t instructionsSize = instructions.size();
            bool done = false;
            while (!done)
            {
                char currentInstruction = instructions[stepCount % instructionsSize];

                for (auto& node : startNodes)
                {
                    auto nodeIt = nodes.end();
                    if (currentInstruction == 'L')
                    {
                        nodeIt = nodes.find(node.stepLeft_);
                    }
                    else
                    {
                        nodeIt = nodes.find(node.stepRight_);
                    }
                    if (nodeIt != nodes.end())
                    {
                        node.key_ = nodeIt->second.key_;
                        node.stepLeft_ = nodeIt->second.stepLeft_;
                        node.stepRight_ = nodeIt->second.stepRight_;
                    }
                    else
                    {
                        std::cout << "Node not found after " << stepCount << " iterations" << std::endl;
                        std::cout << node.key_ << " : " << node.stepLeft_ << " : " << node.stepRight_ << std::endl;
                        exit(-1);
                    }
                }
                stepCount++;

                // Are we done yet?
                done = true;
                for (auto& node : startNodes)
                {
                    if (node.key_[2] != 'Z')
                    {
                        done = false;
                    }
                }
            }
            std::cout << "steps taken: " << stepCount;
        }

        std::cout << std::endl;

    }
}
