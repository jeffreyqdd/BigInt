#include "include/bignum.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
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
