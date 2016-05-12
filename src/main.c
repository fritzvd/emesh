void __fastcall__ Wait_Vblank(void);

void __fastcall__ UnRLE(int data);

void* __fastcall__ memcpy (void* dest, const void* src, int count);

#define PPU_CTRL		*((unsigned char*)0x2000)
#define PPU_MASK		*((unsigned char*)0x2001)
#define PPU_STATUS		*((unsigned char*)0x2002)
#define SCROLL			*((unsigned char*)0x2005)
#define PPU_ADDRESS		*((unsigned char*)0x2006)
#define PPU_DATA		*((unsigned char*)0x2007)

#define OAM_ADDRESS *((unsigned char*)0x2003)
#define OAM_DMA *((unsigned char*)0x4014)
//Globals

#pragma bss-name(push, "ZEROPAGE")
unsigned char index;
unsigned char NMI_flag;
unsigned char Frame_Count;
unsigned char index4;
unsigned char X1;
unsigned char Y1;
unsigned char state;
unsigned char state4;

unsigned char new_background;
unsigned char which_BGD;
int BGD_address;
unsigned char *Caddress; //Caddress is an int that points to a char
unsigned char X1_Right_Side;	//for collision test
unsigned char X1_Left_Side;
unsigned char Y1_Bottom;
unsigned char Y1_Top;
unsigned char corner;


// needs to be here for ZP. OAM will overflow
#include "inputstate.c"

#pragma bss-name(push, "OAM")
unsigned char SPRITES[256];


#pragma bss-name(push, "MAP")
unsigned char C_MAP[256]; // collision map

/// BG stuff

//generated with NES Screen Tool, with RLE option on
#include "nametable.h" //called n1
// // #include "backgrounds/1.h" //called n2
// #include "backgrounds/2.h" //called n2
// #include "backgrounds/3.h" //called n3
// #include "backgrounds/4.h" //called n4

const int All_Backgrounds[] = {(int) &nameTable};
//the address of each background

//collision data, made by exporting csv from Tiled, and slight modification
#include "backgrounds/1.csv" //called c1
#include "backgrounds/2.csv" //called c2
#include "backgrounds/3.csv" //called c3
#include "backgrounds/4.csv" //called c4

const int All_Collision_Maps[] = {(int) &c1, (int) &c2, (int) &c3, (int) &c4 };

// end bg

#include "palette.c"
#include "helpers.c"
#include "sprite.c"
#include "collision.c"

#define Going_Right 0
#define Going_Down	1
#define Going_Left	2
#define Going_Up	3

void check_Start(void){
	if ( ((joypad1 & START) > 0) && ((joypad1old & START) == 0) ){
		++new_background;
	}
}

void move_logic(void) {
	if ((joypad1 & RIGHT) > 0) {
		state = Going_Right;
		++X1;
	}
	if ((joypad1 & LEFT) > 0) {
		state = Going_Left;
		--X1;
	}

	collide_Check_LR();

	if ((joypad1 & DOWN) > 0) {
		state = Going_Down;
		++Y1;
	}

	if ((joypad1 & UP) > 0) {
		state = Going_Up;
		--Y1;
	}
	collide_Check_UD();
}

void every_frame (void) {
	OAM_ADDRESS = 0;
	OAM_DMA = 2;
	PPU_CTRL = 0x90;
	PPU_MASK = 0x1e; // to be replaced with reset_scroll and all_on?
	SCROLL = 0; // wtf
	SCROLL = 0;
	Get_Input();
}

void Draw_Background(void) {
	All_Off();
	PPU_ADDRESS = 0x20;
	PPU_ADDRESS = 0x00;
	BGD_address = All_Backgrounds[which_BGD];
	UnRLE(BGD_address);	//uncompresses our BG tiles

	//now load the collision map to RAM
	(int)Caddress = All_Collision_Maps[which_BGD];
	memcpy (&C_MAP, Caddress, 240);

	Wait_Vblank();	//don't turn on screen until in v-blank
	All_On();
	++which_BGD;
	if (which_BGD == 0) //shuffles between 0-3
		which_BGD = 0;
}

void main (void) {

	All_Off();

	X1 = 0x7f;
	Y1 = 0x77;
	Load_Palette();
	Reset_Scroll();
	All_On();
	//infinite loop
	while (1) {
		while (NMI_flag == 0);
		NMI_flag = 0;
		if (new_background > 0) {
			// PPU_Update();
			Draw_Background();
			// PPU_flag;
			new_background = 0;
		}
		every_frame();
		move_logic();
		check_Start();
		update_Sprites();
	}
};
