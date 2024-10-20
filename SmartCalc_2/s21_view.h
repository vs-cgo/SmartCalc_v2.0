#ifndef VIEW_H
#define VIEW_H

#include <QFont>
#include <QList>
#include <QMainWindow>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QString>

#include "../s21_controller.h"
#include "s21_creditview.h"
#include "s21_depositview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE
namespace s21 {
/**
 * @brief View главное окно калькулятора
 */
class View : public QMainWindow {
  Q_OBJECT
 public:
  /**
   * @brief View конструктор
   * @param[in] с указатель на контроллер
   * @param[in] parent указатель на родителя, nullptr по умолчанию
   */
  View(s21::Controller *с, QWidget *parent = nullptr);
  /**
   * @brief View деструктор
   */
  ~View();

 private slots:
  void Equal();
  void GrafReplot();
  void GrafChecked();
  void InputAns();
  void InputDigit();
  void InputFunction();
  void PressCredit();
  void PressDeposit();
  void InputPoint();
  void InputExp();
  // Clean lineEdit
  void Clean();
  void Delete();
  // X
  void LineVarTextChanged(const QString &text);
  bool CheckInputX();
  void CleanX();
  void SetX();

 private:
  Ui::View *ui;
  s21::Controller *c;
  QStringList list;
  QRegularExpression reg;
};
}  // namespace s21
#endif  // VIEW_H
