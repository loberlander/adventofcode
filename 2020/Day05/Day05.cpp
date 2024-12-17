#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const int numRows = 128;
static const int numColumns = 8;

typedef std::vector<std::vector<bool>> SeatData;


// Format of input: BFBFFFBLRL

bool parseRecord(std::string line, int & row, int & column)
{
    bool success = true;

    row = numRows - 1;
    column = numColumns - 1;

    int currentRowScale = numRows;
    while (currentRowScale > 1)
    {
        switch(line[0])
        {
        case 'F':
            row -= currentRowScale / 2;
            break;
        case 'B':
            // do nothing
            break;
        default:
            break;
        }
        currentRowScale = currentRowScale / 2;
        line.erase(0, 1);
    }


    int currentColumnScale = numColumns;
    while (currentColumnScale > 1)
    {
        switch (line[0])
        {
        case 'L':
            column -= currentColumnScale / 2;
            break;
        case 'R':
            // do nothing
            break;
        default:
            break;
        }
        currentColumnScale = currentColumnScale / 2;
        line.erase(0, 1);
    }

    return success;
}

SeatData readInputFile(std::string fileName)
{
    SeatData data;
    std::string line;
    std::ifstream myFile(fileName);

    data.resize(numRows);
    for (int r = 0; r < numRows; r++)
    {
        data[r].resize(numColumns);
        for (int c = 0; c < numColumns; c++)
        {
            data[r][c] = false;
        }
    }

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            int row = 0;
            int column = 0;
            if (parseRecord(line, row, column))
            {
                data[row][column] = true;
            }
        }
        myFile.close();
    }

    return data;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    SeatData data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        for (int r = 0; r < numRows; r++)
        {
            for (int c = 0; c < numColumns; c++)
            {
                std::cout << (data[r][c] ? "X" : ".");
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << "Part 1:" << std::endl;
        int highestSeatId = 0;

        for (int r = 0; r < numRows; r++)
        {
            for (int c = 0; c < numColumns; c++)
            {
                if (data[r][c])
                {
                    int currentSeatId = 8 * r + c;
                    if (currentSeatId > highestSeatId)
                    {
                        highestSeatId = currentSeatId;
                    }
                }
            }
        }

        std::cout << "Highest Seat ID: " << highestSeatId << std::endl;

        std::cout << "Part 2:" << std::endl;
        // Find the "hole"
        int mySeatId = 0;
        bool foundIt = false;

        int seatId = 0;
        int seatIdM1 = 0;
        int seatIdM2 = 0;

        bool seatStatus = false;
        bool seatStatusM1 = false;
        bool seatStatusM2 = false;

        for (int r = 0; r < numRows; r++)
        {
            for (int c = 0; c < numColumns; c++)
            {
                seatIdM2 = seatIdM1;
                seatIdM1 = seatId;
                seatId = 8 * r + c;

                seatStatusM2 = seatStatusM1;
                seatStatusM1 = seatStatus;
                seatStatus = data[r][c];

                if (!foundIt)
                {
                    if (!seatStatusM1 && seatStatusM2 && seatStatus)
                    {
                        // Found it!
                        mySeatId = seatIdM1;
                        foundIt = true;
                    }
                }

            }
        }

        std::cout << "My Seat ID: " << (foundIt ? std::to_string(mySeatId) : "?" ) << std::endl;

    }
}
