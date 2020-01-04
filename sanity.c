#include "types.h"
#include "user.h"
#include "kthread.h"

volatile int hold;
int result;
int tids[8];
int mutex_id;

void tStart0_task_2_2(){
    tids[0] = kthread_id();
    kthread_exit();
}

void tStart1_task_2_2(){
    tids[1] = kthread_id();
    kthread_exit();
}

void tStart2_task_2_2(){
    tids[2] = kthread_id();
    kthread_exit();
}

void tStart3_task_2_2(){
    tids[3] = kthread_id();
    kthread_exit();
}

void tStart4_task_2_2(){
    tids[4] = kthread_id();
    kthread_exit();
}

void tStart5_task_2_2(){
    tids[5] = kthread_id();
    kthread_exit();
}

void tStart6_task_2_2(){
    tids[6] = kthread_id();
    kthread_exit();
}

void tStart7_task_2_2(){
    tids[7] = kthread_id();
    kthread_exit();
}

void (*threads_starts[])(void) = {tStart0_task_2_2,tStart1_task_2_2,tStart2_task_2_2, tStart3_task_2_2, tStart4_task_2_2, tStart5_task_2_2, tStart6_task_2_2, tStart7_task_2_2};


void test_2_2(){
    void *tStack0 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack1 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack2 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack3 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack4 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack5 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack6 = ((char*)malloc(500*sizeof(char)))+500;
    void *tStack7 = ((char*)malloc(500*sizeof(char)))+500;

    void (*threads_stacks[])(void) ={tStack0, tStack1, tStack2, tStack3, tStack4, tStack5, tStack6, tStack7};

    int flag = 0;

    for(int i = 0;i < 8;i++){
        flag = kthread_create(threads_starts[i], threads_stacks[i]);
        if(flag <= 0)
            printf(1,"Error -> thread %d created unsuccessfully\n",i);
    }

    sleep(500);

    for(int i = 0;i < 8;i++){
        int flag = kthread_join(tids[i]);

        if(flag == -1)
            printf(1,"Error -> joining thread %d was unsuccessfully\n",i);
        else if(flag != 0)
            printf(1,"Error -> unknown result code -> join\n");
    }

    for(int i = 0;i < 8;i++){

        int result = kthread_join(tids[i]);

        if(result == 0)
            printf(1,"Error -> thread %d has joined well while didn't need to -> unsuccessful act\n",i);

        else if(result != -1)
            printf(1,"Error -> unknown result code -> join -> unsuccessful act\n");
    }
}

void tStart0_task_3_1(){
    while(hold){}
    sleep(40);

    int result = kthread_mutex_unlock(mutex_id);
    if(result == 0)
        printf(1,"Error -> mutex unlocked successfully where it should not have been -> thread 0\n");

    result = kthread_mutex_lock(mutex_id);
    if(result < 0)
        printf(1,"Error -> mutex locked unsuccessfully -> thread 0\n");

    sleep(400);

    result = kthread_mutex_unlock(mutex_id);
    if(result < 0){
        printf(1,"Error -> mutex unlocked unsuccessfully -> thread 0\n");
    }
    kthread_exit();
}

void tStart1_task_3_1(){
    while(hold){}
    sleep(200);

    int result = kthread_mutex_lock(mutex_id + 5);
    if(result == 0)
        printf(1,"Error -> mutex locked successfully where it shouldn't have been -> thread 1\n");

    result = kthread_mutex_unlock(mutex_id);
    if(result == 0)
        printf(1,"Error -> mutex unlocked successfully where it shouldn't have been -> thread 1\n");

    result = kthread_mutex_unlock(mutex_id + 10);
    if(result == 0)
        printf(1,"Error -> mutex unlocked successfully where it shouldn't have been -> thread 1\n");
    
    kthread_exit();
}

void (*tStarts[])(void) =
        {tStart0_task_3_1,
         tStart1_task_3_1};

void test_3_1(){
    hold = 1;

    int tids[2];

    void * tStack0 = ((char*)malloc(500*sizeof(char)))+500;
    void * tStack1 = ((char*)malloc(500*sizeof(char)))+500;

    void (*tStacks[])(void) ={tStack0, tStack1};

    mutex_id = kthread_mutex_alloc();
    if(mutex_id == -1)
        printf(1,"Error -> mutex allocated unsuccessfully -> test 3.1\n");

    for(int i = 0;i < 2;i++)
        tids[i] = kthread_create(tStarts[i], tStacks[i]);

    hold = 0;

    for(int i = 0;i < 2;i++){
        int result = kthread_join(tids[i]);
        if(result == -1)
            printf(1,"Error -> joining thread num %d\n",i);
        else if(result!=0)
            printf(1,"Error -> unknown result code from join\n");
    }

    result = kthread_mutex_dealloc(mutex_id);
    if(result == -1)
        printf(1,"Error -> mutex deallocated unsuccessfully\n");
}


void test_3_2(){
    int result;
    trnmnt_tree* tree;
    tree = trnmnt_tree_alloc(1);

    if(tree == 0)
        printf(1,"Error -> 1 trnmnt_tree allocated unsuccessfully\n");

    result = trnmnt_tree_acquire(tree, 0);
    if(result < 0)
        printf(1,"Error -> 1 trnmnt_tree acquire unsuccessfully\n");

    result = trnmnt_tree_release(tree, 0);
    if(result < 0)
        printf(1,"Error -> 1 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_acquire(tree, 1);
    if(result < 0)
        printf(1,"Error -> 2 trnmnt_tree acquire unsuccessfully\n");

    result = trnmnt_tree_release(tree, 1);
    if(result < 0)
        printf(1,"Error -> 2 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_dealloc(tree);
    if(result == -1)
        printf(1,"Error -> 1 trnmnt_tree deallocated unsuccessfully\n");
    else if(result!=0)
        printf(1,"Error -> 1 unknown return code from trnmnt_tree_dealloc\n");



    tree = trnmnt_tree_alloc(2);
    if(tree == 0)
        printf(1,"Error -> 2 trnmnt_tree allocated unsuccessfully\n");


    result = trnmnt_tree_acquire(tree, 0);
    if(result < 0)
        printf(1,"Error -> 3 trnmnt_tree acquire unsuccessfully\n");


    result = trnmnt_tree_release(tree, 0);
    if(result < 0)
        printf(1,"Error -> 3 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_acquire(tree, 1);
    if(result < 0)
        printf(1,"Error-> 4 trnmnt_tree acquire unsuccessfully\n");

    result = trnmnt_tree_release(tree, 1);
    if(result < 0)
        printf(1,"Error -> 4 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_acquire(tree, 2);
    if(result < 0)
        printf(1,"Error -> 5 trnmnt_tree acquire unsuccessfully\n");

    result = trnmnt_tree_release(tree, 2);
    if(result < 0)
        printf(1,"Error -> 5 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_acquire(tree, 3);
    if(result < 0)
        printf(1,"Error -> 6 trnmnt_tree acquire unsuccessfully\n");

    result = trnmnt_tree_release(tree, 3);
    if(result < 0)
        printf(1,"Error -> 6 trnmnt_tree release unsuccessfully\n");

    result = trnmnt_tree_dealloc(tree);
    if(result == -1)
        printf(1,"Error -> 12 trnmnt_tree deallocated unsuccessfully\n");
    else if(result != 0)
        printf(1,"Error -> 2 unkown return code from trnmnt_tree_dealloc\n");

}

int main(int argc, char *argv[]){

    //*******************************************

    test_2_2();

    //*******************************************

    test_3_1();


    //*******************************************

    test_3_2();

    //*******************************************

    printf(1,"if nothing printed so far -> all passed\n");

    exit();
}
