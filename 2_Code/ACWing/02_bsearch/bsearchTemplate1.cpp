// * 整数二分模版
// * 难点及易错点在于处理边界问题

// * 某种性质检测
bool check(int x);

// * Template 1
int bsearch_1(int l, int r) {
    while(l < r) {
        int mid = (l + r) >> 1;
        if(check(mid)) r = mid; // * 左侧满足性质，右边界收缩
        else l = mid + 1;
    }
    return l;
}

// * Template 2
int bsearch_2(int l, int r) {
    while(l < r) {
        int mid = (l + r + 1) >> 1;
        if(check(mid)) l = mid; // * 右侧满足性质，左边界收缩
        else r = mid - 1;
    }
    return l;
}

/**
 ** 模版的选择，取决于check(mid)的判断
 ** 注意mid要在符合结果的范围内
 */