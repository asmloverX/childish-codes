#include <windows.h>
#include "cl_os.h"
#include "cl_test.h"
#include "cl_list.h"


static inline void* _list_node_value_new(void* _x) {
    int* p = __cl_object_new(_x, sizeof(int));
    return (void*)p;
}

static inline void _list_node_value_free(void* _x) {
    __cl_object_free(_x, sizeof(int));
}

static inline void _list_node_value_show(void* _x) {
    printf("this list node vlaue is : %d\n", *(int*)_x);
}

static void _list_test(void) {
    int i = 0;
    struct cl_list* cl = NULL;
    cl_list_iter it = NULL;
    SYSTEMTIME st = {0};
    struct cl_object_func cof = {_list_node_value_new, _list_node_value_free};

    cl = cl_list_new(cof);
    printf("now the list length is : %ld, empty : %s\n", cl_list_size(cl), cl_list_empty(cl) ? "yes" : "no");
    GetLocalTime(&st);
    printf("%d:%.2d:%.3d\n", st.wMinute, st.wSecond, st.wMilliseconds);
    for (i = 0; i < 10; i++) {
        int v = i + 1;
        cl_list_push_back(cl, &v);
    }
    GetLocalTime(&st);
    printf("%d:%.2d:%.3d\n", st.wMinute, st.wSecond, st.wMilliseconds);
    printf("now the list length is : %ld, empty : %s\n", cl_list_size(cl), cl_list_empty(cl) ? "yes" : "no");

    it = cl_list_begin(cl);
    for (; it != cl_list_end(cl); it = cl_list_next(it)) {
        printf("%d\t", *(int*)*it);
    }
    printf("\n");
    cl_list_for_each(cl, _list_node_value_show);

    cl_list_clear(cl);
    printf("now the list length is : %ld, empty : %s\n", cl_list_size(cl), cl_list_empty(cl) ? "yes" : "no");

    cl_list_free(&cl);
}


int 
main(int argc, char* argv[], char* envp[]) {
    int sel = 0;

    /*while (1) {
        printf("*************************************************\n");
        printf("    0 - quit ...\n");
        printf("    1 - the test of debug print function ...\n");
        printf("    2 - the test of threads-list functions ...\n");
        printf("*************************************************\n");

        scanf("%d", &sel);
        if (0 == sel)
            break;
        switch (sel) {
        case 1:
            cl_test_dbg();
            break;
        case 2:
            break;
        default:
            printf("please enter another right selections ...\n\n\n");
        }
        printf("\n\n");
    }*/

    _list_test();

    return 0;
}