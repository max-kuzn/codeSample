#include <cmath>

int gcd(int a, int b) {
    return (b > 0) ? gcd(b, a % b) : a;
}

class Rational{
 private:
    int num, den;

 public:
    int numerator() const {
        return num;
    }

    int denominator() const {
        return den;
    }

    void reduce() {
        int gcdNumDen = gcd(std::abs(num), den);
        num /= gcdNumDen;
        den /= gcdNumDen;
    }

    Rational(int n = 0, int d = 1) {
        if (d > 0) {
            num = n;
            den = d;
        } else {
            num = -n;
            den = -d;
        }
        reduce();
    }

    Rational& operator++() {
        num += den;
        reduce();
        return *this;
    }

    Rational operator++(int) {
        Rational oldValue(num, den);
        num += den;
        reduce();
        return oldValue;
    }

    Rational& operator--() {
        num -= den;
        reduce();
        return *this;
    }

    Rational operator--(int) {
        Rational oldValue(num, den);
        num -= den;
        reduce();
        return oldValue;
    }

    Rational operator += (const Rational& other) {
        num = num * other.denominator() + other.numerator() * den;
        den *= other.denominator();
        reduce();
        return *this;
    }

    Rational operator -= (const Rational& other) {
        num = num * other.denominator() - other.numerator() * den;
        den *= other.denominator();
        reduce();
        return *this;
    }

    Rational operator *= (const Rational& other) {
        num *= other.numerator();
        den *= other.denominator();
        reduce();
        return *this;
    }

    Rational operator /= (const Rational& other) {
        *this *= Rational(other.denominator(), other.numerator());
        return *this;
    }
};

Rational operator + (const Rational& r) {
    return Rational(r.numerator(), r.denominator());
}

Rational operator - (const Rational& r) {
    return Rational(-r.numerator(), r.denominator());
}

Rational operator + (const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp += b;
    return tmp;
}

Rational operator - (const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp -= b;
    return tmp;
}

Rational operator * (const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp *= b;
    return tmp;
}

Rational operator / (const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp /= b;
    return tmp;
}

bool operator == (const Rational& a, const Rational& b) {
    return a.numerator() == b.numerator()
        && a.denominator() == b.denominator();
}

bool operator != (const Rational& a, const Rational& b) {
    return !(a == b);
}

