# Ticket System 2025
### <p align="right"> ——火车票管理系统

## Part 0: CMU15445 Project0~2

The repository: <https://github.com/LiWZHEN/CMU15445.git>

## Part 1: B+ Tree

### How to use

- Include the header file:

      # include "b_plus_tree.h"

- Construction:

      sjtu::bpt\<Value\> bpt_name("map_file_name.txt", "data_file_name.txt");

- The functions:
  - insert
  
        bpt_name.Insert(index, value);

  - delete

        bpt_name.Delete(index, value);

  - find

        const auto ans_vector = bpt_name.Find(index); // ans_vector keeps all the values of "index"

### Inner structure

- No LRU cache pool

- The size of a page is 4096 bit

## Part 2: Ticket System