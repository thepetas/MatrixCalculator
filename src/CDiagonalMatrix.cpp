/** \file CDiagonalMatrix.cpp
 *  Implementation of CDiagonalMatrix class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CDiagonalMatrix.h"

CDiagonalMatrix::CDiagonalMatrix(int rows, int cols, string name, const map< CFraction, set<pair<int, int>> > & numbers)
: AbstractMatrix(rows, cols, name) {
    array = new CFraction[rows];
    for (auto fract : numbers) {
        for (auto coord : fract.second) {
            array[coord.first] = fract.first;
        }
    }
}

CDiagonalMatrix::CDiagonalMatrix(const CDiagonalMatrix& mat) : AbstractMatrix(mat.rows, mat.columns, mat.name) {
    array = new CFraction[mat.rows];
    memcpy(array, mat.array, sizeof (CFraction) * rows);
}

CDiagonalMatrix::~CDiagonalMatrix() {
    delete [] array;
}

AbstractMatrix * CDiagonalMatrix::clone() const {
    return new CDiagonalMatrix(*this);
}

CFraction CDiagonalMatrix::atPosition(int x, int y) const {
    if (x == y) {
        return array[x];
    } else
        return CFraction(0);
}

int CDiagonalMatrix::getWidth(int x, int y) const {
    if (x == y)
        return atPosition(x, y).getWidth();
    else
        return 1;
}

int * CDiagonalMatrix::refreshWidth() const {
    int * width = new int[columns];
    for (int i = 0; i < columns; i++)
        width[i] = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (getWidth(i, j) > width[j])
                width[j] = getWidth(i, j);
        }
    }
    return width;
}

string CDiagonalMatrix::getNameOfType() const {
    return NAMECDIAGONALMATRIX;
}

map<pair<int, int>, CFraction > CDiagonalMatrix::getMap() const {
    map<pair<int, int>, CFraction > map;
    for(int i = 0 ; i < rows ; i++)
        map.insert(make_pair(make_pair(i,i),array[i]));
    return map;
}

CFraction CDiagonalMatrix::gaussianElimination(ostream& os, bool isPrint) {
    if (isPrint) {
        os << this;
        cout << "End of Gaussian elimination." << endl;
    }
    return CFraction(1);
}

void CDiagonalMatrix::gaussJordanElimination() {
    for (int i = 0; i < rows; i++) {
        if (array[i] == 1) return;
        array[i] = CFraction(1);
    }
    return;
}


int CDiagonalMatrix::rank() const {
    return rows;
}
