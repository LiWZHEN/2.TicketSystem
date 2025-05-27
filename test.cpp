#include <iostream>
#include "time_format.h"

void print(const Time::time &t) {
  std::cout << t.month_day.month << '.' << t.month_day.day << ' ' << t.hour_min.hour << ':' << t.hour_min.minute << '\n';
}

int main() {
  Time::time t(6, 1, 0, 10);
  print(t);
  t -= 90;
  print(t);
  return 0;
}