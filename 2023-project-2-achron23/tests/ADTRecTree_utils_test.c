#include <acutest.h>
#include "ADTRecTree_utils.h"

int* create_int(int value){
    int* p = malloc(sizeof(int));
    *p = value;
    return p;

}

void test_get_subtree(void){
    RecTree left_left = rectree_create(create_int(6),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree left_right = rectree_create(create_int(5),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right_left = rectree_create(create_int(4),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right_right = rectree_create(create_int(3),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right = rectree_create(create_int(2),right_left,right_right);
    RecTree left = rectree_create(create_int(1),left_left,left_right);
    RecTree root = rectree_create(create_int(0),left,right);
    RecTree foo = rectree_get_subtree(root,2);
   // TEST_ASSERT(rectree_value(foo) == rectree_value(right));
    TEST_ASSERT(right == foo);
    foo = rectree_get_subtree(root,1);
    TEST_ASSERT(rectree_value(foo) == rectree_value(left));
    TEST_ASSERT(left == foo);
    foo = rectree_get_subtree(root,5);
   // TEST_ASSERT(foo == NULL);
 //   TEST_ASSERT(rectree_size(foo)==1);
    TEST_ASSERT(right_left == foo);
    foo = rectree_get_subtree(root,6);
    TEST_ASSERT(right_right == foo);
}



void test_replace_subtree(void){
    RecTree left_left = rectree_create(create_int(6),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree left_right = rectree_create(create_int(5),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right_left = rectree_create(create_int(4),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right_right = rectree_create(create_int(3),REC_TREE_EMPTY,REC_TREE_EMPTY);
    RecTree right = rectree_create(create_int(2),right_left,right_right);
    RecTree left = rectree_create(create_int(1),left_left,left_right);
    RecTree root = rectree_create(create_int(0),left,right);
    RecTree foo = rectree_replace_subtree(root,1,right);
    TEST_ASSERT (rectree_left(foo)==right);
    RecTree foo1 = rectree_replace_subtree(root,5,left_right);
    TEST_ASSERT (rectree_get_subtree(foo1,5)==left_right);
    RecTree foo2 = rectree_replace_subtree(root,6,left_left);
    TEST_ASSERT (rectree_get_subtree(foo2,6) == left_left);
    RecTree foo3 = rectree_replace_subtree(root,3,right_left);
    TEST_ASSERT (rectree_get_subtree(foo3,3) == right_left);
}


TEST_LIST = {
	{ "get_subtree", test_get_subtree },
    { "replace subtree",test_replace_subtree},
	//{ "RecTree_insert", test_insert },
	//{ "RecTree_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
