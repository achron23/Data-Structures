#include "set_utils.h"
#include "state.h"
#include <stdlib.h>


Pointer set_find_eq_or_greater(Set set,Pointer value){
    Pointer a = set_find(set,value);
    if (a == NULL){
        set_insert(set,value);
        SetNode b = set_find_node(set, value);
        b = set_next(set,b);
        if( b != SET_EOF)
            a = set_node_value(set,b);
        set_remove(set,value);
    }
        return a;
    
}




Pointer set_find_eq_or_smaller(Set set, Pointer value){
    Pointer a = set_find(set,value);
    if (a == NULL){
        set_insert(set,value);
        SetNode b = set_find_node(set, value);
        b = set_previous(set,b);
        if ( b != SET_BOF )
            a = set_node_value(set,b);
        set_remove(set,value);
    }
        return a;
    
}
