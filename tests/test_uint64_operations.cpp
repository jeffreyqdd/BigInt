#include "include/bignum.hpp"
#include "gtest/gtest.h"

#include <iostream>
#include <limits>

TEST(BigNumTests, Initialization) {
	BigNum num;
	ASSERT_EQ(num.to_string(), "0");
}

TEST(BigNumTests, AddWithInt) {
	BigNum num;
	num += 64;

	ASSERT_EQ(num.to_string(), "64");

	BigNum num2 = num + 1;
	ASSERT_EQ(num.to_string(), "64");
	ASSERT_EQ(num2.to_string(), "65");
}

TEST(BigNumTests, AddWithIntBig) {
	BigNum num;
	num += std::numeric_limits<uint64_t>::max();
	num += std::numeric_limits<uint64_t>::max();

	ASSERT_EQ(num.to_string(), "36893488147419103230");

	num += std::numeric_limits<uint64_t>::max();
	num += std::numeric_limits<uint64_t>::max();
	num += std::numeric_limits<uint64_t>::max();
	num += std::numeric_limits<uint64_t>::max();

	ASSERT_EQ(num.to_string(), "110680464442257309690");
}

TEST(BigNumTests, MultiplyWithInt) {
	BigNum num;
	ASSERT_EQ(num.to_string(), "0");

	num += 12;
	num *= 12;
	ASSERT_EQ(num.to_string(), "144");

	BigNum num2 = num * 10;

	ASSERT_EQ(num.to_string(), "144");
	ASSERT_EQ(num2.to_string(), "1440");

	num *= 0;
	ASSERT_EQ(num.to_string(), "0");
}

TEST(BigNumTests, MultiplyWithIntBig) {
	BigNum num;
	num += std::numeric_limits<uint64_t>::max();
	num *= std::numeric_limits<uint64_t>::max();

	ASSERT_EQ(num.to_string(), "340282366920938463426481119284349108225");
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}