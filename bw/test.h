#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

struct cndev_cardnum {
    /* driver version */
    uint16_t version;
    /* total mlu card number */
    uint8_t card_count;
};

struct cn_device_attr {
    uint16_t version;
    int cnt;
    void *data;
};

// 有点奇怪，参数传的是cn_device_attr结构体，但宏定义检查的是c20_device_attr结构体
struct c20_device_attr {
    int size;
    void *data;
};

/*64-bit size compatible. Note that the padding parament don't be deleted at
 * anytime. If not, the size of struct xxx_compat_s is same as the old
 * struct's. Then it is impossible to distinguish the old or new struct.*/
struct mem_alloc_compat_s {
    uint64_t size;
    uint32_t align;
    uint32_t type;
    uint32_t affinity;
    uint32_t flag;
    uint64_t ret_addr;
    uint64_t padding;
};

/*MEM_FREE*/
struct mem_free_param_s {
    uint64_t ret_addr;
    uint32_t size;
};

struct pinned_mem_param {
    uint32_t size;
    uint64_t uaddr;
    uint64_t handle;
};

/*COPY_TO_DDR_ASYNC*/
struct mem_copy_h2d_async_compat_s {
    uint64_t version;
    uint64_t hqueue;
    uint64_t ca;
    uint64_t ia;
    uint64_t total_size;
    uint64_t residual_size;
};

struct sbts_create_queue {
    uint64_t version;
    uint64_t hqueue;
    uint64_t dump_uvaddr; /* reuse by ack_uvaddr */
    uint64_t flags;
    uint64_t priority;
};

struct sbts_kernel {
    uint64_t version;
    uint64_t priv_size;
    uint64_t priv;
    uint64_t param_size;
    uint64_t params;
};

struct sbts_notifier {
    uint64_t version;
    uint64_t hnotifier;
};

struct sbts_notifier_extra {
    uint64_t version;
    uint64_t hnotifier;
    uint64_t fd;
};

struct sbts_idc {
    uint64_t version;
    uint64_t host_addr;
    uint64_t val;
    uint64_t type;
    uint64_t flag;
    /* out val */
    uint64_t status;
    uint64_t ticket;
};

struct sbts_hostfn {
    uint64_t version;
    uint64_t hqueue;
    uint64_t hf_status;
    uint64_t seq;
    uint64_t host_get_trigger_ns;
    uint64_t hostfn_start_ns;
    uint64_t hostfn_end_ns;
    uint64_t extra_ptr;
    uint64_t extra_size;
    uint64_t reserve[7];
};

struct sbts_dma_async {
    uint64_t version;
    uint64_t dir;
    uint64_t is_udvm_support;
    union {
        struct {
            uint64_t src_addr;
            uint64_t dst_addr;
            uint64_t size;
            union {
                /* for udvm */
                uint64_t udvm_fd;
                /* for mlu200 */
                struct {
                    uint64_t src_fd;
                    uint64_t dst_fd;
                } mem;
            };
        } memcpy;

        struct {
            uint64_t fd;
            uint64_t dev_addr;
            uint64_t per_size;
            uint64_t number;
            uint64_t val;
        } memset;
    };
};

struct sbts_dma_priv {
    uint64_t dir;
    union {
        struct {
            uint64_t src_bus_set;
            uint64_t src_pminfo;
            uint64_t dst_bus_set;
            uint64_t dst_pminfo;
        } memcpy;

        struct {
            uint64_t bus_set;
            uint64_t pminfo;
        } memset;
    };
};

struct sbts_dbg_kernel {
    uint64_t version;
    uint64_t priv_size;
    uint64_t priv;
    uint64_t param_size;
    uint64_t params;
    uint64_t ack_buffer;
    uint64_t ack_buffer_size;
};

struct sbts_jpu_async {
    uint64_t version;
    uint32_t type;
    uint32_t is_batch_head;
    uint64_t dataq_addr;
    uint32_t dataq_size;
    uint32_t dataq_seg_size[4];
    uint64_t cb_func;
    uint32_t block_id;
    uint64_t buf_hdl;
    uint64_t efd_queue_sid;
    uint32_t reserved[8];
};

struct sbts_topo_notifier {
    uint64_t version;
    uint64_t hnotifier;
    /* for extra */
    uint64_t fd;
    /* usr / api */
    uint64_t type;
    uint64_t place_total;
    uint64_t qtask_total;
};

/* param invoke_queue_type in sbts_topo_invoke */
enum sbts_topo_invoke_queue_type {
    SBTS_TOPO_INVOKE_IN_LEADER_QUEUE = 0,
    SBTS_TOPO_INVOKE_IN_USER_QUEUE = 1,
    SBTS_TOPO_INVOKE_IN_USER_QUEUE_NUM,
};

struct sbts_topo_invoke {
    uint64_t invoke_queue_type;
};

#define MAX_PRIV_NUM (20)
union sbts_task_priv_data {
    struct sbts_kernel kernel;
    struct sbts_notifier notifier;
    struct sbts_idc idc;
    struct sbts_hostfn hostfn;
    struct sbts_dma_async dma_async;
    struct sbts_dbg_kernel dbg_kernel;
    struct sbts_notifier_extra notifier_extra;
    struct sbts_jpu_async jpu_async;
    struct sbts_topo_notifier topo_notifier;
    struct sbts_topo_invoke topo_invoke;
    uint64_t sbts_task_max_priv[MAX_PRIV_NUM];
};

#define SBTS_QUEUE_INVOKE_COMM_RES (5)
struct sbts_queue_invoke_task {
    uint64_t version;
    uint64_t hqueue;
    uint64_t correlation_id;
    uint16_t task_type;
    uint16_t dev_topo_cmd;
    uint32_t perf_disable;
    uint64_t topo_info;
    uint64_t dev_topo_id;
    uint32_t dev_topo_node_index;
    uint32_t reserve;
    uint64_t res[SBTS_QUEUE_INVOKE_COMM_RES];
    union sbts_task_priv_data priv_data __attribute__((aligned(8)));
} __packed;

enum {
    _CNDEV_CARDNUM = 0,
    _CNDEV_CARDINFO = 1,
    _CNDEV_POWERINFO = 2,
    _CNDEV_MEMINFO = 3,
    _CNDEV_PROCINFO = 4,
    _CNDEV_HEALTHSTATE = 5,
    _CNDEV_ECCINFO = 6,
    _CNDEV_VMINFO = 7,
    _CNDEV_IPUUTIL = 8,
    _CNDEV_CODECUTIL = 9,
    _CNDEV_IPUFREQ = 10,
    _CNDEV_CURBUSINFO = 11,
    _CNDEV_PCIE_THROUGHPUT = 12,
    _M_PINNED_MEM_ALLOC = 13,
    _M_PINNED_MEM_FREE = 14,
    _CNDEV_POWERCAPPING = 15,
    _CNDEV_IPUFREQ_SET = 16,
    _M_PINNED_MEM_GET_HANDLE = 17,
    _M_PINNED_MEM_CLOSE_HANDLE = 18,
    _M_PINNED_MEM_OPEN_HANDLE = 19,
    _M_PINNED_MEM_GET_MEM_RANGE = 20,
    _CNDEV_GET_IOCTL_ATTR = 21,
    _CNDEV_NCS_VERSION = 22,
    _CNDEV_NCS_STATE = 23,
    _CNDEV_NCS_SPEED = 24,
    _CNDEV_NCS_CAPABILITY = 25,
    _CNDEV_NCS_COUNTER = 26,
    _CNDEV_NCS_RESET_COUNTER = 27,
    _CNDEV_NCS_REMOTE_INFO = 28,
    /*29 reserve*/
    _M_PINNED_MEM_LAR4_ALLOC = 30,
    _M_PINNED_MEM_LAR4_GET_HANDLE = 31,
    _M_PINNED_MEM_LAR4_CLOSE_HANDLE = 32,
    _M_PINNED_MEM_LAR4_OPEN_HANDLE = 33,
    _M_PINNED_MEM_LAR4_GET_MEM_RANGE = 34,
    _CNDEV_CHASSISINFO = 35,
    _CNDEV_QOS_RESET = 36,
    _CNDEV_QOS_INFO = 37,
    _CNDEV_QOS_DESC = 38,
    _CNDEV_SET_QOS = 39,
    _CNDEV_SET_QOS_GROUP = 40,
    _CNDEV_ACPUUTIL = 41,
    _CNDEV_ACPUUTIL_TIMER = 42,
    _CNDEV_GET_RETIRE_PAGES = 43,
    _CNDEV_GET_RETIRE_STATUS = 44,
    _CNDEV_GET_REMAPPED_ROWS = 45,
    _CNDEV_RETIRE_SWITCH = 46,
    _CNDEV_NCS_CONFIG = 47,
    _CNDEV_MLULINK_SWITCH_CTRL = 48,
    _CNDEV_IPUFREQ_CTRL = 49,
    _CNDEV_NCS_INFO = 50,
    _M_PINNED_MEM_NODE_ALLOC = 51,
    _CNDEV_CARDINFO_EXT = 52,
    _CNDEV_HOST_CTRL = 53,
    _CNDEV_PROCESS_IPUUTIL = 54,
    _CNDEV_PROCESS_CODECUTIL = 55,
    _M_PINNED_MEM_FLAG_NODE_ALLOC = 56,
    _M_PINNED_MEM_HOST_GET_POINTER = 57,
    _M_PINNED_MEM_HOST_REGISTER = 58,
    _M_PINNED_MEM_HOST_UNREGISTER = 59,
    _M_PINNED_MEM_GET_FLAGS = 60,
    _CNDEV_GET_FEATURE = 61,
    _CNDEV_SET_FEATURE = 62,
    _CNDEV_GET_MIM_VMLU_PROFILE = 63,
    _CNDEV_GET_MIM_POSSIBLE_PLACE = 64,
    _CNDEV_GET_MIM_VMLU_CAPACITY = 65,
    _CNDEV_GET_MIM_DEVICE_INFO = 66,
    _CNDEV_MI_CARD = 67,
    _CNDEV_CARDNUM_EXT = 68,
    _CNDEV_GET_COUNTER = 69,
    _CNDEV_CHASSIS_POWER_INFO = 70,
    _CNDEV_MIM_MODE_SWITCH = 71,
    _CNDEV_SMLU_MODE_SWITCH = 72,
    _CNDEV_GET_SMLU_PROFILE_ID = 73,
    _CNDEV_GET_SMLU_PROFILE_INFO = 74,
    _CNDEV_NEW_SMLU_PROFILE = 75,
    _CNDEV_DELETE_SMLU_PROFILE = 76,
    _CNDEV_DEVICE_RESET = 77,
    _CNDEV_DEVICE_STATE = 78,
    _CNDEV_MAX,
};

enum attr_nr_type {
    _CAMB_GET_API_GLOBAL_SEQ_NR = 10,
    _CAMB_GET_DRIVER_INFO_NR,
    _CAMB_GET_DEVICE_INFO_NR,
    _CAMB_RD_DRIVER_VERSION_NR = 38,
    _CAMB_GET_DEVICE_ATTR_V1_NR = 46,
    _CAMB_GET_DEVICE_ATTR_NR,
    _CAMB_GET_DEVICE_PRIVATE_ATTR_NR,
    _CAMB_GET_MLU_ID_NR = 51,
    _CAMB_GET_DEVICE_NAME_NR,
    _CAMB_GET_API_LIMIT_VER_NR,
    _CAMB_DRIVER_CAPABILITY_NR,
    _CAMB_GET_DEVICE_WORK_MODE_NR,
    _CAMB_GET_DEVICE_UNIQUE_ID_NR,
    _ATTR_MAX_NR_COUNT,
};

enum mm_nr_type {
    _M_MEM_ALLOC = 1,
    _M_MEM_FREE,
    _M_MEM_MERGE,
    _M_MEM_COPY_H2D,
    _M_MEM_COPY_D2H,
    _M_FRAME_BUFFER_ALLOC = 7,
    _M_FB_MEM_ALLOC,
    _M_PHY_PEER_ABLE = 10,
    _M_MEM_COPY_D2D,
    _M_PEER_TO_PEER,
    _M_PEER_ABLE,
    _M_DMA_MEMSET,
    _M_IPCM_GET_HANDLE,
    _M_IPCM_OPEN_HANDLE,
    _M_IPCM_CLOSE_HANDLE,
    _M_MEM_COPY_ASYNC_H2D = 20,
    _M_MEM_COPY_ASYNC_D2H,
    _M_PEER_TO_PEER_ASYNC,
    _M_MDR_ALLOC,
    _M_ENABLE_MEMCHECK,
    _M_GET_MEM_RANGE,
    _M_MEM_COPY_ASYNC_D2D,
    _M_DMA_MEMSETD32,
    _M_DMA_MEMSET_ASYNC,
    _M_DMA_MEMSETD32_ASYNC,
    _M_MEM_BAR_COPY_H2D,
    _M_MEM_BAR_COPY_D2H,
    _M_DMA_MEMSETD16,
    _M_DMA_MEMSETD16_ASYNC,
    _M_MEM_SET_PROT,
    _M_PRT_USER_TRACE,
    _M_PRT_USER_TRACE_ENABLE,
    _M_MEM_GET_UVA,
    _M_MEM_PUT_UVA,
    _M_MEM_GET_IPU_RESV_MEM,
    _M_MEM_KERNEL_TEST,
    _M_MEM_COPY_D2D_2D,
    _M_MEM_COPY_D2D_3D,
    _M_PCIE_DOB_ALLOC,
    _M_PCIE_DOB_FREE,
    _M_PCIE_DOB_WRITE,
    _M_PCIE_DOB_READ,
    _M_PCIE_DOB_RPC_WRITE,
    _M_PCIE_DOB_RPC_READ,
    _M_PCIE_DOB_RPC_OPEN,
    _M_PCIE_DOB_RPC_CLOSE,
    _M_PCIE_SRAM_RPC_WRITE,
    _M_PCIE_SRAM_RPC_READ,

#ifdef PEER_FREE_TEST
    _M_INBD_SHM_ALLOC_TEST = 97,
    _M_OUTBD_SHM_ALLOC_TEST,
    _M_PEER_FREE_TEST,
#endif
    _MM_MAX_NR_COUNT,
};

enum dmaMode { SYNC_MODE, ASYNC_MODE, ASYNC_NO_BATCH_MODE };

/* module ioctl magic */
#define CAMBR_BUS_MAGIC 'B'     /* pcie bus */
#define CAMBR_MM_MAGIC 'M'      /* memory */
#define CAMBRICON_MAGIC_NUM 'Y' /* device attribute */
#define CAMBR_SBTS_MAGIC 'S'    /* sbts */
#define CAMBR_HB_MAGIC 'E'      /* exp_mgnt */
#define CAMBR_NCS_MAGIC 'N'     /* ncs */
#define CAMBR_MIGRATION_MAGIC 0xee
#define CAMBR_CTX_MAGIC 'C' /* ctx */

#define CAMBR_MONITOR_MAGIC 'Z'

#define MONITOR_CNDEV_CARDNUM \
    _IOR(CAMBR_MONITOR_MAGIC, _CNDEV_CARDNUM, unsigned long)
#define CAMB_GET_DEVICE_ATTR \
    _IOWR(CAMBRICON_MAGIC_NUM, _CAMB_GET_DEVICE_ATTR_NR, struct cn_device_attr)

#define M_PINNED_MEM_ALLOC \
    _IOW(CAMBR_MONITOR_MAGIC, _M_PINNED_MEM_ALLOC, struct pinned_mem_param)
#define M_PINNED_MEM_FREE \
    _IOW(CAMBR_MONITOR_MAGIC, _M_PINNED_MEM_FREE, unsigned long)
#define M_MEM_ALLOC \
    _IOW(CAMBR_MM_MAGIC, _M_MEM_ALLOC, struct mem_alloc_compat_s)

#define M_MEM_COPY_ASYNC_H2D                    \
    _IOW(CAMBR_MM_MAGIC, _M_MEM_COPY_ASYNC_H2D, \
         struct mem_copy_h2d_async_compat_s)
#define M_MEM_FREE _IOW(CAMBR_MM_MAGIC, _M_MEM_FREE, struct mem_free_param_s)
enum sbts_cmd_type {
    _SBTS_CREATE_QUEUE = 0,
    _SBTS_DESTROY_QUEUE,
    _SBTS_INVOKE_KERNEL, /* deprecated */
    _SBTS_QUEUE_SYNC,    /* deprecated */
    _SBTS_CORE_DUMP,
    _SBTS_NOTIFIER_CREATE,
    _SBTS_NOTIFIER_DESTROY,
    _SBTS_NOTIFIER_PLACE, /* deprecated */
    _SBTS_NOTIFIER_WAIT,
    _SBTS_NOTIFIER_QUERY,
    _SBTS_NOTIFIER_ELAPSED_TIME,
    _SBTS_NOTIFIER_QUEUE_WAIT, /* deprecated */
    _SBTS_QUEUE_QUERY,
    _SBTS_SET_LOCAL_MEM,       /* deprecated */
    _SBTS_GET_LOCAL_MEM,       /* deprecated */
    _SBTS_INVOKE_KERNEL_DEBUG, /* deprecated */
    _SBTS_INVOKE_CNGDB_TASK,
    _SBTS_INVOKE_TOPO_ENTITY, /* deprecated */
    _SBTS_NCS_COMM_CMD,
    _SBTS_NCS_INVOKE_KERNEL, /* deprecated */
    _SBTS_TCDP_COMM_CMD,
    _SBTS_INVOKE_TCDP, /* deprecated */
    _SBTS_RESERVE2,
    _SBTS_RESERVE3,
    _SBTS_RESERVE4,
    _SBTS_RESERVE5,
    _SBTS_RESERVE6,
    _SBTS_RESERVE7,
    _SBTS_IDC_PLACE_TASK, /* deprecated */
    _SBTS_GET_HW_INFO,
    _SBTS_NOTIFIER_ELAPSED_SW_TIME,
    _SBTS_GET_UNOTIFY_INFO,
    _SBTS_SET_UNOTIFY_FD,
    _SBTS_DEBUG_TASK, /* deprecated */
    _SBTS_DEBUG_CTRL,
    _SBTS_HW_CFG_HDL,
    _SBTS_CORE_DUMP_ACK,
    _SBTS_INVOKE_HOST_FUNC, /* deprecated */
    _SBTS_QUEUE_INVOKE_TASK,
    _SBTS_NOTIFIER_IPC_GETHANDLE,
    _SBTS_NOTIFIER_IPC_OPENHANDLE,
    _SBTS_MULTI_QUEUE_SYNC,
    _SBTS_TASK_TOPO_CTRL,
    _SBTS_CMD_NUM,
};

enum sbts_task_type {
    SBTS_QUEUE_KERNEL = 0,
    SBTS_QUEUE_NOTIFIER_PLACE,
    SBTS_QUEUE_NOTIFIER_WAIT,
    SBTS_QUEUE_IDC_PLACE,
    SBTS_QUEUE_HOSTFN_INVOKE,
    SBTS_QUEUE_NCS_KERNEL,
    SBTS_QUEUE_DMA_ASYNC,
    SBTS_QUEUE_DBG_KERNEL,
    SBTS_QUEUE_DBG_TASK,
    SBTS_QUEUE_SYNC_TASK,
    SBTS_QUEUE_TCDP_TASK,
    SBTS_QUEUE_TCDP_DBG_TASK,
    SBTS_QUEUE_NOTIFIER_WAIT_EXTRA,
    SBTS_QUEUE_JPU_TASK,
    SBTS_QUEUE_TOPO_INVOKE,
    SBTS_QUEUE_TASK_TYPE_NUM,
};

#define SBTS_CREATE_QUEUE \
    _IOW(CAMBR_SBTS_MAGIC, _SBTS_CREATE_QUEUE, struct sbts_create_queue)
#define SBTS_QUEUE_SYNC \
    _IOW(CAMBR_SBTS_MAGIC, _SBTS_QUEUE_SYNC, struct sbts_push_task)
#define SBTS_QUEUE_INVOKE_TASK                      \
    _IOW(CAMBR_SBTS_MAGIC, _SBTS_QUEUE_INVOKE_TASK, \
         struct sbts_queue_invoke_task)

#define CN_DEVICE_ATTRIBUTE_MAX 1036
#define CN_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID 1032
#define CN_DEVICE_ATTRIBUTE_PCI_BUS_ID 1030
#define CN_DEVICE_ATTRIBUTE_PCI_DEVICE_ID 1031

#define print_to_file(string, arg...)         \
    do {                                      \
        FILE *fp;                             \
        char file_name[50];                   \
        sprintf(file_name, "bandwidth_data"); \
        fp = fopen(file_name, "aw+");         \
        fseek(fp, 0, SEEK_END);               \
        fprintf(fp, string, ##arg);           \
        fclose(fp);                           \
        printf(string, ##arg);                \
    } while (0)

#define MLU_CHECK(func)                                                      \
    ({                                                                       \
        int ret = func;                                                      \
        if (ret) {                                                           \
            printf("%s@%d %s return %d FAILED\n", __func__, __LINE__, #func, \
                   ret);                                                     \
            exit(-1);                                                        \
        }                                                                    \
    })

#define SBTS_VERSION (1U)
#define SET_VERSION(host_ver, sbts_ver) \
    (((uint64_t)(host_ver) << 32) | (sbts_ver))

#endif  // TEST_H