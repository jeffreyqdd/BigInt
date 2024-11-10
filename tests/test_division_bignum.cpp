#include "include/bignum.hpp"
#include "gtest/gtest.h"

TEST(Division, Simple) {
	UnsignedBigInt a = std::string("27");
	UnsignedBigInt b = std::string("5");
	a /= b;
	ASSERT_EQ(a.to_string(), "5");
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
