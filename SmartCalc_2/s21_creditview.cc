#include "s21_creditview.h"

#include "ui_s21_creditview.h"

namespace s21 {
CreditView::CreditView(s21::Controller *controller, QWidget *parent)
    : c(controller), QDialog(parent), ui(new Ui::CreditView) {
  ui->setupUi(this);
  QRegularExpression reg_a(R"(^[1-9][0-9]{0,12}(\.\d{0,2})?$)");
  QRegularExpressionValidator *val_a =
      new QRegularExpressionValidator(reg_a, this);
  ui->line_amount->setValidator(val_a);
  QRegularExpression reg_t(R"(^([1-4]\d?|[5]0?|[6-9])$)");
  QRegularExpressionValidator *val_t =
      new QRegularExpressionValidator(reg_t, this);
  ui->line_term->setValidator(val_t);
  QRegularExpression reg_r(
      R"(^([0]\.([1-9]\d{0,2}|0[1-9]\d?)|[1-9]\d{0,2}(\.\d{0,3})?)$)");
  QRegularExpressionValidator *val_r =
      new QRegularExpressionValidator(reg_r, this);
  ui->line_rate->setValidator(val_r);
  this->resize(550, 220);
  ui->table->setVisible(false);
  ui->graf->setVisible(false);
  ui->line_t->setVisible(false);
  ui->line_r->setVisible(false);
  ui->graf->setBackground(QBrush(QColor(50, 50, 50)));
  ui->graf->xAxis2->setVisible(false);
  ui->graf->yAxis2->setVisible(false);
  ui->graf->xAxis->setVisible(false);
  ui->graf->yAxis->setVisible(false);

  ui->graf->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);
  ui->graf->legend->setBrush(QColor(0, 0, 0, 0));
  ui->graf->legend->setBorderPen(Qt::NoPen);
  ui->graf->legend->setTextColor(Qt::white);
  QFont legendFont = font();
  legendFont.setPointSize(12);
  ui->graf->legend->setFont(legendFont);
  ui->graf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  connect(ui->calc, SIGNAL(clicked()), this, SLOT(PushCalc()));
}

CreditView::~CreditView() { delete ui; }

void CreditView::PushCalc() {
  ui->table->clear();
  ui->table->setRowCount(0);
  ui->table->horizontalHeader()->hide();
  ui->table->verticalHeader()->hide();
  if (ui->graf->legend->itemCount() > 0) ui->graf->legend->clearItems();
  ui->graf->replot();
  ui->graf->setVisible(false);
  on_line_term_textEdited(ui->line_term->text());
  on_line_rate_textEdited(ui->line_rate->text());
  if (ui->line_term->hasAcceptableInput() &&
      ui->line_amount->hasAcceptableInput() &&
      ui->line_rate->hasAcceptableInput()) {
    this->resize(750, 590);
    double amount = ui->line_amount->text().toDouble();
    double rate = ui->line_rate->text().toDouble() / 100;
    int term = ui->line_term->text().toInt();
    if (ui->box_term->currentIndex() == 0) term *= 12;
    SettingTable();

    QStringList month = {"Январь",   "Февраль", "Март",   "Апрель",
                         "Май",      "Июнь",    "Июль",   "Август",
                         "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    int type = ui->radioA->isChecked();
    c->SetData(amount, term, rate, type);
    QDate current = QDate::currentDate();
    std::vector<double> arr(6);
    for (int i = 0; i < term; i++) {
      c->CalcCredit(type, arr);
      QString date =
          month[current.month() - 1] + " " + QString("%1").arg(current.year());
      current = current.addMonths(1);
      InputData(i, date, arr);
    }
    InputSummary(term, arr);
    ui->graf->setVisible(true);
    HorizontalBar(arr[4], arr[5]);
  } else {
    ui->table->setVisible(false);
    this->resize(550, 220);
  }
}

// Input QTableWidgetItem
void CreditView::InputData(int i, QString date, std::vector<double> &arr) {
  QTableWidgetItem *ptwi = 0;
  ptwi = new QTableWidgetItem(date);
  ui->table->setItem(i, 0, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[0], 'f', 2));
  ui->table->setItem(i, 1, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[2], 'f', 2));
  ui->table->setItem(i, 2, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[1], 'f', 2));
  ui->table->setItem(i, 3, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[3], 'f', 2));
  ui->table->setItem(i, 4, ptwi);
}
void CreditView::InputSummary(int term, std::vector<double> &arr) {
  QTableWidgetItem *ptwi = 0;
  ptwi = new QTableWidgetItem(QString::number(arr[4] + arr[5], 'f', 2) +
                              "\n(Общая выплата)");
  ui->table->setItem(term, 1, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[4], 'f', 2) +
                              "\n(Выплаченный долг)");
  ui->table->setItem(term, 2, ptwi);
  ptwi = new QTableWidgetItem(QString::number(arr[5], 'f', 2) +
                              "\n(Переплата по кредиту)");
  ui->table->setItem(term, 3, ptwi);
}
// Setting table
void CreditView::SettingTable() {
  QStringList str = {"Месяц", "Сумма платежа", "по основному долгу",
                     "по процентам", "Остаток долга"};
  QStringList str_v;
  int term = ui->line_term->text().toInt();
  if (ui->box_term->currentIndex() == 0) term *= 12;
  for (int k = 0; k < term + 1; k++) {
    if (k < term)
      str_v << QString("%1").arg(k + 1);
    else
      str_v << "";
  }
  ui->table->setVisible(true);
  ui->table->setRowCount(term + 1);
  ui->table->setColumnCount(5);
  ui->table->horizontalHeader()->show();
  ui->table->verticalHeader()->show();
  ui->table->setHorizontalHeaderLabels(str);
  ui->table->setVerticalHeaderLabels(str_v);
  ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui->table->verticalHeader()->setSectionResizeMode(term, QHeaderView::Fixed);
  ui->table->verticalHeader()->resizeSection(term, 40);
  ui->table->setSortingEnabled(false);
  ui->table->setSelectionMode(QAbstractItemView::NoSelection);
  ui->table->setEditTriggers(QAbstractItemView::EditTriggers(0));
}

// Change term
void CreditView::on_box_term_currentIndexChanged(int index) {
  QRegularExpression reg_t(R"(^([1-4]\d?|[5]0?|[6-9])$)");
  const QValidator *t = ui->line_term->validator();
  delete t;
  if (index == 0) {
    ui->line_term->setPlaceholderText("от 1 до 50");
  } else {
    reg_t.setPattern(R"(^([1-9]\d?|[6]00|[1-5]\d{0,2})$)");
    ui->line_term->setPlaceholderText("от 1 до 600");
  }
  QRegularExpressionValidator *val_t =
      new QRegularExpressionValidator(reg_t, this);
  ui->line_term->setValidator(val_t);
  on_line_term_textEdited(ui->line_term->text());
}

void CreditView::on_line_term_textEdited(const QString &arg1) {
  if (ui->line_term->hasAcceptableInput()) {
    if (arg1 != "") {
      ui->line_t->setVisible(false);
      ui->line_t->setText("");
    }
  } else if (!ui->line_term->hasAcceptableInput()) {
    ui->line_t->setVisible(true);
    if (arg1 == "") {
      ui->line_t->setText("Заполните это поле");
    } else {
      if (ui->box_term->currentIndex() == 0) {
        ui->line_t->setText("от 1 до 50");
      } else {
        ui->line_t->setText("от 1 до 600");
      }
    }
  }
}
// Rate change
void CreditView::on_line_rate_textEdited(const QString &arg1) {
  if (ui->line_rate->hasAcceptableInput()) {
    if (arg1 != "") {
      ui->line_r->setVisible(false);
      ui->line_r->setText("");
    }
  } else if (!ui->line_rate->hasAcceptableInput()) {
    ui->line_r->setVisible(true);
    if (arg1 == "") {
      ui->line_r->setText("Заполните это поле");
    } else {
      ui->line_r->setText("от 0.01 до 999.999");
    }
  }
}
// Graf
void CreditView::HorizontalBar(double cr, double pr) {
  QCPAxis *key = ui->graf->yAxis;
  QCPAxis *value = ui->graf->xAxis;
  QCPBars *credit_all = new QCPBars(key, value);
  QCPBars *proc_all = new QCPBars(key, value);
  credit_all->setAntialiased(false);
  proc_all->setAntialiased(false);
  credit_all->setWidthType(QCPBars::WidthType::wtPlotCoords);
  proc_all->setWidthType(QCPBars::WidthType::wtPlotCoords);
  credit_all->setWidth(1);
  proc_all->setWidth(1);
  ui->graf->legend->setIconSize(18, 9);
  ui->graf->legend->setMargins(QMargins{2, 1, 2, 1});
  credit_all->setName("Кредит ");
  credit_all->setPen(QPen(QColor(101, 201, 122).lighter(250)));
  credit_all->setBrush(QColor(101, 201, 122));
  proc_all->setName("Переплата ");
  proc_all->setPen(QPen(QColor(247, 203, 82).lighter(250)));
  proc_all->setBrush(QColor(247, 203, 82));
  QSharedPointer<QCPAxisTicker> tick(new QCPAxisTicker);
  tick->setTickCount(3);
  key->setTicker(tick);
  value->setTicker(tick);
  key->setSubTicks(false);
  value->setSubTicks(false);
  key->setTickLength(0, 2);
  key->setRange(0, 6);
  value->setRange(0, cr + pr);
  credit_all->addData(0, cr);
  proc_all->addData(0, pr);
  proc_all->moveAbove(credit_all);
  proc_all->setStackingGap(-1);
  ui->graf->legend->setVisible(true);
  // ui->graf->legend->setA
  ui->graf->replot();
}
}  // end namespace s21
