#include <stdio.h>
#include <unistd.h>

#include "controller.hpp"

std::atomic<bool> g_led_enabled(false);
std::atomic<Color> g_led_color(Red);
std::atomic<int>    g_led_period(1000000);

void *controller_loop(void *unused) {
    while(1) {
        if (g_led_enabled.load()) {
            Color c = g_led_color.load();
            fprintf(stdout, c==Color::Red ? "<R>\n" : (c==Color::Green ? "<G>\n" : "<B>\n"));
        }
        usleep(g_led_period.load());
    }
}
