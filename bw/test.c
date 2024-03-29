#include <fcntl.h>

int main(int argc, char** argv) {
    int pin_fd;
    pin_fd = open("/dev/cambricon_ctl", O_RDWR | O_TRUNC | O_SYNC, 0666);
}