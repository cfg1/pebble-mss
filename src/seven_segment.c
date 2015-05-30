#include <pebble.h>
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
  
  
void seven_segment_paint_0(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_5(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_5(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_1(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_6(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_6(ctx);
  }
}

void seven_segment_paint_2(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_5(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_5(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_3(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_4(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_6(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_6(ctx);
  }
}

void seven_segment_paint_5(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_6(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_5(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_5(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_7(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_6(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_6(ctx);
  }
}

void seven_segment_paint_8(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_5(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_5(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_paint_9(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_paint_segment_1(ctx);
    seven_segment_15_paint_segment_2(ctx);
    seven_segment_15_paint_segment_3(ctx);
    seven_segment_15_paint_segment_4(ctx);
    seven_segment_15_paint_segment_6(ctx);
    seven_segment_15_paint_segment_7(ctx);
  }
  if (size == 41){
    seven_segment_41_paint_segment_1(ctx);
    seven_segment_41_paint_segment_2(ctx);
    seven_segment_41_paint_segment_3(ctx);
    seven_segment_41_paint_segment_4(ctx);
    seven_segment_41_paint_segment_6(ctx);
    seven_segment_41_paint_segment_7(ctx);
  }
}

void seven_segment_clear(GContext* ctx, int size){
  if (size == 15){
    seven_segment_15_clear(ctx);
  }
  if (size == 41){
    seven_segment_41_clear(ctx);
  }
}


void seven_segment_15_paint_segment_1(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 1, 0), GPoint( 8, 0));
  graphics_draw_line(ctx, GPoint( 2, 1), GPoint( 7, 1));
}

void seven_segment_15_paint_segment_2(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 0, 1), GPoint( 0, 6));
  graphics_draw_line(ctx, GPoint( 1, 2), GPoint( 1, 5));
}

void seven_segment_15_paint_segment_3(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 9, 1), GPoint( 9, 6));
  graphics_draw_line(ctx, GPoint( 8, 2), GPoint( 8, 5));
}

void seven_segment_15_paint_segment_4(GContext* ctx) { // TODO: test coordinates
  graphics_draw_line(ctx, GPoint( 2, 6), GPoint( 7, 6));
  graphics_draw_line(ctx, GPoint( 1, 7), GPoint( 8, 7));
}

void seven_segment_15_paint_segment_5(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 0, 8), GPoint( 0,13));
  graphics_draw_line(ctx, GPoint( 1, 9), GPoint( 1,12));
}

void seven_segment_15_paint_segment_6(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 9, 8), GPoint( 9,13));
  graphics_draw_line(ctx, GPoint( 8, 9), GPoint( 8,12));
}

void seven_segment_15_paint_segment_7(GContext* ctx) {
  graphics_draw_line(ctx, GPoint( 1,14), GPoint( 8,14));
  graphics_draw_line(ctx, GPoint( 2,13), GPoint( 7,13));
}

void seven_segment_15_clear(GContext* ctx){
  graphics_fill_rect(ctx, GRect(0,0,10,15), 0, 0);
}


void seven_segment_41_paint_segment_1(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(02,00), GPoint(23,00));
  graphics_draw_line(ctx, GPoint(03,01), GPoint(22,01));
  graphics_draw_line(ctx, GPoint(04,02), GPoint(21,02));
  graphics_draw_line(ctx, GPoint(05,03), GPoint(20,03));
  graphics_draw_line(ctx, GPoint(06,04), GPoint(19,04));
  graphics_draw_line(ctx, GPoint(07,05), GPoint(18,05));
}

void seven_segment_41_paint_segment_2(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(00,02), GPoint(00,17));
  graphics_draw_line(ctx, GPoint(01,03), GPoint(01,18));
  graphics_draw_line(ctx, GPoint(02,04), GPoint(02,19));
  graphics_draw_line(ctx, GPoint(03,05), GPoint(03,18));
  graphics_draw_line(ctx, GPoint(04,06), GPoint(04,17));
  graphics_draw_line(ctx, GPoint(05,07), GPoint(05,16));
}

void seven_segment_41_paint_segment_3(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(25,02), GPoint(25,17));
  graphics_draw_line(ctx, GPoint(24,03), GPoint(24,18));
  graphics_draw_line(ctx, GPoint(23,04), GPoint(23,19));
  graphics_draw_line(ctx, GPoint(22,05), GPoint(22,18));
  graphics_draw_line(ctx, GPoint(21,06), GPoint(21,17));
  graphics_draw_line(ctx, GPoint(20,07), GPoint(20,16));
}

void seven_segment_41_paint_segment_4(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(04,20), GPoint(21,20));
  graphics_draw_line(ctx, GPoint(04,21), GPoint(21,21));
  graphics_draw_line(ctx, GPoint(05,19), GPoint(20,19));
  graphics_draw_line(ctx, GPoint(06,18), GPoint(19,18));
  graphics_draw_line(ctx, GPoint(05,22), GPoint(20,22));
  graphics_draw_line(ctx, GPoint(06,23), GPoint(19,23));
}

void seven_segment_41_paint_segment_5(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(00,38), GPoint(00,24));
  graphics_draw_line(ctx, GPoint(01,37), GPoint(01,23));
  graphics_draw_line(ctx, GPoint(02,36), GPoint(02,22));
  graphics_draw_line(ctx, GPoint(03,35), GPoint(03,23));
  graphics_draw_line(ctx, GPoint(04,34), GPoint(04,24));
  graphics_draw_line(ctx, GPoint(05,33), GPoint(05,25));
}

void seven_segment_41_paint_segment_6(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(25,38), GPoint(25,24));
  graphics_draw_line(ctx, GPoint(24,37), GPoint(24,23));
  graphics_draw_line(ctx, GPoint(23,36), GPoint(23,22));
  graphics_draw_line(ctx, GPoint(22,35), GPoint(22,23));
  graphics_draw_line(ctx, GPoint(21,34), GPoint(21,24));
  graphics_draw_line(ctx, GPoint(20,33), GPoint(20,25));
}

void seven_segment_41_paint_segment_7(GContext* ctx) {
  graphics_draw_line(ctx, GPoint(02,40), GPoint(23,40));
  graphics_draw_line(ctx, GPoint(03,39), GPoint(22,39));
  graphics_draw_line(ctx, GPoint(04,38), GPoint(21,38));
  graphics_draw_line(ctx, GPoint(05,37), GPoint(20,37));
  graphics_draw_line(ctx, GPoint(06,36), GPoint(19,36));
  graphics_draw_line(ctx, GPoint(07,35), GPoint(18,35));
}

void seven_segment_41_clear(GContext* ctx){
  graphics_fill_rect(ctx, GRect(0,0,26,41), 0, 0);
}

