#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";
static const std::string delimiterFieldName = ": ";
static const std::string delimiterRangeStart = "-";
static const std::string delimiterRangeEnd = " or ";
static const std::string delimiterYourTicket = "your ticket:";
static const std::string delimiterNearbyTickets = "nearby tickets:";
static const std::string delimiterList = ",";

std::vector<std::string> fieldsOfInterest =
{
    "departure location",
    "departure station",
    "departure platform",
    "departure track",
    "departure date",
    "departure time"
};


struct Range
{
    int start;
    int end;
};

struct Field
{
    Field( std::string fieldName, std::vector<struct Range> ranges) :
        m_fieldName(fieldName),
        m_ranges(ranges)
    {}

    std::string m_fieldName;
    std::vector<struct Range> m_ranges;
};

struct Ticket
{
    std::vector<int> ticket;
};

struct Ticketing
{
    std::vector<struct Field> fields;
    struct Ticket yourTicket;
    std::vector<struct Ticket> nearbyTickets;
};


struct Field parseLineFields(std::string line)
{
    std::string fieldName;
    std::vector<struct Range> ranges;

    size_t tokenPosition = 0;

    tokenPosition = line.find(delimiterFieldName, 0);
    if (tokenPosition != std::string::npos)
    {
        fieldName = line.substr(0, tokenPosition);
        line.erase(0, tokenPosition + delimiterFieldName.length());
    }

    while (!line.empty())
    {
        tokenPosition = line.find(delimiterRangeStart, 0);
        if (tokenPosition != std::string::npos)
        {
            Range range;

            range.start = std::stoi(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition + delimiterRangeStart.length());

            tokenPosition = line.find(delimiterRangeEnd, 0);
            if (tokenPosition != std::string::npos)
            {
                range.end = std::stoi(line.substr(0, tokenPosition));
                line.erase(0, tokenPosition + delimiterRangeEnd.length());
            }
            else
            {
                // must be the end of line
                tokenPosition = line.size();
                range.end = std::stoi(line.substr(0, tokenPosition));
                line.erase(0, tokenPosition + delimiterRangeEnd.length());
            }

            ranges.push_back(range);
        }
    }

    return Field(fieldName, ranges);
}

struct Ticket parseTicket(std::string line)
{
    struct Ticket ticket;
    size_t tokenPosition = 0;
    
    while (!line.empty())
    {
        int value = 0;

        tokenPosition = line.find(delimiterList, 0);
        if (tokenPosition != std::string::npos)
        {
            value = std::stoi(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition + delimiterList.length());
        }
        else
        {
            // Must be last element
            tokenPosition = line.size();
            value = std::stoi(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition + delimiterList.length());
        }

        ticket.ticket.push_back(value);
    }
    return ticket;
}


struct Ticketing readInputFile(std::string fileName)
{
    size_t tokenPosition = 0;
    std::vector<struct Field> fields;
    struct Ticket yourTicket;
    std::vector<struct Ticket> nearbyTickets;

    struct Ticketing ticketing;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int i = 1;
        while (getline(myFile, line))
        {
            switch (i)
            {
            case 1:
                while (!line.empty())
                {
                    fields.push_back(parseLineFields(line));
                    getline(myFile, line);
                }
                break;
            case 2:
                tokenPosition = 0;
                tokenPosition = line.find(delimiterYourTicket, 0);
                if (tokenPosition != std::string::npos)
                {
                    line.erase(0, tokenPosition + delimiterYourTicket.length());
                }
                break;
            case 3:
                while (!line.empty())
                {
                    yourTicket = parseTicket(line);
                    getline(myFile, line);
                }
                break;
            case 4:
                tokenPosition = 0;
                tokenPosition = line.find(delimiterNearbyTickets, 0);
                if (tokenPosition != std::string::npos)
                {
                    line.erase(0, tokenPosition + delimiterNearbyTickets.length());
                }
                break;
            case 5:
                while (!line.empty())
                {
                    nearbyTickets.push_back(parseTicket(line));
                    getline(myFile, line);
                }
                break;
            default:
                // ignore if anything
                break;
            }
            ++i;
        }
        myFile.close();
    }

    ticketing.fields = fields;
    ticketing.yourTicket = yourTicket;
    ticketing.nearbyTickets = nearbyTickets;

    return ticketing;
}

bool validateTicket(struct Ticket ticket, struct Ticketing & data, int & errorRate)
{
    bool validTicket = true;
    for (auto value : ticket.ticket)
    {
        bool validValue = false;
        for (auto field : data.fields)
        {
            for (auto range : field.m_ranges)
            {
                if ((value >= range.start) && (value <= range.end))
                {
                    // Found a valid range, no need to search further!
                    validValue = true;
                    break;
                }
            }
            if (validValue)
            {
                // Found a valid range, no need to search further!
                break;
            }
        }
        if (!validValue)
        {
            validTicket = false;
            errorRate += value; // There are 2 tickets with 0 (zero) values present, it will not change the errorRate, but they are invalid regardless!!!
        }
    }

    return validTicket;
}

bool indexAlreadyFound(int index, std::map<std::string, int>& fieldMap)
{
    bool alreadyFound = false;

    for (auto entry : fieldMap)
    {
        if (entry.second == index)
        {
            alreadyFound = true;
            break;
        }
    }
    return alreadyFound;
}

std::vector<int> findField(struct Field field, struct Ticketing& data, std::map<std::string, int> & fieldMap)
{
    std::vector<int> fieldIndex;
    int numValues = (int) data.nearbyTickets[0].ticket.size();

    // Walk through all the possible indexes looking for 
    for (int i = 0; i < numValues; i++)
    {
        if (!indexAlreadyFound(i, fieldMap))
        {
            bool indexGood = true;
            // Walk through all the nearby tickets and check their values
            for (auto ticket : data.nearbyTickets)
            {
                int value = ticket.ticket[i];

                // Check the ranges
                bool valueValid = false;
                for (auto range : field.m_ranges)
                {
                    if ((value >= range.start) && (value <= range.end))
                    {
                        // Found a valid range, no need to search further!
                        valueValid = true;
                        break;
                    }
                }
                if (!valueValid)
                {
                    indexGood = false;
                    break;
                }
            }
            if (indexGood)
            {
                // Found an index that works for this field!
                fieldIndex.push_back(i);
            }
        }
    }
    return fieldIndex;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    struct Ticketing ticketing = readInputFile(inputFileName);
    struct Ticketing data1 = ticketing;
    struct Ticketing data2= ticketing;
    std::map<std::string, int> fieldMap;

    if (data1.nearbyTickets.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        int totalErrors = 0;
        
        for (auto ticket : data1.nearbyTickets)
        {
            validateTicket(ticket, data1, totalErrors);
        }

        std::cout << "Part 1:" << std::endl;
        std::cout << "Scanning error rate: " << totalErrors << std::endl;

        std::cout << "Part 2:" << std::endl;

        // Weed out the invalid tickets
        for (auto it = data2.nearbyTickets.begin(); it !=data2.nearbyTickets.end();)
        {
            auto ticket = *it;
            int totalErrors = 0;
            bool validTicket = validateTicket(ticket, data2, totalErrors);

            if (!validTicket)
            {
                it = data2.nearbyTickets.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Make sure our ticket is also valid...
        int totalYourTicketErrors = 0;
        bool validYourTicket = validateTicket(data2.yourTicket, data2, totalYourTicketErrors);
        if (validYourTicket)
        {
            // Identify all fields (if at all possible, otherwise, this is an infinite loop)
            while (fieldMap.size() != data2.fields.size())
            {
                for (auto field : data2.fields)
                {
                    std::vector<int> fieldIndex = findField(field, data2, fieldMap);
                    if (fieldIndex.size() == 1)
                    {
                        fieldMap.emplace(field.m_fieldName, fieldIndex[0]);
                    }
                    else
                    {
                        // More than a single index would satisfy (unfortunately)...
                        // Ignore this and see if others can be identified...
                        // Eliminate already identified indexes and see how many there are left, then look for a single entry only
                    }
                }
            }

            // Dump field map for debugging purposes
            for (auto entry : fieldMap)
            {
                std::cout << entry.first << " maps to index " << entry.second << std::endl;
            }

            unsigned long long product = 1;
            for (auto fieldName : fieldsOfInterest)
            {
                unsigned value = data2.yourTicket.ticket[fieldMap[fieldName]];
                product *= value;
            }
            std::cout << "The product of interest is " << product << std::endl;
        }


        std::cout << std::endl;
    }
}
