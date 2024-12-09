
#include "raylib.h"
#include "state.h"
#include "interface.h"
#include "ADTList.h"
#include <stdio.h>

Texture ball_img;
Texture star_img;

void interface_init(){
    // Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	// Φόρτωση εικόνων και ήχων
	ball_img = LoadTextureFromImage(LoadImage("assets/my_8ball.png"));
    star_img = LoadTextureFromImage(LoadImage("assets/my_greensun.png"));




}


void interface_close(){

    CloseAudioDevice();
	CloseWindow();

}


void interface_draw_frame(State state){
    BeginDrawing();



	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(RAYWHITE);

	// Σχεδιάζουμε τον χαρακτήρα και τις 2 μπάλες 
    
    StateInfo info = state_info(state);
    float ball_x =  info->ball->rect.x;
	float x_offset =   SCREEN_WIDTH - 600 -ball_x;

	//info->ball->rect.x += x_offset;
	DrawTexture(ball_img, info->ball->rect.x + x_offset, info->ball->rect.y,WHITE); 

    List list = state_objects(state, ball_x-SCREEN_WIDTH , ball_x + 600);
    for (ListNode node = list_first(list); node != LIST_EOF; node = list_next(list,node)){
        Object obj = list_node_value(list , node);

        if (obj->type == PLATFORM){
		    DrawRectangle((float)obj->rect.x + x_offset,(float)obj->rect.y,(float)obj->rect.width,(float)obj->rect.height,GRAY);
		}else {
            DrawTexture(star_img, obj->rect.x + x_offset,obj->rect.y,WHITE);
		}	
    }



	 // Σχεδιάζουμε το σκορ και το FPS counter
	 DrawText(TextFormat("%04i", info->score), 20, 20, 40, GREEN);
	 DrawFPS(SCREEN_WIDTH - 80, 0);

	// // Αν το παιχνίδι έχει τελειώσει, σχεδιάζομαι το μήνυμα για να ξαναρχίσει
	//if (info->paused){

	//}
	if (!info->playing) {
	 	DrawText(
	 		"PRESS [ENTER] TO PLAY AGAIN",
	 		 GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
	 		 GetScreenHeight() / 2 - 50, 20, GRAY
	 	);
	}

	EndDrawing();


}


