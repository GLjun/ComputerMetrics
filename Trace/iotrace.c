/*************************************************************************
	> File Name: IOTrace.c
	> Author: CGN
	> Mail: 
	> Created Time: 五  4/13 16:59:22 2018
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include "util.h"
#define MAX_FUNC_NAME_LEN 128

ssize_t read(int fd, void* buf, size_t count) {
    static void * (*fptr)() = NULL;

    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "read");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    ssize_t ret = (*fptr)(fd, buf, count);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
    return ret;
}
