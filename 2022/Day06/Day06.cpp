#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

class Message
{
public:
    Message()
        : startOfPacket_(-1)
        , startOfMessage_(-1)
    {
    }

    void setMessage(std::string msg)
    {
        msg_ = msg;
     }

    int findStartOfPacketPosition()
    {
        startOfPacket_ = -1;

        char queue[4] = {' ', ' ', ' ', ' '};

        size_t len = msg_.length();
        for(size_t i = 0; i < len; i++)
        {
            queue[3] = queue[2];
            queue[2] = queue[1];
            queue[1] = queue[0];
            queue[0] = msg_.at(i);
            if (i >= 4
                && (queue[0] != queue[1] && queue[0] != queue[2] && queue[0] != queue[3] )
                && (queue[1] != queue[2] && queue[1] != queue[3])
                && (queue[2] != queue[3]))
            {
                startOfPacket_ = i + 1;
                break;
            }
        }
        return startOfPacket_;
    }

    int findStartOfMessagePosition()
    {
        startOfMessage_ = -1;

        constexpr size_t markerLength = 14;

        size_t len = msg_.length();
        // Loop through the whole message, i marking the beginning of each block
        for(size_t i = 0; i < (len - markerLength); i++)
        {
            bool isStillPossible = true;

            // Compare characters #1 is at location c
            for(size_t c = i; c < i + markerLength - 1; c++)
            {
                // Compare character #2 is at location p
                for(size_t p = c + 1; p < i + markerLength; p++)
                {
                    if (startOfMessage_ == -1)
                    {
                        if (msg_.at(c) != msg_.at(p))
                        {
                            // Keep going
                        }
                        else
                        {
                            // No need to continue
                            isStillPossible = false;
                            break;
                        }
                    }
                }
                if (!isStillPossible)
                {
                    break;
                }
                else
                {
                    // Are we at the end without failing?
                    if (c == i + markerLength - 1 - 1)
                    {
                        // Yes, found it!
                        startOfMessage_ = c + 1 + 1;
                    }
                }
                if (startOfMessage_ != -1)
                {
                    break;
                }
            }
            if (startOfMessage_ != -1)
            {
                break;
            }
        }
        return startOfMessage_;
    }

    std::string msg_;
    int startOfPacket_;
    int startOfMessage_;
};

void readInputFile(std::string fileName, Message& message)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                message.setMessage(line);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Message message;

    readInputFile(inputFileName, message);

    if (message.msg_.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        std::cout << "Packet start at " << message.findStartOfPacketPosition();
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        std::cout << "Message start at " << message.findStartOfMessagePosition();

        std::cout << std::endl;

    }
}
