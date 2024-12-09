///////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT RecTree μέσω links
//
///////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTRecTree.h"


// Προς υλοποίηση...


//  Ένα δέντρο είναι pointer σε αυτό το struct
struct rec_tree {
	Pointer value;
	RecTree right;
	RecTree left;
	int size ;

};


// Δημιουργεί και επιστρέφει ένα νέο δέντρο, με τιμή (στη ρίζα) value και υποδέντρα left και right.

RecTree rectree_create(Pointer value, RecTree left, RecTree right) {
	RecTree tree = malloc(sizeof(*tree));
	if (value != NULL){
		tree->value = value;
		tree->size = 1;
	}
	if (left != REC_TREE_EMPTY ){
		tree->size += left->size;
		
	}	
	if (right != REC_TREE_EMPTY){
		tree->size += right->size;			
		
	}
	tree->left = left;
	tree->right = right;
	return tree;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το δέντρο.

int rectree_size(RecTree tree) {
	if (tree == REC_TREE_EMPTY )
		return 0;
	else	
		return tree->size;

}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το δέντρο tree.

void rectree_destroy(RecTree tree) {
	free(tree);
	
}

// Επιστρέφουν την τιμή (στη ρίζα), το αριστερό και το δεξί υποδέντρο του δέντρου tree.

Pointer rectree_value(RecTree tree) {	
	return tree->value;
}

RecTree rectree_left(RecTree tree) {

	return tree->left;
}

RecTree rectree_right(RecTree tree) {
	return tree->right;
}

