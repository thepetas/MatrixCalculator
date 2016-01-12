/** \file CBuilder.cpp
 *  Implementation of CBuilder class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CBuilder.h"

CBuilder::CBuilder(int rows, int columns, const string & name, const string & data) : rows(rows), columns(columns), name(name), cntNull(0) {
    loadData(data);
}

CBuilder::CBuilder(int rows, int columns, const string & name, const map<pair<int, int>, CFraction> & data) : rows(rows),
columns(columns), name(name) {
    cntNull = rows * columns - data.size();
    for (auto it : data) {
        insertFract(it.second, it.first.first, it.first.second);
    }
}

CBuilder::~CBuilder() {
}

void CBuilder::loadData(const string & data) {
    size_t start = data.find_first_of('{');
    if (start == string::npos) throw CException("\"" + name + "\" - invalid format.");
    if (data.find_first_not_of(' ') != start) throw CException("\"" + name + "\" - invalid format.");
    size_t end = 0;
    for (int rws = 0; rws < rows; rws++) {
        start = data.find_first_of('{', start + 1);
        if (start == string::npos) throw CException("\"" + name + "\" - invalid format.");

        //control between rows
        bool wasComma = false;
        for (unsigned int z = end + 1; z < start; z++) {
            if (data[z] == ',' && rws > 0) {
                if (wasComma)
                    throw CException("\"" + name + "\" - invalid format.");
                else
                    wasComma = true;
            } else if (data[z] == ' ');
            else
                throw CException("\"" + name + "\" - invalid format.");
        }
        end = data.find_first_of("}", end + 1);
        if (end == string::npos)  throw CException("\"" + name + "\" - invalid format.");
        size_t startNum, endNum = start;

        //load and save fractions in line
        for (int col = 0; col < columns; col++) {
            startNum = endNum + 1;
            if (col + 1 < columns) {
                endNum = data.find_first_of(',', startNum);
                if (endNum >= end)
                    throw CException("\"" + name + "\" - invalid format.");
            } else {
                endNum = data.find_first_of('}', startNum);
                if (endNum != end)
                    throw CException("\"" + name + "\" - invalid format.");
            }
            if (endNum == string::npos || startNum == string::npos)
                throw CException("\"" + name + "\" - invalid format.");

            try {
                CFraction fract(string(data.begin() + startNum, data.begin() + endNum));
                insertFract(fract, rws, col);
            }
            catch( CException & ex){
                throw CException("\"" + name + "\" - invalid format. ");
            }
        }
    }
    //control behind last rows
    size_t checkEnd = data.find_first_not_of(' ', end + 1);
    end = data.find_first_of("}", end + 1);
    if (checkEnd == string::npos || end == string::npos || checkEnd != end)
        throw CException("\"" + name + "\" - invalid format.");
}

void CBuilder::insertFract(const CFraction& fract, int x, int y) {
    if (fract == 0) {
        cntNull++;
        return;
    }
    auto it = numbers.find(fract);
    if (it == numbers.end()) {
        set<pair<int, int> > num;
        num.insert(make_pair(x, y));
        numbers.insert(make_pair(fract, num));
    } else
        (*it).second.insert(make_pair(x, y));
}

void CBuilder::print() const {
    cout << "rows=" << rows << ", columns=" << columns << ", name=" << name << endl;
    cout << "cnt null= " << cntNull << endl;
    for (auto it : numbers) {
        cout << (it).first << " : ";
        for (auto x : (it).second) {
            cout << "[ " << x.first << " , " << x.second << " ], ";
        }
        cout << endl;
    }
}

AbstractMatrix * CBuilder::getType() const {
    if (cntNull != 0) {
        if (rows == columns && (rows * columns - rows) == cntNull) {
            bool isDiagonal = true;
            for (auto x : numbers) {
                for (auto coord : x.second) {
                    if (coord.first != coord.second) isDiagonal = false;
                }
                if (!isDiagonal) break;
            }
            if (isDiagonal) {
                return new CDiagonalMatrix(rows, columns, name, numbers);
            }
        }
        double nonNull = rows * columns - cntNull;
        if ((nonNull / (rows * columns)) * 100 < 35) {
            return new CSparseMatrix(rows, columns, name, numbers);
        }
    }
    return new CMatrix(rows, columns, name, numbers);
}