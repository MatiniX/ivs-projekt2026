#ifndef CALCULATOR_ERROR_HPP
#define CALCULATOR_ERROR_HPP

#include <stdexcept>
#include <string>

/**
 * @namespace calculator::math
 * @brief Error types used by the calculator math library.
 */
namespace calculator::math {

/**
 * @brief Enumerates user-visible error conditions reported by the math library.
 */
enum class ErrorCode {
    DivisionByZero,        ///< Attempt to divide by zero.
    ModuloByZero,          ///< Attempt to compute modulo with zero divisor.
    NonIntegerModulo,      ///< Modulo was requested for non-integer operands.
    NegativeFactorial,     ///< Factorial was requested for a negative value.
    NonIntegerFactorial,   ///< Factorial was requested for a non-integer value.
    NegativeSquareRoot,    ///< Square root was requested for a negative value.
    ZeroRootDegree,        ///< N-th root was requested with degree zero.
    NonIntegerRootDegree,  ///< N-th root degree was not an integer.
    EvenRootOfNegative,    ///< Even root was requested for a negative value.
    InvalidToken,          ///< Expression contains an unsupported token.
    InvalidExpression,     ///< Expression syntax is malformed.
    MissingOperand,        ///< Expression is missing a required operand.
    OverflowOrDomainError, ///< Numeric result is outside the supported domain or range.
};

/**
 * @brief Exception type thrown by the math library.
 * @details Combines a readable message with a stable error code that can be checked in tests or GUI logic.
 */
class MathError : public std::runtime_error
{
public:
    /**
     * @brief Constructs a math error.
     * @param code Stable error code.
     * @param message Readable description of the error.
     */
    MathError(ErrorCode code, const std::string &message)
        : std::runtime_error(message)
        , code_(code)
    {
    }

    /**
     * @brief Returns the associated error code.
     * @return Stable error code describing the failure.
     */
    ErrorCode code() const noexcept
    {
        return code_;
    }

private:
    ErrorCode code_;
};

} // namespace calculator::math

#endif // CALCULATOR_ERROR_HPP