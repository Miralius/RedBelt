#include <string>
#include <vector>

using namespace std;

#define CONCAT(x, y) x ## y
#define GET_UNIQ_ID(x, y) CONCAT(x, y)
#define UNIQ_ID GET_UNIQ_ID(var_, __LINE__)

int main() {
    [[maybe_unused]] int UNIQ_ID = 0;
    [[maybe_unused]] string UNIQ_ID = "hello";
    [[maybe_unused]] vector<string> UNIQ_ID = {"hello", "world"};
    [[maybe_unused]] vector<int> UNIQ_ID = {1, 2, 3, 4};
    //int UNIQ_ID = 5; string UNIQ_ID = "hello"; // оба определения на одной строке
}