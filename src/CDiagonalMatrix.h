/** \file CDiagonalMatrix.h
 *  Header file for CDiagonalMatrix.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef CIDENTITYMATRIX_H
#define	CIDENTITYMATRIX_H

#include <map>
#include <set>
#include <string>

using namespace std;

#include "AbstractMatrix.h"
#include "CBuilder.h"
#include "CFraction.h"

/// Class for representation diagonal matrix
class CDiagonalMatrix : public AbstractMatrix {
public:
    /// Constructor
    /** \param rows Number of rows
     *  \param col Number of columns
     *  \param name Name of matrix
     *  \param numbers Map of fractions at diagonal */
    CDiagonalMatrix(int rows, int cols, string name, const map<CFraction,set<pair<int, int>> > & numbers);
    /// Copy constructor
    /* \param mat Matrix for copying */
    CDiagonalMatrix(const CDiagonalMatrix & mat);
    virtual AbstractMatrix* clone() const;
    virtual ~CDiagonalMatrix();
    virtual CFraction atPosition(int x, int y) const;
    virtual CFraction gaussianElimination(ostream& os, bool isPrint);
    virtual int rank() const;
    virtual string getNameOfType() const;
    virtual map<pair<int, int>, CFraction> getMap() const;
private:
    virtual void gaussJordanElimination();
    /// Gets width of fraction at position [x, y]
    /** \param x Position x
     *  \param y Position y
     *  \return Width of fraction */
    int getWidth(int x, int y) const;
    virtual int* refreshWidth() const;
    CFraction * array; ///< Represents fractions at diagonal
};

#endif	/* CIDENTITYMATRIX_H */

