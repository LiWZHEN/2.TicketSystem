#ifndef FIXED_STR_H
#define FIXED_STR_H

#include <iostream>

template <int max_len>
struct fixed_str {
private:
  char str[max_len]{};
  int len = 0;

public:
  fixed_str() = default;

  fixed_str(const fixed_str &other) {
    len = other.len;
    for (int i = 0; i < max_len; ++i) {
      str[i] = other.str[i];
    }
  }

  explicit fixed_str(const std::string &string) {
    len = static_cast<int>(string.length());
    for (int i = 0; i < len; ++i) {
      str[i] = string[i];
    }
  }

  fixed_str &operator=(const std::string &string) {
    len = static_cast<int>(string.length());
    for (int i = 0; i < len; ++i) {
      str[i] = string[i];
    }
    return *this;
  }

  fixed_str &operator=(const fixed_str &other) {
    if (this == &other) {
      return *this;
    }
    len = other.len;
    for (int i = 0; i < max_len; ++i) {
      str[i] = other.str[i];
    }
    return *this;
  }

  bool operator==(const fixed_str &other) const {
    if (len != other.len) {
      return false;
    }
    for (int i = 0; i < len; ++i) {
      if (str[i] != other.str[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator>(const fixed_str &other) const {
    for (int i = 0; i < len; ++i) {
      if (i >= other.len) {
        return true;
      }
      if (str[i] > other.str[i]) {
        return true;
      }
      if (str[i] < other.str[i]) {
        return false;
      }
    }
    return false;
  }

  bool operator<(const fixed_str &other) const {
    return other > *this;
  }

  bool operator>=(const fixed_str &other) const {
    return !(*this < other);
  }

  bool operator<=(const fixed_str &other) const {
    return !(*this > other);
  }

  bool operator!=(const fixed_str &other) const {
    return !(*this == other);
  }

  [[nodiscard]] int length() const {
    return len;
  }

  char operator[](const int ind) const {
    return str[ind];
  }

  void print() const {
    for (int i = 0; i < len; ++i) {
      std::cout << str[i];
    }
  }

  friend std::ostream &operator<<(std::ostream &output, const fixed_str &f) {
    for (int i = 0; i < f.len; ++i) {
      output << f.str[i];
    }
    return output;
  }

  [[nodiscard]] std::string ToString() const {
    std::string string;
    for (int i = 0; i < len; ++i) {
      string.push_back(str[i]);
    }
    return string;
  }
};

#endif //FIXED_STR_H