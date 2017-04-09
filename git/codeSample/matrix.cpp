#include <iostream>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

template <typename T>
vector<T> operator /= (vector<T>& v, const T& n) {
    for (size_t i = 0; i != v.size(); ++i)
        v[i] /= n;
    return v;
}

template <typename T>
vector<T> operator * (const vector<T>& v, const T& n) {
    vector<T> res = v;
    for (size_t i = 0; i != res.size(); ++i)
        res[i] *= n;
    return res;
}

template <typename T>
vector<T> operator -= (vector<T>& self, const vector<T>& other) {
    for (size_t i = 0; i != self.size(); ++i)
        self[i] -= other[i];
    return self;
}

template <typename T>
class Matrix {
 private:
    vector<vector<T>> data;

 public:
    explicit Matrix(
        const vector<vector<T>>& d = vector<vector<T>>()): data(d) {
    }

    T& operator ()(size_t i, size_t j) {
        return data[i][j];
    }

    const T& operator ()(size_t i, size_t j) const {
        return data[i][j];
    }

    pair<size_t, size_t> size() const {
        if (data.empty()) {
            return pair<size_t, size_t> (0, 0);
        } else {
            return pair<size_t, size_t> (data.size(), data[0].size());
        }
    }

    template <typename U>
    vector<U> solve(const vector<U>& b) {
        vector<vector<U>> matrix(
            data.size(),
            std::vector<U>(data[0].size()));
        for (size_t i = 0; i != data.size(); ++i) {
            for (size_t j = 0; j != data[0].size(); ++j) {
                matrix[i][j] = static_cast<U>(data[i][j]);
            }
        }
        vector<U> res = b;
        for (size_t i = 0; i != matrix.size(); ++i) {
            size_t j = i;
            while (matrix[i][j] == 0) {
                ++j;
            }
            if (j != i) {
                matrix[i].swap(matrix[j]);
                std::swap(res[i], res[j]);
            }
            res[i] /= matrix[i][i];
            matrix[i] /= matrix[i][i];
            for (size_t j = 0; j != matrix.size(); ++j) {
                if (j != i) {
                    res[j] -= matrix[j][i] * res[i];
                    matrix[j] -= matrix[i] * matrix[j][i];
                }
            }
        }
        return res;
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const Matrix<T>& m) {
    size_t lines = m.size().first,
        columns = m.size().second;
    for (size_t i = 0; i != lines; ++i) {
        for (size_t j = 0; j != columns; ++j) {
            out << m(i, j);
            if (j != columns - 1) {
                out << '\t';
            } else if (i != lines - 1) {
                out << '\n';
            }
        }
    }
    return out;
}

