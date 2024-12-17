#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
typedef std::vector<std::vector<char>> SeatData;
enum Status
{
    EMPTY = 'L',
    FLOOR = '.',
    OCCUPIED = '#'
};


// Format of input:
// LLLLLL.LLLLLLLLL.LLLLLLL.LLLLLLLLLLLLLLL.LLLLLL.LLLLLLL.LLLLLLLLLLLLL.LLLLL.LLLLLLLLLLLLLLL

SeatData readInputFile(std::string fileName)
{
    SeatData data;
    std::string line;
    std::ifstream myFile(fileName);

    int numRows = 0;
    int numColumns = 0;

    int row = 0;
    int column = 0;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            ++numRows;
            data.resize(numRows);

            if (numColumns == 0)
            {
                numColumns = (int) line.size();
            }
            data[row].resize(numColumns);

            column = 0;
            for (auto ch : line)
            {
                data[row][column] = ch;
                ++column;
            }

            ++row;
        }
        myFile.close();
    }

    return data;
}

bool occupied(char state)
{
    if (state == OCCUPIED) return true;

    return false;
}

bool empty(char state)
{
    if (state == EMPTY) return true;

    return false;
}

int getOccupiedDirection(const SeatData& dataIn, int row, int column, int dr, int dc, bool visible)
{
    size_t numRows = dataIn.size() - 1;
    size_t numColumns = dataIn[0].size() - 1;
    int r = row;
    int c = column;
    bool done = false;

    while (!done && ((r + dr) >= 0) && ((r + dr) <= numRows) && ((c + dc) >= 0) && ((c + dc) <= numColumns))
    {
        if (occupied(dataIn[r + dr][c + dc]))
        {
            return 1;
        }

        if (empty(dataIn[r + dr][c + dc]))
        {
            return 0;
        }

        if (visible)
        {
            r += dr;
            c += dc;
        }
        else
        {
            done = true;
        }
    }

    return 0;
}

int getOccupiedNeighbors(const SeatData& dataIn, int row, int column, bool visible)
{
    int occupiedNeighbors = 0;
    size_t numRows = dataIn.size() - 1;
    size_t numColumns = dataIn[0].size() - 1;
    int dr = 0; // delta row
    int dc = 0; // delta column


    // ?..
    // .X. 
    // ...
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, -1, -1, visible);

    // .?.
    // .X. 
    // ...
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, -1, 0, visible);

    // ..?
    // .X. 
    // ...
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, -1, 1, visible);

    // ...
    // ?X. 
    // ...
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, 0, -1, visible);

    // ...
    // .X? 
    // ...
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, 0, 1, visible);

    // ...
    // .X.
    // ?..
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, 1, -1, visible);

    // ...
    // .X.
    // .?.
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, 1, 0, visible);

    // ...
    // .X.
    // ..?
    occupiedNeighbors += getOccupiedDirection(dataIn, row, column, 1, 1, visible);

    return occupiedNeighbors;
}

SeatData computeSeats(const SeatData& dataIn, bool visible)
{
    SeatData dataOut = dataIn; //start with current state, also sets the size

    size_t numRows = dataIn.size();
    size_t numColumns = dataIn[0].size();

    int seatLimit = 4;

    if (visible)
    {
        seatLimit = 5;
    }

    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numColumns; c++)
        {
            switch (dataIn[r][c])
            {
            case EMPTY:
                if (getOccupiedNeighbors(dataIn, r, c, visible) == 0)
                {
                    dataOut[r][c] = OCCUPIED;
                }
                else
                {
                    dataOut[r][c] = EMPTY;
                }
                break;

            case OCCUPIED:
                if (getOccupiedNeighbors(dataIn, r, c, visible) >= seatLimit)
                {
                    dataOut[r][c] = EMPTY;
                }
                else
                {
                    dataOut[r][c] = OCCUPIED;
                }
                break;

            default:
            case FLOOR:
                dataOut[r][c] = dataIn[r][c];
                break;
            }
        }
    }

    return dataOut;
}

int countOccupiedSeats(const SeatData& dataIn)
{
    int occupiedSeats = 0;
    size_t numRows = dataIn.size();
    size_t numColumns = dataIn[0].size();

    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numColumns; c++)
        {
            if (dataIn[r][c] == OCCUPIED)
            {
                ++occupiedSeats;
            }
        }
    }

    return occupiedSeats;
}

void fillAllSeats(SeatData& dataIn)
{
    size_t numRows = dataIn.size();
    size_t numColumns = dataIn[0].size();

    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numColumns; c++)
        {
            if (dataIn[r][c] == EMPTY)
            {
                dataIn[r][c] = OCCUPIED;
            }
        }
    }
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
        SeatData dataIn;
        SeatData dataOut;
        int i;
        bool done;

        std::cout << "Part 1:" << std::endl;
        dataIn = data;
        fillAllSeats(dataIn);
        dataOut = dataIn;
        done = false;
        i = 0;
        while (!done)
        {
            // Iterate until no changes
            dataOut = computeSeats(dataIn, false);
            ++i;
            if (dataIn == dataOut)
            {
                done = true;
            }
            else
            {
                dataIn = dataOut;
            }
        }

        std::cout << "Number of seats occupied: " << countOccupiedSeats(dataOut) << " (after " << i <<  " iterations)" << std::endl;

        std::cout << "Part 2:" << std::endl;

        dataIn = data;
        fillAllSeats(dataIn);
        dataOut = dataIn;
        done = false;
        i = 0;
        while (!done)
        {
            // Iterate until no changes
            dataOut = computeSeats(dataIn, true);
            ++i;
            if (dataIn == dataOut)
            {
                done = true;
            }
            else
            {
                dataIn = dataOut;
            }
        }

        std::cout << "Number of seats occupied: " << countOccupiedSeats(dataOut) << " (after " << i << " iterations)" << std::endl;
    }
}
