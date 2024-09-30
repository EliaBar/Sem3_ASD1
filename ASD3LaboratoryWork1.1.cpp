#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>  
#include <iterator>


void CreatTwoFiles(const std::string& fileName);
void MerginOfSeries(const std::string& fileName);
bool NewIteraton(const std::string& fileName);
void splitAndSort(const std::string& fileName, std::vector<int>& randomNumbers);

int main() {
    std::string fileName = "random_numbers.txt";
    const int targetFileSize = 1 * 1024 * 1024;
    int numberOfInts = targetFileSize / 4;
    std::vector<int> randomNumbers(numberOfInts);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < numberOfInts; ++i) {
        randomNumbers[i] = dist(gen);
    }

    splitAndSort(fileName, randomNumbers);
    bool IsSorted = false;
    while (!IsSorted) {
        IsSorted = NewIteraton(fileName);
    }

    return 0;
}

void CreatTwoFiles(const std::string& fileName) {
    std::ifstream FileA(fileName);
    std::ofstream FileB("FileB.txt");
    std::ofstream FileC("FileC.txt");

    if (!FileA) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return;
    }

    int FirstNumber = 0;
    int NextNum = 0;
    FileA >> FirstNumber;
    FileB << FirstNumber << "\t";
    bool series = true;
    while (FileA >> NextNum) {
        if (NextNum < FirstNumber) {
            series = !series;
        }
        if (series) {
            FileB << NextNum << "\t";
        }
        else {
            FileC << NextNum << "\t";
        }
        FirstNumber = NextNum;
    }

    FileA.close();
    FileB.close();
    FileC.close();
}

void MerginOfSeries(const std::string& fileName) {
    std::ofstream FileA(fileName);
    std::ifstream FileB("FileB.txt");
    std::ifstream FileC("FileC.txt");

    int NumFromB = 0;
    int NumFromC = 0;
    bool NumBExist = static_cast<bool>(FileB >> NumFromB);
    bool NumCExist = static_cast<bool>(FileC >> NumFromC);
    bool EndFileB = true;
    bool EndFileC = true;
    while (NumBExist || NumCExist) {
        if (NumBExist && (!NumCExist || NumFromB <= NumFromC)) {
            FileA << NumFromB << "\t";
            int previousB = NumFromB;
            EndFileB = NumBExist = static_cast<bool>(FileB >> NumFromB);
            if (!EndFileC) {
                NumCExist = false;
            }
            else if (previousB > NumFromB && NumCExist) {
                NumBExist = false;
            }
            else {
                NumCExist = true;
            }
        }
        else {
            FileA << NumFromC << "\t";
            int previousC = NumFromC;
            EndFileC = NumCExist = static_cast<bool>(FileC >> NumFromC);
            if (!EndFileB) {
                NumBExist = false;
            }
            else if (previousC > NumFromC && NumBExist) {
                NumCExist = false;
            }
            else {
                NumBExist = true;
            }
        }
    }

    FileA.close();
    FileB.close();
    FileC.close();
}

bool NewIteraton(const std::string& fileName) {
    CreatTwoFiles(fileName);
    MerginOfSeries(fileName);
    std::ifstream outFile(fileName);
    int num1, num2;
    outFile >> num1;
    while (outFile >> num2) {
        if (num2 < num1) {
            outFile.close();
            return false;
        }
        else {
            num1 = num2;
        }
    }
    outFile.close();
    return true;
}

void splitAndSort(const std::string& fileName, std::vector<int>& randomNumbers) {
    const int numOfChunks = 10;
    const int chunkSize = randomNumbers.size() / numOfChunks;

    std::ofstream outFile(fileName);

    if (!outFile) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return;
    }

    for (int i = 0; i < numOfChunks; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = (i == numOfChunks - 1) ? randomNumbers.size() : startIdx + chunkSize;

        std::vector<int> chunk(randomNumbers.begin() + startIdx, randomNumbers.begin() + endIdx);

        std::sort(chunk.begin(), chunk.end());

        for (int num : chunk) {
            outFile << num << "\t";
        }

        std::cout << "Chunk " << i + 1 << " sorted and written to file." << std::endl;
    }

    outFile.close();
}