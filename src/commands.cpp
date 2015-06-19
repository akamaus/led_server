#include "commands.hpp"
#include "controller.hpp"

template <typename T> struct str_assoc {
    T value;
    string str;
};

template <typename T>
using lookup_table = vector<str_assoc<T>>;

static const lookup_table<bool> led_state_table = {
    { true, "on"},
    { false, "off"}
};

static lookup_table<Color> led_colors_table = {
    { Color::Red, "red" },
    { Color::Green, "green" },
    { Color::Blue, "blue"}
};

static lookup_table<int> led_periods_table = {
    { 100000,  "0"},
    { 1000000, "1"},
    { 2000000, "2"},
    { 3000000, "3"},
    { 4000000, "4"},
    { 5000000, "5"}
};

template <typename T>
Result table_getter(const lookup_table<T> &table, const std::atomic<T> &v) {
    for (auto &a : table) {
        if (a.value == v.load())
            return Result(Status::OK, a.str);
    }
    return Result(Status::FAILED);
}

template <typename T>
Result table_setter(const lookup_table<T> &table, std::atomic<T> &var, const string &new_val) {
    for (auto &a : table) {
        if (new_val == a.str) {
            var.store(a.value);
            return Result(Status::OK);
        }
    }
    return Result(Status::FAILED);
}

vector<command> g_commands = {
    {"set-led-state", 1, [](const vector<string> &args) -> Result { return table_setter(led_state_table, g_led_enabled, args[0]); } },
    {"get-led-state", 0, [](const vector<string> &args) -> Result { return table_getter(led_state_table, g_led_enabled); } },
    {"set-led-color", 1, [](const vector<string> &args) -> Result { return table_setter(led_colors_table, g_led_color, args[0]); } },
    {"get-led-color", 0, [](const vector<string> &args) -> Result { return table_getter(led_colors_table, g_led_color); } },
    {"set-led-rate", 1, [](const vector<string> &args) -> Result { return table_setter(led_periods_table, g_led_period, args[0]); } },
    {"get-led-rate", 0, [](const vector<string> &args) -> Result { return table_getter(led_periods_table, g_led_period); } }
};
