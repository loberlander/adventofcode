#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

static const std::string delimiterStart = "Card ";
static const std::string delimiterCardNumber = ": ";
static const std::string delimiterNumbers = " | ";
static const std::string delimiterNumber = " ";

typedef std::set<int> CardNumbers;
typedef std::set<int> WinningNumbers;

class Card
{
public:
    Card(int cardId, WinningNumbers winningNumbers, CardNumbers cardNumbers)
        : cardId_(cardId)
        , winningNumbers_(winningNumbers)
        , cardNumbers_(cardNumbers)
        , instanceCount_(1)
    {

    }

    int cardId_;
    WinningNumbers winningNumbers_;
    CardNumbers cardNumbers_;
    int instanceCount_;
};


typedef std::vector<Card> Cards;

// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
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


std::set<int> processNumbers(std::string& numbersAsString)
{
    std::set<int> numbers;
    std::vector<std::string> splitNumbers = split(numbersAsString, delimiterNumber);

    for (auto& element : splitNumbers)
    {
        int number = std::stoi(element);
        numbers.emplace(number);
    }

    return numbers;
}

void readInputFile(std::string fileName, Cards& cards)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Card #
                    size_t tokenPosition = 0;
                    int cardId = 0;

                    tokenPosition = line.find(delimiterStart, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterStart.length());
                        tokenPosition = line.find(delimiterCardNumber, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            std::string idAsString = line.substr(0, tokenPosition);
                            line.erase(0, tokenPosition + delimiterCardNumber.length());
                            cardId = std::stoi(idAsString);
                        }
                    }

                    tokenPosition = line.find(delimiterNumbers, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        std::string winningNumbersAsString = line.substr(0, tokenPosition);
                        line.erase(0, tokenPosition + delimiterNumbers.length());

                        WinningNumbers winningNumbers = processNumbers(winningNumbersAsString);
                        CardNumbers cardNumbers = processNumbers(line);
                        cards.emplace_back(cardId, winningNumbers, cardNumbers);
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Cards cards;

    readInputFile(inputFileName, cards);

    if (cards.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;
            for (auto& card : cards)
            {
                int winningCount = 0;
                for (auto& cardNumber : card.cardNumbers_)
                {
                    if (card.winningNumbers_.find(cardNumber) != card.winningNumbers_.end())
                    {
                        winningCount++;
                    }
                }
                if (winningCount > 0)
                {
                    sum += 1 << (winningCount - 1);
                }
            }
            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;
            for (auto& card : cards)
            {
                int winningCount = 0;
                for (auto& cardNumber : card.cardNumbers_)
                {
                    if (card.winningNumbers_.find(cardNumber) != card.winningNumbers_.end())
                    {
                        winningCount++;
                    }
                }
                for (int h = 0; h < card.instanceCount_; h++)
                {
                    for (int i = 0; i < winningCount; i++)
                    {
                        // This is somewhat hacky, take advantage of the id being one more than the index...
                        // The proper way should be to actually search the card ID...
                        cards[card.cardId_ + i].instanceCount_++;
                    }
                }
            }

            for (auto& card : cards)
            {
                sum += card.instanceCount_;
            }

            std::cout << "Total card numbers: " << sum;
        }

        std::cout << std::endl;

    }
}
