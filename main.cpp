#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using std::string;
using std::vector;
using namespace std::literals;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

void printFile(const vector<string> &data) {
  for (const auto &it : data) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
}

class testFraud {
public:
  void readFile() {
    std::ifstream file("fraudTest.csv");

    if (!file.is_open()) {
        std::cerr << "Error: File does not exist." << std::endl;
        return;
    }
    
    string line;

    std::getline(file, line);

    int row;
    while (std::getline(file, line) && data.size() <= 500000) {
      std::istringstream iss(line);
      string col;
      vector<string> row_data;

      int col_count = 0;
      while (std::getline(iss, col, ',') && col_count < 4) {
        row_data.push_back(col);
        col_count++;
      }

      if (col_count == 4) {
        row = std::stoi(row_data[0]);
        row_data.erase(row_data.begin());
        data.insert(std::make_pair(row, std::move(row_data)));
      }
    }
  }

  int keySearch(const int &row) {
    auto it = data.find(row);
    if (it != data.end() && it->first == row) {
      std::cout << it->first << " ";
      printFile(it->second);
      return it->first;
    }
    std::cout << "No results found." << std::endl;
    return 0;
  }

  void keyDelete(const int &key) {
    auto it = data.find(key);
    if (it != data.end() && it->first == key) {
      std::cout << "Entry with id " << it->first << " erased. " << std::endl;
      data.erase(it);
      return;
    }
    std::cout << "No entries found." << std::endl;
  }

  void valueSearch(const string &value) {
    vector<int> found;
    for (auto const &it : data) {
      if (std::find(it.second.begin(), it.second.end(), value) !=
          it.second.end()) {
        found.push_back(it.first);
      }
    }
    if (found.empty()) {
      std::cout << "No entries found." << std::endl;
      return;
    }
    std::cout << found.size() << " entries found: " << std::endl;
    for (int key : found) {
      keySearch(key);
    }
  }

  void valueDelete(const string &value) {
    vector<int> to_be_deleted;
    for (auto const &it : data) {
      if (std::find(it.second.begin(), it.second.end(), value) !=
          it.second.end()) {
        to_be_deleted.push_back(it.first);
      }
    }
    if (to_be_deleted.empty()) {
      std::cout << "No entries found" << std::endl;
      return;
    }
    std::cout << to_be_deleted.size() << " entries erased: " << std::endl;
    for (int key : to_be_deleted) {
      keySearch(key);
      data.erase(key);
    }
  }

  void topValues(const int n = 1) {
    int count = 0;
    for (auto it = data.rbegin(); it != data.rend() && count < n;
         ++it, ++count) {
      std::cout << it->first << " ";
      printFile(it->second);
    }
  }

  void minValues(const int n = 1) {
    int count = 0;
    for (auto it = data.begin(); it != data.end() && count < n; ++it, ++count) {
      std::cout << it->first << " ";
      printFile(it->second);
    }
  }

  void nKeySearch(const vector<int> &keys) {
    for (int key : keys) {
      keySearch(key);
    }
  }

  void nKeyDelete(const vector<int> &keys) {
    for (int key : keys) {
      keyDelete(key);
    }
  }

  void nValueSearch(const string &value, const int &n) {
    vector<int> found;
    for (const auto &entry : data) {
      const auto &row_data = entry.second;
      if (std::find(row_data.begin(), row_data.end(), value) !=
          row_data.end()) {
        found.push_back(entry.first);
        if (found.size() == n)
          break;
      }
    }

    if (found.empty()) {
      std::cout << "No entries found" << std::endl;
      return;
    }

    std::cout << found.size() << " entries found: " << std::endl;
    for (int key : found) {
      keySearch(key);
    }
  }

  void nValueDelete(const string &value, const int &n) {
    vector<int> to_be_deleted;
    for (const auto &entry : data) {
      const auto &row_data = entry.second;
      if (std::find(row_data.begin(), row_data.end(), value) !=
          row_data.end()) {
        to_be_deleted.push_back(entry.first);
        if (to_be_deleted.size() == n)
          break;
      }
    }

    if (to_be_deleted.empty()) {
      std::cout << "No entries found" << std::endl;
      return;
    }

    std::cout << to_be_deleted.size() << " entries erased: " << std::endl;
    for (int key : to_be_deleted) {
      keySearch(key);
      data.erase(key);
    }
  }

  std::map<int, vector<string>> data;
};

void timer()
{
    testFraud fraud;
    auto startReadFile = high_resolution_clock::now();
    fraud.readFile();
    auto endReadFile = high_resolution_clock::now();
    auto durationReadFile = duration_cast<microseconds>(endReadFile - startReadFile);
    std::cout << "Amount of time(microseconds) taken to execute readFile is " << durationReadFile.count() << std::endl;

    auto startTopValues = high_resolution_clock::now();
    fraud.topValues(10);
    auto endTopValues = high_resolution_clock::now();
    auto durationTopValues = duration_cast<microseconds>(endTopValues - startTopValues);
    std::cout << "Amount of time(microseconds) taken to execute topValues is " << durationTopValues.count() << std::endl;

    auto startMinValues = high_resolution_clock::now();
    fraud.minValues(10);
    auto endMinValues = high_resolution_clock::now();
    auto durationMinValues = duration_cast<microseconds>(endMinValues - startMinValues);
    std::cout << "Amount of time(microseconds) taken to execute minValues is " << durationMinValues.count() << std::endl;

    auto startKeySearch = high_resolution_clock::now();
    fraud.keySearch(1);
    auto endKeySearch = high_resolution_clock::now();
    auto durationKeySearch = duration_cast<microseconds>(endKeySearch - startKeySearch);
    std::cout << "Amount of time(microseconds) taken to execute keySearch is " << durationKeySearch.count() << std::endl;

    auto startValueSearch = high_resolution_clock::now();
    fraud.valueSearch("fraud_Tillman LLC");
    auto endValueSearch = high_resolution_clock::now();
    auto durationValueSearch = duration_cast<microseconds>(endValueSearch - startValueSearch);
    std::cout << "Amount of time(microseconds) taken to execute valueSearch is " << durationValueSearch.count() << std::endl;

    auto startKeyDelete = high_resolution_clock::now();
    fraud.keyDelete(10);
    auto endKeyDelete = high_resolution_clock::now();
    auto durationKeyDelete = duration_cast<microseconds>(endKeyDelete - startKeyDelete);
    std::cout << "Amount of time(microseconds) taken to execute keyDelete is " << durationKeyDelete.count() << std::endl;

    auto startValueDelete = high_resolution_clock::now();
    fraud.valueDelete("fraud_Reichel LLC");
    auto endValueDelete = high_resolution_clock::now();
    auto durationValueDelete = duration_cast<microseconds>(endValueDelete - startValueDelete);
    std::cout << "Amount of time(microseconds) taken to execute valueDelete is " << durationValueDelete.count() << std::endl;

    auto startNValueSearch = high_resolution_clock::now();
    fraud.nValueSearch("fraud_Tillman LLC", 5);
    auto endNValueSearch = high_resolution_clock::now();
    auto durationNValueSearch = duration_cast<microseconds>(endNValueSearch - startNValueSearch);
    std::cout << "Amount of time(microseconds) taken to execute nValueSearch is " << durationNValueSearch.count() << std::endl;

    auto startNKeySearch = high_resolution_clock::now();
    fraud.nKeySearch({10, 20, 30});
    auto endNKeySearch = high_resolution_clock::now();
    auto durationNKeySearch = duration_cast<microseconds>(endNKeySearch - startNKeySearch);
    std::cout << "Amount of time(microseconds) taken to execute nKeySearch is " << durationNKeySearch.count() << std::endl;

    auto startNValueDelete = high_resolution_clock::now();
    fraud.nValueDelete("fraud_Tillman LLC", 5);
    auto endNValueDelete = high_resolution_clock::now();
    auto durationNValueDelete = duration_cast<microseconds>(endNValueDelete - startNValueDelete);
    std::cout << "Amount of time(microseconds) taken to execute nValueDelete is " << durationNValueDelete.count() << std::endl;

    auto startNKeyDelete = high_resolution_clock::now();
    fraud.nKeyDelete({10, 20, 30});
    auto endNKeyDelete = high_resolution_clock::now();
    auto durationNKeyDelete = duration_cast<microseconds>(endNKeyDelete - startNKeyDelete);
    std::cout << "Amount of time(microseconds) taken to execute nKeyDelete is " << durationNKeyDelete.count() << std::endl;
}

int main()
{
    testFraud fraud;
    timer();

    std::getchar();
    return 0;
}