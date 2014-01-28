// key detection function

char up_front(unsigned short key)
{
	if(!(old_key&key))
		if(key_pressed&key)
			return 1 ;
			
	return 0 ;
}

char down_front(unsigned short key)
{
	if(old_key&key)
		if(!(key_pressed&key))
			return 1 ;
			
	return 0 ;
}

char continu_key(unsigned short key)
{
	if(old_key&key)
		if(key_pressed&key)
			return 1 ;
			
	return 0 ;
}

char instant_key(unsigned short key)
{
	if(key_pressed&key)	return 1 ;

	return 0 ;
}

void refresh_key(void)
{
	old_key = key_pressed ;
	GpKeyGetEx(&key_pressed);
}

void wait_key(unsigned short key)
{
	while(!(key_pressed&key)) refresh_key() ;
}