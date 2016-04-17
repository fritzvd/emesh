void four_Sides (void) {
  if (X1 < (255 - 3)) {
    X1_Left_Side = X1 + 3;
  } else {
    X1_Left_Side = 255;
  }

  if (X1 < (255 - 12)) {
    X1_Right_Side = X1 + 12;
  } else {
    X1_Right_Side = 255;
  }
  Y1_Top = Y1;

  if (Y1 < (255 - 15)) {
    Y1_Bottom = Y1 + 15;
  } else {
    Y1_Bottom = 255;
  }
}

void collide_Check_LR (void) {
  four_Sides();

  if ((joypad1 & RIGHT) > 0) {
    corner = ((X1_Right_Side & 0xf0) >> 4) + (Y1_Top & 0xf0);
    if (C_MAP[corner] > 0)
      X1 = (X1 & 0xf0) + 3;

    corner = ((X1_Right_Side & 0xf0) >> 4) + (Y1_Bottom & 0xf0);
    if (C_MAP[corner] > 0)
      X1 = (X1 & 0xf0) + 3;
  }

  else if ((joypad1 & LEFT) > 0) {
    corner = ((X1_Left_Side & 0xf0) >> 4) + (Y1_Top & 0xf0);
    if (C_MAP[corner] > 0)
      X1 = (X1 & 0xf0) + 13;
  }
}


void collide_Check_UD(void){
	four_Sides();//recheck these
	if ((joypad1 & DOWN) > 0){ //down first
		corner = ((X1_Right_Side & 0xf0) >> 4) + (Y1_Bottom & 0xf0); //bottom right
		if (C_MAP[corner] > 0)
			Y1 = (Y1 & 0xf0); //if collision, realign

		corner = ((X1_Left_Side & 0xf0) >> 4) + (Y1_Bottom & 0xf0); //bottom left
		if (C_MAP[corner] > 0)
			Y1 = (Y1 & 0xf0); //if collision, realign
	}
	else if ((joypad1 & UP) > 0) { //or up
		corner = ((X1_Right_Side & 0xf0) >> 4) + (Y1_Top & 0xf0); //top right
		if (C_MAP[corner] > 0)
			Y1 = (Y1 + 15 & 0xf0); //if collision, realign

		corner = ((X1_Left_Side & 0xf0) >> 4) + (Y1_Top & 0xf0); //top left
		if (C_MAP[corner] > 0)
			Y1 = ((Y1 + 15) & 0xf0); //if collision, realign
	}
}
