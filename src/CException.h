/** \file CException.h
 *  Header file for CException.cpp
 *  \author Petr Panský <panskpe1@fit.cvut.cz> */


#ifndef CEXCEPTION_H
#define	CEXCEPTION_H

#include <string>
using namespace std;

/// Class for throw exception in program's run
class CException {
public:
    /// Constructor with message
    /** \param message Error message */
    CException(const string & message);
    /// Gets message
    /** \return Error message */
    string getMessage() const;
private:
    string message; ///< error message
};

#endif	/* CEXCEPTION_H */