#ifndef CALCULATOR_ERROR_HPP
#define CALCULATOR_ERROR_HPP

#include <stdexcept>
#include <string>

namespace calculator::math {

// Stable error codes let tests and GUI distinguish domain errors reliably.
enum class ErrorCode {
    DivisionByZero,
    ModuloByZero,
    NonIntegerModulo,
    NegativeFactorial,
    NonIntegerFactorial,
    NegativeSquareRoot,
    ZeroRootDegree,
    NonIntegerRootDegree,
    EvenRootOfNegative,
    InvalidToken,
    InvalidExpression,
    MissingOperand,
    OverflowOrDomainError,
};

// MathError carries a user-facing message together with a machine-checkable code.
class MathError : public std::runtime_error
{
public:
    MathError(ErrorCode code, const std::string &message)
        : std::runtime_error(message)
        , code_(code)
    {
    }

    ErrorCode code() const noexcept
    {
        return code_;
    }

private:
    ErrorCode code_;
};

} // namespace calculator::math

#endif // CALCULATOR_ERROR_HPP