#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../s21_Controller.h"

class SmartCalcTest : public testing::Test {
 protected:
  SmartCalcTest()
      : model(),
        cmodel(),
        dmodel(),
        controller(&model, &cmodel, &dmodel),
        vec_cor{
            "2   +   2",
            "2 / (3 + 2) * 5",
            "+1.1 + 1.1",
            "+.1 - p   *2+ 1.1 * 4",
            "-sin(2p * 3.3) + log(10",
            "sin(2p *.5) + -log(10^  -5 ",
            "+2 -3 + 4 - 5 + 6 +(2 ",
            "sin(1) + -sqrt(4",
            "+8",
            "8 * sin(5)-log(3)",
            "sin(log(-cos(sqrt(4) ^ 2",
            "sin(5) + cos(-sin(5) / 5)",
            "sin(5) + cos(-5 / 5)",
            " 12x+2*sin(p)-cos(log(10))/14+12x",
            "-tan(1)% ^ 3",
            "3 + 4 * 2 / (1 - 5)^2",
            "ln(-sin(5))",
            "atan(acos(asin(1 / 10) / 5",
            "5 * 3.45 mod -10.2",
            "3 * 2^2 ^ 2^ 2",
            "5x* ( x - 3) ^ x",
            "ln(e) + e",
            "2 + -3 * -(2) + (+(+2))",
            "1.2 *e* 2 + 5",
            "1.2E2 + 5",
            "sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(3.4))))))))))))*"
            "sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(3))))))))))))+"
            "sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(1))))))))))))-"
            "sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(sin(cos(0.2))))))))))))"},
        vec_incor{"sin(1) + * 2 - +sqrt(4",
                  "sin(5) + 234 -(-sqrt(++4",
                  "",
                  "+",
                  "---4 + 4*sin(--log(10 ^ 3",
                  "(-++--3",
                  "2435,234 + 234",
                  "2 * asin2",
                  "tan2",
                  "4 * -2 +(-%2)",
                  "(+1.1 + 1.1)) * 4",
                  "ln(sin(5))",
                  "asin(1 + 0.001)",
                  "0.90620566431866acos(sin(acos(acos(asin(32",
                  "sin(log(cos(sqrt(4) ^ 2",
                  "1*tan(p / 0",
                  "(2 + (+6) ) / (cos(0) - 1)",
                  "5/ (sin(2) -sin(2)"},
        ans{4,
            2,
            2.2,
            -1.7831853072,
            0.0489434837,
            5,
            6,
            -1.1585290152,
            8,
            -8.148515452,
            -0.1836113149,
            0.0227413107,
            -0.4186219688,
            47.96140698,
            -0.0000037775,
            3.5,
            -0.04194317,
            0.998053913,
            7.05,
            768,
            10,
            3.71828182845905,
            10,
            11.52387639,
            125,
            0.48490139},
        kEps(1e-7){};
  ~SmartCalcTest(){};
  // void TearDown() overrride {};
  void SetUp() override{};
  s21::Model model;
  s21::CreditModel cmodel;
  s21::DepositModel dmodel;
  s21::Controller controller;
  std::vector<std::string> vec_cor, vec_incor;
  std::vector<double> ans;
  const double kEps;
};
