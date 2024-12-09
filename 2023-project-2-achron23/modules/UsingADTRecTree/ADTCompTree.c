#include "ADTCompTree.h"
#include "ADTRecTree.h"
#include "ADTRecTree_utils.h"
#include <math.h>
#include <stdlib.h>

// struct comptree{
//     Pointer value;
//     CompTree right;
//     CompTree left;
//     int size;
// };

// Δημιουργεί και επιστρέφει ένα νέο δέντρο, με τιμή (στη ρίζα) value και υποδέντρα left και right.

CompTree comptree_create(Pointer value, CompTree left1, CompTree right2){
    RecTree left = (RecTree)left1;
    RecTree right = (RecTree)right2;
    CompTree tree = (CompTree)rectree_create(value,left,right);
    return tree;

}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το δέντρο (0 για κενά δέντρα).

 int comptree_size(CompTree tree){
    if (tree == COMP_TREE_EMPTY)
        return 0;

    return rectree_size((RecTree)tree);
    
 }

// // Ελευθερώνει τη μνήμη που δεσμεύει το δέντρο tree (ΔΕΝ καταστρέφει τα υποδέντρα του).

void comptree_destroy(CompTree tree){
    free (tree);
    return;
}

// // Επιστρέφουν την τιμή (στη ρίζα), το αριστερό και το δεξί υποδέντρο του δέντρου tree.

 Pointer comptree_value(CompTree tree){
    return rectree_value((RecTree)tree);
 }
 CompTree comptree_left(CompTree tree){
    return (CompTree)rectree_left((RecTree)tree);

 }
 CompTree comptree_right(CompTree tree){
    return (CompTree)rectree_right((RecTree)tree);

 }


CompTree comptree_get_subtree(CompTree tree,int pos){
    tree = (CompTree)rectree_get_subtree((RecTree)tree,pos);
    return tree;
}

CompTree comptree_replace_subtree(CompTree tree,int pos,CompTree subtree){
    tree = (CompTree)rectree_replace_subtree((RecTree)tree,pos,(RecTree)subtree);
    return tree;
}



// // Δημιουργεί και επιστρέφει ένα νέο δέντρο που προκύπτει από το tree μετά την προσθήκη της
// // τιμής value στο "τέλος" του δέντρου (ώστε να παραμείνει complete). Τυχόν υποδέντρα που
// // "μεταβάλλονται" κατά την προσθήκη αυτή καταστρέφονται αυτόματα.

CompTree comptree_insert_last(CompTree tree, Pointer value){

    
    CompTree new_tree = comptree_create(value,COMP_TREE_EMPTY,COMP_TREE_EMPTY);

    return comptree_replace_subtree(tree,comptree_size(tree),new_tree);
    
 
}




// Δημιουργεί και επιστρέφει ένα νέο δέντρο που προκύπτει από το tree μετά την διαγραφή του
// "τελευταίου" υποδέντρου του (ώστε να παραμείνει complete). Το υποδέντρο που αφαιρείται, και
// τυχόν υποδέντρα που "μεταβάλλονται" κατά τη διαγραφή αυτή καταστρέφονται αυτόματα.

CompTree comptree_remove_last(CompTree tree){
    int size = comptree_size(tree);
    free (comptree_get_subtree(tree,size-1));
    return comptree_replace_subtree(tree,comptree_size(tree)-1,COMP_TREE_EMPTY);
    

}