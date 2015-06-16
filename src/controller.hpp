#pragma once

#include <atomic>

void *controller_loop(void *unused);

enum Color {Red, Green, Blue};

extern std::atomic<bool> g_led_enabled;
extern std::atomic<Color> g_led_color;
extern std::atomic<int>    g_led_period;
