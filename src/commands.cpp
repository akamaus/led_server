#include "commands.hpp"
#include "controller.hpp"

template <typename T> struct str_assoc {
    T value;
    const char *str;
};

template <typename T>
const char *lookup_string(str_assoc<T>table[], T v) {
    for (int i=0; i<ARRAY_SIZE(table); i++) {
        if (table[i].value == v)
            return table[i].str;
    }
    return nullptr;
}

template <typename T>
const T *lookup_value(str_assoc<T>table[], const string &str) {
    for (int i=0; i<ARRAY_SIZE(table); i++) {
        if (str == table[i].str)
            return &table[i].value;
    }
    return nullptr;
}

static str_assoc<bool> led_state_table[] = {
    { true, "on"},
    { false, "off"}
};

Result set_led_state(vector<string> args) {
    const bool *tgt = lookup_value(led_state_table, args[0]);
    if (tgt) {
        g_led_enabled.store(*tgt);
        return Result(Status::OK);
    } else {
        return Result(Status::FAILED);
    }
}

Result get_led_state(vector<string> args) {
    return Result(Status::OK, g_led_enabled.load()?"on":"off");
}

static str_assoc<Color> led_colors_table[] = {
    { Color::Red, "red" },
    { Color::Green, "green" },
    { Color::Blue, "blue"}
};

command g_commands[CMD_NUM] = { {"set-led-state", 1, set_led_state},
                                {"get-led-state", 0, get_led_state}
                              };
