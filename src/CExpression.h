/** \file CExpression.h
 *  Header file for CExpression.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef CEXPRESSION_H
#define	CEXPRESSION_H

#include "AbstractMatrix.h"
#include "CException.h"

#include <string>
#include <vector>
#include <iostream>
using namespace std;

/// Class for calculating expression

class CExpression {
private:
    /// Class for representation matrix or fraction in array

    class CItem {
    public:
        /// Default constructor
        CItem() : isMatrix(false) {
        };
        /// Constructor which set matrix
        /** \param mat Matrix for insert */
        CItem(const AbstractMatrix * mat) {
            isMatrix = true;
            matrix = mat->clone();
        }
        /// Constructor which set fraction
        /** \param fract Fraction for insert */
        CItem(const CFraction & fract) {
            isMatrix = false;
            this->fract = fract;
            matrix = NULL;
        }
        /// Copy constructor
        /** \param it CItem to copy into this item */
        CItem(const CItem & it) {
            isMatrix = it.isMatrix;
            if (it.isMatrix) {
                matrix = it.matrix->clone();
            } else {
                matrix = NULL;
                fract = it.fract;
            }
        }
        /// Save it to this item
        /** \param it Item which will be set into this */
        CItem operator=(const CItem & it) {
            if (this->isMatrix)
                delete matrix;
            isMatrix = it.isMatrix;
            if (it.isMatrix) {
                matrix = it.matrix->clone();
            } else {
                matrix = NULL;
                fract = it.fract;
            }
            return *this;
        }
        /// Destructor
        ~CItem() {
            if (isMatrix) {
                delete matrix;
            }
        }
        bool isMatrix; ///< If in item: matrix - true, fraction - false 
        AbstractMatrix * matrix; ///< Matrix
        CFraction fract; ///< Fraction
    };
public:
    /// Constructor
    /** \param expr Expression which will be calculated 
     *  \param matrices Map with matrices, source of matrices in expression */
    CExpression(const string & expr, const map<string, AbstractMatrix* > & matrices);
    /// Destructor
    ~CExpression();
    /// Solve expression
    void solve();
    /// Show result into standart output
    void showResult() const;
    /// Gets result
    CItem getResult() const;
private:
    /// Erase 2 operands at index \em i and \em i \em + \em 1, erase operators at index \em i
    void erase(unsigned int i);
    /// Insert fraction or matrix into array with operands
    /** \param data All expression
     *  \param matrices Source of matrices in expression
     *  \param left Current left position in string - start of operands 
     *  \param middle Current middle position in string - first char of operands
     *  \param right Current right position in string - end of operands (or operators) */
    void insertItem(const string & data, const map<string, AbstractMatrix*>& matrices, size_t left, size_t & middle, size_t & right);
    /// Insert fraction into array with operands
    /** \param name Text with fraction
     *  \param oper Operators after fraction, 'X' = no next operator */
    void insertFraction(const string & name, char oper);
    /// Read matrix from console
    /** \param data All expression
     *  \param left Current left position in string
     *  \param middle Current middle position in string - start of matrix
     *  \param right Current right position in string */
    void readInsertMatrix(const string data, size_t left, size_t & middle, size_t & right);
    vector<char> operators; ///< operators from expression
    vector<CItem> operands; ///< operands from expression
    static string VALID_OPERATORS; ///< Valid operators
    static string INVERSEMATRIX; ///< TAG[...] - make inverse matrix
    static string TRANSPOSEMATRIX; ///< TAG[...] - make transpose matrix
    static string DETERMINANT; ///< TAG[...] - make determinant of matrix
    static string RANK; ///< TAG[...] - make rank of matrix
    int cntConsoleMatrix; ///< Counter of matrices which were added in console
};

#endif	/* CEXPRESSION_H */

