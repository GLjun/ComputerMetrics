/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月20日 星期五 22时19分06秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include "util.h"


int do_work() {
    int a[7] = {1, 2, 3, 4, 5, 6, 7};
    int n = 100000;
    int sum = 0;
    for(int i = 0; i < n; i ++) {
        for(int j = 0;j < 7; j ++) {
            sum += a[j];          
        }
    }
    return sum;
}

int main(int argc, char **argv) {

    int EventSet = PAPI_NULL;
    long long values[10], values1[10];
    for(int i = 0; i < 10; i ++)
        values[i] = values1[i] = 0;

    CHECK(PAPI_library_init(PAPI_VER_CURRENT) == PAPI_VER_CURRENT);
    CHECK(PAPI_multiplex_init() == PAPI_OK);

    CHECK(PAPI_create_eventset(&EventSet) == PAPI_OK);

    //CHECK(PAPI_assign_eventset_component(EventSet, 0) == PAPI_OK);
    

    CHECK(PAPI_add_event(EventSet, PAPI_TOT_INS) == PAPI_OK);
    CHECK(PAPI_set_multiplex(EventSet) == PAPI_OK);
    CHECK(PAPI_add_event(EventSet, PAPI_TOT_CYC) == PAPI_OK);

    CHECK(PAPI_add_event(EventSet, PAPI_L1_TCM) == PAPI_OK);

    CHECK(PAPI_add_event(EventSet, PAPI_LD_INS) == PAPI_OK);
    CHECK(PAPI_add_event(EventSet, PAPI_SR_INS) == PAPI_OK);

    /*CHECK(PAPI_add_event(EventSet, PAPI_L2_TCM) == PAPI_OK);
    CHECK(PAPI_add_event(EventSet, PAPI_L2_TCA) == PAPI_OK);

    CHECK(PAPI_add_event(EventSet, PAPI_TLB_TL) == PAPI_OK);

    CHECK(PAPI_add_event(EventSet, PAPI_BR_MSP) == PAPI_OK);
    CHECK(PAPI_add_event(EventSet, PAPI_BR_PRC) == PAPI_OK);*/

    CHECK(PAPI_start(EventSet) == PAPI_OK);
    CHECK(PAPI_read(EventSet, values) == PAPI_OK);
    do_work(); 
    CHECK(PAPI_stop(EventSet, values1) == PAPI_OK);
    CHECK(PAPI_cleanup_eventset(EventSet) == PAPI_OK);
    CHECK(PAPI_destroy_eventset(&EventSet) == PAPI_OK);
    PAPI_shutdown();

    for(int i = 0;i < 10; ++i)
        printf("%llu ", values[i]);
    printf("\n");
    for(int i = 0;i < 10; ++i)
        printf("%llu ", values1[i]);
    printf("\n");

    return 0;
}

