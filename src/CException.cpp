/** \file CException.cpp
 *  Implementation of CException class
*  \author Petr Panský <panskpe1@fit.cvut.cz> */

#include "CException.h"

CException::CException(const string & message) : message(message) {
}

string CException::getMessage() const {
    return message;
}