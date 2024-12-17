#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";
//static const char inputFileName[] = "inputTest2.txt";


typedef std::vector<unsigned long long> Nums;

// efficient integer exponentiation from asymmetric cryptography
unsigned long long ipow(unsigned long long base, unsigned long long exp)
{
    unsigned long long result = (unsigned long long) 1;
    for (;;)
    {
        if (exp & 1)
        {
            result *= base;
        }
        exp >>= 1;
        if (!exp)
        {
            break;
        }
        base *= base;
    }

    return result;
}

unsigned long long fromSnafu(std::string line)
{
    unsigned long long x = 0;

    int numDigits = (int) line.size();
    for (int i = 0; i < numDigits; i++)
    {
        switch (line[i])
        {
            case '0':
            case '1':
            case '2':
            {
                int count = std::stoi(line.substr(i, 1));
                x +=  count * ipow(5, numDigits - 1 - i);
                break;
            }
            case '-':
            {
                x -= 1 * ipow(5, numDigits - 1 - i);
                break;
            }
            case '=':
            {
                x -= 2 * ipow(5, numDigits - 1 - i);
                break;
            }
            default:
            {
                // should never get here
                break;
            }
        }

    }

    return x;
}

std::string toSnafu(unsigned long long x)
{
    std::string result;
    unsigned long long rem;

    while (x > 0)
    {
        rem = x % (unsigned long long) 5;
        char nextDigit = std::to_string(rem).c_str()[0];
        if (rem > (unsigned long long) 2)
        {
            rem = (unsigned long long )5 - rem;
            x = x + (unsigned long long)5;
            nextDigit = (rem == (unsigned long long) 1) ? '-' : '=';
        }
        result = nextDigit + result;
        x /= (unsigned long long)5;
    }

    

    return result;
}

void readInputFile(std::string fileName, Nums& nums)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    int y = 0;
    int elfId = 0;

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
                        unsigned long long x = 0;

                        x = fromSnafu(line);
                        std::cout << line << " is " << x << std::endl;

                        nums.emplace_back(x);
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



int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Nums nums;

    readInputFile(inputFileName, nums);


    if (nums.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;
            unsigned long long total = 0;
            for (auto& num : nums)
            {
                total += num;
            }
            std::cout << "The total (decimal) is " << total << std::endl;
            std::cout << "The total (SNAFU) is " << toSnafu(total) << std::endl;
            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
}
