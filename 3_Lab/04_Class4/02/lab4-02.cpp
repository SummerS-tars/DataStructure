#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

int repeatedStringMatch(const std::string& a, const std::string& b)
{
    /**
     * thought:
     * above is a little complex
     * we can imagine that when the length of b(|b|) is longer than a(|a|)
     * we can can calculate the time = roundup(|b| / |a|)
     * if time = 2, we can image three situation:  
     * 1. b contains in 2 * a
     * 2. b contains in 3 * a
     * 3. b not contains in any times of a
     * the problem is how to check the 3. situation  
     * actually this is easy, as if the 2. not work, the b must be in the 3. situation
     * as the 3 * a is contained in k * a (k > 3)
     * so we just need to check the 1. and 2. situation
     * 
     * so the steps are:  
     * 1. if b contained in time * a
     * 2. if b contained in (time + 1) * a
     * 3. else return -1
     */
    
    // Build KMP next array
    auto buildNext = [](const std::string& pattern) {
        std::vector<size_t> next(pattern.size(), 0);
        for (size_t i = 1, j = 0; i < pattern.size(); ++i) {
            while (j > 0 && pattern[i] != pattern[j]) {
                j = next[j - 1];
            }
            if (pattern[i] == pattern[j]) {
                ++j;
            }
            next[i] = j;
        }
        return next;
    };
    
    // KMP search
    auto kmpSearch = [&](const std::string& text, const std::string& pattern, 
                         const std::vector<size_t>& next) {
        for (size_t i = 0, j = 0; i < text.size(); ++i) {
            while (j > 0 && text[i] != pattern[j]) {
                j = next[j - 1];
            }
            if (text[i] == pattern[j]) {
                ++j;
            }
            if (j == pattern.size()) {
                return true;
            }
        }
        return false;
    };
    
    const size_t minTimes = static_cast<size_t>(std::ceil(static_cast<double>(b.size()) / a.size()));
    const auto next = buildNext(b);
    
    // Check minTimes * a
    std::string repeated;
    repeated.reserve(a.size() * (minTimes + 1));
    for (size_t i = 0; i < minTimes; ++i) {
        repeated += a;
    }
    
    if (kmpSearch(repeated, b, next)) {
        return static_cast<int>(minTimes);
    }
    
    // Check (minTimes + 1) * a
    repeated += a;
    if (kmpSearch(repeated, b, next)) {
        return static_cast<int>(minTimes + 1);
    }
    
    return -1;
}

int main()
{
    const std::string a = "abcd";
    const std::string b = "cdabcdab";
    std::cout << repeatedStringMatch(a, b) << '\n';

    const std::string a2 = "abc";
    const std::string b2 = "wxyz";
    std::cout << repeatedStringMatch(a2, b2) << '\n';

    const std::string a3 = "ahjpjau";
    const std::string b3 = "ja";
    std::cout << repeatedStringMatch(a3, b3) << '\n';

    const std::string a4 = "abcde";
    const std::string b4 = "cdeabc";
    std::cout << repeatedStringMatch(a4, b4) << '\n';

    return 0;
}
