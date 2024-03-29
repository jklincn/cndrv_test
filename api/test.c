#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#include "cn_api.h"
#include "common.h"

#define MEMORY_SIZE 0x100000
#define CN_CHECK(func)                                                       \
    do {                                                                     \
        CNresult ret = func;                                                 \
        if (ret) {                                                           \
            printf("%s:%d %s return %d FAILED\n", __func__, __LINE__, #func, \
                   ret);                                                     \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
    } while (0)

void* test_thread(void* arg);

CNdev dev[8];
void* host_mem_ptr[8];
CNaddr dev_mem_ptr[8];
int thread_id[8];
pthread_t thread[8];
CNcontext ctx[8];

int main(int argc, char** argv) {
    int count;

    CN_CHECK(cnInit(0));
    CN_CHECK(cnDeviceGetCount(&count));
    assert(count == 8);

    for (int i = 0; i < 8; i++) {
        CN_CHECK(cnDeviceGet(&dev[i], i));
        CN_CHECK(cnMallocHost(&host_mem_ptr[i], MEMORY_SIZE));
        printf("dev %d alloc host memory %p\n", i, host_mem_ptr[i]);
        CN_CHECK(pthread_create(&thread[i], NULL, test_thread, (void*)&i));
        pthread_join(thread[i], NULL);
        cnFreeHost(host_mem_ptr[i]);
    }
}

void* test_thread(void* arg) {
    int i = *(int*)arg;
    cnCtxCreate(&ctx[i], 0, dev[i]);
    CN_CHECK(cnMalloc(&dev_mem_ptr[i], MEMORY_SIZE));
    printf("dev %d alloc dev memory 0x%lx\n", i, dev_mem_ptr[i]);
    TIME_CALC(
        CN_CHECK(cnMemcpyHtoD(dev_mem_ptr[i], host_mem_ptr[i], MEMORY_SIZE)));
    cnCtxDestroy(ctx[i]);
    pthread_exit(NULL);
}