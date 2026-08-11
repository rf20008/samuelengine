#ifndef CS3520_MINIPROJECT_ERRORS
#define CS3520_MINIPROJECT_ERRORS
#include <exception>
#include <stdexcept>
#include <string>



class NotImplementedError: public std::logic_error {
    public:
        NotImplementedError(const std::string& message = "This function is not yet implemented") : std::logic_error(message) {}
};


class UnknownPiece : public std::logic_error {
public:
    UnknownPiece(const std::string& message) : std::logic_error(message) {}
};

class InvalidFEN : public std::runtime_error {
    public:
        InvalidFEN(const std::string& message) : std::runtime_error(message) {}
};

class OutOfTime : public std::exception {
    private:
        std::string message;
    public:
        OutOfTime(const std::string& err = "Search stopped") : message(err) {}
        virtual const char* what() const noexcept override {
            return message.c_str();
        }
};
#endif