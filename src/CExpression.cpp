/** \file CExpression.cpp
 *  Implementation of CExpression class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CExpression.h"
#include "CBuilder.h"

CExpression::CExpression(const string& expr, const map<string, AbstractMatrix*>& matrices) {
    if (expr.empty()) throw CException("Empty calculation line.");
    cntConsoleMatrix = 0;
    size_t left = 0, middle = expr.find_first_not_of(VALID_OPERATORS + " "), right = expr.find_first_of(VALID_OPERATORS, middle);
    while (1) {
        if (expr.at(middle) == '{') {
            readInsertMatrix(expr, left, middle, right);
            if (right == string::npos) break;
        } else {
            insertItem(expr, matrices, left, middle, right);
            if (right == string::npos) break;
        }
        left = right + 1;
        if (left == string::npos)
            throw CException("Invalid calculation line.");
        middle = expr.find_first_not_of(VALID_OPERATORS + " ", left);

        if (middle == string::npos)
            throw CException("Invalid calculation line.");
        right = expr.find_first_of(VALID_OPERATORS, middle);
    }
    solve();
}

CExpression::~CExpression() {
}

void CExpression::insertItem(const string & data, const map<string, AbstractMatrix*>& matrices, size_t left, size_t & middle, size_t & right) {
    if (data.empty()) throw CException("Empty line in calculation.");
    // Insert fraction into vector
    if ((data[middle] <= '9' && data[middle] >= '0') || data[middle] == '-') {
        if (right == string::npos)
            insertFraction(string(data.begin() + left, data.end()), 'X');
        else
            insertFraction(string(data.begin() + left, data.begin() + right), data[right]);
    } else {
        size_t startBracket = data.find_first_of('[', left), endBracket = data.find_last_of(']', right);
        // Function as determinant, inversion matrix,...
        if (startBracket != string::npos && endBracket != string::npos && startBracket < endBracket) {
            right = data.find_first_of(VALID_OPERATORS, endBracket);
            string function(data.begin() + middle, data.begin() + startBracket);
            size_t nameStart = data.find_first_not_of(' ', startBracket + 1), nameEnd = data.find_last_not_of(' ', endBracket - 1) + 1;
            if (nameStart == string::npos || nameEnd > endBracket) nameStart = startBracket + 1;
            if (nameEnd == string::npos) nameEnd = endBracket;
            string matrixName(data.begin() + nameStart, data.begin() + nameEnd);

            AbstractMatrix * matrix;
            if (matrixName[0] == '{') {
                size_t endFirstRow = matrixName.find_first_of('}');
                if (endFirstRow == string::npos)
                    throw CException("Invalid format in calculation.");
                size_t columns = count(matrixName.begin(), matrixName.begin() + endFirstRow, ',') + 1;
                size_t rows = count(matrixName.begin(), matrixName.end(), '{') - 1;
                ostringstream newName;
                newName << "Console" << cntConsoleMatrix++;
                CBuilder build(rows, columns, newName.str(), matrixName);
                matrix = build.getType();
            } else {
                auto findM = matrices.find(matrixName);
                if (findM == matrices.end())
                    throw CException("Matrix \"" + matrixName + "\" doesn't exist.");
                matrix = (*findM).second->clone();
            }
            if (function == DETERMINANT) {
                CFraction deter = matrix->determinant();
                if (right != string::npos)
                    insertFraction(deter.ToString(), data[right]);
                else
                    insertFraction(deter.ToString(), 'X');
            } else if (function == INVERSEMATRIX) {
                try {
                    AbstractMatrix * mat = matrix->getInverseMatrix();
                    operands.push_back(CItem(mat));
                    delete mat;
                    if (right != string::npos) operators.push_back(data[right]);
                } catch (CException & ex) {
                    delete matrix;
                    throw CException(ex.getMessage());
                }
            } else if (function == RANK) {
                ostringstream number;
                number << matrix->rank();
                if (right != string::npos)
                    insertFraction(number.str(), data[right]);
                else
                    insertFraction(number.str(), 'X');
            } else if (function == TRANSPOSEMATRIX) {
                AbstractMatrix * mat = matrix->getTranspose();
                operands.push_back(CItem(mat));
                delete mat;
                if (right != string::npos) operators.push_back(data[right]);
            } else
                throw CException("Function \"" + function + "\" doesn't exist.");
            delete matrix;

        }// Find matrix in database and insert into vector
        else {
            string name;
            if (data.find_first_not_of(' ', left) != middle) throw CException("Invalid format in calculation.");
            size_t endName = data.find_first_of(' ', middle);
            if (right == string::npos) {
                if (endName == string::npos) {
                    name = string(data.begin() + middle, data.end());
                } else if (data.find_first_not_of(' ', endName + 1) != string::npos) throw CException("Invalid format in calculation.");
                else
                    name = string(data.begin() + middle, data.begin() + endName);
            } else {
                if (data.find_first_not_of(' ', endName + 1) != right) throw CException("Invalid format in calculation.");
                name = string(data.begin() + middle, data.begin() + endName);
            }
            auto findM = matrices.find(name);
            if (findM == matrices.end())
                throw CException("Matrix \"" + name + "\"doesn't exist.");
            else {
                operands.push_back(CItem((*findM).second));
                if (right != string::npos) operators.push_back(data[right]);
            }
        }
    }
}

void CExpression::solve() {
CONTINUE:
    for (unsigned int i = 0; i < operators.size(); i++) {
        if (operators[i] == '*') {
            if (operands[i].isMatrix && !operands[i + 1].isMatrix)
                throw CException("Cannot multiply matrix * constant.");
            else if (!operands[i].isMatrix && operands[i + 1].isMatrix) {
                AbstractMatrix * mult = operands[i].fract * *operands[i + 1].matrix;
                erase(i);
                operands.insert(operands.begin() + i, CItem(mult));
                delete mult;
                goto CONTINUE;
            } else if (operands[i].isMatrix && operands[i + 1].isMatrix) {
                AbstractMatrix * mult = *operands[i].matrix * operands[i + 1].matrix;
                erase(i);
                operands.insert(operands.begin() + i, CItem(mult));
                delete mult;
                goto CONTINUE;
            }
        } else if (operators[i] == '/') {
            if (!operands[i].isMatrix && !operands[i + 1].isMatrix) {
                CFraction fract = operands[i].fract + operands[i + 1].fract;
                erase(i);
                operands.insert(operands.begin() + i, fract);
                goto CONTINUE;
            } else if (operands[i].isMatrix && operands[i + 1].isMatrix)
                throw CException("Cannot divide 2 matrices.");
            else if (operands[i].isMatrix && !operands[i + 1].isMatrix) {
                AbstractMatrix * div = *operands[i].matrix / operands[i + 1].fract;
                erase(i);
                operands.insert(operands.begin() + i, div);
                delete div;
                goto CONTINUE;
            }
        }
    }
CONTINUE2:
    for (unsigned int i = 0; i < operators.size(); i++) {
        if (operators[i] == '+') {
            if (operands[i].isMatrix && operands[i + 1].isMatrix) {
                AbstractMatrix * mult = *operands[i].matrix + operands[i + 1].matrix;
                erase(i);
                operands.insert(operands.begin() + i, CItem(mult));
                delete mult;
                goto CONTINUE2;
            } else if (!operands[i].isMatrix && !operands[i + 1].isMatrix) {
                CFraction fract = operands[i].fract + operands[i + 1].fract;
                erase(i);
                operands.insert(operands.begin() + i, CItem(fract));
            } else
                throw CException("Cannot sum constant + matrix.");
        }
        if (operators[i] == '-') {
            if (operands[i].isMatrix && operands[i + 1].isMatrix) {
                AbstractMatrix * mult = *operands[i].matrix - operands[i + 1].matrix;
                erase(i);
                operands.insert(operands.begin() + i, CItem(mult));
                delete mult;
                goto CONTINUE2;
            } else if (!operands[i].isMatrix && !operands[i + 1].isMatrix) {
                CFraction fract = operands[i].fract - operands[i + 1].fract;
                erase(i);
                operands.insert(operands.begin() + i, CItem(fract));
            } else
                throw CException("Cannot sum constant + matrix.");
        }
    }
}

void CExpression::showResult() const {
    if (!operands.empty()) {
        cout << "=== R E S U L T ===" << endl;
        if (operands.front().isMatrix)
            cout << operands.front().matrix << endl;

        else
            cout << operands.front().fract << endl;
    }
}

CExpression::CItem CExpression::getResult() const {
    if (!operands.empty())
        return operands.front();

    else
        return NULL;
}

void CExpression::erase(unsigned int i) {
    operands.erase(operands.begin() + i);
    operands.erase(operands.begin() + i);
    operators.erase(operators.begin() + i);
}

void CExpression::insertFraction(const string& name, char oper) {
    CFraction fract(name);

    if (!operands.empty() && !operands.back().isMatrix) {
        if (operators.back() == '*') {
            operands.back().fract = operands.back().fract * fract;
            if (oper == 'X') operators.pop_back();
            else operators.back() = oper;
            return;
        }
        if (operators.back() == '/') {
            operands.back().fract = operands.back().fract / fract;
            if (oper == 'X') operators.pop_back();
            else operators.back() = oper;
            return;
        } else if (operators.back() == '+' && fract < 0) {
            operators.back() = '-';
            operands.push_back(CFraction(fract * -1));
            if (oper != 'X') operators.push_back(oper);
        } else if (operators.back() == '-' && fract < 0) {
            operators.back() = '+';
            operands.push_back(CFraction(fract * -1));
            if (oper != 'X') operators.push_back(oper);
        } else {
            operands.push_back(CFraction(fract));
            if (oper != 'X') operators.push_back(oper);
        }
    } else {
        operands.push_back(CFraction(fract));

        if (oper != 'X') operators.push_back(oper);
    }
}

void CExpression::readInsertMatrix(const string expr, size_t left, size_t& middle, size_t& right) {
    string format;
    if (expr.find_first_not_of(' ', left) != middle) throw CException("Invalid calculation line.");
    left = middle;
    if (right == string::npos) {
        format = string(expr.begin() + left, expr.end());
    } else {
        right = middle;
        while (1) {
            middle = expr.find_first_of('}', right);
            if (middle == string::npos) {
                format = string(expr.begin() + left, expr.end());
                break;
            }
            right = expr.find_first_of('}', middle + 1);
            if (right == string::npos) {
                format = string(expr.begin() + left, expr.end());
                break;
            }
            if (expr.find_first_not_of(' ', middle + 1) == right) {
                right = expr.find_first_of(VALID_OPERATORS, right + 1);
                if (right == string::npos) {
                    format = string(expr.begin() + left, expr.end());
                } else
                    format = string(expr.begin() + left, expr.begin() + right);
                break;
            }
        }
    }
    size_t endFirstRow = format.find_first_of('}');
    if (endFirstRow == string::npos)
        throw CException("Invalid calculation line.");
    size_t columns = count(format.begin(), format.begin() + endFirstRow, ',') + 1;
    size_t rows = count(format.begin(), format.end(), '{') - 1;
    ostringstream number;
    number << cntConsoleMatrix++;
    CBuilder x(rows, columns, "Console" + number.str(), format);
    AbstractMatrix * mat = x.getType();
    operands.push_back(CItem(mat));
    delete mat;

    if (right != string::npos && middle != string::npos) {
        operators.push_back(expr[right]);
    }
}

string CExpression::VALID_OPERATORS = "+-*/";
string CExpression::DETERMINANT = "det";
string CExpression::INVERSEMATRIX = "inv";
string CExpression::RANK = "rank";
string CExpression::TRANSPOSEMATRIX = "trans";