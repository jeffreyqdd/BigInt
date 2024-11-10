#pragma once

#include "aligned_alloc.tpp"

#include <cstdint>
#include <string>
#include <vector>

typedef std::vector<uint64_t, AlignedAllocator<uint64_t>> container;

class BigNumUnderflowException;

/// @brief immutable big num container representing unsigned numbers in little endian format
class BigNum {
public:
	/// @brief construct big num with default value = 0
	BigNum(size_t allocation_size = BigNum::INITIAL_ALLOCATIONS_SIZE);
	BigNum(const std::string& number);

	/// @brief construct big num from string representation
	// BigNum(const std::string& s);

	BigNum operator+(const uint64_t& other) const;
	BigNum operator-(const uint64_t& other) const;
	BigNum operator*(const uint64_t& other) const;
	BigNum operator/(const uint64_t& other) const;
	BigNum operator<<(const uint64_t& other) const;
	BigNum operator>>(const uint64_t& other) const;

	BigNum& operator+=(const uint64_t& other);
	BigNum& operator-=(const uint64_t& other);
	BigNum& operator*=(const uint64_t& other);
	BigNum& operator/=(const uint64_t& other);
	BigNum& operator<<=(const uint64_t& other);
	BigNum& operator>>=(const uint64_t& other);

	BigNum operator+(const BigNum& other) const;
	BigNum operator-(const BigNum& other) const;
	BigNum operator*(const BigNum& other) const;
	BigNum operator/(const BigNum& other) const;
	BigNum operator%(const BigNum& other) const;
	BigNum operator^(const BigNum& other) const;
	BigNum operator<<(const BigNum& other) const;
	BigNum operator>>(const BigNum& other) const;

	BigNum& operator+=(const BigNum& other);
	BigNum& operator-=(const BigNum& other);
	BigNum& operator*=(const BigNum& other);
	BigNum& operator/=(const BigNum& other);
	BigNum& operator%=(const BigNum& other);
	BigNum& operator^=(const BigNum& other);
	BigNum& operator<<=(const BigNum& other);
	BigNum& operator>>=(const BigNum& other);

	std::string to_string() const;
	std::string to_bitstring() const;

private:
	static constexpr size_t KARATSUBA_THRESHOLD = 30;
	static constexpr size_t INITIAL_ALLOCATIONS_SIZE = 8;

	size_t leading_digit() const;

	/// @brief use base 2^64 as repr because int64 operations are faster on x86
	// TODO: look into efficient memory allocation
	container _digits;
};