/* https://github.com/amarillys */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	/*if(argc < 2){
		printf("usage:extract <filename>");
		return 1;
	}*/
	FILE *fp;
	fp = fopen(argv[1],"rb");	
	int size,count,i;
	fseek(fp,0l,SEEK_END);
	if((size = ftell(fp)) == 49152){
		fclose(fp);remove(argv[1]);return 2;}
			
	fseek(fp,0l,SEEK_SET);
	if((fgetc(fp)) == 161){
		fclose(fp);
		char tmp[30];
		strcpy(tmp,argv[1]);
		strcat(tmp,".txt");
		rename(argv[1],tmp);
		return 0;
	}
	for(count = 0,i = 0;i < size; i++)
		if(fgetc(fp) == 94)
			count++;
	if(count * 200 < size){
		fclose(fp);
		remove(argv[1]);
	}else{
		char tmp[30] = {"NOT "};
		strcat(tmp,argv[1]);
		system(tmp);
		fclose(fp);
	}
	return 0;
}
