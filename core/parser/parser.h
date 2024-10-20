#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "../common.h"

struct Parser {
    FILE *f = NULL;

    std::vector<std::string> column_names;
    std::vector<std::vector<std::string>> rows;

    ~Parser();

    bool open(char *filename);

    // Return the last character read
    char read_cell(char buf[BUF_SIZE]);
    char read_row(bool are_items = true);

    void read_all();

    // Return (column_name, column_items) pairs
    std::map<std::string, std::vector<double>> to_mapd();
    std::map<std::string, std::vector<std::string>> to_maps();
};
