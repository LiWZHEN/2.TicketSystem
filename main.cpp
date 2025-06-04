#include <iostream>
#include "length_fixed_string/fixed_str.h"
#include "length_fixed_string/fixed_Chinese.h"
#include "bpt/b_plus_tree.h"
#include "stl/map.hpp"
#include "user.h"
#include "train.h"
#include "bpt/file_processor.h"
#include "stl/vector.hpp"
#include "stl/priority_queue.hpp"
#include "ticket.h"

int main() {
  std::string command;
  sjtu::map<std::string, user::user_info> logged;
  sjtu::bpt<user::user_info> users("users_map_info.txt", "users_data.txt");
  file_processor<train::train_info> train_information("train_info.txt");
  sjtu::bpt<int> train_info_pos("train_id_map_info.txt", "train_id.txt");
  sjtu::bpt<train::train_station_ind> station("station_map_info.txt", "station.txt");
  sjtu::bpt<ticket::record> recording("recording_map_info.txt", "recording.txt");
  sjtu::bpt<ticket::waiting> waiting("waiting_map_info.txt", "waiting.txt");
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
        std::cout << '[' << time_stamp << "] 0\n";
      } else if (!users.Find(username).empty()) {
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        auto iter = logged.find(cur_username);
        if (iter == logged.end() || iter->second.privilege <= privilege) {
          std::cout << '[' << time_stamp << "] -1\n";
        } else {
          users.Insert(username, new_user);
          std::cout << '[' << time_stamp << "] 0\n";
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
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        auto iter = logged.find(username);
        if (iter != logged.end()) { // already logged in
          std::cout << '[' << time_stamp << "] -1\n";
        } else {
          logged.insert({username, find_info[0]});
          std::cout << '[' << time_stamp << "] 0\n";
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
      auto find_info = logged.find(username);
      if (find_info == logged.end()) {
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        logged.erase(find_info);
        std::cout << '[' << time_stamp << "] 0\n";
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
      auto find_info = logged.find(cur_username);
      auto find_target = users.Find(username);
      if (find_info == logged.end() || (find_info->second.privilege <= find_target[0].privilege && cur_username != username)) {
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        std::cout << '[' << time_stamp << "] " << username << ' ' << find_target[0].name << ' ' << find_target[0].mailAddr << ' ' << find_target[0].privilege << '\n';
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
      auto cur_user = logged.find(cur_username);
      auto user_info = users.Find(username);
      if (cur_user == logged.end() || (cur_user->second.privilege <= user_info[0].privilege && cur_username != username) || cur_user->second.privilege <= privilege) {
        std::cout << '[' << time_stamp << "] -1\n";
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
        std::cout << '[' << time_stamp << "] " << username << ' ' << target.name << ' ' << target.mailAddr << ' ' << target.privilege << '\n';
      }
    } else if (cmd == "add_train") {
      std::string trainID;
      int stationNum = 0, seatNum = 0;
      train::train_info new_train;
      new_train.released = false;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'i') {
          ++it;
          ++it;
          while (it < l) {
            trainID += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
          new_train.trainID = trainID;
        } else if (command[it] == 'n') {
          ++it;
          ++it;
          while (it < l && command[it] > ' ') {
            stationNum = stationNum * 10 + (command[it++] - '0');
          }
          new_train.stationNum = stationNum;
        } else if (command[it] == 'm') {
          ++it;
          ++it;
          while (it < l && command[it] > ' ') {
            seatNum = seatNum * 10 + (command[it++] - '0');
          }
          new_train.seatNum = seatNum;
        } else if (command[it] == 's') {
          ++it;
          ++it;
          int station_ind = 0;
          while (true) {
            std::string single_station;
            while (it < l && command[it] != '|' && command[it] != ' ') {
              single_station += command[it++];
            }
            new_train.stations[station_ind++] = single_station;
            if (it >= l || command[it] == ' ') {
              break;
            }
            ++it;
          }
        } else if (command[it] == 'p') {
          ++it;
          ++it;
          int price_ind = 0;
          while (true) {
            int single_price = 0;
            while (it < l && command[it] != '|' && command[it] != ' ') {
              single_price = single_price * 10 + (command[it++] - '0');
            }
            if (price_ind == 0) {
              new_train.cumulative_prices[0] = single_price;
            } else {
              new_train.cumulative_prices[price_ind] = new_train.cumulative_prices[price_ind - 1] + single_price;
            }
            if (it >= l || command[it] == ' ') {
              break;
            }
            ++price_ind;
            ++it;
          }
        } else if (command[it] == 'x') {
          ++it;
          ++it;
          new_train.start_time.hour = (command[it] - '0') * 10 + (command[it + 1] - '0');
          new_train.start_time.minute = (command[it + 3] - '0') * 10 + (command[it + 4] - '0');
          it += 5;
        } else if (command[it] == 't') {
          ++it;
          ++it;
          int time_ind = 0;
          while (true) {
            int single_gap = 0;
            while (it < l && command[it] != '|' && command[it] != ' ') {
              single_gap = single_gap * 10 + (command[it++] - '0');
            }
            if (time_ind == 0) {
              new_train.travel_times[0] = single_gap;
            } else {
              new_train.travel_times[time_ind] = new_train.travel_times[time_ind - 1] + single_gap;
            }
            if (it >= l || command[it] == ' ') {
              break;
            }
            ++time_ind;
            ++it;
          }
        } else if (command[it] == 'o') {
          ++it;
          ++it;
          if (command[it] == '_') {
            ++it;
            continue;
          }
          int single_stopover_ind = 0;
          while (true) {
            int single_stopover = 0;
            while (it < l && command[it] != '|' && command[it] != ' ') {
              single_stopover = single_stopover * 10 + (command[it++] - '0');
            }
            if (single_stopover_ind == 0) {
              new_train.stopover_times[0] = single_stopover;
            } else {
              new_train.stopover_times[single_stopover_ind] = new_train.stopover_times[single_stopover_ind - 1] + single_stopover;
            }
            if (it >= l || command[it] == ' ') {
              break;
            }
            ++single_stopover_ind;
            ++it;
          }
        } else if (command[it] == 'd') {
          ++it;
          ++it;
          new_train.sale_begin.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          new_train.sale_begin.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          new_train.sale_end.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          new_train.sale_end.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 2;
        } else if (command[it] == 'y') {
          ++it;
          ++it;
          new_train.type = command[it++];
        }
      }
      if (train_info_pos.Find(trainID).empty()) {
        const int sale_begin_ind = new_train.sale_begin - Time::date(6, 1), sale_end_ind = new_train.sale_end - Time::date(6, 1);
        for (int i = sale_begin_ind; i <= sale_end_ind + 3; ++i) {
          for (int j = 0; j < new_train.stationNum - 1; ++j) {
            new_train.seat_remain[i][j] = new_train.seatNum;
          }
        }
        int ind = static_cast<int>(train_information.WriteBlock(new_train));
        train_info_pos.Insert(trainID, ind);
        std::cout << '[' << time_stamp << "] 0\n";
      } else {
        std::cout << '[' << time_stamp << "] -1\n";
      }
    } else if (cmd == "delete_train") {
      it += 4;
      std::string trainID;
      while (it < l) {
        trainID += command[it++];
      }
      auto target = train_info_pos.Find(trainID);
      if (target.empty()) {
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        int ind = target[0];
        auto info = train_information.ReadBlock(ind);
        if (info.released) {
          std::cout << '[' << time_stamp << "] -1\n";
        } else {
          train_info_pos.Delete(trainID, ind);
          std::cout << '[' << time_stamp << "] 0\n";
        }
      }
    } else if (cmd == "release_train") {
      it += 4;
      std::string trainID;
      while (it < l) {
        trainID += command[it++];
      }
      auto target = train_info_pos.Find(trainID);
      if (target.empty()) {
        std::cout << '[' << time_stamp << "] -1\n";
      } else {
        int ind = target[0];
        auto info = train_information.ReadBlock(ind);
        if (info.released) {
          std::cout << '[' << time_stamp << "] -1\n";
        } else {
          info.released = true;
          train_information.WriteBack(info, ind);
          for (int i = 0; i < info.stationNum; ++i) {
            station.Insert(info.stations[i].ToString(), {ind, i});
          }
          std::cout << '[' << time_stamp << "] 0\n";
        }
      }
    } else if (cmd == "query_train") {
      std::string trainID;
      Time::date day;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'i') {
          it += 2;
          while (it < l) {
            trainID += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 'd') {
          it += 2;
          day.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          day.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 2;
        }
      }
      auto target = train_info_pos.Find(trainID);
      if (target.empty()) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      int ind = target[0];
      auto info = train_information.ReadBlock(ind);
      if (info.sale_begin - day > 0 || info.sale_end - day < 0) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      std::cout << '[' << time_stamp << "] " << trainID << ' ' << info.type << '\n';
      Time::time t(day, info.start_time);
      std::cout << info.stations[0] << " xx-xx xx:xx -> " << t << " 0 " << info.seat_remain[day - Time::date(6, 1)][0] << '\n';
      if (info.stationNum == 2) {
        auto arrival = t + info.travel_times[0];
        std::cout << info.stations[1] << ' ' << arrival << " -> xx-xx xx:xx " << info.cumulative_prices[0] << " x\n";
      } else {
        auto not_halt = t + info.travel_times[0], halt = not_halt + info.stopover_times[0];
        std::cout << info.stations[1] << ' ' << not_halt << " -> " << halt << ' ' << info.cumulative_prices[0] << ' ' << info.seat_remain[halt.month_day - Time::date(6, 1)][1] << '\n';
        for (int i = 2; i < info.stationNum - 1; ++i) {
          auto all_not_halt = t + info.travel_times[i - 1], arrival = all_not_halt + info.stopover_times[i - 2], departure = all_not_halt + info.stopover_times[i - 1];
          std::cout << info.stations[i] << ' ' << arrival << " -> " << departure << ' ' << info.cumulative_prices[i - 1] << ' ' << info.seat_remain[departure.month_day - Time::date(6, 1)][i] << '\n';
        }
        auto arrival = t + info.travel_times[info.stationNum - 2] + info.stopover_times[info.stationNum - 3];
        std::cout << info.stations[info.stationNum - 1] << ' ' << arrival << " -> xx-xx xx:xx " << info.cumulative_prices[info.stationNum - 2] << " x\n";
      }
    } else if (cmd == "query_ticket") {
      std::string start, destination;
      bool sort_by_time = true;
      Time::date day;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 's') {
          it += 2;
          while (it < l) {
            start += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 't') {
          it += 2;
          while (it < l) {
            destination += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 'd') {
          it += 2;
          day.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          day.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 2;
        } else if (command[it] == 'p') {
          it += 2;
          if (command[it] == 'c') {
            sort_by_time = false;
          }
          it += 4;
        }
      }
      auto pass_s = station.Find(start), pass_t = station.Find(destination);
      int s_it = 0, t_it = 0;
      if (sort_by_time) {
        sjtu::priority_queue<train::train_time_cost, train::Compare_t_id> same;
        while (s_it < pass_s.size() && t_it < pass_t.size()) {
          if (pass_s[s_it].train_ind == pass_t[t_it].train_ind) {
            if (pass_s[s_it].station_ind < pass_t[t_it].station_ind) {
              auto info = train_information.ReadBlock(pass_s[s_it].train_ind);
              int t_gap;
              if (pass_s[s_it].station_ind == 0) {
                t_gap = 0;
              } else {
                t_gap = info.travel_times[pass_s[s_it].station_ind - 1] + info.stopover_times[pass_s[s_it].station_ind - 1];
              }
              Time::time earliest(info.sale_begin, info.start_time), latest(info.sale_end, info.start_time);
              earliest += t_gap, latest += t_gap;
              if (earliest.month_day - day > 0 || latest.month_day - day < 0) {
                ++s_it, ++t_it;
                continue;
              }
              Time::time leave_s(day, info.start_time), arrive_t(day, info.start_time);
              int travel_t;
              if (pass_s[s_it].station_ind != 0) {
                leave_s += info.travel_times[pass_s[s_it].station_ind - 1] + info.stopover_times[pass_s[s_it].station_ind - 1];
                travel_t = info.travel_times[pass_t[t_it].station_ind - 1] + info.stopover_times[pass_t[t_it].station_ind - 2] - info.travel_times[pass_s[s_it].station_ind - 1] - info.stopover_times[pass_s[s_it].station_ind - 1];
              } else if (pass_t[t_it].station_ind == 1) {
                travel_t = info.travel_times[0];
              } else {
                travel_t = info.travel_times[pass_t[t_it].station_ind - 1] + info.stopover_times[pass_t[t_it].station_ind - 2];
              }
              leave_s.month_day = day;
              arrive_t = leave_s + travel_t;
              int cost;
              if (pass_s[s_it].station_ind == 0) {
                cost = info.cumulative_prices[pass_t[t_it].station_ind - 1];
              } else {
                cost = info.cumulative_prices[pass_t[t_it].station_ind - 1] - info.cumulative_prices[pass_s[s_it].station_ind - 1];
              }
              int max_ticket = info.seatNum;
              auto leave_i = leave_s;
              for (int i = pass_s[s_it].station_ind; i < pass_t[t_it].station_ind; ++i) {
                int d = leave_i.month_day - Time::date(6, 1);
                if (info.seat_remain[d][i] < max_ticket) {
                  max_ticket = info.seat_remain[d][i];
                }
                if (i > 0) {
                  leave_i = leave_i + (info.travel_times[i] - info.travel_times[i - 1]) + (info.stopover_times[i] - info.stopover_times[i - 1]);
                } else {
                  leave_i = leave_i + info.travel_times[i] + info.stopover_times[i];
                }
              }
              same.push({info.trainID.ToString(), arrive_t - leave_s, cost, leave_s, arrive_t,
                info.stations[pass_s[s_it].station_ind].ToString(), info.stations[pass_t[t_it].station_ind].ToString(), max_ticket});
            }
            ++s_it, ++t_it;
          } else if (pass_s[s_it].train_ind < pass_t[t_it].train_ind) {
            ++s_it;
          } else {
            ++t_it;
          }
        }
        std::cout << '[' << time_stamp << "] " << same.size() << '\n';
        while (!same.empty()) {
          auto best = same.top();
          same.pop();
          std::cout << best << '\n';
        }
      } else {
        sjtu::priority_queue<train::train_time_cost, train::Compare_c_id> same;
        while (s_it < pass_s.size() && t_it < pass_t.size()) {
          if (pass_s[s_it].train_ind == pass_t[t_it].train_ind) {
            if (pass_s[s_it].station_ind < pass_t[t_it].station_ind) {
              auto info = train_information.ReadBlock(pass_s[s_it].train_ind);
              int t_gap;
              if (pass_s[s_it].station_ind == 0) {
                t_gap = 0;
              } else {
                t_gap = info.travel_times[pass_s[s_it].station_ind - 1] + info.stopover_times[pass_s[s_it].station_ind - 1];
              }
              Time::time earliest(info.sale_begin, info.start_time), latest(info.sale_end, info.start_time);
              earliest += t_gap, latest += t_gap;
              if (earliest.month_day - day > 0 || latest.month_day - day < 0) {
                ++s_it, ++t_it;
                continue;
              }
              Time::time leave_s(day, info.start_time), arrive_t(day, info.start_time);
              int travel_t;
              if (pass_s[s_it].station_ind != 0) {
                leave_s += info.travel_times[pass_s[s_it].station_ind - 1] + info.stopover_times[pass_s[s_it].station_ind - 1];
                travel_t = info.travel_times[pass_t[t_it].station_ind - 1] + info.stopover_times[pass_t[t_it].station_ind - 2] - info.travel_times[pass_s[s_it].station_ind - 1] - info.stopover_times[pass_s[s_it].station_ind - 1];
              } else if (pass_t[t_it].station_ind == 1) {
                travel_t = info.travel_times[0];
              } else {
                travel_t = info.travel_times[pass_t[t_it].station_ind - 1] + info.stopover_times[pass_t[t_it].station_ind - 2];
              }
              leave_s.month_day = day;
              arrive_t = leave_s + travel_t;
              int cost;
              if (pass_s[s_it].station_ind == 0) {
                cost = info.cumulative_prices[pass_t[t_it].station_ind - 1];
              } else {
                cost = info.cumulative_prices[pass_t[t_it].station_ind - 1] - info.cumulative_prices[pass_s[s_it].station_ind - 1];
              }
              int max_ticket = info.seatNum;
              auto leave_i = leave_s;
              for (int i = pass_s[s_it].station_ind; i < pass_t[t_it].station_ind; ++i) {
                int d = leave_i.month_day - Time::date(6, 1);
                if (info.seat_remain[d][i] < max_ticket) {
                  max_ticket = info.seat_remain[d][i];
                }
                if (i > 0) {
                  leave_i = leave_i + (info.travel_times[i] - info.travel_times[i - 1]) + (info.stopover_times[i] - info.stopover_times[i - 1]);
                } else {
                  leave_i = leave_i + info.travel_times[i] + info.stopover_times[i];
                }
              }
              same.push({info.trainID.ToString(), arrive_t - leave_s, cost, leave_s, arrive_t,
                info.stations[pass_s[s_it].station_ind].ToString(), info.stations[pass_t[t_it].station_ind].ToString(), max_ticket});
            }
            ++s_it, ++t_it;
          } else if (pass_s[s_it].train_ind < pass_t[t_it].train_ind) {
            ++s_it;
          } else {
            ++t_it;
          }
        }
        std::cout << '[' << time_stamp << "] " << same.size() << '\n';
        while (!same.empty()) {
          auto best = same.top();
          same.pop();
          std::cout << best << '\n';
        }
      }
    } else if (cmd == "query_transfer") {
      std::string start, destination;
      bool sort_by_time = true;
      Time::date day;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 's') {
          it += 2;
          while (it < l) {
            start += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 't') {
          it += 2;
          while (it < l) {
            destination += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 'd') {
          it += 2;
          day.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          day.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 2;
        } else if (command[it] == 'p') {
          it += 2;
          if (command[it] == 'c') {
            sort_by_time = false;
          }
          it += 4;
        }
      }
      auto pass_s = station.Find(start), pass_t = station.Find(destination);
      sjtu::map<std::string, train::train_time_cost> same;
      for (auto train_station : pass_s) {
        // the start station is the train_station.station_ind-th station of the train_station.train_ind-th of train
        auto info = train_information.ReadBlock(train_station.train_ind); // the train information
        start = info.stations[train_station.station_ind].ToString();
        for (int i = train_station.station_ind + 1; i < info.stationNum; ++i) {
          // we hope to get the needed time to go from start to info.stations[i]
          destination = info.stations[i].ToString();
          Time::time leave_s(day, info.start_time), arrive_t(day, info.start_time);
          int travel_t;
          if (train_station.station_ind != 0) {
            leave_s += info.travel_times[train_station.station_ind - 1] + info.stopover_times[train_station.station_ind - 1];
            travel_t = info.travel_times[i - 1] + info.stopover_times[i - 2] - info.travel_times[train_station.station_ind - 1] - info.stopover_times[train_station.station_ind - 1];
          } else if (i == 1) {
            travel_t = info.travel_times[0];
          } else {
            travel_t = info.travel_times[i - 1] + info.stopover_times[i - 2];
          }
          leave_s.month_day = day;
          arrive_t = leave_s + travel_t;
          int cost;
          if (train_station.station_ind == 0) {
            cost = info.cumulative_prices[i - 1];
          } else {
            cost = info.cumulative_prices[i - 1] - info.cumulative_prices[train_station.station_ind - 1];
          }
          int max_ticket = info.seatNum;
          auto leave_i = leave_s;
          for (int ind = train_station.station_ind; ind < i; ++ind) {
            int d = leave_i.month_day - Time::date(6, 1);
            if (info.seat_remain[d][ind] < max_ticket) {
              max_ticket = info.seat_remain[d][ind];
            }
            if (ind > 0) {
              leave_i = leave_i + (info.travel_times[ind] - info.travel_times[ind - 1]) + (info.stopover_times[ind] - info.stopover_times[ind - 1]);
            } else {
              leave_i = leave_i + info.travel_times[ind] + info.stopover_times[ind];
            }
          }
          same.insert({destination, {info.trainID.ToString(), arrive_t - leave_s, cost, leave_s, arrive_t,
                info.stations[train_station.station_ind].ToString(), info.stations[i].ToString(), max_ticket}});
        }
      }
      if (sort_by_time) { // total_time, total_cost, trainID1, trainID2;
        int min_time = 2147483647, min_cost = 2147483647;
        train::train_time_cost train1, train2;
        for (auto train_station : pass_t) {
          auto info = train_information.ReadBlock(train_station.train_ind);
          destination = info.stations[train_station.station_ind].ToString();
          for (int i = 0; i < train_station.station_ind; ++i) {
            // we hope to get the needed time to go from info.stations[i] to destination
            start = info.stations[i].ToString();
            auto find_transfer_station = same.find(start);
            if (find_transfer_station == same.end()) {
              continue;
            }
            if (find_transfer_station->second.trainID == info.trainID.ToString()) {
              continue;
            }
            Time::time leave_s(find_transfer_station->second.arrive.month_day, info.start_time), arrive_t = leave_s;
            int start_to_s = 0;
            int travel_t;
            if (i != 0) {
              start_to_s = info.travel_times[i - 1] + info.stopover_times[i - 1];
              leave_s += start_to_s;
              travel_t = info.travel_times[train_station.station_ind - 1] + info.stopover_times[train_station.station_ind - 2] - info.travel_times[i - 1] - info.stopover_times[i - 1];
            } else if (train_station.station_ind == 1) {
              travel_t = info.travel_times[0];
            } else {
              travel_t = info.travel_times[train_station.station_ind - 1] + info.stopover_times[train_station.station_ind - 2];
            }
            leave_s.month_day = find_transfer_station->second.arrive.month_day;
            if (leave_s - find_transfer_station->second.arrive < 0) {
              leave_s.month_day += 1;
            }
            Time::time earliest2(info.sale_begin, info.start_time), latest2(info.sale_end, info.start_time);
            earliest2 += start_to_s, latest2 += start_to_s;
            if (earliest2 - leave_s > 0 || latest2 - leave_s < 0) {
              continue;
            }
            arrive_t = leave_s + travel_t;
            int cost2;
            if (i == 0) {
              cost2 = info.cumulative_prices[train_station.station_ind - 1];
            } else {
              cost2 = info.cumulative_prices[train_station.station_ind - 1] - info.cumulative_prices[i - 1];
            }
            const int total_cost = cost2 + find_transfer_station->second.cost;
            int max_ticket = info.seatNum;
            auto leave_i = leave_s;
            for (int ind = i; ind < train_station.station_ind; ++ind) {
              int d = leave_i.month_day - Time::date(6, 1);
              if (info.seat_remain[d][ind] < max_ticket) {
                max_ticket = info.seat_remain[d][ind];
              }
              if (ind > 0) {
                leave_i = leave_i + (info.travel_times[ind] - info.travel_times[ind - 1]) + (info.stopover_times[ind] - info.stopover_times[ind - 1]);
              } else {
                leave_i = leave_i + info.travel_times[ind] + info.stopover_times[ind];
              }
            }
            int time2 = arrive_t - leave_s;
            const int total_time = arrive_t - find_transfer_station->second.leave;
            if (total_time > min_time) {
              continue;
            }
            if (total_time < min_time) {
              min_time = total_time;
              min_cost = total_cost;
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            if (total_cost > min_cost) {
              continue;
            }
            if (total_cost < min_cost) {
              min_cost = total_cost;
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            if (find_transfer_station->second.trainID > train1.trainID) {
              continue;
            }
            if (find_transfer_station->second.trainID < train1.trainID) {
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            std::string cur_train2ID = info.trainID.ToString();
            if (cur_train2ID > train2.trainID) {
              continue;
            }
            if (cur_train2ID < train2.trainID) {
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
            }
          }
        }
        if (min_time == 2147483647) {
          std::cout << '[' << time_stamp << "] 0\n";
        } else {
          std::cout << '[' << time_stamp << "] " << train1 << '\n';
          std::cout << train2 << '\n';
        }
      } else { // total_cost, total_time, trainID1, trainID2;
        int min_time = 2147483647, min_cost = 2147483647;
        train::train_time_cost train1, train2;
        for (auto train_station : pass_t) {
          auto info = train_information.ReadBlock(train_station.train_ind);
          destination = info.stations[train_station.station_ind].ToString();
          for (int i = 0; i < train_station.station_ind; ++i) {
            // we hope to get the needed time to go from info.stations[i] to destination
            start = info.stations[i].ToString();
            auto find_transfer_station = same.find(start);
            if (find_transfer_station == same.end()) {
              continue;
            }
            if (find_transfer_station->second.trainID == info.trainID.ToString()) {
              continue;
            }
            Time::time leave_s(find_transfer_station->second.arrive.month_day, info.start_time), arrive_t = leave_s;
            int start_to_s = 0;
            int travel_t;
            if (i != 0) {
              start_to_s = info.travel_times[i - 1] + info.stopover_times[i - 1];
              leave_s += start_to_s;
              travel_t = info.travel_times[train_station.station_ind - 1] + info.stopover_times[train_station.station_ind - 2] - info.travel_times[i - 1] - info.stopover_times[i - 1];
            } else if (train_station.station_ind == 1) {
              travel_t = info.travel_times[0];
            } else {
              travel_t = info.travel_times[train_station.station_ind - 1] + info.stopover_times[train_station.station_ind - 2];
            }
            leave_s.month_day = find_transfer_station->second.arrive.month_day;
            if (leave_s - find_transfer_station->second.arrive < 0) {
              leave_s.month_day += 1;
            }
            Time::time earliest2(info.sale_begin, info.start_time), latest2(info.sale_end, info.start_time);
            earliest2 += start_to_s, latest2 += start_to_s;
            if (earliest2 - leave_s > 0 || latest2 - leave_s < 0) {
              continue;
            }
            arrive_t = leave_s + travel_t;
            int cost2;
            if (i == 0) {
              cost2 = info.cumulative_prices[train_station.station_ind - 1];
            } else {
              cost2 = info.cumulative_prices[train_station.station_ind - 1] - info.cumulative_prices[i - 1];
            }
            const int total_cost = cost2 + find_transfer_station->second.cost;
            int max_ticket = info.seatNum;
            auto leave_i = leave_s;
            for (int ind = i; ind < train_station.station_ind; ++ind) {
              int d = leave_i.month_day - Time::date(6, 1);
              if (info.seat_remain[d][ind] < max_ticket) {
                max_ticket = info.seat_remain[d][ind];
              }
              if (ind > 0) {
                leave_i = leave_i + (info.travel_times[ind] - info.travel_times[ind - 1]) + (info.stopover_times[ind] - info.stopover_times[ind - 1]);
              } else {
                leave_i = leave_i + info.travel_times[ind] + info.stopover_times[ind];
              }
            }
            int time2 = arrive_t - leave_s;
            const int total_time = arrive_t - find_transfer_station->second.leave;
            if (total_cost > min_cost) {
              continue;
            }
            if (total_cost < min_cost) {
              min_time = total_time;
              min_cost = total_cost;
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            if (total_time > min_time) {
              continue;
            }
            if (total_time < min_time) {
              min_time = total_time;
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            if (find_transfer_station->second.trainID > train1.trainID) {
              continue;
            }
            if (find_transfer_station->second.trainID < train1.trainID) {
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
              continue;
            }
            std::string cur_train2ID = info.trainID.ToString();
            if (cur_train2ID > train2.trainID) {
              continue;
            }
            if (cur_train2ID < train2.trainID) {
              train1 = find_transfer_station->second;
              train2 = {info.trainID.ToString(), time2, cost2, leave_s, arrive_t, start, destination, max_ticket};
            }
          }
        }
        if (min_cost == 2147483647) {
          std::cout << '[' << time_stamp << "] 0\n";
        } else {
          std::cout << '[' << time_stamp << "] " << train1 << '\n';
          std::cout << train2 << '\n';
        }
      }
    } else if (cmd == "buy_ticket") {
      std::string username, trainID, from, to;
      Time::date day;
      bool queue = false;
      int num = 0;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'u') {
          it += 2;
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
        } else if (command[it] == 'i') {
          it += 2;
          while (it < l && command[it] > ' ') {
            trainID += command[it++];
          }
        } else if (command[it] == 'd') {
          it += 2;
          day.month = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 3;
          day.day = (command[it] - '0') * 10 + (command[it + 1] - '0');
          it += 2;
        } else if (command[it] == 'n') {
          it += 2;
          while (it < l && command[it] > ' ') {
            num = num * 10 + (command[it++] - '0');
          }
        } else if (command[it] == 'f') {
          it += 2;
          while (it < l) {
            from += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 't') {
          it += 2;
          while (it < l) {
            to += command[it++];
            if (it + 1 < l && command[it + 1] == '-') {
              break;
            }
          }
        } else if (command[it] == 'q') {
          it += 2;
          if (command[it] == 't') {
            queue = true;
            it += 4;
          } else {
            it += 5;
          }
        }
      }
      auto target_pos = train_info_pos.Find(trainID);
      if (target_pos.empty()) {
        std::cout << '[' << time_stamp << "] -1\n"; // no such train
        continue;
      }
      auto info = train_information.ReadBlock(target_pos[0]);
      if (!info.released) {
        std::cout << '[' << time_stamp << "] -1\n"; // train hasn't been released
        continue;
      }
      int ind_s, ind_t;
      bool find_s = false, find_t = false;
      for (int i = 0; i < info.stationNum; ++i) {
        if (info.stations[i].ToString() == from) {
          ind_s = i;
          find_s = true;
          continue;
        }
        if (info.stations[i].ToString() == to) {
          ind_t = i;
          find_t = true;
          break;
        }
      }
      if (!find_s || !find_t) {
        std::cout << '[' << time_stamp << "] -1\n"; // stations are not valid
        continue;
      }
      int t_gap;
      if (ind_s == 0) {
        t_gap = 0;
      } else {
        t_gap = info.travel_times[ind_s - 1] + info.stopover_times[ind_s - 1];
      }
      Time::time earliest(info.sale_begin, info.start_time), latest(info.sale_end, info.start_time);
      earliest += t_gap, latest += t_gap;
      if (earliest.month_day - day > 0 || latest.month_day - day < 0) {
        std::cout << '[' << time_stamp << "] -1\n"; // out of the range of sale
        continue;
      }
      Time::time leave_i(day, info.start_time);
      if (ind_s != 0) {
        leave_i += info.travel_times[ind_s - 1] + info.stopover_times[ind_s - 1];
      }
      leave_i.month_day = day;
      auto leave_ii = leave_i;
      Time::date normal(6, 1);
      int max_ticket = 2147483647;
      for (int i = ind_s; i < ind_t; ++i) {
        if (info.seat_remain[leave_i.month_day - normal][i] < max_ticket) {
          max_ticket = info.seat_remain[leave_i.month_day - normal][i];
        }
        if (i > 0) {
          leave_i = leave_i + (info.travel_times[i] - info.travel_times[i - 1]) + (info.stopover_times[i] - info.stopover_times[i - 1]);
        } else {
          leave_i = leave_i + info.travel_times[i] + info.stopover_times[i];
        }
      }
      if (max_ticket >= num) {
        for (int i = ind_s; i < ind_t; ++i) {
          info.seat_remain[leave_ii.month_day - normal][i] -= num;
          if (i > 0) {
            leave_ii = leave_ii + (info.travel_times[i] - info.travel_times[i - 1]) + (info.stopover_times[i] - info.stopover_times[i - 1]);
          } else {
            leave_ii = leave_ii + info.travel_times[i] + info.stopover_times[i];
          }
        }
        train_information.WriteBack(info, target_pos[0]);
        int t_travel;
        if (ind_s == 0) {
          if (ind_t == 1) {
            t_travel = info.travel_times[0];
          } else {
            t_travel = info.travel_times[ind_t - 1] + info.stopover_times[ind_t - 2];
          }
        } else {
          if (ind_t == ind_s + 1) {
            t_travel = info.travel_times[ind_s] - info.travel_times[ind_s - 1];
          } else {
            t_travel = info.travel_times[ind_t - 1] - info.travel_times[ind_s - 1] + info.stopover_times[ind_t - 2] - info.stopover_times[ind_s - 1];
          }
        }
        int price;
        if (ind_s == 0) {
          price = info.cumulative_prices[ind_t - 1];
        } else {
          price = info.cumulative_prices[ind_t - 1] - info.cumulative_prices[ind_s - 1];
        }
        Time::time leaving_time(day, earliest.hour_min);
        recording.Insert(username, {fixed_str<20>(username), time_stamp, info.trainID,
          fixed_Chinese<10>(from), fixed_Chinese<10>(to), leaving_time,
          leaving_time + t_travel, price, num, 0, ind_s, ind_t});
        std::cout << '[' << time_stamp << "] " << price * num << '\n';
      } else {
        if (!queue) {
          std::cout << '[' << time_stamp << "] -1\n"; // do not have sufficient tickets
          continue;
        }
        int t_travel;
        if (ind_s == 0) {
          if (ind_t == 1) {
            t_travel = info.travel_times[0];
          } else {
            t_travel = info.travel_times[ind_t - 1] + info.stopover_times[ind_t - 2];
          }
        } else {
          if (ind_t == ind_s + 1) {
            t_travel = info.travel_times[ind_s] - info.travel_times[ind_s - 1];
          } else {
            t_travel = info.travel_times[ind_t - 1] - info.travel_times[ind_s - 1] + info.stopover_times[ind_t - 2] - info.stopover_times[ind_s - 1];
          }
        }
        int price;
        if (ind_s == 0) {
          price = info.cumulative_prices[ind_t - 1];
        } else {
          price = info.cumulative_prices[ind_t - 1] - info.cumulative_prices[ind_s - 1];
        }
        Time::time leaving_time(day, earliest.hour_min);
        recording.Insert(username, {fixed_str<20>(username), time_stamp, info.trainID,
          fixed_Chinese<10>(from), fixed_Chinese<10>(to), leaving_time,
          leaving_time + t_travel, price, num, 1, ind_s, ind_t});
        waiting.Insert(trainID, {time_stamp, fixed_str<20>(username), info.trainID,
          ind_s, ind_t, day, num});
        std::cout << '[' << time_stamp << "] queue\n";
      }
    } else if (cmd == "query_order") {
      it += 4;
      std::string username;
      while (it < l) {
        username += command[it++];
      }
      auto check = logged.find(username);
      if (check == logged.end()) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      auto target = recording.Find(username);
      std::cout << '[' << time_stamp << "] " << target.size() << '\n';
      for (int i = static_cast<int>(target.size()) - 1; i >= 0; --i) {
        std::cout << target[i] << '\n';
      }
    } else if (cmd == "refund_ticket") {
      std::string username;
      int n = 1;
      while (true) {
        while (it < l && command[it] <= ' ') {
          ++it;
        }
        if (it == l) {
          break;
        }
        ++it;
        if (command[it] == 'u') {
          it += 2;
          while (it < l && command[it] > ' ') {
            username += command[it++];
          }
        } else if (command[it] == 'n') {
          n = 0;
          it += 2;
          while (it < l && command[it] > ' ') {
            n = n * 10 + (command[it++] - '0');
          }
        }
      }
      auto check = logged.find(username);
      if (check == logged.end()) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      auto target = recording.Find(username);
      if (target.size() < n) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      auto to_be_refunded = target[target.size() - n];
      if (to_be_refunded.status != 0) {
        std::cout << '[' << time_stamp << "] -1\n";
        continue;
      }
      std::string trainID = to_be_refunded.trainID.ToString();
      auto train_info_ind = train_info_pos.Find(trainID);
      auto info = train_information.ReadBlock(train_info_ind[0]);
      Time::time ti = to_be_refunded.leaving;
      for (int i = to_be_refunded.ind_s; i < to_be_refunded.ind_t; ++i) {
        info.seat_remain[ti.month_day - Time::date(6, 1)][i] += to_be_refunded.num;
        if (i == 0) {
          ti += info.travel_times[0] + info.stopover_times[0];
        } else {
          ti += info.travel_times[i] + info.stopover_times[i] - info.travel_times[i - 1] - info.stopover_times[i - 1];
        }
      }
      recording.Delete(username, to_be_refunded);
      to_be_refunded.status = 2;
      recording.Insert(username, to_be_refunded);
      auto waiting_list = waiting.Find(trainID);
      for (const auto &request : waiting_list) {
        int travel_t;
        if (request.ind_start == 0) {
          travel_t = info.travel_times[request.ind_destination - 1] + info.stopover_times[request.ind_destination - 1];
        } else {
          travel_t = info.travel_times[request.ind_destination - 1] + info.stopover_times[request.ind_destination - 1]
              - info.travel_times[request.ind_start - 1] - info.stopover_times[request.ind_start - 1];
        }
        Time::time depart(info.sale_begin, info.start_time);
        Time::time depart_needed = depart + travel_t;
        const int delta_date = request.date - depart_needed.month_day;
        depart.month_day += delta_date;
        if (request.ind_start > 0) {
          depart += info.travel_times[request.ind_start - 1] + info.stopover_times[request.ind_start - 1];
        }
        ti = depart; // the departure time at ind_start
        bool satisfiable = true;
        for (int i = request.ind_start; i < request.ind_destination; ++i) {
          if (info.seat_remain[depart.month_day - Time::date(6, 1)][i] < request.require_seats) {
            satisfiable = false;
            break;
          }
          if (i == 0) {
            depart += info.travel_times[0] + info.stopover_times[0];
          } else {
            depart += info.travel_times[i] + info.stopover_times[i] - info.travel_times[i - 1] - info.stopover_times[i - 1];
          }
        }
        if (!satisfiable) {
          continue;
        }
        for (int i = request.ind_start; i < request.ind_destination; ++i) {
          info.seat_remain[ti.month_day - Time::date(6, 1)][i] -= request.require_seats;
          if (i == 0) {
            ti += info.travel_times[0] + info.stopover_times[0];
          } else {
            ti += info.travel_times[i] + info.stopover_times[i] - info.travel_times[i - 1] - info.stopover_times[i - 1];
          }
        }
        waiting.Delete(trainID, request);
        std::string user_name = request.username.ToString();
        auto find_record = recording.Find(user_name);
        ticket::record target_record;
        for (const auto &iterator : find_record) {
          if (iterator.query_time_stamp == request.query_time_stamp) {
            target_record = iterator;
            break;
          }
        }
        recording.Delete(user_name, target_record);
        target_record.status = 0;
        recording.Insert(user_name, target_record);
      }
      train_information.WriteBack(info, train_info_ind[0]);
      std::cout << '[' << time_stamp << "] 0\n";
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