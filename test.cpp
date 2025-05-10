#include <iostream>
#include "fixed_Chinese.h"
// #include "fixed_str.h"

int main() {
  std::string str[5] = {"汉字串一", "这是站点名", "哇那这很好了", "浙江温州平阳", "上海闵行"};

  for (int i = 0; i < 5; ++i) {
    fixed_Chinese<1, 5> Chinese(str[i]);

    if (Chinese.is_valid()) {
      std::cout << "Chinese valid\n";
    } else {
      std::cout << "Chinese invalid\n";
    }

    std::cout << "Length: " << Chinese.length() << '\n';

    std::cout << "Print test ...\n";
    Chinese.print();
    std::cout << std::endl;

    std::cout << Chinese[0] << std::endl;
  }
  return 0;
}