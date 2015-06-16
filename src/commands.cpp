#include "commands.hpp"
#include "controller.hpp"

Result set_led_state(vector<string> args) {
    if (args[0] == "on") {
        fprintf(stderr, "enabling led\n");
        g_led_enabled.store(true);
        return Result(Status::OK);
    } else if (args[0] == "off") {
        fprintf(stderr, "disabling led\n");
        g_led_enabled.store(false);
        return Result(Status::OK);
    } else {
        fprintf(stderr, "unknown arg %s\n", args[0].c_str());
        return Result(Status::FAILED);
    }
}

Result get_led_state(vector<string> args) {
    return Result(Status::OK, g_led_enabled.load()?"on":"off");
}

command g_commands[CMD_NUM] = { {"set-led-state", 1, set_led_state},
                                {"get-led-state", 0, get_led_state}
                              };
