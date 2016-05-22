// This file is only for the following platforms:
// CHALK (Pebble Time Round)


  //clear all with background_color_clock:
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_context_set_stroke_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 180, 180), 0, 0);
  
  //draw outlines:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, background_color_lines);
  //graphics_draw_line(ctx, GPoint(110,   0+Y_OFFSET), GPoint(110,  15+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(  0,  16+Y_OFFSET), GPoint(180,  16+Y_OFFSET));
  graphics_draw_line(ctx, GPoint( 47+X_OFFSET,  17+Y_OFFSET), GPoint( 47+X_OFFSET,  49+Y_OFFSET));
  graphics_draw_line(ctx, GPoint( 85+X_OFFSET,  17+Y_OFFSET), GPoint( 85+X_OFFSET,  68+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(  0,  50+Y_OFFSET), GPoint( 84+X_OFFSET,  50+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(  0,  69+Y_OFFSET), GPoint(180,  69+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(  0,  90+Y_OFFSET), GPoint(180,  90+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(  0, 154+Y_OFFSET), GPoint(180, 154+Y_OFFSET));

  graphics_draw_line(ctx, GPoint( 20,     69+Y_OFFSET), GPoint( 20,     90+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(180-25,  69+Y_OFFSET), GPoint(180-25,  90+Y_OFFSET));
  
  #ifdef PBL_COLOR
    //draw background areas:
    graphics_context_set_fill_color(ctx, background_color_clock);
    graphics_context_set_stroke_color(ctx, background_color_clock);
    graphics_fill_rect(ctx, GRect(0, 91+Y_OFFSET, 180, 153-91+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_date);
    graphics_context_set_stroke_color(ctx, background_color_date);
    graphics_fill_rect(ctx, GRect(20+1, 70+Y_OFFSET, 180-20-25-2, 89-70+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_status);
    graphics_context_set_stroke_color(ctx, background_color_status);
    graphics_fill_rect(ctx, GRect(0, 155+Y_OFFSET, 180, 180-155-Y_OFFSET+1), 0, 0);
    graphics_fill_rect(ctx, GRect(0,  70+Y_OFFSET,  20, 89-70+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_weather);
    graphics_context_set_stroke_color(ctx, background_color_weather);
    graphics_fill_rect(ctx, GRect( 0,          51+Y_OFFSET, 85+X_OFFSET, 18), 0, 0);
    graphics_fill_rect(ctx, GRect(86+X_OFFSET, 17+Y_OFFSET, 58+X_OFFSET, 52), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_moon);
    graphics_context_set_stroke_color(ctx, background_color_moon);
    graphics_fill_rect(ctx, GRect(48+X_OFFSET, 17+Y_OFFSET, 37, 33), 0, 0);
  
  
    if (warning_color_location){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else {
      graphics_context_set_fill_color(ctx, background_color_location);
      graphics_context_set_stroke_color(ctx, background_color_location);
    }
    graphics_fill_rect(ctx, GRect(0, 0, 180, 16+Y_OFFSET), 0, 0);
    
    if (warning_color_last_update){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else {
      graphics_context_set_fill_color(ctx, background_color_last_update);
      graphics_context_set_stroke_color(ctx, background_color_last_update);
    }
    graphics_fill_rect(ctx, GRect(180-25+1, 70+Y_OFFSET, 25-1, 89-70+1), 0, 0);
  
  #endif
  
  //draw dots of time:
  graphics_context_set_fill_color(ctx, textcolor_clock);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  graphics_fill_rect(ctx, GRect(69+X_OFFSET-5, 102+Y_OFFSET, 7, 7), 0, 0);
  graphics_fill_rect(ctx, GRect(69+X_OFFSET-5, 124+Y_OFFSET, 7, 7), 0, 0);
  
  //draw arrows of sun rise/set:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_sun);
  graphics_draw_line(ctx, GPoint(90-2-3, 157+Y_OFFSET), GPoint(90-2-3, 165+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(90-2-2, 158+Y_OFFSET), GPoint(90-2-4, 158+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(90-2-1, 159+Y_OFFSET), GPoint(90-2-5, 159+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(90+2+3, 157+Y_OFFSET), GPoint(90+2+3, 165+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(90+2+2, 164+Y_OFFSET), GPoint(90+2+4, 164+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(90+2+1, 163+Y_OFFSET), GPoint(90+2+5, 163+Y_OFFSET));
  
  //draw battery:
  graphics_context_set_fill_color(ctx, bkgrcolor_bat);
  graphics_context_set_stroke_color(ctx, bkgrcolor_bat);
  GRect layer_bounds = GRect(0, 17+Y_OFFSET, 84-38+1+X_OFFSET, 49-17+1);
  graphics_fill_rect(ctx, layer_bounds, 0, 0);
  
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_bat);
  graphics_draw_line(ctx, GPoint( 0+1+X_OFFSET, 0+2+17+Y_OFFSET), GPoint(41+1+X_OFFSET, 0+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint( 0+1+X_OFFSET,14+2+17+Y_OFFSET), GPoint(41+1+X_OFFSET,14+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint( 0+1+X_OFFSET, 0+2+17+Y_OFFSET), GPoint( 0+1+X_OFFSET,14+2+17+Y_OFFSET));
  
  graphics_draw_line(ctx, GPoint(41+1+X_OFFSET, 0+2+17+Y_OFFSET), GPoint(41+1+X_OFFSET, 4+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(41+1+X_OFFSET,10+2+17+Y_OFFSET), GPoint(41+1+X_OFFSET,14+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(43+1+X_OFFSET, 4+2+17+Y_OFFSET), GPoint(43+1+X_OFFSET,10+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(41+1+X_OFFSET, 4+2+17+Y_OFFSET), GPoint(43+1+X_OFFSET, 4+2+17+Y_OFFSET));
  graphics_draw_line(ctx, GPoint(41+1+X_OFFSET,10+2+17+Y_OFFSET), GPoint(43+1+X_OFFSET,10+2+17+Y_OFFSET));

