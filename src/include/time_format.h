#ifndef TIME_H
#define TIME_H

namespace Time {

  inline constexpr int day_num[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  struct clock {
    int hour = 0;
    int minute = 0;

    clock() = default;
    clock(const int h, const int m) : hour(h), minute(m) {}
    explicit clock(const std::string &time) {
      hour = (time[0] - '0') * 10 + (time[1] - '0');
      minute = (time[3] - '0') * 10 + (time[4] - '0');
    }
    clock(const clock &other) {
      hour = other.hour;
      minute = other.minute;
    }

    clock &operator=(const clock &other) {
      if (this == &other) {
        return *this;
      }
      hour = other.hour;
      minute = other.minute;
      return *this;
    }

    friend std::ostream &operator<<(std::ostream &output, const clock &c) {
      if (c.hour < 10) {
        output << '0';
      }
      output << c.hour << ':';
      if (c.minute < 10) {
        output << '0';
      }
      output << c.minute;
      return output;
    }
  };

  struct date {
    int month = 0;
    int day = 0;

    date() = default;
    date(const int m, const int d) : month(m), day(d) {}
    explicit date(const std::string &time) {
      month = (time[0] - '0') * 10 + (time[1] - '0');
      day = (time[3] - '0') * 10 + (time[4] - '0');
    }
    date(const date &other) {
      month = other.month;
      day = other.day;
    }

    date &operator=(const date &other) {
      if (this == &other) {
        return *this;
      }
      month = other.month;
      day = other.day;
      return *this;
    }

    bool operator==(const date &other) const {
      return month == other.month && day == other.day;
    }

    friend int operator-(const date &d1, const date &d2) {
      if (d1.month < d2.month) { // the answer is negative
        int ans = day_num[d1.month] - d1.day;
        for (int i = d1.month + 1; i < d2.month; ++i) {
          ans += day_num[i];
        }
        ans += d2.day;
        return -ans;
      }
      if (d1.month == d2.month) {
        return d1.day - d2.day;
      }
      int ans = day_num[d2.month] - d2.day;
      for (int i = d2.month + 1; i < d1.month; ++i) {
        ans += day_num[i];
      }
      ans += d1.day;
      return ans;
    }

    friend std::ostream &operator<<(std::ostream &output, const date &d) {
      if (d.month < 10) {
        output << '0';
      }
      output << d.month << '-';
      if (d.day < 10) {
        output << '0';
      }
      output << d.day;
      return output;
    }
  };

  struct time {
    date month_day;
    clock hour_min;

    time() = default;
    time(const date &d, const clock &c) {
      month_day = d;
      hour_min = c;
    }
    time(const time &other) {
      month_day = other.month_day;
      hour_min = other.hour_min;
    }
    time(const int m, const int d, const int hour, const int min) {
      month_day = {m, d};
      hour_min = {hour, min};
    }

    time operator+(const int minute) const {
      time ans;
      ans.hour_min.minute = hour_min.minute + minute;
      int carry = ans.hour_min.minute / 60;
      ans.hour_min.minute %= 60;
      ans.hour_min.hour = hour_min.hour + carry;
      carry = ans.hour_min.hour / 24;
      ans.hour_min.hour %= 24;
      if (carry == 0) {
        ans.month_day = month_day;
      } else {
        ans.month_day.day = month_day.day + carry;
        ans.month_day.month = month_day.month;
        while (true) {
          if (ans.month_day.day > day_num[ans.month_day.month]) {
            ans.month_day.day -= day_num[ans.month_day.month];
            ans.month_day.month = (ans.month_day.month + 1) % 12;
          } else {
            break;
          }
        }
      }
      return ans;
    }
    time &operator+=(const int minute) {
      hour_min.minute += minute;
      int carry = hour_min.minute / 60;
      hour_min.minute %= 60;
      hour_min.hour += carry;
      carry = hour_min.hour / 24;
      hour_min.hour %= 24;
      if (carry == 0) {
        return *this;
      }
      month_day.day += carry;
      while (true) {
        if (month_day.day > day_num[month_day.month]) {
          month_day.day -= day_num[month_day.month];
          month_day.month = (month_day.month + 1) % 12;
        } else {
          return *this;
        }
      }
    }

    time operator-(const int minute) const {
      time ans;
      ans.hour_min.minute = hour_min.minute - minute;
      if (ans.hour_min.minute >= 0) {
        ans.hour_min.hour = hour_min.hour;
        ans.month_day = month_day;
        return ans;
      }
      int minus = (- ans.hour_min.minute - 1) / 60 + 1;
      ans.hour_min.minute += minus * 60;
      ans.hour_min.hour = hour_min.hour - minus;
      if (ans.hour_min.hour >= 0) {
        ans.month_day = month_day;
        return ans;
      }
      minus = (- ans.hour_min.hour - 1) / 24 + 1;
      ans.hour_min.hour += minus * 24;
      ans.month_day.day = month_day.day - minus;
      ans.month_day.month = month_day.month;
      while (true) {
        if (ans.month_day.day >= 1) {
          return ans;
        }
        ans.month_day.month = (ans.month_day.month + 10) % 12 + 1;
        ans.month_day.day += day_num[ans.month_day.month];
      }
    }
    time &operator-=(const int minute) {
      hour_min.minute -= minute;
      if (hour_min.minute >= 0) {
        return *this;
      }
      int minus = (- hour_min.minute - 1) / 60 + 1;
      hour_min.minute += minus * 60;
      hour_min.hour -= minus;
      if (hour_min.hour >= 0) {
        return *this;
      }
      minus = (- hour_min.hour - 1) / 24 + 1;
      hour_min.hour += minus * 24;
      month_day.day -= minus;
      while (true) {
        if (month_day.day >= 1) {
          return *this;
        }
        month_day.month = (month_day.month + 10) % 12 + 1;
        month_day.day += day_num[month_day.month];
      }
    }

    friend int operator-(const time &t1, const time &t2) {
      const int minute1 = t1.hour_min.hour * 60 + t1.hour_min.minute, minute2 = t2.hour_min.hour * 60 + t2.hour_min.minute;
      const int delta_date = t1.month_day - t2.month_day;
      return delta_date * 1440 + minute1 - minute2;
    }

    friend std::ostream &operator<<(std::ostream &output, const time &t) {
      output << t.month_day << ' ' << t.hour_min;
      return output;
    }
  };
}

#endif //TIME_H
