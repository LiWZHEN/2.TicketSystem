# Ticket System 2025
### <p align="right"> ——火车票管理系统

## Part 0: CMU15445 Project0~2

The repository: <https://github.com/LiWZHEN/CMU15445.git>

## Part 1: B+ Tree

### The functions:
  - insert
  - delete
  - find
  - size
  - clean

### Inner structure

  - No LRU cache pool
  - The size of a page is 4096 bit

## Part 2: Ticket System

### User-related

#### functions
  - add_user
  - login
  - logout
  - query_profile
  - modify_profile

#### structure
  - in the memory
    - a map with key being username and value being user_info.
  - file (of bpt)
    - users_map_info.txt, users_data.txt

### Train-related

#### functions
  - add_train
  - delete_train
  - release_train
  - query_train
  - query_ticket
  - query_transfer

#### structure
  - file (of bpt)
    - train_id_map_info.txt, train_id.txt: fixed_str<20> trainID -> int train_id.
    - station_map_info.txt, station.txt: fixed_Chinese<10> station -> int train_id.
  - file
    - train_info.txt: store all information of a train at targeted address.

### Ticket-related

#### functions
  - buy_ticket
  - query_order
  - refund_ticket

#### structure
  - file (of bpt)
    - waiting_map_info.txt, waiting.txt: fixed_str<20> trainID -> {time, require_num, username}.

### Others

#### functions
  - clean
  - exit