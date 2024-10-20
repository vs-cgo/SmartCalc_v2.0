#include "s21_depositview.h"

#include "ui_s21_depositview.h"

namespace s21 {
DepositView::DepositView(s21::Controller *controller, QWidget *parent)
    : c(controller), QDialog(parent), ui(new Ui::DepositView) {
  ui->setupUi(this);
  this->resize(this->width(), 350);
  SettingView();
  connect(ui->add, SIGNAL(clicked()), this, SLOT(AddClicked()));
  connect(ui->drawal, SIGNAL(clicked()), this, SLOT(AddClicked()));
  connect(ui->line_amount, SIGNAL(textEdited(QString)), this,
          SLOT(CheckAmount()));
  connect(ui->line_term, SIGNAL(textEdited(QString)), this, SLOT(CheckTerm()));
  connect(ui->line_rate, SIGNAL(textEdited(QString)), this, SLOT(CheckRate()));
  connect(ui->line_tax, SIGNAL(textEdited(QString)), this, SLOT(CheckTax()));
  connect(ui->calc, SIGNAL(clicked()), this, SLOT(PushCalculation()));
}
// First setting
void DepositView::SettingView() {
  QRegularExpression reg_a(R"(^[1-9][0-9]{0,12}(\.\d{0,2})?$)");
  QRegularExpressionValidator *val_a =
      new QRegularExpressionValidator(reg_a, this);
  ui->line_amount->setValidator(val_a);

  QRegularExpression reg_t(R"(^([1-9]\d?|[6]00|[1-5]\d{0,2})$)");
  QRegularExpressionValidator *val_t =
      new QRegularExpressionValidator(reg_t, this);
  ui->line_term->setValidator(val_t);

  QRegularExpression reg_r(
      R"(^([0]\.([1-9]\d{0,2}|0[1-9]\d?)|[1-9]\d{0,2}(\.\d{0,3})?)$)");
  QRegularExpressionValidator *val_r =
      new QRegularExpressionValidator(reg_r, this);
  ui->line_rate->setValidator(val_r);

  QRegularExpression reg_tax(R"(^([0]|[1-9]\d?)$)");
  QRegularExpressionValidator *val_tax =
      new QRegularExpressionValidator(reg_tax, this);
  ui->line_tax->setValidator(val_tax);

  ui->line_amount->setFocus();
  ui->line_a->setVisible(false);
  ui->line_te->setVisible(false);
  ui->line_r->setVisible(false);
  ui->line_ta->setVisible(false);
  ui->table_pay->verticalHeader()->hide();
  ui->table_tax->verticalHeader()->hide();
  ui->label_1copy->setVisible(false);
  ui->label_2copy->setVisible(false);
  ui->table_pay->setVisible(false);
  ui->table_tax->setVisible(false);
  // ui->scroll_add->setVisible(false);
  // ui->scroll_drawal->setVisible(false);
  //  Size
  ui->scroll_add->setMaximumHeight(0);
  ui->scroll_drawal->setMaximumHeight(0);
  ui->sawc_add->setMaximumHeight(0);
  ui->sawc_drawal->setMaximumHeight(0);
  ui->scroll_add->resize(ui->scroll_add->width(), 0);
  ui->scroll_drawal->resize(ui->scroll_drawal->width(), 0);
  // Margins
  ui->scroll_add->setContentsMargins(0, 0, 0, 0);
  ui->scroll_drawal->setContentsMargins(0, 0, 0, 0);
  ui->scroll_add->widget()->setContentsMargins(0, 0, 0, 0);
  ui->scroll_drawal->widget()->setContentsMargins(0, 0, 0, 0);
  ui->calc->setDefault(true);
  ui->gridLayoutView->update();
}
DepositView::~DepositView() { delete ui; }

// Add deposit or drawal
void DepositView::AddClicked() {
  QScrollArea *sa = ui->scroll_add;
  QString ph = "Сумма пополнения";
  int var = 0;
  if (qobject_cast<QPushButton *>(sender())->text() == "+ Добавить снятие") {
    var = 1;
    ph = "Сумма снятия";
    sa = ui->scroll_drawal;
  }
  // sa->setVisible(true);
  if (sa->widget()->layout() == nullptr) {
    QGridLayout *grid_layout = new QGridLayout();
    sa->widget()->setLayout(grid_layout);
    grid_layout->setContentsMargins(0, 0, 0, 0);
    grid_layout->setSpacing(0);
  }
  QGridLayout *ptr = qobject_cast<QGridLayout *>(sa->widget()->layout());
  if (ptr->count() < 80) {
    if (sa->maximumHeight() < 160) {
      sa->setMaximumHeight(sa->height() + 32);
      this->resize(this->width(), this->height() + 32);
      this->update();
    }
    sa->widget()->setMaximumHeight(sa->widget()->height() + 32);
    ui->gridLayoutView->update();

    // QComboBox for select frequency
    QComboBox *fill = new QComboBox();
    QStringList str = {"Разовое",       "Раз в месяц",   "Раз в 2 месяца",
                       "Раз в квартал", "Раз в полгода", "Раз в год"};
    fill->addItems(str);
    // Calendare for set date
    QDateEdit *date = new QDateEdit(QDate::currentDate());
    date->setCalendarPopup(true);
    date->setDisplayFormat("dd/MM/yy");
    date->setMinimumDate(QDate::currentDate());
    date->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
    date->setLocale(QLocale::Language(QLocale::Russian));
    // QLineEdit for amount witn validator
    QLineEdit *amount = new QLineEdit();
    QRegularExpression reg_a(R"(^[1-9][0-9]{0,12}(\.\d{0,2})?$)");
    QRegularExpressionValidator *val_a =
        new QRegularExpressionValidator(reg_a, this);
    amount->setValidator(val_a);
    amount->setAlignment(Qt::AlignLeft);
    amount->setPlaceholderText(ph);
    amount->setStyleSheet(
        "font: 13pt \"Times New Roman\";\ncolor: rgb(0, 0, 0);\n"
        "background-color: rgb(255, 255, 255);"
        "border: 0px solid rgb(55, 55, 55);");
    // X button
    QPushButton *del = new QPushButton();
    del->setStyleSheet(
        "color: rgb(203, 68, 74);\nbackground-color: rgb(49, 49, 49);\n "
        "border: "
        "0px solid rgb(49, 49, 49);");
    del->setText("x");
    del->setFlat(true);
    del->setMinimumSize(25, 25);

    int row = 0;
    for (; ptr->itemAtPosition(row, 0) != 0; row++) {
    }
    ptr->addWidget(fill, row, 0, 1, 1);
    ptr->addWidget(date, row, 1, 1, 1);
    ptr->addWidget(amount, row, 2, 1, 1);
    ptr->addWidget(del, row, 3, 1, 1);
    this->setMinimumWidth(650);
    if (!var) {
      connect(del, SIGNAL(clicked()), this, SLOT(ButtonX()));
      if (!row) ChangePosition(ui->gridLayoutView->indexOf(ui->label_1));
    } else {
      connect(del, SIGNAL(clicked()), this, SLOT(ButtonX()));
      if (!row) ChangePosition(ui->gridLayoutView->indexOf(ui->label_2));
    }
  }
}
void DepositView::ChangePosition(int index) {
  if (ui->gridLayoutView->indexOf(ui->label_1) == index) {
    ui->label_1->setVisible(false);
    ui->label_1copy->setVisible(true);
  } else if (ui->gridLayoutView->indexOf(ui->label_2) == index) {
    ui->label_2->setVisible(false);
    ui->label_2copy->setVisible(true);
  } else if (ui->gridLayoutView->indexOf(ui->label_1copy) == index) {
    ui->label_1->setVisible(true);
    ui->label_1copy->setVisible(false);
  } else if (ui->gridLayoutView->indexOf(ui->label_2copy) == index) {
    ui->label_2->setVisible(true);
    ui->label_2copy->setVisible(false);
  }
}

// Delete row deposit or drawal
void DepositView::ButtonX() {
  int row = 0, index = 0;
  QPushButton *ptr = qobject_cast<QPushButton *>(sender());
  QWidget *wid = qobject_cast<QWidget *>(ptr->parent());
  QScrollArea *sa =
      wid->objectName() == "sawc_add" ? ui->scroll_add : ui->scroll_drawal;
  QGridLayout *lay = qobject_cast<QGridLayout *>(wid->layout());

  while ((lay->itemAtPosition(row, 0)) != 0) ++row;
  --row;
  QPushButton *pb = qobject_cast<QPushButton *>(sender());
  if (pb) index = lay->indexOf(pb) - 3;
  for (int i = 0; row >= 0 && i < 4; i++) {
    QWidget *w = lay->itemAt(index)->widget();
    lay->removeWidget(w);
    delete w;
  }

  if (row >= 0) {
    QLayoutItem *ch;
    QVector<QLayoutItem *> plist;
    int count = lay->count();
    while ((ch = lay->takeAt(index)) != 0) {
      plist.push_back(ch);
    }
    for (; index < count; index++) {
      lay->addItem(plist.takeFirst(), index / 4, index % 4);
    }
  }
  sa->setMaximumHeight(row > 4 ? 160 : row * 32);
  wid->setMaximumHeight(row * 32);
  if (row <= 4) this->resize(this->width(), this->height() - 32);
  lay = ui->gridLayoutView;
  if (row == 0)
    ChangePosition(lay->indexOf(
        wid->objectName() == "sawc_add" ? ui->label_1copy : ui->label_2copy));
}

// If index of term has been changed
void DepositView::on_box_term_currentIndexChanged(int index) {
  QRegularExpression reg_t(R"(^([1-9]\d?|[6]00|[1-5]\d{0,2})$)");
  if (index == 0) {
    reg_t.setPattern(
        R"(^([1-9]\d{0,3}|1[0-7]\d{0,3}|18[0-1]\d{0,2}|182[0-4]\d?|18250)$)");
  } else if (index == 2) {
    reg_t.setPattern(R"(^([1-4]\d?|[5]0?|[6-9])$)");
  }
  QRegularExpressionValidator *val_t =
      new QRegularExpressionValidator(reg_t, this);
  ui->line_term->setValidator(val_t);
  CheckTerm();
}

// Check amount
void DepositView::CheckAmount() {
  if (ui->line_amount->hasAcceptableInput()) {
    ui->line_a->setVisible(false);
  } else {
    ui->line_a->setVisible(true);
    ui->line_a->setText("Заполните это поле");
  }
}
// Check term
void DepositView::CheckTerm() {
  if (ui->line_term->hasAcceptableInput()) {
    ui->line_te->setVisible(false);
  } else {
    ui->line_te->setVisible(true);
    int i = ui->box_term->currentIndex();
    if (ui->line_term->text() == "") {
      ui->line_te->setText("Заполните это пол");
    } else if (i == 0) {
      ui->line_te->setText("от 1 до 18250");
    } else if (i == 1) {
      ui->line_te->setText("от 1 до 600");
    } else {
      ui->line_te->setText("от 1 до 50");
    }
  }
}
// Check rate
void DepositView::CheckRate() {
  if (ui->line_rate->hasAcceptableInput()) {
    ui->line_r->setVisible(false);
  } else {
    ui->line_r->setVisible(true);
    ui->line_r->setText("от 0.01 до 999.999");
  }
}
void DepositView::CheckTax() {
  if (ui->line_tax->hasAcceptableInput()) {
    ui->line_ta->setVisible(false);
  } else {
    ui->line_ta->setVisible(true);
    ui->line_ta->setText("от 0 до 99");
  }
}
// Check add and drawal
bool DepositView::CheckAddDrawal(QGridLayout *ptr) {
  bool result = false;
  if (!ptr) return result;
  for (int i = 2; i < ptr->count() && !result; i += 4) {
    if (!qobject_cast<QLineEdit *>(ptr->itemAt(i)->widget())
             ->hasAcceptableInput())
      result = true;
  }
  return result;
}
// If push buttona calculation
void DepositView::PushCalculation() {
  if (ui->table_pay->isVisible())
    this->resize(this->width(), this->height() -
                                    ui->table_pay->minimumHeight() -
                                    ui->table_tax->maximumHeight());
  ui->table_pay->clear();
  ui->table_pay->setRowCount(0);
  ui->table_pay->setVisible(false);
  ui->table_pay->setMinimumHeight(0);
  ui->table_tax->clear();
  ui->table_tax->setRowCount(0);
  ui->table_tax->setVisible(false);
  int i = 0;
  if (!ui->line_amount->hasAcceptableInput()) CheckAmount(), ++i;
  if (!ui->line_term->hasAcceptableInput()) CheckTerm(), ++i;
  if (!ui->line_rate->hasAcceptableInput()) CheckRate(), ++i;
  if (!ui->line_tax->hasAcceptableInput()) CheckTax(), ++i;
  if (CheckAddDrawal(qobject_cast<QGridLayout *>(ui->sawc_add->layout()))) ++i;
  if (CheckAddDrawal(qobject_cast<QGridLayout *>(ui->sawc_drawal->layout())))
    ++i;
  if (!i) {
    CalcDeposit();
  }
}

void DepositView::CalcDeposit() {
  double amount = ui->line_amount->text().toDouble();
  double rate = ui->line_rate->text().toDouble() / 100;
  double tax = ui->line_tax->text().toDouble() / 100;
  int cap = ui->box_cap->isChecked();
  double all_percent = 0, profit = 0;

  QMap<QDate, double> map_date;
  QMap<QDate, double> map_refill;
  QTableWidgetItem *ptwi = 0;
  SchedulePay(map_date);
  QDate current = QDate::currentDate();
  QDate end = map_date.empty() ? current : map_date.lastKey();

  c->SetData(amount, rate, tax, current.daysInYear(), cap);

  ScheduleDep(map_refill, end, 1);
  ScheduleDep(map_refill, end, -1);

  int row = map_date.count();
  FillTableView(row, 0, current, 0, amount, c->GetRest());
  int i = 1, days_in_year = 0, count_tax = 0;
  for (; current <= end;) {
    current = current.addDays(1);
    if (days_in_year != current.daysInYear()) c->SetDays(current.daysInYear());
    c->CalcDeposit();
    if (map_refill.count(current)) {
      double rest = c->GetRest();
      InsertPay(i++, current, map_refill[current], rest);
      if (rest + map_refill[current] >= 0.01)
        c->SetRest(rest + map_refill[current]);
      row++;
    }
    if (map_date.contains(current)) {
      double p = c->Payment();
      profit += p;
      FillTableView(row, i++, current, p, cap ? p : 0, c->GetRest());
      map_date[current] = c->GetRest();
      all_percent += p;
    }
    if (current.toString("dd.MM") == "31.12" || current == end) {
      if (profit > 160000.00) FillTaxView(current.year(), profit, tax);
      profit = 0;
      ++count_tax;
    }
  }
  int h = row < 10 ? row * 32 : 300;
  int h2 = (count_tax * 32 < 150) ? count_tax * 32 + 32 : 150;
  ui->table_pay->setMinimumHeight(h);
  ui->table_tax->setMaximumHeight(h2);
  this->resize(this->width(), this->height() + h + h2);
  FillTableView(row, i, current, all_percent, 0, c->GetRest());
}

void DepositView::SchedulePay(QMap<QDate, double> &map) {
  int freq = ui->box_freq->currentIndex();
  int term = ui->line_term->text().toInt();

  QDate current = QDate::currentDate();
  QDate start = current, end;

  if (ui->box_term->currentIndex() == 0) end = current.addDays(term);
  if (ui->box_term->currentIndex() == 1) end = current.addMonths(term);
  if (ui->box_term->currentIndex() == 2) end = current.addYears(term);

  map[current] = ui->line_amount->text().toDouble();
  while (start < end) {
    if (freq == 0) {
      start = start.addDays(1);
    } else if (freq == 1) {
      start = start.addDays(7);
    } else if (freq == 2) {
      start = start.addMonths(1);
    } else if (freq == 3) {
      start = start.addMonths(3);
    } else if (freq == 4) {
      start = start.addMonths(6);
    } else if (freq == 5) {
      start = start.addYears(1);
    } else if (freq == 6) {
      start = end;
    }
    if (start < end) {
      map[start] = 0;
    }
  }
  map[end] = 0;
}

void DepositView::ScheduleDep(QMap<QDate, double> &map, QDate end, int x) {
  QGridLayout *ptr = qobject_cast<QGridLayout *>(
      ((x == 1) ? ui->sawc_add : ui->sawc_drawal)->layout());
  if (!ptr) return;
  int count = ptr->count();
  for (int i = 0; i < count; i += 4) {
    QComboBox *box = qobject_cast<QComboBox *>(ptr->itemAt(i)->widget());
    QDateEdit *d = qobject_cast<QDateEdit *>(ptr->itemAt(i + 1)->widget());
    QLineEdit *line = qobject_cast<QLineEdit *>(ptr->itemAt(i + 2)->widget());
    QDate date = d->date();
    double val = line->text().toDouble();
    int freq = box->currentIndex();
    while (date <= end) {
      map[date] += val * x;
      if (freq == 0) {
        date = end;
        date = date.addDays(1);
      } else if (freq == 1) {
        date = date.addMonths(1);
      } else if (freq == 2) {
        date = date.addMonths(2);
      } else if (freq == 3) {
        date = date.addMonths(3);
      } else if (freq == 4) {
        date = date.addMonths(6);
      } else if (freq == 5) {
        date = date.addYears(1);
      }
    }
  }
}

void DepositView::InsertPay(int row, QDate date, double add, double rest) {
  QTableWidgetItem *ptwi = 0;
  ui->table_pay->insertRow(row);
  ptwi = new QTableWidgetItem(date.toString("dd.MM.yyyy"));
  ui->table_pay->setItem(row, 0, ptwi);
  if (rest + add >= 0.01) {
    ptwi = new QTableWidgetItem(QString::number(add, 'f', 2));
    if (add > 0) ptwi->setForeground(QBrush(Qt::green));
    if (add < 0) ptwi->setForeground(QBrush(Qt::red));
    ui->table_pay->setItem(row, 2, ptwi);
    ptwi = new QTableWidgetItem(QString::number(rest + add, 'f', 2));
    ui->table_pay->setItem(row, 3, ptwi);
  } else {
    ptwi = new QTableWidgetItem(QString::number(add, 'f', 2) +
                                " снять невозможно");
    ptwi->setForeground(QBrush(Qt::red));
    ui->table_pay->setItem(row, 2, ptwi);
  }
}
void DepositView::FillTableView(int count, int row, QDate current, double per,
                                double add, double rest) {
  QTableWidgetItem *ptwi = 0;
  if (ui->table_pay->rowCount() == 0) {
    ui->table_pay->setVisible(true);
    QStringList str = {"Дата", "Начислено процентов", "Вклад пополнен",
                       "Остаток на вкладе"};
    ui->table_pay->setRowCount(count + 1);
    ui->table_pay->setColumnCount(4);
    ui->table_pay->horizontalHeader()->show();
    ui->table_pay->setHorizontalHeaderLabels(str);
    ui->table_pay->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->table_pay->horizontalHeader()->setSectionResizeMode(0,
                                                            QHeaderView::Fixed);
    ui->table_pay->horizontalHeader()->resizeSection(0, 80);
    ui->table_pay->setSortingEnabled(false);
    ui->table_pay->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_pay->setEditTriggers(QAbstractItemView::EditTriggers(0));
    ptwi = new QTableWidgetItem(current.toString("dd.MM.yyyy"));
    ui->table_pay->setItem(row, 0, ptwi);
    ptwi = new QTableWidgetItem(QString::number(add, 'f', 2));
    ui->table_pay->setItem(row, 2, ptwi);
    ptwi = new QTableWidgetItem(QString::number(rest, 'f', 2));
    ui->table_pay->setItem(row, 3, ptwi);
  } else if (row < count) {
    ptwi = new QTableWidgetItem(current.toString("dd.MM.yyyy"));
    ui->table_pay->setItem(row, 0, ptwi);
    ptwi = new QTableWidgetItem(QString::number(per, 'f', 2));
    ui->table_pay->setItem(row, 1, ptwi);
    ptwi = new QTableWidgetItem(QString::number(add, 'f', 2));
    ui->table_pay->setItem(row, 2, ptwi);
    ptwi = new QTableWidgetItem(QString::number(rest, 'f', 2));
    ui->table_pay->setItem(row, 3, ptwi);
  } else if (row == count) {
    ui->table_pay->verticalHeader()->setSectionResizeMode(row,
                                                          QHeaderView::Fixed);
    ui->table_pay->verticalHeader()->resizeSection(row, 40);
    ptwi = new QTableWidgetItem("Итого:");
    ui->table_pay->setItem(row, 0, ptwi);
    ptwi = new QTableWidgetItem(QString::number(per, 'f', 2) +
                                "\n(Сумма начисленных процентов)");
    ui->table_pay->setItem(row, 1, ptwi);
    ptwi = new QTableWidgetItem(QString::number(rest, 'f', 2) +
                                "\n(Остаток на вкладе)");
    ui->table_pay->setItem(row, 3, ptwi);
  }
}

void DepositView::FillTaxView(int year, double profit, double tax_rate) {
  ui->table_tax->setVisible(true);
  if (ui->table_tax->rowCount() == 0) {
    QStringList str = {"Год",          "Доход",
                       "Вычет",        "Доход после вычета",
                       "Сумма налога", "Оплатить до"};
    ui->table_tax->setColumnCount(6);
    ui->table_tax->horizontalHeader()->show();
    ui->table_tax->setHorizontalHeaderLabels(str);
    ui->table_tax->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->table_tax->horizontalHeader()->setSectionResizeMode(0,
                                                            QHeaderView::Fixed);
    ui->table_tax->horizontalHeader()->resizeSection(0, 40);
    ui->table_tax->setSortingEnabled(false);
    ui->table_tax->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_tax->setEditTriggers(QAbstractItemView::EditTriggers(0));
  }
  QTableWidgetItem *ptwi = 0;
  int row = ui->table_tax->rowCount();
  ui->table_tax->insertRow(row);
  ptwi = new QTableWidgetItem(QString::number(year));
  ui->table_tax->setItem(row, 0, ptwi);
  ptwi = new QTableWidgetItem(QString::number(profit, 'f', 2));
  ui->table_tax->setItem(row, 1, ptwi);
  ptwi = new QTableWidgetItem("160000.00");
  ui->table_tax->setItem(row, 2, ptwi);
  ptwi = new QTableWidgetItem(QString::number(profit - 160000, 'f', 2));
  ui->table_tax->setItem(row, 3, ptwi);
  ptwi = new QTableWidgetItem(
      QString::number((profit - 160000) * tax_rate, 'f', 2));
  ui->table_tax->setItem(row, 4, ptwi);
  ptwi = new QTableWidgetItem("1 декабря " + QString::number(year + 1));
  ui->table_tax->setItem(row, 5, ptwi);
}
}  // end namespace s21
