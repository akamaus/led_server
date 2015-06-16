#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>

#include "commands.hpp"

#define INPUT_FIFO "/tmp/led_input"
#define OUTPUT_FIFO "/tmp/led_output"

constexpr int BUF_SIZE=1024;

typedef FILE* FIFO;
enum class FIFO_ROLE { Reader, Writer};

FIFO open_fifo(const char *path, FIFO_ROLE role) {
    if (access(path, F_OK ) != -1 ) {
    } else {
        if (mkfifo(path, 0666) != 0) throw std::runtime_error(strerror(errno));
    }
    FILE *handle = fopen(path, role == FIFO_ROLE::Reader? "r":"w");
    if (handle == nullptr) throw std::runtime_error(strerror(errno));
    return handle;
}

Result interpret_line(char *line) {
    string cmd(strtok(line, " \n"));
    vector<string> args;
    char *arg;
    while(arg = strtok(nullptr, " \n")) {
        args.push_back(arg);
    }

    for (int i=0; i<sizeof(g_commands) / sizeof(g_commands[0]); i++) {
        if (cmd == g_commands[i].cmd_str) {
            if (g_commands[i].num_args == args.size()) {
                return g_commands[i].func(args);
            } else {
                fprintf(stderr, "wrong args number\n");
                return Result(Status::FAILED);
            }
        }
    }
    fprintf(stderr, "unknown command '%s'\n", cmd.c_str());
    return Result(Status::FAILED);
}

int main(int argc, char *argv[]) {
    char line_buffer[BUF_SIZE];

    while(true) {
        FIFO input = open_fifo(INPUT_FIFO, FIFO_ROLE::Reader);
        FIFO output = nullptr;
        while(fgets(line_buffer, BUF_SIZE, input)) {
            fprintf(stderr, "got %s", line_buffer);
            Result res = interpret_line(line_buffer);
            if (!output) {
                output = open_fifo(OUTPUT_FIFO, FIFO_ROLE::Writer);
            }
            fprintf(output, "%s%s%s\n", (res.status == Status::OK) ? "OK":"FAILED", res.value.size()==0 ? "" :" ", res.value.c_str());

        }
        fprintf(stderr, "closing\n");
        fclose(input);
        fclose(output);
    }
}
