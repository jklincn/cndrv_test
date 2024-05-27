#include "test.h"

#define CARDNUM 3
#define DEFAULT_SIZE 0x40000000L  // 1GB
#define REPEAT_TIMES 10

int main(int argc, char **argv) {
    int ret;

    int dev_fd[CARDNUM];
    char tmp_str[100];
    for (int i = 0; i < CARDNUM; i++) {
        // 打开 8 个设备节点
        sprintf(tmp_str, "/dev/cambricon_c10Dev%d", i);
        dev_fd[i] = open(tmp_str, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0666);
        assert(dev_fd[i] > 0);
        // 获取bdf
        int domain_id, bus_id, device_id, slot_id, func_id;
        u_int32_t attr_v2[CN_DEVICE_ATTRIBUTE_MAX];
        struct cn_device_attr attr_c20 = {
            .version = 1,
            .cnt = CN_DEVICE_ATTRIBUTE_MAX,
            .data = attr_v2,
        };
        ret = ioctl(dev_fd[i], CAMB_GET_DEVICE_ATTR, &attr_c20);
        assert(!ret);
        domain_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID];
        bus_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_BUS_ID];
        device_id = attr_v2[CN_DEVICE_ATTRIBUTE_PCI_DEVICE_ID];
        slot_id = device_id >> 3;
        func_id = device_id & 0x7;
        printf("Card %d BDF: %04x:%02x:%02x.%x\n", i, domain_id, bus_id,
               slot_id, func_id);
        // 开始跑测试，参数如下：
        // DMA_H2D/mode=QUICK_MODE/DEFAULT_SIZE/mem_mode=PINNED
        // noaligned/dma_mode=ASYNC_MODE/thread_num=1
        // 让设备驱动程序申请一块固定主机内存
        struct pinned_mem_param pm_param = {.size = DEFAULT_SIZE};
        ret = ioctl(pin_fd, M_PINNED_MEM_ALLOC, &pm_param);
        assert(!ret);
        void *host_addr;
        host_addr = (void *)pm_param.uaddr;
        uint64_t dev_addr;
        memset(host_addr, 0, DEFAULT_SIZE);
        // 分配设备内存
        struct mem_alloc_compat_s mm_alloc = {
            .size = DEFAULT_SIZE,
            .align = 0x10000,
            .type = 2,
            .affinity = -2,
        };
        ret = ioctl(dev_fd[i], M_MEM_ALLOC, &mm_alloc);
        assert(!ret);
        dev_addr = mm_alloc.ret_addr;
        // 创建一个队列
        void *queue = NULL;
        struct sbts_create_queue param = {
            .version = SET_VERSION(6U, SBTS_VERSION),
            .flags = 0,
            .priority = 0,
            .dump_uvaddr = 0,
        };
        ret = ioctl(dev_fd[i], SBTS_CREATE_QUEUE, &param);
        assert(!ret);
        // 准备传输任务
        queue = (void *)param.hqueue;
        struct sbts_queue_invoke_task invoke_task = {
            .version = 0,
            .hqueue = (uint64_t)queue,
            .correlation_id = 0,
            .task_type = SBTS_QUEUE_DMA_ASYNC,
            .priv_data.dma_async.version = SBTS_VERSION,
            .priv_data.dma_async.dir = 0,
            .priv_data.dma_async.is_udvm_support = false,
            .priv_data.dma_async.memcpy.src_addr = (uint64_t)host_addr,
            .priv_data.dma_async.memcpy.dst_addr = dev_addr,
            .priv_data.dma_async.memcpy.size = DEFAULT_SIZE,
            .priv_data.dma_async.memcpy.mem.src_fd = -1,
            .priv_data.dma_async.memcpy.mem.dst_fd = dev_fd[i]};
        // 开始测时
        time_start();
        for (int count = 0; count < REPEAT_TIMES; count++) {
            // 开始传输
            ret = ioctl(dev_fd[i], SBTS_QUEUE_INVOKE_TASK, &invoke_task,
                        sizeof(invoke_task));
            assert(!ret);
            struct sbts_queue_invoke_task invoke_task2 = {
                .version = 0,
                .hqueue = (uint64_t)queue,
                .correlation_id = 0,
                .task_type = SBTS_QUEUE_SYNC_TASK};
            // 可能是同步任务或者检查传输是否已完成？
            ret = ioctl(dev_fd[i], SBTS_QUEUE_INVOKE_TASK, &invoke_task2,
                        sizeof(invoke_task2));
            assert(!ret);
        }
        // 输出时间间隔
        long interval = time_end_usec();
        printf("Bandwidth: %.2f GB/s\n\n", (double)DEFAULT_SIZE / 1024 / 1024 /
                                               1024 / interval * 1000000L *
                                               REPEAT_TIMES);
        // 释放主机内存
        unsigned long uaddr = (unsigned long)(char *)host_addr;
        ret = ioctl(pin_fd, M_PINNED_MEM_FREE, &uaddr);
        assert(!ret);
        // 释放设备内存
        struct mem_free_param_s mm_free = {.ret_addr = dev_addr};
        ret = ioctl(dev_fd[i], M_MEM_FREE, &mm_free);
        assert(!ret);
    }
}