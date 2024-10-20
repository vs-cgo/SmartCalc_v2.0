#include "s21_view.h"

#include "./ui_s21_view.h"

namespace s21 {
View::View(s21::Controller *controller, QWidget *parent)
    : c(controller), QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  // Reg for x
  ui->lineVarError->setVisible(false);
  QRegularExpression reg_x(
      R"(^[+-]?((0|[1-9]\d*)(\.\d*)?|(\.\d+))([E|e][+-]?(0|[1-9]\d*))?$)");
  // qDebug() << reg_x.isValid();
  QRegularExpressionValidator *val_x =
      new QRegularExpressionValidator(reg_x, this);
  ui->lineVar->setValidator(val_x);
  reg.setPattern("[0123456789.]");
  // Service
  connect(ui->equal, SIGNAL(clicked()), this, SLOT(Equal()));
  connect(ui->button_graf, SIGNAL(clicked()), this, SLOT(GrafChecked()));
  connect(ui->button_replot, SIGNAL(clicked()), this, SLOT(GrafReplot()));
  connect(ui->clean_C, SIGNAL(clicked()), this, SLOT(Clean()));
  connect(ui->clean_D, SIGNAL(clicked()), this, SLOT(Delete()));
  connect(ui->clean_AC_x, SIGNAL(clicked()), this, SLOT(CleanX()));
  connect(ui->set_x, SIGNAL(clicked()), this, SLOT(SetX()));
  connect(ui->ANS, SIGNAL(clicked()), this, SLOT(InputAns()));
  connect(ui->credit, SIGNAL(clicked()), this, SLOT(PressCredit()));
  connect(ui->deposit, SIGNAL(clicked()), this, SLOT(PressDeposit()));

  // Digit
  connect(ui->digit_0, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_1, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_2, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_3, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_4, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_5, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_6, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_7, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_8, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->digit_9, SIGNAL(clicked()), this, SLOT(InputDigit()));
  connect(ui->oper_point, SIGNAL(clicked()), this, SLOT(InputPoint()));
  connect(ui->EXP, SIGNAL(clicked()), this, SLOT(InputExp()));

  // Operation
  connect(ui->oper_lb, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_rb, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_pow, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_div, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_mod, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_mult, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_plus, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_minus, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_unopm, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->oper_percent, SIGNAL(clicked()), this, SLOT(InputFunction()));
  // Function
  connect(ui->func_sin, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_cos, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_tan, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_asin, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_acos, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_atan, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_ln, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_log, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->func_sqrt, SIGNAL(clicked()), this, SLOT(InputFunction()));
  // Variables
  connect(ui->variable_e, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->variable_p, SIGNAL(clicked()), this, SLOT(InputFunction()));
  connect(ui->variable_x, SIGNAL(clicked()), this, SLOT(InputFunction()));

  // Check X
  connect(ui->lineVar, SIGNAL(textChanged(const QString &)), this,
          SLOT(LineVarTextChanged(const QString &)));
  //  Hide graf
  ui->graf->setVisible(false);
  ui->frame->setVisible(false);
}

View::~View() { delete ui; }
// InputDigit [0-9]

void View::InputDigit() {
  if (ui->lineEdit->hasFocus() || ui->lineVar->hasFocus()) {
    QString temp = ((QPushButton *)sender())->text();
    QLineEdit *ptr = ui->lineVar;
    QString arr =
        (ui->lineEdit->hasFocus()) ? ui->lineEdit->text() : ui->lineVar->text();

    if (temp == "EXP") temp = "E";
    if (arr.size() + temp.size() < 256) {
      int check = 0;
      QString str;
      if (ui->lineEdit->hasFocus()) {
        arr = ui->lineEdit->text();
        if (!list.empty() && list.back().contains(reg)) {
          str = list.back();
          str = (str != "0" || (str == "0" && temp == ".")) ? str + temp : temp;
          if (ptr->validator()->validate(str, check) != QValidator::Invalid)
            list.back() = str;
        } else {
          if (temp == "." || temp == "E") temp = "0" + temp;
          list.push_back(temp);
        }
        arr = list.join("");
        ui->lineEdit->setText(arr);
      } else if (ui->lineVar->hasFocus()) {
        arr = ui->lineVar->text();
        str = arr + temp;
        if (ptr->validator()->validate(str, check) != QValidator::Invalid) {
          arr += temp;
        }
        ui->lineVar->setText(arr);
      }
    }
  }
}
void View::InputFunction() {
  QString temp = ((QPushButton *)sender())->text();
  QString arr;
  QStringList fun = {"sin", "cos", "tan", "acos", "asin", "atan", "log", "ln"};
  QStringList var = {"p", "e", "x"};
  // QRegularExpression reg("[0123456789.]");
  //  QStringList uno = {"sin(",  "cos(", "tan(", "sqrt(", "acos(", "asin(",
  //                     "atan(", "log(", "ln(",  "p",     "e",     "x"};

  QLineEdit *ptr = ui->lineEdit;
  if (ui->lineEdit->hasFocus()) {
    arr = ptr->text();
    if (temp == "√") temp = "sqrt(";
    if (temp == "pow") temp = "^";
    if (temp == "±") temp = "(-";
    if (temp == "÷") temp = "/";
    if (temp == "π") temp = "p";
    if (fun.contains(temp)) temp += "(";

    if (arr.size() + temp.size() < 256) {
      if (temp == 'e' && !list.empty() && list.back().contains(reg))
        list.push_back("*");
      if (temp == "(-" && !list.empty()) {
        int size = list.size();
        if (list.back().contains(reg) || var.contains(list.back())) {
          if (size == 1 || (size > 1 && !list[size - 2].contains("(-"))) {
            list.insert(size - 1, "(-");
          } else {
            list.remove(size - 2);
          }
        } else if (list.back().contains("(-")) {
          list.pop_back();
        } else {
          list.push_back(temp);
        }
      } else {
        list.push_back(temp);
      }
      ptr->setText(list.join("") == "" ? "0" : list.join(""));
    }
  } else if (ui->lineVar->hasFocus() && (temp == "+" || temp == "-")) {
    int check = 0;
    QString str = ui->lineVar->text() + temp;
    if (ui->lineVar->validator()->validate(str, check) != QValidator::Invalid) {
      ui->lineVar->setText(str);
    }
  }
}

// Point
void View::InputPoint() {
  if (ui->lineVar->hasFocus()) {
    if (!ui->lineVar->text().contains(".")) InputDigit();
  } else if (ui->lineEdit->hasFocus()) {
    if (list.empty() || !list.back().contains(reg) ||
        (list.back().contains(reg) && !list.back().contains(".")))
      InputDigit();
  }
}

// EXP
void View::InputExp() {
  if (ui->lineVar->hasFocus()) {
    if (!ui->lineVar->text().contains("E")) InputDigit();
  } else if (ui->lineEdit->hasFocus()) {
    if (list.empty() || !list.back().contains(reg) ||
        (list.back().contains(reg) && !list.back().contains("E")))
      InputDigit();
  }
}

// InputAns
void View::InputAns() {
  int check = 0;
  QString num = QString::number(c->GetAns(), 'g', 14), str;
  QLineEdit *ptr = ui->lineVar;
  if (ui->lineEdit->hasFocus()) {
    if (list.empty() || !list.back().contains(reg)) {
      list.push_back(num);
    } else {
      str = list.back() + num;
      if (ptr->validator()->validate(str, check) != QValidator::Invalid) {
        list.back() += num;
      }
    }
    ui->lineEdit->setText(list.join(""));
  } else if (ui->lineVar->hasFocus()) {
    str = ptr->text() + num;
    if (ptr->validator()->validate(str, check) != QValidator::Invalid) {
      ptr->setText(str);
    }
  }
}

// Set x
void View::SetX() {
  if (ui->lineVar->isEnabled()) {
    ui->lineVar->setEnabled(false);
    if (ui->lineVar->text() == "") ui->lineVar->setVisible(false);
  } else {
    ui->lineVar->setVisible(true);
    ui->lineVar->setEnabled(true);
    ui->lineVar->setFocus();
  }
}

// Clean x
void View::CleanX() {
  ui->lineVar->setText("");
  if (!ui->set_x->isChecked()) {
    ui->lineVar->setEnabled(false);
    ui->lineVar->setVisible(false);
  }
}

// Clean input
void View::Clean() {
  ui->lineEdit->setText("0");
  list.clear();
}
// Delete last
void View::Delete() {
  if (ui->lineEdit->hasFocus()) {
    if (!list.empty()) {
      if (list.back().contains(reg)) {
        list.back().chop(1);
        if (list.back().size() == 0) list.pop_back();
      } else {
        list.pop_back();
      }
    }
    ui->lineEdit->setText(list.empty() ? "0" : list.join(""));
  } else if (ui->lineVar->hasFocus()) {
    QString temp = ui->lineVar->text();
    if (temp.size()) {
      temp.chop(1);
      ui->lineVar->setText(temp);
    }
  }
}
// CheckInputX
bool View::CheckInputX() {
  if (ui->lineVar->hasAcceptableInput() || ui->lineVar->text() == "") {
    ui->lineVarError->setText("");
    ui->lineVarError->setVisible(false);
    return true;
  } else {
    ui->lineVarError->setVisible(true);
    ui->lineVarError->setText("Некорректный х");
    return false;
  }
};
// LineVarErrorChanged (hide lineVarError if value acceptable)
void View::LineVarTextChanged(const QString &text) {
  if (ui->lineVar->hasAcceptableInput() || ui->lineVar->text() == "") {
    ui->lineVarError->setVisible(false);
  }
}

// Clean lexem
void View::Equal() {
  QString input = ui->lineEdit->text();
  std::string str = input.toUtf8().toStdString();
  c->SetVarx(ui->lineVar->text().toDouble());
  if (CheckInputX()) {
    try {
      ui->lineEdit->setText(QString::number(c->Answer(str), 'g', 14));
    } catch (s21::Incorrect_string) {
      ui->lineEdit->setText("Неверный ввод");
    } catch (s21::Uncertain_result) {
      ui->lineEdit->setText("Неопределенный результат");
    } catch (s21::DivByZero) {
      ui->lineEdit->setText("На ноль делить нельзя");
    }
  }
  list.clear();
}

void View::GrafChecked() {
  if (ui->button_graf->isChecked()) {
    this->resize(600, 750);
    ui->graf->setVisible(true);
    ui->frame->setVisible(true);
    ui->graf->xAxis->setRange(ui->x_begin->value(), ui->x_end->value());
    ui->graf->yAxis->setRange(ui->y_begin->value(), ui->y_end->value());
    ui->graf->replot();
  } else {
    ui->graf->clearGraphs();
    ui->graf->setVisible(false);
    ui->frame->setVisible(false);
    this->resize(600, 250);
  }
}
void View::GrafReplot() {
  if (ui->button_graf->isChecked()) {
    ui->graf->clearGraphs();
    if (ui->graf->graphCount() > 0) ui->graf->graph(0)->data()->clear();
    ui->graf->replot();

    QString input = ui->lineEdit->text();
    std::string str = input.toUtf8().toStdString();

    double x_begin = ui->x_begin->value();
    double x_end = ui->x_end->value();
    double y_begin = ui->y_begin->value();
    double y_end = ui->y_end->value();
    double step = (x_end - x_begin) / 250000, start = x_begin;

    if (!(x_begin >= x_end || y_begin >= y_end)) {
      QVector<double> x, y;
      for (int i = 0; i < 250000; ++i, start += step) {
        try {
          c->SetVarx(start);
          double ans = c->Answer(str);
          if (ans >= -1000000 && ans <= 1000000) {
            x.push_back(start);
            y.push_back(ans);
          }
        } catch (s21::Incorrect_string) {
          break;
        } catch (...) {
          continue;
        }
      }
      if (!x.isEmpty()) {
        ui->graf->clearGraphs();
        ui->graf->addGraph();
        ui->graf->xAxis->setRange(x_begin, x_end);
        ui->graf->yAxis->setRange(y_begin, y_end);
        ui->graf->graph(0)->addData(x, y);
        ui->graf->graph(0)->setPen(QColor(Qt::blue));
        ui->graf->graph(0)->setScatterStyle(
            QCPScatterStyle(QCPScatterStyle::ssDot));
        ui->graf->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->graf->xAxis->setLabel("x");
        ui->graf->yAxis->setLabel("y");
        ui->graf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                  QCP::iSelectPlottables);
        ui->graf->replot();
      }
    }
  }
}
void View::PressCredit() {
  CreditView *credit = new CreditView(c, this);
  credit->setWindowTitle("Credit");
  credit->show();
}

void View::PressDeposit() {
  DepositView *deposit = new DepositView(c, this);
  deposit->setWindowTitle("Deposit");
  deposit->show();
}
}  // end namespace s21
