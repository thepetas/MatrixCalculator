/** \file CMatrix.h
 *  Header file for CMatrix.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef CMATRIX_H
#define	CMATRIX_H

#include <sstream>
#include <algorithm>
#include <iomanip>
#include <string.h>
#include <map>
#include <set>

#include "CFraction.h"
#include "AbstractMatrix.h"
/// Class for representation normal matrix
class CMatrix : public AbstractMatrix {
public:
    /// Constructor
    /** \param rows Number of rows
     *  \param col Number of columns
     *  \param name Name of matrix
     *  \param numbers Nonnull map of fractions */
    CMatrix(int rows, int col, string name,const map<CFraction,set<pair<int, int>> > & numbers);
    /// Copy constructor
    /* \param mat Matrix for copying */
    CMatrix(const CMatrix & mat);
    /// Destructor
    virtual ~CMatrix();
    virtual int rank() const;
    virtual CFraction gaussianElimination(ostream& os, bool print);
    virtual AbstractMatrix * clone() const;
    virtual string getNameOfType() const;
    virtual CFraction atPosition(int x, int y) const;
    virtual map<pair<int, int>, CFraction> getMap() const;
private:
    virtual void gaussJordanElimination();
    virtual int* refreshWidth() const;
    /// Increment line to second line
    /** \param baseLine Line which will be sum to other line 
     *  \param line Current for incrementing 
     *  \param column First nonNull fraction at baseLine */
    void sum2Lines(int baseLine, int line, int column);
    /// Set fraction at position [line,line] to 1
    /** \param line Line for divide */
    void divideLine(int line);
    CFraction ** array; ///< representation of matrix's body
};

#endif	/* CMATRIX_H */

