#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <stdexcept>

typedef int FIFO;

FIFO open_fifo(const char *path, int flags) {
    if (access(path, F_OK ) != -1 ) {
    } else {
        if (mkfifo(path, 0666) != 0) throw std::runtime_error(strerror(errno));
    }
    int handle = open(path, flags);
    if (handle == -1) throw std::runtime_error(strerror(errno));
    return handle;
}

int main(int argc, char *argv[]) {
    FIFO input = open_fifo("/tmp/led_input", O_RDONLY);
    FIFO output = open_fifo("/tmp/led_output", O_WRONLY);


}
