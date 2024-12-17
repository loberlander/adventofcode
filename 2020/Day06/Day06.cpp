#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <set>

static const char inputFileName[] = "input.txt";
static const std::string fieldPersonDelimiter = " ";

struct Group
{
    std::map<char, int> answers; // answers(key): 'a', 'b' .. 'z', value: number of people in the group
    int numPersons;
    int numGroupAnyoneYesAnswers; // Anyone that answered yes
    int numGroupEveryoneYesAnswers; // Everyone that answered yes
};
// Format of input:
//
// lfnghcsvpyrdjtxozimb
// mdtbnorpfalcijxvhsy
// elmwjkfbihydxcpqtovsrun
// tlhmsdjingyxcbfrvpo
//
// a
// a
// xqh
//
// mxdeqcinvfg
// vbncrgzxqefka

struct Group parseRecord(std::string line)
{
    struct Group group;

    group.numPersons = 0;
    group.numGroupAnyoneYesAnswers = 0;
    group.numGroupEveryoneYesAnswers = 0;

    while (!line.empty())
    {
        size_t fieldPersonPosition = 0;

        // Parse the next field (person)
        fieldPersonPosition = line.find(fieldPersonDelimiter, 0);
        if (fieldPersonPosition != std::string::npos)
        {
            ++group.numPersons; // one more person in the group

            std::string answers = line.substr(0, fieldPersonPosition);
            line.erase(0, fieldPersonPosition + fieldPersonDelimiter.length());

            for (auto answer : answers)
            {
                if (group.answers.find(answer) != group.answers.end())
                {
                    // answer already exists, increment the answer count
                    group.answers[answer] += 1;
                }
                else
                {
                    // answer is new, must add it
                    group.answers.emplace(answer, 1);
                    ++group.numGroupAnyoneYesAnswers;
                }
            }
        }
    }

    // Sanity check, they should be the same...
    if (group.answers.size() != group.numGroupAnyoneYesAnswers)
    {
        group.numGroupAnyoneYesAnswers = (int) group.answers.size();
    }

    for (auto answer : group.answers)
    {
        if (answer.second == group.numPersons)
        {
            ++group.numGroupEveryoneYesAnswers;
        }
    }

    return group;
}

std::vector<struct Group> readInputFile(std::string fileName)
{
    std::vector<struct Group> data;
    std::string line;
    std::string record;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (line.empty())
            {
                // A record should be fully parsed...
                if (!record.empty())
                {
                    record = record + " "; // Add a value delimiter at the end!
                    data.push_back(parseRecord(record));
                    record.clear();
                }
            }
            else
            {
                // Beginning or continuation of a record is getting parsed
                if (!record.empty())
                {
                    record = record + " "; // Add a value delimiter between broken lines!
                }
                record = record + line;
            }
        }

        // Add the last record (end of file will not result in a new line!
        if (!record.empty())
        {
            record = record + " "; // Add a value delimiter at the end!
            data.push_back(parseRecord(record));
            record.clear();
        }

        myFile.close();
    }

    return data;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<struct Group> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;

        int totalAnyoneYesAnswers = 0;
        for (auto group : data)
        {
            totalAnyoneYesAnswers += group.numGroupAnyoneYesAnswers;
        }

        std::cout << "Total answers: " << totalAnyoneYesAnswers << std::endl;

        std::cout << "Part 2:" << std::endl;

        int totalEveryoneYesAnswers = 0;
        for (auto group : data)
        {
            totalEveryoneYesAnswers += group.numGroupEveryoneYesAnswers;
        }

        std::cout << "Total answers: " << totalEveryoneYesAnswers << std::endl;

    }
}
