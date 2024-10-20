#ifndef _SMARTCALC_v2_0_S21_DEPOSIT_MODEL_H_
#define _SMARTCALC_v2_0_S21_DEPOSIT_MODEL_H_

#include <cmath>
#include <map>

namespace s21 {
/**
 * @brief The DepositModel class модель для рассчета выплат по депозиту
 */
class DepositModel {
 public:
  /**
   * @brief DepositModel конструктор
   */
  DepositModel();

  /**
   * @brief ~DepositModel деструктор
   */
  ~DepositModel();

  /**
   * @brief CalcDeposit метод для рассчета процентов на 1 день
   */
  void CalcDeposit();

  /**
   * @brief Payment метод для вывода накопленных процентов(если капитализация то
   * добавляется к остатку на счете)
   * @return сумма проценто на текущий расчетный период(после вызова сумма
   * процентов равна 0)
   */
  double Payment();
  // Modifiers
  /**
   * @brief SetData метод для первичной установки значений
   * @param[in] amount сумма депозита
   * @param[in] rate годовая процентная ставка / 100
   * @param[in] tax налоговая ставка / 100
   * @param[in] days количество дней в году
   * @param[in] cap капитализация (0 - нет, 1 - да)
   */
  void SetData(double amount, double rate, double tax, int days, int cap);

  /**
   * @brief SetDays метод для установки количества дней в году
   * @param[in] days количество дней текущем году
   */
  void SetDays(int days);

  /**
   * @brief SetRest метод для установки остатка на счете(если произошло снятие
   * или пополнение средств)
   * @param[in] rest текущий остаток
   */
  void SetRest(double rest);

  /**
   * @brief GetRest метод для возврата текущего остатка
   * @return возвращает текущий баланс счета
   */
  double GetRest();

  /**
   * @brief GetPerc метод для возврата процентов
   * @return сумма накопленных процентов на текущий день()
   */
  double GetPerc();

 private:
  double amount, rate, tax, percent;
  int days, cap;
};

}  // namespace s21
#endif
