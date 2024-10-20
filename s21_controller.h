#ifndef _SMARTCALC_v2_0_S21_CONTROLLER_H_
#define _SMARTCALC_v2_0_S21_CONTROLLER_H_

#include "s21_credit_model.h"
#include "s21_deposit_model.h"
#include "s21_model.h"

/**
 *@brief s21 namespace в котором определены основны компоненты калькулятора
 */
namespace s21 {

/**
 *@brief Controller класс основной целью которого является передача от View к
 *Model и обратно
 */
class Controller {
 public:
  /**
   * @brief Controller конструктор по умолчанию(значения для указателей nullptr)
   */
  Controller() noexcept;

  /**
   * @brief Controller параметризованный конструктор
   * @param[in] m указатель на модель основного калькулятора
   * @param[in] cm указатель на модель кредитного калькулятора
   * @param[in] dm указатель на модель депозитного калькулятора
   */
  Controller(Model *m, CreditModel *cm, DepositModel *dm);

  /**
   * @brief Деструктор
   */
  ~Controller();

  // Methods
  /**
   * @brief Answer метод для вычисления математического выражения и возврата
   * ответа
   * @param[in] str строка с математическим выражение для передачи в Model
   * @return ответ
   */
  double Answer(std::string str);

  /**
   * @brief SetVarx метод для передачи значения х в модель
   * @param[in] var значение переменной х
   */
  void SetVarx(double var);

  /**
   * @brief GetVar получение значения апеременной из модели
   * @return значение переменной х
   */
  double GetVar();

  /**
   * @brief GetAns получение значения ответа предыдущего вычисления(если
   * предыдущего не было или было выброшено исключение, то 0)
   * @return ответ предыдущего вычисления
   */
  double GetAns();

  // Credit
  /**
   * @brief SetData первичное установка значений после нажатия кнопки рассчитать
   * @param[in] amount сумма кредита
   * @param[in] term срок кредита
   * @param[in] rate ставка кредита
   * @param[in] type тип кредита(1- аннуитетный, 0 - дифференцированный)
   */
  void SetData(double amount, double term, double rate, int type);

  /**
   * @brief CalcCredit метод для расчета показателей(ежемесячный платеж,
   * проценты, остаток...)
   * @param[in] type тип кредитования(1- аннуитетный, 0 - дифференцированный)
   * @param[in] arr ссылка на массив размером 6 для возврата 0-платеж на
   * текущий расчетный период, 1 - по процентам, 2 - по основному долгу, 3 -
   * остаток, 4 - выплачено по основному долгу, 5 - выплачено по процентам
   */
  void CalcCredit(int type, std::vector<double> &arr);

  // Deposit
  /**
   * @brief CalcDeposit метод для расчета проценто по депозиту на каждый день
   */
  void CalcDeposit();

  /**
   * @brief Payment метод для возврата накопленных процетов согласно графику
   * выплат
   * @return сумма процентовна период выплат
   */
  double Payment();

  /**
   * @brief SetData метод для установки значений перед рассчетом
   * @param[in] amount сумма депозита
   * @param[in] rate годовая процентная ставка / 100
   * @param[in] tax налоговая ставка / 100
   * @param[in] days количество дней в году
   * @param[in] cap капитализация(0 - нет, 1 - да)
   */
  void SetData(double amount, double rate, double tax, int days, int cap);

  /**
   * @brief SetDays метод для передачи в модель количества дней в году
   * @param[in] days количество дней в текущем году
   */
  void SetDays(int days);

  /**
   * @brief SetRest метод для передачи текущего отстатка в модель
   * @param[in] rest текущее значения остатка на счете
   */
  void SetRest(double rest);

  /**
   * @brief GetRest метод для извлечения из модели текущего остатка
   * @return значения остатка на счете
   */
  double GetRest();

  /**
   * @brief GetPerc метод для получения начисленных процентов на рассчетный
   * период
   * @return сумма процентов
   */
  double GetPerc();

 private:
  Model *model;
  CreditModel *cmodel;
  DepositModel *dmodel;
};
}  // end namespace s21
#endif  // _SMARTCALC_v2_0_S21_CONTROLLER_H_
