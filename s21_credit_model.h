#ifndef _SMARTCALC_v2_0_S21_CREDIT_MODEL_H_
#define _SMARTCALC_v2_0_S21_CREDIT_MODEL_H_

#include <cmath>
#include <iostream>
#include <vector>

namespace s21 {
/**
 * @brief The CreditModel class модель для рассчета платежей по кредиту
 */
class CreditModel {
 public:
  /**
   * @brief CreditModel конструктор
   */
  CreditModel();

  /**
   * @brief CreditModel деструктор
   */
  ~CreditModel();

  /**
   * @brief SetData первичная установка параметров
   * @param[in] amount сумма кредита
   * @param[in] term срок кредита в месяцах
   * @param[in] rate годовая процентная ставка / 100
   */
  void SetData(double amount, double term, double rate);

  /**
   * @brief Init первичный рассчет (для аннуитетного кредита - ежемесячного
   * платежаб дифференцированного кредита - ежемсячного платеа по основному
   * долгу)
   * @param[in] type тип кредита(1 - аннуитетныйб 0 - дифференцированный)
   */
  void Init(int type);

  /**
   * @brief Annuity метод ля рассчета аннуитетного кредита
   * @param[out] arr ссылка на std::vector<double> для возврата показателей на
   * текущий рассчетный период(по индексу 0 - платеж на текущий расчетный
   * период, 1 - платеж по процентам, 2 - платеж по основному долгу, 3 - остаток
   * кредита, 4 - выплачено по основному долгу, 5 - выплачено по процентам)
   */
  void Annuity(std::vector<double> &arr);

  /**
   * @brief Different метод ля рассчета дифференцированного кредита
   * @param[out] arr ссылка на std::vector<double> для возврата показателей на
   * текущий рассчетный период(по индексу 0 - платеж на текущий расчетный
   * период, 1 - платеж по процентам, 2 - платеж по основному долгу, 3 - остаток
   * кредита, 4 - выплачено по основному долгу, 5 - выплачено по процентам)
   */
  void Different(std::vector<double> &arr);

 private:
  double amount, term, rate, temp, pay_debt, pay_percent;
};
}  // namespace s21

#endif  // _SMARTCALC_v2_0_S21_CREDIT_MODEL_H_
