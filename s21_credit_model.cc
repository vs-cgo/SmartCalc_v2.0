#include "s21_credit_model.h"

namespace s21 {
// Constructor
// Default
CreditModel::CreditModel()
    : amount(0), term(0), rate(0), temp(0), pay_debt(0), pay_percent(0){};
// Destructor
CreditModel::~CreditModel(){};

// Set Data
void CreditModel::SetData(double a, double t, double r) {
  amount = a;
  term = t;
  rate = r / 12;
  temp = pay_debt = pay_percent = 0;
}
// Init pay per mounth for annuity or pay debt for different
void CreditModel::Init(int type) {
  if (type) {
    temp = amount * rate * pow((1 + rate), term) / (pow(1 + rate, term) - 1);
  } else {
    temp = amount / term;
  }
}
// Calculation annuity credit
void CreditModel::Annuity(std::vector<double> &arr) {
  arr[0] = temp;           // pay per mounth
  arr[1] = amount * rate;  // pay for percent
  arr[2] = temp - arr[1];  // pay for debt
  arr[3] = amount - arr[2] < 0.01 ? amount = 0 : (amount -= arr[2]);  // rest
  arr[4] = pay_debt += arr[2];     // pay_debt
  arr[5] = pay_percent += arr[1];  // pay_percent
}
void CreditModel::Different(std::vector<double> &arr) {
  arr[2] = temp;
  arr[1] = amount * rate;
  arr[0] = temp + arr[1];
  arr[3] = amount - arr[2] < 0.01 ? amount = 0 : (amount -= arr[2]);  // rest
  arr[4] = pay_debt += arr[2];     // pay_debt
  arr[5] = pay_percent += arr[1];  // pay_percent
}
}  // namespace s21
