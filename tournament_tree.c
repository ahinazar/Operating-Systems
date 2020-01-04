//
// Created by ahinazar@wincs.cs.bgu.ac.il on 4/30/19.
//

#include "types.h"
#include "user.h"
#include "kthread.h"
#include "tournament_tree.h"

trnmnt_tree* trnmnt_tree_alloc(int depth){
    trnmnt_tree *tree = malloc(sizeof(trnmnt_tree));
    int *treeArr = (int*)malloc((1<<depth) * sizeof(int));
    int *used_ids = (int*)malloc((1<<depth)*sizeof(int));
    tree->tree = treeArr;
    tree->depth = depth;
    tree->used_ids = used_ids;
    for(int i=0; i<(1<<depth) ; i++){
        tree->used_ids[i] = 0;
        int mutex_id = kthread_mutex_alloc();
        if(mutex_id == -1){
            trnmnt_tree_dealloc(tree);
            return 0;
        }else
            tree->tree[i] = (int)mutex_id;
    }
    return tree;
}

int trnmnt_tree_dealloc(trnmnt_tree* tree){

    if(!tree)
        return -1;

    for(int i=0; i< (1<<(tree->depth)); i++) {
        if(tree->used_ids[i])
            return -1;
    }

    for(int i=0; i<(1<<(tree->depth)); i++){
        if(kthread_mutex_dealloc(tree->tree[i]) == -1)
            return -1;
    }
    free(tree->tree);
    free(tree->used_ids);
    free(tree);
    return 0;
}

int trnmnt_tree_acquire(trnmnt_tree* tree,int ID) {
    int i = (1<<(tree->depth)) + ID;
    int j = i/2;
    if(tree->used_ids[ID] == 1)
        return -1;
    else tree->used_ids[ID] = 1;
    while (j>=1){
        if(kthread_mutex_lock(tree->tree[j])==-1) {
            return -1;
        }
        else {
            j = j / 2;
        }
    }
    return 0;
}

int trnmnt_tree_release(trnmnt_tree* tree,int ID){
    int i = (1<<(tree->depth)) + ID;
    int j = i/2;
    int locks_local[tree->depth];
    int l=0;
    while (j>=1){
        locks_local[l++]=tree->tree[j];
        j = j/2;
    }
    for(l = tree->depth-1;l>=0;l-- ){
        if(kthread_mutex_unlock(locks_local[l])==-1)
            return -1;
    }
    tree->used_ids[ID] = 0;
    return 0;
}