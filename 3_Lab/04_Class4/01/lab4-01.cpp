#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

#define wordIter pair<string::iterator, string::iterator>

int numMatchingSubseq(string s, vector<string> &words);

/**
 * time complexity: O(n*m)
 * space complexity: O(m)
 */
// int numMatchingSubseq(string s, vector<string> &words)
// {
//     int cnt = 0;
//     vector<wordIter> iters;
//     for(auto& w : words)
//         iters.push_back({w.begin(), w.end()});
    
//     for(char c : s)
//     {
//         vector<wordIter> changed;

//         for(auto& [cur, end] : iters)
//         {
//             if(cur != end && *cur == c)
//                 cur++;
//             changed.push_back({cur, end});
//         }
//         iters = changed;
//     }

//     for(auto& [cur, end] : iters)
//     {
//         if(cur == end)
//             cnt++;
//     }
//     return cnt;
// }

/**
 * time complexity: O(n + k)
 * the k depends on the patch times of words in s
 * normally k << n * m
 * space complexity: O(m + character set size)
 */
int numMatchingSubseq(string s, vector<string> &words)
{
    unordered_map<char, vector<wordIter>> buckets;
    for(auto& w : words)
        buckets[w[0]].push_back({w.begin(), w.end()});

    int cnt = 0;
    for(char c : s)
    {
        auto curBucket = buckets[c];
        buckets[c].clear(); // 清空当前桶，避免重复处理
        for(auto& [cur, end] : curBucket)
        {
            cur++;
            if(cur == end)
                cnt++;
            else
                buckets[*cur].push_back({cur, end});
        }
    }

    return cnt;
}

int main()
{
    // 测试用例 1: 原始测试
    cout << "=== Test Case 1: Original ===" << endl;
    string s1 = "rwpddkvbnnuglnagtvamxkqtwhqgwbqgfbvgkwyuqkdwhzudsxvjubjgloeofnpjqlkdsqvruvabjrikfwronbrdyyjnakstqjac";
    vector<string> words1 = {"wpddkvbnn", "lnagtva", "kvbnnuglnagtvamxkqtwhqgwbqgfbvgkwyuqkdwhzudsxvju", "rwpddkvbnnugln", "gloeofnpjqlkdsqvruvabjrikfwronbrdyyj", "vbgeinupkvgmgxeaaiuiyojmoqkahwvbpwugdainxciedbdkos", "mspuhbykmmumtveoighlcgpcapzczomshiblnvhjzqjlfkpina", "rgmliajkiknongrofpugfgajedxicdhxinzjakwnifvxwlokip", "fhepktaipapyrbylskxddypwmuuxyoivcewzrdwwlrlhqwzikq", "qatithxifaaiwyszlkgoljzkkweqkjjzvymedvclfxwcezqebx"};
    cout << "Expected: 5, Got: " << numMatchingSubseq(s1, words1) << endl;
    
    // 测试用例 2: 简单情况 - 所有单词都匹配
    cout << "\n=== Test Case 2: All Match ===" << endl;
    string s2 = "abcde";
    vector<string> words2 = {"a", "ab", "abc", "ace", "bde", "abcde"};
    cout << "Expected: 6, Got: " << numMatchingSubseq(s2, words2) << endl;
    cout << "解释: 所有单词都是 s 的子序列" << endl;
    
    // 测试用例 3: 边界情况 - 包含重复单词和无匹配
    cout << "\n=== Test Case 3: Mixed Scenario ===" << endl;
    string s3 = "dsahjpjauf";
    vector<string> words3 = {
        "ahjpjau",    // 匹配
        "ja",         // 匹配
        "ahbwzgqnuk", // 不匹配 (b, w, z, g, n, k 不在 s 中或顺序错误)
        "tnmlanowax", // 不匹配
        "ja",         // 匹配 (重复)
        "d",          // 匹配
        "au"          // 匹配
    };
    cout << "Expected: 5, Got: " << numMatchingSubseq(s3, words3) << endl;
    cout << "匹配的单词: ahjpjau, ja(×2), d, au" << endl;

    return 0;
}
