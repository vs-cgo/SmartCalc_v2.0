#include "s21_deposit_model.h"

namespace s21 {
// Constructor
// Default
DepositModel::DepositModel()
    : amount(0), rate(0), tax(), percent(), days(), cap(){};
DepositModel::~DepositModel(){};

// Calculation deposit
void DepositModel::CalcDeposit() { percent += amount * rate / days; }
// Payment
double DepositModel::Payment() {
  if (cap) amount += percent;
  double temp = percent;
  percent = 0;
  return temp;
}

// SetData
void DepositModel::SetData(double a, double r, double t, int d, int c) {
  amount = a;
  rate = r;
  tax = t;
  percent = 0;
  days = d;
  cap = c;
}
void DepositModel::SetDays(int d) { days = d; };
void DepositModel::SetRest(double x) { amount = x; };
// Get rest perc
double DepositModel::GetRest() { return amount; };
double DepositModel::GetPerc() { return percent; };
}  // namespace s21
//
