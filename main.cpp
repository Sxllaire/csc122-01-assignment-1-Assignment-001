//*************************************************************
//
//Danil Alieinikov
//H00880905
//CSC122-01
//
//I certify that this is my work and where appropriate an extension
//of the starter code provided by the assignment
//*************************************************************


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

class MailMerge {
private:
    std::vector<std::string> fieldNames;
    std::vector<std::vector<std::string>> dataRows;
    std::string mergeTemplate;

public:
    void run() {
        std::string dataFileName, mergeFileName;

        std::cout << "Enter the data file name: ";
        std::getline(std::cin, dataFileName);

        std::cout << "Enter to merge file name: ";
        std::getline(std::cin, mergeFileName);

        if (!loadDataFile(dataFileName) || !loadMergeFile(mergeFileName)) {
            std::cerr << "Error reading files.\n";
            return;
        }

        int fileCount = 0;
        for (const auto& row : dataRows) {
            std::string mergedContent = mergeRow(row);
            std::string id = getFieldValue("id", row);
            std::string outputFileName = id + ".txt";
            std::ofstream outFile(outputFileName);
            outFile << mergedContent;
            outFile.close();

            std::cout << outputFileName << " merged\n";
            fileCount++;
        }

        std::cout << "\nMerge Complete.  " << fileCount << " files generated.\n";
    }

private:
    bool loadDataFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;

        // Read header
        if (!std::getline(file, line)) return false;
        parseCSVLine(line, fieldNames);

        // Read data rows
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            parseCSVLine(line, row);
            if (row.size() == fieldNames.size()) {
                dataRows.push_back(row);
            }
        }

        return true;
    }

    bool loadMergeFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::ostringstream oss;
        oss << file.rdbuf();
        mergeTemplate = oss.str();

        return true;
    }

    std::string mergeRow(const std::vector<std::string>& row) {
        std::string result = mergeTemplate;

        for (size_t i = 0; i < fieldNames.size(); ++i) {
            std::string placeholder = "%" + fieldNames[i] + "%";
            size_t pos = 0;
            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.length(), row[i]);
                pos += row[i].length();
            }
        }

        return result;
    }

    std::string getFieldValue(const std::string& field, const std::vector<std::string>& row) {
        for (size_t i = 0; i < fieldNames.size(); ++i) {
            if (fieldNames[i] == field) {
                return row[i];
            }
        }
        return "";
    }

    void parseCSVLine(const std::string& line, std::vector<std::string>& fields) {
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            fields.push_back(trim(item));
        }
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }
};

int main() {
    MailMerge mm;
    mm.run();
    return 0;
}
