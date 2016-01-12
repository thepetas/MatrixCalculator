/** \file AbstractMatrix.cpp
 *  Implementation of AbstractMatrix class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "AbstractMatrix.h"
#include "CBuilder.h"

AbstractMatrix::AbstractMatrix(int rows, int columns, const string & name) {
    this->columns = columns;
    this->rows = rows;
    this->name = name;
}

AbstractMatrix::~AbstractMatrix() {
}

int AbstractMatrix::getColumns() const {
    return columns;
}

int AbstractMatrix::getRows() const {
    return rows;
}

string AbstractMatrix::getName() const {
    return name;
}

void AbstractMatrix::setName(const string & name) {
    this->name = name;
}

ostream & operator<<(ostream & os, const AbstractMatrix * mat) {
    cout << mat->name << "( " << mat->rows << " x " << mat->columns << " ),type: " << mat->getNameOfType() << endl;
    mat->print(os);
    return os;
}

void AbstractMatrix::print(ostream& os) const {
    int * width = refreshWidth();
    for (int i = 0; i < rows; i++) {
        os << "( ";
        for (int j = 0; j < columns; j++) {
            os << setw(width[j]) << atPosition(i, j) << " ";
        }
        os << ")" << endl;
    }
    delete [] width;
}

AbstractMatrix * AbstractMatrix::operator+(const AbstractMatrix * mat) const {
    if (this->rows != mat->rows || this->columns != mat->columns) {
        throw CException("Matrix \"" + name + "\" haven't same size as \"" + mat->name + "\".");
    }
    map<pair<int, int>, CFraction > right;
    right = mat->getMap();
    for (auto it : getMap()) {
        auto find = right.find(it.first);
        if (find == right.end())
            right.insert(make_pair(it.first, it.second));
        else if ((*find).second + it.second != 0)
            (*find).second = (*find).second + it.second;
    }
    CBuilder x(rows, columns, name + "+" + mat->name, right);
    return x.getType();
}

AbstractMatrix* AbstractMatrix::operator-(const AbstractMatrix* mat) const {
    if (this->rows != mat->rows || this->columns != mat->columns) {
        throw CException("Matrix \"" + name + "\" haven't same size as \"" + mat->name + "\".");
    }
    map<pair<int, int>, CFraction > left;
    left = getMap();


    for (auto it : mat->getMap()) {
        auto find = left.find(it.first);
        if (find == left.end())
            left.insert(make_pair(it.first, it.second * -1));
        else {
            if ((*find).second - it.second != 0)
                (*find).second = (*find).second - it.second;
            else
                left.erase(find);
        }
    }

    CBuilder x(rows, columns, name + "-" + mat->name, left);
    return x.getType();
}

AbstractMatrix* AbstractMatrix::operator*(const AbstractMatrix* mat) const {
    if (columns != mat->rows) {
        throw CException("Matrix \"" + this->name + "\" hasn't same number of columns as matrix \"" + mat->name + "\" rows.");
    }
    map<pair<int, int>, CFraction > map;

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < mat->columns; y++) {
            CFraction fract(0);
            for (int z = 0; z < columns; z++) {
                CFraction l1 = atPosition(x, z), r1 = mat->atPosition(z, y);
                if (l1 != 0 && r1 != 0) {
                    fract = fract + (l1 * r1);
                }
            }
            if (fract != 0)
                map.insert(make_pair(make_pair(x, y), fract));
        }
    }
    CBuilder x(rows, mat->columns, name + "*" + mat->name, map);
    return x.getType();
}

AbstractMatrix * operator*(const CFraction & num, const AbstractMatrix & mat) {
    map<pair<int, int>, CFraction > map2;
    if (num != 0) {
        for (auto it : mat.getMap()) {
            map2.insert(make_pair(it.first, it.second * num));
        }
    }
    CBuilder x(mat.getRows(), mat.getColumns(), num.ToString() + "*" + mat.name, map2);
    return x.getType();
}

AbstractMatrix* AbstractMatrix::operator/(const CFraction & num) const {
    map<pair<int, int>, CFraction> map;
    map = getMap();
    for (auto it = map.begin(); it != map.end(); it++) {
        (*it).second = (*it).second / num;
    }
    CBuilder x(rows, columns, name + "/" + num.ToString(), map);
    return x.getType();
}

AbstractMatrix* AbstractMatrix::getTranspose() const {
    map<pair<int, int>, CFraction > trans;
    for (auto it : getMap()) {
        trans.insert(make_pair(make_pair(it.first.second, it.first.first), it.second));
    }
    CBuilder x(columns, rows, "transpose(" + name + ")", trans);
    return x.getType();
}

CFraction AbstractMatrix::determinant() const {
    if (rows != columns) return CFraction(0);
    AbstractMatrix * mat = clone();
    CFraction deter_denom = mat->gaussianElimination(cout, false);
    CFraction deter_num(1);
    for (int i = 0; i < rows; i++) {
        deter_num = deter_num * mat->atPosition(i, i);
    }
    delete mat;
    if (deter_num == 0) return 0;
    return deter_num / deter_denom;
}

AbstractMatrix* AbstractMatrix::getInverseMatrix() const {
    if (rows != columns)
        throw CException("Can't make inversion matrix. It's not regular matrix.");
    if (rank() != rows)
        throw CException("Can't make inversion matrix. There is some dependent rows.");

    map<pair<int, int>, CFraction > map;
    map = getMap();

    for (int i = 0; i < rows; i++) {
        map.insert(make_pair(make_pair(i, columns + i), CFraction(1)));
    }
    CBuilder x(rows, columns * 2, "Inverse(" + name + ")", map);
    AbstractMatrix * mat = x.getType();

    mat->gaussJordanElimination();
    map.clear();
    for (auto it : mat->getMap()) {
        if (it.first.second >= rows) {
            map.insert(make_pair(make_pair(it.first.first, it.first.second - rows), it.second));
        }
    }
    delete mat;
    CBuilder x1(rows, rows, "Inverse(" + name + ")", map);
    return x1.getType();
}

string AbstractMatrix::fileFormat() const {
    ostringstream oss;
    oss << name << "[" << rows << "," << columns << "]" << "{";
    for (int line = 0; line < rows; line++) {
        oss << "{";
        for (int col = 0; col < columns; col++) {
            oss << atPosition(line, col);
            if (col + 1 < columns)
                oss << ",";
        }
        if (line + 1 == rows)
            oss << "}}";
        else
            oss << "},";
    }
    return oss.str();
}

string AbstractMatrix::NAMECMATRIX = "NormalMatrix";
string AbstractMatrix::NAMECDIAGONALMATRIX = "DiagonalMatrix";
string AbstractMatrix::NAMECSPARSEMATRIX = "SparseMatrix";
string AbstractMatrix::INVALIDCHARACTERS = "+-*/()[]{} ";