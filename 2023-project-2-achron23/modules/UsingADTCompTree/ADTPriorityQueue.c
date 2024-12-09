#include "ADTCompTree.h"
#include "ADTPriorityQueue.h"
#include "ADTVector.h"
#include <stdlib.h>


struct priority_queue{
    CompTree tree;
    CompareFunc compare;
    DestroyFunc destroy_value;
};


static Pointer node_value(PriorityQueue pqueue, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return comptree_value(comptree_get_subtree(pqueue->tree, node_id - 1));
}

// Ανταλλάσει τις τιμές των κόμβων node_id1 και node_id2

static void node_swap(PriorityQueue pqueue, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	
    CompTree value1 = comptree_get_subtree(pqueue->tree,node_id1-1);
    CompTree value2 = comptree_get_subtree(pqueue->tree,node_id2 -1 );

    CompTree subtree = comptree_create(comptree_value(value2),comptree_left(value1),comptree_right(value1)); 
    CompTree flag = comptree_create(comptree_value(value1),comptree_left(value2),comptree_right(value2)); 


    subtree = comptree_replace_subtree(subtree,1,flag);
    

    pqueue->tree = comptree_replace_subtree(pqueue->tree,node_id1 -1,subtree );
   

}

static void bubble_up(PriorityQueue pqueue, int node_id) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
	if (node_id == 1)
		return;

	int parent = node_id / 2;		// Ο πατέρας του κόμβου. Τα node_ids είναι 1-based

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if (pqueue->compare(node_value(pqueue, parent), node_value(pqueue, node_id)) < 0) {
		node_swap(pqueue, parent, node_id);
		bubble_up(pqueue, parent);
	}
}

static void bubble_down(PriorityQueue pqueue, int node_id) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	int left_child = 2 * node_id;
	int right_child = left_child + 1;

	int size = pqueue_size(pqueue);
	if (left_child > size)
		return;

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	int max_child = left_child;
	if (right_child <= size && pqueue->compare(node_value(pqueue, left_child), node_value(pqueue, right_child)) < 0)
			max_child = right_child;

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if (pqueue->compare(node_value(pqueue, node_id), node_value(pqueue, max_child)) < 0) {
		node_swap(pqueue, node_id, max_child);
		bubble_down(pqueue, max_child);
	}
}



static void efficient_heapify(PriorityQueue pqueue,Vector values){
    CompTree array[vector_size(values)] ;

    for (int i = vector_size(values)/2; i < vector_size(values); i++){
        array[i] = comptree_create(vector_get_at(values,i),COMP_TREE_EMPTY,COMP_TREE_EMPTY); 
    }
    for (int i = vector_size(values)/2-1 ; i >= 0; i -- ){
        if ( 2*i+2 < vector_size(values))
            array[i] = comptree_create(vector_get_at(values,i),array[2*i+1],array[2*i+2]);
        else 
            array[i] = comptree_create(vector_get_at(values,i),array[2*i+1],COMP_TREE_EMPTY);

    }
        pqueue->tree = array[0];
        for(int i = vector_size(values)/2-1; i >= 0; i--) {
            bubble_down(pqueue, i+1); // Note: bubble_down needs 1-based index.
        }

}

// Δημιουργεί και επιστρέφει μια νέα ουρά προτεραιότητας, της οποίας τα στοιχεία συγκρίνονται με βάση τη συνάρτηση compare.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.
// Αν values != NULL, τότε η ουρά αρχικοποιείται με τα στοιχεία του Vector values.

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values){
    PriorityQueue pqueue = malloc(sizeof(*pqueue));
    pqueue->compare = compare;
    pqueue->destroy_value = destroy_value;
    pqueue->tree = comptree_create(COMP_TREE_EMPTY,COMP_TREE_EMPTY,COMP_TREE_EMPTY);

    if (values != NULL)
       efficient_heapify(pqueue,values);
     
    return pqueue;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά pqueue

int pqueue_size(PriorityQueue pqueue){
    return comptree_size(pqueue->tree);

}

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer pqueue_max(PriorityQueue pqueue){
    // Pointer value = comptree_value(comptree_get_subtree(pqueue->tree,0));
    // return value;
    return node_value(pqueue,1);

}

// Προσθέτει την τιμή value στην ουρά pqueue.

void pqueue_insert(PriorityQueue pqueue, Pointer value){

    comptree_insert_last(pqueue->tree,value);

    bubble_up(pqueue,pqueue_size(pqueue));




}

// Αφαιρεί την μεγαλύτερη τιμή της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

void pqueue_remove_max(PriorityQueue pqueue){
    if (pqueue->destroy_value != NULL)
        pqueue->destroy_value(pqueue_max(pqueue));
    //node_swap(pqueue,1,pqueue_size(pqueue));
    //comptree_remove_last(pqueue->tree);
    bubble_down(pqueue,1);
    comptree_remove_last(pqueue->tree);




}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value){
    DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά pqueue.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void pqueue_destroy(PriorityQueue pqueue){
    comptree_destroy(pqueue->tree);
    free(pqueue);
}


