#include <iostream>
#include <map>
#include <vector>

using std::map;
using std::vector;

template <typename T>
vector<T> monomial(const T& x, size_t i) {
    vector<T> res(i + 1,
            T());
    res[i] = x;
    return res;
}

template <typename T>
class Polynomial {
 private:
    map<size_t, T> coef;

    void removeZeroes() {
        for (auto it = coef.begin(); it != coef.end();) {
            if (it->second == T()) {
                it = coef.erase(it);
            } else {
                ++it;
            }
        }
    }

 public:
    Polynomial(const T& x = T()) {
        if (x != T())
            coef[0] = x;
    }

    Polynomial(const vector<T>& v) {
        for (size_t i = 0; i != v.size(); ++i)
            if (v[i] != T())
                coef[i] = v[i];
    }

    template <typename It>
    Polynomial(It first, It last) {
        size_t i = 0;
        for (auto it = first; it != last; ++it) {
            if (*it != T())
                coef[i] = *it;
            ++i;
        }
    }

    const map<size_t, T>& getCoef() const {
        return coef;
    }

    T operator[](size_t i) const {
        if (coef.find(i) != coef.end()) {
            return coef.at(i);
        } else {
            return 0;
        }
    }

    int Degree() const {
        if (coef.empty())
            return -1;
        return coef.rbegin()->first;
    }

    size_t size() const {
        return coef.size();
    }

    decltype(coef.cbegin()) begin() const {
        return coef.cbegin();
    }

    decltype(coef.cend()) end() const {
        return coef.cend();
    }

    decltype(coef.crbegin()) rbegin() const {
        return coef.crbegin();
    }

    decltype(coef.crend()) rend() const {
        return coef.crend();
    }

    Polynomial<T>& operator += (const Polynomial<T>& other) {
        for (const auto& kv : other)
            coef[kv.first] += kv.second;
        removeZeroes();
        return *this;
    }

    friend Polynomial<T> operator + (const Polynomial<T>& p) {
        return Polynomial<T>(p.getCoef());
    }

    friend Polynomial<T> operator + (
            const Polynomial<T> left,
            const Polynomial<T> right) {
        auto res = left;
        res += right;
        return res;
    }

    Polynomial<T>& operator -= (const Polynomial<T>& other) {
        for (const auto& kv : other)
            coef[kv.first] -= kv.second;
        removeZeroes();
        return *this;
    }

    friend Polynomial<T> operator - (const Polynomial<T>& p) {
        Polynomial<T> res = p;
        for (auto& kv : res.coef)
            kv.second = -kv.second;
        return res;
    }

    friend Polynomial<T> operator - (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        auto res = left;
        res -= right;
        return res;
    }

    Polynomial<T>& operator *= (const Polynomial<T>& other) {
        if (other.Degree() == -1 || Degree() == -1) {
            *this = Polynomial<T>();
            return *this;
        }
        vector<T> newCoef(
            other.Degree() + Degree() + 1,
            T());
        for (size_t i = 0; i != newCoef.size(); ++i) {
            for (size_t j = 0; j != i + 1; ++j) {
                newCoef[i] += (*this)[j] * other[i - j];
            }
        }
        *this = Polynomial<T>(newCoef);
        removeZeroes();
        return *this;
    }

    friend Polynomial<T> operator * (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        auto res = left;
        res *= right;
        return res;
    }

    friend bool operator == (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        return left.coef == right.coef;
    }

    friend bool operator != (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        return !(left == right);
    }

    T operator()(const T& x) const {
        if (Degree() == -1)
            return 0;
        size_t last_pow = Degree();
        T res = T();
        for (auto it = coef.rbegin(); it != coef.rend(); ++it) {
            for (size_t i = it->first; i != last_pow; ++i)
                res *= x;
            res += it->second;
            last_pow = it->first;
        }
        for (size_t i = 0; i != last_pow; ++i)
            res *= x;
        return res;
    }

    friend Polynomial<T> operator & (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (left.Degree() == -1)
            return 0;
        size_t last_pow = left.Degree();
        Polynomial<T> res;
        for (auto it = left.rbegin(); it != left.rend(); ++it) {
            for (size_t i = it->first; i != last_pow; ++i)
                res *= right;
            res += it->second;
            last_pow = it->first;
        }
        for (size_t i = 0; i != last_pow; ++i)
            res *= right;
        return res;
    }

    friend Polynomial<T> operator / (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (left.Degree() < right.Degree())
            return Polynomial<T>();
        Polynomial<T> divident = left;
        vector<T> res(left.Degree() - right.Degree() + 1);
        while (divident.Degree() >= right.Degree()) {
            res[divident.Degree() - right.Degree()] =
                divident[divident.Degree()] / right[right.Degree()];
            divident -= right * Polynomial<T>(
                    monomial(res[divident.Degree() - right.Degree()],
                        divident.Degree() - right.Degree()));
        }
        return Polynomial<T>(res);
    }

    friend Polynomial<T> operator % (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (left.Degree() < right.Degree())
            return left;
        Polynomial<T> divident = left;
        vector<T> res(left.Degree() - right.Degree() + 1);
        while (divident.Degree() >= right.Degree()) {
            res[divident.Degree() - right.Degree()] =
                divident[divident.Degree()] / right[right.Degree()];
            divident -= right * Polynomial<T>(
                monomial(res[divident.Degree() - right.Degree()],
                    divident.Degree() - right.Degree()));
        }
        return divident;
    }

    friend Polynomial<T> operator , (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (right == 0) {
            return left / left[left.Degree()];
        } else {
            return (right, left % right);
        }
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& p) {
    bool good = false;
    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        size_t pow = it->first;
        T x = it->second;
        if (x == 1) {
            if (pow != p.Degree())
                out << '+';
            if (pow == 0)
                out << '1';
        } else if (x == -1) {
            out << '-';
            if (pow == 0)
                out << '1';
        } else if (x > 0) {
            if (pow != p.Degree())
                out << '+';
            out << x;
        } else if (x < 0) {
            out << x;
        }
        if (x != 0 && x != 1 && x != -1 && pow != 0)
            out << '*';
        if (x != 0 && pow != 0)
            out << 'x';
        if (x != 0 && pow > 1)
            out << '^' << pow;
        if (x != 0)
            good = true;
    }
    if (!good)
        out << '0';
    return out;
}

