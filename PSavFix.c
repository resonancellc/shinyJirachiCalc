 /*
  * PSavFIX - Copyright (c) 2004 pika - All rights reserved.
  *
  * -----------------------------------------------------------------------------
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  * -----------------------------------------------------------------------------
  *
  * 
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int Chksum(int length, int *Data)
{
	int Chk,i,tmp;
	length = length>>2;
	Chk=0;
	for(i=0; i<length; i++){
		Chk += Data[i];
    }
		
	tmp = Chk>>16;
	tmp +=Chk;

	Chk = (tmp&0xFFFF);

	return Chk;
}


int main(int argc, char** argv)
{


	char *Map,*p,*header;
	int *Imap,*Data;
	int Found,id,fix,CHK,OK,i;
	FILE *fpm, *fp;
	short *MapPtr;

	short FLMAP[] = { 0xF24,0xF80,0xF80,0xF80,0xEC0,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0x7D0,0x01C,0x100};
	short RSMAP[] = { 0x890,0xF80,0xF80,0xF80,0xC40,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0x7D0,0xF80,0xF80};
	short MAX[]   = { 0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80,0xF80};

	OK   = 1;
	fix  = 0;

	if(argc!=4)
	{
		printf("Usage: %s [-fix|-nofix] [-RS|-FL|-MAX] [infile]\n", argv[0]);
		return 0;
	}


	if(!strcmp(argv[2], "-RS"))
	{
		printf("Running in RS Mode\r\n");
		MapPtr = &RSMAP[0];
	}
	else if(!strcmp(argv[2], "-FL"))
	{
		printf("Running in FL Mode\r\n");
		MapPtr = &FLMAP[0];
	}
	else if(!strcmp(argv[2], "-MAX"))
	{
		printf("Running in MAXCompat Mode - note this mode is NOT recommented\r\n");
		MapPtr = &MAX[0];
	}
	else
	{
		printf("Please select a mode (-RS -FL -MAX). Read readme for more Information. \r\n");
		return 0;
	}



	// allocate main ram and load sav
	header = (char *)malloc(0x1000);
	Data = (int*)header;

	printf("Loading: %s...\r\n", argv[3]);
	fp = fopen(argv[3], "rb+");
	if (!fp)
    {
		printf("error loading sav \r\n");
        return 0;
    }	

    
	//check if fix is ON
	if(!strcmp(argv[1], "-fix"))
	{
		printf("Autofix: ON\r\n");
		fix = 1;
	}
	else
		printf("Autofix: OFF\r\n");



	for(i = 0; i<0x20; i++)
	{
		fread(header, 1, 0x1000, fp);
	    
		CHK=Chksum(MapPtr[Data[1021]&0xF], Data);
		Found = (Data[1021]>>16)&0xFFFF;
		id = (Data[1021])&0xFF;

		if((CHK==Found && Data[1022]==0x8012025) || (id==0 && CHK==0 && Found==0))
			printf("Size: %03X Segment: %02X Calc: %04X Found: %04X Sig: %08X - OK\r\n",MapPtr[Data[1021]&0xF],id, CHK,Found, Data[1022]);
		else
		{
			printf("Size: %03X Segment: %02X Calc: %04X Found: %04X Sig: %08X \r\n",MapPtr[Data[1021]&0xF],id, CHK,Found, Data[1022]);
			OK = 0;
			if(fix)
			{
				fseek(fp, -0xa,SEEK_CUR);
				header[0xff6]= (0xFF &CHK);
				header[0xff7]= (0xFF &(CHK>>8));
				Data[1022]=0x8012025;		
				p = &header[0xff6];
				fwrite(p,1,6,fp);
				fseek(fp, 0x4,SEEK_CUR);
			}

		}
	}

	fclose(fp);
    
	if(OK)
		printf("No Problems found :)\r\n");
	else
		printf("Problems found! :( (remember Sig=08012025)\r\n");
	
	return 0;
}
