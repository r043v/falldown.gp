unsigned char tile_draw_method = 1 ;

//-----------------------------------------------------------------------

/* charge et initialise le tableau de tiles
	unsigned char * tile_bmp	: pointeur sur l'image contenant tous les tiles
					  tiles non animés, premier tile a gauche, dernier a droite
	unsigned char nb_tiles		: nombre de tiles a charger	*/

void load_tiles(struct map * map, unsigned char * tile_bmp, unsigned short nb_tiles, unsigned short transp_color)
{
	unsigned short cpt, cpt_x, cpt_y;
	
	for(cpt=0;cpt<nb_tiles;cpt++)
	{
		map->tab_tiles[cpt].img[0] = tile_bmp + (cpt*map->size_tile_x*map->size_tile_y* bbp/8) ;
		
		map->tab_tiles[cpt].transp = 0 ;

		if(transp_color)
		{
			for(cpt_x=0;cpt_x<map->size_tile_x-1;cpt_x++)
				for(cpt_y=0;cpt_y<map->size_tile_y-1;cpt_y++)
				{
					if(transp_color == *((unsigned char *)map->tab_tiles[cpt].img[0] + cpt_x*map->size_tile_y + (map->size_tile_y-cpt_y)))
					{
						map->tab_tiles[cpt].transp=1 ;
						cpt_y = map->size_tile_y ;
						cpt_x = map->size_tile_x ;
					}
				} ;
		}
		
		map->tab_tiles[cpt].animated = 0 ;
		map->tab_tiles[cpt].nb_tiles = 1 ;
		map->tab_tiles[cpt].time_anim = 150 ;
		map->tab_tiles[cpt].last_time = 0 ;
		map->tab_tiles[cpt].anim_id = 0 ;
		map->transp_color = transp_color ;
		if(!transp_color) map->transp_map = 0 ;
		else  map->transp_map = 1 ;
	} ;
}

//-----------------------------------------------------------------------

void set_walkable_on_tiles(struct map * map,unsigned char * walkable, unsigned short nb_tiles)
{
	unsigned short cpt;
	
	for(cpt=0;cpt<nb_tiles;cpt++)
		map->tab_tiles[cpt].walkable = walkable[cpt] ;
}

//-----------------------------------------------------------------------

void set_map(struct map * map,short * map_tab, unsigned short size_x, unsigned short size_y, int scrool_x, int scrool_y, unsigned char * tiles, unsigned short nb_tiles, unsigned char size_tile_x, unsigned char size_tile_y, unsigned short transp_color)
{	
	map->map = map_tab ;
	
	map->size_x = size_x ;
	map->size_y = size_y ;
	
	map->scrool_x = scrool_x ;
	map->scrool_y = scrool_y ;

	map->size_tile_x = size_tile_x ;
	map->size_tile_y = size_tile_y ;

	map->map_x_max = size_x*size_tile_x - 320 ;
	map->map_y_max = size_y*size_tile_x - 240 ;

	map->nb_tile_draw_x = 320/size_tile_x ;
	map->nb_tile_draw_y = 240/size_tile_y ;
		
	load_tiles(map,tiles,nb_tiles,transp_color) ;
}

//-----------------------------------------------------------------------

/* charge et initialise un tile animé ds le tableau de tiles de la map 'map'
	struct map * map		: pointeur sur la map
	unsigned char * tile_bmp	: pointeur sur l'image contenant le tile animé
					  premiere frame a gauche, derniere a droite
	unsigned char nb_tiles		: nombre de frames du tile animé (10 max)
	char id_tile			: numero du tile animé ds le tableau de tiles	*/

void load_animated_tile(struct map * map, unsigned char * tile_bmp, unsigned short nb_tiles, unsigned short id_tile, unsigned char type_anim, unsigned short time_anim)
{
	unsigned short cpt;
	
	for(cpt=0;cpt<nb_tiles;cpt++)
	{
		if(bbp==16) map->tab_tiles[id_tile].img[cpt] = tile_bmp + (cpt*map->size_tile_x*map->size_tile_y*2) ;
		else map->tab_tiles[id_tile].img[cpt] = tile_bmp + (cpt*map->size_tile_x*map->size_tile_y) ;		
	} ;
	
	map->tab_tiles[id_tile].animated = 1 ;
	map->tab_tiles[id_tile].nb_tiles = nb_tiles ;
	map->tab_tiles[id_tile].time_anim = time_anim ;
	map->tab_tiles[id_tile].last_time = 0 ;
	map->tab_tiles[id_tile].anim_id = rand()%nb_tiles ;
	map->tab_tiles[id_tile].type_anim = type_anim ;
}


//-----------------------------------------------------------------------

/* affiche un tile decoupé aux dimension de l'ecran
	short x			: position x
	short y			: position y
	unsigned char* sprite	: pointeur sur l'image du tile a afficher	*/

void afficher_tile_cut(struct map * map, short x, short y, unsigned char* sprite,unsigned short id)
{
	short aff_x = x ;
	char aff_y = y,taille_y = map->size_tile_y, taille_x = map->size_tile_x ;
	short debut_x = 0, debut_y = 0 ;
	
	if(x < 0)
	{
		aff_x = 0 ;
		taille_x = map->size_tile_x + x ;
		debut_x = aff_x - x ;
	}
	
	if(y < 0)
	{
		aff_y = 0 ;
		taille_y = map->size_tile_y + y ;
		debut_y = aff_y - y ;
	}
	
	if(x+map->size_tile_x > 320)
	{
		if(x < 320) taille_x = 320 - x ;
		else taille_x = 0 ;
	}
	
	if(y+map->size_tile_y > 240)
	{
		if(y < 240) taille_y = 240 - y ;
		else taille_y = 0 ;
	}

	if(map->tab_tiles[id].transp)
	{	
		if(bbp==16)	GpTransBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y,map->size_tile_x,map->size_tile_y,map->transp_color) ;
		else		GpTransBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y,map->size_tile_x,map->size_tile_y,map->transp_color) ;
	}
	else
		if(tile_draw_method)
		{
			if(bbp==16)	GpBitBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y,map->size_tile_x,map->size_tile_y) ;
			else		GpBitBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y,map->size_tile_x,map->size_tile_y) ;
		}
}

//-----------------------------------------------------------------------

void set_next_tile_frame_number(struct map * map, unsigned short id)
{
		switch (map->tab_tiles[id].type_anim)
		{
	 		case 0 : // 123123...
				map->tab_tiles[id].anim_id++ ;
				if(map->tab_tiles[id].anim_id >= map->tab_tiles[id].nb_tiles) map->tab_tiles[id].anim_id = 0 ;
		 		break;
	
	 		case 1 : // 123333...
	 			map->tab_tiles[id].anim_id++ ;
				if(map->tab_tiles[id].anim_id >= map->tab_tiles[id].nb_tiles) map->tab_tiles[id].anim_id = map->tab_tiles[id].nb_tiles ;
		 		break;
	
	 		case 2 : // 123111...
	 			map->tab_tiles[id].anim_id++ ;
				if(map->tab_tiles[id].anim_id >= map->tab_tiles[id].nb_tiles) map->tab_tiles[id].anim_id = map->tab_tiles[id].animated = 0 ;
		 		break;

	 		case 3 : // 1232123...
				if(!map->tab_tiles[id].sens_anim) // 123
				{
					map->tab_tiles[id].anim_id++ ;
					if(map->tab_tiles[id].anim_id >= map->tab_tiles[id].nb_tiles) map->tab_tiles[id].sens_anim = 1 ;
				}
				else // 321
				{
					map->tab_tiles[id].anim_id-- ;
					if(map->tab_tiles[id].anim_id == 0) map->tab_tiles[id].sens_anim = 0 ;
				}
		 		break;
		}
}

//-----------------------------------------------------------------------



/* gere l'animation et l'affichage d'un tile decoupé au dimmension de l'ecran
	short id	: numero ds le tableau de tile du tile a animer et afficher
	short px	: position x
	short py	: position y	*/

void draw_tile_xy_cut(struct map * map, unsigned short id,unsigned short px,unsigned short py)
{
	if(map->tab_tiles[id].animated)
	{
		afficher_tile_cut(map,px,py,map->tab_tiles[id].img[map->tab_tiles[id].anim_id],id) ;
		
		if(GpTickCountGet() > (map->tab_tiles[id].last_time + map->tab_tiles[id].time_anim)) // si temps d'anim ecoulé, on met a jour l'image
		{
			set_next_tile_frame_number(map,id) ;
			map->tab_tiles[id].last_time = GpTickCountGet() ;
		}
	}
	else
	{
		afficher_tile_cut(map,px,py,map->tab_tiles[id].img[0],id) ;
	}
}

//-----------------------------------------------------------------------

/* gere l'animation et l'affichage d'un tile non decoupé
	short id	: numero ds le tableau de tile du tile a animer et afficher
	short px	: position x
	short py	: position y	*/

void draw_tile_xy(struct map * map, unsigned short id,unsigned short px,unsigned short py)
{
	if(map->tab_tiles[id].animated)
	{
		if(map->tab_tiles[id].transp)
		{
			if(bbp==16) GpTransBlt16(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[map->tab_tiles[id].anim_id],0,0,map->size_tile_x,map->size_tile_y,map->transp_color) ;
			else GpTransBlt(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[map->tab_tiles[id].anim_id],0,0,map->size_tile_x,map->size_tile_y,map->transp_color) ;
		}
		else
		if(tile_draw_method)
		{
			if(bbp==16) GpBitBlt16(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[map->tab_tiles[id].anim_id],0,0,map->size_tile_x,map->size_tile_y) ;
			else GpBitBlt(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[map->tab_tiles[id].anim_id],0,0,map->size_tile_x,map->size_tile_y) ;		
		}

		if(GpTickCountGet() > (map->tab_tiles[id].last_time + map->tab_tiles[id].time_anim)) // si temps d'anim ecoulé, on met a jour l'image
		{
			set_next_tile_frame_number(map,id) ;
			map->tab_tiles[id].last_time = GpTickCountGet() ;
		}
	}
	else
	{
		if(map->tab_tiles[id].transp)
		{
			if(bbp==16) GpTransBlt16(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[0],0,0,map->size_tile_x,map->size_tile_y,map->transp_color) ;
			else GpTransBlt(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[0],0,0,map->size_tile_x,map->size_tile_y,map->transp_color) ;
		}
		else
		if(tile_draw_method)
		{
			if(bbp==16) GpBitBlt16(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[0],0,0,map->size_tile_x,map->size_tile_y) ;
			else GpBitBlt(NULL,&gpDraw[nflip],px,py,map->size_tile_x,map->size_tile_y,map->tab_tiles[id].img[0],0,0,map->size_tile_x,map->size_tile_y) ;		
		}
	}
}

//-----------------------------------------------------------------------

/* affiche une map en 16*16 sur l'ecran entier, animée */

void draw_map_animated_16(struct map * map)
{
	short debut_case_aff_x, decalage_case_x ;
	short debut_case_aff_y, decalage_case_y ;
	short cpt_x,cpt_y ;
	unsigned short id ;
	
	debut_case_aff_x = map->scrool_x / map->size_tile_x ;
	decalage_case_x = ((debut_case_aff_x+1)*map->size_tile_x) - map->scrool_x ;

	debut_case_aff_y = map->scrool_y / map->size_tile_y ;
	decalage_case_y = ((debut_case_aff_y+1)*map->size_tile_y) - map->scrool_y ;
	
	// affichage ligne haute et basse en tiles cut
	for(cpt_x=debut_case_aff_x;cpt_x<=(debut_case_aff_x+20);cpt_x++)
	{
		id=map->map[debut_case_aff_y*map->size_x + cpt_x] ;
		if(id) draw_tile_xy_cut(map,id,((cpt_x-debut_case_aff_x-1)*(map->size_tile_x))+decalage_case_x,decalage_case_y-(map->size_tile_y)) ;
		
		id=map->map[(debut_case_aff_y+15)*map->size_x + cpt_x] ;
		if(id) draw_tile_xy_cut(map,id,((cpt_x-debut_case_aff_x-1)*(map->size_tile_x))+decalage_case_x,decalage_case_y+(240-map->size_tile_y)) ;
	};

	// affichage collone gauche et droite en tiles cut (moins les tiles du haut et du bas)
	
	for(cpt_y=debut_case_aff_y+1;cpt_y<(debut_case_aff_y+15);cpt_y++)
	{
		id=map->map[cpt_y*map->size_x + debut_case_aff_x] ;
		if(id) draw_tile_xy_cut(map,id,decalage_case_x-map->size_tile_x,((cpt_y-debut_case_aff_y-1)*map->size_tile_y)+decalage_case_y) ;
		
		id=map->map[cpt_y*map->size_x + debut_case_aff_x+20] ;
		if(id) draw_tile_xy_cut(map,id,decalage_case_x+(320-map->size_tile_x),((cpt_y-debut_case_aff_y-1)*map->size_tile_y)+decalage_case_y) ;
	}
	
	// affichage du reste de l'ecran en tiles pleins

		for(cpt_x=1;cpt_x<20;cpt_x++)
			for(cpt_y=1;cpt_y<15;cpt_y++)
			{
				id=map->map[(cpt_y+debut_case_aff_y)*map->size_x + cpt_x+debut_case_aff_x] ;
				if(id) draw_tile_xy(map,id,((cpt_x-1)*map->size_tile_x)+decalage_case_x,((cpt_y-1)*map->size_tile_y)+decalage_case_y) ;
			} ;
}



/* affiche une map en 32*32 sur l'ecran entier, animée */

void draw_map_animated_32(struct map * map)
{
	short debut_case_aff_x, decalage_case_x ;
	short debut_case_aff_y, decalage_case_y ;
	short cpt_x,cpt_y ;
	unsigned short id ;
	
	debut_case_aff_x = map->scrool_x >> 5 ;
	decalage_case_x = ((debut_case_aff_x+1)*map->size_tile_x) - map->scrool_x ;

	debut_case_aff_y = map->scrool_y >> 5 ;
	decalage_case_y = ((debut_case_aff_y+1)*map->size_tile_y) - map->scrool_y ;
	
	// affichage ligne haute et basse en tiles cut
	for(cpt_x=debut_case_aff_x;cpt_x<=(debut_case_aff_x+map->nb_tile_draw_x);cpt_x++)
	{
		id=map->map[debut_case_aff_y*map->size_x + cpt_x] ;
		if(id) draw_tile_xy_cut(map,id,((cpt_x-debut_case_aff_x-1)*(map->size_tile_x))+decalage_case_x,decalage_case_y-(map->size_tile_y)) ;
		
		id=map->map[(debut_case_aff_y+map->nb_tile_draw_y+1)*map->size_x + cpt_x] ;
		if(id) draw_tile_xy_cut(map,id,((cpt_x-debut_case_aff_x-1)*(map->size_tile_x))+decalage_case_x,decalage_case_y+((map->nb_tile_draw_y)*map->size_tile_y)) ;
	};

	// affichage collone gauche et droite en tiles cut (moins les tiles du haut et du bas)
	
	for(cpt_y=debut_case_aff_y+1;cpt_y<(debut_case_aff_y+map->nb_tile_draw_y+1);cpt_y++)
	{
		id=map->map[cpt_y*map->size_x + debut_case_aff_x] ;
		if(id) draw_tile_xy_cut(map,id,decalage_case_x-map->size_tile_x,((cpt_y-debut_case_aff_y-1)*map->size_tile_y)+decalage_case_y) ;
		
		id=map->map[cpt_y*map->size_x + debut_case_aff_x+map->nb_tile_draw_x] ;
		if(id) draw_tile_xy_cut(map,id,decalage_case_x+(320-map->size_tile_x),((cpt_y-debut_case_aff_y-1)*map->size_tile_y)+decalage_case_y) ;
	}

	// affichage du reste de l'ecran en tiles pleins

		for(cpt_x=1;cpt_x<map->nb_tile_draw_x;cpt_x++)
			for(cpt_y=1;cpt_y<=map->nb_tile_draw_y;cpt_y++)
			{
				id=map->map[(cpt_y+debut_case_aff_y)*map->size_x + cpt_x+debut_case_aff_x] ;
				if(id) draw_tile_xy(map,id,((cpt_x-1)*map->size_tile_x)+decalage_case_x,((cpt_y-1)*map->size_tile_y)+decalage_case_y) ;
			} ;
}

//-----------------------------------------------------------------------

/* retourne le numero du tile affiché a un certain point de l'ecran
	short x		: position en x a l'ecran du point a tester
	short y 	: position en y		*/

short get_tile(struct map * map, short x, short y)
{
	unsigned short map_x = (map->scrool_x + x)/map->size_tile_x ;
	unsigned short map_y = (map->scrool_y + y)/map->size_tile_y ;
	
	return map->map[map_y*map->size_x + map_x] ;
}

//-----------------------------------------------------------------------

/* change ds la map le tile qui est affiché a un certain point de l'ecran par un autre tile
	short x		: position en x a l'ecran du point a tester
	short y 	: position en y
	short id	: nouveau numero de tile	*/

void set_tile(struct map * map, short x, short y, short id)
{
	unsigned short map_x = (map->scrool_x + x)/map->size_tile_x ;
	unsigned short map_y = (map->scrool_y + y)/map->size_tile_y ;
	
	map->map[map_y*map->size_x + map_x] = id ;
}


//-----------------------------------------------------------------------

// marche pas, reecrire en utilisant get_start_pos_of_a_tile

/* retourne 1 si les 2 points de l'ecran donné en paramettre sont sur le mm tile
	short x1	: position en x a l'ecran du premier point a tester
	short y1 	: position en y	a l'ecran du premier point a tester
	
	short x2	: position en x a l'ecran du 2eme point a tester
	short y2 	: position en y	a l'ecran du 2eme point a tester
*/


unsigned char same_tile(struct map * map, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	unsigned short map_x1 = (map->scrool_x + x1)/map->size_tile_x ;
	unsigned short map_y1 = (map->scrool_y + y1)/map->size_tile_y ;

	unsigned short map_x2 = (map->scrool_x + x2)/map->size_tile_x ;
	unsigned short map_y2 = (map->scrool_y + y2)/map->size_tile_y ;
	
	if(map_x1==map_x2)
		if(map_y1==map_y2)
			return 1 ;

	return 0 ;
}

//-----------------------------------------------------------------------

/* renvoi si le tile 'id' de la map 'map' est walkable ou non */

char get_walkable(struct map * map, short id)
{		
	return map->tab_tiles[id].walkable ;
}

//-----------------------------------------------------------------------

/* affecte le tile 'id' de la map 'map' walkable ou non */

void set_walkable(struct map * map, short id, unsigned char walkable)
{		
	map->tab_tiles[id].walkable = walkable ;
}

//-----------------------------------------------------------------------

char test_tile(struct map * map, short id_tile, unsigned char masq)
{
	/*
		masq format to test the walkable var of the tile :
		
		bite 7 -> masq = 10000000 = 128 = 0x80
		bite 6 -> masq = 01000000 = 64  = 0x40
		bite 5 -> masq = 00100000 = 32  = 0x20
		bite 4 -> masq = 00010000 = 16  = 0x10
		bite 3 -> masq = 00001000 = 8   = 0x8
		bite 2 -> masq = 00000100 = 4   = 0x4
		bite 1 -> masq = 00000010 = 2   = 0x2
		bite 0 -> masq = 00000001 = 1   = 0x1
	*/
	
	//if( (map->tab_tiles[id_tile].walkable & masq) == masq ) return 1 ;
	
	return (map->tab_tiles[id_tile].walkable & masq) ;
	
	//return 0 ;
}

//-----------------------------------------------------------------------

/* scrolle automatiquement l'ecran pour suivre un objet qui se deplace
	
	struct map * map	: pointeur sur la map
	short pos_x		: position en x ds la map de l'objet a suivre
	short pos_y		: position en y ...
	short size_x		: taille en x de l'objet a suivre
	short size_y		: taille en y de l'objet a suivre
	short limit_up		: l'ecran scrolle si l'objet est a limit_up pixels du haut de l'ecran
	short limit_down	: l'ecran scrolle si l'objet est a limit_down pixels du bas de l'ecran
	short limit_right	: l'ecran scrolle si l'objet est a limit_right pixels de la droite de l'ecran
	short limit_left	: l'ecran scrolle si l'objet est a limit_left pixels de la gauche de l'ecran	*/

void auto_scrool(struct map * map, short pos_x, short pos_y, short size_x, short size_y, short limit_up, short limit_down, short limit_right, short limit_left)
{
	// test gauche
	if((pos_x - map->scrool_x) < limit_left)
	{
		map->scrool_x -= limit_left - (pos_x - map->scrool_x) ;
		if(map->scrool_x < 0) map->scrool_x=0 ;
	}
	else // test droite
	{
		if(((pos_x+size_x) - map->scrool_x) > (320-limit_right))
		{
			map->scrool_x += ((pos_x+size_x) - map->scrool_x) - (320-limit_right) ;
			if(map->scrool_x > map->map_x_max) map->scrool_x = map->map_x_max ;
		}
	}

	// test haut
	if((pos_y - map->scrool_y) < limit_up)
	{
		map->scrool_y -= limit_up - (pos_y - map->scrool_y) ;
		if(map->scrool_y < 0) map->scrool_y=0 ;
	}
	else // test bas
	{
		if(((pos_y+size_y) - map->scrool_y) > (240-limit_down))
		{
			map->scrool_y += ((pos_y+size_y) - map->scrool_y) - (240-limit_down) ;
			if(map->scrool_y > map->map_y_max) map->scrool_y = map->map_y_max ;
		}
	}
}

//-----------------------------------------------------------------------

void folow_gfx(struct map * map, struct gfx * gfx, short limit_up, short limit_down, short limit_right, short limit_left)
{
	auto_scrool(map,gfx->pos_x,gfx->pos_y,gfx->size_x,gfx->size_y,limit_up,limit_down,limit_right,limit_left) ;
}

//-----------------------------------------------------------------------

void get_start_pos_of_a_tile(struct map * map,unsigned short x,unsigned short y,short *startx,short *starty)
{
	short tmpx, tmpy ;
	char decalage_x, decalage_y ;
	char dec_x = get_decalage(map->size_tile_x) ;
	char dec_y = get_decalage(map->size_tile_y) ;

	decalage_x = ((((map->scrool_x >> dec_x) + 1) << dec_x)) - map->scrool_x ;
	decalage_y = ((((map->scrool_y >> dec_y) + 1) << dec_y)) - map->scrool_y ;
	
	tmpx = x - decalage_x  ;
	*startx = (x - (tmpx - ((tmpx>>dec_x)<<dec_x))) + map->scrool_x ;
	
	tmpy = y - decalage_y ;
	*starty = (y - (tmpy - ((tmpy>>dec_y)<<dec_y))) + map->scrool_y ;
}

unsigned short pixel_test_gfx_on_map(struct map * map, struct gfx * gfx,unsigned short gx, unsigned short gy, char anim, char count, unsigned short trace, unsigned short transp)
{
	struct gfx temp ;
	short startx, starty ;
	unsigned short x = gx+gfx->pos_x - map->scrool_x ;
	unsigned short y = gy+gfx->pos_y - map->scrool_y ;
	//char buffer[20] ;

	char tile_number = get_tile(map,x,y) ;
	
	if(!tile_number) return 0 ;
	
	get_start_pos_of_a_tile(map,x,y,&startx,&starty) ;

	temp.pos_x = startx ;
	temp.pos_y = starty ;
	temp.size_x = map->size_tile_x ;
	temp.size_y = map->size_tile_y ;
	temp.animation[0].curent_frame = 0 ;
	
	if(map->tab_tiles[tile_number].animated)	temp.img[0] = map->tab_tiles[tile_number].img[map->tab_tiles[tile_number].anim_id] ;
	else						temp.img[0] = map->tab_tiles[tile_number].img[0] ;
	
	temp.transp_color = transp ;

	/*GPSPRINTF(buffer,"%i|%i",startx,starty) ;
 	
	if(bbp==16) GpTextOut16(NULL, &gpDraw[nflip],10,10,buffer,0xB3);
	else GpTextOut(NULL, &gpDraw[nflip],10,10,buffer,0xB3);*/
	
	return test_colision_pixel(gfx,&temp,anim,0,count,trace) ;	
}