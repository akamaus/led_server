#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

enum Status { OK, FAILED };
class Result {
public:
    Result(Status _s, string _v=""):status(_s), value(_v){}
    Status status;
    string value;
};

typedef Result (*cmd)(const vector<string> &);

struct command {
    const char *cmd_str;
    int num_args;
    cmd func;
};

extern vector<command> g_commands;
