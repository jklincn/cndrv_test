#include "test.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define CARDNUM 8

int main(int argc, char **argv) {
    int ctl_fd;
    int ret;
    // 打开控制节点
    ctl_fd = open("/dev/cambricon_ctl", O_RDWR | O_TRUNC | O_SYNC, 0666);
    // 获取卡的数量
    struct cndev_cardnum param;
    ioctl(ctl_fd, MONITOR_CNDEV_CARDNUM, &param);
    assert(param.card_count == CARDNUM);

    int dev_fd[CARDNUM];
    char tmp_str[100];
    for (int i = 0; i < CARDNUM; i++) {
        // 打开 8 个设备节点
        sprintf(tmp_str, "/dev/cambricon_dev%d", i);
        dev_fd[i] = open(tmp_str, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0666);
        assert(dev_fd[i] > 0);
        // 获取bdf
        int domain_id, bus_id, device_id, slot_id, func_id;
        u_int32_t attr_v2[CN_DEVICE_ATTRIBUTE_MAX];
        struct cn_device_attr attr_c20;
        attr_c20.version = 1;
        attr_c20.cnt = CN_DEVICE_ATTRIBUTE_MAX;
        attr_c20.data = attr_v2;
        ret = ioctl(dev_fd[i], CAMB_GET_DEVICE_ATTR, &attr_c20);
        assert(!ret);
        domain_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID];
        bus_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_BUS_ID];
        device_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_DEVICE_ID];
        slot_id = device_id >> 3;
        func_id = device_id & 0x7;
        printf("%04x:%02x:%02x.%x\n", domain_id, bus_id, slot_id, func_id);
    }
}