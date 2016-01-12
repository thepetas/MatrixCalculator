/** \file CFraction.h
 *  Header file for CFraction.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef FRACTION_H
#define	FRACTION_H

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "CException.h"

/// Class for representation fraction
class CFraction {
public:
    /// Implicit constructor
    /** \param num Value of fraction */
    CFraction(int num = 0);
    /// Constructor makes fraction from string
    /** \param fract Text format of fraction */
    CFraction(const string & fract);
    /// Constructor with numerator and denominator
    /** \param num Numerator of fraction
     *  \param denom Denominator of fraction */
    CFraction(int num, int denom);
    /// Copy constructor
    /** \param fract Fraction for copying */
    CFraction(const CFraction & fract);
    /// Destructor
    ~CFraction();
    /// Put formated fraction into ostream
    /** \param os Ostream for print
     *  \param fract Fraction for printing*/
    friend ostream & operator<<(ostream & os, const CFraction & frac);
    /// operator for copying fraction
    /** \param frac Fraction for copying into this fraction 
     *  \return Fraction*/
    CFraction & operator=(const CFraction & frac);
    /// Gets absolute value of fraction
    /**  \return Fraction in absolute form */
    CFraction Abs() const;
    /// Gets width of text representation of fraction
    /** \return Width of texted fraction*/
    int getWidth() const;
    /// Compares fractions if not equal 
    /** \param fract Fraction for comparing 
    *  \return true - not equal, false - equal */
    bool operator!=(const CFraction & fract) const;
    /// Compares fractions if equal
    /** \param fract Fraction for comparing 
     *  \return True - equal, false - not equal*/
    bool operator==(const CFraction & fract) const;
    /// Compares fractions if this is less than fract
    /** \param fract Fraction for comparing 
     *  \return True - this is less, false - fract is less or equal */
    bool operator<(const CFraction & fract) const;
    /// Compares fractions if this is greater than fract
    /** \param fract Fraction for comparing 
     *  \return True - this is greater, false = fract is less or equal*/
    bool operator>(const CFraction & fract) const;
    /// Sums two fractions
    /** \param fract Fraction for sum with this 
     *  \return Sum of two fractions */
    CFraction operator+(const CFraction & fract) const;
    /// Substracts two fractions
    /** \param fract Fraction for substract with this
     *  \return Substract of two fractions */
    CFraction operator-(const CFraction & fract) const;
    /// Multiples two fractions
    /** \param fract Fraction multiplier
     *  \return Multiple of two fractions */
    CFraction operator*(const CFraction & fract) const;
    /// Multiples fraction with number
    /** \param x Number multiplier
     *  \return Multiple of fraction and number */
    CFraction operator*(int x) const;
    /// Divides two fractions
    /** \param fract Fraction divider
     *  \return Quotient of two fractions */
    CFraction operator/(const CFraction & fract) const;
    /// Text representation of fraction
    /** \return Fraction in text */
    string ToString() const;
    /// Compares fraction if is equal number
    /** \param cmp Number for compare
     *  \return True - equal, false - not equal */
    bool operator==(int cmp) const;
    /// Compares fraction if is not equal number
    /** \param cmp Number for compare
     *  \return True - not equal, false - equal */
    bool operator!=(int cmp) const;
    /// Compares fraction if is less than number
    /** \param cmp Number for compare
     *  \return True - this is less, false - number is greater of equal  */
    bool operator<(int cmp) const;
    /// Compares fraction if is greater than number
    /** \param cmp Number for compare
     *  \return True - this is greater, false - number is less or equal */
    bool operator>(int x) const;
private:
    /// Greatest common divisor of 2 numbers
    /** \param a Number no. 1
     *  \param b Number no. 2
     *  \return GCD of no.1 and no.2 */
    int GCD(int a, int b) const;
    /// Least common multiple of 2 numbers
    /** \param a Number no. 1
     *  \param b Number no. 2
     *  \return LCM of no.1 and no.2 */
    int LCM(int a, int b) const;
    /// Reduce numerator and denominator
    void Reduce();
    /// Find width of number
    /** \param num Number
     *  \return Width of number*/
    int findWidth(int num) const;
    int num; ///< Numerator
    int denom; ///< Denominator
    int width; ///< Width of text representation
};

#endif	/* FRACTION_H */