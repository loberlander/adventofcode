#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <algorithm>

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "inputTest.txt";

static const long long MAGIC_NUMBER = 811589153;

class Value
{
public:
    Value(long long value, int position, long long magic)
        : visited_(false)
        , value_(value * magic)
        , position_(position)
        , originalPosition_(position)

    {
    }

    bool visited_;
    long long value_;
    int position_;
    int originalPosition_;
};

typedef std::list<Value> Values;

void readInputFile(std::string fileName, Values& values, long long magic = (long long) 1)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);
    int position = 0;

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
                        int value = std::stoi(line);
                        values.emplace_back(value, position, magic);
                        position++;
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

    Values values1;

    readInputFile(inputFileName, values1);

    Values values2;

    readInputFile(inputFileName, values2, MAGIC_NUMBER);

    if (values1.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            long long numElements = values1.size();
            long long numMod = numElements - 1;
            auto it = values1.begin();
            while (it != values1.end())
            {
                auto nextIt = it;
                if (nextIt == values1.end())
                {
                    nextIt = values1.begin();
                }
                nextIt++;

                if (!it->visited_)
                {
                    Value value = *it;
                    value.visited_ = true;

                    it = values1.erase(it);

                    if (value.value_ > 0)
                    {
                        for (int i = 0; i < value.value_ % numMod; i++)
                        {
                            if (it == values1.end())
                            {
                                it = values1.begin();
                            }
                            it++;
                            if (it == values1.end())
                            {
                                it = values1.begin();
                            }
                        }
                    }
                    else if (value.value_ < 0)
                    {
                        for (int i = 0; i < std::abs(value.value_ % numMod); i++)
                        {
                            if (it == values1.begin())
                            {
                                it = values1.end();
                            }
                            it--;
                        }
                    }
                    values1.insert(it, value);
                }
                else
                {
                    it++;
                }

                it = nextIt;
            }

            // Make sure all elements were visited 
            int i = 0;
            it = values1.begin();
            while (it != values1.end())
            {
                if (!it->visited_)
                {
                    std::cout << "Element " << i << "was not visited!" << std::endl;
                }
                it++;
                i++;
            }

            std::cout << "Part 1:" << std::endl;
            {
                int i = 0;
                auto it = values1.begin();
                while (it != values1.end() && it->value_ != 0)
                {
                    it++;
                    i++;
                }

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values1.end())
                    {
                        it = values1.begin();
                    }
                }

                int value1000 = it->value_;

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values1.end())
                    {
                        it = values1.begin();
                    }
                }

                int value2000 = it->value_;

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values1.end())
                    {
                        it = values1.begin();
                    }
                }

                int value3000 = it->value_;

                std::cout << value1000 + value2000 + value3000;
            }

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            for (int loop = 0; loop < 10; loop++)
            {
                long long numElements = values2.size();
                long long numMod = numElements - (long long) 1;
                for (int cycle = 0; cycle < numElements; cycle++)
                {
                    auto it = values2.begin();
                    while (it->originalPosition_ != cycle)
                    {
                        it++;
                    }


                    //while (it != values2.end())
                    {
                        //auto nextIt = it;
                        //if (nextIt == values2.end())
                        //{
                            //nextIt = values2.begin();
                        //}
                        //nextIt++;

                        //if (!it->visited_)
                        {
                            Value value = *it;
                            value.visited_ = true;

                            it = values2.erase(it);

                            //if (it == values2.end())
                            //{
                                //it = values2.begin();
                            //}

                            if (value.value_ > 0)
                            {
                                for (long long i = 0; i < value.value_ % numMod; i++)
                                {
                                    if (it == values2.end())
                                    {
                                        it = values2.begin();
                                    }
                                    it++;
                                }
                            }
                            else if (value.value_ < 0)
                            {
                                for (long long i = 0; i < std::abs(value.value_ % numMod); i++)
                                {
                                    if (it == values2.begin())
                                    {
                                        it = values2.end();
                                    }
                                    it--;
                                }
                            }
                            values2.insert(it, value);
                        }
                        //else
                        //{
                            //it++;
                        //}

                        //it = nextIt;
                    }
                }

                // Clear visited part of each element
                int i = 0;
                auto it = values2.begin();
                while (it != values2.end())
                {
                    it->visited_ = false;
                    it++;
                    i++;
                }
            }

            //std::cout << "Part 2:" << std::endl;
            {
                int i = 0;
                auto it = values2.begin();
                while (it != values2.end() && it->value_ != 0)
                {
                    it++;
                    i++;
                }

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values2.end())
                    {
                        it = values2.begin();
                    }
                }

                long long  value1000 = it->value_;

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values2.end())
                    {
                        it = values2.begin();
                    }
                }

                long long value2000 = it->value_;

                for (int i = 0; i < 1000; i++)
                {
                    it++;
                    if (it == values2.end())
                    {
                        it = values2.begin();
                    }
                }

                long long value3000 = it->value_;

                std::cout << value1000 + value2000 + value3000;
            }

            std::cout << std::endl;
        }
    }
}
