#include <acutest.h>
#include "ADTCompTree.h"
#include "ADTMap.h"



int* create_int(int value){
    int* p = malloc(sizeof(int));
    *p = value;
    return p;

}

void test_create(void){
    //TEST_ASSERT(REC_TREE_EMPTY);
    CompTree lefttree = comptree_create(create_int(3),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree righttree = comptree_create(create_int(1),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree tree = comptree_create(create_int(5),lefttree,righttree);
    TEST_ASSERT(comptree_size(tree) == 3);
    TEST_ASSERT(comptree_size(lefttree)==1);
    TEST_ASSERT(comptree_size(righttree)==1);
    TEST_ASSERT(comptree_value(comptree_left(tree))== comptree_value(lefttree));
    TEST_ASSERT(comptree_value(comptree_right(tree))== comptree_value(righttree));
    CompTree emptytree = comptree_create(NULL,COMP_TREE_EMPTY,COMP_TREE_EMPTY) ;
    TEST_ASSERT(!comptree_size(emptytree));
    comptree_destroy(lefttree);
    comptree_destroy(righttree);
    comptree_destroy(tree);
    //TEST_ASSERT(comptree_size(tree) == 0);
    comptree_destroy(emptytree);




}


void test_insert(void){
    CompTree left_left = comptree_create(create_int(6),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree left_right = comptree_create(create_int(5),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right_left = comptree_create(create_int(4),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right_right = comptree_create(create_int(3),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right = comptree_create(create_int(2),right_left,right_right);
    CompTree left = comptree_create(create_int(1),left_left,left_right);
    CompTree root = comptree_create(create_int(0),left,right);
    Pointer value = create_int(10);
    root = comptree_insert_last(root,value);
    TEST_ASSERT(comptree_size(root)==8);
    TEST_ASSERT(comptree_value(comptree_get_subtree(root,7)) == value  );
}

void test_remove(void){
    CompTree left_left = comptree_create(create_int(6),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree left_right = comptree_create(create_int(5),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right_left = comptree_create(create_int(4),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right_right = comptree_create(create_int(3),COMP_TREE_EMPTY,COMP_TREE_EMPTY);
    CompTree right = comptree_create(create_int(2),right_left,right_right);
    CompTree left = comptree_create(create_int(1),left_left,left_right);
    CompTree root = comptree_create(create_int(0),left,right);
    Pointer value = create_int(10);
    root = comptree_insert_last(root,value);
    TEST_ASSERT(comptree_size(root)==8);
    root = comptree_remove_last(root);
    TEST_ASSERT(comptree_size(root)==7);


}



// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "CompTree_create", test_create },
	{ "CompTree_insert", test_insert },
	{ "CompTree_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};