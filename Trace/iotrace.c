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
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <string.h>
#include "util.h"
#define MAX_FUNC_NAME_LEN 128

extern void trace_begin() __attribute__((constructor));
extern void trace_end() __attribute__((destructor));


typedef struct io_record {
    int read_cnt;
    int write_cnt;
    size_t read_size;
    size_t write_size;
    double read_time;
    double write_time;
} io_record;

static io_record rw_record;

static void inc_read_log(int size, double time) {
    rw_record.read_cnt ++;
    rw_record.read_size += size;
    rw_record.read_time += time;
}

static void inc_write_log(int size, double time) {
    rw_record.write_cnt ++;
    rw_record.write_size += size;
    rw_record.write_time += time;
}

void trace_begin() {
    //init rw_record
    rw_record.read_cnt = 0;
    rw_record.write_cnt = 0;
    rw_record.read_size = 0;
    rw_record.write_size = 0;
    rw_record.read_time = 0.0;
    rw_record.write_time = 0.0;
    LOGINFO("finished initializing trace!");
}

void trace_end() {
    LOGINFO("read:");
    LOG("\tcount: %d", rw_record.read_cnt);
    LOG("\ttotal size : %d", rw_record.read_size);
    LOG("\taverage size: %.2f", 1.0 * rw_record.read_size / rw_record.read_cnt);
    LOG("\ttotal time : %f", rw_record.read_time);
    LOG("\taverage time: %f", rw_record.read_time, rw_record.read_cnt);
    LOGINFO("write:");
    LOG("\tcount: %d", rw_record.write_cnt);
    LOG("\ttotal size : %d", rw_record.write_size);
    LOG("\taverage size: %.2f", 1.0 * rw_record.write_size / rw_record.write_cnt);
    LOG("\ttotal time : %f", rw_record.write_time);
    LOG("\taverage time: %f", rw_record.write_time, rw_record.write_cnt);
}



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

    inc_read_log(ret, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d", "string", ret);

    return ret;
}

ssize_t write(int fd, const void* buf, size_t count) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "write");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    ssize_t ret = (*fptr)(fd, buf, count);
    MARK_TIME(tt);

    inc_write_log(ret, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d", "string", ret);
    return ret;
}

int fputc(int c, FILE* stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fputc");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(c, stream);
    MARK_TIME(tt);
    inc_write_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}

int putc(int c, FILE* stream) __attribute__((alias("fputc")));

int fputs(const char* s, FILE* stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fputs");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(s, stream);
    MARK_TIME(tt);
    inc_write_log(ret, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d", "string", ret);
    return ret;
}

int putchar(int c) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "putchar");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(c);
    MARK_TIME(tt);
    inc_write_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}

int puts(const char* s) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "puts");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(s);
    MARK_TIME(tt);
    inc_write_log(ret, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d", "string", ret);
    return ret;
}

int fgetc(FILE *stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fgetc");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(stream);
    MARK_TIME(tt);
    inc_read_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}

char *fgets(char *s, int size, FILE *stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fgets");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    char* ret = (*fptr)(s, size, stream);
    MARK_TIME(tt);
    int len = (ret != NULL) ? strlen(ret) : 0;
    inc_read_log(len, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","string", len);
    return ret;
}

int getc(FILE *stream){
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "getc");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(stream);
    MARK_TIME(tt);
    inc_read_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}

int getchar() {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "getchar");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)();
    MARK_TIME(tt);
    inc_read_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}

/*int ungetc(int c, FILE *stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "ungetc");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(c, stream);
    MARK_TIME(tt);
    inc_read_log(1, DIFF_TIME(tt, ts));
    LOG("type: %s, size: %d","char", 1);
    return ret;
}*/

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fread");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    size_t ret = (*fptr)(ptr, size, nmemb, stream);
    MARK_TIME(tt);

    inc_read_log(ret, DIFF_TIME(tt, ts));
    LOG("type size:%d, count: %d", size, ret);
    return ret;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "fwrite");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    size_t ret = (*fptr)(ptr, size, nmemb, stream);
    MARK_TIME(tt);
    inc_write_log(ret, DIFF_TIME(tt, ts));
    LOG("type size:%d, count: %d", size, ret);
    return ret;
}

void exit_group(int no)
{
    static void * (*fptr)() = NULL;

    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "exit_group");
        CHECK(fptr != NULL);
    }
    LOGINFO("exit");
}

void _exit(int no)
{
    static void * (*fptr)() = NULL;

    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "_exit");
        CHECK(fptr != NULL);
    }
    LOGINFO("exit");
}

void exit(int no)
{
    static void * (*fptr)() = NULL;

    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "exit");
        CHECK(fptr != NULL);
    }
    LOGINFO("exit");
}

#ifdef __cplusplus
}
#endif
