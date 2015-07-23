/* https://github.com/amarillys */
#include <stdio.h>

int main(int argc,char *argv[]){
	if(argc < 2){
		printf("usage:NOT <filename>\n");
		return 1;
	}
	FILE *fp_i,*fp_o;
	fp_i = fopen(argv[1],"rb");
	strcat(argv[1],".out");
	fp_o = fopen(argv[1],"wb");
	int ch;
	int f_size;
	int i;
	i = 0;
	fseek(fp_i,0L,SEEK_END);
	f_size = ftell(fp_i);
	fseek(fp_i,0L,SEEK_SET);
	while(i < f_size){
		fputc(~fgetc(fp_i),fp_o);
		i++;
	}
	fclose(fp_i);
	fclose(fp_o);
	return 0;
}
