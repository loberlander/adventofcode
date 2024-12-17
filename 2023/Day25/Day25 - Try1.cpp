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
#include <random>

#include "helper.h"

#define TEST

#ifndef TEST
static const char inputFileName[] = "input.txt";
#else
static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";
#endif

typedef long long BigNumber;

class Node
{
public:
    Node()
    {}

    Node(std::string id, std::set<std::string>& links)
        : id_(id)
        , links_(links)
        , frequency_(0)
    {}

    Node(std::string id)
        : id_(id)
        , frequency_(0)
    {}

    bool operator==(const Node& other) const
    {
        return (id_ == other.id_);
    }

    std::string id_;
    std::set<std::string> links_;
    BigNumber frequency_;
    std::string fromId_;
};

typedef std::map<std::string, Node> Nodes;

typedef std::set<std::string> Visited;

static const std::string delimiterNodeId = ": ";
static const std::string delimiterNodes = " ";

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

BigNumber lcm(std::vector<BigNumber> values)
{
    // Requires C++17 that provides std::gcd()
    // calculate the LCM of values in a vector
    // lcm(a, b) = (a*b)/gcd(a,b)
    BigNumber lcmTmp = values.back();
    values.pop_back();

    for (auto value : values)
    {
        lcmTmp = lcmTmp * value / std::gcd(lcmTmp, value);
    }

    return lcmTmp;
}


void readInputFile(std::string fileName, Nodes& nodes)
{
    std::string line;
    std::ifstream myFile(fileName);

    BigNumber id = 0;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Workflows
                    if (!line.empty())
                    {
                        std::vector<std::string> parts = split(line, delimiterNodeId);
                        std::vector<std::string> linksAsString = split(parts[1], delimiterNodes);

                        std::set<std::string> links;
                        for (auto& link : linksAsString)
                        {
                            links.emplace(link);
                        }
                        nodes.emplace(parts[0], Node(parts[0], links));


                        for (auto& link : linksAsString)
                        {
                            auto nodeIt = nodes.find(link);

                            if (nodeIt == nodes.end())
                            {
                                // not found , create it
                                std::set<std::string> newLinks({ parts[0] });
                                nodes.emplace(link, Node(link, newLinks));
                            }
                            else
                            {
                                // found
                                nodeIt->second.links_.emplace(parts[0]);
                            }
                        }
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }

            }
        }
        myFile.close();
    }
}

void findShortestPath(Nodes& nodes, Node& start, Node& end)
{
    std::deque<Node> q;
    Visited visited;

    q.emplace_back(start);

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        if (end == current)
        {
            // we are done
            return;
        }

        if (visited.find(current.id_) != visited.end())
        {
            // Been here before, okay to skip
            continue;
        }

        visited.emplace(current.id_);

        auto currentNodeIt = nodes.find(current.id_);
        if (currentNodeIt != nodes.end())
        {
            // found node
            for (auto& newNodeId : currentNodeIt->second.links_)
            {
                if (visited.find(newNodeId) != visited.end())
                {
                    // It has been already visited, there is no need to add it again
                    continue;
                }

                auto newNodeIt = nodes.find(newNodeId);
                if (newNodeIt != nodes.end())
                {
                    newNodeIt->second.fromId_ = current.id_;
                }

                q.emplace_back(newNodeIt->second);
            }
        }
    }
    return;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Nodes nodes;

    readInputFile(inputFileName, nodes);

    std::vector<Node> nodeIds;

    for (auto node : nodes)
    {
        nodeIds.emplace_back(node.second);
    }

    if (nodes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;

            size_t numEntries = nodes.size();
            std::random_device rd;
            std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
            std::uniform_int_distribution<> distrib(1, numEntries - 1);

            for (int i = 0; i < 10000; i++)
            {
                std::string start = nodeIds[distrib(gen)].id_;
                std::string end = nodeIds[distrib(gen)].id_;

                Nodes nodesWork = nodes;
                findShortestPath(nodesWork, nodesWork[start], nodesWork[end]);

                // back track from end to start to reveal the path taken
                std::string current = end;
                while (current != start)
                {
                    nodes[current].frequency_++;
                    current = nodesWork[current].fromId_;
                }
                nodes[current].frequency_++;
            }

            for (auto& node : nodeIds)
            {
                node.frequency_ = nodes[node.id_].frequency_;
            }

            struct
            {
                bool operator()(Node a, Node b) const { return a.frequency_ < b.frequency_; }
            } customLess;

            // sorting is needed for the order to be right
            // Some shapes might already be too low by the time they are processed
            std::sort(nodeIds.begin(), nodeIds.end(), customLess);



            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
    }
}
