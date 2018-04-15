/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 六  4/14 15:03:01 2018
 ************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "util.h"

int main()
{
    int fd = open("data.txt", O_RDONLY);

    CHECK(fd != -1);
    char buf[256];
    ssize_t size = read(fd, buf, 256);
    LOG("%d", size);
    LOGINFO(buf);
    close(fd);
    return 0;
}


