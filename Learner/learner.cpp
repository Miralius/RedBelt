#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    size_t oldSize = dict.size();
    copy(words.cbegin(), words.cend(), inserter(dict, dict.begin()));
    return static_cast<int>(dict.size() - oldSize);
  }

  vector<string> KnownWords() {
    return { dict.cbegin(), dict.cend() };
  }
};

int main() {
    Learner learner;
    string line;
    while (getline(cin, line)) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        cout << learner.Learn(words) << "\n";
    }
    cout << "=== known words ===\n";
    for (const auto& word : learner.KnownWords()) {
        cout << word << "\n";
    }
}