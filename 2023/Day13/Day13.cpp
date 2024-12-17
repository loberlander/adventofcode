#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";

// Too low: 38174

typedef std::vector<std::string> Pattern;
typedef std::vector<Pattern> Patterns;

typedef unsigned long long BigNumber;

struct SearchKey
{
    size_t numRowsRefleced;
    size_t numRowsAbove;
    bool isRow;
};

bool operator<(const SearchKey& sk1, const SearchKey& sk2)
{
    return sk1.numRowsRefleced < sk2.numRowsRefleced;
}

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


void readInputFile(std::string fileName, Patterns& patterns)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        Pattern pattern;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                pattern.emplace_back(line);
            }
            else
            {
                patterns.emplace_back(pattern);
                pattern.clear();
            }
        }
        if (!pattern.empty())
        {
            patterns.emplace_back(pattern);
            pattern.clear();
        }
        myFile.close();
    }
}

Pattern transpose(Pattern& pattern)
{
    Pattern transposed;

    size_t numRows = pattern.size();
    size_t numcolumns = pattern[0].size();

    for (size_t column = 0; column < numcolumns; column++)
    {
        std::string line;
        for (size_t row = 0; row < numRows; row++)
        {
            line += pattern[row][column];
        }
        transposed.push_back(line);
    }

    return transposed;
}

void countMatchesAt(Pattern& pattern, SearchKey& searchKey)
{
    searchKey.numRowsRefleced = 0;

    size_t row1 = searchKey.numRowsAbove;
    size_t row2 = row1 + 1;
    size_t numRows = pattern.size();
    bool done = false;

    while (!done)
    {
        if (pattern[row1] == pattern[row2])
        {
            searchKey.numRowsRefleced++;
            if (row1 > 0 && row2 < numRows - 1)
            {
                row1--;
                row2++;
            }
            else
            {
                done = true;
            }
        }
        else
        {
            // Must match maximum numbers
            searchKey.numRowsRefleced = 0;
            done = true;
        }
    }
}

BigNumber calculateReflectionScore1(Pattern& pattern, SearchKey& foundSearchKey)
{
    BigNumber result = 0;

    size_t numRows = pattern.size();
    size_t numcolumns = pattern[0].size();

    std::set<SearchKey> bestMatch;
    std::vector<SearchKey> matches;

    for (size_t row = 0; row < numRows - 1; row++)
    {
        if (pattern[row] == pattern[row + 1])
        {
            // Found a match
            SearchKey searchKey;
            searchKey.numRowsRefleced = 0;
            searchKey.numRowsAbove = row;
            searchKey.isRow = true;

            countMatchesAt(pattern, searchKey);
            if (searchKey.numRowsRefleced > 0)
            {
                //matches.emplace_back(searchKey);
                bestMatch.emplace(searchKey);
            }
        }
    }

    Pattern patternT = transpose(pattern);
    for (size_t row = 0; row < numcolumns - 1; row++)
    {
        if (patternT[row] == patternT[row + 1])
        {
            // Found a match
            SearchKey searchKey;
            searchKey.numRowsRefleced = 0;
            searchKey.numRowsAbove = row;
            searchKey.isRow = false;

            countMatchesAt(patternT, searchKey);

            if (searchKey.numRowsRefleced > 0)
            {
                //matches.emplace_back(searchKey);
                bestMatch.emplace(searchKey);
            }
        }
    }
    /*
    std::set<SearchKey> bestMatch;
    for (auto& match : matches)
    {
        if (match.isRow)
        {
            countMatchesAt(pattern, match);
        }
        else
        {
            countMatchesAt(patternT, match);
        }
        bestMatch.emplace(match);
    }
    */
    auto it = bestMatch.rbegin();

    if (it != bestMatch.rend())
    {
        int factor = 1;
        if (it->isRow)
        {
            factor = 100;
        }
        result = (it->numRowsAbove + 1) * factor;
        foundSearchKey = *it;
    }
    else
    {
        // should not get here!
        result = result;
    }

    return result;
}

bool isSameKey(SearchKey& searchKey1, SearchKey& searchKey2)
{
    if ((searchKey1.isRow == searchKey2.isRow) &&
        (searchKey1.numRowsAbove == searchKey2.numRowsAbove) &&
        (searchKey1.numRowsRefleced == searchKey2.numRowsRefleced))
    {
        return true;
    }

    return false;
}


BigNumber calculateReflectionScore2(Pattern& pattern, SearchKey& searchKeyOriginal)
{
    BigNumber result = 0;

    size_t numRows = pattern.size();
    size_t numColumns = pattern[0].size();

    std::set<SearchKey> bestMatch;
    std::vector<SearchKey> matches;

    for (size_t row = 0; row < numRows - 1; row++)
    {
        if (pattern[row] == pattern[row + 1])
        {
            // Found a match
            SearchKey searchKey;
            searchKey.numRowsRefleced = 0;
            searchKey.numRowsAbove = row;
            searchKey.isRow = true;

            countMatchesAt(pattern, searchKey);
            if (searchKey.numRowsRefleced > 0 && !isSameKey(searchKey, searchKeyOriginal))
            {
                //matches.emplace_back(searchKey);
                bestMatch.emplace(searchKey);
            }
        }
    }

    Pattern patternT = transpose(pattern);
    for (size_t row = 0; row < numColumns - 1; row++)
    {
        if (patternT[row] == patternT[row + 1])
        {
            // Found a match
            SearchKey searchKey;
            searchKey.numRowsRefleced = 0;
            searchKey.numRowsAbove = row;
            searchKey.isRow = false;

            countMatchesAt(patternT, searchKey);

            if (searchKey.numRowsRefleced > 0 && !isSameKey(searchKey, searchKeyOriginal))
            {
                //matches.emplace_back(searchKey);
                bestMatch.emplace(searchKey);
            }
        }
    }
    /*
    std::set<SearchKey> bestMatch;
    for (auto& match : matches)
    {
        if (match.isRow)
        {
            countMatchesAt(pattern, match);
        }
        else
        {
            countMatchesAt(patternT, match);
        }
        bestMatch.emplace(match);
    }
    */
    auto it = bestMatch.rbegin();

    if (it != bestMatch.rend())
    {
        int factor = 1;
        if (it->isRow)
        {
            factor = 100;
        }
        result = (it->numRowsAbove + 1) * factor;
    }
    else
    {
        // should not get here!
        result = result;
    }

    return result;
}


BigNumber findSmudgedScore(Pattern& pattern)
{
    size_t numRows = pattern.size();
    size_t numColumns = pattern[0].size();
    BigNumber score = 0;

    SearchKey searchKeyOriginal;
    calculateReflectionScore1(pattern, searchKeyOriginal);

    for (size_t row = 0; row < numRows; row++)
    {
        for (size_t column = 0; column < numColumns; column++)
        {
            Pattern patternP = pattern;
            if (patternP[row][column] == '.')
            {
                patternP[row][column] = '#';
            }
            else
            {
                patternP[row][column] = '.';
            }

            SearchKey searchKey;
            score = calculateReflectionScore2(patternP, searchKeyOriginal);
            //score = calculateReflectionScore1(patternP, searchKey);
            if (score > 0) // && !isSameKey(searchKey, searchKeyOriginal))
            {
                return score;
            }
        }
    }

    return score;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Patterns patterns;

    readInputFile(inputFileName, patterns);

    if (patterns.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;
            for (auto& pattern : patterns)
            {
                SearchKey searchKey;
                total += calculateReflectionScore1(pattern, searchKey);
            }
            

            std::cout << "Summary: " << total;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;
            for (auto& pattern : patterns)
            {
                total += findSmudgedScore(pattern);
            }


            std::cout << "Summary: " << total;
        }

        std::cout << std::endl;

    }
}
