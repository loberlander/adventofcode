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

// comment out the below for Part 1
// uncomment the below for Part 2
//#define PART2

typedef unsigned long long BigNumber;

typedef std::map<char, int> CardRanks;

#ifdef PART2
CardRanks cardRanks
{
    {'A', 14},
    {'K', 13},
    {'Q', 12},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2},
    {'J', 1},
};
#else
CardRanks cardRanks
{
    {'A', 14},
    {'K', 13},
    {'Q', 12},
    {'J', 11},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2}
};
#endif


enum ComboRank
{
    FiveOfAKind,
    FourOfAKind,
    FullHouse,
    ThreeOfAKind,
    TwoPair,
    OnePair,
    HighCard
};

class Card
{
public:
    Card(char c)
        : c_(c)
    {
        rank_ = cardRanks[c_];
    }

    char c_;
    int rank_;
};

typedef std::vector<Card> Cards;

class Hand
{
public:
    Hand(std::string cardsAsString, int bid)
        : cardsAsString_(cardsAsString)
        , bid_(bid)
    {
        int conutCards = cardsAsString_.size();
        for (int i = 0; i < conutCards; i++)
        {
            cards_.push_back(cardsAsString_[i]);
            if (cardsAsString_[i] == 'J')
            {
                jLocations_.emplace_back(i);
            }
        }

        sameness_ = countSame();
        switch (sameness_.size())
        {
            case 1:
            {
                comboRank_ = FiveOfAKind;
                break;
            }
            case 2:
            {
                auto it0 = sameness_.begin();
                auto it1 = sameness_.begin(); it1++;
                if (it0->second == 4 || it1->second == 4)
                {
                    comboRank_ = FourOfAKind;
                }
                else
                {
                    comboRank_ = FullHouse; // Must be
                }
                break;
            }
            case 3:
            {
                auto it0 = sameness_.begin();
                auto it1 = sameness_.begin(); it1++;
                auto it2 = sameness_.begin(); it2++; it2++;
                if (it0->second == 3 || it1->second == 3 || it2->second == 3)
                {
                    comboRank_ = ThreeOfAKind;
                }
                else
                {
                    comboRank_ = TwoPair;
                }
                break;
            }
            case 4:
            {
                comboRank_ = OnePair;
                break;
            }
            default:
            {
                comboRank_ = HighCard;
                break;
            }
        }
    }

    std::map<char, int> countSame()
    {
        std::map<char, int> sameCards;

        for (auto& card : cardsAsString_)
        {
            int count = 1;
            auto it = sameCards.find(card);
            if (it != sameCards.end())
            {
                count = sameCards[card];
                count++;
                sameCards.erase(it);
            }
            sameCards.emplace(card, count);
        }

        return sameCards;
    }


    std::string cardsAsString_;
    Cards cards_;
    int bid_;
    ComboRank comboRank_;
    std::map<char, int> sameness_;
    std::vector<int> jLocations_;
};


struct HandComparator
{
    //bool operator()(const Hand& lhs, const Hand& rhs) const;
    bool operator()(const Hand& lhs, const Hand& rhs) const
    {
        // lhs is bigger return true
        // rhs is bigger return false
        if (lhs.comboRank_ > rhs.comboRank_)
        {
            return true;
        }
        if (lhs.comboRank_ < rhs.comboRank_)
        {
            return false;
        }

        // Must be the same combo rank
        // Compare 
        for (int i = 0; i < 5; i++)
        {
            if (lhs.cards_[i].rank_ < rhs.cards_[i].rank_)
            {
                return true;
            }
            if (lhs.cards_[i].rank_ > rhs.cards_[i].rank_)
            {
                return false;
            }
        }


        return true;
    }
};

typedef std::set<Hand, HandComparator> Hands;

ComboRank findBestComboRank(Hand hand)
{
    Hands possibleHands;
    ComboRank bestComboRank = hand.comboRank_;

    switch (hand.jLocations_.size())
    {
        case 5:
        case 4:
        {
            bestComboRank = FiveOfAKind;
            break;
        }
        case 3:
        {
            switch (hand.comboRank_)
            {
                case FiveOfAKind:
                case FullHouse:
                {
                    bestComboRank = FiveOfAKind;
                    break;
                }
                case FourOfAKind:
                case ThreeOfAKind:
                case TwoPair:
                case OnePair:
                {
                    bestComboRank = FourOfAKind;
                    break;
                }
                case HighCard:
                {
                    bestComboRank = FourOfAKind;
                    break;
                }
            }
            break;
        }
        case 2:
        {
            switch (hand.comboRank_)
            {
                case FiveOfAKind:
                case FourOfAKind:
                case FullHouse:
                case ThreeOfAKind:
                {
                    bestComboRank = FiveOfAKind;
                    break;
                }
                case TwoPair:
                {
                    bestComboRank = FourOfAKind;
                    break;
                }
                case OnePair:
                {
                    bestComboRank = ThreeOfAKind;
                    break;
                }
                case HighCard:
                {
                    bestComboRank = ThreeOfAKind;
                    break;
                }
            }
            break;
        }
        case 1:
        {
            switch (hand.comboRank_)
            {
                case FiveOfAKind:
                case FourOfAKind:
                {
                    bestComboRank = FiveOfAKind;
                    break;
                }
                case FullHouse:
                {
                    bestComboRank = FourOfAKind;
                    break;
                }
                case ThreeOfAKind:
                {
                    bestComboRank = FourOfAKind;
                    break;
                }
                case TwoPair:
                {
                    bestComboRank = FullHouse;
                    break;
                }
                case OnePair:
                {
                    bestComboRank = ThreeOfAKind;
                    break;
                }
                case HighCard:
                {
                    bestComboRank = OnePair;
                    break;
                }
            }
            break;
        }
        default:
            break;
    }

    return bestComboRank;
}



/*
void searchCard(Hands& searchHands, Hand currentHand, int position)
{
    std::string hand = currentHand.cardsAsString_;

    for (auto& cardRank : cardRanks)
    {
        hand[position] = cardRank.first;
        searchHands.emplace(Hand(hand, currentHand.bid_));
    }
}

void findBestJReplacements(Hand& hand)
{
    Hand bestReplacements;







    Hands bestHands;

    std::string testHandAsString;
    int handSize = hand.cardsAsString_.size();

    for (auto& c0 : cardRanks)
    {

        for (auto& c1 : cardRanks)
        {
            for (auto& c2 : cardRanks)
            {
                for (auto& c3 : cardRanks)
                {
                    for (auto& c4 : cardRanks)
                    {
                    }
                }
            }
        }
    }


        hand.cardsAsString_;
        if (hand.cardsAsString_[i] == 'J')
        {

            CardRanks cardRanks;
            for (auto& carRank : cardRanks)
            {
                bestHands.emplace(Hand(testHandAsString, hand.bid_));
            }
        }
    }
}
*/

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


void readInputFile(std::string fileName, Hands& hands)
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
                    if (!line.empty())
                    {
                        auto parts = split(line, " ");
                        Hand hand(parts[0], std::stoi(parts[1]));

#ifdef PART2
                        if (!hand.jLocations_.empty())
                        {
                            if (hand.cardsAsString_ == "QQJ9J")
                            {
                                int a = 0;
                            }
                            // There is at least a single J that needs special handling
                            ComboRank bestComboRank = findBestComboRank(hand);
                            // Replace originally calculated combo rank
                            hand.comboRank_ = bestComboRank;
                        }
#endif
                        hands.emplace(hand);
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

    Hands hands;

    readInputFile(inputFileName, hands);

    if (hands.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Walk through the sorted set
            int count = 1;
            int total = 0;
            for(auto& hand : hands)
            {
                total += count * hand.bid_;
                count++;
            }
            std::cout << "Total Winning: " << total;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Walk through the sorted set
            int count = 1;
            int total = 0;
            for (auto& hand : hands)
            {
                total += count * hand.bid_;
                count++;
            }
            std::cout << "Total Winning: " << total;
        }

        std::cout << std::endl;

    }
}
