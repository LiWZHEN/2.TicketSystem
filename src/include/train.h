#ifndef TRAIN_H
#define TRAIN_H

#include "fixed_str.h"
#include "fixed_Chinese.h"
#include "time_format.h"

namespace train {
  struct train_info {
    fixed_str<20> trainID;
    int stationNum;
    fixed_Chinese<10> stations[100];
    int seatNum;
    int prices[99];

  };
}

#endif //TRAIN_H
