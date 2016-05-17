//this example code shows how to put some text in nametable
//it assumes that you have ASCII-encoded font in the CHR tiles $00-$3f

#include "base/neslib.h"
#include "title.h"
#include "level.h"


#define NTADR(x,y) ((0x2000|((y)<<5)|x))
#define PPU_ATTRIB_TABLE_0  0x23c0 // attribute table for nametable 0
#define PPU_ATTRIB_TABLE_1  0x27c0 // attribute table for nametable 1
#define PPU_ATTRIB_TABLE_2  0x2bc0 // attribute table for nametable 2
#define PPU_ATTRIB_TABLE_3  0x2fc0 // attribute table for nametable 3
#define STRING_OFFSET  0xa0

static unsigned char spr;
static unsigned char currentSlide = 0;

void put_str(unsigned int adr,const char *str)
{
	vram_adr(adr);
	while(1)
	{
		if(!*str) break;

		vram_put((*str++) + STRING_OFFSET);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
	}
}

const unsigned char palSprites[16]={
	0x0d,0x15,0x25,0x25,
	0x0d,0x19,0x29,0x39,
	0x0d,0x11,0x21,0x31,
	0x0d,0x17,0x27,0x37
};

const unsigned char palBG [16]={
	0x0d,0x21,0x26,0x38,
	0x0d,0x14,0x21,0x31,
	0x0d,0x29,0x16,0x26,
	0x0d,0x09,0x19,0x29 };

void make_slide() {
	ppu_off();

	ppu_on_all();
}

void next_slide() {
	// currentSlide++;
	currentSlide = 0;
}

void prev_slide () {
	currentSlide = 0;
}

const unsigned char sprPlayer[]={
	0, 0,0x05,0,
	8, 0,0x20,0,
	0, 7,0x06,0,
	8, 7,0x21,0,
	128
};


static unsigned char i, bg_bright,spr_bright;


void fade_screen_in(void)
{
	for(i=0;i<16;i++)
	{
		ppu_wait_nmi();
		if(!(i&3))
		{
			bg_bright++;
			pal_bg_bright(bg_bright);
			spr_bright++;
			pal_spr_bright(spr_bright);
		}
	}
}

void fade_screen_out(void)
{
	for(i=0;i<16;i++)
	{
		ppu_wait_nmi();
		if(!(i&3)) {
			bg_bright--;
			pal_bg_bright(bg_bright);
			spr_bright--;
			pal_spr_bright(spr_bright);
		}
	}
}

unsigned char screen_state; // 0 == title 1 == game 2 == bla

void start_title_screen () {
	screen_state = 0;
	fade_screen_in();
	ppu_off();
	pal_bg(palBG);
	bank_bg(1);
	vram_adr(NAMETABLE_A);
	vram_unrle(title);
	put_str(NTADR_A(11, 22), "PRESS START");
	ppu_on_all();
}

void start_game_screen () {
	screen_state = 1;
	fade_screen_in();
	ppu_off();
	pal_bg(palBG);
	bank_bg(1);
	vram_adr(NAMETABLE_A);
	vram_unrle(level);
	vram_adr(NAMETABLE_C);
	vram_unrle(title);

	pal_col(1,0x28);//set white color

	bank_spr(0);
	ppu_on_all();
	spr = oam_spr(60, 60, 0x40, 1&3, spr);
}

int x;
int y;

void game_logic () {
	spr = oam_meta_spr(x, y, spr, sprPlayer);
}

void input (void) {
	static unsigned char joy;

	// joy=pad_trigger(0);
	joy=pad_poll(0);

	if (screen_state == 1) {
		if(joy & PAD_UP) {
			y--;
			scroll(x, y);
		}
		if(joy & PAD_DOWN) {
			y++;
			scroll(x, y);
		}

		if(joy & PAD_RIGHT) {
			x++;
			scroll(x, y);
		}
		if(joy & PAD_LEFT) {
			x--;
			scroll(x, y);
		}
	}

	if (screen_state == 0) {
		if (joy & PAD_START) {
			fade_screen_out();
			start_game_screen();
		}
	}
}

void main(void) {

	x = 10;
	y = 80;
	pal_spr(palSprites);

	start_title_screen();
	ppu_on_all();
	// fade_screen_in();



	while(1) {
		ppu_wait_nmi();
		set_vram_update(NULL);//disable update just in case, not really needed in this example

		if (screen_state == 1) {
			game_logic();
		}
		input();
	}
}
