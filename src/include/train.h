#ifndef TRAIN_H
#define TRAIN_H

#include "length_fixed_string/fixed_str.h"
#include "length_fixed_string/fixed_Chinese.h"
#include "time_format.h"

namespace train {

  struct train_info {
    fixed_str<20> trainID;
    int stationNum;
    fixed_Chinese<10> stations[24];
    int seatNum;
    int cumulative_prices[23];
    int travel_times[23];
    int stopover_times[23];
    int seat_remain[95][23];
    bool released;
    Time::date sale_begin, sale_end;
    Time::clock start_time;
    char type;
  };

  struct train_station_ind {
    int train_ind;
    int station_ind;

    bool operator==(const train_station_ind &other) const {
      return train_ind == other.train_ind && station_ind == other.station_ind;
    }
    bool operator>(const train_station_ind &other) const {
      if (train_ind > other.train_ind) {
        return true;
      }
      if (train_ind < other.train_ind) {
        return false;
      }
      return station_ind > other.station_ind;
    }
    bool operator<(const train_station_ind &other) const {
      if (train_ind < other.train_ind) {
        return true;
      }
      if (train_ind > other.train_ind) {
        return false;
      }
      return station_ind < other.station_ind;
    }
    bool operator!=(const train_station_ind &other) const {
      return !(*this == other);
    }
    bool operator>=(const train_station_ind &other) const {
      return !(*this < other);
    }
    bool operator<=(const train_station_ind &other) const {
      return !(*this > other);
    }
  };

  struct train_time_cost {
    std::string trainID;
    int time;
    int cost;
    Time::time leave, arrive;
    std::string from, to;
    int seat;

    friend std::ostream &operator<<(std::ostream &output, const train_time_cost &t) {
      output << t.trainID << ' ' << t.from << ' ' << t.leave << " -> " << t.to << ' ' << t.arrive << ' ' << t.cost << ' ' << t.seat;
      return output;
    }
  };


  class Compare_t_id {
  public:
    constexpr bool operator()(const train_time_cost &t1, const train_time_cost &t2) const {
      if (t1.time > t2.time) {
        return true;
      }
      if (t1.time < t2.time) {
        return false;
      }
      return t1.trainID > t2.trainID;
    }
  };

  class Compare_c_id {
  public:
    constexpr bool operator()(const train_time_cost &t1, const train_time_cost &t2) const {
      if (t1.cost > t2.cost) {
        return true;
      }
      if (t1.cost < t2.cost) {
        return false;
      }
      return t1.trainID > t2.trainID;
    }
  };

}

#endif //TRAIN_H
