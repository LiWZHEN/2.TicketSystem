#ifndef USER_H
#define USER_H

#include "fixed_str.h"
#include "fixed_Chinese.h"

namespace user {
  struct user_info {
    fixed_str<20> username;
    fixed_str<30> password;
    fixed_Chinese<5> name;
    fixed_str<30> mailAddr;
    int privilege;
    // bool online = false;

    bool operator<(const user_info &) const {
      return true;
    }
    bool operator>(const user_info &) const {
      return true;
    }
    bool operator==(const user_info &) const {
      return true;
    }
    bool operator<=(const user_info &) const {
      return true;
    }
    bool operator>=(const user_info &) const {
      return true;
    }
    bool operator!=(const user_info &) const {
      return true;
    }
  };
}

#endif //USER_H
