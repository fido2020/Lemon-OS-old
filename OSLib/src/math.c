/*static const int hextable[] = {
   [0 ... 255] = -1,
   ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
   ['A'] = 10, 11, 12, 13, 14, 15,
   ['a'] = 10, 11, 12, 13, 14, 15
};

int hextodec(unsigned const char *hex) {
   int dec = 0; 
   while (*hex && dec >= 0) {
      dec = (dec << 4) | hextable[*hex++];
   }
   return dec; 
}*/

int hextobin(unsigned const char *hex){
	return 1;//dectobin(hextodec(hex));
}

int dectobin(int num){
	return num;
}

int power(int num, int power){
	int final = num;
	for(int i=1;i<=power;i++){
		final = final * num;
	}
	return final;
}