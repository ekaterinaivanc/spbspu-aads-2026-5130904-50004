#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace ivantsova {

  bool isValidBirthday(const std::string& date);
  int calculateAge(const std::string& birthday);
  void trim(std::string& str);
  bool isLeapYear(int year);
  int daysInMonth(int month, int year);

}

#endif
