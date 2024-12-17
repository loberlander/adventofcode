#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiter0 = " ";
static const std::string delimiterCmd = "$ ";
static const std::string delimiterCmdCD = "cd ";
static const std::string delimiterCmdLS = "ls";
static const std::string folderSeparator = "/";
static const std::string delimiterDIR = "dir ";

enum Commands
{
    Undef,
    CD,
    LS,
};

class Element
{
public:
    Element(std::string path, int fileSize)
        : path_(path)
        , fileSize_(fileSize)
    {
    }

    std::string path_;
    int fileSize_;
};

class FileSystem
{
public:
    FileSystem()
        : lastCommand_(Undef)
        , currentPath_(folderSeparator)
    {
    }

    void digest(std::string line)
    {
        size_t tokenPosition = 0;
        tokenPosition = line.find(delimiterCmd, 0);
        if (tokenPosition != std::string::npos)
        {
            line.erase(0, tokenPosition + delimiterCmd.length());

            tokenPosition = line.find(delimiterCmdCD, 0);
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiterCmdCD.length());
                lastCommand_ = CD;
            }
            else
            {
                tokenPosition = line.find(delimiterCmdLS, 0);
                if (tokenPosition != std::string::npos)
                {
                    line.erase(0, tokenPosition + delimiterCmdLS.length());
                    lastCommand_ = LS;
                }
            }
        }

        switch(lastCommand_)
        {
            case CD:
            {
                if (line == "..")
                {
                    size_t lastSlashPosition = currentPath_.find_last_of(folderSeparator);
                    if(lastSlashPosition != std::string::npos)
                    {
                        currentPath_.erase(lastSlashPosition, std::string::npos);
                    }
                }
                else
                {
                    if (line == "/")
                    {
                        currentPath_ = line;
                    }
                    else
                    {
                        if (currentPath_ == "/")
                        {
                            currentPath_ += line;
                        }
                        else
                        {
                            currentPath_ = currentPath_ + "/" + line;
                        }
                    }
                }
                break;
            }
            case LS:
            {
                if (line.length() > 0 )
                {
                    tokenPosition = line.find(delimiterDIR, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        // Ignore
                    }
                    else
                    {
                        tokenPosition = line.find(delimiter0, 0);
                        std::string fileSize = line.substr(0, tokenPosition);
                        line.erase(0, tokenPosition + delimiter0.length());
                        int fSize = std::stoi(fileSize);
                        std::string fullPath;
                        if (currentPath_ == "/")
                        {
                            fullPath = currentPath_ + line;
                        }
                        else
                        {
                            fullPath = currentPath_ + "/" + line;
                        }
                        // Add a file
                        fs_.emplace(std::make_pair(fullPath, Element(fullPath, fSize)));

                        std::string folder = currentPath_;

                        while (folder.length() > 0)
                        {
                            auto it = fsDirSizes_.find(folder);
                            int folderSize = fSize;

                            if (it != fsDirSizes_.end())
                            {
                                // already exists
                                folderSize += it->second;
                                fsDirSizes_.erase(it);
                            }
                            fsDirSizes_.emplace(std::make_pair(folder, folderSize));

                            if (folder == "/")
                            {
                                // We are done
                                break;
                            }

                            size_t lastSlashPosition = folder.find_last_of(folderSeparator);
                            if (lastSlashPosition != std::string::npos)
                            {
                                folder.erase(lastSlashPosition, std::string::npos);
                            }

                            if (folder.length() == 0)
                            {
                                folder = "/";
                            }
                        }
                    }
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

    std::map<std::string, Element> fs_;
    std::map<std::string, int> fsDirSizes_;
    std::string currentPath_;
    char lastCommand_;
};

void readInputFile(std::string fileName, FileSystem& filesystem)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                filesystem.digest(line);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    FileSystem fileSystem;

    readInputFile(inputFileName, fileSystem);

    if (fileSystem.fs_.size() == 0)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int total = 0;
        for (auto& folder : fileSystem.fsDirSizes_)
        {
            if (folder.second <= 100000)
            {
                total += folder.second;
            }
        }
        std::cout << "total: " << total;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        int totalUsed = fileSystem.fsDirSizes_["/"];
        int available = 70000000 - totalUsed;
        int minimumNeeded = 30000000 - available;
        int minimumFound = 30000000;
        for (auto& folder : fileSystem.fsDirSizes_)
        {
            if (folder.second >= minimumNeeded && folder.second < minimumFound)
            {
                minimumFound = folder.second;
            }
        }
        std::cout << "minimum found: " << minimumFound;
        std::cout << std::endl;

    }
}
