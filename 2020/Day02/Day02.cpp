#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const std::string delimiter1 = "-";
static const std::string delimiter2 = " ";
static const std::string delimiter3 = ": ";

// Format of input:  "14-16 z: zzzzzzzzzrzzblzw"
struct PasswordPolicy
{
    int minCount;
    int maxCount;
    std::string letter;
    std::string password;
    bool isPasswordValidRule1;
    bool isPasswordValidRule2;
};

struct PasswordPolicy parseLine(std::string line)
{
    // Initialize 
    PasswordPolicy passwordPolicy = {0, 0, "", "", false, false};

    size_t tokenPosition = 0;

    // Parse 1st parameter
    tokenPosition = line.find(delimiter1, 0);
    if (tokenPosition != std::string::npos)
    {
        passwordPolicy.minCount = std::stoi(line.substr(0, tokenPosition));
        line.erase(0, tokenPosition + delimiter1.length());
    }

    // Parse 2nd parameter
    tokenPosition = line.find(delimiter2, 0);
    if (tokenPosition != std::string::npos)
    {
        passwordPolicy.maxCount = std::stoi(line.substr(0, tokenPosition));
        line.erase(0, tokenPosition + delimiter2.length());
    }

    // Parse 3rd parameter
    tokenPosition = line.find(delimiter3, 0);
    if (tokenPosition != std::string::npos)
    {
        passwordPolicy.letter = line.substr(0, tokenPosition);
        line.erase(0, tokenPosition + delimiter3.length());
    }

    // Parse 4th (last) parameter
    passwordPolicy.password = line;

    // Validate against Rule #1
    auto letterCount = std::count(passwordPolicy.password.begin(), passwordPolicy.password.end(), passwordPolicy.letter[0]);
    if (letterCount >= passwordPolicy.minCount && letterCount <= passwordPolicy.maxCount)
    {
        passwordPolicy.isPasswordValidRule1 = true;
    }

    // Validate against Rule #2
    if (
        ((passwordPolicy.letter[0] == passwordPolicy.password[passwordPolicy.minCount - 1]) &&
        (passwordPolicy.letter[0] != passwordPolicy.password[passwordPolicy.maxCount - 1]))
        ||
        ((passwordPolicy.letter[0] != passwordPolicy.password[passwordPolicy.minCount - 1]) &&
        (passwordPolicy.letter[0] == passwordPolicy.password[passwordPolicy.maxCount - 1]))
       )
    {
        passwordPolicy.isPasswordValidRule2 = true;
    }

    return passwordPolicy;
}

std::vector<PasswordPolicy> readInputFile(std::string fileName)
{
    std::vector<PasswordPolicy> data;
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


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<PasswordPolicy> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int validCount1 = 0;
        for (auto password : data)
        {
            if (password.isPasswordValidRule1)
            {
                ++validCount1;
            }
        }
        std::cout << "Valid Passwords: " << validCount1 << std::endl;

        std::cout << "Part 2:" << std::endl;
        int validCount2 = 0;
        for (auto password : data)
        {
            if (password.isPasswordValidRule2)
            {
                ++validCount2;
            }
        }
        std::cout << "Valid Passwords: " << validCount2 << std::endl;
    }
}
