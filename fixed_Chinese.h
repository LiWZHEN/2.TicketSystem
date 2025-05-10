#ifndef FIXED_CHINESE_H
#define FIXED_CHINESE_H

#include <string>
#include <stdexcept>
#include <cstdint>

inline std::string To_UTF_8(const char32_t ch) {
  std::string ans;
  if (ch <= 0x7F) { // 1B: 0xxxxxxx
    ans.push_back(static_cast<char>(ch));
  } else if (ch <= 0x7FF) { // 2B: 110xxxxx 10xxxxxx
    ans.push_back(static_cast<char>(0xC0 | ((ch >> 6) & 0x1F)));
    ans.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
  } else if (ch <= 0xFFFF) { // 3B: 1110xxxx 10xxxxxx 10xxxxxx
    ans.push_back(static_cast<char>(0xE0 | ((ch >> 12) & 0x0F)));
    ans.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
    ans.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
  } else if (ch <= 0x10FFFF) { // 4B: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    ans.push_back(static_cast<char>(0xF0 | ((ch >> 18) & 0x07)));
    ans.push_back(static_cast<char>(0x80 | ((ch >> 12) & 0x3F)));
    ans.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
    ans.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
  } else {
    throw std::runtime_error("Invalid Unicode code point");
  }
  return ans;
}

inline char32_t Three_Bytes_to_Char32(const char b0, const char b1, const char b2) {
  const auto u0 = static_cast<uint8_t>(b0);
  const auto u1 = static_cast<uint8_t>(b1);
  const auto u2 = static_cast<uint8_t>(b2);

  if ((u0 & 0xF0) != 0xE0 || (u1 & 0xC0) != 0x80 || (u2 & 0xC0) != 0x80) {
    throw std::runtime_error("Invalid UTF-8 three-byte sequence");
  }

  char32_t cp = 0;
  cp |= (u0 & 0x0F) << 12;
  cp |= (u1 & 0x3F) << 6;
  cp |= (u2 & 0x3F);

  return cp;
}

template <int max_len>
struct fixed_Chinese {
private:
  char32_t str[max_len]{};
  int len = 0;
public:
  fixed_Chinese() = default;

  fixed_Chinese(const fixed_Chinese &other) {
    len = other.len;
    for (int i = 0; i < max_len; ++i) {
      str[i] = other.str[i];
    }
  }

  explicit fixed_Chinese(const std::string &string) {
    len = static_cast<int>(string.length() / 3);
    for (int i = 0; i < len; ++i) {
      str[i] = Three_Bytes_to_Char32(string[3 * i], string[3 * i + 1], string[3 * i + 2]);
    }
  }

  fixed_Chinese &operator=(const std::string &string) {
    len = static_cast<int>(string.length() / 3);
    for (int i = 0; i < len; ++i) {
      str[i] = Three_Bytes_to_Char32(string[3 * i], string[3 * i + 1], string[3 * i + 2]);
    }
    return *this;
  }
  fixed_Chinese &operator=(const fixed_Chinese &other) {
    if (this == &other) {
      return *this;
    }
    len = other.len;
    for (int i = 0; i < max_len; ++i) {
      str[i] = other.str[i];
    }
    return *this;
  }

  bool operator==(const fixed_Chinese &other) const {
    if (len != other.len) {
      return false;
    }
    for (int i = 0; i < len * 3; ++i) {
      if (str[i] != other.str[i]) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] int length() const {
    return len;
  }

  std::string operator[](const int ind) const {
    return To_UTF_8(str[ind]);
  }

  void print() const {
    for (int i = 0; i < len; ++i) {
      std::cout << To_UTF_8(str[i]);
    }
  }

  std::string ToString() {
    std::string string;
    for (int i = 0; i < len; ++i) {
      string.push_back((*this)[i]);
    }
    return string;
  }
};

#endif //FIXED_CHINESE_H
