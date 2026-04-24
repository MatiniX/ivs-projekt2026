#include <cstdio>
#include <limits>

#include "calculator/error.hpp"
#include "calculator/math.hpp"

int main()
{
	using namespace calculator::math;

	std::size_t count = 0;
	double mean = 0.0;
	double m2 = 0.0;
	double value = 0.0;

	try
	{
		while (std::scanf("%lf", &value) == 1)
		{
			++count;

			const double n = static_cast<double>(count);
			const double delta = subtract(value, mean);
			mean = add(mean, divide(delta, n));
			const double delta2 = subtract(value, mean);
			m2 = add(m2, multiply(delta, delta2));
		}

		if (count < 2)
		{
			std::printf("0\n");
			return 0;
		}

		double variance = divide(m2, static_cast<double>(count - 1));
		if (variance < 0.0 && variance > -std::numeric_limits<double>::epsilon())
		{
			variance = 0.0;
		}

		const double stddev = squareRoot(variance);
		std::printf("%.15g\n", stddev);
		return 0;
	}
	catch (const MathError &error)
	{
		std::fprintf(stderr, "stddev error: %s\n", error.what());
		return 1;
	}
}
