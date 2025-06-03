#ifndef TICKET_H
#define TICKET_H

#include "length_fixed_string/fixed_str.h"
#include "length_fixed_string/fixed_Chinese.h"
#include "time_format.h"

namespace ticket {

  struct waiting {
    int query_time_stamp;
    fixed_str<20> username;
    fixed_str<20> trainID;
    int ind_start, ind_destination;
    Time::date date;
    int require_seats;

    bool operator==(const waiting &other) const {
      return query_time_stamp == other.query_time_stamp;
    }
    bool operator<(const waiting &other) const {
      return query_time_stamp < other.query_time_stamp;
    }
    bool operator>(const waiting &other) const {
      return query_time_stamp > other.query_time_stamp;
    }
    bool operator!=(const waiting &other) const {
      return query_time_stamp != other.query_time_stamp;
    }
    bool operator<=(const waiting &other) const {
      return query_time_stamp <= other.query_time_stamp;
    }
    bool operator>=(const waiting &other) const {
      return query_time_stamp >= other.query_time_stamp;
    }
  };

  struct record {
    fixed_str<20> username;
    int query_time_stamp;
    fixed_str<20> trainID;
    fixed_Chinese<10> from, to;
    Time::time leaving, arriving;
    int price, num;
    int status; // 0: success, 1: pending, 2: refunded
    int ind_s, ind_t;

    bool operator==(const record &other) const {
      return query_time_stamp == other.query_time_stamp;
    }
    bool operator<(const record &other) const {
      return query_time_stamp < other.query_time_stamp;
    }
    bool operator>(const record &other) const {
      return query_time_stamp > other.query_time_stamp;
    }
    bool operator!=(const record &other) const {
      return query_time_stamp != other.query_time_stamp;
    }
    bool operator<=(const record &other) const {
      return query_time_stamp <= other.query_time_stamp;
    }
    bool operator>=(const record &other) const {
      return query_time_stamp >= other.query_time_stamp;
    }

    friend std::ostream &operator<<(std::ostream &output, const record &r) {
      if (r.status == 0) {
        output << "[success] ";
      } else if (r.status == 1) {
        output << "[pending] ";
      } else if (r.status == 2) {
        output << "[refunded] ";
      }
      output << r.trainID << ' ' << r.from << ' ' << r.leaving << " -> "
          << r.to << ' ' << r.arriving << ' ' << r.price << ' ' << r.num;
      return output;
    }
  };
}

#endif //TICKET_H
