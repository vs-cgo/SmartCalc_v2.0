#include "s21_model.h"

namespace s21 {
// Constructors
// Default
Model::Model() noexcept : data(0), varx(0){};
// Initializer
Model::Model(double x) : data(x), varx(0){};
// Copy ctor
Model::Model(const Model &other) : data(other.data){};
// Move ctor
Model::Model(Model &&other) : data(other.data){};
// Destructor
Model::~Model(){};
// Assignment copy
Model &Model::operator=(const Model &rhs) {
  data = rhs.data;
  return *this;
};
// Assignment move
Model &Model::operator=(const Model &&rhs) {
  data = rhs.data;
  return *this;
};

// Methods

// Lexer divide string to lexem
void Model::Lexer(std::string &str) {
  Clear();
  SkipSpace(str);

  int bracket = 0;
  while (str.size() > 0) {
    double value = 0;
    size_t len = 0;
    Operation oper = Check(str);
    if (oper == kOpen) bracket++;
    if (oper == kClosed) bracket--;

    // если не нашел операцию или число бросает объект incorrect_string
    if (oper == kError || bracket < 0) {
      lexem.clear();
      throw Incorrect_string();
    }

    if (oper == kDigit) {
      value = stod(str, &len);
    } else {
      if (((oper >= 12 && oper <= 23) || oper == 2) &&
          (lexem.size() && lexem.back().oper == 1)) {
        lexem.push_back(Token{Operation::kMult, value});
      }
      len = Length(oper);
    }
    lexem.push_back(Token{oper, value});
    str.erase(0, len);
  }

  for (; bracket > 0; --bracket) {
    lexem.push_back(Token{Operation::kClosed, 0.0});  // add ')' brackets
  }
}

// Validation check incorrect input
void Model::Validation() {
  int digit = 0, op = 0, fun = 0, uno = 0;
  for (auto s = lexem.begin(), e = lexem.end(); s != e; ++s) {
    Operation h_oper = (*s).oper;
    if (h_oper == kDigit || (h_oper >= 21 && h_oper <= 23)) {
      digit == 1 ? (throw Incorrect_string()) : (digit = 1);
      op = 0;
      fun = 0;
      uno = 0;
    } else if (h_oper >= 2) {
      auto temp = s;
      if (h_oper == 2 || (h_oper >= 12 && h_oper <= 20)) {
        if (digit == 1) throw Incorrect_string();
        if (h_oper >= 12 && h_oper <= 20) {
          if (++temp == e || (*temp).oper != 2) throw Incorrect_string();
          fun = 1;
        }
        uno = 0;
      } else if (h_oper == kClosed) {
        if (op == 1) throw Incorrect_string();
      } else if (h_oper == kPro) {
        if (digit == 0) throw Incorrect_string();
      } else if (h_oper == kPlus || h_oper == kMinus) {
        if (op == 0 && digit) {
          digit = 0;
          op = 1;
        } else if (uno == 0) {
          (*s).oper = kUno;
          if (h_oper == kPlus) lexem.erase(s--);
          // (*s).value = (h_oper == kPlus) ? 1 : -1;
          uno = 1;
        } else {
          throw Incorrect_string();  // нельзя типа 1+++2, но можно 1+(+(+2))
        }
      } else if (h_oper >= kMult && h_oper <= kMod) {
        if (digit == 0) throw Incorrect_string();
        digit = 0;
        op = 1;
      }
    }
  }
  if (op == 1 || fun == 1 || uno == 1 || digit == 0) throw Incorrect_string();
}
// SortYard make rpn
void Model::SortYard() {
  std::list<Token> temp, stack;
  for (std::list<Token>::iterator s = lexem.begin(), e = lexem.end(); s != e;
       ++s) {
    Operation oper = (*s).oper;
    if (oper == kDigit || oper == kPro || (oper >= kE && oper <= kVar)) {
      temp.push_back(*s);
      if (stack.size() > 0 && stack.back().oper == kUno) {
        temp.push_back(stack.back());
        stack.pop_back();
      }
    } else if (oper == kOpen || oper == kUno ||
               (oper >= kSqrt && oper <= kAtan)) {
      stack.push_back(*s);
    } else if (oper >= kPlus) {
      int ipr = Priority(*s);
      if (stack.size() > 0) {
        for (int spr = Priority(stack.back());
             spr > ipr || (spr == ipr && IsLeft(oper));) {
          temp.push_back(stack.back());
          stack.pop_back();
          spr = Priority(stack.back());
        }
      }
      stack.push_back(*s);
    } else if (oper == kClosed) {
      for (Token i = stack.back(); stack.size() > 0 && i.oper != kOpen;) {
        temp.push_back(i);
        stack.pop_back();
        i = stack.back();
      }
      if (stack.size() == 0) throw Incorrect_string();
      stack.pop_back();
      if (stack.size() > 0 && IsFunc(stack.back().oper)) {
        temp.push_back(stack.back());
        stack.pop_back();
      }
      if (stack.size() > 0 && stack.back().oper == kUno) {
        temp.push_back(stack.back());
        stack.pop_back();
      }
    }
  }
  for (size_t i = stack.size(); i > 0; --i) {
    temp.push_back(stack.back());
    stack.pop_back();
  }
  lexem.swap(temp);
}
// Calculator
void Model::Calculate() {
  data = 0;
  // print(lexem);
  using iterator = std::list<Token>::iterator;
  std::list<Token> copy(lexem);
  for (iterator s = copy.begin(), e = copy.end(); s != e; ++s) {
    Operation oper = (*s).oper;
    if (oper == kPi) {
      (*s).value = M_PI;
    } else if (oper == kE) {
      (*s).value = M_E;
    } else if (oper == kVar) {
      (*s).value = varx;
    } else if (oper >= kPlus) {
      int o = IsOperation(oper);
      iterator temp = s--;
      if (o == 1) {
        if (s != e) {
          Unary((*s).value, oper);
          copy.erase(temp);
        } else {
          throw Incorrect_string();
        }
      } else if (o == 2) {
        iterator temp2 = s--;
        if (temp2 != e && s != e) {
          Binary((*s).value, (*temp2).value, oper);
          copy.erase(temp);
          copy.erase(temp2);
        } else {
          throw Incorrect_string();
        }
      } else {
        throw Incorrect_string();
      }
    }
    // print(copy);
  }
  if (copy.size() != 1 ||
      (copy.front().oper >= kOpen && copy.front().oper <= kAtan))
    throw Incorrect_string();
  data = copy.front().value;
}

// Unary operations
void Model::Unary(double &value, const Operation &oper) {
  if (oper == kUno) {
    value *= -1.0;
  } else if (oper == kLn) {
    if (value <= 0) throw Uncertain_result();
    value = log(value);
  } else if (oper == kLog) {
    if (value <= 0) throw Uncertain_result();
    value = log10(value);
  } else if (oper == kSqrt) {
    if (value < 0) throw Uncertain_result();
    value = sqrt(value);
  } else if (oper == kPro) {
    value *= 0.01;
  } else if (oper == kSin) {
    value = sin(value);
  } else if (oper == kCos) {
    value = cos(value);
  } else if (oper == kTan) {
    value = tan(value);
  } else if (oper == kAsin) {
    if (fabs(value) > 1) throw Uncertain_result();
    value = asin(value);
  } else if (oper == kAcos) {
    if (fabs(value) > 1) throw Uncertain_result();
    value = acos(value);
  } else if (oper == kAtan) {
    value = atan(value);
  }
}

// Binary operations
void Model::Binary(double &lhs, double &rhs, const Operation &oper) {
  if (oper == kPlus) {
    lhs += rhs;
  } else if (oper == kMinus) {
    lhs -= rhs;
  } else if (oper == kMult) {
    lhs *= rhs;
  } else if (oper == kDiv) {
    if (rhs == 0) throw DivByZero();
    lhs /= rhs;
  } else if (oper == kPow) {
    if (lhs < 0 && fmod(rhs, 1) > 0) throw Uncertain_result();
    lhs = pow(lhs, rhs);
  } else if (oper == kMod) {
    if (rhs == 0) throw DivByZero();
    lhs = fmod(lhs, rhs);
  }
}

// Operation 1 - unary, 2 - binary
int Model::IsOperation(const Operation &x) {
  int res = 0;
  if ((x >= kPlus && x <= kMod) && x != kUno) {
    res = 2;
  } else if ((x >= kPro && x <= kAtan) || x == kUno) {
    res = 1;
  }
  return res;
}

// Check find lexem in string
Operation Model::Check(std::string &str) {
  Operation res = kError;
  if (str.size() > 0) {
    if (isdigit(str[0]) || (str[0] == 46 && isdigit(str[1]))) {
      res = kDigit;
    } else if (!str.compare(0, 1, "e")) {
      res = kE;
    } else if (!str.compare(0, 1, "p")) {
      res = kPi;
    } else if (!str.compare(0, 1, "x")) {
      res = kVar;
    } else if (!str.compare(0, 1, "(")) {
      res = kOpen;
    } else if (!str.compare(0, 1, ")")) {
      res = kClosed;
    } else if (!str.compare(0, 1, "+")) {
      res = kPlus;
    } else if (!str.compare(0, 1, "-")) {
      res = kMinus;
    } else if (!str.compare(0, 1, "~")) {  // заменяю банарный +- унарный
      res = kUno;
    } else if (!str.compare(0, 1, "*")) {
      res = kMult;
    } else if (!str.compare(0, 1, "/")) {
      res = kDiv;
    } else if (!str.compare(0, 1, "^")) {
      res = kPow;
    } else if (!str.compare(0, 3, "mod")) {
      res = kMod;
    } else if (!str.compare(0, 1, "%")) {
      res = kPro;
    } else if (!str.compare(0, 4, "sqrt")) {
      res = kSqrt;
    } else if (!str.compare(0, 2, "ln")) {
      res = kLn;
    } else if (!str.compare(0, 3, "log")) {
      res = kLog;
    } else if (!str.compare(0, 3, "sin")) {
      res = kSin;
    } else if (!str.compare(0, 3, "cos")) {
      res = kCos;
    } else if (!str.compare(0, 3, "tan")) {
      res = kTan;
    } else if (!str.compare(0, 4, "asin")) {
      res = kAsin;
    } else if (!str.compare(0, 4, "acos")) {
      res = kAcos;
    } else if (!str.compare(0, 4, "atan")) {
      res = kAtan;
    }
  }
  return res;
}
// Length
int Model::Length(Operation x) {
  int len = 0;
  if ((x >= 2 && x <= 9) || x == 11 || (x >= 21 && x <= 23)) {
    len = 1;
  } else if (x == 13) {
    len = 2;
  } else if (x == 10 || (x >= 14 && x <= 17)) {
    len = 3;
  } else if (x == 12 || (x >= 18 && x <= 20)) {
    len = 4;
  }
  return len;
}
// Priority
int Model::Priority(const Token &l) {
  int res = 0;
  Operation x = l.oper;
  if (x >= 4 && x <= 6) {
    res = 1;
  } else if (x == 7 || x == 8 || x == 10 || x == 11) {
    res = 2;
  } else if (x >= 13 && x <= 20) {
    res = 3;
  } else if (x == 9 || x == 12) {
    res = 4;
  }
  return res;
}
// IsLeft
bool Model::IsLeft(const Operation &x) {
  return ((x >= 4 && x <= 11) && x != 6) ? true : false;
}
// IsFunc
bool Model::IsFunc(const Operation &x) {
  return (x >= 12 && x <= 20) ? true : false;
}
// SkipSpace
void Model::SkipSpace(std::string &str) {
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}
// Clear
void Model::Clear() { lexem.clear(); }
// Size
size_t Model::Size() { return lexem.size(); }

double Model::GetAns() { return data; }
double Model::GetVar() { return varx; }
std::string Model::GetStr() { return string; }

void Model::SetAns(double x) { data = x; }
void Model::SetVarx(double x) { varx = x; }
void Model::SetStr(const std::string &str) { string = str; }

void Model::print(std::list<Token> &list) const noexcept {
  for (auto m = list.begin(), e = list.end(); m != e; ++m) {
    if ((*m).oper == 1)
      std::cout << (*m).value;
    else
      std::cout << '(' << (*m).oper << ':' << (*m).value << ')';
  }
  std::cout << '\n';
}
}  // end namespace s21
