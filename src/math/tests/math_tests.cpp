#include "calculator/math.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

using calculator::math::ErrorCode;
using calculator::math::MathError;

struct TestCase {
    std::string name;
    std::function<void()> body;
};

void assertNear(double actual, double expected, const char *message)
{
    if (std::fabs(actual - expected) > 1e-9) {
        throw std::runtime_error(
            std::string(message) + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual));
    }
}

void assertRelativeNear(double actual, double expected, double relativeTolerance, const char *message)
{
    const double scale = std::max(std::fabs(expected), 1.0);
    if (std::fabs(actual - expected) > relativeTolerance * scale) {
        throw std::runtime_error(
            std::string(message) + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual));
    }
}

void assertError(const std::function<void()> &callable, ErrorCode expectedCode, const char *message)
{
    try {
        callable();
    } catch (const MathError &error) {
        if (error.code() != expectedCode) {
            throw std::runtime_error(std::string(message) + ": wrong error code");
        }

        return;
    }

    throw std::runtime_error(std::string(message) + ": exception was not thrown");
}

} // namespace

int main()
{
    using namespace calculator::math;

    const std::vector<TestCase> tests = {
        {"add", [] { assertNear(add(2.5, 1.5), 4.0, "add"); }},
        {"subtract", [] { assertNear(subtract(10.0, 3.5), 6.5, "subtract"); }},
        {"multiply", [] { assertNear(multiply(2.0, -4.0), -8.0, "multiply"); }},
        {"divide", [] { assertNear(divide(9.0, 3.0), 3.0, "divide"); }},
        {"modulo", [] { assertNear(modulo(17, 5), 2.0, "modulo"); }},
        {"power", [] { assertNear(power(2.0, 5.0), 32.0, "power"); }},
        {"squareRoot", [] { assertNear(squareRoot(81.0), 9.0, "squareRoot"); }},
        {"nthRoot", [] { assertNear(nthRoot(27.0, 3), 3.0, "nthRoot"); }},
        {"factorial", [] { assertNear(factorial(6), 720.0, "factorial"); }},
        {"factorial zero boundary", [] { assertNear(factorial(0), 1.0, "factorial zero boundary"); }},
        {"factorial max supported boundary", [] {
            assertRelativeNear(factorial(170), 7.257415615307999e306, 1e-12, "factorial max supported boundary");
        }},
        {"nthRoot negative odd radicand", [] { assertNear(nthRoot(-27.0, 3), -3.0, "nthRoot negative odd radicand"); }},
        {"nthRoot negative degree", [] { assertNear(nthRoot(16.0, -2), 0.25, "nthRoot negative degree"); }},
        {"whitespace in evaluator", [] {
            assertNear(evaluateExpression("  sqrt(16) +  2 * 3  "), 10.0, "whitespace in evaluator");
        }},
        {"unary minus in evaluator", [] { assertNear(evaluateExpression("-3+5"), 2.0, "unary minus in evaluator"); }},
        {"readable modulo token", [] { assertNear(evaluateExpression("17 mod 5"), 2.0, "readable modulo token"); }},
        {"legacy power token", [] { assertNear(evaluateExpression("2r3"), 8.0, "legacy power token"); }},
        {"evaluate precedence", [] { assertNear(evaluateExpression("2+3*4"), 14.0, "evaluate precedence"); }},
        {"evaluate gui power", [] { assertNear(evaluateExpression("2^3"), 8.0, "evaluate gui power"); }},
        {"evaluate gui nth root", [] { assertNear(evaluateExpression("3n27"), 3.0, "evaluate gui nth root"); }},
        {"evaluate postfix factorial", [] { assertNear(evaluateExpression("5!"), 120.0, "evaluate postfix factorial"); }},
        {"evaluate prefix sqrt", [] { assertNear(evaluateExpression("s9"), 3.0, "evaluate prefix sqrt"); }},
        {"evaluate readable nroot", [] { assertNear(evaluateExpression("nroot(3,27)"), 3.0, "evaluate readable nroot"); }},
        {"evaluate parentheses", [] { assertNear(evaluateExpression("(2+3)*4"), 20.0, "evaluate parentheses"); }},
        {"divide by zero error", [] {
            assertError([] { (void)divide(1.0, 0.0); }, ErrorCode::DivisionByZero, "divide by zero");
        }},
        {"modulo by zero error", [] {
            assertError([] { (void)modulo(4, 0); }, ErrorCode::ModuloByZero, "modulo by zero");
        }},
        {"power domain error", [] {
            assertError([] { (void)power(-1.0, 0.5); }, ErrorCode::OverflowOrDomainError, "power domain error");
        }},
        {"negative square root error", [] {
            assertError([] { (void)squareRoot(-1.0); }, ErrorCode::NegativeSquareRoot, "negative square root");
        }},
        {"zero root degree error", [] {
            assertError([] { (void)nthRoot(16.0, 0); }, ErrorCode::ZeroRootDegree, "zero root degree");
        }},
        {"even root of negative error", [] {
            assertError([] { (void)nthRoot(-16.0, 2); }, ErrorCode::EvenRootOfNegative, "even root of negative");
        }},
        {"negative degree root of zero error", [] {
            assertError([] { (void)nthRoot(0.0, -3); }, ErrorCode::OverflowOrDomainError, "negative degree root of zero");
        }},
        {"negative factorial error", [] {
            assertError([] { (void)factorial(-1); }, ErrorCode::NegativeFactorial, "negative factorial");
        }},
        {"factorial overflow error", [] {
            assertError([] { (void)factorial(171); }, ErrorCode::OverflowOrDomainError, "factorial overflow");
        }},
        {"non integer modulo in evaluator", [] {
            assertError([] { (void)evaluateExpression("5.5%2"); }, ErrorCode::NonIntegerModulo, "non integer modulo");
        }},
        {"non integer factorial in evaluator", [] {
            assertError([] { (void)evaluateExpression("3.5!"); }, ErrorCode::NonIntegerFactorial, "non integer factorial");
        }},
        {"non integer root degree in evaluator", [] {
            assertError([] { (void)evaluateExpression("nroot(2.5,32)"); }, ErrorCode::NonIntegerRootDegree, "non integer root degree");
        }},
        {"invalid expression error", [] {
            assertError([] { (void)evaluateExpression("2+"); }, ErrorCode::MissingOperand, "missing operand");
        }},
        {"invalid token error", [] {
            assertError([] { (void)evaluateExpression("2a3"); }, ErrorCode::InvalidToken, "invalid token");
        }},
        {"empty expression error", [] {
            assertError([] { (void)evaluateExpression(""); }, ErrorCode::InvalidExpression, "empty expression");
        }},
        {"negative grouped factorial error", [] {
            assertError([] { (void)evaluateExpression("(-1)!"); }, ErrorCode::NegativeFactorial, "negative grouped factorial");
        }},
        {"prefix sqrt over negative expression", [] {
            assertError([] { (void)evaluateExpression("s-9"); }, ErrorCode::NegativeSquareRoot, "prefix sqrt over negative expression");
        }},
    };

    std::size_t failures = 0;

    for (const auto &test : tests) {
        try {
            test.body();
        } catch (const std::exception &error) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << ": " << error.what() << '\n';
        }
    }

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All " << tests.size() << " math tests passed\n";
    return 0;
}