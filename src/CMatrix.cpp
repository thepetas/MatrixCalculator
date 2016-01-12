/** \file CMatrix.cpp
 *  Implementation of CMatrix class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CMatrix.h"
#include "CBuilder.h"

CMatrix::CMatrix(int rows, int col, string name, const map<CFraction, set<pair<int, int>> > & numbers) : AbstractMatrix(rows, col, name) {
    array = new CFraction*[rows];
    for (int i = 0; i < rows; i++) {
        array[i] = new CFraction[col];
        for (int x = 0; x < columns; x++)
            array[i][x] = CFraction(0);
    }
    for (auto fr : numbers) {
        for (auto coord : fr.second) {
            array[coord.first][coord.second] = fr.first;
        }
    }
}

CMatrix::CMatrix(const CMatrix & mat) : AbstractMatrix(mat.rows, mat.columns, mat.name) {
    array = new CFraction*[rows];
    for (int i = 0; i < rows; i++) {
        array[i] = new CFraction[columns];
        for (int x = 0; x < columns; x++)
            array[i][x] = mat.array[i][x];
    }
}

CMatrix::~CMatrix() {
    for (int i = 0; i < rows; i++) {
        delete [] array[i];
    }
    delete [] array;
}

AbstractMatrix* CMatrix::clone() const {
    return new CMatrix(*this);
}

CFraction CMatrix::atPosition(int x, int y) const {
    return array[x][y];
}

int * CMatrix::refreshWidth() const {
    int * width = new int [columns];
    for (int i = 0; i < columns; i++)
        width[i] = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (array[i][j].getWidth() > width[j])
                width[j] = array[i][j].getWidth();
        }
    }
    return width;
}

CFraction CMatrix::gaussianElimination(ostream & os, bool isPrint) {
    CFraction multipleDeterminant(1);
    if (isPrint)
        os << this;
    int gausLine = 0;
    for (int baseColumn = 0; baseColumn < columns; baseColumn++) {
        ostringstream changing;
        for (int line = gausLine + 1; line < rows; line++) {
            if (array[gausLine][baseColumn] == 0 && array[line][baseColumn] != 0) {
                swap(array[gausLine], array[line]);
                changing << "--move lines " << gausLine + 1 << " & " << line + 1 << endl;
                multipleDeterminant = multipleDeterminant* -1;
            } else if (array[line][baseColumn] != 0) {
                if (array[line][baseColumn].Abs() < array[gausLine][baseColumn].Abs()) {
                    swap(array[gausLine], array[line]);
                    changing << "--move lines " << baseColumn + 1 << " & " << line + 1 << endl;
                    multipleDeterminant = multipleDeterminant * -1;
                }
                sum2Lines(gausLine, line, baseColumn);
                multipleDeterminant = multipleDeterminant * array[gausLine][baseColumn];
                changing << "--sum lines " << gausLine + 1 << " & " << line + 1 << endl;
            }
        }
        if (!changing.str().empty()) {
            gausLine++;
            if (isPrint) {
                cout << "-------------------------------------------------------------------------------" << endl;
                print(os);
                cout << changing.str();
            }
        } else if (gausLine < rows && array[gausLine][baseColumn] != 0)
            gausLine++;
    }
    if (isPrint)
        cout << "End of Gaussian elimination." << endl;

    return multipleDeterminant;
}

void CMatrix::gaussJordanElimination() {
    gaussianElimination(cout, false);
    for (int line = rows - 1; line >= 0; line--) {
        for (int col = rows - 1; col > line; col--)
            if (array[line][col] != 0) {
                sum2Lines(col, line, col);
            }
    }

    for (int i = 0; i < rows; i++)
        divideLine(i);
    return;
}

void CMatrix::divideLine(int line) {
    if (array[line][line] == 1) return;
    CFraction div = CFraction(1) / array[line][line];
    for (int i = line; i < columns; i++)
        array[line][i] = array[line][i] * div;
}

void CMatrix::sum2Lines(int base, int line, int column) {
    CFraction firstElem(array[line][column]);
    if ((array[line][column] < 0 && array[base][column] < 0) || (array[line][column] > 0 && array[base][column] > 0)) {
        for (int i = 0; i < columns; i++) {
            array[line][i] = array[line][i] * array[base][column] - array[base][i] * firstElem;
        }
    } else {
        for (int i = 0; i < columns; i++)
            array[line][i] = (array[line][i] * array[base][column]) - (array[base][i] * firstElem);
    }
}

string CMatrix::getNameOfType() const {
    return NAMECMATRIX;
}

map<pair<int, int>, CFraction> CMatrix::getMap() const {
    map<pair<int, int>, CFraction > map;
    for (int i = 0; i < rows; i++) {
        for (int z = 0; z < columns; z++)
            if (array[i][z] != 0)
                map.insert(make_pair(make_pair(i, z), array[i][z]));
    }
    return map;
}

int CMatrix::rank() const {
    CMatrix res(*this);
    res.gaussianElimination(cout, false);
    int rank = res.rows - 1;
    while (rank >= 0) {
        for (int i = res.columns - 1; i >= 0; i--) {
            if (res.array[rank][i] != 0) {
                return rank + 1;
            }
        }
        rank--;
    }
    return 0;
}
