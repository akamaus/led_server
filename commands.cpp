#include "commands.hpp"

Result set_led_state(vector<string> args) {
    if (args[0] == "on") {
        fprintf(stderr, "enabling led\n");
        return Result(Res::OK);
    } else if (args[0] == "off") {
        fprintf(stderr, "disabling led\n");
        return Result(Res::OK);
    } else {
        fprintf(stderr, "unknown arg %s\n", args[0].c_str());
        return Result(Res::FAILED);
    }
}

Result get_led_state(vector<string> args) {
    return Result(Res::OK, "on");
}

command g_commands[CMD_NUM] = { {"set-led-state", 1, set_led_state},
                                {"get-led-state", 0, get_led_state}
                              };
