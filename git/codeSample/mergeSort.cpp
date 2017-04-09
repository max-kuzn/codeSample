#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

template <typename It1, typename It2, typename ItOut>
void merge(It1 begin1, It1 end1, It2 begin2, It2 end2, ItOut Out) {
    while (begin1 != end1 && begin2 != end2)
        if (*begin1 <= *begin2)
            *Out++ = *begin1++;
        else
            *Out++ = *begin2++;
    for (; begin1 != end1; ++begin1)
        *Out++ = *begin1;
    for (; begin2 != end2; ++begin2)
        *Out++ = *begin2;
}

void mergeSort(vector<int>& v, size_t l, size_t r) {
    if (r - l <= 1)
        return;
    size_t mid = (l + r) / 2;
    mergeSort(v, l, mid);
    mergeSort(v, mid, r);
    vector<int> x(r - l);
    merge(v.begin() + l, v.begin() + mid,
        v.begin() + mid, v.begin() + r,
        x.begin());
    for (size_t i = l; i != r; ++i)
        v[i] = x[i - l];
}

void mergeSort(vector<int>& v) {
    mergeSort(v, 0, v.size());
}

int main() {
    size_t n;
    cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i != n; ++i)
        cin >> a[i];
    mergeSort(a);
    for (size_t i = 0; i != n; ++i) {
        cout << a[i];
        if (i != n)
            cout << ' ';
        else
            cout << '\n';
    }
}

