/**
 * @file ronnNtp.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * @brief
 * @version 1.0
 * @date 2023-09-29
 *
 * @copyright Copyright (c) 2023
 *
 */

WiFiUDP ntpUDP; // buat send req. time ke NTP server
NTPClient timeClient(ntpUDP);
String ntpServer; // buat simpan nama NTP server (biar gak panggil eeprom terus)

long oldLong = 0; // Epoc Time

char namaHari[7][15] = {"MIN", "SEN", "SEL", "RAB", "KAM", "JUM", "SAB"};
char namaBulan[12][15] = {"JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGU", "SEP", "OKT", "NOV", "DES"};
long secUpTime = 0; // default pertama dinyalakan
String shUpTime;		// show UP Time (buat tampung view up time)

String ntp_all; // Rabu, 01 Januari 2020 - 15:04:32
String ntp_jam; // 15:04:32
String ntp_tgl; // RAB, 01 JAN 2020

void configNtp()
{
  Serial.print("Get NTP Server : \"");
  ntpServer = readEEPROM("ntp");
  ntpServer = (ntpServer.length() == 50 || ntpServer.length() == 0) ? "time.google.com" : ntpServer; // if eeprom not valid
  Serial.print(ntpServer);
  Serial.println("\"");

  timeClient.setPoolServerName(ntpServer.c_str());
  timeClient.setTimeOffset(timeZone * 60 * 60); // convert jam ke detik
  timeClient.setUpdateInterval(6000);          // update tiap menit
  timeClient.begin();
  // timeClient.update();
  // timeClient.forceUpdate();
}
// Get Time
void getNtpDate()
{
  unsigned long nowLong;
  char buf[50];

  nowLong = timeClient.getEpochTime();
  // Serial.println("NTP________________");
  // Serial.println(nowLong);
  // sprintf(buf, "%d:%02d:%02d:%02d",
  //        timeClient.getDay(),
  //        timeClient.getHours(),
  //        timeClient.getMinutes(),
  //        timeClient.getSeconds());
  // Serial.println(buf);

  if (nowLong <= 1000000000)
  { // gak dapet waktu = 25200
    // Serial.println(nowLong);
    ntp_all = "--no data--";
    ntp_jam = "--no data--";
    ntp_tgl = "--no data--";
    if (WiFi.status() == WL_CONNECTED && nowLong - oldLong > 20)
    {                      // setiap 20 detik
      timeClient.update(); // jika ada koneksi wifi coba update lagi...
      oldLong = nowLong;
    }
  }
  else
  {
    char buf[50];
    time_t t = nowLong;
    tm *ptm = gmtime(&t);

    snprintf(buf, 50, "%s, %02d %s %d %02d:%02d:%02d", namaHari[ptm->tm_wday], ptm->tm_mday, namaBulan[ptm->tm_mon], ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    ntp_all = String(buf);
    snprintf(buf, 50, "%s, %02d %s %d", namaHari[ptm->tm_wday], ptm->tm_mday, namaBulan[ptm->tm_mon], ptm->tm_year + 1900);
    ntp_tgl = String(buf);
    snprintf(buf, 50, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    ntp_jam = String(buf);

    // epoch time dimulai tahun 1900
    // bulan dimulai dai 0
    getJWS(ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
  }
}
void uptime()
{
  char buf[50]; // buffer

  // UP TIME --> waktu dari dinyalakan__________________________________________________________________________________
  secUpTime++;
  snprintf(buf, 50, "%ld hari %02d:%02d:%02d", getDAY(secUpTime), getHOUR(secUpTime), getMIN(secUpTime), getSEC(secUpTime));
  shUpTime = String(buf);
}