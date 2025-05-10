#ifndef FIXED_STR_H
#define FIXED_STR_H

template <int min_len, int max_len, char *special_valid, int special_valid_len>
struct fixed_str {
private:
  char str[max_len]{};
  int len = 0;
  bool valid = true;
public:
  explicit fixed_str(const std::string &string) {
    if (string.length() < min_len || string.length() > max_len) { // the length is invalid
      valid = false;
    } else { // the length is valid
      len = static_cast<int>(string.length());
      for (int i = 0; i < len; ++i) {
        str[i] = string[i];
      }
      if (special_valid_len != 0) {
        for (int i = 0; i < len; ++i) {
          if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            continue;
          }
          valid = false;
          for (int j = 0; j < special_valid_len; ++j) {
            if (str[i] == *(special_valid + j)) {
              valid = true;
              break;
            }
          }
          if (!valid) {
            break;
          }
        }
      } else {
        for (int i = 0; i < len; ++i) {
          if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            continue;
          }
          valid = false;
          break;
        }
      }
    }
  }

  [[nodiscard]] int length() const {
    return len;
  }

  [[nodiscard]] bool is_valid() const {
    return valid;
  }

  char operator[](const int ind) const {
    return str[ind];
  }

  void print() const {
    for (int i = 0; i < len; ++i) {
      std::cout << str[i];
    }
  }
};

#endif //FIXED_STR_H