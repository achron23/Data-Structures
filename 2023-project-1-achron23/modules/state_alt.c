#include <stdlib.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "ADTSet.h"
#include "set_utils.h"
#include "state.h"
#include "stdio.h"

// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (Πλατφόρμες, Αστέρια)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height, VerticalMovement vert_mov, float speed, bool unstable) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type; 
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	obj->vert_mov = vert_mov;	
	obj->vert_speed = speed;	
	obj->unstable = unstable;	
	return obj;
}


int compare_recs(Pointer a, Pointer b){
		Object c = *(Object*)  a;
		Object d = *(Object*)  b; 
		if (c->rect.x > d->rect.x )
			return 1;
		else if (c->rect.x == d->rect.x)
			return 0;
		else 
			return -1;		
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από x = start_x, και επεκτείνονται προς τα δεξιά.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι η αρχή της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά.
//
// - Στον άξονα y το 0 είναι το πάνω μέρος της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα έχει σταθερό ύψος, οπότε όλες οι
//   συντεταγμένες y είναι ανάμεσα στο 0 και το SCREEN_HEIGHT.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_x) {
	// Προσθέτουμε PLATFORM_NUM πλατφόρμες, με τυχαία χαρακτηριστικά.

	for (int i = 0; i < PLATFORM_NUM; i++) {
		Object platform = create_object(
			PLATFORM,
			start_x + 150 + rand() % 80,						// x με τυχαία απόσταση από το προηγούμενο στο διάστημα [150, 230]
			SCREEN_HEIGHT/4 + rand() % SCREEN_HEIGHT/2,			// y τυχαία στο διάστημα [SCREEN_HEIGHT/4, 3*SCREEN_HEIGHT/4]
			i == 0 ? 250 : 50 + rand()%200,						// πλάτος τυχαία στο διάστημα [50, 250] (η πρώτη πάντα 250)
			20,													// ύψος
			i < 3 || rand() % 2 == 0 ? MOVING_UP : MOVING_DOWN,	// τυχαία αρχική κίνηση (οι πρώτες 3 πάντα πάνω)
			0.6 + 3*(rand()%100)/100 ,							// ταχύτητα τυχαία στο διάστημα [0.6, 3.6]
			i > 0 && (rand() % 10) == 0							// το 10% (τυχαία) των πλατφορμών είναι ασταθείς (εκτός από την πρώτη)
		);
		vector_insert_last(state->objects, platform);

		// Στο 50% των πλατφορμών (τυχαία), εκτός της πρώτης, προσθέτουμε αστέρι
		if(i != 0 && rand() % 2 == 0) {
			Object star = create_object(
				STAR,
				start_x + 200 + rand() % 60,				// x με τυχαία απόσταση από την προηγούμενη πλατφόρμα στο διάστημα [200,260]
				SCREEN_HEIGHT/8 + rand() % SCREEN_HEIGHT/2,	// y τυχαία στο διάστημα [SCREEN_HEIGHT/8, 5*SCREEN_HEIGHT/8]
				30, 30,										// πλάτος, ύψος
				IDLE,										// χωρίς κίνηση
				0,										 	// ταχύτητα 0
				false										// 'unstable' πάντα false για τα αστέρια
			);
			vector_insert_last(state->objects, star);
		}

		start_x = platform->rect.x + platform->rect.width;	// μετακίνηση των επόμενων αντικειμένων προς τα δεξιά
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->info.score = 0;				// Αρχικό σκορ 0

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_x = 0.
	state->objects = vector_create(0, NULL);
	add_objects(state, 0);

	// Δημιουργούμε την μπάλα τοποθετώντας τη πάνω στην πρώτη πλατφόρμα
	Object first_platform = vector_get_at(state->objects, 0);
	state->info.ball = create_object(
		BALL,
		first_platform->rect.x,			// x στην αρχή της πλατφόρμας
		first_platform->rect.y - 40,	// y πάνω στην πλατφόρμα
		45, 45,							// πλάτος, ύψος
		IDLE,							// χωρίς αρχική κατακόρυφη κίνηση
		0,								// αρχική ταχύτητα 0
		false							// "unstable" πάντα false για την μπάλα
	);
	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	// Προς υλοποίηση

	return &state->info; //gyrizei mia diefthinsi tou struct me tis plhrofories
}

Object * my_create_object(Object value){
    Object* p = malloc(sizeof(Object));
    *p = value;
    return p ;
}

Set my_set(State state){
    Set flag = set_create(compare_recs,NULL);
    for (int i = 0; i < vector_size(state->objects);i++){
        Object obj = vector_get_at(state->objects,i);
        set_insert(flag,my_create_object(obj));
    }

    
    return flag;


}



// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(State state, float x_from, float x_to) {
	// Προς υλοποίηση
	List list=list_create(NULL);
	ListNode lnode=LIST_BOF;    //iconic node before first
    Set set = my_set(state);
    Object a = set_find_eq_or_greater(set,my_create_object(create_object(PLATFORM,x_from,200,100,20,IDLE,1,0)));
    Object b = set_find_eq_or_smaller(set,my_create_object(create_object(PLATFORM,x_to,200,100,20,IDLE,1,0)));
    SetNode node_a = set_find_node(set,a);
    SetNode node_b = set_find_node(set,b);

	for (SetNode snode = node_a; snode != set_next(set,node_b); snode = set_next(set,snode)){
        list_insert_next(list,lnode,set_node_value(set,snode));
        lnode = list_last(list);
		
	}


	return list;
}



void my_vector_remove(State state , Object a ){
	Vector vec1= vector_create(0,NULL);
	for (int i = 0; i < vector_size(state->objects) ;i ++){
		Object flag = vector_get_at(state->objects,i);
		if (a != flag){
			vector_insert_last(vec1,flag);
		}

	}
	state->objects = vec1;
	//vector_destroy(vec1);
	

	
}


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	// Προς υλοποίηση
	if (!state->info.playing && keys->enter){ // if not playing and press ENTER start again
		state_destroy(state);
		state_create();
	}	 	 	 	 
	if (!state->info.paused && keys->p) // if playing and press P pause
	 	 state->info.paused = true;
	//ball movement
	if((state->info.playing && !state->info.paused) || (state->info.paused && keys->n)){	// if we playing and not paused

		if(keys->right)
		 	state->info.ball->rect.x += 6 * state->speed_factor;
		else if(keys->left)
		 	state->info.ball->rect.x += 1 * state->speed_factor;
		else
		 	state->info.ball->rect.x += 4 * state->speed_factor;

		if(state->info.ball->vert_mov == JUMPING){
			state->info.ball->rect.y -= state->info.ball->vert_speed;	//jump
			state->info.ball->vert_speed *= 0.85;	//jump less in the next frame
			if(state->info.ball->vert_speed <= 0.5)
				state->info.ball->vert_mov = FALLING;
		}
		else if(state->info.ball->vert_mov == FALLING){
			state->info.ball->rect.y += state->info.ball->vert_speed; //fall
			if(state->info.ball->vert_speed <= 7)
				state->info.ball->vert_speed += state->info.ball->vert_speed*0.1; //fall faster in the next frame
		}
		else 
			if(keys->up){
				state->info.ball->vert_mov = JUMPING;
				state->info.ball->vert_speed = 17 * state->speed_factor; 
			}

		//platform movement
		List list = state_objects(state,state->info.ball->rect.x - SCREEN_WIDTH,state->info.ball->rect.x + SCREEN_WIDTH);
		//for (int i=0;i<vector_size(state->objects);i++){	//check all objects
		for (ListNode node = list_first(list);node != LIST_BOF; node = list_next(list,node)){

			Object a = list_node_value(list,node);	//is a cast from pointer to object
			if (a->type == PLATFORM){	// if is a platform
				a->vert_speed *= state->speed_factor;
				if (a->unstable){	// and an unstable one
					if(a->vert_mov == MOVING_UP){ 	//if its moving up
						a->rect.y -= a->vert_speed;		//move it up
						if(a->rect.y < SCREEN_HEIGHT/4)		//because y is decreasing going up		
							a->vert_mov = MOVING_DOWN;	// when enough up start going down
					}
					else if(a->vert_mov == MOVING_DOWN){	//if its moving down
						a->rect.y += a->vert_speed;		//move it down
						if(a->rect.y > 3*SCREEN_HEIGHT/4)	//because y is increasing going down
							a->vert_mov = MOVING_UP;
					}
					else if(a->vert_mov == FALLING)
						a->rect.y += 4 * state->speed_factor;
				}
				//idle ball touches platform
				if (state->info.ball->vert_mov == IDLE){
					if(state->info.ball->rect.x >= a->rect.x && state->info.ball->rect.x <= a->rect.x + a->rect.width )// if greater than x and smaller than x+width dld mesa sta oria ths platform
						state->info.ball->rect.y = a->rect.y - state->info.ball->rect.height; // because y starts from upper left of object and ball has to be above platform
					else if (state->info.ball->rect.x > a->rect.x + a->rect.width) {
						state->info.ball->vert_mov = FALLING;
						state->info.ball->vert_speed = 1.5 * state->speed_factor;
					}	
				}

			}

		}
		// collisions
		if(state->info.ball->rect.y + state->info.ball->rect.height >= SCREEN_HEIGHT)
			state->info.playing = false;	
		for (ListNode node = list_first(list);node != LIST_BOF; node = list_next(list,node)){
			Object a = list_node_value(list,node);
			if (a->type == STAR && CheckCollisionRecs(state->info.ball->rect,a->rect)){		//if the obj is star and the collision is true
				state->info.score += 10;
				my_vector_remove(state,a);
			}
			else if(a->type == PLATFORM ){
				if (a->vert_mov == FALLING && a->rect.y == SCREEN_HEIGHT){
					my_vector_remove(state,a);
				}	
				if(state->info.ball->vert_mov == FALLING && CheckCollisionRecs(state->info.ball->rect,a->rect)){
					state->info.ball->rect.y = a->rect.y - state->info.ball->rect.height; // y of ball becomes y of platform plus ball's height 
					state->info.ball->vert_mov = IDLE;

				}
			}
					
		}
		// make the map forever
		VectorNode vnode = vector_last(state->objects);
		Object a = vector_node_value(state->objects,vnode);
			if(state->info.ball->rect.x + SCREEN_WIDTH >= a->rect.x){	//check for distance
				state->speed_factor += 0.1 * state->speed_factor;
				add_objects(state,a->rect.x);
			}		
	}

}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
	vector_destroy(state->objects);
	free(state);
} 
	
	