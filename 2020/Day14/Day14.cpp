#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";
static const std::string delimiterMask = "mask = ";
static const std::string delimiterMemStart = "mem[";
static const std::string delimiterMemEnd = "] = ";

// Format of input:
// mask = 1110X1110XXX101X0011010X110X10X0110X
// mem[40257] = 51331021
// mem[18433] = 464024066
// mem[9993] = 463909
// mask = 11X011010X110X101X011X1X010X10100001
// mem[54152] = 692939
// mem[31079] = 22525259
// mem[33597] = 474240

enum Token
{
    MASK,
    WRITE,
    UNKNOWN
};

struct Mask
{
    unsigned long long andMask; // 0 wherever original mask had 0 or 1; 1 wherever the original mask had X
    unsigned long long orMask;  // 1 wherever the original mask had 1
};


struct Operation
{
    Token token;
    struct Mask mask;
    std::vector<unsigned long long> addressMasks;
    unsigned long long location;
    unsigned long long value;
};

void decodeMask(std::string maskString, struct Operation & operation)
{
    struct Mask mask = { 0, 0 };
    std::vector<unsigned long long> addressMasks;
    unsigned long long addressMask = 0;

    for(auto ch : maskString)
    {
        // make room for new bit
        mask.andMask = mask.andMask << 1;
        mask.orMask = mask.orMask << 1;
        switch (ch)
        {
        case 'X':
            mask.andMask |= 1;

            addressMask = addressMask << 1;
            addressMask |= 1;
            break;
        case '1':
            mask.orMask |= 1;
            break;
        case '0':
            break;
        default:
            break;
        }
    }

    operation.mask = mask;


    for (unsigned long long currentMask = 0; currentMask <= addressMask; currentMask++)
    {
        unsigned long long aMask = mask.andMask;
        unsigned long long cMask = currentMask;
        unsigned long long address = 0; // this is where the next floating address collects

        unsigned long long numBits = sizeof(unsigned long long) * 8;
        static unsigned long long ONE = 1;

        for (int bitLocation = 0; bitLocation < numBits; bitLocation++)
        {
            address = address >> 1;
            // Is it time to add a bit to the address?
            if ((aMask & 1) != 0)
            {
                // Is there a one bit to include?
                if ((cMask & 1) != 0)
                {
                    // Add a bit at the MSB location
                    address = address | (ONE << (numBits - 1));
                }
                cMask = cMask >> 1;
            }
            aMask = aMask >> 1;
        }
        operation.addressMasks.emplace_back(address);
    }
}

struct Operation parseLine(std::string line)
{
    // Initialize 
    struct Operation operation;

    size_t tokenPosition = 0;

    tokenPosition = line.find(delimiterMask, 0);
    if (tokenPosition != std::string::npos)
    {
        operation.token = MASK;

        line.erase(0, tokenPosition + delimiterMask.length());

        decodeMask(line, operation);
    }
    else
    {
        tokenPosition = line.find(delimiterMemStart, 0);
        if (tokenPosition != std::string::npos)
        {
            operation.token = WRITE;

            line.erase(0, tokenPosition + delimiterMemStart.length());

            tokenPosition = line.find(delimiterMemEnd, 0);
            if (tokenPosition != std::string::npos)
            {
                operation.location = std::stoull(line.substr(0, tokenPosition));
                line.erase(0, tokenPosition + delimiterMemEnd.length());

                operation.value = std::stoull(line);
            }
        }
        else
        {
            // Should never get here
        }
    }

    return operation;
}

std::vector<Operation> readInputFile(std::string fileName)
{
    std::vector<Operation> data;
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

void execute1(std::vector<Operation> & data, std::map<unsigned long long, unsigned long long> & memoryMap)
{
    struct Mask currentMask = { 0, 0 };

    for (auto operation : data)
    {
        switch (operation.token)
        {
        case MASK:currentMask = operation.mask;
            break;
        case WRITE:
            memoryMap[operation.location] = (operation.value & currentMask.andMask) | currentMask.orMask;
            break;
        default:
            break;
        }
    }
}

void execute2(std::vector<Operation>& data, std::map<unsigned long long, unsigned long long>& memoryMap)
{
    struct Mask currentMask = { 0, 0 };
    std::vector<unsigned long long> addressMasks;

    for (auto operation : data)
    {
        switch (operation.token)
        {
        case MASK:
            currentMask = operation.mask;
            addressMasks = operation.addressMasks;
            break;
        case WRITE:
            if (!addressMasks.empty())
            {
                for (auto addressMask : addressMasks)
                {
/*                    unsigned long long location;
                    location = operation.location;
                    location = location & ~currentMask.andMask;
                    location = location | currentMask.orMask;
                    location = location | addressMask; 
                    memoryMap[location] = operation.value; */
                    memoryMap[(operation.location & ~currentMask.andMask) | currentMask.orMask | addressMask] = operation.value;
                }
            }
            else
            {
                memoryMap[operation.location] = operation.value;
            }
            break;
        default:
            break;
        }
    }
}

unsigned long long checksum(std::map<unsigned long long, unsigned long long>& memoryMap)
{
    unsigned long long sum = 0;

    for (auto memory : memoryMap)
    {
        sum += memory.second;
    }

    return sum;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::map<unsigned long long, unsigned long long> memoryMap1;
    std::map<unsigned long long, unsigned long long> memoryMap2;
    std::vector<Operation> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        execute1(data, memoryMap1);
        unsigned long long sum1 = checksum(memoryMap1);
        std::cout << "Today's magic number is: " << sum1 << std::endl;

        std::cout << "Part 2:" << std::endl;
        execute2(data, memoryMap2);
        unsigned long long sum2 = checksum(memoryMap2);
        std::cout << "Today's magic number is: " << sum2 << std::endl;

        std::cout << std::endl;
    }
}
