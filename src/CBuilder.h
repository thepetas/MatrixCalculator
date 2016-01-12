/** \file CBuilder.h
 *  Header file for CBuilder.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef CBUILDER_H
#define	CBUILDER_H

#include <string>
#include <map>
#include <set>
using namespace std;

#include "AbstractMatrix.h"
#include "CDiagonalMatrix.h"
#include "CException.h"
#include "CFraction.h"
#include "CMatrix.h"
#include "CSparseMatrix.h"

/// Class for build type of matrix
class CBuilder {
public:
    /// Constructor with string format of matrix
    /** \param rows Rows number of matrix
     *  \param columns Columns number of matrix
     *  \param name Name of matrix
     *  \param text Representation of matrix's body */
    CBuilder(int rows, int columns, const string & name, const string & data);
    /// Constructor with string format of matrix
    /** \param rows Rows number of matrix
     *  \param columns Columns number of matrix
     *  \param name Name of matrix
     *  \param data Map with nonnull fraction (position is key, second is fraction)  */
    CBuilder(int rows, int columns, const string & name, const map<pair<int, int>, CFraction> & data);
    /// Destructor
    ~CBuilder();
    /// Print all fraction with position/s (debug tool) 
    void print() const;
    /// Make type of matrix (Normal, Diagonal or Sparse matrix)
    /** \return New AbstractMatrix* */
    AbstractMatrix * getType() const;
    /// Load data from string and save into map
    /** \param data Text representation of matrix's body */
    void loadData(const string & data);
private:
    /// Insert fraction into map
    /** \param fract Fraction which will be saved into map
     *  \param x First part of position
     *  \param y Second part of position */
    void insertFract(const CFraction & fract, int x, int y);
    int rows; ///< Rows of matrix
    int columns; ///< Columns of matrix
    string name; ///< Name of matrix
    int cntNull; ///< Number of null fractions
    map<CFraction, set<pair<int, int>> > numbers; ///< Map with saved position of nonnull fraction for matrix
};

#endif	/* CBUILDER_H */

