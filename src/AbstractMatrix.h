/** \file AbstractMatrix.h
 *  Header file for AbstractMatrix.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef ABSTRACTMATRIX_H
#define	ABSTRACTMATRIX_H

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>

#include "CFraction.h"
using namespace std;

/// This class is abstract. It's a parent of CDiagonalMatrix, CMatrix and CSparseMatrix

class AbstractMatrix {
public:
    /// Constructor 
    /** \param rows Rows of matrix
     *  \param columns Columns of matrix
     *  \param name Name of matrix */
    AbstractMatrix(int rows, int columns, const string & name);
    /// Destructor
    virtual ~AbstractMatrix();
    /// Gets number of rows
    /** \return Number of rows */
    int getRows() const;
    /// Gets number of columns
    /** \return Number of columns */
    int getColumns() const;
    /// Gets name of matrix
    /** \return Text with name of class */
    string getName() const;
    /// Gets name of class
    /** \return Class name */
    virtual string getNameOfType() const = 0;
    /// Sets name of matrix
    /** \param name New name of matrix */
    void setName(const string & name);
    /// Make a clone of AbstractMatrix*
    /** \return new AbstractMatrix* */
    virtual AbstractMatrix * clone() const = 0;
    /// Gets a fraction at position in a matrix
    /** \param x Row x
     *  \param y Column y
     *  \return Fraction at [x,y] */
    virtual CFraction atPosition(int x, int y) const = 0;
    /// Gets determinant of matrix
    /** \return Determinant (fraction) */
    CFraction determinant() const;
    /// Put matrix with information into ostream
    /** \param os Ostream, in which will be put matrix
     *  \param mat Matrix for putting*/
    friend ostream & operator<<(ostream & os, const AbstractMatrix * mat);
    /// Gets an inversion matrix
    AbstractMatrix * getInverseMatrix() const;
    /// Sum of matrix and second matrix
    /** \param mat Second matrix
     *  \return Sum of 2 matrices */
    AbstractMatrix * operator+(const AbstractMatrix * mat) const;
    /// Difference of matrix and second matrix
    /** \param mat Second matrix
     *  \return Sum of 2 matrices */
    AbstractMatrix * operator-(const AbstractMatrix * mat) const;
    /// Multiple of matrix and second matrix
    /** \param mat Second matrix
     *  \return Sum of 2 matrices */
    AbstractMatrix * operator*(const AbstractMatrix * mat) const;
    /// Quotient of matrix and fraction
    /** \param fract Fraction divider
     *  \return Sum of 2 matrices */
    AbstractMatrix * operator/(const CFraction & fract) const;
    /// Multiple of fraction and matrix
    /** \param fract Fraction 
     *  \param mat Matrix which will be multiplied 
     *  \return AbstractMatrix* which is multiplied by fraction */
    friend AbstractMatrix * operator*(const CFraction & fract, const AbstractMatrix & mat);
    /// Gets map with positions and nonnull fractions
    /** \return Map, key is position and second is nonnull fraction */
    virtual map<pair<int, int>, CFraction > getMap() const = 0;
    /// Change matrix to Gaussian elimination form
    /** \param os Ostream to put 
     *  \param isPrint True - print steps, false - no print 
     *  \return Fraction if was move 2 lines - fraction *= -1 , if line was multiplied by num - fraction *= num */
    virtual CFraction gaussianElimination(ostream & os, bool isPrint) = 0;
    /// Gets rank of matrix
    /** \return Rank of matrix */
    virtual int rank() const = 0;
    /// Gets transpose matrix of this matrix
    /** \return Transpose AbstractMatrix* of this matrix */
    AbstractMatrix * getTranspose() const;
    /// Gets format of matrix to save into file
    /** \return Text format for save */
    string fileFormat() const;
    static string NAMECMATRIX; ///< name of CMatrix type
    static string NAMECDIAGONALMATRIX; ///< name of CDiagonalMatrix type
    static string NAMECSPARSEMATRIX; ///< name of CSparseMatrix type
    static string INVALIDCHARACTERS; ///< Characters which can't be in name of matrix
protected:
    /// Put text representation of a matrix's body into a ostream
    /** \param os Ostream for put matrix */
    void print(ostream & os) const;
    /// Change matrix to Jordan elimination form
    virtual void gaussJordanElimination() = 0;
    /// Gets width of each columns in matrix before print
    /** \return Array with width for each column */
    virtual int * refreshWidth() const = 0;
    string name; ///< name of matrix
    int rows; ///< number of rows
    int columns; ///< number of columns
};

#endif	/* ABSTRACTMATRIX_H */

