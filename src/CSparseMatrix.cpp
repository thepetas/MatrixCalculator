/** \file CSparseMatrix.cpp
 *  Implementation of CSparseMatrix class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CSparseMatrix.h"
#include "CBuilder.h"

CSparseMatrix::CSparseMatrix(int rows, int cols, const string& name, const map<CFraction, set<pair<int, int>> > & numbers)
: AbstractMatrix(rows, cols, name) {

    for (auto frac : numbers) {
        for (auto coord : frac.second) {
            array.insert(make_pair(make_pair(coord.first, coord.second), frac.first));
        }
    }
}

CSparseMatrix::CSparseMatrix(const CSparseMatrix& mat) : AbstractMatrix(mat.rows, mat.columns, mat.name) {
    array = mat.array;
}

CSparseMatrix::~CSparseMatrix() {

}

AbstractMatrix* CSparseMatrix::clone() const {
    return new CSparseMatrix(*this);
}

CFraction CSparseMatrix::atPosition(int x, int y) const {
    auto it = array.find(make_pair(x, y));
    if (it == array.end()) return CFraction(0);
    else
        return (*it).second;
}

int CSparseMatrix::getLength(int x, int y) const {
    return atPosition(x, y).getWidth();
}

string CSparseMatrix::getNameOfType() const {
    return NAMECSPARSEMATRIX;
}

int * CSparseMatrix::refreshWidth() const {
    int * width = new int[columns];
    for (int i = 0; i < columns; i++)
        width[i] = 1;
    for (auto it : array) {
        if (it.second.getWidth() > width[it.first.second])
            width[it.first.second] = it.second.getWidth();
    }
    return width;
}

void CSparseMatrix::setAtPosition(int x, int y, const CFraction& frac) {
    auto it = array.find(make_pair(x, y));
    if (frac == 0) {
        array.erase(it);
        return;
    }
    if (it == array.end()) {
        array.insert(make_pair(make_pair(x, y), frac));
    } else {
        (*it).second = frac;
    }
}

map<pair<int, int>, CFraction> CSparseMatrix::getMap() const {
    return array;
}

CFraction CSparseMatrix::gaussianElimination(ostream& os, bool isPrint) {
    CFraction multipleDeterminant(1);
    if (isPrint)
        cout << this;
    int gausLine = 0;
    for (int baseColumn = 0; baseColumn < columns; baseColumn++) {
        ostringstream changing;
        for (int line = gausLine + 1; line < rows; line++) {
            if (atPosition(gausLine, baseColumn) == 0 && atPosition(line, baseColumn) != 0) {
                move2Lines(gausLine, line);
                changing << "--move lines " << gausLine + 1 << " & " << line + 1 << endl;
                multipleDeterminant = multipleDeterminant * -1;
            } else if (atPosition(line, baseColumn) != 0) {
                if (atPosition(line, baseColumn).Abs() < atPosition(gausLine, baseColumn).Abs()) {
                    move2Lines(gausLine, line);
                    changing << "--move lines " << gausLine + 1 << " & " << line + 1 << endl;
                    multipleDeterminant = multipleDeterminant * -1;
                }
                sum2Lines(gausLine, line, baseColumn);
                multipleDeterminant = multipleDeterminant * atPosition(gausLine, baseColumn);
                changing << "--sum lines " << gausLine + 1 << " & " << line + 1 << endl;
            }
        }
        if (changing.str() != "") {
            gausLine++;
            if (isPrint) {
                cout << "-------------------------------------------------------------------------------" << endl;
                print(os);
                cout << changing.str();
            }
        } else if (changing.str() == "" && atPosition(gausLine, baseColumn) != 0)
            gausLine++;
    }
    if (isPrint)
        cout << "End of Gaussian elimination." << endl;
    return multipleDeterminant;
}

void CSparseMatrix::move2Lines(int a, int b) {
    map<pair<int, int>, CFraction> lines;
    for (int i = 0; i < columns; i++) {
        auto it1 = array.find(make_pair(a, i));
        auto it2 = array.find(make_pair(b, i));
        if (it1 != array.end()) {
            lines.insert(make_pair(make_pair(b, (*it1).first.second), (*it1).second));
            array.erase(it1);
        }
        if (it2 != array.end()) {
            lines.insert(make_pair(make_pair(a, (*it2).first.second), (*it2).second));
            array.erase(it2);
        }
    }
    array.insert(lines.begin(), lines.end());
}

void CSparseMatrix::gaussJordanElimination() {
    gaussianElimination(cout, false);
    for (int line = rows - 1; line >= 0; line--) {
        for (int col = rows - 1; col > line; col--)
            if (atPosition(line, col) != 0) {
                sum2Lines(col, line, col);
            }
    }

    for (int i = 0; i < rows; i++)
        divideLine(i);
}

void CSparseMatrix::divideLine(int line) {
    if (atPosition(line, line) == 1) return;
    CFraction div = CFraction(1) / atPosition(line, line);
    for (int i = line; i < columns; i++)
        atPosition(line, i) = atPosition(line, i) * div;
}

void CSparseMatrix::sum2Lines(int baseLine, int line, int column) {
    CFraction tmp(atPosition(line, column));
    if ((atPosition(line, column) < 0 && atPosition(baseLine, column) < 0) || (atPosition(line, column) > 0 && atPosition(baseLine, column) > 0)) {
        for (int i = 0; i < columns; i++) {
            CFraction ins = atPosition(line, i) * atPosition(baseLine, column) - atPosition(baseLine, i) * tmp;
            if (ins == 0) {
                array.erase(make_pair(line, i));
            } else
                setAtPosition(line, i, ins);
        }
    } else {
        for (int i = 0; i < columns; i++) {
            CFraction ins = (atPosition(line, i) * atPosition(baseLine, column)) - (atPosition(baseLine, i) * tmp);
            if (ins == 0) {
                array.erase(make_pair(line, i));
            } else
                setAtPosition(line, i, ins);
        }
    }
}

int CSparseMatrix::rank() const {
    CSparseMatrix mat(*this);
    mat.gaussianElimination(cout, false);
    map<pair<int, int>, CFraction> map;
    map = mat.getMap();
    if (map.empty()) return 0;
    else {
        auto it = --map.end();
        return (*it).first.first + 1;
    }
}