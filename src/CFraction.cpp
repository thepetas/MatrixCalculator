/** \file CFraction.cpp
 *  Implementation of CFraction class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */


#include "CFraction.h"

CFraction::CFraction(int num) : num(num), denom(1) {
    if (num == 0) {
        denom = 0;
        width = 1;
    } else
        Reduce();
}

CFraction::CFraction(int num, int denom) {
    if (denom == 0)
        throw CException("Invalid operation. Null denominator.");
    this->num = num;
    this->denom = denom;
    Reduce();
}

CFraction::CFraction(const string& fract) {
    if (fract.length() == 0)
        throw CException("Invalid format of fraction.");
    num = 0;
    denom = 0;
    bool wasNum = false, wasDenom = false, wasFlash = false, firstNum = false, firstDenom = false, negNum = false, negDenom = false;
    for (unsigned int i = 0; i < fract.length(); i++) {
        if (!wasFlash) {
            if (fract[i] == ' ' &&  (!firstNum || wasNum ));
            else if (fract[i] == '-' && !negNum && !firstNum)
                negNum = true;
            else if (fract[i] <= '9' && fract[i] >= '0' && !wasNum) {
                firstNum = true;
                num *= 10;
                num += fract[i] - 48;
            } else if (fract[i] == ' ' && firstNum)
                wasNum = true;
            else if (fract[i] == '/' && !wasFlash)
                wasFlash = true;
            else
                throw CException("Invalid format of fraction.");
        } else {
            if (fract[i] == ' ' && !firstDenom);
            else if (fract[i] == '-' && !negDenom && !firstDenom)
                negDenom = true;
            else if (fract[i] <= '9' && fract[i] >= '0' && !wasDenom) {
                firstDenom = true;
                denom *= 10;
                denom += fract[i] - 48;
            } else if (fract[i] == ' ' && firstDenom)
                wasDenom = true;
            else
                throw CException("Invalid format of fraction.");
        }
    }


    if (negNum && num != 0)
        num *= -1;
    else if (negNum)
        throw CException("Invalid format of fraction.");

    if (negDenom && denom != 0)
        denom *= -1;
    else if (negDenom)
        throw CException("Invalid format of fraction.");

    if (!wasFlash)
        denom = 1;

    if (wasFlash && !firstDenom)
        throw CException("Invalid format of fraction.");
    
    Reduce();
}

CFraction::~CFraction() {
}

CFraction::CFraction(const CFraction& dec) {
    this->denom = dec.denom;
    this->num = dec.num;
    this->width = dec.width;
}

CFraction& CFraction::operator=(const CFraction& dec) {
    if (this != &dec) {
        this->num = dec.num;
        this->denom = dec.denom;
        this->width = dec.width;
    }
    return *this;
}

ostream & operator<<(ostream& os, const CFraction& dec) {
    ostringstream oss;
    if (dec.num == 0)
        oss << "0";
    else if (dec.denom == 1)
        oss << dec.num;
    else
        oss << dec.num << "/" << dec.denom;

    os << oss.str();
    return os;
}

CFraction CFraction::operator+(const CFraction& dec) const {
    if (this->num == 0) return dec;
    if (dec.num == 0) return *this;

    CFraction res;
    res.denom = LCM(denom, dec.denom);
    res.num = (res.denom / dec.denom) * dec.num + (res.denom / denom) * num;
    res.Reduce();
    return res;
}

CFraction CFraction::operator-(const CFraction& dec) const {
    if (*this == 0 && dec == 0) {
        return CFraction(0);
    }
    if (dec == 0) return *this;
    if (*this == 0) {
        CFraction x(dec);
        x.num *= -1;
        x.Reduce();
        return x;
    }
    CFraction res(0);
    res.denom = LCM(denom, dec.denom);
    res.num = (res.denom / dec.denom * num) - ((res.denom / denom) * dec.num);
    res.Reduce();
    return res;
}

CFraction CFraction::operator*(const CFraction& dec) const {
    CFraction res;
    if (dec == 0 || *this == 0) return CFraction(0);
    int tmp1 = GCD(num, dec.denom), tmp2 = GCD(dec.num, denom);
    res.num = num / tmp1 * dec.num / tmp2;
    res.denom = denom / tmp2 * dec.denom / tmp1;
    res.Reduce();
    return res;
}

CFraction CFraction::operator/(const CFraction& dec) const {
    if (dec.num == 0)
        throw CException("Invalid operation. Divide by zero.");
    return operator*(CFraction(dec.denom, dec.num));
}

CFraction CFraction::operator*(int x) const {
    return *this * CFraction(x);
}

bool CFraction::operator<(const CFraction& dec) const {
    if (this->num == 0) return 0 < dec.num;
    if (dec.num == 0) return this->num < 0;
    if (dec.num == 0 && this->num == 0) return false;
    int lcm = LCM(this->denom, dec.denom);
    return ((lcm / this->denom) * this->num < (lcm / dec.denom) * dec.num);
}

bool CFraction::operator<(int compNum) const {
    return *this < CFraction(compNum);
}

bool CFraction::operator>(int compNum) const {
    return *this > CFraction(compNum);
}

bool CFraction::operator>(const CFraction& dec) const {
    if (this->num == 0) return 0 > dec.num;
    if (dec.num == 0) return this->num > 0;
    if (dec.num == 0 && this->num == 0) return false;
    int lcm = LCM(denom, dec.denom);
    return (this->num * (lcm / this->denom) > dec.num * (lcm / dec.denom));
}

CFraction CFraction::Abs() const {
    CFraction res;
    res.num = abs(num);
    res.denom = abs(denom);
    res.width = width;
    return res;
}

bool CFraction::operator==(int cmp) const {
    if (cmp == 0 && num == 0 && denom == 0) return true;
    else {
        return (num == cmp && denom == 1);
    }
}

bool CFraction::operator!=(int cmp) const {
    return !(*this == cmp);
}

bool CFraction::operator!=(const CFraction & dec) const {
    return !(*this == dec);
}

bool CFraction::operator==(const CFraction& dec) const {
    return (this->denom == dec.denom && this->num == dec.num);
}

int CFraction::findWidth(int number) const {
    int sum = 0;
    int x = number;
    while (x != 0) {
        sum++;
        x /= 10;
    }
    return sum;
}

void CFraction::Reduce() {
    if (denom == 0 && num != 0)
        throw CException("Invalid operation. Null denominator.");
    int gcd = GCD(num, denom);
    num /= gcd;
    denom /= gcd;

    if (denom < 0) {
        denom *= -1;
        num *= -1;
    }

    this->width = 0;
    if (num == 0)
        width = 1;
    else {
        this->width += findWidth(num);
        if (num < 0)
            this->width++;
        if (denom != 1) {
            this->width += findWidth(denom) + 1;
        }
    }
}

int CFraction::GCD(int a, int b) const {
    a = abs(a);
    b = abs(b);
    int tmp = 0;
    do {
        tmp = a % b;
        a = b;
        b = tmp;
    } while (b != 0);
    return a;
}

int CFraction::LCM(int a, int b) const {
    if (a == 0 || b == 0)
        return 0;
    return ( a * b) / GCD(a, b);
}

int CFraction::getWidth() const {
    return width;
}

string CFraction::ToString() const {
    ostringstream oss;
    oss << *this;
    return oss.str();
}
