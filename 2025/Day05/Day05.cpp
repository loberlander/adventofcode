#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;

class Range
{
    public:
        Range()
            : low_(0)
            , high_(0)
            , size_(high_ - low_ + 1)
        {
        }

        Range(BigNumber low, BigNumber high)
            : low_(low)
            , high_(high)
            , size_(high_ - low_ + 1)
        {
            if (low_ > high_)
            {
                throw;
            }
        }

        BigNumber low()
        {
            return low_;
        }

        BigNumber high()
        {
            return high_;
        }

        BigNumber size()
        {
            return size_;
        }

        bool isInRange(BigNumber id)
        {
            return low_ <= id && id <= high_;
        }

        bool isIntersection(Range& other)
        {
            // <------range 1------>
            //         <------range 2------>

            BigNumber intersection_low = std::max(other.low_, low_);
            BigNumber intersection_high = std::min(other.high_, high_);

            return intersection_low <= intersection_high;
        }

        bool merge(Range& other)
        {
            // <------range 1------>
            //         <------range 2------>

            if (!isIntersection(other))
            {
                return false;
            }

            low_ = std::min(other.low_, low_);
            high_ = std::max(other.high_, high_);
            size_ = high_ - low_ + 1;

            return true;
        }

private:
        BigNumber low_;
        BigNumber high_;
        BigNumber size_;
};

typedef std::vector<Range> Ranges;
typedef std::vector<BigNumber> Ids;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::vector<std::string> delimiters)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;
    if (delimiters.empty())
    {
        return result;
    }

    int delimIndex = 0;

    while (!line.empty())
    {
        std::string delimiter = delimiters[delimIndex];

        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());

                delimIndex = delimIndex < delimiters.size() - 1 ? delimIndex + 1 : delimIndex;
                delimiter = delimiters[delimIndex];
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


void readInputFile(std::string fileName, Ranges& ranges, Ids& ids)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read ranges
                    if (!line.empty())
                    {
                        auto ids = split(line, { "-" });
                        if (ids.size() == 2)
                        {
                            Range range(std::stoll(ids[0]), std::stoll(ids[1]));
                            ranges.emplace_back(range);
                        }
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    // Read IDs
                    if (!line.empty())
                    {
                        ids.emplace_back(std::stoll(line));
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 3:
                {
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

    Ranges ranges;
    Ids ids;

    readInputFile(inputFileName, ranges, ids);

    if (ranges.empty() || ids.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            for (auto& id : ids)
            {
                for (auto& range : ranges)
                {
                    bool isFresh = range.isInRange(id);

                    if (isFresh)
                    {
                        ++result;
                        break;
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            Ranges mergedRanges;

            // Iterate through all the defined ranges and build a new "merged" list with the goal of eliminating any overlaps
            for (auto& newRange : ranges)
            {
                bool isDone = false;

                // Merge the "new" (next) range into the merged list
                while (!isDone)
                {
                    // Create an iterator to save the element where merge happen
                    // Use auto so the exact iterator type does not need to be figured out
                    //auto mergedIt = mergedRanges.begin();
                    Ranges::iterator mergedIt;

                    bool isMerged = false;

                    // The new range might overlap with multiple already present merged ranges, pick the first one that it can be merged with
                    // Iterate through all the already existing merged ranges and find one that the new range overlaps with (if any)
                    for (auto it = mergedRanges.begin(); it != mergedRanges.end(); it++)
                    {
                        // attempt to merge new range into an already existing range that overlaps
                        isMerged = it->merge(newRange);
                        if (isMerged)
                        {
                            // merge success
                            mergedIt = it;
                            break;
                        }
                    }

                    if (isMerged)
                    {
                        // Merged successfully with one of the ranges already in the merged list!
                        // Must check if the newly created (merged) range overlaps with any other range in the merged list...
                        // Make a copy of the merged range so that it can be fed back
                        newRange = *mergedIt; // reuse newRange so the same merging logic can be used from above
                        // Remove range from the merged ranges and keep looping while reusing newRange
                        mergedRanges.erase(mergedIt);
                    }
                    else
                    {
                        // Merge with an already existing range did not happen (there was no overlap), so just add the new range as is!
                        mergedRanges.emplace_back(newRange);
                        isDone = true;
                    }
                }
            }

            // Calculate the sum of thesize of merged ranges
            for (auto& range : mergedRanges)
            {
                result += range.size();
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
