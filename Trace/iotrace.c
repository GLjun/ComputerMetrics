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
#include "util.h"
#define MAX_FUNC_NAME_LEN 128

extern void trace_begin() __attribute__((constructor));
extern void trace_end() __attribute__((destructor));

typedef struct io_record {
    
    
} io_record;

void trace_begin() {

}

void trace_end() {
    LOGINFO("clean\n");
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    int ret = (*fptr)(s);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    int ret = (*fptr)(s);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
    return ret;
}

int ungetc(int c, FILE *stream) {
    static void * (*fptr)() = NULL;
    if(fptr == NULL) {
        fptr = (void *(*)())dlsym(RTLD_NEXT, "ungetc");
        CHECK(fptr != NULL);
    }
    TIME_T ts, tt;
    MARK_TIME(ts);
    int ret = (*fptr)(c, stream);
    MARK_TIME(tt);
    LOG("%lf", DIFF_TIME(tt, ts));
    return ret;
}

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
    LOG("%lf", DIFF_TIME(tt, ts));
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
    LOG("%lf", DIFF_TIME(tt, ts));
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
