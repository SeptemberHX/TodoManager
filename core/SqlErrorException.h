//
// Created by septemberhx on 5/20/18.
//

#ifndef TODOMANAGER_SQLERROREXCEPTION_H
#define TODOMANAGER_SQLERROREXCEPTION_H


#include <QException>

namespace todo {
class SqlErrorException : public QException {
public:
    void raise() const { throw *this; }

    SqlErrorException *clone() const { return new SqlErrorException(*this); }
};
}

#endif //TODOMANAGER_SQLERROREXCEPTION_H
