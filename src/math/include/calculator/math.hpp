#ifndef CALCULATOR_MATH_HPP
#define CALCULATOR_MATH_HPP

#include <string>

#include "calculator/error.hpp"

namespace calculator::math {

// Primitive operations stay public so evaluator logic can be tested in small units.
double add(double left, double right);
double subtract(double left, double right);
double multiply(double left, double right);
double divide(double left, double right);
double modulo(long long left, long long right);
double power(double base, double exponent);
double squareRoot(double value);
double nthRoot(double value, long long degree);
double factorial(long long value);

// GUI should call this entry point instead of reimplementing expression parsing.
double evaluateExpression(const std::string &expression);

} // namespace calculator::math

#endif // CALCULATOR_MATH_HPP