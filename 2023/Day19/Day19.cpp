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
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

static const std::string delimiterStart = "{";
static const std::string delimiterEnd = "}";
static const std::string delimiterList = ",";
static const std::string delimiterWorkflow = ":";
static const std::string delimiterLessThan = "<";
static const std::string delimiterBiggerThan = ">";

typedef long long BigNumber;

typedef std::map<char, BigNumber> PartRatings;

class Rating
{
public:
    Rating(PartRatings partRatings)
        : partRatings_(partRatings)
    {

    }

    PartRatings partRatings_;
};

typedef std::vector<Rating> Ratings;

class Range
{
public:
    Range()
        : x1_(0)
        , x2_(0)
    {

    }

    Range(BigNumber x1, BigNumber x2)
        : x1_(std::min(x1, x2))
        , x2_(std::max(x1, x2))
    {

    }

    bool isInRange(BigNumber y)
    {
        if (x1_ <= y && y <= x2_)
        {
            return true;
        }
        return false;
    }

    // Always return two ranges, one range can be empty
    std::vector<Range> splitAt(BigNumber y)
    {
        std::vector<Range> newRanges;

        if (isInRange(y))
        {
            newRanges.emplace_back(x1_, y);
        }
        else
        {
            //return
            newRanges.emplace_back(0, 0);
        }

        if (x1_ <= y && y < x2_)
        {
            newRanges.emplace_back(y + 1, x2_);
        }
        else
        {
            newRanges.emplace_back(0, 0);
        }

        return newRanges;
    }

    BigNumber size()
    {
        return x2_ - x1_ + 1;
    }

    BigNumber x1_;
    BigNumber x2_;
};

typedef std::map<char, Range> Ranges;

enum RuleType
{
    RuleAccept = 'A',
    RuleReject = 'R',
    RuleWorkflow = 'W',
    RuleLessThan = '<',
    RuleBiggerThan = '>',
    RuleInconclusive = 'I',
    RuleUnknown = '?'
};

class Rule
{
public:
    Rule()
    {

    }

    Rule(std::string ruleAsString)
        : ruleAsString_(ruleAsString)
    {
        ruleType_ = RuleUnknown;

        if (ruleAsString.size() == 1)
        {
            if (ruleAsString == "A")
            {
                ruleType_ = RuleAccept;
            }
            else if (ruleAsString == "R")
            {
                ruleType_ = RuleReject;
            }
        }
        else
        {
            size_t tokenPosition1 = 0;
            tokenPosition1 = ruleAsString.find(delimiterLessThan, 0);
            if (tokenPosition1 != std::string::npos)
            {
                ruleType_ = RuleLessThan;
            }

            size_t tokenPosition2 = 0;
            tokenPosition2 = ruleAsString.find(delimiterBiggerThan, 0);
            if (tokenPosition2 != std::string::npos)
            {
                ruleType_ = RuleBiggerThan;
            }

            if (ruleType_ == RuleLessThan || ruleType_ == RuleBiggerThan)
            {
                category_ = ruleAsString[0];
                ruleAsString.erase(0, 2);

                size_t tokenPosition = ruleAsString.find(delimiterWorkflow, 0);
                if (tokenPosition != std::string::npos)
                {
                    std::string valueAsString = ruleAsString.substr(0, tokenPosition);
                    limit_ = std::stoi(valueAsString);
                    ruleAsString.erase(0, tokenPosition + delimiterWorkflow.length());

                    if (ruleAsString.size() == 1)
                    {
                        if (ruleAsString == "A")
                        {
                            actionType_ = RuleAccept;
                        }
                        else if (ruleAsString == "R")
                        {
                            actionType_ = RuleReject;
                        }
                    }
                    else
                    {
                        actionType_ = RuleWorkflow;
                        id_ = ruleAsString;
                    }
                }
            }
            else
            {
                ruleType_ = RuleWorkflow;
                id_ = ruleAsString;
            }
        }
    }

    RuleType getAction(Rating& rating, std::string& nextId)
    {
        switch (ruleType_)
        {
            case RuleAccept:
            case RuleReject:
            {
                return ruleType_;
                break;
            }
            case RuleWorkflow:
            {
                nextId = id_;
                return ruleType_;
                break;
            }
            case RuleLessThan:
            case RuleBiggerThan:
            {
                BigNumber ratingValue = rating.partRatings_[category_];
                if ((ruleType_ == RuleLessThan && ratingValue < limit_) ||
                    (ruleType_ == RuleBiggerThan && ratingValue > limit_))
                {
                    switch (actionType_)
                    {
                        case RuleAccept:
                        case RuleReject:
                        {
                            return actionType_;
                            break;
                        }
                        case RuleWorkflow:
                        {
                            nextId = id_;
                            return actionType_;
                            break;
                        }
                        default:
                        {
                            // should never get here
                            break;
                        }
                    }
                }
                else
                {
                    return RuleInconclusive;
                }
                break;
            }
            default:
                // should never get here
                break;
        }

        // should never get here
        return RuleUnknown;
    }

    RuleType getAction(Ranges& rangesIn, Ranges& rangesOutTrue, Ranges& rangesOutFalse, std::string& nextId)
    {
        rangesOutTrue = rangesIn;
        rangesOutFalse = rangesIn;

        switch (ruleType_)
        {
            case RuleAccept:
            case RuleReject:
            {
                return ruleType_;
                break;
            }
            case RuleWorkflow:
            {
                nextId = id_;
                return RuleWorkflow;
                break;
            }
            case RuleLessThan:
            case RuleBiggerThan:
            {
                Range range = rangesIn[category_];
                std::vector<Range> splitRanges;
                if (ruleType_ == RuleLessThan)
                {
                    splitRanges = range.splitAt(limit_ - 1);
                    rangesOutTrue[category_] = splitRanges[0];
                    rangesOutFalse[category_] = splitRanges[1];
                }
                else
                {
                    splitRanges = range.splitAt(limit_);
                    rangesOutTrue[category_] = splitRanges[1];
                    rangesOutFalse[category_] = splitRanges[0];
                }

                switch (actionType_)
                {
                    case RuleAccept:
                    case RuleReject:
                    {
                        return actionType_;
                        break;
                    }
                    case RuleWorkflow:
                    {
                        nextId = id_;
                        return RuleWorkflow;
                        break;
                    }
                    default:
                    {
                        // should never get here
                        break;
                    }
                }

                // Should not get here
                return RuleUnknown;
                break;
            }
            default:
                // should never get here
                break;
        }

        // should never get here
        return RuleUnknown;
    }

    std::string ruleAsString_;
    RuleType ruleType_;
    char category_;
    std::string id_;
    RuleType actionType_;
    BigNumber limit_;
};

typedef std::vector<Rule> Rules;

class Workflow
{
public:
    Workflow()
    {

    }

    Workflow(std::string id, std::vector<std::string> rulesAsVector)
        : id_(id)
        , rulesAsVector_(rulesAsVector)
    {
        for (auto& ruleAsString : rulesAsVector)
        {
            rules_.emplace_back(ruleAsString);
        }
    }

    std::string id_;
    Rules rules_;
    std::vector<std::string> rulesAsVector_;
};

typedef std::map<std::string, Workflow> WorkflowsAsMap; // id is used as key


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


void readInputFile(std::string fileName, WorkflowsAsMap& workflowsAsMap, Ratings& ratings)
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
                    // Read Workflows
                    if (!line.empty())
                    {
                        size_t tokenPosition = 0;

                        tokenPosition = line.find(delimiterStart, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            std::string id = line.substr(0, tokenPosition);

                            line.erase(0, tokenPosition + delimiterStart.length());

                            tokenPosition = line.find(delimiterEnd, 0);

                            std::string rulesAsString;
                            if (tokenPosition != std::string::npos)
                            {
                                rulesAsString = line.substr(0, tokenPosition);
                                line.erase(0, tokenPosition + delimiterEnd.length());
                            }

                            std::vector<std::string> rulesAsVector = split(rulesAsString, delimiterList);

                            Workflow element(id, rulesAsVector);

                            workflowsAsMap.emplace(id, element);
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
                    // Read Ratings
                    if (!line.empty())
                    {
                        size_t tokenPosition = 0;

                        tokenPosition = line.find(delimiterStart, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            std::string id = line.substr(0, tokenPosition);

                            line.erase(0, tokenPosition + delimiterStart.length());

                            tokenPosition = line.find(delimiterEnd, 0);

                            std::string ratingAsString;
                            if (tokenPosition != std::string::npos)
                            {
                                ratingAsString = line.substr(0, tokenPosition);
                                line.erase(0, tokenPosition + delimiterEnd.length());
                            }

                            std::vector<std::string> rulesAsVector = split(ratingAsString, delimiterList);

                            PartRatings partRatings;
                            for (auto& element : rulesAsVector)
                            {
                                BigNumber value = std::stoi(element.substr(2));
                                partRatings.emplace(element[0], value);
                            }
                            ratings.emplace_back(partRatings);
                        }
                    }
                }
            }
        }
        myFile.close();
    }
}


RuleType evaluate(Rating& rating, WorkflowsAsMap& workflowsAsMap, std::string id)
{
    RuleType result = RuleUnknown;

    if (id.empty())
    {
        // should never get here
        result = result;
    }
    else
    {
        Rules rules = workflowsAsMap[id].rules_;

        for (auto& rule : rules)
        {
            std::string nextId;
            RuleType ruleAction = rule.getAction(rating, nextId);

            switch (ruleAction)
            {
                case RuleAccept:
                case RuleReject:
                {
                    return ruleAction;
                    break;
                }
                case RuleWorkflow:
                {
                    return evaluate(rating, workflowsAsMap, nextId);
                    break;
                }
                case RuleInconclusive:
                {
                    // Continue to the next rule
                    continue;
                    break;
                }
                default:
                {
                    // should never get here, all actions should resolve to reject or accept
                    break;
                }
            }
        }
    }

    return result;
}

BigNumber evaluate(WorkflowsAsMap& workflowsAsMap, Ranges ranges, std::string id)
{
    BigNumber result = 0;

    if (id.empty())
    {
        // should never get here
        result = result;
    }
    else
    {
        Rules rules = workflowsAsMap[id].rules_;
        for (auto& rule : rules)
        {
            std::string nextId;
            Ranges newRangesTrue;
            Ranges newRangesFalse;
            RuleType ruleAction = rule.getAction(ranges, newRangesTrue, newRangesFalse, nextId);

            ranges = newRangesFalse; // Prepare ranges for iteration for the false cases
            if (ruleAction == RuleReject)
            {
                // no range left to work with
                continue;
            }

            if (nextId.empty())
            {
                result += newRangesTrue['x'].size() * newRangesTrue['m'].size() * newRangesTrue['a'].size() * newRangesTrue['s'].size();
            }
            else
            {
                result += evaluate(workflowsAsMap, newRangesTrue, nextId);
            }

            // Must process newRangesFalse portion, so keep looping
        }
    }

    return result;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    WorkflowsAsMap workflowsAsMap;
    Ratings ratings;

    readInputFile(inputFileName, workflowsAsMap, ratings);

    if (workflowsAsMap.empty() || ratings.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;

            for (auto& rating : ratings)
            {
                RuleType result = evaluate(rating, workflowsAsMap, "in");
                if (result == RuleAccept)
                {
                    BigNumber part = rating.partRatings_['x'] + rating.partRatings_['m'] + rating.partRatings_['a'] + rating.partRatings_['s'];
                    total += part;
                }
            }
            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;
            Range range(1, 4000);
            Ranges ranges = { { 'x', range }, { 'm', range }, { 'a', range }, { 's', range } };

            total = evaluate(workflowsAsMap, ranges, "in");

            std::cout << "The total is: " << total;

        }

        std::cout << std::endl;

    }
}
