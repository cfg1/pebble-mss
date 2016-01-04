//#include <pebble.h>
#include "seven_segment.h"
  
  
/*
    ----   <-- 1
  |      | <-- 3
  | <-- 2|
    ----   <-- 4
  | <-- 5|
  |      | <-- 6
    ----   <-- 7
*/
  
  
void seven_segment_paint_0(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_5(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_5(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_1(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
  }
}

void seven_segment_paint_2(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_5(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_5(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_3(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_4(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
  }
}

void seven_segment_paint_5(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_6(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_5(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_5(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_7(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
  }
}

void seven_segment_paint_8(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_5(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_5(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_paint_9(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx, pos);
    seven_segment_15_paint_segment_2(ctx, pos);
    seven_segment_15_paint_segment_3(ctx, pos);
    seven_segment_15_paint_segment_4(ctx, pos);
    seven_segment_15_paint_segment_6(ctx, pos);
    seven_segment_15_paint_segment_7(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx, pos);
    seven_segment_41_paint_segment_2(ctx, pos);
    seven_segment_41_paint_segment_3(ctx, pos);
    seven_segment_41_paint_segment_4(ctx, pos);
    seven_segment_41_paint_segment_6(ctx, pos);
    seven_segment_41_paint_segment_7(ctx, pos);
  }
}

void seven_segment_clear(GContext* ctx, int size, GPoint pos){
  if (size == 15){
    seven_segment_15_clear(ctx, pos);
  }
  if (size == 41){
    seven_segment_41_clear(ctx, pos);
  }
}


void seven_segment_15_paint_segment_1(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 1+pos.x, 0+pos.y), GPoint( 8+pos.x, 0+pos.y));
  graphics_draw_line(ctx, GPoint( 2+pos.x, 1+pos.y), GPoint( 7+pos.x, 1+pos.y));
}

void seven_segment_15_paint_segment_2(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 0+pos.x, 1+pos.y), GPoint( 0+pos.x, 6+pos.y));
  graphics_draw_line(ctx, GPoint( 1+pos.x, 2+pos.y), GPoint( 1+pos.x, 5+pos.y));
}

void seven_segment_15_paint_segment_3(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 9+pos.x, 1+pos.y), GPoint( 9+pos.x, 6+pos.y));
  graphics_draw_line(ctx, GPoint( 8+pos.x, 2+pos.y), GPoint( 8+pos.x, 5+pos.y));
}

void seven_segment_15_paint_segment_4(GContext* ctx, GPoint pos) { // TODO: test coordinates
  graphics_draw_line(ctx, GPoint( 2+pos.x, 6+pos.y), GPoint( 7+pos.x, 6+pos.y));
  graphics_draw_line(ctx, GPoint( 1+pos.x, 7+pos.y), GPoint( 8+pos.x, 7+pos.y));
}

void seven_segment_15_paint_segment_5(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 0+pos.x, 8+pos.y), GPoint( 0+pos.x,13+pos.y));
  graphics_draw_line(ctx, GPoint( 1+pos.x, 9+pos.y), GPoint( 1+pos.x,12+pos.y));
}

void seven_segment_15_paint_segment_6(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 9+pos.x, 8+pos.y), GPoint( 9+pos.x,13+pos.y));
  graphics_draw_line(ctx, GPoint( 8+pos.x, 9+pos.y), GPoint( 8+pos.x,12+pos.y));
}

void seven_segment_15_paint_segment_7(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint( 1+pos.x,14+pos.y), GPoint( 8+pos.x,14+pos.y));
  graphics_draw_line(ctx, GPoint( 2+pos.x,13+pos.y), GPoint( 7+pos.x,13+pos.y));
}

void seven_segment_15_clear(GContext* ctx, GPoint pos){
  graphics_fill_rect(ctx, GRect(0+pos.x,0+pos.y,10,15), 0, 0);
}


void seven_segment_41_paint_segment_1(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(02+pos.x,00+pos.y), GPoint(23+pos.x,00+pos.y));
  graphics_draw_line(ctx, GPoint(03+pos.x,01+pos.y), GPoint(22+pos.x,01+pos.y));
  graphics_draw_line(ctx, GPoint(04+pos.x,02+pos.y), GPoint(21+pos.x,02+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,03+pos.y), GPoint(20+pos.x,03+pos.y));
  graphics_draw_line(ctx, GPoint(06+pos.x,04+pos.y), GPoint(19+pos.x,04+pos.y));
  graphics_draw_line(ctx, GPoint(07+pos.x,05+pos.y), GPoint(18+pos.x,05+pos.y));
}

void seven_segment_41_paint_segment_2(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(00+pos.x,02+pos.y), GPoint(00+pos.x,17+pos.y));
  graphics_draw_line(ctx, GPoint(01+pos.x,03+pos.y), GPoint(01+pos.x,18+pos.y));
  graphics_draw_line(ctx, GPoint(02+pos.x,04+pos.y), GPoint(02+pos.x,19+pos.y));
  graphics_draw_line(ctx, GPoint(03+pos.x,05+pos.y), GPoint(03+pos.x,18+pos.y));
  graphics_draw_line(ctx, GPoint(04+pos.x,06+pos.y), GPoint(04+pos.x,17+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,07+pos.y), GPoint(05+pos.x,16+pos.y));
}

void seven_segment_41_paint_segment_3(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(25+pos.x,02+pos.y), GPoint(25+pos.x,17+pos.y));
  graphics_draw_line(ctx, GPoint(24+pos.x,03+pos.y), GPoint(24+pos.x,18+pos.y));
  graphics_draw_line(ctx, GPoint(23+pos.x,04+pos.y), GPoint(23+pos.x,19+pos.y));
  graphics_draw_line(ctx, GPoint(22+pos.x,05+pos.y), GPoint(22+pos.x,18+pos.y));
  graphics_draw_line(ctx, GPoint(21+pos.x,06+pos.y), GPoint(21+pos.x,17+pos.y));
  graphics_draw_line(ctx, GPoint(20+pos.x,07+pos.y), GPoint(20+pos.x,16+pos.y));
}

void seven_segment_41_paint_segment_4(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(04+pos.x,20+pos.y), GPoint(21+pos.x,20+pos.y));
  graphics_draw_line(ctx, GPoint(04+pos.x,21+pos.y), GPoint(21+pos.x,21+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,19+pos.y), GPoint(20+pos.x,19+pos.y));
  graphics_draw_line(ctx, GPoint(06+pos.x,18+pos.y), GPoint(19+pos.x,18+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,22+pos.y), GPoint(20+pos.x,22+pos.y));
  graphics_draw_line(ctx, GPoint(06+pos.x,23+pos.y), GPoint(19+pos.x,23+pos.y));
}

void seven_segment_41_paint_segment_5(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(00+pos.x,38+pos.y), GPoint(00+pos.x,24+pos.y));
  graphics_draw_line(ctx, GPoint(01+pos.x,37+pos.y), GPoint(01+pos.x,23+pos.y));
  graphics_draw_line(ctx, GPoint(02+pos.x,36+pos.y), GPoint(02+pos.x,22+pos.y));
  graphics_draw_line(ctx, GPoint(03+pos.x,35+pos.y), GPoint(03+pos.x,23+pos.y));
  graphics_draw_line(ctx, GPoint(04+pos.x,34+pos.y), GPoint(04+pos.x,24+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,33+pos.y), GPoint(05+pos.x,25+pos.y));
}

void seven_segment_41_paint_segment_6(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(25+pos.x,38+pos.y), GPoint(25+pos.x,24+pos.y));
  graphics_draw_line(ctx, GPoint(24+pos.x,37+pos.y), GPoint(24+pos.x,23+pos.y));
  graphics_draw_line(ctx, GPoint(23+pos.x,36+pos.y), GPoint(23+pos.x,22+pos.y));
  graphics_draw_line(ctx, GPoint(22+pos.x,35+pos.y), GPoint(22+pos.x,23+pos.y));
  graphics_draw_line(ctx, GPoint(21+pos.x,34+pos.y), GPoint(21+pos.x,24+pos.y));
  graphics_draw_line(ctx, GPoint(20+pos.x,33+pos.y), GPoint(20+pos.x,25+pos.y));
}

void seven_segment_41_paint_segment_7(GContext* ctx, GPoint pos) {
  graphics_draw_line(ctx, GPoint(02+pos.x,40+pos.y), GPoint(23+pos.x,40+pos.y));
  graphics_draw_line(ctx, GPoint(03+pos.x,39+pos.y), GPoint(22+pos.x,39+pos.y));
  graphics_draw_line(ctx, GPoint(04+pos.x,38+pos.y), GPoint(21+pos.x,38+pos.y));
  graphics_draw_line(ctx, GPoint(05+pos.x,37+pos.y), GPoint(20+pos.x,37+pos.y));
  graphics_draw_line(ctx, GPoint(06+pos.x,36+pos.y), GPoint(19+pos.x,36+pos.y));
  graphics_draw_line(ctx, GPoint(07+pos.x,35+pos.y), GPoint(18+pos.x,35+pos.y));
}

void seven_segment_41_clear(GContext* ctx, GPoint pos){
  graphics_fill_rect(ctx, GRect(0+pos.x,0+pos.y,26,41), 0, 0);
}

