#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>

#include "commands.hpp"
#include "controller.hpp"

#define INPUT_FIFO "/tmp/led_input"
#define OUTPUT_FIFO "/tmp/led_output"

constexpr int BUF_SIZE=1024;

typedef FILE* FIFO;
enum class FIFO_ROLE { Reader, Writer};

void create_fifo(const char *path) {
    if (access(path, F_OK ) == -1 ) {
        if (mkfifo(path, 0666) != 0) throw std::runtime_error(string("can't create fifo: ") + strerror(errno));
    } else {
        struct stat s_res;
        memset(&s_res, 0, sizeof(s_res));
        if (stat(path, &s_res) == -1) {
            throw std::runtime_error(string("can't stat fifo") + strerror(errno));
        } else if (!S_ISFIFO(s_res.st_mode)) {
            throw std::runtime_error(string("file exists but is not fifo: ") + path);
        }
    }
}

FIFO open_fifo(const char *path, FIFO_ROLE role) {
    FILE *handle = fopen(path, role == FIFO_ROLE::Reader? "r":"w");
    if (handle == nullptr) throw std::runtime_error(string("can't open fifo: ") + strerror(errno));
    return handle;
}

Result interpret_line(char *line) {
    string cmd(strtok(line, " \n"));
    vector<string> args;
    char *arg;
    while(arg = strtok(nullptr, " \n")) {
        args.push_back(arg);
    }

    for (command &entry : g_commands) {
        if (cmd == entry.cmd_str) {
            if (entry.num_args == args.size()) {
                return entry.func(args);
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
    create_fifo(INPUT_FIFO);
    create_fifo(OUTPUT_FIFO);

    pthread_t thr;
    int res;
    res = pthread_create(&thr, nullptr, controller_loop, nullptr);
    if (res != 0) {
        throw std::runtime_error(strerror(res));
    }

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
