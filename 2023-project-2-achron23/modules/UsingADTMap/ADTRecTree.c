///////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT RecTree μέσω ADT Map
//
///////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTRecTree.h"
#include "ADTMap.h"

Map values_map ;
Map left_map;
Map right_map;
Map size;

Pointer tree = 0;

int compare_pointers(Pointer a , Pointer b){
    return a - b;
}


// Δημιουργεί και επιστρέφει ένα νέο δέντρο, με τιμή (στη ρίζα) value και υποδέντρα left και right.

RecTree rectree_create(Pointer value, RecTree left, RecTree right) {
    // if (tree == 0){
    //     values_map = map_create(compare_pointers,free,free);
    //     left_map = map_create(compare_pointers,free,free);
    //     right_map = map_create(compare_pointers,free,free);
    // }
    //tree++;
    if (tree  == 0 ){
		values_map = map_create(compare_pointers,NULL,NULL);
        left_map = map_create(compare_pointers,NULL,NULL);
        right_map = map_create(compare_pointers,NULL,NULL);
		size = map_create(compare_pointers,NULL,NULL);
	}
    tree++;
    if (value != NULL){
        map_insert(values_map,tree,value);
        long a = 1 + rectree_size(left) + rectree_size(right);
        map_insert(size,tree,(Pointer)a);
    }
    else{ 
        map_insert(size,tree,(Pointer)0);
    }

	if (left != REC_TREE_EMPTY ){
        map_insert(left_map,tree,left);
	}	

	if (right != REC_TREE_EMPTY){
        map_insert(right_map,tree,right);
	}
    
    return tree;
    
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το δέντρο.
 
int rectree_size(RecTree tree) {
    if (tree != REC_TREE_EMPTY){
        Pointer p = (Pointer)0;
        p = map_find(size,tree);
        int size = p - (Pointer)0;
        return size;
    }else
        return 0;
    

}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το δέντρο tree.

void rectree_destroy(RecTree tree) {
    if(tree != REC_TREE_EMPTY){
        map_remove(size,tree);
        map_remove(values_map,tree);
        map_remove(right_map,tree);
        map_remove(values_map,tree); 
        //free(tree);
        if (map_size(size) == 0){
            map_destroy(values_map);
            map_destroy(size);
            map_destroy(right_map);
            map_destroy(left_map);
        }
    }
	
}

// Επιστρέφουν την τιμή (στη ρίζα), το αριστερό και το δεξί υποδέντρο του δέντρου tree.

Pointer rectree_value(RecTree tree) {	
	return map_find(values_map,tree);
}

RecTree rectree_left(RecTree tree) {
    // MapNode left= map_next(map,map_find_node(map,tree));
    // tree = map_node_value(map,left);  
	return map_find(left_map,tree);
}

RecTree rectree_right(RecTree tree) {
    // MapNode left= map_next(map,map_find_node(map,tree));
    // MapNode right = map_next(map,left);
    // tree = map_node_value(map,right);  
	return map_find(right_map,tree);
}

