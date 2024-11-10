#pragma once

#include "aligned_alloc.tpp"

#include <cassert>
#include <cstdint>
#include <format>
#include <limits>
#include <memory>
#include <string>
#include <vector>

/// @brief immutable container representing unsigned numbers in little endian format
class UnsignedBigInt {
public:
	// =============================
	// Section: Includes and Imports
	// =============================

	constexpr UnsignedBigInt() noexcept
		: m_digits(1)
		, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) { }

	constexpr UnsignedBigInt(int number) noexcept
		: m_digits(1)
		, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
		assert(number >= 0);
		m_container[0] = number;
	}

	constexpr UnsignedBigInt(uint64_t number) noexcept
		: m_digits(1)
		, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
		// static_assert(sizeof(number) <= HEAP_THRESHOLD * sizeof(base_t));
		m_container[0] = number;
	}

	constexpr UnsignedBigInt(__uint128_t number) noexcept
		: m_digits(number & UPPER_MASK_128 ? 2 : 1)
		, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
		// treat 0th index as pointer to __uint128, setting both at once
		// without masks or shifts
		*reinterpret_cast<__uint128_t*>(m_container.data()) = number;
	}

	/// @brief
	/// @param number
	UnsignedBigInt(const std::string& number);

	/// @brief
	/// @param number
	/// @return
	UnsignedBigInt& operator=(const int& number);

	/// @brief
	/// @param number
	/// @return
	UnsignedBigInt& operator=(const uint64_t& number);

	/// @brief
	/// @param number
	/// @return
	UnsignedBigInt& operator=(const __uint128_t& number);

	/// @brief
	/// @param number
	/// @return
	UnsignedBigInt& operator=(const std::string& number);

	/// @brief
	/// @param number
	/// @return
	UnsignedBigInt& operator=(const UnsignedBigInt& number);

	// =============================
	// Section: Includes and Imports
	// =============================
	UnsignedBigInt operator+(const uint64_t& other) const;
	UnsignedBigInt operator-(const uint64_t& other) const;
	UnsignedBigInt operator*(const uint64_t& other) const;
	// UnsignedBigInt operator/(const uint64_t& other) const;
	UnsignedBigInt operator<<(const uint64_t& other) const;
	UnsignedBigInt operator>>(const uint64_t& other) const;

	UnsignedBigInt& operator+=(const uint64_t& other);
	UnsignedBigInt& operator-=(const uint64_t& other);
	UnsignedBigInt& operator*=(const uint64_t& other);
	// UnsignedBigInt& operator/=(const uint64_t& other);
	UnsignedBigInt& operator<<=(const uint64_t& other);
	UnsignedBigInt& operator>>=(const uint64_t& other);

	// =============================
	// Section: Includes and Imports
	// =============================
	UnsignedBigInt operator+(const UnsignedBigInt& other) const;
	UnsignedBigInt operator-(const UnsignedBigInt& other) const;
	UnsignedBigInt operator*(const UnsignedBigInt& other) const;
	UnsignedBigInt operator/(const UnsignedBigInt& other) const;
	UnsignedBigInt operator%(const UnsignedBigInt& other) const;
	UnsignedBigInt operator^(const UnsignedBigInt& other) const;
	UnsignedBigInt modulus_exp(const UnsignedBigInt& exp, const UnsignedBigInt& mod) const;
	// UnsignedBigInt operator<<(const UnsignedBigInt& other) const;
	// UnsignedBigInt operator>>(const UnsignedBigInt& other) const;

	UnsignedBigInt& operator+=(const UnsignedBigInt& other);
	UnsignedBigInt& operator-=(const UnsignedBigInt& other);
	UnsignedBigInt& operator*=(const UnsignedBigInt& other);
	UnsignedBigInt& operator/=(const UnsignedBigInt& other);
	UnsignedBigInt& operator%=(const UnsignedBigInt& other);
	UnsignedBigInt& operator^=(const UnsignedBigInt& other);
	UnsignedBigInt& modulus_exp_eq(const UnsignedBigInt& exp, const UnsignedBigInt& mod);
	// UnsignedBigInt& operator<<=(const UnsignedBigInt& other);
	// UnsignedBigInt& operator>>=(const UnsignedBigInt& other);

	// =============================
	// Section: Comparators
	// =============================
	bool operator<(const UnsignedBigInt& other) const;
	bool operator>(const UnsignedBigInt& other) const;
	bool operator<=(const UnsignedBigInt& other) const;
	bool operator>=(const UnsignedBigInt& other) const;
	bool operator==(const UnsignedBigInt& other) const;
	bool operator!=(const UnsignedBigInt& other) const;

	size_t digits() const noexcept;
	size_t most_significant_bit() const noexcept;
	void set_bit(size_t idx, bool on) noexcept;

	std::string to_string() const;
	std::string to_bitstring() const;

	~UnsignedBigInt() = default;

private:
	typedef uint64_t base_t;
	typedef __uint128_t carry_t;
	typedef std::vector<base_t, AlignedAllocator<base_t>> container;

	// class container {
	// 	std::unique_ptr<base_t[]> m_digits;
	// 	size_t m_capacity;
	// };

	inline static constexpr uint64_t MAX_U64_VALUE = std::numeric_limits<uint64_t>::max();
	inline static constexpr uint64_t LOWER_MASK_64 = 0x00000000FFFFFFFFull;
	inline static constexpr uint64_t UPPER_MASK_64 = 0xFFFFFFFF00000000ull;

	inline static constexpr __uint128_t LOWER_MASK_128 =
		static_cast<__uint128_t>(0xFFFFFFFFFFFFFFFF);
	inline static constexpr __uint128_t UPPER_MASK_128 =
		static_cast<__uint128_t>(0xFFFFFFFFFFFFFFFF) << 64;

	// inline static constexpr size_t KARATSUBA_THRESHOLD = 30;
	// inline static constexpr size_t FFT_THRESHOLD = 50000;
	inline static constexpr size_t INITIAL_ALLOCATIONS_SIZE = 8;

	// inline static constexpr size_t HEAP_THRESHOLD = sizeof(container) / sizeof(base_t);
	// inline static constexpr carry_t STORAGE_MASK = std::numeric_limits<base_t>::max();

	size_t m_digits;
	container m_container;
};

class BigNumDivideByZeroException : public std::exception {
	const char* what() const noexcept override {
		return "cannot divide by zero";
	}
};

class BigNumUnderflowException : public std::exception {
private:
	std::string _error_msg;

public:
	explicit BigNumUnderflowException(const UnsignedBigInt& lhs, const uint64_t& rhs) {
		_error_msg = std::format("cannot subtract {} from {}", rhs, lhs.to_string());
	}
	explicit BigNumUnderflowException(const UnsignedBigInt& lhs, const UnsignedBigInt& rhs) {
		_error_msg = std::format("cannot subtract {} from {}", rhs.to_string(), lhs.to_string());
	}

	const char* what() const noexcept override {
		return _error_msg.c_str();
	}
};
