/** \file CSparseMatrix.h
 *  Header file for CSparseMatrix.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef SPARSEMATRIX_H
#define	SPARSEMATRIX_H

#include <string>
#include <set>
#include <map>
using namespace std;
#include "AbstractMatrix.h"

/// Class for representation sparse matrix
class CSparseMatrix : public AbstractMatrix {
public:
    /// Constructor
    /** \param rows Number of rows
     *  \param col Number of columns
     *  \param name Name of matrix
     *  \param numbers Nonnull map of fractions */
    CSparseMatrix(int rows, int cols, const string & name, const map<CFraction,set<pair<int, int>> > & numbers);
    /// Copy constructor
    /* \param mat Matrix for copying */
    CSparseMatrix(const CSparseMatrix & mat);
    virtual AbstractMatrix* clone() const;
    virtual ~CSparseMatrix();
    virtual string getNameOfType() const;
    virtual CFraction atPosition(int x, int y) const;
    virtual CFraction gaussianElimination(ostream& os, bool print);
    virtual map<pair<int, int>, CFraction> getMap() const;
    virtual int rank() const;
private:
    virtual void gaussJordanElimination();
    virtual int * refreshWidth() const;
    virtual int getLength(int x, int y) const;
    /// Sets fraction at position [x, y]
    /** \param x Position x
     *  \param y Position y
     *  \param fract New fraction for setting */
    void setAtPosition(int x, int y, const CFraction & frac);
    /// Increment line to second line
    /** \param baseLine Line which will be sum to other line 
     *  \param line Current for incrementing 
     *  \param column First nonNull fraction at baseLine */
    void sum2Lines(int baseLine, int line, int column);
    /// Moves 2 lines
    /** \param a Line number 1
     *  \param b Line number 2 */
    void move2Lines(int a, int b);
    /// Set fraction at position [line,line] to 1
    /** \param line Line for divide */
    void divideLine(int line);
    map<pair<int,int>, CFraction> array; ///< Map for represenation matrix's body

};

#endif	/* SPARSEMATRIX_H */

