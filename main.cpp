#include <iostream>
#include "length_fixed_string/fixed_str.h"
#include "length_fixed_string/fixed_Chinese.h"
#include "bpt/b_plus_tree.h"
#include "stl/map.hpp"
#include "user.h"

int main() {
  std::string command;
  sjtu::map<fixed_str<20>, user::user_info> logged;
  sjtu::bpt<user::user_info> users("users_map_info.txt", "users_data.txt");
  while (true) {
    if (!std::getline(std::cin, command)) {
      logged.clear();
      break;
    }
    const int l = static_cast<int>(command.length());
    int it = 0;
    int time_stamp = 0;
    while (command[it] <= ' ') {
      ++it;
    }
    ++it;
    while (command[it] != ']') {
      time_stamp = time_stamp * 10 + (command[it++] - '0');
    }
    ++it;
    while (command[it] <= ' ') {
      ++it;
    }

    std::string cmd;
    while (command[it] > ' ') {
      cmd += command[it++];
    }

    if (cmd == "add_user") {
      std::string cur_username, username, password, name, mailAddr;
      int privilege = 0;
      user::user_info new_user;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'c') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            cur_username += command[it++];
          }
        } else if (command[it] == 'u') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
          new_user.username = username;
        } else if (command[it] == 'p') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            password += command[it++];
          }
          new_user.password = password;
        } else if (command[it] == 'n') {
          ++it;
          ++it;
          while (it < l && command[it] != ' ') {
            name += command[it++];
          }
          new_user.name = name;
        } else if (command[it] == 'm') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            mailAddr += command[it++];
          }
          new_user.mailAddr = mailAddr;
        } else if (command[it] == 'g') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            privilege = privilege * 10 + (command[it++] - '0');
          }
          new_user.privilege = privilege;
        }
      }
      if (users.Size() == 0) {
        new_user.privilege = 10;
        users.Insert(username, new_user);
        std::cout << "[" << time_stamp << "] 0\n";
      } else if (!users.Find(username).empty()) {
        std::cout << "[" << time_stamp << "] -1\n";
      } else {
        auto iter = logged.find(fixed_str<20>(cur_username));
        if (iter == logged.end() || iter->second.privilege <= privilege) {
          std::cout << "[" << time_stamp << "] -1\n";
        } else {
          users.Insert(username, new_user);
          std::cout << "[" << time_stamp << "] 0\n";
        }
      }
    } else if (cmd == "login") {
      std::string username, password;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'u') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
        } else if (command[it] == 'p') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            password += command[it++];
          }
        }
      }
      auto find_info = users.Find(username);
      if (find_info.empty() || find_info[0].password.ToString() != password) {
        std::cout << "[" << time_stamp << "] -1\n";
      } else {
        auto iter = logged.find(fixed_str<20>(username));
        if (iter != logged.end()) { // already logged in
          std::cout << "[" << time_stamp << "] -1\n";
        } else {
          sjtu::pair<const fixed_str<20>, user::user_info> pr(username, find_info[0]);
          logged.insert(pr);
          std::cout << "[" << time_stamp << "] 0\n";
        }
      }
    } else if (cmd == "logout") {
      std::string username;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        ++it;
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        while (it < l && command[it] > ' ') {
          username += command[it++];
        }
      }
      auto find_info = logged.find(fixed_str<20>(username));
      if (find_info == logged.end()) {
        std::cout << "[" << time_stamp << "] -1\n";
      } else {
        logged.erase(find_info);
        std::cout << "[" << time_stamp << "] 0\n";
      }
    } else if (cmd == "query_profile") {
      std::string cur_username, username;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'u') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
        } else if (command[it] == 'c') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            cur_username += command[it++];
          }
        }
      }
      auto find_info = logged.find(fixed_str<20>(cur_username));
      auto find_target = users.Find(username);
      if (find_info == logged.end() || (find_info->second.privilege <= find_target[0].privilege && cur_username != username)) {
        std::cout << "[" << time_stamp << "] -1\n";
      } else {
        std::cout << "[" << time_stamp << "] " << username << " " << find_target[0].name << " " << find_target[0].mailAddr << " " << find_target[0].privilege << "\n";
      }
    } else if (cmd == "modify_profile") {
      bool p = false, n = false, m = false, g = false;
      std::string cur_username, username, password, name, mailAddr;
      int privilege = 0;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'c') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            cur_username += command[it++];
          }
        } else if (command[it] == 'u') {
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
        } else if (command[it] == 'p') {
          p = true;
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            password += command[it++];
          }
        } else if (command[it] == 'n') {
          n = true;
          ++it;
          ++it;
          while (it < l && command[it] != ' ') {
            name += command[it++];
          }
        } else if (command[it] == 'm') {
          m = true;
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            mailAddr += command[it++];
          }
        } else if (command[it] == 'g') {
          g = true;
          ++it;
          while (it < l && command[it] <= ' ') {
            ++it;
          }
          while (it < l && command[it] > ' ') {
            privilege = privilege * 10 + (command[it++] - '0');
          }
        }
      }
      auto cur_user = logged.find(fixed_str<20>(cur_username));
      auto user_info = users.Find(username);
      if (cur_user == logged.end() || (cur_user->second.privilege <= user_info[0].privilege && cur_username != username) || cur_user->second.privilege <= privilege) {
        std::cout << "[" << time_stamp << "] -1\n";
      } else {
        auto target = user_info[0];
        users.Delete(username, target);
        if (p) {
          target.password = password;
        }
        if (n) {
          target.name = name;
        }
        if (m) {
          target.mailAddr = mailAddr;
        }
        if (g) {
          target.privilege = privilege;
        }
        users.Insert(username, target);
        std::cout << "[" << time_stamp << "] " << username << " " << target.name << " " << target.mailAddr << " " << target.privilege << "\n";
      }
    } else if (cmd == "add_train") {

    } else if (cmd == "delete_train") {

    } else if (cmd == "release_train") {

    } else if (cmd == "query_train") {

    } else if (cmd == "query_ticket") {

    } else if (cmd == "query_transfer") {

    } else if (cmd == "buy_ticket") {

    } else if (cmd == "query_order") {

    } else if (cmd == "refund_ticket") {

    } else if (cmd == "clean") {
      logged.clear();
      users.Clean();
      std::cout << "[" << time_stamp << "] 0\n";
    } else if (cmd == "exit") {
      logged.clear();
      std::cout << "[" << time_stamp << "] bye\n";
      break;
    }
  }
  return 0;
}