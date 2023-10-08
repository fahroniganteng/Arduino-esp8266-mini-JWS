/**
 * @file ronnEeprom.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-10-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/**
 * SET EEPROM
 * *************************************************************************
 * 512 Address --> EEPROM.begin(512)
 * Manual mapping address EEPROM (gak pake library)
 * *************************************************************************
 * 0    - 99    --> Buat array data (masih belum jadi dipake)
 * 100  - 149   --> ssid  --> SSID Wifi
 * 150  - 199   --> pass  --> Password Wifi
 * 200  - 249   --> ntp   --> ntp server
 * 250  - 259   --> gmt   --> time zone (GMT +XX) -12 ~ +12
 * 260  - 274   --> latitude  --> latitude
 * 275  - 289   --> longitude   --> longitude
 * 290  - 294   --> intensity   --> kecerahan led
 * 295  - 300   --> shTgl     --> waktu menampilkan tanggal setiap ... detik
 * 300  - 309   --> shJws    --> waktu menampilkan suhu setiap ... detik
 * *************************************************************************
 */
#define START 0  // Address EEPROM nya
#define LENGTH 1 // Panjang EEPROM nya
int eepr[2];

// Get list EEPROM
void getListEEPROM(String e)
{
  if (e == "ssid")
  {
    eepr[START] = 100;
    eepr[LENGTH] = 50;
  }
  else if (e == "pass")
  {
    eepr[START] = 150;
    eepr[LENGTH] = 50;
  }
  else if (e == "ntp")
  {
    eepr[START] = 200;
    eepr[LENGTH] = 50;
  }
  else if (e == "gmt")
  {
    eepr[START] = 250;
    eepr[LENGTH] = 10;
  }
  else if (e == "latitude")
  {
    eepr[START] = 260;
    eepr[LENGTH] = 15;
  }
  else if (e == "longitude")
  {
    eepr[START] = 275;
    eepr[LENGTH] = 15;
  }
  else if (e == "intensity")
  {
    eepr[START] = 290;
    eepr[LENGTH] = 5;
  } // eeprom nya bisa nyimpen integer padahal... hihihiii... biarin lah, biar satu fungsi panggilnya gampang
  else if (e == "shTgl")
  {
    eepr[START] = 295;
    eepr[LENGTH] = 5;
  }
  else if (e == "shJws")
  {
    eepr[START] = 300;
    eepr[LENGTH] = 5;
  }
}

// Write to EEPROM
void writeEEPROM(String e, String data)
{
  getListEEPROM(e);
  for (int i = 0; i < eepr[LENGTH]; ++i)
  {
    if (i < data.length())
      EEPROM.write(i + eepr[START], data[i]);
    else
      EEPROM.write(i + eepr[START], ' '); // selebihnya isi spasi (di trim pas read)
  }
  EEPROM.commit(); // on esp board, after write must be commit
}

// Read from EEPROM to string
String readEEPROM(String e)
{
  getListEEPROM(e);
  String eeprData = "";
  for (int i = 0; i < eepr[LENGTH]; ++i)
  {
    eeprData += char(EEPROM.read(i + eepr[START]));
  }
  eeprData.trim();

  //  DEBUG ONLY --> jangan dienable terus --> pas baca password keluar di console
  Serial.print("Read EEPROM " + e + " : \"");
  Serial.print(eeprData);
  Serial.println("\"");
  return eeprData;
}

void resetEEPROM()
{
  Serial.print("RESET EEPROM : ");
  Serial.println(EEPROM.length());
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  delay(10);
}