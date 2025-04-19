#include <fstream>

int main() {
  bool exist = false;
  std::fstream file("data.txt");
  if (file.is_open()) {
    exist = true;
  }
  file.close();
  if (!exist) {
    std::ofstream new_file("data.txt");
    new_file.close();
  }
  file.open("data.txt");

  file.close();
  return 0;
}