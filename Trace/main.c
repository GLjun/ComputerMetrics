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

void test_by_read()
{
    int fd = open("data.txt", O_RDONLY);

    CHECK(fd != -1);
    char buf[256];
    ssize_t size = read(fd, buf, 256);
    write(fd, buf, 256);
    LOG("%d", size);
    LOGINFO(buf);
    close(fd);
}

void test_by_file()
{
    FILE* file = fopen("data.txt", "r");
    char line[256];
    fgets(line, 256, file);
    fputs(line, stdout);
    fclose(file);
}

int main()
{
    test_by_read();
    test_by_file();
    printf("helo\n");
 
    return 0;
}


