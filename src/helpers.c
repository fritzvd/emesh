//turn off the screen
void All_Off (void) {
	PPU_CTRL = 0;
	PPU_MASK = 0;
}

void Load_Palette (void) {
	//load the palette
	PPU_ADDRESS = 0x3f; //set an address in the PPU of 0x3f00
	PPU_ADDRESS = 0x00;
	for(index = 0; index < sizeof(PALETTE); ++index){
		PPU_DATA = PALETTE[index];
	}
}

void Reset_Scroll (void) {
	//reset the scroll position
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;
}

void All_On (void) {
	//turn on screen
	PPU_CTRL = 0x90; //screen is on, NMI on
	PPU_MASK = 0x1e;
}
