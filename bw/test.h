#ifndef TEST_H
#define TEST_H

#include <linux/ioctl.h>
#include <stdint.h>

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

enum {
    _CNDEV_CARDNUM = 0,
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

#define CN_DEVICE_ATTRIBUTE_MAX 1036
#define CN_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID 1032
#define CN_DEVICE_ATTRIBUTE_PCI_BUS_ID 1030
#define CN_DEVICE_ATTRIBUTE_PCI_DEVICE_ID 1031
#endif  // TEST_H