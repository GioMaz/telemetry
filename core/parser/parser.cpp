#include <stdio.h>

#include "parser.h"

Parser::~Parser()
{
    if (this->f) {
        fclose(this->f);
    }
}

void Parser::clear()
{
    fclose(this->f);
    column_names.clear();
    rows.clear();
}

bool Parser::open(char *filename)
{
    this->f = NULL;
    this->column_names.clear();
    this->rows.clear();

    this->f = fopen(filename, "r");
    return (this->f != NULL);
}

static bool is_trimmable(char c)
{
    return c == ' '
        || c == '\r'
        || c == '\t';
}

char Parser::read_cell(char buf[BUF_SIZE])
{
    int pos = 0;

    // Trim left
    char c;
    do {
        c = fgetc(this->f);
    } while (is_trimmable(c));

    // Read cell
    while (c != ',' && c != '\n' && c != EOF) {
        buf[pos++] = c;
        c = fgetc(this->f);
    }

    // Trim right
    while (pos > 0 && is_trimmable(buf[--pos]));
    buf[pos+1] = '\0';

    // std::cout << "_" << c << "_\n";
    return c;
}

char Parser::read_row(bool are_rows)
{
    std::vector<std::string> row;

    char c;
    do {
        char buf[BUF_SIZE];
        c = read_cell(buf);

        (are_rows ? row : this->column_names)
            .push_back(std::string(buf));

    } while (c != '\n' && c != EOF);

    if (row.size() == this->column_names.size()) {
        this->rows.push_back(row);
    }

    return c;
}

void Parser::read_all()
{
    // Read column names
    read_row(false);

    // Read all lines
    while (read_row() != EOF);
}

std::map<std::string, std::vector<double>> Parser::to_mapd()
{
    std::map<std::string, std::vector<double>> map;
    for (const auto &row : this->rows) {
        for (int i = 0; i < row.size(); i++) {
            double d;
            sscanf(row[i].data(), "%lf", &d);
            map[this->column_names[i]].push_back(d);
        }
    }
    return map;
}

std::map<std::string, std::vector<std::string>> Parser::to_maps()
{
    std::map<std::string, std::vector<std::string>> map;
    for (const auto &row : this->rows) {
        for (int i = 0; i < row.size(); i++) {
            map[this->column_names[i]].push_back(row[i]);
        }
    }
    return map;
}
