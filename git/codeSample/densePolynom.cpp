#include <iostream>
#include <vector>

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
    vector<T> coef;

    void removeZeroes() {
        while (!coef.empty() && coef.back() == 0) {
            coef.pop_back();
        }
    }

 public:
    Polynomial(const T& x = T()) {
        coef.push_back(x);
        removeZeroes();
    }

    Polynomial(const vector<T>& v): coef(v) {
        removeZeroes();
    }

    template <typename It>
    Polynomial(It first, It last) {
        coef = vector<T>(first, last);
    }

    const vector<T>& getCoef() const {
        return coef;
    }

    T operator[](size_t i) const {
        if (i < coef.size() && i >= 0) {
            return coef[i];
        } else {
            return 0;
        }
    }

    size_t size() const {
        return coef.size();
    }

    int Degree() const {
        return coef.size() - 1;
    }

    T operator()(const T& x) const {
        T res = T();
        for (size_t i = 0; i != size(); ++i) {
            res *= x;
            res += coef[Degree() - i];
        }
        return res;
    }

    decltype(coef.cbegin()) begin() const {
        return coef.cbegin();
    }

    decltype(coef.cend()) end() const {
        return coef.cend();
    }

    Polynomial<T>& operator += (const Polynomial<T>& other) {
        for (size_t i = 0; i != other.size(); ++i) {
            if (i < coef.size()) {
                coef[i] += other[i];
            } else {
                coef.push_back(other[i]);
            }
        }
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
        for (size_t i = 0; i != other.size(); ++i) {
            if (i < coef.size()) {
                coef[i] -= other[i];
            } else {
                coef.push_back(-other[i]);
            }
        }
        removeZeroes();
        return *this;
    }

    friend Polynomial<T> operator - (const Polynomial<T>& p) {
        Polynomial<T> res = p;
        for (T& x : res.coef)
            x = -x;
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
        if (other.size() == 0 || coef.size() == 0) {
            *this = Polynomial<T>();
            return *this;
        }
        vector<T> newCoef(
            other.size() + coef.size() - 1,
            T());
        for (size_t i = 0; i != newCoef.size(); ++i) {
            for (size_t j = 0; j != i + 1; ++j) {
                newCoef[i] += (*this)[j] * other[i - j];
            }
        }
        coef = newCoef;
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

    friend Polynomial<T> operator & (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        Polynomial<Polynomial<T>> tmp(left.begin(), left.end());
        Polynomial<T> res = tmp(right);
        return res;
    }

    friend Polynomial<T> operator / (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (left.Degree() < right.Degree())
            return Polynomial<T>();
        Polynomial<T> x = left;
        vector<T> res(left.Degree() - right.Degree() + 1);
        while (x.Degree() >= right.Degree()) {
            res[x.Degree() - right.Degree()] =
                x[x.Degree()] / right[right.Degree()];
            x -= right * Polynomial<T>(
                monomial(res[x.Degree() - right.Degree()],
                    x.Degree() - right.Degree()));
        }
        return Polynomial<T>(res);
    }

    friend Polynomial<T> operator % (
            const Polynomial<T>& left,
            const Polynomial<T>& right) {
        if (left.Degree() < right.Degree())
            return left;
        Polynomial<T> x = left;
        vector<T> res(left.Degree() - right.Degree() + 1);
        while (x.Degree() >= right.Degree()) {
            res[x.Degree() - right.Degree()] =
                x[x.Degree()] / right[right.Degree()];
            x -= right * Polynomial<T>(
                monomial(res[x.Degree() - right.Degree()],
                    x.Degree() - right.Degree()));
        }
        return x;
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
    for (size_t j = 0; j != p.size(); ++j) {
        size_t i = p.Degree() - j;
        if (p[i] == 1) {
            if (i != p.Degree())
                out << '+';
            if (i == 0)
                out << '1';
        } else if (p[i] == -1) {
            out << '-';
            if (i == 0)
                out << '1';
        } else if (p[i] > 0) {
            if (i != p.Degree())
                out << '+';
            out << p[i];
        } else if (p[i] < 0) {
            out << p[i];
        }
        if (p[i] != 0 && p[i] != 1 && p[i] != -1 && i != 0)
            out << '*';
        if (p[i] != 0 && i != 0)
            out << 'x';
        if (p[i] != 0 && i > 1)
            out << '^' << i;
        if (p[i] != 0)
            good = true;
    }
    if (!good)
        out << '0';
    return out;
}

