const unsigned char MetaSprite_Y[] = {
  0, 0, 8, 8
};

const unsigned char MetaSprite_Tile[] = {
  2, 3, 0x12, 0x13,
  0, 1, 0x10, 0x11,
  6, 7, 0x16, 0x17,
  4, 5, 0x14, 0x15
};

const unsigned char MetaSprite_Attrib[] = {
  0, 0, 0, 0
};

const unsigned char MetaSprite_X[] = {
  0, 8, 0, 8
};

void update_Sprites (void) {
  state4 = state << 2;
  index4 = 0;

  for (index = 0; index < 4; ++index) {
    SPRITES[index4] = MetaSprite_Y[index] + Y1;
    ++index4;
    SPRITES[index4] = MetaSprite_Tile[index + state4];
    ++index4;
    SPRITES[index4] = MetaSprite_Attrib[index];
    ++index4;
    SPRITES[index4] = MetaSprite_X[index] + X1;
    ++index4;
  }
}
