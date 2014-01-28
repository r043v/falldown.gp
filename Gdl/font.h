/*************************************

	     font.h
	
	a part of gfxlib
	
    font functions version 0.1

*************************************/



// l'objet fonte
struct font
{
	unsigned char *img[0xFF] ;
	unsigned char size_x, size_y ;
	unsigned short transp_color ;
} ;

// charge l'image des char d'une fonte ds un objet fonte
void load_font(struct font * font, unsigned char * img_font, unsigned char sx, unsigned char sy, unsigned short tr_color, unsigned char nb_char)
{
	unsigned char cpt ;

	font->size_x = sx ;
	font->size_y = sy ;
	font->transp_color = tr_color ;

	for(cpt=0;cpt<nb_char;cpt++)
	{
		font->img[cpt+32] = img_font + (cpt*sx*sy*(bbp/8)) ;
	} ;
}

// affiche du txt a une certaine position de l'ecran
void draw_text(struct font * font,char * text, unsigned short x, unsigned short y)
{
	unsigned short cpt;
	unsigned char nb_char = GPSTRLEN(text) ;
	
	for(cpt=0;cpt<nb_char;cpt++)
	{	
		afficher_sprite_cut(x + cpt*font->size_x,y,font->size_x,font->size_y,font->img[text[cpt]],0,font->transp_color) ;
	} ;
}

// affiche du txt, la fin du txt se trouvera a la position specifié (alignement a droite)
void draw_text_rl(struct font * font,char * text, unsigned short x, unsigned short y)
{
	unsigned short cpt;
	unsigned char nb_char = GPSTRLEN(text) ;
	
	unsigned short start_x = x - font->size_x*nb_char ;
	
	for(cpt=0;cpt<nb_char;cpt++)
	{	
		afficher_sprite_cut(start_x + cpt*font->size_x,y,font->size_x,font->size_y,font->img[text[cpt]],0,font->transp_color) ;
	} ;
}