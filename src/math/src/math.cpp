#include "calculator/math.hpp"

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>
#include <utility>

namespace calculator::math
{

    namespace
    {

        // tolerance keep integer-only operations predictable for values parsed from doubles.
        constexpr double kIntegerTolerance = 1e-9;

        bool isIntegerValue(double value)
        {
            if (!std::isfinite(value))
            {
                return false;
            }

            const double rounded = std::round(value);
            return std::fabs(value - rounded) <= kIntegerTolerance;
        }

        long long requireInteger(double value, ErrorCode code, const char *message)
        {
            if (!isIntegerValue(value))
            {
                throw MathError(code, message);
            }

            const double rounded = std::round(value);
            if (rounded < static_cast<double>(std::numeric_limits<long long>::min()) || rounded > static_cast<double>(std::numeric_limits<long long>::max()))
            {
                throw MathError(ErrorCode::OverflowOrDomainError, "Value is outside integer range");
            }

            return static_cast<long long>(rounded);
        }

        double validateFinite(double value, const char *message)
        {
            if (!std::isfinite(value))
            {
                throw MathError(ErrorCode::OverflowOrDomainError, message);
            }

            return value;
        }

    class Parser
        {
        public:
            explicit Parser(std::string expression)
                : expression_(std::move(expression))
            {
            }

            double parse()
            {
                const double value = parseExpression();
                skipWhitespace();

                if (!isAtEnd())
                {
                    throw MathError(ErrorCode::InvalidToken, "Unexpected token in expression");
                }

                return value;
            }

        private:
            // Recursiv parser with precedence: unary/postfix > power/root > term > expression.
            double parseExpression()
            {
                double value = parseTerm();

                while (true)
                {
                    skipWhitespace();

                    if (match('+'))
                    {
                        value = add(value, parseTerm());
                        continue;
                    }

                    if (match('-'))
                    {
                        value = subtract(value, parseTerm());
                        continue;
                    }

                    return value;
                }
            }

            double parseTerm()
            {
                double value = parsePowerAndRoot();

                while (true)
                {
                    skipWhitespace();

                    if (match('*'))
                    {
                        value = multiply(value, parsePowerAndRoot());
                        continue;
                    }

                    if (match('/'))
                    {
                        value = divide(value, parsePowerAndRoot());
                        continue;
                    }

                    if (match('%') || matchKeyword("mod"))
                    {
                        const double rhs = parsePowerAndRoot();
                        value = modulo(
                            requireInteger(value, ErrorCode::NonIntegerModulo, "Modulo operands must be integers"),
                            requireInteger(rhs, ErrorCode::NonIntegerModulo, "Modulo operands must be integers"));
                        continue;
                    }

                    return value;
                }
            }

            double parsePowerAndRoot()
            {
                double value = parseUnary();

                while (true)
                {
                    skipWhitespace();

                    if (match('^') || match('r'))
                    {
                        value = power(value, parseUnary());
                        continue;
                    }

                    if (match('n'))
                    {
                        // GUI stores nth-root as degree n value -napr(3n27)
                        const double radicand = parseUnary();
                        value = nthRoot(
                            radicand,
                            requireInteger(value, ErrorCode::NonIntegerRootDegree, "Root degree must be an integer"));
                        continue;
                    }

                    return value;
                }
            }

            double parseUnary()
            {
                skipWhitespace();

                if (match('+'))
                {
                    return parseUnary();
                }

                if (match('-'))
                {
                    return -parseUnary();
                }

                if (matchKeyword("sqrt") || match('s'))
                {
                    return squareRoot(parseUnary());
                }

                if (matchKeyword("nroot"))
                {
                    expect('(', "Expected '(' after nroot");
                    const double degree = parseExpression();
                    expect(',', "Expected ',' between nroot arguments");
                    const double value = parseExpression();
                    expect(')', "Expected ')' after nroot arguments");

                    return nthRoot(
                        value,
                        requireInteger(degree, ErrorCode::NonIntegerRootDegree, "Root degree must be an integer"));
                }

                return parsePostfix();
            }

            double parsePostfix()
            {
                double value = parsePrimary();

                while (true)
                {
                    skipWhitespace();

                    if (match('!') || match('f'))
                    {
                        value = factorial(requireInteger(value, ErrorCode::NonIntegerFactorial, "Factorial requires a whole number"));
                        continue;
                    }

                    return value;
                }
            }

            double parsePrimary()
            {
                skipWhitespace();

                if (match('('))
                {
                    const double value = parseExpression();
                    expect(')', "Expected ')' after expression");
                    return value;
                }

                return parseNumber();
            }

            double parseNumber()
            {
                skipWhitespace();

                const char *begin = expression_.c_str() + position_;
                char *end = nullptr;

                errno = 0;
                const double value = std::strtod(begin, &end);

                if (begin == end)
                {
                    throw MathError(ErrorCode::MissingOperand, "Expected a number");
                }

                if (errno == ERANGE)
                {
                    throw MathError(ErrorCode::OverflowOrDomainError, "Number is outside supported range");
                }

                position_ += static_cast<std::size_t>(end - begin);
                return value;
            }

            bool match(char token)
            {
                skipWhitespace();

                if (isAtEnd() || expression_[position_] != token)
                {
                    return false;
                }

                ++position_;
                return true;
            }

            bool matchKeyword(const char *keyword)
            {
                skipWhitespace();

                const std::string text(keyword);
                if (expression_.compare(position_, text.size(), text) != 0)
                {
                    return false;
                }

                position_ += text.size();
                return true;
            }

            void expect(char token, const char *message)
            {
                if (!match(token))
                {
                    throw MathError(ErrorCode::InvalidExpression, message);
                }
            }

            void skipWhitespace()
            {
                while (!isAtEnd() && std::isspace(static_cast<unsigned char>(expression_[position_])))
                {
                    ++position_;
                }
            }

            bool isAtEnd() const
            {
                return position_ >= expression_.size();
            }

            std::string expression_;
            std::size_t position_ = 0;
        };

    } // namespace

    double add(double left, double right)
    {
        return left + right;
    }

    double subtract(double left, double right)
    {
        return left - right;
    }

    double multiply(double left, double right)
    {
        return left * right;
    }

    double divide(double left, double right)
    {
        if (std::fabs(right) <= kIntegerTolerance)
        {
            throw MathError(ErrorCode::DivisionByZero, "Division by zero");
        }

        return left / right;
    }

    double modulo(long long left, long long right)
    {
        if (right == 0)
        {
            throw MathError(ErrorCode::ModuloByZero, "Modulo by zero");
        }

        return static_cast<double>(left % right);
    }

    double power(double base, double exponent)
    {
        return validateFinite(std::pow(base, exponent), "Invalid power operation");
    }

    double squareRoot(double value)
    {
        if (value < 0.0)
        {
            throw MathError(ErrorCode::NegativeSquareRoot, "Square root of a negative number is undefined");
        }

        return std::sqrt(value);
    }

    double nthRoot(double value, long long degree)
    {
        if (degree == 0)
        {
            throw MathError(ErrorCode::ZeroRootDegree, "Root degree cannot be zero");
        }

        if (value < 0.0 && degree % 2 == 0)
        {
            throw MathError(ErrorCode::EvenRootOfNegative, "Even root of a negative number is undefined");
        }

        const long long absDegree = std::llabs(degree);
        double result = std::pow(std::fabs(value), 1.0 / static_cast<double>(absDegree));
        result = validateFinite(result, "Invalid root operation");

        if (value < 0.0)
        {
            result = -result;
        }

        if (degree < 0)
        {
            if (std::fabs(result) <= kIntegerTolerance)
            {
                throw MathError(ErrorCode::OverflowOrDomainError, "Root with negative degree is undefined for zero");
            }

            result = divide(1.0, result);
        }

        return result;
    }

    double factorial(long long value)
    {
        if (value < 0)
        {
            throw MathError(ErrorCode::NegativeFactorial, "Factorial of a negative number is undefined");
        }

        if (value > 170)
        {
            throw MathError(ErrorCode::OverflowOrDomainError, "Factorial result is outside supported range");
        }

        double result = 1.0;
        // Iterative factorial avoids recursion depth and keeps overflow handling explicit.
        for (long long current = 2; current <= value; ++current)
        {
            result *= static_cast<double>(current);
        }

        return result;
    }

    double evaluateExpression(const std::string &expression)
    {
        if (expression.empty())
        {
            throw MathError(ErrorCode::InvalidExpression, "Expression cannot be empty");
        }

        return Parser(expression).parse();
    }

} // namespace calculator::math