#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>

#include "interface.h"
#include "state.h"

State state;
//KeyState keys;


void update_and_draw() {



	//keys = malloc(sizeof(*keys));
	struct key_state keys;
	keys.enter = IsKeyPressed(KEY_ENTER);
	//keys.enter = true;

	keys.down = IsKeyDown(KEY_DOWN); 
	keys.up = IsKeyDown(KEY_UP);
	//keys.up = true;

	keys.left = IsKeyDown(KEY_LEFT);
	keys.right = IsKeyDown(KEY_RIGHT);
	keys.n = IsKeyPressed(KEY_N);
	keys.p = IsKeyPressed(KEY_P);



	state_update(state,&keys);
	interface_draw_frame(state);	
}

int main() {
	state = state_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}