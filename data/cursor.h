/* GIMP RGBA C-Source image dump (cursor.c) */

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[12 * 20 * 4 + 1];
} cursor_image = {
  12, 20, 4,
  "\0\0\0U\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\34\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\342\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\377UUU\377\0\0\0\252\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\34\34\34\377\0\0"
  "\0U\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\377\377\377\377\377\342\342\342\377\0\0\0\377\0\0\0\34\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\377\252\252\252\377\0\0\0\342\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377UUU\377\0\0\0\252\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\34"
  "\34\34\377\0\0\0U\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\342\342\342\377"
  "\0\0\0\377\0\0\0\34\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\252"
  "\252\252\377\0\0\0\342\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377UUU\377\0\0\0\252\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\34\34\34\377\0\0\0U\0\0\0\0\0\0\0\0\0\0"
  "\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\342\342\342\377\0\0\0\377\0"
  "\0\0\34\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\252\252\252\377\0\0\0\342\0\0\0\0\0\0\0\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\342\342\342\377\215\215\215\377888\377\0\0\0\377\0\0\0\252\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\342\342\342\377qqq\3778"
  "88\377\0\0\0\377\0\0\0\377\0\0\0\342\0\0\0\215\0\0\0""8\0\0\0\377\342\342"
  "\342\377\215\215\215\377888\377\0\0\0\377\0\0\0\377\0\0\0\342\0\0\0\215\0"
  "\0\0""8\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\342\0"
  "\0\0\215\0\0\0""8\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\342"
  "\0\0\0\215\0\0\0""8\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0",
};
