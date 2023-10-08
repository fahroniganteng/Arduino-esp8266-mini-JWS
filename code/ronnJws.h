/**
 * @file ronnJws.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * @brief
 * @version 1.0
 * @date 2023-09-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "PrayerTimes.h"

double location_lat = -6.088912766430358;
double location_long = 106.9967877051682;
int prayShow = -1;
double times[sizeof(TimeName) / sizeof(char *)];
String prayTime[8] = {"", "", "", "", "", "", "", ""};
String prayName[8] = {
    "   SUBUH", // 0 Fajr
    "-",        // 1 sunrise
    "   ZUHUR", // 2 Dhuhr
    "_ ASAR",   // 3 Asr
    "-",        // 4 sunset
    "MAGRIB",   // 5 Maghrib
    "_ISYA",    // 6 Isha
    "-"         // 7 TimesCount
};
String prayName1[8] = {
    "Subuh",    // 0 Fajr
    "Terbit",   // 1 sunrise
    "Zuhur",    // 2 Dhuhr
    "Asar",     // 3 Asr
    "Terbenam", // 4 sunset
    "Magrib",   // 5 Maghrib
    "Isya",     // 6 Isha
    "-"         // 7 TimesCount
};

void getJWS(int tahun, int bulan, int tanggal, int jam, int menit)
{
  // Metode kemenag
  set_calc_method(Custom);
  set_fajr_angle(20);
  set_asr_method(Shafii);
  set_isha_angle(18);
  set_high_lats_adjust_method(AngleBased);

  get_prayer_times(tahun, bulan, tanggal, location_lat, location_long, timeZone, times);

  char buffer[80];
  bool jwsFound = false;
  prayShow = 0;
  for (int i = 0; i < sizeof(times) / sizeof(double); i++)
  {
    char tmp[10];
    int hours, minutes;
    get_float_time_parts(times[i], hours, minutes);
    // 0 	       Fajr 4.08  	 04:05
    // 1 	    Sunrise 5.48   	 05:29
    // 2 	      Dhuhr 11.71 	 11:42
    // 3 	        Asr 15.13 	 15:08
    // 4 	     Sunset 17.93 	 17:56
    // 5 	    Maghrib 17.93 	 17:56
    // 6 	       Isha 19.19 	 19:11
    // 7 	 TimesCount 0.00 	    00:00
    sprintf(buffer, "%02d:%02d", hours, minutes);
    prayTime[i] = buffer;

    // setelah isya ambil jam isya
    if (TimeName[i] == "Isha" && jam >= hours)
      prayShow = i;
    else if (!jwsFound && (TimeName[i] == "Fajr" || TimeName[i] == "Dhuhr" || TimeName[i] == "Asr" || TimeName[i] == "Maghrib" || TimeName[i] == "Isha"))
    {
      // akan berganti jadwal selanjutnya setelah 30 menit
      if ((hours * 60 + minutes + 30) >= (jam * 60 + menit))
      {
        prayShow = i;
        jwsFound = true;
      }
    }
  }
}