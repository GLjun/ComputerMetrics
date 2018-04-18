/*************************************************************************
	> File Name: IOTrace.c
	> Author: CGN
	> Mail: 
	> Created Time: 五  4/13 16:59:22 2018
 ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include "util.h"
#define MAX_FUNC_NAME_LEN 128
extern void clean() __attribute__((destructor));

ssize_t read(int fd, void* buf, size_t count) {
    static ssize_t (*fptr)(int, void*, size_t) = NULL;
    if(fptr == NULL) {
        fptr = (ssize_t (*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    ssize_t ret = (*fptr)(fd, buf, count);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
    LOG("%s", buf);
    return ret;
}

ssize_t write(int fd, void* buf, size_t count) {
    static ssize_t (*fptr)(int, void*, size_t) = NULL;
    if(fptr == NULL) {
        fptr = (ssize_t (*)(int, void*, size_t))dlsym(RTLD_NEXT, "write");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    LOG("%s", buf);
    ssize_t ret = (*fptr)(fd, buf, count);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
    fflush(stdout);
    return ret;
}






void clean() {
    LOGINFO("clean\n");
}



#ifdef __cplusplus
}
#endif
