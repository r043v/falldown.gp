
#define _max_image_number	100	// size of gfx image bank
#define _max_anim_number	30	// number of anim by gfx object
#define _max_custom_short_var	10	// number of custom short var by object
#define _max_custom_long_var	5	// number of custom long var by object
#define _max_tile_anim_size	10	// max size of anim for the tiles
#define _max_tile_number	300	// max tab tile lenth for map


struct anim // animation : 30 anim max par gfx , au max 100 img ds un gfx
{
	unsigned char curent_frame ;		// position actuelle ds l'anim
	unsigned char frame_start ;		// premiere image de l'anim (parmi les 100 de l'objet gfx)
	unsigned char frame_stop ;		// derniere image de l'anim
	unsigned short time_anim ;	// temps en miliseconde entre chaque changement de frame
	unsigned char finish ;			// 0 : anim en cour   1 : anim terminé
	
	unsigned char repeat ; // 0=123 1=123212.. 2=123123.. 3=123333.. 4=123111..
	unsigned char sens ; // 0=123 1=321
	
	long last_aff_time ;		// moment du denier changement de frame
} ;


struct gfx
{
	// info image
	unsigned char *img[_max_image_number] ;	// tableau de pointeur sur les images (100 images max)
	unsigned char transp[_max_image_number] ;
	short move[_max_image_number][2] ;
	
	short real_size_x[_max_image_number], real_size_y[_max_image_number], real_pos_x[_max_image_number], real_pos_y[_max_image_number] ;
	
	unsigned char moving ;
	
	unsigned short size_x, size_y ;	// tailles x et y de l'objet
	int pos_x, pos_y ;		// position x et y de l'objet a l'ecran
	unsigned short transp_color ;	// couleur de transpartence de l'objet
	
	struct anim animation[_max_anim_number] ;	// les animations
	
	// variable non utilisée, pour l'utilisateur, utilisez des #define c bc mieux :) ex : #define enabled custom_short_var[0]
	long custom_var[_max_custom_long_var] ;
	short custom_short_var[_max_custom_short_var] ;
} ;

struct tile
{
	unsigned char * img[_max_custom_short_var] ; // anim de 10 image max pour les tiles

	unsigned char anim_id ;		// position actuelle ds l'anim du tile
	unsigned char nb_tiles ;	// nb d'image ds l'anim du tile
	unsigned char animated ;	// 1 : le tile est animé  0 : tile fixe
	unsigned char sens_anim ;	// 0 : 123  1 : 321
	unsigned char type_anim ;	// 0 : 123123...  1 : 123333... 2 : 123111... 3 : 1232123...
	unsigned short time_anim ;	// temps en millisecondes entre chaque images de l'anim
	
	unsigned char transp ;		// 1 : tile transparent  0 : non
	
	long last_time ;	// moment du denier changement de frame
	
	unsigned char walkable ;
	
	short custom_short_var[_max_custom_short_var] ;
} ;

struct map
{
	unsigned short size_x ;
	unsigned short size_y ;

	unsigned char size_tile_x ;
	unsigned char size_tile_y ;

	unsigned int map_x_max ; // size_x*size_tile_x - 320
	unsigned int map_y_max ; // size_y*size_tile_x - 240

	int scrool_x ;
	int scrool_y ;
	
	unsigned char nb_tile_draw_x ;
	unsigned char nb_tile_draw_y ;
	
	struct tile tab_tiles[_max_tile_number] ;
	unsigned short transp_color ;
	
	char transp_map ;
	
	short * map ;
	
	short custom_short_var[_max_custom_short_var] ;
} ;