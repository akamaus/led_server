#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>

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

constexpr int BUF_SIZE=1024;

int main(int argc, char *argv[]) {

//    FIFO output = open_fifo("/tmp/led_output", FIFO_ROLE::Writer);

    char line_buffer[BUF_SIZE];

    while(true) {
        FIFO input = open_fifo("/tmp/led_input", FIFO_ROLE::Reader);
        while(fgets(line_buffer, BUF_SIZE, input)) {
            fprintf(stderr, "got %s", line_buffer);
        }
        fprintf(stderr, "closing\n");
        fclose(input);
    }
}
