/** \file CCalculator.h
 *  Header file for CCalculator.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#ifndef CCALCULATOR_H
#define	CCALCULATOR_H

#include <map>
#include <fstream>
#include <iomanip>

#include "CFraction.h"
#include "CException.h"
#include "AbstractMatrix.h"
#include "CBuilder.h"
#include "CExpression.h"
using namespace std;

/** @def MENU_ITEMS
    Number of items in menu */
#define MENU_ITEMS 8 
#define CALC_MENU_ITEMS 7

/// Class for calculating with matrices and fractions
class CCalculator {
public:
    /// Constructor with path file
    /** \param file Path of file */
    CCalculator(const string & file);
    /// Print menu of calculator
    void printMenu() const;
    /// List all matrices
    void listAll() const;
    /// Add matrix into database
    void addMatrix();
    /// Delet matrix into database
    void deleteMatrix();
    /// Print Gaussian elimination on matrix
    void gaussElim() const;
    /// Import matrices from file
    /** \param filename Path of files */
    void ImportFile(const string & fileName);
    /// Import from file
    void importFromFile();
    /// Calculate with matrices and fractions
    void calculate();
    /// Print matrix
    void printMatrix() const;
    /// Save matrices into file
    void saveToFile(const string & nameFile) const;
    /// Run app
    void runApp();
    /// Destructor
    ~CCalculator();
private:
    /// Read name of matrix
    string readName() const;
    map<string, AbstractMatrix* > matrices; ///< Array of matrices
    static string menuArray[MENU_ITEMS]; ///< Array with menu items
    static string calcMenu[CALC_MENU_ITEMS]; ///< Array with calc menu items;
    string saveFile; ///< Default file from input and output matrices
    bool isSaveFile;
    static string banner; ///< Banner at the top of menu
};

#endif	/* CCALCULATOR_H */

