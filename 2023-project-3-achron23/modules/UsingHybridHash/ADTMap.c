/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω υβριδικού Hash Table
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTVector.h"

#include "ADTMap.h"

// Κάθε θέση i θεωρείται γεινοτική με όλες τις θέσεις μέχρι και την i + NEIGHBOURS
#define NEIGHBOURS 3

// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.5

typedef enum {
	EMPTY,OCCUPIED
} State;

// Το μέγεθος του Hash Table ιδανικά θέλουμε να είναι πρώτος αριθμός σύμφωνα με την θεωρία.
// Η παρακάτω λίστα περιέχει πρώτους οι οποίοι έχουν αποδεδιγμένα καλή συμπεριφορά ως μεγέθη.
// Κάθε re-hash θα γίνεται βάσει αυτής της λίστας. Αν χρειάζονται παραπάνω απο 1610612741 στοχεία, τότε σε καθε rehash διπλασιάζουμε το μέγεθος.
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

typedef struct array_node *ArNode;



struct pair {
	Pointer key;
	Pointer value;
};

struct map_node {
    struct pair node;
    State state;
    Vector chain;   // περιέχει MapNodes
};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	MapNode array;
	int size;
	int capacity;
	HashFunc hash_function;
	CompareFunc compare;
	DestroyFunc destroy_key;	
	DestroyFunc destroy_value;

};

	// static void rehash(Map map) {
	// 	// Αποθήκευση των παλιών δεδομένων
	// 	int old_capacity = map->capacity;
	// 	MapNode old_array = map->array;

	// 	// Βρίσκουμε τη νέα χωρητικότητα, διασχίζοντας τη λίστα των πρώτων ώστε να βρούμε τον επόμενο. 
	// 	int prime_no = sizeof(prime_sizes) / sizeof(int);	// το μέγεθος του πίνακα
	// 	for (int i = 0; i < prime_no; i++) {					// LCOV_EXCL_LINE
	// 		if (prime_sizes[i] > old_capacity) {
	// 			map->capacity = prime_sizes[i]; 
	// 			break;
	// 		}
	// 	}
	// 	// Αν έχουμε εξαντλήσει όλους τους πρώτους, διπλασιάζουμε
	// 	if (map->capacity == old_capacity)					// LCOV_EXCL_LINE
	// 		map->capacity *= 2;								// LCOV_EXCL_LINE

	// 	// Δημιουργούμε ένα μεγαλύτερο hash table
	// 	map->array = malloc(map->capacity * sizeof(struct map_node));
	// 	for (int i = 0; i < map->capacity; i++)
	// 		map->array[i].state = EMPTY;

	// 	// Τοποθετούμε ΜΟΝΟ τα entries που όντως περιέχουν ένα στοιχείο (το rehash είναι και μία ευκαιρία να ξεφορτωθούμε τα deleted nodes)
	// 	map->size = 0;
	// 	for (int i = 0; i < old_capacity; i++)
	// 		if (old_array[i].state == OCCUPIED)
	// 			map_insert(map, old_array[i].key, old_array[i].value);

	// 	//Αποδεσμεύουμε τον παλιό πίνακα ώστε να μήν έχουμε leaks
	// 	free(old_array);
	// }





Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	Map map = malloc(sizeof(*map));
	//map->size = map_size(map);
	map->capacity = prime_sizes[0];
	map->array = malloc(map->capacity * sizeof(struct map_node));

	// Αρχικοποιούμε τους κόμβους που έχουμε σαν διαθέσιμους.
	for (int i = 0; i < map->capacity; i++){
		map->array[i].state = EMPTY;
		map->array[i].chain = vector_create(0,NULL);
	}	

	map->size = 0;
	//map->deleted = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}


// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;										
}

MapNode search_in_vector(Map map ,int pos,Pointer key){
	if (pos >= map->capacity)
	return NULL;

	for (int i = 0;i< vector_size(map->array[pos].chain); i++){
		//if (vector_get_at(map->array[pos].chain,i) == )
		MapNode a = NULL;
		a = vector_get_at(map->array[pos].chain,i);

	if(  a->node.key != NULL && a->node.key == key ){
			return a;
		}			
	}
	return NULL;




}

void remove_from_vector(Map map,int pos,MapNode to_remove){
	MapNode node = (MapNode)vector_find_node(map->array[pos].chain,to_remove,map->compare);
	//MapNode last = (MapNode)vector_last(map->array[pos].chain);
	Vector vec = vector_create(0,free);
	for(MapNode flag =(MapNode)vector_last(map->array[pos].chain);flag != node;flag = (MapNode)vector_previous(map->array[pos].chain,(VectorNode)flag)){
		vector_insert_last(vec,flag);
		vector_remove_last(map->array[pos].chain);
	}
	vector_remove_last(map->array[pos].chain);
	for (int i = 0;i<vector_size(vec);i++){
		vector_insert_last(map->array[pos].chain,vector_get_at(vec,i));
	}
	vector_destroy(vec);
	

}

void insert_in_vector(Vector vec,MapNode new){
	vector_insert_last(vec,(Pointer)new);
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

void map_insert(Map map, Pointer key, Pointer value) {
	bool already_in_map = false;
	bool already_in_vec = false;
	MapNode new_node = malloc(sizeof(MapNode));
	// new_node->key = key;
	// new_node->value = value;
	new_node = NULL;
	uint index = map->hash_function(key) % map->capacity;
	uint pos;
	for (pos = map->hash_function(key) % map->capacity + NEIGHBOURS; pos > index; pos--){
		if(map->array[pos].state == EMPTY){
			// map->array[pos].node.key = key;
			// map->array[pos].node.value = value;
			// map->array[pos].state = OCCUPIED;
			new_node = &map->array[pos];
		}else if(search_in_vector(map,pos,key) != NULL ){
			already_in_vec = true;
			MapNode a = search_in_vector(map,pos,key);
			new_node = a;
			break;
		}else if(map->array[pos].node.key == key){
			already_in_map = true;
			new_node = &map->array[pos];
			break;
		}
	}
	if (new_node == NULL)
		new_node = &map->array[pos];

	if (already_in_map)	{
		if (new_node->node.key != key && map->destroy_key != NULL)
			map->destroy_key(new_node->node.key);

		if (new_node->node.value != value && map->destroy_value != NULL)
			map->destroy_value(new_node->node.value);
	}else if(already_in_vec){
		remove_from_vector(map,pos,new_node);
		new_node->node.key = key;
		new_node->node.value = value;
		insert_in_vector(map->array[pos].chain,new_node);
		

	}else 
		map->size++;
	
	new_node->state = OCCUPIED;
	new_node->node.key = key;
	new_node->node.value = value;
}
	
	




int get_pos(Map map , Pointer key){
	for (int i=0;i<map->capacity;i++){
		if (map->array[i].node.key == key || search_in_vector(map,i,key) != NULL)
			return i;
	}
	return -1;
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {
	MapNode anode = map_find_node(map,key);
	int pos = get_pos(map,key);
	MapNode vnode = search_in_vector(map,pos,key);

	if (anode == MAP_EOF && vnode == NULL)
		return false;
	else if (vnode != NULL){	
		remove_from_vector(map,pos,vnode);		
	}else {
		
		if (map->destroy_key != NULL)
			map->destroy_key(anode->node.key);
		if (map->destroy_value != NULL)
			map->destroy_value(anode->node.value);
		map->array[pos].state = EMPTY;	
	}		

	map->size-- ;
	return true;
	
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

Pointer map_find(Map map, Pointer key) {
	MapNode anode = map_find_node(map,key);
	// int pos = get_pos(map,key);
	// MapNode vnode = search_in_vector(map,pos,key);
	if (anode != NULL)
		return anode->node.value;
	// else if(vnode != NULL)
	// 	return vnode->value; 
	return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	return NULL;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	return NULL;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {

}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
	for (int i = 0 ;i<map->capacity;i++){
		if (map->array[i].state == OCCUPIED)
			return &map->array[0];
	}
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	for (int i = node - map->array + 1; i < map->capacity; i++)
		if (map->array[i].state == OCCUPIED)
			return &map->array[i];

	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	return NULL;
}

Pointer map_node_value(Map map, MapNode node) {
	return NULL;
}

MapNode map_find_node(Map map, Pointer key) {
	// for(MapNode mnode = map_first(map);mnode != MAP_EOF; mnode = map_next(map,mnode)){
	// }
	int count = 0;
	uint index = map->hash_function(key) % map->capacity;
	uint pos ;
	for (pos = map->hash_function(key) % map->capacity + NEIGHBOURS; pos >= index; pos--){
		if (map->array[pos].state == OCCUPIED && map->compare(map->array[pos].node.key, key) == 0)
			return &map->array[pos];
		else if(search_in_vector(map,pos,key) != NULL){
			MapNode a = search_in_vector(map,pos,key);
			return a ;
		}	

		// Αν διασχίσουμε ολόκληρο τον πίνακα σταματάμε. Εφόσον ο πίνακας δεν μπορεί να είναι όλος OCCUPIED,
		// αυτό μπορεί να συμβεί μόνο στην ακραία περίπτωση που ο πίνακας έχει γεμίσει DELETED τιμές!
		count++;
		if (count == map->capacity - 3)
			break;
	}	


	return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;


}

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}