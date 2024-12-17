#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";
static const std::string delimiterRuleNum = ": ";
static const std::string delimiterRuleLetter = "\"";
static const std::string delimiterRuleOr = " | ";
static const std::string delimiterRule = " ";

struct Rule
{
    Rule(int inputId, std::string inputRuleString, std::vector<std::vector<int>>inputIds, std::vector<std::string> inputMatches) :
        id(inputId),
        ruleString(inputRuleString),
        ids(inputIds),
        matches(inputMatches)
    {}
    int id;
    std::string ruleString;
    std::vector<std::vector<int>> ids;
    std::vector<std::string> matches;
};

struct Playfield
{
    std::map<int, struct Rule> rules;
    std::vector<std::string> messages;
};

std::vector<int> parseRule(std::string line)
{
    std::vector<int> rule;
    int id;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        tokenPosition = line.find(delimiterRule, 0);
        if (tokenPosition != std::string::npos)
        {
            id = std::stoi(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition + delimiterRule.length());
        }
        else
        {
            id = std::stoi(line.substr(0, line.size()));
            line.erase(0, line.size());
        }
        rule.push_back(id);
    }

    return rule;
}

struct Rule parseLineRule(std::string line)
{
    int id;
    std::string lineDuplicate(line);
    std::vector<std::string> match;
    std::vector<std::vector<int>> rules;

    size_t tokenPosition = 0;
    tokenPosition = line.find(delimiterRuleNum, 0);
    if (tokenPosition != std::string::npos)
    {
        id = std::stoi(line.substr(0, tokenPosition));
        line.erase(0, tokenPosition + delimiterRuleNum.length());
    }

    tokenPosition = line.find(delimiterRuleLetter, 0);
    if (tokenPosition != std::string::npos)
    {
        line.erase(0, tokenPosition + delimiterRuleLetter.length());

        tokenPosition = line.find(delimiterRuleLetter, 0);
        if (tokenPosition != std::string::npos)
        {
            match.push_back(line.substr(0, tokenPosition));
        }
        line.erase(0, tokenPosition + delimiterRuleLetter.length());
    }

    while (!line.empty())
    {
        tokenPosition = line.find(delimiterRuleOr, 0);

        if (tokenPosition != std::string::npos)
        {
            rules.push_back(parseRule(line.substr(0, tokenPosition)));
            line.erase(0, tokenPosition + delimiterRuleOr.length());
        }
        else
        {
            rules.push_back(parseRule(line.substr(0, line.size())));
            line.erase(0, line.size());
        }
    }

    return Rule(id, lineDuplicate, rules, match);
}

bool isMessagePartialMatch(std::vector<std::string>& messages, std::string& rule)
{
    // Check if there is at least one message that partially matches the rule
    size_t ruleLength = rule.size();

    for (auto& message : messages)
    {
        size_t compLength = ruleLength;
        if (message.size() < ruleLength)
        {
            compLength = message.size();
        }

        if (message.substr(0, compLength) == rule.substr(0,compLength))
        {
            return true;
        }
    }
    return false;
}

// return true when a new evaluation completed
bool evaluateRuleIfNeeded(struct Rule & currentRule, struct Playfield & playfield, size_t maxMessageLength)
{
    bool looping = false;
    if (currentRule.matches.empty())
    {
        //std::vector<std::vector<std::string>> matches; // This is where we collect the matches this rule produces
        std::vector<std::string> matches;

        // Loop through the ids this rule needs to match
        for (auto& idVec : currentRule.ids)
        {
            std::vector<std::string> match1;
            for (auto& id : idVec)
            {
                auto mapIt = playfield.rules.find(id);
                if (mapIt != playfield.rules.end())
                {
                    if (id == currentRule.id)
                    {
                        // Looping on the same rule!!!
                        looping = true;
                    }

                    // Let's see if this rule id has been fully evaluated already
                    if (!mapIt->second.matches.empty() || looping)
                    {
                        if (match1.empty())
                        {
                            match1 = mapIt->second.matches;
                        }
                        else
                        {
                            std::vector<std::string>& matchSoFar = mapIt->second.matches;

                            if (looping)
                            {
                                matchSoFar = matches;
                            }

                            std::vector<std::string> oldMatches = match1;
                            match1.resize(0); // Erase old content
                            for (auto& ruleMatches : oldMatches)
                            {
                                for (auto& match : matchSoFar)
                                {
                                    std::string newRule = ruleMatches + match;
                                    // Add all the found matches to the matches if it makes sense
                                    if ((currentRule.id == 0) || (looping && (currentRule.id == 8)))
                                    {
                                        //if (isMessagePartialMatch(playfield.messages, newRule))
                                        {
                                            match1.push_back(newRule);
                                        }
                                    }
                                    else
                                    {
                                        match1.push_back(newRule);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // This rule cannot be evaluated fully yet!
                        return false;
                    }
                }
                else
                {
                    // Should never get here!!!
                    return false;
                }
            }

            //matches.push_back(match1);
            bool newMatchesAllLong = true;

            for (auto& mm : match1)
            {
                matches.push_back(mm);
                if (mm.size() < maxMessageLength / 2)
                {
                    newMatchesAllLong = false;
                }
            }
            if (looping && newMatchesAllLong)
            {
                // it is time to exit
                break;
            }

        }

        // If we get this far that means that all dependent rules were successfully evaluated, so the current rule is resolved!
        // Just populate the matches in a one dimensional vector
/*
        for (auto& matchVec : matches)
        {
            for (auto& match : matchVec)
            {
                currentRule.matches.push_back(match);
            }
        }
*/
        if (matches.size() > 0)
        {
            currentRule.matches = matches;
            return true;
        }
    }
    return false;
}


bool isRuleZeroResolved(std::map<int, struct Rule>& rules)
{
    auto mapIt = rules.find(0);
    if (mapIt != rules.end())
    {
        if (!mapIt->second.matches.empty())
        {
            return true;
        }
    }
    return false;
}


void evaluateRules(struct Playfield & playfield, size_t maxMessageLength)
{
    int rulesResolved = 0;
    size_t numRules = playfield.rules.size();

    // Loop until all rules are resolved
    while (!isRuleZeroResolved(playfield.rules))
    {
        for (auto& rule : playfield.rules)
        {
            //std::cout << "Attempting to resolve: " << rule.second.id << std::endl;
            if (evaluateRuleIfNeeded(rule.second, playfield, maxMessageLength))
            {
                //std::cout << "Resolved: " << rule.second.id << std::endl;
                ++rulesResolved;
            }
        }
    }
}

struct Playfield readInputFile(std::string fileName)
{
    struct Playfield playfield;

    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int i = 1;
        while (getline(myFile, line))
        {
            switch (i)
            {
            case 1:
                while (!line.empty())
                {
                    struct Rule rule = parseLineRule(line);
                    playfield.rules.emplace(rule.id, rule);
                    getline(myFile, line);
                }
                break;
            case 2:
                while (!line.empty())
                {
                    playfield.messages.push_back(line);
                    getline(myFile, line);
                }
                break;
            default:
                // ignore if anything
                break;
            }
            ++i;
        }
        myFile.close();
    }

    return playfield;
}

bool isMessageMatchingRuleZero(std::string message, std::vector<std::string> & ruleZero)
{
    size_t msgSize = message.size();

    for (auto& validMessage : ruleZero)
    {
        if ((msgSize == validMessage.size()) && (message == validMessage))
        {
            return true;
        }
    }
    return false;
}

void fixRulesForPart2(std::map<int, struct Rule> & rules)
{
    auto mapIt1 = rules.find(8);
    if (mapIt1 != rules.end())
    {
        rules.erase(mapIt1);
        struct Rule rule = parseLineRule("8: 42 | 42 8");
        rules.emplace(rule.id, rule);
    }

    auto mapIt2 = rules.find(11);
    if (mapIt2 != rules.end())
    {
        rules.erase(mapIt2);
        struct Rule rule = parseLineRule("11: 42 31 | 42 11 31");
        rules.emplace(rule.id, rule);
    }
}

size_t getMaxMessageLength(struct Playfield playfield)
{
    size_t maxMessageLength = 0;
    for (auto& message : playfield.messages)
    {
        if (message.length() > maxMessageLength)
        {
            maxMessageLength = message.length();
        }
    }

    return maxMessageLength;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    struct Playfield playfield1 = readInputFile(inputFileName);
    struct Playfield playfield2 = playfield1;

    fixRulesForPart2(playfield2.rules);
    size_t maxMessageLength = getMaxMessageLength(playfield1);

    if (playfield1.rules.empty() || playfield1.messages.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        evaluateRules(playfield1, maxMessageLength);

        std::cout << "Part 1:" << std::endl;

        int matchedMessages1 = 0;
        auto mapIt1 = playfield1.rules.find(0);
        if (mapIt1 != playfield1.rules.end())
        {
            std::vector<std::string>& validMessages1 = mapIt1->second.matches;
            for (auto& message : playfield1.messages)
            {
                if (isMessageMatchingRuleZero(message, validMessages1))
                {
                    ++matchedMessages1;
                }
            }
        }

        std::cout << "Matched messages: " << matchedMessages1 << std::endl;

        std::cout << "Part 2:" << std::endl;
        
        evaluateRules(playfield2, maxMessageLength);

        int matchedMessages2 = 0;
        auto mapIt2 = playfield2.rules.find(0);
        if (mapIt2 != playfield2.rules.end())
        {
            std::vector<std::string>& validMessages2 = mapIt2->second.matches;
            for (auto& message : playfield2.messages)
            {
                if (isMessageMatchingRuleZero(message, validMessages2))
                {
                    ++matchedMessages2;
                }
            }
        }

        std::cout << "Matched messages: " << matchedMessages2 << std::endl;


        std::cout << std::endl;

        std::cout << std::endl;
    }
}
