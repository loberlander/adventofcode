#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <set>

static const char inputFileName[] = "input.txt";
static const std::vector<std::string> mandatoryFields = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
static const std::string optionalField = "cid";
static const std::string fieldNameDelimiter = ":";
static const std::string fieldValueDelimiter = " ";
static const std::string heightFieldValueDelimiterCm = "cm";
static const std::string heightFieldValueDelimiterIn = "in";
static const std::set<std::string> validEcl = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

struct Passport
{
    std::map<std::string, std::string> fields;
    bool isPassportValid1;
    bool isPassportValid2;
    bool isOptionalField;
};
// Format of input:
// pid:827837505 byr:1976
// hgt:187cm
// iyr:2016
// hcl:#fffffd
// eyr:2024
// 
// hgt:189cm byr:1987 pid:572028668 iyr:2014 hcl:#623a2f
// eyr:2028 ecl:amb
// 
// pid:#e9bf38 hcl:z iyr:2029 byr:2028 ecl:#18f71a hgt:174in eyr:2036

bool passportValid(struct Passport & passport)
{
    if (!passport.isPassportValid1)
    {
        return false;
    }

    // Validate byr
    if (passport.fields["byr"].length() != 4) return false;
    int byr = std::stoi(passport.fields["byr"]);
    if ((byr < 1920) || (byr > 2002)) return false;

    // Validate iyr
    if (passport.fields["iyr"].length() != 4) return false;
    int iyr = std::stoi(passport.fields["iyr"]);
    if ((iyr < 2010) || (iyr > 2020)) return false;

    // Validate eyr
    if (passport.fields["eyr"].length() != 4) return false;
    int eyr = std::stoi(passport.fields["eyr"]);
    if ((eyr < 2020) || (eyr > 2030)) return false;

    // Validate hgt
    if (passport.fields["hgt"].empty())
    {
        return false;
    }
    else
    {
        std::string heightRecord = passport.fields["hgt"];
        size_t hgtUnitCmPosition = heightRecord.find(heightFieldValueDelimiterCm, 0);
        if (hgtUnitCmPosition != std::string::npos)
        {
            int hgt = std::stoi(heightRecord.substr(0, hgtUnitCmPosition));
            if ((hgt < 150) || (hgt > 193)) return false;
        }
        else
        {
            size_t hgtUnitInPosition = heightRecord.find(heightFieldValueDelimiterIn, 0);
            if (hgtUnitInPosition != std::string::npos)
            {
                int hgt = std::stoi(heightRecord.substr(0, hgtUnitInPosition));
                if ((hgt < 59) || (hgt > 76)) return false;
            }
            else
            {
                return false;
            }
        }
    }

    // Validate hcl
    if (passport.fields["hcl"].length() != 7)
    {
        return false;
    }
    else
    {
        std::string hcl = passport.fields["hcl"];
        if (hcl[0] != '#') return false;
        hcl.erase(0, 1);
        for (auto ch : hcl)
        {
            if (
                ((ch >= '0') && (ch <= '9')) ||
                ((ch >= 'a') && (ch <= 'f'))
                )
            {
            }
            else return false;
        }
    }

    // Validate ecl
    if (passport.fields["ecl"].length() != 3)
    {
        return false;
    }
    else
    {
        std::string ecl = passport.fields["ecl"];
        if (validEcl.find(ecl) == validEcl.end()) return false;
    }

    // Validate pid
    if (passport.fields["pid"].length() != 9)
    {
        return false;
    }
    else
    {
        std::string pid = passport.fields["pid"];
        for (auto ch : pid)
        {
            if (
                ((ch >= '0') && (ch <= '9'))
               )
            {
            }
            else return false;
        }
    }

    return true;
}


struct Passport parseRecord(std::string line)
{
    struct Passport passport;

    passport.isPassportValid1 = false;
    passport.isPassportValid2 = false;
    passport.isOptionalField = false;

    while (!line.empty())
    {
        size_t fieldNamePosition = 0;

        // Parse the next field name
        fieldNamePosition = line.find(fieldNameDelimiter, 0);
        if (fieldNamePosition != std::string::npos)
        {
            std::string fieldName = line.substr(0, fieldNamePosition);
            line.erase(0, fieldNamePosition + fieldNameDelimiter.length());

            size_t fieldValuePosition = 0;

            // Parse the next field value
            fieldValuePosition = line.find(fieldValueDelimiter, 0);
            if (fieldValuePosition != std::string::npos)
            {
                std::string fieldValue = line.substr(0, fieldValuePosition);
                line.erase(0, fieldValuePosition + fieldValueDelimiter.length());

                passport.fields.emplace(fieldName, fieldValue);
            }
        }
    }

    if (!passport.fields.empty())
    {
        int mandatoryFieldCount = 0;
        for (auto field : mandatoryFields)
        {
            if (passport.fields.find(field) != passport.fields.end())
            {
                // Found a mandatory field!!!
                ++mandatoryFieldCount;
            }
        }

        if (mandatoryFieldCount == mandatoryFields.size())
        {
            passport.isPassportValid1 = true;
        }

        if (passport.fields.find(optionalField) != passport.fields.end())
        {
            passport.isOptionalField = true;
        }

        if (passport.isPassportValid1)
        {
            passport.isPassportValid2 = passportValid(passport);
        }

    }

    return passport;
}

std::vector<struct Passport> readInputFile(std::string fileName)
{
    std::vector<struct Passport> data;
    std::string line;
    std::string record;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (line.empty())
            {
                // A record should be fully parsed...
                if (!record.empty())
                {
                    record = record + " "; // Add a value delimiter at the end!
                    data.push_back(parseRecord(record));
                    record.clear();
                }
            }
            else
            {
                // Beginning or continuation of a record is getting parsed
                if (!record.empty())
                {
                    record = record + " "; // Add a value delimiter between broken lines!
                }
                record = record + line;
            }
        }

        // Add the last record (end of file will not result in a new line!
        if (!record.empty())
        {
            record = record + " "; // Add a value delimiter at the end!
            data.push_back(parseRecord(record));
            record.clear();
        }

        myFile.close();
    }

    return data;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<struct Passport> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;

        int validPassports1 = 0;
        for (auto passport : data)
        {
            if (passport.isPassportValid1)
            {
                ++validPassports1;
            }
        }

        std::cout << "Valid passports: " << validPassports1 << std::endl;

        std::cout << "Part 2:" << std::endl;

        int validPassports2 = 0;
        for (auto passport : data)
        {
            if (passport.isPassportValid2)
            {
                ++validPassports2;
            }
        }

        std::cout << "Valid passports: " << validPassports2 << std::endl;

    }
}
