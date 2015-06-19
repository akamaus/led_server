#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

enum Status { OK, FAILED };
class Result {
public:
    Result(Status _s, string _v=""):status(_s), value(_v){}
    Status status;
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
