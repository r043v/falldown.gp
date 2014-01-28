/***************************************************
		Test Gfx Lib : falldown
***************************************************/
#include "Gdl.h"

#include ".\gfx\map\tile_bg.h"
#include ".\gfx\map\map_bg.h"
#include ".\gfx\map\tiles.h"

#include ".\gfx\sprite\perso.h"

#include ".\gfx\font\font16.h"
#include ".\gfx\font\font8.h"

#include ".\gfx\title.h"

#include "falldown.h"


#include ".\modPlay\gpmodplay.h"
#include ".\zik\fun_reag.h"


void GpMain(void *arg)
{    			
	unsigned char cpt ;
	
	for(cpt=0;cpt<0xFF;cpt++) white_pallette[cpt] = 0xFFFF ;

	init_screen(8) ;

	load_font(&font,(unsigned char *)FONT16,16,16,179,64) ;
	load_font(&small_font,(unsigned char *)FONT8,8,8,179,95) ;

	load_anim(&perso,0,0,21,(unsigned char *)perso_img,150,2,0,32,32,135,0,0xB3) ;
	
	perso.animation[0].frame_stop = 2 ;
	set_anim(&perso,1,2,4,100,1,0) ;
	set_anim(&perso,2,5,20,55,2,0) ;
	
	GpFatInit() ;
	
	MODPlay mp ;
	MODPlay_Init ( &mp );
	MODPlay_SetStereo (&mp, TRUE );
	MODPlay_SetPlayFreq (&mp,22050);
	MODPlay_SetMOD ( &mp, fun_reag );

	
	set_map(&back,(short *)map_level_1_bg,320,27,0,0,(unsigned char *)tile_level_1_bg,52,16,16,0) ;
	set_map(&level,(short *)the_map,10,5000,0,0,(unsigned char *)tiles_level_1,26,32,32,179) ;
	
	white_pal() ;
	GpBitBlt(NULL,&gpDraw[nflip], 0, 0, 320, 240, (unsigned char*)title, 0, 0, 320, 240) ;
	
	read_save_file() ;

	draw_menu() ;
	
	srand(GpTickCountGet()) ;
	
	do {
		MODPlay_Start ( &mp );
		
		do {			
			auto_scrool(&back,perso.pos_x+200,perso.pos_y-level.scrool_y + 200,perso.size_x,perso.size_y,20,20,60,60) ;
			back.scrool_x = (back.scrool_x >> 1) ;
			back.scrool_y = (back.scrool_y >> 1) ;
						
			draw_map_animated_16(&back) ;
			draw_map_animated_32(&level) ;	
			
			play_anim_xy(&perso,animation_number,perso.pos_x-level.scrool_x,perso.pos_y-level.scrool_y,0) ;
			
			old_key = key_pressed ;
			GpKeyGetEx(&key_pressed);

			if(key_pressed&1) // gauche
			{
				perso.animation[2].sens = 0 ;

				go_left(key_speed) ;
			}
			
			if(key_pressed&4) // droite
			{
				perso.animation[2].sens = 1 ;
				
				go_right(key_speed) ;
			}
			
			if(!(old_key&256)) // start front montant
				if(key_pressed&256)
				{
					// pause
					
					char draw = 1 ;
					unsigned short frec = 300 ;
					long last_draw = 0 ;
					
					do
					{					
						old_key = key_pressed ;
						GpKeyGetEx(&key_pressed);
		
						draw_map_animated_16(&back) ;
						draw_map_animated_32(&level) ;	
						
						play_anim_xy(&perso,animation_number,perso.pos_x-level.scrool_x,perso.pos_y-level.scrool_y,0) ;
						
						if(draw) draw_text(&font,"PAUSE",120,112) ;
						
						// pour faire clignoter
						if(frec + last_draw < GpTickCountGet())
						{
							draw ^= 1 ;
							last_draw = GpTickCountGet() ;
						}
						
						flip_screen() ;
	
					} while( ! ( (!(old_key&256))&&(key_pressed&256) ) ) ;
				}

			if(key_pressed==512) 	dead=1;
			
			// gravity :-)
				go_down(down_speed) ;
			
			GPSPRINTF(buffer,"%i",points) ;
			draw_text_rl(&font,buffer,318,4) ;
			
			level.scrool_y+=speed ;

			screen_pos = perso.pos_y + perso.size_y - level.scrool_y ;
			
			if(screen_pos < 0) dead=1 ; ;
					
			flip_screen() ;	
	            		
		} while(!dead) ;
		dead=0 ;
		
		MODPlay_Stop ( &mp );

		finish_game() ;
		
		draw_menu() ;
		
	} while(1) ;
}

