#include <acutest.h>
#include "ADTRecTree.h"
#include "ADTMap.h"



int* create_int(int value){
    int* p = malloc(sizeof(int));
    *p = value;
    return p;

}

void test_create(void){
    //TEST_ASSERT(REC_TREE_EMPTY);
    RecTree lefttree = rectree_create(create_int(3),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree righttree = rectree_create(create_int(1),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree tree = rectree_create(create_int(5),lefttree,righttree);
    TEST_ASSERT(rectree_size(tree) == 3);
    TEST_ASSERT(rectree_size(lefttree)==1);
    TEST_ASSERT(rectree_size(righttree)==1);
    TEST_ASSERT(rectree_value(rectree_left(tree))== rectree_value(lefttree));
    TEST_ASSERT(rectree_value(rectree_right(tree))== rectree_value(righttree));
    RecTree emptytree = rectree_create(NULL,REC_TREE_EMPTY,REC_TREE_EMPTY) ;
    TEST_ASSERT(!rectree_size(emptytree));
    rectree_destroy(lefttree);
    rectree_destroy(righttree);
    rectree_destroy(tree);
   // TEST_ASSERT(rectree_size(tree) == 0);
    rectree_destroy(emptytree);




}


void test_insert(void){

}

void test_remove(void){
    // RecTree lefttree = rectree_create(create_int(3),NULL,NULL);
    // RecTree righttree = rectree_create(create_int(1),NULL,NULL);
    // RecTree tree = rectree_create(create_int(5),lefttree,righttree);
    // rectree_destroy(rectree_left(tree));
    // TEST_ASSERT(rectree_size(tree) == 2);
    // rectree_destroy(rectree_right(tree));
    // TEST_ASSERT(rectree_size(tree) == 1);

 


}



// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "RecTree_create", test_create },
	{ "RecTree_insert", test_insert },
	{ "RecTree_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
