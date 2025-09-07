#include <algorithm>
#include <format>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::ranges::reverse(v);
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << std::format("reversed sum = {}\n", sum);
    return 0;
}