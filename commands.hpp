#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

enum Res { OK, FAILED };
class Result {
public:
    Result(Res _r, string _v=""):res(_r), value(_v){}
    Res res;
    string value;
};

typedef Result (*cmd)(vector<string>);

struct command {
    const char *cmd_str;
    int num_args;
    cmd func;
};

constexpr int CMD_NUM=2;
extern command g_commands[CMD_NUM];
