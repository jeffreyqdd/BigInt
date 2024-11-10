#include "include/bignum.hpp"

#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>
#include <format>
#include <iostream>
#include <sstream>
#include <string>

BigNum::BigNum(size_t allocation_size) {
	_digits = std::move(container(allocation_size, 0));
}
BigNum::BigNum(const std::string& number) {
	_digits = std::move(container(BigNum::INITIAL_ALLOCATIONS_SIZE, 0));

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

BigNum BigNum::operator+(const uint64_t& other) const {
	BigNum ret = *this;
	ret += other;
	return ret;
}
BigNum& BigNum::operator+=(const uint64_t& other) {
	__uint128_t carry = static_cast<__uint128_t>(other);

	for(size_t i = 0; i < _digits.size(); i++) {
		__uint128_t sum = _digits[i] + carry;
		_digits[i] = static_cast<uint64_t>(sum);
		carry = sum >> 64;
	}

	if(carry) {
		_digits.push_back(carry);
	}

	return *this;
}

BigNum BigNum::operator*(const uint64_t& other) const {
	BigNum ret = *this;
	ret *= other;
	return ret;
}
BigNum& BigNum::operator*=(const uint64_t& other) {
	__uint128_t carry = 0;
	for(size_t i = 0; i < _digits.size(); i++) {
		__uint128_t product = static_cast<__uint128_t>(_digits[i]) * other + carry;
		_digits[i] = static_cast<uint64_t>(product); // use truncation
		carry = product >> 64;
	}
	if(carry) {
		_digits.push_back(static_cast<uint64_t>(carry));
	}
	return *this;
}

BigNum BigNum::operator-(const uint64_t& other) const {
	BigNum ret = *this;
	ret -= other;
	return ret;
}
BigNum& BigNum::operator-=(const uint64_t& other) {
	return *this;
}

BigNum BigNum::operator/(const uint64_t& other) const {
	BigNum ret = *this;
	ret /= other;
	return ret;
}

BigNum& BigNum::operator/=(const uint64_t& other) {
	return *this;
}

BigNum BigNum::operator>>(const uint64_t& other) const {
	BigNum ret = *this;
	ret >>= other;
	return ret;
}
BigNum& BigNum::operator>>=(const uint64_t& other) {
	return *this;
}

BigNum BigNum::operator<<(const uint64_t& other) const {
	BigNum ret = *this;
	ret <<= other;
	return ret;
}
BigNum& BigNum::operator<<=(const uint64_t& other) {
	const size_t base_shift = other / 64;
	const size_t bit_shift = other % 64;
	const size_t leading_idx = leading_digit();
	const size_t required_size = leading_idx + base_shift + 2;

	_digits.resize(required_size, 0);

	for(int i = leading_idx; i >= 0; i--) {
		const __uint128_t after_shift = static_cast<__uint128_t>(_digits[i]) << bit_shift;
		const uint64_t upper_half = static_cast<uint64_t>(after_shift >> 64ull);
		std::cout << "upper:half: " << upper_half << std::endl;
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

size_t BigNum::leading_digit() const {
	if(_digits.size() == 0) {
		return 0;
	}

	for(int i = _digits.size() - 1; i >= 0; i--) {
		if(_digits[i] != 0) {
			return i;
		}
	}

	return 0;
}

std::string BigNum::to_string() const {
	const uint64_t BASE = 1000000000000000000ULL;
	std::vector<uint64_t> result;

	for(int i = _digits.size() - 1; i >= 0; i--) {
		uint64_t carry = _digits[i];

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

std::string BigNum::to_bitstring() const {
	std::stringstream ss;

	for(int i = leading_digit(); i >= 0; i--) {
		ss << std::bitset<sizeof(uint64_t) * 8>(_digits[i]) << ' ';
	}

	return ss.str();
}

class BigNumUnderflowException : public std::exception {
private:
	std::string _error_msg;

public:
	explicit BigNumUnderflowException(const BigNum& lhs, const uint64_t& rhs) {
		_error_msg = std::format("cannot subtract {} from {}", rhs, lhs.to_string());
	}

	const char* what() const noexcept override {
		return _error_msg.c_str();
	}
};