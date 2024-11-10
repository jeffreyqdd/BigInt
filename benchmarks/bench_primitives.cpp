#include <benchmark/benchmark.h>
#include <limits>
#include <random>

template <typename T, typename Operation>
static void integer_multiplication(benchmark::State& state) {
	std::mt19937 gen(42);
	std::uniform_int_distribution<T> dist(1, std::numeric_limits<T>::max());

	volatile T a = dist(gen);
	volatile T b = dist(gen);
	for(auto _ : state) {
		benchmark::DoNotOptimize(Operation()(a, b));
	}
}

template <typename T>
struct Add {
	T operator()(T a, T b) const {
		return a + b;
	}
};

template <typename T>
struct Sub {
	T operator()(T a, T b) const {
		return a - b;
	}
};

template <typename T>
struct Mul {
	T operator()(T a, T b) const {
		return a * b;
	}
};

template <typename T>
struct Div {
	T operator()(T a, T b) const {
		return (b != 0) ? a / b : static_cast<T>(1);
	} // Avoid division by zero
};

BENCHMARK_TEMPLATE(integer_multiplication, uint32_t, Add<uint32_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint32_t, Sub<uint32_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint32_t, Mul<uint32_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint32_t, Div<uint32_t>);

BENCHMARK_TEMPLATE(integer_multiplication, uint64_t, Add<uint64_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint64_t, Sub<uint64_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint64_t, Mul<uint64_t>);
BENCHMARK_TEMPLATE(integer_multiplication, uint64_t, Div<uint64_t>);

BENCHMARK_TEMPLATE(integer_multiplication, __uint128_t, Add<__uint128_t>);
BENCHMARK_TEMPLATE(integer_multiplication, __uint128_t, Sub<__uint128_t>);
BENCHMARK_TEMPLATE(integer_multiplication, __uint128_t, Mul<__uint128_t>);
BENCHMARK_TEMPLATE(integer_multiplication, __uint128_t, Div<__uint128_t>);

BENCHMARK_MAIN();