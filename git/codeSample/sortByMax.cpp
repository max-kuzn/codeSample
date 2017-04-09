#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

template <typename It>
void changeMax(It begin, It end) {
    It max = begin;
    for (It i = begin; i != end; ++i)
        if (*i > *max)
            max = i;
    if (max != end && max != --end) {
        auto x = *max;
        *max = *end;
        *end = x;
    }
}

template <typename It>
void sortByMax(It begin, It end) {
    while (end != begin)
        changeMax(begin, end--);
}

int main() {
    size_t n;
    cin >> n;
    vector<int> v(n);
    for (size_t i = 0; i != n; ++i)
        cin >> v[i];
    sortByMax(v.begin(), v.end());
    for (size_t i = 0; i != n; ++i) {
        cout << v[i];
        if (i != n - 1)
            cout << ' ';
        else
            cout << '\n';
    }
}

