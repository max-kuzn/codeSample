#include <iostream>
#include <vector>
#include <ctime>

using std::vector;
using std::pair;
using std::cout;

template <typename It, typename Func>
It partition(It begin, It end, Func f) {
    while (begin != end) {
        if (f(*begin)) {
            ++begin;
        } else {
            auto x = *--end;
            *end = *begin;
            *begin = x;
        }
    }
    return begin;
}

template <typename T>
pair<size_t, size_t> partition(vector<T>& v, size_t l, size_t r) {
    T p = v[l + rand() % (r - l)];
    auto it2 = partition(v.begin() + l, v.begin() + r, [p](T x) {
        return x <= p;
    });
    auto it1 = partition(v.begin() + l, it2, [p](T x) {
        return x < p;
    });
    return pair<size_t, size_t>(it1 - v.begin(), it2 - v.begin());
}

template <typename T>
void quickSort(vector<T>& v, size_t l, size_t r) {
    while (l + 1 < r) {
        auto p = partition(v, l, r);
        if (p.first + p.second > r + l) {  // p.first - l < r - p.second
            quickSort(v, l, p.first);
            l = p.second;
        } else {
            quickSort(v, p.second, r);
            r = p.first;
        }
    }
}

template <typename T>
void quickSort(vector<T>& v) {
    quickSort(v, 0, v.size());
}

int main() {
    size_t n = 0;
    std::cin >> n;
    std::vector<int> v(n);
    for (size_t i = 0; i != n; ++i)
        std::cin >> v[i];
    quickSort(v);
    for (size_t i = 0; i != n; ++i) {
        std::cout << v[i];
        if (i != n)
            std::cout << ' ';
        else
            std::cout << '\n';
    }
}

