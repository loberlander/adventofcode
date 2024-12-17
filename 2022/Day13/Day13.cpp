#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";


static const char delimiterOpen = '[';
static const char delimiterClose = ']';
static const char delimiterComma = ',';

enum Eval
{
    EvalCorrect,
    EvalIncorrect,
    EvalUndecided,

};

class Element
{
public:
    Element()
        : isList_(false)
        , isEmpty_(true)
        , number_(0)
    {}
/*
    Element(Element& element)
        : isList_(true)
        , isEmpty_(false)
        , number_(0)
    {
        elements_.emplace_back(element);
    }
*/
    Element(std::vector<Element> elements)
        : isList_(true)
        , isEmpty_(false)
        , elements_(elements)
        , number_(0)
    {}

    Element(int data)
        : isList_(false)
        , isEmpty_(false)
        , number_(data)
    {}

    int number_;
    std::vector<Element> elements_;
    bool isList_;
    bool isEmpty_;

};

class Message
{
public:
    Message(std::string line)
        : line_(line)
    {
        elements_ = decode(line, valueStack_);
    }

    std::vector<Element> decode(std::string& line, std::string& valueStack)
    {
        std::vector<Element> elements;

        while (!line.empty())
        {
            char ch = line.at(0);
            line.erase(0, 1);

            switch (ch)
            {
                case delimiterOpen:
                {
                    // List of elements is starting
                    elements.emplace_back(decode(line, valueStack));
                    break;
                }

                case delimiterClose:
                {
                    // List of elements is ending
                    if (!valueStack.empty())
                    {
                        // Add a pending value to the list
                        elements.emplace_back(Element(std::stoi(valueStack)));
                        valueStack.clear();
                    }
/*
                    if (elements.empty())
                    {
                        // Must have been an empty element
                        elements.emplace_back(Element());
                    }
*/
                    return elements;
                }

                case delimiterComma:
                {
                    if (!valueStack.empty())
                    {
                        // Add a pending value to the list
                        elements.emplace_back(Element(std::stoi(valueStack)));
                        valueStack.clear();
                    }
                    break;
                }

                default:
                    if (std::isdigit(ch))
                    {
                        // Must be a digit
                        valueStack += ch;
                    }
                    else
                    {
                        // should never get here
                    }
                    break;
            }
        }

        return elements;
    }

    Eval checkElements(const Element& elementLeft, const Element& elementRight) const
    {
        if (elementLeft.isList_ && elementRight.isList_)
        {
            size_t elementsSizeLeft = elementLeft.elements_.size();
            size_t elementsSizeRight = elementRight.elements_.size();
            size_t elementListSize = elementsSizeRight;

            if (elementsSizeLeft == 0 && elementsSizeRight == 0)
            {
                // comparing two empty lists: [] [] 
                return EvalUndecided;
            }

            if (elementsSizeLeft > elementsSizeRight)
            {
                elementListSize = elementsSizeLeft;
            }

            Eval result = EvalUndecided;
            for (size_t i = 0; i <= elementListSize; i++)
            {
                if (elementsSizeLeft == elementsSizeRight
                    && i >= elementsSizeLeft
                    && result == EvalUndecided)
                {
                    return EvalUndecided;
                }
                if (i >= elementsSizeLeft)
                {
                    // Left is out of elements, correct order
                    return EvalCorrect;
                }
                if (i >= elementsSizeRight)
                {
                    // Left is out of elements, incorrect order
                    return EvalIncorrect;
                }
                result = checkElements(elementLeft.elements_[i], elementRight.elements_[i]);
                if (result == EvalIncorrect || result == EvalCorrect)
                {
                    return result;
                }
            }
        }
        else if (elementLeft.isList_ && !elementRight.isList_)
        {
            std::vector<Element> fixedElements;
            fixedElements.emplace_back(elementRight);
            return checkElements(elementLeft, fixedElements);
        }
        else if (!elementLeft.isList_ && elementRight.isList_)
        {
            std::vector<Element> fixedElements;
            fixedElements.emplace_back(elementLeft);
            return checkElements(fixedElements, elementRight);
        }
        else if (!elementLeft.isList_ && !elementRight.isList_)
        {
            if (elementLeft.number_ < elementRight.number_)
            {
                return EvalCorrect;
            }
            else if (elementLeft.number_ == elementRight.number_)
            {
                return EvalUndecided;
            }
            return EvalIncorrect;
        }

        // should never get here
        return EvalCorrect;
    }

    bool operator < (const Message& other) const
    {
        // Must be strict weak ordering otherwise assert will trigger in framework
        // for more detail: https://www.boost.org/sgi/stl/StrictWeakOrdering.html
        // comp(x, x) must return false!
        // comp(x, y) must return tha same as !comp(y, x)

        if (!other.elements_.empty() && !elements_.empty())
        {
            Eval result = checkElements(elements_[0], other.elements_[0]);
            return result == EvalCorrect;
        }
        
        return false;
    }

    std::string line_;
    std::vector<Element> elements_;
    std::string valueStack_;

};


class Packet
{
public:
    Packet(Message left, Message right)
        : left_(left)
        , right_(right)
    {}

    Message left_;
    Message right_;
};

typedef std::vector<Packet> Packets;

void readInputFile(std::string fileName, Packets& packets)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

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
                        lineLeft = line;
                        state++;
                        break;
                    }
                    case 2:
                    {
                        lineRight = line;

                        packets.emplace_back(Message(lineLeft), Message(lineRight));

                        state = 1;
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
    Packets packets;

    readInputFile(inputFileName, packets);


    if (packets.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            size_t numPackets = packets.size();
            size_t totalCorrect = 0;
            for (size_t i = 0; i < numPackets; i++)
            {
                if (packets[i].left_ < packets[i].right_)
                {
                    totalCorrect += (i + 1);
                }

                if ((packets[i].left_ < packets[i].right_) != !(packets[i].right_ < packets[i].left_))
                {
                    // put a breakpoint here, it should never trigger
                    std::cout << "bingo" << std::endl;
                }
            }
            std::cout << "Correct index total: " << totalCorrect;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            {

                std::set<Message> elementSet;

                Message special2("[[2]]");
                Message special6("[[6]]");

                for (auto& packet : packets)
                {
                    elementSet.emplace(packet.left_);
                    elementSet.emplace(packet.right_);
                }

                // Add the marker elements last and save their iterators!
                auto it2 = elementSet.emplace(special2);
                auto it6 = elementSet.emplace(special6);

                size_t index2 = std::distance(elementSet.begin(), it2.first) + 1;
                size_t index6 = std::distance(elementSet.begin(), it6.first) + 1;
                std::cout << "[[2]] index: " << index2 << std::endl;
                std::cout << "[[6]] index: " << index6 << std::endl;
                std::cout << "Correct product total: " << index2 * index6;
                std::cout << std::endl;

            }
        }
    }
}
