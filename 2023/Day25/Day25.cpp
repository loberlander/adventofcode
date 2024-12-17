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

//#define TEST

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

    Node(std::string id, int index, std::set<std::string>& connections)
        : id_(id)
        , index_(index)
        , connections_(connections)
    {}

    bool operator==(const Node& other) const
    {
        return (id_ == other.id_);
    }

    std::string id_;
    int index_;
    std::set<std::string> connections_;
};

typedef std::vector<std::vector<int>> ConnectionMatrix;

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

                        auto nodeNewIt = nodes.find(parts[0]);
                        if (nodeNewIt == nodes.end())
                        {
                            size_t index = nodes.size();
                            std::set<std::string> newLinks({ parts[0] });
                            nodes.emplace(parts[0], Node(parts[0], index, newLinks));
                        }
                        for (auto& link : linksAsString)
                        {
                            nodes[parts[0]].connections_.emplace(link);
                        }

                        for (auto& link : linksAsString)
                        {
                            auto nodeIt = nodes.find(link);
                            if (nodeIt == nodes.end())
                            {
                                // not found , create it
                                size_t index = nodes.size();
                                std::set<std::string> newLinks({ parts[0] });
                                nodes.emplace(link, Node(link, index, newLinks));
                            }
                            else
                            {
                                // found
                                nodeIt->second.connections_.emplace(parts[0]);
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

int GetIndex(std::map<std::string, int>&nameMap, const std::string& s)
{
    if (nameMap.find(s) != nameMap.cend())
    {
        return nameMap[s];
    }
    
    int index = nameMap.size();
    nameMap[s] = index;
    
    return index;
};

// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
// Adjacency matrix implementation of Stoer–Wagner min cut algorithm.
//
// Running time:
//     O(|V|^3)
std::pair<int, std::vector<int>> globalMinCut(std::vector<std::vector<int>>& mat)
{
    std::pair<int, std::vector<int>> best = { std::numeric_limits<int>::max(), {} };
    int n = mat.size();
    std::vector<std::vector<int>> co(n);

    for (int i = 0; i < n; i++)
    {
        co[i] = { i };
    }

    for (int ph = 1; ph < n; ph++)
    {
        std::vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++)
        { // O(V^2) -> O(E log V) with prio. queue
            w[t] = std::numeric_limits<int>::min();
            s = t, t = max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++)
            {
                w[i] += mat[t][i];
            }
        }
        best = min(best, { w[t] - mat[t][t], co[t] });
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
        mat[0][t] = std::numeric_limits<int>::min();
    }

    return best;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Nodes nodes;

    readInputFile(inputFileName, nodes);

    // Create the adjacency matrix
    // "1" is placed in the intersection of nodes where there is a path (conncection), "0" otherwise
    std::vector<std::vector<int>> adj(nodes.size(), std::vector<int>(nodes.size(), 0));

    for (const auto& node : nodes)
    {
        for (const auto& connection : node.second.connections_)
        {
            //  from: index         to: index
            adj[node.second.index_][nodes[connection].index_] = 1;
        }
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

            auto result = globalMinCut(adj);

            std::cout << "The result is: ";
            std::cout << result.second.size() * (nodes.size() - result.second.size());

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
