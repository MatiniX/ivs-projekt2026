#ifndef CALCULATOR_MATH_HPP
#define CALCULATOR_MATH_HPP

#include <string>

#include "calculator/error.hpp"

/**
 * @namespace calculator::math
 * @brief Public API of the calculator math library.
 */
namespace calculator::math {

/**
 * @brief Adds two numbers.
 * @param left First operand.
 * @param right Second operand.
 * @return Sum of both operands.
 */
double add(double left, double right);

/**
 * @brief Subtracts the second number from the first one.
 * @param left Value to subtract from.
 * @param right Value to subtract.
 * @return Difference of both operands.
 */
double subtract(double left, double right);

/**
 * @brief Multiplies two numbers.
 * @param left First operand.
 * @param right Second operand.
 * @return Product of both operands.
 */
double multiply(double left, double right);

/**
 * @brief Divides the first number by the second one.
 * @param left Dividend.
 * @param right Divisor.
 * @return Quotient of both operands.
 * @throw MathError Thrown when the divisor is zero.
 */
double divide(double left, double right);

/**
 * @brief Computes the remainder after integer division.
 * @param left Dividend.
 * @param right Divisor.
 * @return Remainder of left divided by right.
 * @throw MathError Thrown when the divisor is zero.
 * @note This operation is defined only for integer operands.
 */
double modulo(long long left, long long right);

/**
 * @brief Raises a base to the given exponent.
 * @param base Base of the power operation.
 * @param exponent Exponent of the power operation.
 * @return Result of base raised to exponent.
 * @throw MathError Thrown when the result is outside the supported domain.
 */
double power(double base, double exponent);

/**
 * @brief Computes the square root of a number.
 * @param value Input value.
 * @return Square root of the input value.
 * @throw MathError Thrown when the input value is negative.
 */
double squareRoot(double value);

/**
 * @brief Computes the n-th root of a number.
 * @param value Radicand of the root operation.
 * @param degree Degree of the root.
 * @return N-th root of the input value.
 * @throw MathError Thrown when the degree is zero or the operation is outside the supported domain.
 */
double nthRoot(double value, long long degree);

/**
 * @brief Computes the factorial of a non-negative integer.
 * @param value Input value.
 * @return Factorial of the input value.
 * @throw MathError Thrown when the input value is negative or the result overflows the supported range.
 */
double factorial(long long value);

/**
 * @brief Evaluates a mathematical expression.
 * @param expression Expression to evaluate.
 * @return Numerical result of the expression.
 * @throw MathError Thrown when the expression is invalid or contains a domain error.
 * @note Supports operators +, -, *, /, %, ^, postfix factorial, square root and n-th root.
 */
double evaluateExpression(const std::string &expression);

} // namespace calculator::math

#endif // CALCULATOR_MATH_HPP