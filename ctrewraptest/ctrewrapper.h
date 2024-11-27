#ifndef CTREWRAPPER_H
#define CTREWRAPPER_H

#include "ctre.hpp"
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::string extract_number(const std::string& mystring) {
  std::string_view s = mystring;
  if (auto m = ctre::match<"([0-9]+)">(s)) {
    return std::string(m.get<1>().to_view());
    }
  else {
    return "";
    }
  }

std::vector<std::string>
extract_number_and_letter(const std::string& mystring) {
  std::string_view s = mystring;
  auto m = ctre::match<"([0-9])([A-Z])">(s);
  std::vector<std::string> tempvec;
  tempvec.reserve(2);
  if (m) {
    tempvec.emplace_back(m.get<1>().to_view());
    tempvec.emplace_back(m.get<2>().to_view());
    }
  return tempvec;
  }

typedef struct mydate {
  std::string year;
  std::string month;
  std::string day;
  } date;

date extract_date(const std::string& mystring) noexcept {
  std::string_view s = mystring;
  auto result =
    ctre::match<"^.*(?<year>\\d{4})/(?<month>\\d{1,2})/(?<day>\\d{1,2}).*$">(
        s);
  if (!result) {
    return date{ "NA", "NA", "NA" };
    }
  std::string y = std::string(result.get<"year">());
  std::string m = std::string(result.get<"month">());
  std::string d = std::string(result.get<"day">());
  return date{ y, m, d };
  }

int testctre() {
  std::string teststring = "123345";
  std::string teststring2 = "1B";
  std::string datestring = "here is a date 2021/01/01 somewhere";
  std::string datestring2 = "here is NO date anywhere";

  std::cout << extract_number(teststring) << '\n';
  auto testvec = extract_number_and_letter(teststring2);
  for (const auto& st : testvec) {
    std::cout << st << " " << '\n';
    }
  date d = extract_date(datestring);
  std::cout << d.year << " " << d.month << " " << d.day << '\n';
  date d2 = extract_date(datestring2);
  std::cout << d2.year << " " << d2.month << " " << d2.day << '\n';
  return 0;
  }

#endif // CTREWRAPPER_H