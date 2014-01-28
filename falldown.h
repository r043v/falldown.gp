#define animation_number perso.custom_short_var[0]

struct save_file
{
	char enames[3][4] ;
	char nnames[3][4] ;
	char hnames[3][4] ;
	unsigned short score[3][3] ;
} ;

unsigned int white_pallette[256] ;

struct map back ;
struct map level ;

struct font font ;
struct font small_font ;

struct gfx perso ;

long screen_pos ;
short speed, key_speed, down_speed ;
	
unsigned int scrool_count = 0 ;
unsigned short points = 0 ;

char buffer[10] ;

char dead = 0 ;

char dificult = 1 ; // normal

short the_map[5000][10] ;

struct save_file scores ;

void sauver_save_file(void)
{
	F_HANDLE hfile;
	
	GpFileOpen("gp:\\gpmm\\falldown.sav", OPEN_W, &hfile) ;
	
	GpFileWrite(hfile,&scores,sizeof(struct save_file));
	
	GpFileClose(hfile) ;
}

void read_save_file(void)
{
	char cpt ;
	F_HANDLE hfile;
	
	if (SM_OK != GpFileOpen("gp:\\gpmm\\falldown.sav", OPEN_R, &hfile))
	{		
		GpFileCreate("gp:\\gpmm\\falldown.sav",ALWAYS_CREATE, &hfile);
		
		for(cpt=0;cpt<3;cpt++)
		{
			GPSTRCPY(scores.enames[cpt],"ROV");
			GPSTRCPY(scores.nnames[cpt],"ROV");
			GPSTRCPY(scores.hnames[cpt],"ROV");
		} ;
		
		for(cpt=0;cpt<3;cpt++)
		{
			scores.score[2][cpt] = 25 ;
			scores.score[1][cpt] = 50 ;
			scores.score[0][cpt] = 75 ;
		} ;
		
		sauver_save_file() ;
	}
	else
	{
		unsigned long m_size=0 ;
		GpFileRead(hfile,&scores,sizeof(struct save_file),&m_size);
		GpFileClose(hfile) ;
	}
}

void init_map(void)
{
	unsigned int cptx=0, cpty=0 ;
	unsigned short skip=0, nb_stop_cases=0, value, tmp=0 ;
	
	for(cpty=0;cpty<10;cpty++)
	{
		for(cptx=1;cptx<9;cptx++)
			the_map[cpty][cptx] = 0 ;
	
		if(tmp)
		{
			the_map[cpty][0] = 3 ;
			the_map[cpty][9] = 3 ;
			tmp=0 ;
		}
		else
		{
			the_map[cpty][0] = 4 ;
			the_map[cpty][9] = 4 ;
			tmp=1 ;
		}		
	} ;
	
	tmp=0 ;
	
	for(cpty=10;cpty<5000;cpty++)
	{
		skip++ ;
		
		if(skip==4) // add line
		{
			tmp++ ;
			
			if(tmp==4) tmp=2 ;
						
			skip=0 ;
			nb_stop_cases=0 ;

			for(cptx=0;cptx<10;cptx+=2)
			{
				value = rand()%3 ;

				if(nb_stop_cases < 8)
				{
					if(value)
					{
						the_map[cpty][cptx] = tmp ;
						nb_stop_cases++ ;
					} else the_map[cpty][cptx] = 0 ;
				}
				else the_map[cpty][cptx] = 0 ;
			} ;

			for(cptx=1;cptx<10;cptx+=2)
			{
				value = rand()%3 ;
				
				if(nb_stop_cases < 8)
				{
					if(value)
					{
						the_map[cpty][cptx] = tmp ;
						nb_stop_cases++ ;
					} else the_map[cpty][cptx] = 0 ;
				}
				else the_map[cpty][cptx] = 0 ;
			} ;
		}
		else	for(cptx=0;cptx<10;cptx++)	the_map[cpty][cptx] = 0 ;		
	} ;
}

void draw_score(void)
{
	char info[5][15] ;

	
	if(dificult == 1)
	{	
		GPSPRINTF(info[1],"%s - %i",scores.nnames[0],scores.score[0][1]) ;
		GPSPRINTF(info[2],"%s - %i",scores.nnames[1],scores.score[1][1]) ;
		GPSPRINTF(info[3],"%s - %i",scores.nnames[2],scores.score[2][1]) ;
		GPSPRINTF(info[4],"<NORMAL>") ;
		speed = 4 ;
		key_speed = 3 ;
		down_speed = 5 ;
	}
	else	if(dificult == 2)
		{
			GPSPRINTF(info[1],"%s - %i",scores.hnames[0],scores.score[0][2]) ;
			GPSPRINTF(info[2],"%s - %i",scores.hnames[1],scores.score[1][2]) ;
			GPSPRINTF(info[3],"%s - %i",scores.hnames[2],scores.score[2][2]) ;
			GPSPRINTF(info[4],"<HARD>") ;
			speed = 5 ;
			key_speed = 5 ;
			down_speed = 7 ;
		}
		else		if(dificult == 0)
				{				
					GPSPRINTF(info[1],"%s - %i",scores.enames[0],scores.score[0][0]) ;
					GPSPRINTF(info[2],"%s - %i",scores.enames[1],scores.score[1][0]) ;
					GPSPRINTF(info[3],"%s - %i",scores.enames[2],scores.score[2][0]) ;
					GPSPRINTF(info[4],"<EASY>") ;
					speed = 3 ;
					key_speed = 3 ;
					down_speed = 4 ;
				}

	draw_text(&font,info[1],160,10) ;
	draw_text(&font,info[2],160,30) ;
	draw_text(&font,info[3],160,50) ;
	draw_text_rl(&font,info[4],310,155) ;
	
	draw_text(&small_font,"Gdl demo release - http://rov.fr.st",16,222);

}

void draw_menu(void)
{
	long last_key=0 ;
	
	
	fade_to_palette(30,white_pallette) ;

	GpBitBlt(NULL,&gpDraw[nflip], 0, 0, 320, 240, (unsigned char*)title, 0, 0, 320, 240) ;
	draw_score() ;
	
	flip_screen() ;	
	fade_to_palette(30,title_Pal) ;
	
	do
	{
		GpBitBlt(NULL,&gpDraw[nflip], 0, 0, 320, 240, (unsigned char*)title, 0, 0, 320, 240) ;
		draw_score() ;
		
		flip_screen() ;
		
		old_key = key_pressed ;
		GpKeyGetEx(&key_pressed) ;
	
		if(key_pressed&1) // gauche
			if((!(old_key&1)) || (last_key + 800 < GpTickCountGet()))
			{
				if(dificult) dificult-- ;
				last_key = GpTickCountGet() ;
			}
		
		if(key_pressed&4) // droite
			if((!(old_key&4)) || (last_key + 800 < GpTickCountGet()))
			{
				if(dificult!=2) dificult++ ;
				last_key = GpTickCountGet() ;
			}
	
		if(key_pressed==512)
		{
			fade_to_palette(50,white_pallette) ;
			GpAppExit();
		}
	
	} while(key_pressed!=256) ;
	
	
	key_pressed=0 ;

	fade_to_palette(50,white_pallette) ;
	
	init_map() ;

	level.scrool_y=0 ;
	back.scrool_y = 120 ;
	
	points = 0 ;
	perso.pos_x=135 ;
	perso.pos_y = 30 ;	

	auto_scrool(&back,perso.pos_x+200,perso.pos_y-level.scrool_y + 200,perso.size_x,perso.size_y,20,20,60,60) ;
	back.scrool_x = (back.scrool_x >> 1) ;
	back.scrool_y = (back.scrool_y >> 1) ;

	draw_map_animated_16(&back) ;
	draw_map_animated_32(&level) ;

	draw_text(&font,"GET READY !!!",56,112) ;
	
	flip_screen() ;

	fade_to_palette(50,wa_pal) ;
}



char can_perso_down(void)
{	
	if(!get_tile(&level, perso.pos_x + 4 - level.scrool_x, perso.pos_y + perso.size_y - 3 - level.scrool_y))
		if(!get_tile(&level, perso.pos_x + perso.size_x - 5 - level.scrool_x, perso.pos_y + perso.size_y - 3 - level.scrool_y))
			return 1 ;	

	return 0 ;
}

char can_perso_left(void)
{
	if(perso.pos_x <= 0) return 0 ;
	
	if(!get_tile(&level, perso.pos_x + 2 - level.scrool_x, perso.pos_y + 4 - level.scrool_y))
		if(!get_tile(&level, perso.pos_x + 2 - level.scrool_x, perso.pos_y + perso.size_y - 4 - level.scrool_y))
			return 1 ;
	return 0 ;
}

char can_perso_right(void)
{
	if(perso.pos_x >= 288) return 0 ;
	
	if(!get_tile(&level, perso.pos_x + perso.size_x - 2 - level.scrool_x, perso.pos_y + 4 - level.scrool_y))
		if(!get_tile(&level, perso.pos_x + perso.size_x - 2 - level.scrool_x, perso.pos_y + perso.size_y - 4 - level.scrool_y))
			return 1 ;
	return 0 ;
}

unsigned short go_down(unsigned char powa)
{
	do
	{
		if(can_perso_down())
		{
			if(perso.pos_y + perso.size_y - level.scrool_y != 240)	perso.pos_y++ ;
			
			animation_number = 2 ;
			
			scrool_count ++ ;
		
			if(scrool_count >= 128)
			{
				points++ ;
				scrool_count=0 ;
			}			
		}
		else
		{
			animation_number = 0 ;
			return 0 ;
		}
	} while(powa--) ;
	return 1 ;
}

unsigned short go_left(unsigned char powa)
{
	do
	{
		if(can_perso_left())
		{
			perso.pos_x-- ;
		}
		else  return 0 ;
	} while(powa--) ;
	return 1 ;
}

unsigned short go_right(unsigned char powa)
{
	do
	{
		if(can_perso_right())
		{
			perso.pos_x++ ;
		}
		else  return 0 ;
	} while(powa--) ;
	return 1 ;
}

void finish_game(void)
{
	short position=0 ;
	
	char letter = 'A' ;
	char names[4] ;
	char letter_num = 0 ;
	char cpt ;
	char txt[2] ;
	
	char draw = 1 ;
	unsigned short frec = 300 ;
	long last_draw = 0 ;
	long last_key = 0 ;
	
		
	char buffer[3][30] ;
	
	if(points > scores.score[0][dificult]) position = 1 ;
	else	if(points > scores.score[1][dificult]) position = 2 ;
		else	if(points > scores.score[2][dificult]) position = 3 ;
	
	if(position) // si ds le high score
	{
		GPSPRINTF(buffer[0],"%i POINTS !!",points) ;
		
		do {
	
		old_key = key_pressed ;
		GpKeyGetEx(&key_pressed);
		
		draw_map_animated_16(&back) ;
		draw_map_animated_32(&level) ;

		draw_text(&font,buffer[0],((320 - (GPSTRLEN(buffer[0])*16))>>1),15) ;
		draw_text(&font,"NEW RECORD !",64,35) ;
		draw_text(&font,"ENTER YOUR NAME !",24,55) ;

		play_anim_xy(&perso,1,5,203,0) ;
		play_anim_xy(&perso,1,283,203,0) ;

		draw_text_rl(&small_font,"A : valid letter",275,200) ;
		draw_text_rl(&small_font,"B : cancel letter",275,210) ;
		draw_text_rl(&small_font,"START : valid name",275,220) ;
		
		// affiche les lettre déjà validées
		
		for(cpt=0;cpt<letter_num;cpt++)
		{
			GPSPRINTF(txt,"%c",names[cpt]) ;
			draw_text(&font,txt,136+cpt*16,75) ;
		} ;	
		
		if(key_pressed) draw=1 ;
		
		// affiche la lettre courante, qui clignote		
		if(draw)
		{
			char txt[2] ;
			txt[0] = letter ;
			txt[1] = ' ' ;

			if(letter_num != 3) draw_text(&font,txt,136+letter_num*16,75) ;
			else	draw_text(&small_font,"OK",136+letter_num*16,79) ;
		}
		
		// pour faire clignoter
		if(frec + last_draw < GpTickCountGet())
		{
			draw ^= 1 ;
			last_draw = GpTickCountGet() ;
		}

		if(key_pressed&1) // gauche
			if((!(old_key&1)) || (last_key + 175 < GpTickCountGet()))
			{
				if(letter > 'A') letter-- ;
				else letter = 'Z' ;
				
				last_key = GpTickCountGet() ;
			}
		
		if(key_pressed&4) // droite
			if((!(old_key&4)) || (last_key + 175 < GpTickCountGet()))
			{
				if(letter < 'Z') letter++ ;
				else letter = 'A' ;
				
				last_key = GpTickCountGet() ;
			}
		
		// A : validation de la lettre
		if(!(old_key&64))
			if(key_pressed&64)
			{
				if(letter_num == 3) key_pressed = 256 ; // auto validation si les 3 lettre rentrées
				else
				{
					names[letter_num] = letter ;
					letter_num++ ;
				}
			}

		// B : annulation de la derniere lettre rentrée
		if(!(old_key&32))
			if(key_pressed&32)
			{
				if(letter_num)
				{
					if(letter_num < 3) names[letter_num] = ' ' ;
					letter_num-- ;
					letter = names[letter_num] ;
				}
			}
		
		
		if(key_pressed&256)
			if(letter_num)
			{
				// si pas 3 lettres
				while(letter_num < 3)
				{
					names[letter_num] = ' ' ;
					letter_num++ ;
				} ;
				
				// sauvegarde
				
				if(!dificult)
				{
					if(position == 1)
					{
						GPSTRCPY(scores.enames[2],scores.enames[1]) ;
						scores.score[2][dificult] = scores.score[1][dificult] ;
						
						GPSTRCPY(scores.enames[1],scores.enames[0]) ;
						scores.score[1][dificult] = scores.score[0][dificult] ;						
					}

					if(position == 2)
					{
						GPSTRCPY(scores.enames[2],scores.enames[1]) ;
						scores.score[2][dificult] = scores.score[1][dificult] ;
					}

					GPSTRCPY(scores.enames[position-1],names) ;					
				}
				else	if(dificult==1)
					{
						if(position == 1)
						{
							GPSTRCPY(scores.nnames[2],scores.nnames[1]) ;
							scores.score[2][dificult] = scores.score[1][dificult] ;
							
							GPSTRCPY(scores.nnames[1],scores.nnames[0]) ;
							scores.score[1][dificult] = scores.score[0][dificult] ;						
						}
	
						if(position == 2)
						{
							GPSTRCPY(scores.nnames[2],scores.nnames[1]) ;
							scores.score[2][dificult] = scores.score[1][dificult] ;
						}

						GPSTRCPY(scores.nnames[position-1],names) ;
					}
					else	if(dificult==2)
						{
							if(position == 1)
							{
								GPSTRCPY(scores.hnames[2],scores.hnames[1]) ;
								scores.score[2][dificult] = scores.score[1][dificult] ;
								
								GPSTRCPY(scores.hnames[1],scores.hnames[0]) ;
								scores.score[1][dificult] = scores.score[0][dificult] ;						
							}
		
							if(position == 2)
							{
								GPSTRCPY(scores.hnames[2],scores.hnames[1]) ;
								scores.score[2][dificult] = scores.score[1][dificult] ;
							}

							GPSTRCPY(scores.hnames[position-1],names) ;							
						}
	
				scores.score[position-1][dificult] = points ;
	
				sauver_save_file() ;			
			}
		
		flip_screen() ;
		
		} while(!(key_pressed&256)) ;
	}
}

