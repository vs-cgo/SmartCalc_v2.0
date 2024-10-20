#include "s21_controller.h"

namespace s21 {
// Constructo default
Controller::Controller() noexcept
    : model(nullptr), cmodel(nullptr), dmodel(nullptr){};
// Constructor by Model
Controller::Controller(Model *m, CreditModel *cm, DepositModel *dm)
    : model(m), cmodel(cm), dmodel(dm){};
// Destructor
Controller::~Controller() {
  model = nullptr;
  cmodel = nullptr;
  dmodel = nullptr;
};

// Methods
double Controller::Answer(std::string str) {
  try {
    if (str.compare(model->GetStr()) != 0) {
      model->SetStr(str);
      model->Lexer(str);
      model->Validation();
      model->SortYard();
    }
    model->Calculate();
  } catch (Incorrect_string) {
    model->Clear();
    throw;
  } catch (Uncertain_result) {
    model->SetAns();
    throw;
  } catch (DivByZero) {
    model->SetAns();
    throw;
  }
  return model->GetAns();
}

void Controller::SetVarx(double x) { model->SetVarx(x); }
double Controller::GetVar() { return model->GetVar(); }
double Controller::GetAns() { return model->GetAns(); }

// CreditModel
void Controller::SetData(double a, double t, double r, int type) {
  cmodel->SetData(a, t, r);
  cmodel->Init(type);
}
void Controller::CalcCredit(int type, std::vector<double> &arr) {
  if (type) {
    cmodel->Annuity(arr);
  } else {
    cmodel->Different(arr);
  }
}
// DepositModel
void Controller::CalcDeposit() { dmodel->CalcDeposit(); }
double Controller::Payment() { return dmodel->Payment(); }
void Controller::SetData(double a, double r, double t, int d, int c) {
  dmodel->SetData(a, r, t, d, c);
};
void Controller::SetDays(int d) { dmodel->SetDays(d); };
void Controller::SetRest(double val) { dmodel->SetRest(val); };

double Controller::GetRest() { return dmodel->GetRest(); };
double Controller::GetPerc() { return dmodel->GetPerc(); };

}  // end namespace s21
