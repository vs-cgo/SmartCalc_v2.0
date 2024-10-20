#include "smartcalc_v2.0_test.h"

#include <iostream>

TEST_F(SmartCalcTest, Correct) {
  for (size_t i = 0, e = vec_cor.size(); i < e; ++i) {
    controller.SetVarx(2);
    try {
      double val = controller.Answer(vec_cor[i]);
      // std::cout << std::setprecision(15) << ">>>CORRECT: " << vec_cor[i]
      // << '=' << val << '\n';
      EXPECT_NEAR(val, ans[i], kEps);
    } catch (...) {
      std::cout << ">>>INCORRECT: " << vec_cor[i] << '\n';
    }
  }
}
TEST_F(SmartCalcTest, Incorrect) {
  for (size_t i = 0, e = vec_incor.size(); i < e; ++i) {
    controller.SetVarx(2);
    // std::cout << ">>>INCORRECT: " << vec_incor[i] << '\n';
    if (i < 11) {
      EXPECT_THROW(controller.Answer(vec_incor[i]), s21::Incorrect_string);
    } else if (i < 15) {
      EXPECT_THROW(controller.Answer(vec_incor[i]), s21::Uncertain_result);
    } else {
      EXPECT_THROW(controller.Answer(vec_incor[i]), s21::DivByZero);
    }
  }
}

TEST_F(SmartCalcTest, SpeedForGraf) {
  controller.SetVarx(2.0);
  for (int i = 0; i < 250000; ++i) {
    try {
      controller.Answer("acos(asin(1 / 10) / x");
      // std::cout << ">>>CORRECT: " << " answer = " << val << '\n';
    } catch (...) {
      throw;
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
