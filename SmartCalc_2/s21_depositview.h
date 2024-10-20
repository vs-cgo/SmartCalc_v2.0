#ifndef S21_DEPOSITVIEW_H
#define S21_DEPOSITVIEW_H

#include <QCalendarWidget>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>

#include "../s21_controller.h"
#include "QtWidgets/qgridlayout.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DepositView;
}
QT_END_NAMESPACE

namespace s21 {
/**
 * @brief The DepositView class калькулятор для рассчета вкладов с возможность
 * пополнения и снятия
 */
class DepositView : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief DepositView
   * @param[in] c указатель на контроллер
   * @param[in] parent указатель на родителя по умолчанию nullptr
   */
  explicit DepositView(s21::Controller *c, QWidget *parent = nullptr);
  /**
   * @brief DepositView деструктор
   */
  ~DepositView();

 private:
  // Starting setting
  void SettingView();
  void CalcDeposit();
  void SchedulePay(QMap<QDate, double> &);
  void ScheduleDep(QMap<QDate, double> &, QDate, int);
  void InsertPay(int, QDate, double, double);
  void FillTableView(int, int, QDate, double, double, double);
  void FillTaxView(int year, double profit, double tax_rate);
  bool CheckAddDrawal(QGridLayout *ptr);
  void ChangePosition(int);

 private slots:
  void AddClicked();
  void on_box_term_currentIndexChanged(int index);
  void CheckAmount();
  void CheckRate();
  void CheckTerm();
  void CheckTax();

  void PushCalculation();
  void ButtonX();

 private:
  Ui::DepositView *ui;
  s21::Controller *c;
};
}  // end namespace s21
#endif  // S21_DEPOSITVIEW_H
