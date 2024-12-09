//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "state.h"
#include "set_utils.h"
#include "ADTList.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	TEST_ASSERT(info->ball !=NULL);
	TEST_ASSERT(info->ball->type == BALL );
	
	TEST_ASSERT(info->ball->rect.x);
	TEST_ASSERT(info->ball->rect.y);
	TEST_ASSERT(info->ball->rect.height);
	TEST_ASSERT(info->ball->rect.width);
	TEST_ASSERT(!info->ball->unstable);
	// TEST_ASSERT(info->ball->vert_mov);
	// TEST_ASSERT(info->ball->vert_speed);
	List objects1 = state_objects(state,0,1000);
	List objects2 = state_objects(state,210,240);
	List objects3 = state_objects(state,230,260);
	TEST_ASSERT(objects1 != NULL);
	TEST_ASSERT(objects2 != NULL);
	TEST_ASSERT(objects3 != NULL);








	


	// Προσθέστε επιπλέον ελέγχους
}

 void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, η μπάλα μετακινείται 4 pixels δεξιά
	Rectangle old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->ball->rect;

	TEST_ASSERT( new_rect.x == old_rect.x + 4 );

	// Με πατημένο το δεξί βέλος, η μπάλα μετακινείται 6 pixels δεξιά
	keys.right = true;
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;

	TEST_ASSERT( new_rect.x == old_rect.x + 6 );

	 //Προσθέστε επιπλέον ελέγχους
	keys.up = true;	// pressed up arrow
	state_info(state)->ball->vert_mov = IDLE;	//and idle
	state_update(state, &keys);
	TEST_ASSERT(state_info(state)->ball->vert_mov == JUMPING ); //has to jump
	TEST_ASSERT(state_info(state)->ball->vert_speed == 17 ); // and set vertical speed at 17

	old_rect = state_info(state)->ball->rect;	
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;	//after jumping
	TEST_ASSERT(old_rect.y == new_rect.y + 17);	// first frame after jump has to be higher 17 pixels
	state_update(state, &keys);
	TEST_ASSERT(old_rect.y > new_rect.y );	// has to be higher than prev
	
	state_info(state)->ball->vert_speed = 0.5;	//has to turn to falling
	state_update(state,&keys);
	TEST_ASSERT(state_info(state)->ball->vert_mov == FALLING );

	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;	//after starting falling
	TEST_ASSERT(old_rect.y < new_rect.y );	 // has to be lower
	

	state_info(state)->ball->rect.y = SCREEN_HEIGHT;
	state_update(state,&keys);
	TEST_ASSERT(!state_info(state)->playing);

	
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	TEST_ASSERT(!CheckCollisionRecs(old_rect ,new_rect ));
	new_rect = state_info(state)->ball->rect;	//after starting falling
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	TEST_ASSERT(CheckCollisionRecs(old_rect ,new_rect ));

}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};