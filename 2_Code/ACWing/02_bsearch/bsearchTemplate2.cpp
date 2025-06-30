// * 浮点数二分模版
// * 很简单，因为不用处理边界问题
// * 不过判断搜寻结束的条件需要改变，根据题目所需的精度进行选择
// * 一般限制搜寻区间范围在题干要求再多2位精度即可

// * 某种性质检测
bool check(double x);

// * Template 1
int bsearch_float(double l, double r) {
    while(r - l > 1e-8) {
        double mid = (l + r) / 2;
        if(check(mid)) r = mid; // * 左侧满足性质，右边界收缩
        else l = mid;
    }
    return l;
}