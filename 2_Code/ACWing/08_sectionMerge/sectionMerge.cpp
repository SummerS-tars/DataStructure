#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> PII;

const int N = 100010;
vector<PII> segs, res;

int main(){
    int n;
    cin >> n;

    for(int i = 0; i < n; i ++){
        int l, r;
        scanf("%d%d", &l, &r);
        segs.push_back({l, r});
    }

    sort(segs.begin(), segs.end());

    res.push_back(segs[0]);
    for(auto seg : segs){
        if(seg.first > res.back().second){
            res.push_back(seg);
        }
        else if(seg.second > res.back().second){
            res.back().second = seg.second;
        }
    }

    cout << res.size() << endl;
    return 0;
}