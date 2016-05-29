

  #ifdef PBL_COLOR
    if (ColorProfile == 1){ //Black on White
      textcolor_clock              = GColorBlack;
      textcolor_seconds            = GColorBlack;
      textcolor_tz                 = GColorBlack;
      textcolor_Steps              = GColorBlack;
      textcolor_cal                = GColorBlack; //calendar week
      background_color_clock       = GColorWhite;
      
      textcolor_date               = GColorBlack;
      background_color_date        = GColorWhite;
      
      textcolor_weather            = GColorBlack;
      background_color_weather     = GColorWhite;
      
      textcolor_moon               = GColorBlack;
      background_color_moon        = GColorWhite;
      
      textcolor_sun                = GColorBlack;
      textcolor_con                = GColorBlack; //connection
      background_color_status      = GColorWhite;
      
      textcolor_location           = GColorBlack;
      background_color_location    = GColorWhite;
      
      textcolor_last_update        = GColorBlack;
      background_color_last_update = GColorWhite;
  
      background_color_lines       = GColorBlack;
    } else if (ColorProfile == 5){ //Black Bkgr. and green clock
      textcolor_clock              = GColorFromHEX(0x00FF00);
      textcolor_seconds            = GColorFromHEX(0x00AAAA);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0x55FF55);
      textcolor_cal                = GColorFromHEX(0x00AAAA);   //=GColorTiffanyBlue //calendar week
      background_color_clock       = GColorBlack;
      
      textcolor_date               = GColorFromHEX(0x00AAAA);   //=GColorTiffanyBlue
      background_color_date        = GColorBlack;
      
      textcolor_weather            = GColorFromHEX(0x00FFAA);   //GColorMediumSpringGreen
      background_color_weather     = GColorBlack;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean  //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorFromHEX(0xFFAA00);   //=GColorChromeYellow
      background_color_location    = GColorFromHEX(0x005500);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005500);
  
      background_color_lines       = GColorWhite;
    } else if (ColorProfile == 13){ //colored high contrast (blue clock on yellow)
      textcolor_clock              = GColorFromRGB(0, 0, 85);
      textcolor_seconds            = GColorFromRGB(0, 170, 170);
      textcolor_tz                 = GColorFromRGB(85, 85, 85); //OK
      textcolor_Steps              = GColorFromHEX(0x000000);
      textcolor_cal                = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue; //calendar week
      background_color_clock       = GColorFromHEX(0xFFFF55);
      
      textcolor_date               = GColorFromRGB(170, 0, 85); //= GColorJazzberryJam; ;
      background_color_date        = GColorFromHEX(0xFFFF00);
      
      textcolor_weather            = GColorBlack;
      background_color_weather     = GColorWhite;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorFromRGB(255, 255, 0);   //=GColorYellow //OK
      textcolor_con                = GColorFromRGB(0, 170, 255);   //GColorVividCerulean //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorBlack;
      background_color_location    = GColorWhite;
      
      textcolor_last_update        = GColorBlack;
      background_color_last_update = GColorWhite;
  
      background_color_lines       = GColorFromRGB(170, 170, 170);
    } else if (ColorProfile == 14){ //TESTING (Blue with colored background)
      textcolor_clock              = GColorFromHEX(0x00FFFF);
      textcolor_seconds            = GColorFromHEX(0xFF5500);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0xFFFFFF);
      textcolor_cal                = GColorFromHEX(0x00AAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000055);
      
      textcolor_date               = GColorFromHEX(0xAAFFFF);
      background_color_date        = GColorFromHEX(0x0000AA);
      
      textcolor_weather            = GColorFromHEX(0x55FF00);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFAA00);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 15){ //TESTING (White with colored background)
      textcolor_clock              = GColorFromHEX(0xFFFFFF);
      textcolor_seconds            = GColorFromHEX(0x00AAFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0xFFFFFF);
      textcolor_cal                = GColorFromHEX(0x00AAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0x000000);
      background_color_date        = GColorFromHEX(0xAAAAAA);
      
      textcolor_weather            = GColorFromHEX(0x55FF00);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x005500);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005500);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);  
    
    } else if (ColorProfile == 6){ //TESTING (Blue with black Bkgr.)
      textcolor_clock              = GColorFromHEX(0x0055FF);
      textcolor_seconds            = GColorFromHEX(0x00AAFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0x5555FF);
      textcolor_cal                = GColorFromHEX(0x00AAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0x0055FF);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0x55FF00);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x00AAFF);
      background_color_location    = GColorFromHEX(0x000055);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0x0000AA);
    } else if (ColorProfile == 4){ //TESTING (Red with black Bkgr.)
      textcolor_clock              = GColorFromHEX(0xFF0000);
      textcolor_seconds            = GColorFromHEX(0xFF5500);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0xFF5555);
      textcolor_cal                = GColorFromHEX(0xAA00AA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0xFF0000);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0xFF5555);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFAAAA);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAA0000);
    } else if (ColorProfile == 7){ //TESTING (Red-Red)
      textcolor_clock              = GColorFromHEX(0xFFFF00);
      textcolor_seconds            = GColorFromHEX(0xFFFF00);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_Steps              = GColorFromHEX(0xFFAAAA);
      textcolor_cal                = GColorFromHEX(0xFFAA55); //calendar week
      background_color_clock       = GColorFromHEX(0x550000);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0xAA0000);
      
      /*
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      */
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 8){ //TESTING (Green-Green)
      textcolor_clock              = GColorFromHEX(0xFFFF00);
      textcolor_seconds            = GColorFromHEX(0xFFFF00);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_Steps              = GColorFromHEX(0xAAFFAA);
      textcolor_cal                = GColorFromHEX(0xAAFF55); //calendar week
      background_color_clock       = GColorFromHEX(0x005500);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x00AA00);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x005500);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005555);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 9){ //TESTING (Blue-Blue)
      textcolor_clock              = GColorFromHEX(0x00FFFF);
      textcolor_seconds            = GColorFromHEX(0x00FFFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0xAAAAFF);
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000055);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x0000AA);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x000055);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005555);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 10){ //TESTING (light red)
      textcolor_clock              = GColorFromHEX(0x550000);
      textcolor_seconds            = GColorFromHEX(0x550000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0x000000);
      textcolor_cal                = GColorFromHEX(0x550000); //calendar week
      background_color_clock       = GColorFromHEX(0xFFAA55);
      
      textcolor_date               = GColorFromHEX(0x550000);
      background_color_date        = GColorFromHEX(0xFFAA55);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xFFFFAA);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0xFFAA00);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0xFFFF00);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 11){ //TESTING (light green)
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0x000000);
      textcolor_cal                = GColorFromHEX(0x005500); //calendar week
      background_color_clock       = GColorFromHEX(0x00FF00);
      
      textcolor_date               = GColorFromHEX(0x005500);
      background_color_date        = GColorFromHEX(0x00FF00);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xAAFFAA);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x00AA00);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0x00FF00);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 12){ //TESTING 10
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_Steps              = GColorFromHEX(0x000000);
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x00AAFF);
      
      textcolor_date               = GColorFromHEX(0x000000);
      background_color_date        = GColorFromHEX(0x00AAFF);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xAAFFFF);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0x00AAFF);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0x55FFFF);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 2){ //TESTING (White on Black with Colors)
      textcolor_clock              = GColorFromHEX(0xFFFFFF);
      textcolor_seconds            = GColorFromHEX(0xFFFFFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorWhite;
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x000000);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x000000);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 3){ //TESTING (Black on White with Colors)
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_Steps              = GColorFromHEX(0x000000);
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0xFFFFFF);
      
      textcolor_date               = GColorFromHEX(0x000000);
      background_color_date        = GColorFromHEX(0xFFFFFF);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xFFFFFF);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0xFFFFFF);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0xFFFFFF);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else { //default = BW
      textcolor_clock              = GColorWhite;
      textcolor_seconds            = GColorWhite;
      textcolor_tz                 = GColorWhite;
      textcolor_Steps              = GColorWhite;
      textcolor_cal                = GColorWhite; //calendar week
      background_color_clock       = GColorBlack;
      
      textcolor_date               = GColorWhite;
      background_color_date        = GColorBlack;
      
      textcolor_weather            = GColorWhite;
      background_color_weather     = GColorBlack;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorWhite;
      textcolor_con                = GColorWhite; //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorWhite;
      background_color_location    = GColorBlack;
      
      textcolor_last_update        = GColorWhite;
      background_color_last_update = GColorBlack;
  
      background_color_lines       = GColorWhite;    
    }
  #endif
