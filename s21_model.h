#ifndef _SMARTCALC_v2_0_S21_MODEL_H_
#define _SMARTCALC_v2_0_S21_MODEL_H_

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace s21 {
/**
 * @brief The Operation enum для цифрового обозначения типа узла
 */
enum Operation {
  kError = 0,
  kDigit,
  kOpen,
  kClosed,
  kPlus,
  kMinus,
  kUno,
  kMult,
  kDiv,
  kPow,
  kMod,
  kPro,
  kSqrt,
  kLn,
  kLog,
  kSin,
  kCos,
  kTan,
  kAsin,
  kAcos,
  kAtan,
  kE,
  kPi,
  kVar
};
/**
 * @brief The Incorrect_string struct выбрасывается если входная строка не
 * корректна
 */
struct Incorrect_string {};

/**
 * @brief The Uncertain_result struct выбрасывается если результат
 * математического выражения не определен (пр. log(-2))
 */
struct Uncertain_result {};

/**
 * @brief The DivByZero struct выбрасывается если присутсвует деление на 0
 */
struct DivByZero {};
// const long double kPi = 3.14159265358979323846264338327950288;
// const long double kE  = 2.71828182845904523536028747135266250;

// [0-9]. ( ) + - ± * ÷ xª mod % sqrt ln log sin cos tan (asin-atan) e  π   var
//   1    2 3 4 5 6 7 8 9  10  11 12  13 14  15  16  17   18 19 20   e  p    x
/**
 * @brief The Model class для расчета математических выражений
 */
class Model {
 public:
  /**
   * @struct Token
   * @brief агрегат для хранения типа и значения
   */
  struct Token {
    /**
     * @brief enum хранящий тип лексемы
     */
    Operation oper;
    /**
     * @brief значение лексемы (0 если не число)
     */
    double value;
  };
  /**
   * @brief Alias для типа std::string
   */
  using strval = std::string;

  /**
   * @brief Model конструктор по умолчанию
   */
  Model() noexcept;

  /**
   * @brief Model параметризованный конструктор
   * @param[in] x значение переменной х
   */
  Model(double x);

  /**
   * @brief ~Model деструктор
   */
  ~Model();

  /**
   * @brief Model конструктор копирования
   * @param[in] other константная ссылка на модель другого класса
   */
  Model(const Model &other);

  /**
   * @brief Model конструктор перемещения
   * @param[in] other rvalue reference
   */
  Model(Model &&other);

  /**
   * @brief operator = оператор копирования
   * @param[in] rhs модель
   * @return ссылка на левый операнд
   */
  Model &operator=(const Model &rhs);

  /**
   * @brief operator = оператор перемещения
   * @param[in] rhs модель
   * @return ссылка на левый операнд
   */
  Model &operator=(const Model &&rhs);

  // Methods
  /**
   * @brief SkipSpace метод для удаления пробелов из входящей строки
   * @param[in] str ссылка на входящую строку
   */
  void SkipSpace(std::string &str);

  /**
   * @brief Lexer разбивание входящей строки на лексемы
   * @param[in] str строка с математическим выражением
   */
  void Lexer(std::string &str);

  /**
   * @brief Validation валидация полученного списка лексем
   */
  void Validation();

  /**
   * @brief SortYard превращения списка лексем из инфиксной в постфиксную
   * нотацию
   */
  void SortYard();

  /**
   * @brief Calculate рассчет математического выражения
   */
  void Calculate();

  /**
   * @brief Size метод для расчета разера листа лексем который хранит модель
   * @return размер листа
   */
  size_t Size();

  /**
   * @brief Clear метод для очистки списка лексем
   */
  void Clear();

  /**
   * @brief GetAns получение значения ответа предыдущего вычисления(если
   * предыдущего не было или было выброшено исключение, то 0)
   * @return получения значения ответа
   */
  double GetAns();

  /**
   * @brief GetVar получения значения хранящееся в переменной х
   * @return значение х
   */
  double GetVar();

  /**
   * @brief GetStr получения строки последнего вычисления(если не было, то "")
   * @return значение std::string
   */
  strval GetStr();

  /**
   * @brief SetAns передача ответа(по умолчанию 0)
   * @param[in] x значение ответа
   */
  void SetAns(double x = 0);

  /**
   * @brief SetVarx передача значения переменной в модель
   * @param[in] var значение переменной
   */
  void SetVarx(double var);

  /**
   * @brief SetStr запись в модель строки с математическим выражением
   * @param[in] str ссылка на std::string с математическим выражением
   */
  void SetStr(const std::string &str);

  /**
   * @brief print метод для вывода в std::cout списка лексем
   * @param[in] list ссылка на список лексем
   */
  void print(std::list<Token> &list) const noexcept;

 private:
  Operation Check(std::string &str);
  int Length(Operation);
  int Priority(const Token &);
  bool IsLeft(const Operation &);
  bool IsFunc(const Operation &);
  int IsOperation(const Operation &);
  void Unary(double &, const Operation &);
  void Binary(double &, double &, const Operation &);

  std::list<Token> lexem;
  std::string string;
  double data, varx;
};  // end declaration class Model
}  // namespace s21
#endif  // _SMARTCALC_v2_0_S21_MODEL_H_
