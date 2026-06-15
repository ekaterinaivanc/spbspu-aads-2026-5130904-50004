#include <cctype>
#include <ctime>
#include "utils.hpp"

bool ivantsova::isValidBirthday(const std::string& date)
{
  if (date.size() != 10) {
    return false;
  }
  if (date[2] != '.' || date[5] != '.') {
    return false;
  }
  for (size_t i = 0; i < date.size(); ++i) {
    if (i == 2 || i == 5) {
      continue;
    }
    if (!std::isdigit(static_cast< unsigned char >(date[i]))) {
      return false;
    }
  }

  int day = (date[0] - '0') * 10 + (date[1] - '0');
  int month = (date[3] - '0') * 10 + (date[4] - '0');
  int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

  if (year < 1900 || year > 2026) {
    return false;
  }
  if (month < 1 || month > 12) {
    return false;
  }
  if (day < 1 || day > daysInMonth(month, year)) {
    return false;
  }
  return true;
}

int ivantsova::daysInMonth(int month, int year)
{
  if (month == 2) {
    return isLeapYear(year) ? 29 : 28;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  }
  return 31;
}

bool ivantsova::isLeapYear(int year)
{
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int ivantsova::calculateAge(const std::string& birthday)
{
  int day = (birthday[0] - '0') * 10 + (birthday[1] - '0');
  int month = (birthday[3] - '0') * 10 + (birthday[4] - '0');
  int year = (birthday[6] - '0') * 1000 + (birthday[7] - '0') * 100 + (birthday[8] - '0') * 10 + (birthday[9] - '0');

  std::time_t t = std::time(nullptr);
  std::tm* now = std::localtime(&t);
  int currentYear = now->tm_year + 1900;
  int currentMonth = now->tm_mon + 1;
  int currentDay = now->tm_mday;

  int age = currentYear - year;
  if (month > currentMonth || (month == currentMonth && day > currentDay)) {
    --age;
  }
  return age;
}

void ivantsova::trim(std::string& str)
{
  if (str.empty()) {
    return;
  }
  size_t start = 0;
  while (start < str.size() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
    ++start;
  }
  size_t end = str.size() - 1;
  while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
    --end;
  }
  std::string result = "";
  for (size_t i = start; i <= end; ++i) {
    result += str[i];
  }
  str = result;
}
