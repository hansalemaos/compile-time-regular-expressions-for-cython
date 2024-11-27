# Example for wrapping CTRE [ compile-time-regular-expressions ] in Cython

## A little test to get Cython running using a good (very fast) Regex Engine

C++ has the crappiest regex engine on the planet. Here is a way to use one of the best Regex Engines in Python/Cython

## Download the [newest header only version from CTRE](https://github.com/hanickadot/compile-time-regular-expressions/tree/main/single-header)

### Example wrapper code, all regexes must be defined before compiling, no runtime regexes allowed

```cpp 
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

// to test the code in CPP
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
```

### wrapping the code in Cython

```py 

cimport cython
from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdio cimport printf

cdef extern from "ctrewrapper.h" nogil :
    ctypedef struct mydate:
        string year;
        string month;
        string day;
    ctypedef mydate date 
    vector[string] extract_number_and_letter(const string& mystring)
    date extract_date (const string& mystring)
    string extract_number (const string& mystring)

cdef int functioncalltest(string strtocheck):
    cdef:
        string result0
        vector[string] result1
        date result2
    result0=extract_number(strtocheck);
    result1=extract_number_and_letter(strtocheck);
    result2=extract_date(strtocheck);
    printf("result0: %s\n", result0.c_str())
    if result1.size()==2:
        printf("result1: %s\t%s\n", result1[0].c_str(), result1[1].c_str())
    printf("result2: %s\t%s\t%s\n", result2.year.c_str(), result2.month.c_str(), result2.day.c_str())
    return 1

cdef:
    string teststring = <string>b"123345"
    string teststring2 = <string>b"1B"
    string datestring = <string>b"here is a date 2021/01/01 somewhere"
    string datestring2 = <string>b"here is NO date anywhere"
    int t1=functioncalltest(teststring)
    int t2=functioncalltest(teststring2)
    int t3=functioncalltest(datestring)
    int t4=functioncalltest(datestring2)


```


