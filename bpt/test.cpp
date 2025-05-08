#include <iostream>
#include <fstream>
#include "b_plus_tree.h"

constexpr long SIZE = 16384;

void Print(const long pos) {
  if (pos == -1) {
    std::cout << "\n--------------------------------------------------------------\n";
    return;
  }
  std::ifstream read_file("data_file.txt");
  read_file.seekg(pos * SIZE);
  sjtu::bpt<int>::block target;
  read_file.read(reinterpret_cast<char *>(&target), sizeof(target));
  if (target.son_pos[0] == -1) {
    std::cout << "The leaf block.    Position: " << pos << '\n';
    std::cout << "Size: " << target.block_size << '\n';
    std::cout << "Next: " << target.next_block << '\n';
    std::cout << "Data: " << '\n';
    for (int i = 0; i < target.block_size; ++i) {
      std::cout << "\033[32m" << '(' << target.r_min[i].index.hash1 << ", " << target.r_min[i].index.hash2 << ") : " << "\033[33m" << target.r_min[i].value << ' ';
    }
    std::cout << "\033[0m" << "\n--------------------------------------------------------------\n";
  } else {
    std::cout << "The index block.   Position: " << pos << '\n';
    std::cout << "Size: " << target.block_size << '\n';
    std::cout << "Data: " << '\n';
    for (int i = 0; i < target.block_size; ++i) {
      std::cout << "\033[31m" << target.son_pos[i] << ' ' << "\033[32m" << '(' << target.r_min[i].index.hash1 << ", " << target.r_min[i].index.hash2 << ") : " << "\033[33m" << target.r_min[i].value << ' ';
    }
    std::cout << "\033[31m" << target.son_pos[target.block_size] << "\033[0m" << "\n--------------------------------------------------------------\n";
    for (int i = 0; i <= target.block_size; ++i) {
      Print(target.son_pos[i]);
    }
  }
  read_file.close();
}

int main() {
  sjtu::bpt<int> bpt("map_file.txt", "data_file.txt");
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "insert") {
      std::string index;
      int value;
      std::cin >> index >> value;
      bpt.Insert(index, value);
      Print(bpt.map_information.root);
    } else if (command == "delete") {
      std::string index;
      int value;
      std::cin >> index >> value;
      bpt.Delete(index, value);
      Print(bpt.map_information.root);
    } else if (command == "find") {
      std::string index;
      std::cin >> index;
      const auto ans = bpt.Find(index);
      for (const auto it : ans) {
        std::cout << it << ' ';
      }
      if (ans.empty()) {
        std::cout << "null";
      }
      std::cout << '\n';
    }
  }
  return 0;
}