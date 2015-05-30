#include <pebble.h>
#include "mooncalc.h"
//#include "config.h"
  
/** 
 *  Given a presumably UTC time, return the astronomical julian day.
 *  This is not day-of-year, but a much larger value.
 * 
 *  @param timeUTC UTC time, unpacked.
 */ 
int tm2jd(struct tm *timeUTC)
{
   int y, m, d, a, b, c, e, f;
   y = timeUTC->tm_year + 1900;
   m = timeUTC->tm_mon + 1;
   d = timeUTC->tm_mday;
   if (m < 3)
   {
      m += 12;
      y -= 1;
   }
   a = y / 100;
   b = a / 4;
   c = 2 - a + b;
   e = 365.25 * (y + 4716);
   f = 30.6001 * (m + 1);
   return c + d + e + f - 1524;
}


int moon_phase(int jdn)
{
   double jd;
   jd = jdn - 2451550.1;
   jd /= 29.530588853;
   jd -= (int)jd;
   return (int)(jd * 27 + 0.5); // scale fraction from 0-27 and round by adding 0.5
}

int calc_moonphase_number(double latitude)
{
  // moon
  int moonphase_number = 0;
  time_t timeNow;
  timeNow = time(NULL);
  moonphase_number = moon_phase(tm2jd(gmtime(&timeNow)));
  // correct for southern hemisphere
  if ((moonphase_number > 0) && (latitude < 0))
    moonphase_number = 28 - moonphase_number;
  
  return moonphase_number;
  
  //static int i = 0;
  //i++;
  //if (i > 27) i = 0;
  //return i;
}

int moonphase_char_number(int moonphase_number)
{

  //static char moon[] = "m";
  int char_number = 0;
  
  // select correct font char
  if (moonphase_number == 14)
  {
    char_number = 48;
  } else if (moonphase_number == 0)
  {
    char_number = 49;
  } else if (moonphase_number < 14)
  {
    char_number = moonphase_number + 96;
  } else
  {
    char_number = moonphase_number + 95;
  }
  
  return char_number;
}



