#ifndef CS3520_MINIPROJECT_ERRORS
#define CS3520_MINIPROJECT_ERRORS
#include <stdexcept>
#include <string>

class UnknownPiece : public std::logic_error {
public:
    UnknownPiece(const std::string& message) : std::logic_error(message) {}
};
#endif