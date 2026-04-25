#include <cstdio>
#include <limits>
#include <vector>

#include "calculator/error.hpp"
#include "calculator/math.hpp"

int main()
{
	using namespace calculator::math;

	std::size_t count = 0;
	double value = 0.0;
	std::vector<double> values;

	try
	{
		while (std::scanf("%lf", &value) == 1)
		{
			values.push_back(value);
		}

		count = values.size();

		if (count < 2)
		{
			std::printf("0\n");
			return 0;
		}

		double sum = 0.0;
		for (double current : values)
		{
			sum = add(sum, current);
		}

		const double mean = divide(sum, static_cast<double>(count));

		double squaredDeviationSum = 0.0;
		for (double current : values)
		{
			const double diff = subtract(current, mean);
			squaredDeviationSum = add(squaredDeviationSum, multiply(diff, diff));
		}

		double variance = divide(squaredDeviationSum, static_cast<double>(count - 1));
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
