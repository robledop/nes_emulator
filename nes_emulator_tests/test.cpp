#include "pch.h"
extern "C" {
#include "SDL.h"
}

TEST(TestCaseName, TestName) {
	printf("%s", "Test");
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}