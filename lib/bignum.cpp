#include "include/bignum.hpp"

#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>
#include <format>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

class BigNumUnderflowException : public std::exception {
private:
	std::string _error_msg;

public:
	explicit BigNumUnderflowException(const UnsignedBigInt& lhs, const uint64_t& rhs) {
		_error_msg = std::format("cannot subtract {} from {}", rhs, lhs.to_string());
	}

	const char* what() const noexcept override {
		return _error_msg.c_str();
	}
};

// ============================================================================
// Section: Constructors
// ============================================================================

constexpr UnsignedBigInt::UnsignedBigInt() noexcept
	: m_digits(1)
	, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) { }

constexpr UnsignedBigInt::UnsignedBigInt(int number) noexcept
	: m_digits(1)
	, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
	// static_assert(sizeof(number) <= HEAP_THRESHOLD * sizeof(base_t));
	assert(number > 0);
	m_container[0] = number;
}

constexpr UnsignedBigInt::UnsignedBigInt(uint64_t number) noexcept
	: m_digits(1)
	, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
	// static_assert(sizeof(number) <= HEAP_THRESHOLD * sizeof(base_t));
	m_container[0] = number;
}

constexpr UnsignedBigInt::UnsignedBigInt(__uint128_t number) noexcept
	: m_digits(number & UPPER_MASK_128 ? 2 : 1)
	, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
	// static_assert(sizeof(number) <= HEAP_THRESHOLD * sizeof(base_t));

	// treat 0th index as pointer to __uint128, setting both at once
	// without masks or shifts
	*reinterpret_cast<__uint128_t*>(m_container.data()) = number;
}

UnsignedBigInt::UnsignedBigInt(const std::string& number)
	: m_digits(1)
	, m_container(UnsignedBigInt::INITIAL_ALLOCATIONS_SIZE, 0) {
	static constexpr size_t LEN = 18;
	static constexpr uint64_t BASE = 1000000000000000000ULL;

	std::string_view slice{ number };

	while(slice.length() >= LEN) {
		std::string sliced_number{ slice.substr(0, LEN) };
		slice.remove_prefix(LEN);
		uint64_t num = std::strtoull(sliced_number.c_str(), nullptr, 10);
		*this *= BASE;
		*this += num;
	}

	if(slice.length()) {
		uint64_t number = std::stoull(slice.data());
		*this *= std::pow(10ull, slice.length());
		*this += number;
	}
}
// ============================================================================
// Section: Assignment Operators
// ============================================================================
UnsignedBigInt& UnsignedBigInt::operator=(const int& value) {
	*this = UnsignedBigInt(value);
	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator=(const uint64_t& value) {
	*this = UnsignedBigInt(value);
	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator=(const __uint128_t& value) {
	*this = UnsignedBigInt(value);
	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator=(const std::string& number) {
	*this = UnsignedBigInt(number);
	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator=(const UnsignedBigInt& bignum) {
	if(this != &bignum) {
		m_container = bignum.m_container;
		m_digits = bignum.m_digits;
	}
	return *this;
}

// ============================================================================
// Section: algebraic operations
// ============================================================================
template <typename T>
inline UnsignedBigInt apply_binary_op(const UnsignedBigInt& lhs,
									  const T& rhs,
									  UnsignedBigInt& (UnsignedBigInt::*op)(const T&)) {
	UnsignedBigInt ret = lhs;
	(ret.*op)(rhs);
	return ret;
}

UnsignedBigInt UnsignedBigInt::operator+(const uint64_t& other) const {
	return apply_binary_op(*this, other, &UnsignedBigInt::operator+=);
}
UnsignedBigInt UnsignedBigInt::operator-(const uint64_t& other) const {
	return apply_binary_op(*this, other, &UnsignedBigInt::operator-=);
}
UnsignedBigInt UnsignedBigInt::operator*(const uint64_t& other) const {
	return apply_binary_op(*this, other, &UnsignedBigInt::operator*=);
}
// UnsignedBigInt UnsignedBigInt::operator/(const uint64_t& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator/=);
// }
// UnsignedBigInt UnsignedBigInt::operator<<(const uint64_t& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator<<=);
// }
// UnsignedBigInt UnsignedBigInt::operator>>(const uint64_t& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator>>=);
// }

UnsignedBigInt UnsignedBigInt::operator+(const UnsignedBigInt& other) const {
	return apply_binary_op(*this, other, &UnsignedBigInt::operator+=);
}
// UnsignedBigInt UnsignedBigInt::operator-(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator-=);
// }
// UnsignedBigInt UnsignedBigInt::operator*(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator*=);
// }
// UnsignedBigInt UnsignedBigInt::operator/(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator/=);
// }
// UnsignedBigInt UnsignedBigInt::operator<<(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator<<=);
// }
// UnsignedBigInt UnsignedBigInt::operator>>(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator>>=);
// }
// UnsignedBigInt UnsignedBigInt::operator^(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator^=);
// }
// UnsignedBigInt UnsignedBigInt::operator%(const UnsignedBigInt& other) const {
// 	return apply_binary_op(*this, other, &UnsignedBigInt::operator%=);
// }

// ============================================================================
// Section: assignment algebraic operations for primitives
// ============================================================================

UnsignedBigInt& UnsignedBigInt::operator+=(const uint64_t& other) {
	__uint128_t carry = static_cast<__uint128_t>(other);

	for(size_t i = 0; i < m_digits; i++) {
		__uint128_t sum = m_container[i] + carry;
		m_container[i] = static_cast<uint64_t>(sum);
		carry = sum >> 64;
	}

	if(carry) {
		if(m_digits < m_container.size()) {
			m_container[m_digits] = carry;
		} else {
			m_container.push_back(carry);
		}
		m_digits++;
	}

	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator-=(const uint64_t& other) {
	if(m_digits == 1 && m_container[0] < other) {
		throw BigNumUnderflowException(*this, other);
	}

	size_t i = 0;
	uint64_t borrow = other;
	for(; i < m_digits; i++) {
		if(borrow == 0) {
			break;
		}

		if(m_container[i] >= borrow) {
			// fully absorb the borrow
			m_container[i] -= borrow;
			borrow = 0;
		} else {
			uint64_t new_value = (UnsignedBigInt::MAX_U64_VALUE - borrow) + m_container[i];
			m_container[i] = new_value;
			borrow = 1;
		}
	}

	if(i == m_digits && m_digits != 1 && m_container[i - 1] == 0) {
		// if we borrowed from the highest digit,
		// we need to check if we decreased the number of digits
		// in the number
		m_digits--;
	}

	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator*=(const uint64_t& other) {
	__uint128_t carry = 0;
	for(size_t i = 0; i < m_digits; i++) {
		__uint128_t product = static_cast<__uint128_t>(m_container[i]) * other + carry;
		m_container[i] = product; // use truncation
		carry = product >> 64;
	}

	if(carry) {
		if(m_digits < m_container.size()) {
			m_container[m_digits] = carry;
		} else {
			m_container.push_back(carry);
		}
		m_digits++;
	}

	return *this;
}

/*
UnsignedBigInt& UnsignedBigInt::operator<<=(const uint64_t& other) {
	const size_t base_shift = other / 64;
	const size_t bit_shift = other % 64;
	const size_t leading_idx = leading_digit();
	const size_t required_size = leading_idx + base_shift + 2;

	_digits.resize(required_size, 0);

	for(int i = leading_idx; i >= 0; i--) {
		const __uint128_t after_shift = static_cast<__uint128_t>(_digits[i]) << bit_shift;
		const uint64_t upper_half = static_cast<uint64_t>(after_shift >> 64ull);
		const uint64_t lower_half = static_cast<uint64_t>(after_shift & 0xFFFFFFFFFFFFFFFF);
		_digits[i + base_shift + 1] |= upper_half;
		_digits[i + base_shift] = lower_half;
	}

	// fill in with zeros
	for(size_t i = 0; i < base_shift; i++) {
		_digits[i] = 0;
	}

	return *this;
}
*/

// ============================================================================
// Section: assignment algebraic operations for big ints
// ============================================================================
UnsignedBigInt& UnsignedBigInt::operator+=(const UnsignedBigInt& other) {
	size_t min_digit = std::min(m_digits, other.m_digits);
	size_t max_digit = std::max(m_digits, other.m_digits);

	// reserve 1 more space to account for carry
	m_container.reserve(max_digit + 1);

	if(other.m_digits > m_digits) {
		m_container.resize(other.m_digits, 0);
	}

	__uint128_t carry = 0;
	for(size_t i = 0; i < max_digit; i++) {
		const __uint128_t lhs = m_container[i];
		const __uint128_t rhs = other.m_container[i];
		const __uint128_t sum = lhs + rhs + carry;

		m_container[i] = sum & UnsignedBigInt::LOWER_MASK_128;
		carry = sum >> 64;
	}

	m_digits = max_digit;
	if(carry) {
		if(m_digits < m_container.size()) {
			m_container[m_digits] = carry;
		} else {
			m_container.push_back(carry);
		}
		m_digits++;
	}

	return *this;
}

UnsignedBigInt& UnsignedBigInt::operator*=(const UnsignedBigInt& other) {

	container buffer(m_digits + other.m_digits, 0);

	__uint128_t carry = 0;
	for(size_t i = 0; i < m_digits; i++) {
		for(size_t j = 0; j < other.m_digits; j++) {
			const __uint128_t& lhs = m_container[i];
			const __uint128_t& rhs = other.m_container[j];
			const __uint128_t product = (lhs * rhs) + carry + buffer[i + j];
			carry = product >> 64;
			buffer[i + j] = product & UnsignedBigInt::LOWER_MASK_128;
		}

		if(carry) {
			buffer[i + other.m_digits] += carry;
			carry = 0;
		}
	}

	m_container = std::move(buffer);
	return *this;
}

std::string UnsignedBigInt::to_string() const {
	const uint64_t BASE = 1000000000000000000ULL;
	std::vector<uint64_t> result;

	for(int i = m_digits - 1; i >= 0; i--) {
		uint64_t carry = m_container[i];

		for(size_t j = 0; j < result.size(); j++) {
			// Perform the multiplication and addition, ensuring the result fits in 128 bits
			__uint128_t temp = (static_cast<__uint128_t>(result[j]) << 64) + carry;
			result[j] = static_cast<uint64_t>(temp % BASE);
			carry = static_cast<uint64_t>(temp / BASE);
		}

		// Push any remaining carry as a new digit
		while(carry > 0) {
			result.push_back(carry % BASE);
			carry /= BASE;
		}
	}
	// Convert the result to a string
	std::string base10;
	if(result.empty()) {
		return "0";
	}

	base10 = std::to_string(result.back());
	for(int i = result.size() - 2; i >= 0; --i) {
		std::string part = std::to_string(result[i]);
		base10 += std::string(18 - part.length(), '0') + part;
	}

	return base10;
}

std::string UnsignedBigInt::to_bitstring() const {
	std::stringstream ss;

	for(int i = m_digits - 1; i >= 0; i--) {
		ss << std::bitset<sizeof(uint64_t) * 8>(m_container[i]) << ' ';
	}

	return ss.str();
}

size_t UnsignedBigInt::digits() const noexcept {
	return m_digits;
}