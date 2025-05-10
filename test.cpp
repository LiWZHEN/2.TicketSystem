#include <iostream>
#include "fixed_Chinese.h"
#include "fixed_str.h"

int main() {
  std::string str("这个");
  fixed_Chinese<1, 5> fixed1(str);

  if (fixed1.is_valid()) {
    std::cout << "valid\n";
  } else {
    std::cout << "invalid\n";
  }

  std::cout << "Length: " << fixed1.length() << '\n';

  std::cout << "Print test ...\n";
  fixed1.print();
  std::cout << std::endl;

  std::cout << fixed1[0] << std::endl;

  fixed_Chinese fixed2(fixed1);
  fixed1 = "中途变化";
  if (fixed1.is_valid()) {
    std::cout << "valid\n";
  } else {
    std::cout << "invalid\n";
  }

  std::cout << "Length: " << fixed1.length() << '\n';

  std::cout << "Print test ...\n";
  fixed1.print();
  std::cout << std::endl;

  std::cout << fixed1[0] << std::endl;

  fixed1 = fixed2;

  if (fixed1.is_valid()) {
    std::cout << "valid\n";
  } else {
    std::cout << "invalid\n";
  }

  std::cout << "Length: " << fixed1.length() << '\n';

  std::cout << "Print test ...\n";
  fixed1.print();
  std::cout << std::endl;

  std::cout << fixed1[0] << std::endl;
  return 0;
}