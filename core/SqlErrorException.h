//
// Created by septemberhx on 5/20/18.
//

#ifndef TODOMANAGER_SQLERROREXCEPTION_H
#define TODOMANAGER_SQLERROREXCEPTION_H


#include <stdexcept>

namespace todo {
class SqlErrorException : public std::runtime_error {
public:
    SqlErrorException(const char *msg = "") : runtime_error(msg) { }
    ~SqlErrorException() throw() = default;
};
}

#endif //TODOMANAGER_SQLERROREXCEPTION_H
