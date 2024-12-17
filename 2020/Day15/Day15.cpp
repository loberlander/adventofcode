#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set>

static const int PUZZLE_LIMIT1 = 2020;

static const int PUZZLE_LIMIT2 = 30000000;

struct Entry
{
    Entry(int val, int lastI, int turnD,  bool FirstT) :
        value(val),
        lastIndex(lastI),
        turnDiff(turnD),
        firstTime(FirstT)
    {}
    int value;
    int lastIndex;
    int turnDiff;
    bool firstTime;
};

struct CustomCompare
{
    // this member is required to let container be aware that 
    // comparator is capable of dealing with types other than key
public: using is_transparent = std::true_type;

      //    bool operator()(const int& lhs, const int& rhs) const
      //    {
      //        return lhs < rhs;
      //    }

      bool operator()(const int& lhs, const Entry& rhs) const
      {
          return lhs < rhs.value;
      }

      bool operator()(const Entry& lhs, const int& rhs) const
      {
          return lhs.value < rhs;
      }

      bool operator()(const Entry& lhs, const Entry& rhs) const
      {
          return lhs.value < rhs.value;
      }
};


void addNew(int newValue, std::vector<int>& puzzle, std::set<Entry, CustomCompare>& lastCache)
{
    bool firstTime = true;
    int turnDiff = 0;
    auto puzzleSize = puzzle.size();

    puzzle.push_back(newValue);
    auto it = lastCache.find(newValue);
    if (it != lastCache.end())
    {
        firstTime = false;
        turnDiff = puzzleSize - it->lastIndex;
        lastCache.erase(it);
    }
    lastCache.emplace(newValue, puzzleSize, turnDiff, firstTime);
}

int takeTurn(std::vector<int> & puzzle, std::set<Entry, CustomCompare>& lastCache)
{
    auto puzzleSize = puzzle.size();
    int lastNumber = puzzle.back();

    auto it = lastCache.find(lastNumber);

    // First time spoken?
    if (it == lastCache.end() ||
        (it != lastCache.end() && it->firstTime && it->lastIndex == puzzleSize - 1))
    {
        return 0;
    }

    //return (int) (puzzleSize - 1 - it->lastIndex);
    return (int)(it->turnDiff);
}

int main()
{
    std::set<Entry, CustomCompare> lastValueCache;
    std::vector<int> puzzle;

    addNew(0, puzzle, lastValueCache);
    addNew(3, puzzle, lastValueCache);
    addNew(1, puzzle, lastValueCache);
    addNew(6, puzzle, lastValueCache);
    addNew(7, puzzle, lastValueCache);
    addNew(5, puzzle, lastValueCache);
/*
    addNew(0, puzzle, lastValueCache);
    addNew(3, puzzle, lastValueCache);
    addNew(6, puzzle, lastValueCache);
*/
    std::vector<int> data;
    std::set<Entry, CustomCompare> lastCache;

    std::cout << "Solving problem..." << std::endl;
    std::cout << "Part 1:" << std::endl;

    data = puzzle;
    lastCache = lastValueCache;
    int loopLimit1 = PUZZLE_LIMIT1 - data.size();

    int lastEntry1 = 0;
    for (int i = 0; i < loopLimit1; i++)
    {
        lastEntry1 = takeTurn(data, lastCache);
        //std::cout << lastEntry1 << " ";
        addNew(lastEntry1, data, lastCache);
    }
    std::cout << std::endl;
    std::cout << "Today's magic number is: " << lastEntry1 << std::endl;

    std::cout << "Part 2:" << std::endl;

    data = puzzle;
    lastCache = lastValueCache;
    int loopLimit2 = PUZZLE_LIMIT2 - data.size();

    int lastEntry2 = 0;
    for (int i = 0; i < loopLimit2; i++)
    {
        lastEntry2 = takeTurn(data, lastCache);
        //std::cout << lastEntry2 << " ";
        addNew(lastEntry2, data, lastCache);
    }
    std::cout << std::endl;
    std::cout << "Today's magic number is: " << lastEntry2 << std::endl;


}
