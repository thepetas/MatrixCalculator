/** \file main.cpp
 *  Main class for start CCalculator
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

using namespace std;

#include "CCalculator.h"
#include <getopt.h>


 /** \mainpage
 *  This project implements MatrixCalculator. This calculator can calculate with matrices and fractions (by mathematical rules).
 *  \section Main Description
 *  For representation of matrix we have one abstract class. This abstract class is AbstractMatrix and it's a parent of the other matrix classes. 
 *  Type of matrix is:
 *  \li CMatrix, which represents normal matrix with small count of null numbers.
 *  \li CDiagonalMatrix, which represents diagonal matrix. This matrix has only nonnull numbers at main diagonal.
 *  \li CSparseMatrix, which represents matrix with large count of null numbers. For this implementation - the sparse matrix is matrix, which have maximum 35 % of nonnull numbers.
 *  This calculator can do determinant and rank of matrix, inverse matrix, transpose matrix, Gaussian elimination with commentation of steps. It can do operation with matrix as +, - , *, and fraction * matrix or matrix / fraction.
*/



/// Start main program
int main(int argc, char **argv) {
    string filePath;
    /*----------------------------------------------------------------------------------------*/
    /*This part of program is copied from EDUX, author - Miro Hroncok, example of load options*/
    /*----------------------------------------------------------------------------------------*/
    while (1) {
        static struct option long_options[] = {
            {"file", 1, 0, 'f'},
            {0, 0, 0, 0}
        };
        // getopt_long stores the option index here
        int option_index = 0;
        int c = getopt_long(argc, argv, "f:", long_options, &option_index);
        // detect the end of the options
        if (c == -1) {
            break;
        }
        switch (c) {
            case 'f':
                filePath = optarg;
                break;
            default:
                cout << "Start program with parameter -f <filePath>" << endl;
                return 1;
        }
    }
    /*----------------------------------------------------------------------------------------*/
    /*This part of program is copied from EDUX, author - Miro Hroncok, example of load options*/
    /*----------------------------------------------------------------------------------------*/

    CCalculator calc(filePath);
    calc.runApp();
    return 0;
}