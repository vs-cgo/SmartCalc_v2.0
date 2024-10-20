#ifndef S21_CREDITVIEW_H
#define S21_CREDITVIEW_H

#include <QDialog>
#include <QTableWidget>

#include "../s21_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CreditView;
}
QT_END_NAMESPACE

namespace s21 {
/**
 * @brief The CreditView class для расчета платежей и вывода в табличной форме.
 * @ref CREDIT
 */
class CreditView : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief CreditView конструктор
   * @param[in] c указатель на контроллер
   * @param[in] parent уазатель на родитея 0 по умолчанию
   */
  explicit CreditView(s21::Controller *c, QWidget *parent = nullptr);
  /**
   * @brief Деструктор
   */
  ~CreditView();

 private slots:
  void on_line_rate_textEdited(const QString &arg1);
  void PushCalc();
  void InputData(int, QString, std::vector<double> &);
  void InputSummary(int, std::vector<double> &);
  void SettingTable();
  void HorizontalBar(double cr, double pr);

  void on_box_term_currentIndexChanged(int index);
  void on_line_term_textEdited(const QString &arg1);

 private:
  Ui::CreditView *ui;
  s21::Controller *c;
};
}  // end namespace s21
#endif  // S21_CREDITVIEW_H
