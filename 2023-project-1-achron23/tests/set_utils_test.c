//////////////////////////////////////////////////////////////////
//
// Test για το set_utils.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "ADTSet.h"

#include "set_utils.h"
#include <stdio.h>

int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

int compare_ints(Pointer a, Pointer b){
    return *(int*)a -*(int*)b;
}


void test_set_find_eq_or_greater() {
    Set set = set_create(compare_ints,free);
    for (int i = 0;i < 20; i+=2){
        set_insert(set,create_int(i));
    }
    Pointer flag = set_find_eq_or_greater(set,create_int(8));
    TEST_ASSERT(flag != NULL);
    TEST_ASSERT(*(int*)flag == 8 );

    Pointer flag1 = set_find_eq_or_greater(set,create_int(9));
    TEST_ASSERT(flag1 != NULL);
    TEST_ASSERT(*(int*)flag1 == 10 );

    Pointer flag2 = set_find_eq_or_greater(set,create_int(20));
    TEST_ASSERT(flag2 == NULL);

    Pointer flag3 = set_find_eq_or_greater(set,create_int(-1));
    TEST_ASSERT(flag3 != NULL);

}

void test_set_find_eq_or_smaller(){
    Set set = set_create(compare_ints,free);
    for (int i = 0;i < 20; i+=2){
        set_insert(set,create_int(i));
    }
    Pointer flag = set_find_eq_or_smaller(set,create_int(8));
    TEST_ASSERT(flag != NULL);
    TEST_ASSERT(*(int*)flag == 8 );

    Pointer flag1 = set_find_eq_or_smaller(set,create_int(9));
    TEST_ASSERT(flag1 != NULL);
    TEST_ASSERT(*(int*)flag1 == 8 );

    Pointer flag2 = set_find_eq_or_smaller(set,create_int(-1));
    TEST_ASSERT(flag2 == NULL);

}


TEST_LIST = {
	{ "test_set_find_eq_or_greater", test_set_find_eq_or_greater },
	{ "test_set_find_eq_or_smaller", test_set_find_eq_or_smaller },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};