#ifndef BETTERC___VARIABLE_HPP
#define BETTERC___VARIABLE_HPP

#include <any>
#include <functional>
#include <memory>
#include <iostream>

#include "object.hpp"

class variable {
private:
    std::any m_data;
    std::function<bool(variable const &lhs, variable const &rhs)> m_equals;
    std::function<bool(variable const &lhs, variable const &rhs)> m_less_than;
    std::function<variable(variable const &lhs, variable const &rhs)> m_plus;
    std::function<variable(variable const &lhs, variable const &rhs)> m_minus;
    std::function<variable(variable const &lhs, variable const &rhs)> m_multiply;
    std::function<variable(variable const &lhs, variable const &rhs)> m_divide;
    std::function<variable &(variable &lhs, std::string const &name)> m_get_member;
    std::function<variable &(variable &lhs, std::size_t idx)> m_get_index;
    std::function<std::string(variable const &var)> m_stringify;

    template<typename T>
    auto get_as() noexcept -> T & {
        if constexpr (std::is_same_v<T, object>)
            return **std::any_cast<std::shared_ptr<T>>(&m_data);
        else
            return *std::any_cast<T>(&m_data);
    }

    template<typename T>
    auto get_as() const noexcept -> T const & {
        if constexpr (std::is_same_v<T, object>)
            return **std::any_cast<std::shared_ptr<T>>(&m_data);
        else
            return *std::any_cast<T>(&m_data);
    }

    friend int get_return_code(variable const &);

public:
    variable() noexcept
            : m_data{},
              m_equals{[](variable const &lhs, variable const &rhs) {
                  return lhs.m_data.has_value() == rhs.m_data.has_value();
              }},
              m_less_than{[](variable const &lhs, variable const &rhs) -> bool {
                  throw "none cannot be compared";
              }},
              m_get_member{[](variable &lhs, std::string const &name) -> variable & {
                  throw "none don't have members";
              }},
              m_get_index{[](variable &lhs, std::size_t idx) -> variable & {
                  throw "none don't have members";
              }},
              m_plus{[](variable const &lhs, variable const &rhs) -> variable {
                  throw "none cannot be added";
              }},
              m_minus{[](variable const &lhs, variable const &rhs) -> variable {
                  throw "none cannot be subtracted";
              }},
              m_multiply{[](variable const &lhs, variable const &rhs) -> variable {
                  throw "none cannot be multiplied";
              }},
              m_divide{[](variable const &lhs, variable const &rhs) -> variable {
                  throw "none cannot be divided";
              }},
              m_stringify{[](variable const &var) {
                  return "none";
              }} {}

    variable(int i) noexcept: variable((long double) i) {}

    variable(long double f) noexcept
            : m_data{f},
              m_equals{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() == rhs.get_as<long double>();
              }},
              m_less_than{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() < rhs.get_as<long double>();
              }},
              m_get_member{[](variable &lhs, std::string const &name) -> variable & {
                  throw "numbers don't have members";
              }},
              m_get_index{[](variable &lhs, std::size_t idx) -> variable & {
                  throw "numbers don't have members";
              }},
              m_plus{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() + rhs.get_as<long double>();
              }},
              m_minus{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() - rhs.get_as<long double>();
              }},
              m_multiply{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() * rhs.get_as<long double>();
              }},
              m_divide{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<long double>() / rhs.get_as<long double>();
              }},
              m_stringify{[](variable const &var) {
                  return std::to_string(var.get_as<long double>());
              }} {}

    variable(object &&o) noexcept
            : m_data{std::make_shared<object>(std::move(o))},
              m_equals{[](variable const &lhs, variable const &rhs) {
                  return &lhs.get_as<object>() == &rhs.get_as<object>();
              }},
              m_less_than{
                      [](variable const &lhs, variable const &rhs) -> bool { throw "cannot compare objects"; }},
              m_get_member{[](variable &lhs, std::string name) -> variable & {
                  return lhs.get_as<object>()[name];
              }},
              m_get_index{[](variable &lhs, std::size_t idx) -> variable & {
                  return std::next(lhs.get_as<object>().begin(), idx)->second;
              }},
              m_plus{[](variable const &lhs, variable const &rhs) -> variable {
                  variable copy = lhs;
                  auto &map = copy.get_as<object>();
                  if (rhs.m_data.type() == typeid(std::shared_ptr<object>)) {
                      auto rhsobj = rhs.get_as<object>();
                      map.insert(rhsobj.begin(), rhsobj.end());
                  } else
                      throw "cannot merge";
                  return copy;
              }},
              m_minus{[](variable const &lhs, variable const &rhs) -> variable {
                  variable copy = lhs;
                  auto &map = copy.get_as<object>();
                  if (rhs.m_data.type() == typeid(long double))
                      map.erase(std::next(map.begin(), rhs.get_as<long double>()));
                  else if (rhs.m_data.type() == typeid(std::string))
                      map.erase(rhs.get_as<std::string>());
                  else
                      throw "can't remove element";
                  return copy;
              }},
              m_multiply{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot multiply objects"; }},
              m_divide{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot divide objects"; }},
              m_stringify{[](variable const &var) {
                  auto obj = var.get_as<object>();
                  if (obj.empty())
                      return std::string{"{}"};
                  auto format = [](auto &&o) { return o.first + ": " + o.second.m_stringify(o.second); };
                  if (obj.size() == 1)
                      return "{ " + format(*obj.begin()) + " }";
                  std::string s = "{ " + format(*obj.begin());
                  for (auto iter = std::next(obj.begin()); iter != obj.end(); ++iter)
                      s += ", " + format(*iter);
                  return s + " }";
              }} {}

    template<std::size_t N>
    variable(const char s[N]) noexcept
            : variable(std::string(s)) {}

    variable(char const *s) noexcept
            : variable(std::string(s)) {}

    variable(std::string s) noexcept
            : m_data{std::move(s)},
              m_equals{[](variable const &lhs, variable const &rhs) {
                  return lhs.get_as<std::string>() == rhs.get_as<std::string>();
              }},
              m_less_than{
                      [](variable const &lhs, variable const &rhs) -> bool { throw "cannot compare strings"; }},
              m_get_member{[](variable &lhs, std::string name) -> variable & {
                  throw "strings don't have members";
              }},
              m_get_index{[](variable &lhs, std::size_t idx) -> variable & {
                  throw "strings don't have members";
              }},
              m_plus{[](variable const &lhs, variable const &rhs) -> variable {
                  return lhs.get_as<std::string>() + rhs.toString().get_as<std::string>();
              }},
              m_minus{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot subtract strings"; }},
              m_multiply{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot multiply strings"; }},
              m_divide{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot divide strings"; }},
              m_stringify{[](variable const &var) {
                  return var.get_as<std::string>();
              }} {}

    variable(std::initializer_list<variable> list) noexcept
            : m_data{std::vector<variable>{list.begin(), list.end()}},
              m_equals{
                      [](variable const &lhs, variable const &rhs) {
                          return lhs.get_as<std::vector<variable>>() == rhs.get_as<std::vector<variable>>();
                      }
              },
              m_less_than{
                      [](variable const &lhs, variable const &rhs) -> bool { throw "cannot compare arrays"; }
              },
              m_get_member{
                      [](variable &lhs, std::string name) -> variable & {
                          throw "arrays don't have members";
                      }
              },
              m_get_index{[](variable &lhs, std::size_t idx) -> variable & {
                  return lhs.get_as<std::vector<variable>>()[idx];
              }},
              m_plus{
                      [](variable const &lhs, variable const &rhs) -> variable {
                          variable copy = lhs;
                          auto &vec = copy.get_as<std::vector<variable>>();
                          vec.emplace_back(rhs);
                          return copy;
                      }
              },
              m_minus{[](variable const &lhs, variable const &rhs) -> variable {
                  variable copy = lhs;
                  auto &vec = copy.get_as<std::vector<variable>>();
                  auto const erased = std::erase(vec, rhs);
                  if (erased == 0 && rhs.m_data.type() == typeid(long double))
                      vec.erase(vec.begin() + rhs.get_as<long double>());
                  return copy;
              }},
              m_multiply{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot multiply arrays"; }},
              m_divide{[](variable const &lhs, variable const &rhs) -> variable { throw "cannot divide arrays"; }},
              m_stringify{
                      [](variable const &var) {
                          auto obj = var.get_as<std::vector<variable>>();
                          if (obj.empty())
                              return std::string{"{}"};
                          auto format = [](auto &&o) { return o.m_stringify(o); };
                          if (obj.size() == 1)
                              return "{ " + format(*obj.begin()) + " }";
                          std::string s = "{ " + format(*obj.begin());
                          for (auto iter = std::next(obj.begin()); iter != obj.end(); ++iter)
                              s += ", " + format(*iter);
                          return s + " }";
                      }
              } {}

    variable(variable const &) noexcept = default;

    variable(variable &&) noexcept = default;

    variable &operator=(variable const &) = default;

    variable &operator=(variable &&) = default;

    bool operator==(variable const &other) const noexcept {
        if (m_data.has_value() != other.m_data.has_value()) return false;
        if (m_data.type() != other.m_data.type()) return false;
        return m_equals(*this, other);
    }

    bool operator!=(variable const &other) const noexcept {
        return !operator==(other);
    }

    bool operator<(variable const &other) const noexcept {
        if (m_data.has_value() != other.m_data.has_value())
            throw "cannot compare objects";
        if (m_data.type() != other.m_data.type()) throw "cannot compare objects";
        return m_less_than(*this, other);
    }

    bool operator<=(variable const &other) const noexcept {
        return *this < other || *this == other;
    }

    bool operator>(variable const &other) const noexcept {
        return !(*this <= other);
    }

    bool operator>=(variable const &other) const noexcept {
        return !(*this < other);
    }

    variable &operator[](std::string name) const noexcept {
        return m_get_member(*const_cast<variable *>(this), name);
    }

    variable &operator[](std::size_t idx) const noexcept {
        return m_get_index(*const_cast<variable *>(this), idx);
    }

    variable operator+(variable const &other) const noexcept {
        return m_plus(*this, other);
    }

    variable operator-(variable const &other) const noexcept {
        return m_minus(*this, other);
    }

    variable operator*(variable const &other) const noexcept {
        return m_multiply(*this, other);
    }

    variable operator/(variable const &other) const noexcept {
        return m_divide(*this, other);
    }

    variable &operator+=(variable const &other) noexcept {
        return *this = m_plus(*this, other);
    }

    variable &operator-=(variable const &other) noexcept {
        return *this = m_minus(*this, other);
    }

    variable &operator*=(variable const &other) noexcept {
        return *this = m_multiply(*this, other);
    }

    variable &operator/=(variable const &other) noexcept {
        return *this = m_divide(*this, other);
    }

    variable toString() const noexcept {
        return m_stringify(*this);
    }

    friend void print(variable const &v) noexcept {
        std::cout << v.m_stringify(v) << std::flush;
    }

    friend void println(variable const &v) noexcept {
        std::cout << v.m_stringify(v) << std::endl;
    }
};


auto make_object(std::initializer_list<std::pair<std::string, variable>> list) {
    object o;
    for (auto &&e : list) {
        o[e.first] = e.second;
    }
    return o;
}

#endif //BETTERC___VARIABLE_HPP
