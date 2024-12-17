#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>

static const char inputFileName[] = "input.txt";

typedef std::vector<std::string> Data;

void readInputFile(std::string fileName, Data& data)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                // Add new elf's data
                data.emplace_back(line);
            }
        }
        myFile.close();
    }
}

int findFirstDigit(std::string data)
{
    std::array<std::string, 9> digits{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    int value = 0;
    size_t bestIndex = 0;
    size_t bestPosition = data.size();

    // Let's find the first digit if it exists
    auto const first_digit_it = std::find_if(begin(data), end(data), [](char c) {
        return std::isdigit(c);
        });
    if (first_digit_it != end(data))
    {
        bestPosition = first_digit_it - data.begin();
        value = first_digit_it[0] - '0';
    }

    for (size_t i = 0, max = digits.size(); i < max; ++i)
    {
        size_t const pos = data.find(digits[i]);
        if (pos == std::string::npos) { continue; }
        if (pos >= bestPosition) { continue; }

        bestPosition = pos;
        bestIndex = i;
        value = (int)i + 1;
    }

    return value;
}

int findLastDigit(std::string data)
{
    std::array<std::string, 9> digits{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    int value = 0;
    size_t bestIndex = 0;
    size_t bestPosition = 0;

    // Let's find the first digit if it exists
    auto const first_digit_it = std::find_if(rbegin(data), rend(data), [](char c) {
        return std::isdigit(c);
        });
    if (first_digit_it != rend(data))
    {
        bestPosition = data.rend() - first_digit_it - 1;
        value = first_digit_it[0] - '0';
    }

    for (size_t i = 0, max = digits.size(); i < max; ++i)
    {
        size_t const pos = data.rfind(digits[i]);
        if (pos == std::string::npos) { continue; }
        if (pos <= bestPosition) { continue; }

        bestPosition = pos;
        bestIndex = i;
        value = (int)i + 1;
    }

    return value;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Data data;
    readInputFile(inputFileName, data);


    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int sum1 = 0;
        for (auto element : data)
        {
            auto const first_digit_it = std::find_if(begin(element), end(element), [](char c) {
                return std::isdigit(c);
                });
            auto const last_digit_it = std::find_if(rbegin(element), rend(element), [](char c) {
                return std::isdigit(c);
                });

            int firstDigit = first_digit_it[0] - '0';
            int lastDigit = last_digit_it[0] - '0';
            sum1 += 10 * (firstDigit) + (lastDigit);
        }
        std::cout << "Sum is:" << sum1 << std::endl;

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;

        int sum2 = 0;
        for (auto element : data)
        {
            int firstDigit = findFirstDigit(element);
            int lastDigit = findLastDigit(element);
            sum2 += 10 * (firstDigit) + (lastDigit);
        }
        std::cout << "Sum is:" << sum2 << std::endl;

        std::cout << std::endl;
    }
}
