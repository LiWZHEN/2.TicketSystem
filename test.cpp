#include <iostream>
#include "fixed_Chinese.h"
#include "fixed_str.h"

int main() {
  std::string str("A123B_kaj_");
  fixed_str<1, 20, allow_underline,underline_group_num> fixed1(str);

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

  fixed_str fixed2(fixed1);
  fixed1 = "change1";
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