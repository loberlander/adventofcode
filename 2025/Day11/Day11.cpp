#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;

typedef std::vector<std::string> Connections;
typedef std::vector<std::string> Path;

class Node
{
    public:
        Node()
        { }

        Node(std::string name, Connections connections)
            : name_(name)
            , connections_(connections)
        { }

        std::string name()
        {
            return name_;
        }

        Connections connections()
        {
            return connections_;
        }

    std::string asString() const
    {
        return name_;
    }

    private:
        std::string name_;
        Connections connections_;
};


typedef std::map<std::string, Node> Nodes;

typedef std::map<std::string, BigNumber> Cache;


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


std::string trimLeft(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the first non-whitespace character
    size_t pos = str.find_first_not_of(pattern);

    // Extract the substring from the first non-whitespace to the end
    return (pos == std::string::npos) ? "" : str.substr(pos);
}


std::string trimRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the last non-whitespace character
    size_t pos = str.find_last_not_of(pattern);

    // Extract the substring from the beginning to the last non-whitespace 
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}


std::string trimLeftRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    return trimLeft(trimRight(str, pattern), pattern);
}


void readInputFile(std::string fileName, Nodes& nodes)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read ranges
                    if (!line.empty())
                    {
                        auto elements1 = split(line, { ":" });

                        auto elements2 = split(elements1[1], {" "});

                        Node node(elements1[0], elements2);

                        nodes.emplace(node.name(), node);
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    break;
                }
            }
        }
        myFile.close();
    }
}


BigNumber countPaths(Nodes& nodes, Cache& cache, std::string start, std::string end)
{
    BigNumber result = 0;
    std::string key = start + ":" + end;

    // Check if already calculated this start->end combination and therefore know the answer...
    auto it = cache.find(key);
    if (it != cache.end())
    {
        return it->second;
    }

    // Arrived at the node that is trivial
    if (start == end)
    {
        result = 1;
    }
    else
    {
        // count all the subpaths that the current node connects to
        for (auto& connection : nodes[start].connections())
        {
            result += countPaths(nodes, cache, connection, end);
        }
    }

    // Save result in the cache, so we do not need to calculate this again
    cache.emplace(key, result);

    return result;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Nodes nodes;

    readInputFile(inputFileName, nodes);

    if (nodes.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;
            Cache cache;

            result = countPaths(nodes, cache, "you", "out");

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result1 = 0;
            BigNumber result2 = 0;
            Cache cache;

            // Idea:
            // - Assuming that there are no cycles in the graph, only one of the below will be non-zero
            // - The number of paths is the product of the subpaths:
            //   START -> FFT -> DAC -> END  => count(START -> FFT) * count(FFT -> DAC) * count(DAC -> END)
            //   START -> DAC -> FFT -> END  => count(START -> DAC) * count(DAC -> FFT) * count(FFT -> END)
            //
            //   if both FFT -> DAC and DAC -> FFT existed, it would be a cycle

            result1 = countPaths(nodes, cache, "svr", "fft") * countPaths(nodes, cache, "fft", "dac") * countPaths(nodes, cache, "dac", "out");
            result2 = countPaths(nodes, cache, "svr", "dac") * countPaths(nodes, cache, "dac", "fft") * countPaths(nodes, cache, "fft", "out");

            std::cout << "Path 1: " << result1 << std::endl;;
            std::cout << "Path 2: " << result2 << std::endl;;
            std::cout << "Total: " << result1 + result2;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
