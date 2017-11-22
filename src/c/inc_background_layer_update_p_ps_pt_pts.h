// This file is only for the following platforms:
// APLITE (Pebble and Pebble Steel)
// BASALT (Pebble Time and Pebble Time Steel)


  //clear all with background_color_clock:
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_context_set_stroke_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 168), 0, 0);
  
  //draw outlines:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, background_color_lines);
  graphics_draw_line(ctx, GPoint(110,   0), GPoint(110,  15));
  graphics_draw_line(ctx, GPoint(  0,  16), GPoint(168,  16));
  graphics_draw_line(ctx, GPoint( 47,  17), GPoint( 47,  49));
  graphics_draw_line(ctx, GPoint( 85,  17), GPoint( 85,  68));
  graphics_draw_line(ctx, GPoint(  0,  50), GPoint( 84,  50));
  graphics_draw_line(ctx, GPoint(  0,  69), GPoint(168,  69));
  graphics_draw_line(ctx, GPoint(  0, TIME_POS()-4), GPoint(168,  90));
  graphics_draw_line(ctx, GPoint(  0, 154), GPoint(168, 154));
  
  #ifndef PBL_PLATFORM_APLITE
    //draw background areas:
    graphics_context_set_fill_color(ctx, background_color_clock);
    graphics_context_set_stroke_color(ctx, background_color_clock);
    graphics_fill_rect(ctx, GRect(0, 91, 144, 153-91+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_date);
    graphics_context_set_stroke_color(ctx, background_color_date);
    graphics_fill_rect(ctx, GRect(0, 70, 144, 89-70+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_status);
    graphics_context_set_stroke_color(ctx, background_color_status);
    graphics_fill_rect(ctx, GRect(0, 155, 144, 167-155+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_weather);
    graphics_context_set_stroke_color(ctx, background_color_weather);
    graphics_fill_rect(ctx, GRect( 0, 51, 85, 18), 0, 0);
    graphics_fill_rect(ctx, GRect(86, 17, 58, 52), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_moon);
    graphics_context_set_stroke_color(ctx, background_color_moon);
    graphics_fill_rect(ctx, GRect(48, 17, 37, 33), 0, 0);
  
  
    if (warning_color_location == 1){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else if (warning_color_location == 2){
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_context_set_stroke_color(ctx, GColorBlack);
    } else {
      graphics_context_set_fill_color(ctx, background_color_location);
      graphics_context_set_stroke_color(ctx, background_color_location);
    }
    graphics_fill_rect(ctx, GRect(0, 0, 110, 16), 0, 0);
    
    if (warning_color_last_update){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else {
      graphics_context_set_fill_color(ctx, background_color_last_update);
      graphics_context_set_stroke_color(ctx, background_color_last_update);
    }
    graphics_fill_rect(ctx, GRect(111, 0, 33, 16), 0, 0);
  
  #endif
  
  //draw dots of time:
  graphics_context_set_fill_color(ctx, textcolor_clock);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  graphics_fill_rect(ctx, GRect(69, TIME_POS()+6, 7, 7), 0, 0);
  graphics_fill_rect(ctx, GRect(69, TIME_POS()+32, 7, 7), 0, 0);
  
  //draw arrows of sun rise/set:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_sun);
  graphics_draw_line(ctx, GPoint(  3, 157), GPoint(  3, 165));
  graphics_draw_line(ctx, GPoint(  2, 158), GPoint(  4, 158));
  graphics_draw_line(ctx, GPoint(  1, 159), GPoint(  5, 159));
  graphics_draw_line(ctx, GPoint(106, 157), GPoint(106, 165));
  graphics_draw_line(ctx, GPoint(105, 164), GPoint(107, 164));
  graphics_draw_line(ctx, GPoint(104, 163), GPoint(108, 163));
  
  //draw battery:
  graphics_context_set_fill_color(ctx, bkgrcolor_bat);
  graphics_context_set_stroke_color(ctx, bkgrcolor_bat);
  GRect layer_bounds = GRect(0, 17, 84-38+1, 49-17+1);
  graphics_fill_rect(ctx, layer_bounds, 0, 0);
  
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_bat);
  graphics_draw_line(ctx, GPoint( 0+1, 0+2+17), GPoint(41+1, 0+2+17));
  graphics_draw_line(ctx, GPoint( 0+1,14+2+17), GPoint(41+1,14+2+17));
  graphics_draw_line(ctx, GPoint( 0+1, 0+2+17+Y_OFFSET), GPoint( 0+1,14+2+17));
  
  graphics_draw_line(ctx, GPoint(41+1, 0+2+17), GPoint(41+1, 4+2+17));
  graphics_draw_line(ctx, GPoint(41+1,10+2+17), GPoint(41+1,14+2+17));
  graphics_draw_line(ctx, GPoint(43+1, 4+2+17), GPoint(43+1,10+2+17));
  graphics_draw_line(ctx, GPoint(41+1, 4+2+17), GPoint(43+1, 4+2+17));
  graphics_draw_line(ctx, GPoint(41+1,10+2+17), GPoint(43+1,10+2+17));