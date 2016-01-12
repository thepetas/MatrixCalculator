/** \file CCalculator.cpp
 *  Implementation of CCalculator class
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CCalculator.h"

CCalculator::CCalculator(const string & file) {
    saveFile = file;
    if (saveFile.empty()) isSaveFile = false;
    else {
        try {
            ImportFile(saveFile);
            isSaveFile = true;
        } catch (CException & ex) {
            cout << ex.getMessage() << endl;
            isSaveFile = false;
        }
    }
}

CCalculator::~CCalculator() {
    if (isSaveFile)
        saveToFile(saveFile);
    for (auto it : matrices)
        delete it.second;
}

void CCalculator::addMatrix() {
    string name;
    try {
        name = readName();
    } catch (CException & ex) {
        cout << ex.getMessage() << endl;
        return;
    }

    if (matrices.find(name) != matrices.end()) {
        cout << "Name \"" + name + "\" already exists.";
        return;
    }

    int rows, cols;
    cout << "Enter number of rows and columns" << endl;
    cin >> rows;
    if (cin.bad() || rows < 1) {
        cout << "Invalid rows number." << endl;
        return;
    }
    cin >> cols;
    if (cin.bad() || cols < 1) {
        cout << "Invalid columns number." << endl;
        return;
    }
    cin.get();
    cout << "Enter matrix in format {{1,1},{5,5}}" << endl;
    string data;
    getline(cin, data);
    try {
        CBuilder x(rows, cols, name, data);
        matrices.insert(make_pair(name, x.getType()));
        cout << "Matrix \"" << name << "\" added." << endl;
    } catch (CException & ex) {
        cout << "This matrix has invalid format." << endl;
        //        cout << ex.getMessage() << endl;
    }
}

void CCalculator::listAll() const {
    if (matrices.empty())
        cout << "Database is empty." << endl;
    else {
        cout << "Total: " << matrices.size() << endl;

        for (auto it : matrices) {
            cout << it.second->getName() << " " << it.second->getRows() << "x" << it.second->getColumns() <<
                    ", type: " << it.second->getNameOfType() << endl;
        }
    }
}

void CCalculator::deleteMatrix() {
    string name;
    try {
        name = readName();
    } catch (CException & ex) {
        cout << ex.getMessage() << endl;
        return;
    }
    auto it = matrices.find(name);
    if (it == matrices.end()) {
        cout << "Matrix \"" << name << "\" doesn't exist." << endl;
        return;
    }
    delete (*it).second;
    matrices.erase(it);
    cout << "Matrix \"" << name << "\" was deleted." << endl;
}

void CCalculator::printMatrix() const {
    string name;
    try {
        name = readName();
    } catch (CException & ex) {
        cout << ex.getMessage() << endl;
        return;
    }
    auto it = matrices.find(name);
    if (it == matrices.end()) {
        cout << "Matrix \"" + name + "\" doesn't exist." << endl;
        return;
    }
    cout << (*it).second;
    return;
}

void CCalculator::gaussElim() const {
    cout << "Enter name of matrix:" << endl;
    string name;
    getline(cin, name);
    if (cin.bad() || name.length() == 0 || name.find_last_of(AbstractMatrix::INVALIDCHARACTERS) != string::npos) {
        cout << "Invalid name. Name needn't contain this characters \"" + AbstractMatrix::INVALIDCHARACTERS + "\"." << endl;
        return;
    }
    auto it = matrices.find(name);
    if (it == matrices.end()) {
        cout << "Matrix \"" + name + "\" doesn't exist." << endl;
        return;
    } else {
        AbstractMatrix * mat = (*it).second->clone();
        mat->gaussianElimination(cout, true);
        delete mat;
    }
}

void CCalculator::saveToFile(const string & nameFile) const {
    ofstream ofs(nameFile);
    if (ofs.fail()) {
        ofs.close();
        cout << "File \"" + nameFile + "\" wasn't created. Permision denied." << endl;
        return;
    }
    for (auto it : matrices) {
        ofs << it.second->fileFormat() << endl;
    }
    ofs.close();
    cout << "All matrices was saved to \"" + nameFile + "\"." << endl;
}

void CCalculator::ImportFile(const string& fileName) {
    ifstream ifs(fileName);
    if (ifs.fail()) {
        ifs.close();
        throw CException("File \"" + fileName + "\" doesn't exist.");
        return;
    }
    string name;

CONTINUE:
    while (getline(ifs, name, '[')) {
        if (matrices.find(name) != matrices.end()) {
            cout << "\"" + name + "\" - match - existing name." << endl;
            getline(ifs, name);
            goto CONTINUE;
        }
        int row, col;
        char comma, end;
        ifs >> row >> comma >> col >> end;
        if (ifs.bad() || name.empty() || comma != ',' || row < 1 || col < 1 || end != ']'
                || name.find_first_of(AbstractMatrix::INVALIDCHARACTERS) != string::npos || (name[0] <= '9' && name[0] >= '0')) {
            cout << "\"" << name << "\" - invalid format." << endl;
            getline(ifs, name);
            goto CONTINUE;
        }
        string data;
        getline(ifs, data);
        if (ifs.bad()) {
            cout << "\"" << name << "\" - invalid format." << endl;
            getline(ifs, name);
            goto CONTINUE;
        }
        try {
            CBuilder x(row, col, name, data);
            matrices.insert(make_pair(name, x.getType()));
            cout << "\"" << name << "\" - imported." << endl;
        } catch (CException & ex) {
            cout << ex.getMessage() << endl;
        }
    }
    ifs.close();
}

void CCalculator::printMenu() const {
    cout << banner << endl;
    cout << "Menu:\n";
    for (int i = 0; i < MENU_ITEMS; i++) {
        cout << "   * " << i << " - ";
        cout << setw(menuArray[i].length()) << menuArray[i] << endl;
    }
    cout << "Enter number of operation:\n";
}

void CCalculator::importFromFile() {
    cout << "Enter path of file with matrices:" << endl;
    string name;
    getline(cin, name);
    if (cin.bad()) {
        cout << "Invalid name." << endl;
        return;
    }
    try {
        ImportFile(name);
    } catch (CException & ex) {
        cout << ex.getMessage() << endl;
    }
}

string CCalculator::readName() const {
    cout << "Enter name of matrix:" << endl;
    string name;
    getline(cin, name);
    if (cin.bad() || name.empty() || name.find_first_of(AbstractMatrix::INVALIDCHARACTERS) != string::npos) {
        throw CException("Invalid name. Name needn't contain this characters \"" + AbstractMatrix::INVALIDCHARACTERS + "\".");
    }
    if (name[0] <= '9' && name[0] >= '0')
        throw CException("Invalid name. First character needn't be number.");
    return name;
}

void CCalculator::calculate() {
    for (int i = 0; i < CALC_MENU_ITEMS; i++) {
        if (i + 1 == CALC_MENU_ITEMS)  cout << calcMenu[0] << endl;
        cout << calcMenu[i] << endl;
    }
    string line;
    getline(cin, line);
    if (cin.bad()) {
        cout << "Invalid format of calculation line." << endl;
        return;
    }
    try {
        CExpression expr(line, matrices);
        expr.showResult();
        if (expr.getResult().isMatrix) {
            cout << "Do you want to save this matrix? [Y/n]" << endl;
            char reply;
            cin.get(reply);
            getchar();
            if (reply == 'Y') {
                string name = readName();
                AbstractMatrix * matrix = expr.getResult().matrix->clone();
                matrix->setName(name);
                matrices.insert(make_pair(name, matrix));
                cout << "Matrix \"" << name << "\" was added." << endl;
            } else
                return;
        }

    } catch (CException & ex) {
        cout << ex.getMessage() << endl;
    }
}

void CCalculator::runApp() {
    while (1) {
        string operation;
        int num = -1;
        printMenu();
        getline(cin, operation);
        if (!cin.good()) goto error;
        if (operation.length() == 1) num = operation[0] - 48;
        if (num == 0) {
            cout << "Do you really want to quit? [Y/n]" << endl;
            char quit;
            cin.get(quit);
            if (quit == 'Y') {
                if (!isSaveFile) {
                    getchar();
                    cout << "You haven't file to save, enter name of file/path:" << endl;
                    getline(cin, saveFile);
                    if (cin.bad() || saveFile.empty()) {
                        cout << "Invalid name/ path of file." << endl;
                    } else
                        isSaveFile = true;
                }
                return;
            }
        } else if (num == 1)
            addMatrix();
        else if (num == 2)
            deleteMatrix();
        else if (num == 3)
            listAll();
        else if (num == 4)
            printMatrix();
        else if (num == 5)
            gaussElim();
        else if (num == 6)
            importFromFile();
        else if (num == 7) {
            calculate();
        } else {
error:
            cout << "unknown num of operation." << endl;
        }

        string clear;
        cout << "Press enter..." << endl;
        getline(cin, clear);
    }
}


string CCalculator::banner = "------------------------ M A T R I X   C A L C U L A T O R ---------------------";
string CCalculator::menuArray[] = {"End of program", "Add matrix", "Delete matrix", "List all matrices with basic information",
    "Print matrix", "Gaussian elimination", "Import from file", "Calculate with matrix"};
string CCalculator::calcMenu[] = {"--------------------------------------------------------------------------------",
    "A is matrix", "trans[A] = transpose matrix of A        det[A] = determinant of A",
    "inv[A] = inverse matrix A              rank[A] = rank of A",
    "You can enter matrix into terminal:", "\"rank[{{1,2},{3,4}}]\" or \"{{1,2},{3,4}} * A\"", "Please enter expression:"}; 