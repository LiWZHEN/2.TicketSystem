#include <iostream>
#include "fixed_str.h"

char allow_underline[1] = {'_'};
char allow_at_and_point[2] = {'@', '.'};
char allow_visible[32] = {'!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',':',';','<','=','>','?','@','[','\\',']','^','_','`','{','|','}','~'};

int main() {
  int n;
  std::cin >> n;
  std::string str = "An apple";

  for (int i = 0; i < n; ++i) {
    fixed_str<3, 10, allow_underline, 1> underline(str);
    fixed_str<3, 10, allow_at_and_point, 2> at_and_point(str);
    fixed_str<3, 10, allow_visible, 32> visible(str);

    if (underline.is_valid()) {
      std::cout << "Underline valid.\n";
    } else {
      std::cout << "Underline invalid.\n";
    }
    if (at_and_point.is_valid()) {
      std::cout << "At and point valid.\n";
    } else {
      std::cout << "At and point invalid.\n";
    }
    if (visible.is_valid()) {
      std::cout << "Visible valid.\n";
    } else {
      std::cout << "Visible invalid.\n";
    }

    std::cout << "Length: " << underline.length() << " " << at_and_point.length() << " " << visible.length() << '\n';

    std::cout << "Print test ...\n";
    underline.print();
    std::cout << '\n';
    at_and_point.print();
    std::cout << '\n';
    visible.print();
    std::cout << '\n';

    std::cout << underline[0];
    std::cout << at_and_point[1];
    std::cout << visible[visible.length() - 1] << '\n';

    std::cin >> str;
  }
  return 0;
}