#include <QApplication>

#include "s21_view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "C");
  s21::Model m;
  s21::CreditModel cm;
  s21::DepositModel dm;
  s21::Controller c(&m, &cm, &dm);
  s21::View v(&c);
  v.setWindowTitle("Calculator_v2.0");
  v.show();
  return a.exec();
}
