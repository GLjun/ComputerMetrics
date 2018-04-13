/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月10日 星期二 16时44分52秒
 ************************************************************************/

#include "GZProf.h"
#include <stdio.h>
#include <iostream>

void swap_int(int& a, int&b)
{
    int tmp = a;
    a = b;
    b = a;
}

void swap_int2(int& a, int&b)
{
    int tmp = a;
    a = b;
    b = a;
}

void swap_int3(int& a, int&b)
{
    int tmp = a;
    a = b;
    b = a;
}

void test3()
{
    int a[] = {0, 1};
    int n = 350;
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < n; ++k)
                swap_int3(a[0], a[1]);
    }
    printf("test3 a %d %d\n", a[0], a[1]);
}

void test2()
{
    int a[] = {0, 1};
    int n = 400;
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < n; ++k)
                swap_int2(a[0], a[1]);
    }
    printf("test2 a %d %d\n", a[0], a[1]);
    test3();
}

void test()
{
    int a[] = {0, 1};
    int n = 500;
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < n; ++k)
                swap_int(a[0], a[1]);
    }

    test2();

    printf("test a %d %d\n", a[0], a[1]);
}



int main(int argc, char** argv)
{
    
    GZ::gzprof_start();
    test();
    return 0;
}
