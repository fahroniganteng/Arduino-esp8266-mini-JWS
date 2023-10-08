/**
 * @file code.ino
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-10-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <math.h> // buat ngitung suhu (fungsi log)
#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "Timer.h"
// #include "time.h"
#include "ronnLedAnimation.h" //==> FUNGSI ANIMASI MATRIX LED PINDAH SINI SEMUA BIAR GAK PUSING BACA NYA....

// FUNCTION HITUNG HARI
#define SECS_PER_MIN (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY (SECS_PER_HOUR * 24L)

#define getSEC(_time_) (_time_ % SECS_PER_MIN)
#define getMIN(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define getHOUR(_time_) ((_time_ % SECS_PER_DAY) / SECS_PER_HOUR)
#define getDAY(_time_) (_time_ / SECS_PER_DAY)

// Variable Timer
Timer tmr;
int tmrEvJam; // Event buat menampilkan jam di matrix led

int ledIntensity = 0; // kecerahan led
float timeZone;				// buat simpan zona waktu --> ada yang GMT -3:30 --> -3.5 jadinya pake float

// EEPROM Function
#include "ronnEeprom.h"
// JWS Function
#include "ronnJws.h"
// Wifi Function
#include "ronnWifi.h"
// NTP Function
#include "ronnNtp.h"

void setIntensity()
{
	String buf = readEEPROM("intensity");
	ledIntensity = (buf.length() == 5 || buf.length() == 0) ? 0 : buf.toInt(); // if eeprom not valid
	ledIntensity = ledIntensity > 10 ? 10 : ledIntensity;											 // kecerahan max 10 (biar awet)
	mx.control(MD_MAX72XX::INTENSITY, ledIntensity);													 // 0-15  --> #define MAX_INTENSITY 0xf
}

int shJws = 10; // on startup diganti dari eeprom
int shTgl = 30; // on startup diganti dari eeprom
int shKustomText = 0;
String kustomText = "";
String jsonData = "";
#include "ronnHtml.h" // move here --> biar enak dibaca

/**
 * MATRIX LED FUNCTION
 * *************************************************************************
 * TSL  --> Transform Shift Left one pixel element
 * TSR  --> Transform Shift Right one pixel element
 * TSU  --> Transform Shift Up one pixel element
 * TSD  --> Transform Shift Down one pixel element
 * TFLR --> Transform Flip Left to Right
 * TFUD --> Transform Flip Up to Down
 * TRC  --> Transform Rotate Clockwise 90 degrees
 * TINV --> Transform INVert (pixels inverted)
 */
int loopDetik = 0;
int loopDetik1 = 0;
String old_time = "xx:xx"; // hhii
bool jamOnDisplay = false;

// big font 00:00
void shJamStyle1()
{
	String new_time = ntp_jam.substring(0, 5);
	if (!jamOnDisplay)
	{
		clearLeft();
		mx.setFont(font_ICON);
		scrollDown(12, 10, "9");
		delay(300);
		clearRight();
		mx.setFont(font_JAM1); // ganti font
		printTextRight(new_time, 2);
		loopDetik = 6; // biar langsung bergerak;
	}

	uint8_t detik[20] = {':', ':', ':', ':', ':', ':', ':', ';', '<', '=', '>', '>', '>', '>', '>', '>', '>', '=', '<', ';'}; // di font dipake detik semua
	mx.setFont(font_JAM1);																																																		// ganti font
	mx.setChar(16, detik[loopDetik]);

	// String new_time = ntp_jam.substring(3,8); //Ambil detik buat tes
	if (old_time != new_time)
	{
		mx.setFont(font_JAM1); // ganti font
		mx.setChar(31 - 1, new_time[0]);
		mx.setChar(23, new_time[1]);
		mx.setChar(15 - 2, new_time[3]);
		mx.setChar(7 - 1, new_time[4]);
		old_time = new_time;
	}

	jamOnDisplay = true;
	loopDetik = loopDetik < 19 ? loopDetik + 1 : 0;
}

// icon + 00:00
void shJamStyle2()
{
	String detik = "EEFFGGHHIIJJKKLL"; // kasih 1 kecepeten
	String detik1 = "::::;;;;;";
	String new_time = ntp_jam.substring(0, 5);
	mx.setFont(font_ICON);
	if (!jamOnDisplay)
	{
		clearLeft();
		loopDetik = 0;
		loopDetik1 = 1;
		animIcon(String(detik[loopDetik]));
		delay(200);
		mx.setFont(font_4X7); // ganti font
		printText(new_time, 11);
	}
	else
	{
		printText(String(detik[loopDetik]), 1);		 // icon
		mx.setFont(font_4X7);											 // ganti font
		printText(String(detik1[loopDetik1]), 21); // titik dua (:)
		if (new_time != old_time)
		{
			printText(new_time, 11);
			old_time = new_time;
		}
	}
	// Serial.println(loopDetik);

	jamOnDisplay = true;
	loopDetik = loopDetik < detik.length() - 1 ? loopDetik + 1 : 0;
	loopDetik1 = loopDetik1 < detik1.length() - 1 ? loopDetik1 + 1 : 0;
}

// small font 00:00:00
int animStyle3 = 0;
void shJamStyle3()
{
	String detik = "::::;;;;;";
	mx.setFont(font_3X5); // ganti font

	if (!jamOnDisplay)
	{ // pertama load (animasi)
		clearRight();
		animIcon("G");
		delay(200);
		mx.setFont(font_3X5);												// ganti font
		scrollDown(23, 7, ntp_jam.substring(3, 5)); // menit
		delay(100);
		scrollDown(13, 7, ntp_jam.substring(0, 2)); // jam
		printText(":", 21);
		animStyle3 = 20;
	}
	else if (animStyle3 > 0)
	{
		printText(String(detik[loopDetik]), 21);
		// menit
		if (ntp_jam.substring(3, 4) != old_time.substring(3, 4))
			scrollDown(13, 7, ntp_jam.substring(3, 5));
		else if (ntp_jam.substring(4, 5) != old_time.substring(4, 5))
			scrollDown(17, 3, ntp_jam.substring(4, 5));
		// jam
		if (ntp_jam.substring(0, 1) != old_time.substring(0, 1))
			scrollDown(3, 7, ntp_jam.substring(0, 2));
		else if (ntp_jam.substring(1, 2) != old_time.substring(1, 2))
			scrollDown(7, 3, ntp_jam.substring(1, 2));

		animStyle3--;
		if (animStyle3 < 1)
		{
			for (uint8_t i = 0; i <= 9; i++)
			{
				mx.transform(MD_MAX72XX::TSL);
				delay(10);
			}
			delay(100);
			printText(ntp_jam.substring(6, 8), 23);
			// scrollDown(23,7,ntp_jam.substring(6,8));//detik
			printText(String(detik[loopDetik]), 21);
			// printText("/0",11);
		}
	}
	else
	{
		// ntp_jam = 00:00:00 --> 0,2 jam; 3,5 menit; 6,8 detik

		// show titik dua (:)
		printText(String(detik[loopDetik]), 11);
		printText(String(detik[loopDetik]), 21);
		// detik
		if (ntp_jam.substring(6, 7) != old_time.substring(6, 7))
			scrollDown(23, 7, ntp_jam.substring(6, 8));
		else if (ntp_jam.substring(7, 8) != old_time.substring(7, 8))
			scrollDown(27, 3, ntp_jam.substring(7, 8));
		// menit
		if (ntp_jam.substring(3, 4) != old_time.substring(3, 4))
			scrollDown(13, 7, ntp_jam.substring(3, 5));
		else if (ntp_jam.substring(4, 5) != old_time.substring(4, 5))
			scrollDown(17, 3, ntp_jam.substring(4, 5));
		// jam
		if (ntp_jam.substring(0, 1) != old_time.substring(0, 1))
			scrollDown(3, 7, ntp_jam.substring(0, 2));
		else if (ntp_jam.substring(1, 2) != old_time.substring(1, 2))
			scrollDown(7, 3, ntp_jam.substring(1, 2));

		// printText(ntp_jam,3);
	}

	old_time = ntp_jam;
	jamOnDisplay = true;
	loopDetik = loopDetik < detik.length() - 1 ? loopDetik + 1 : 0;
}

// Pergantian style jam
int jamStyle = 3;
void shJam()
{
	if (ntp_jam == "--no data--")
		return;
	else if (jamStyle == 3)
		shJamStyle3();
	else if (jamStyle == 2)
		shJamStyle2();
	else
		shJamStyle1();
}
void changeJamStyle()
{
	if (jamStyle == 3)
		jamStyle = 2;
	else if (jamStyle == 2)
		jamStyle = 1;
	else
		jamStyle = 3;
}

int shTglAfter = 0;
int shJwsAfter = 0;
int countSts = 0;
int countApMode = 0;
bool errorNtp = false;
bool errorWifi = false;
void runApplication()
{
	// cek clienr connect pada mode soft-AP
	softap_stations_cnt = wifi_softap_get_station_num();
	// Serial.println(softap_stations_cnt);

	if (ntp_all != "--no data--")
		errorNtp = false;
	if (WiFi.status() == WL_CONNECTED)
		errorWifi = false;

	int detik = ntp_jam.substring(6, 8).toInt();
	if (apMode)
	{
		String icon = "EFGHIJKL";
		mx.setFont(font_ICON);									 // ganti font
		printText(String(icon[countApMode]), 1); // icon
		countApMode = countApMode < icon.length() - 1 ? countApMode + 1 : 0;
	}

	else if (WiFi.status() != WL_CONNECTED)
	{
		if (errorWifi)
			return; // biar nggak berulang animasi nya
		tmr.stop(tmrEvJam);
		tmr.stop(tmrEvJam);
		clearLeft();
		animIcon("8");
		mx.setFont(font_3X5); // ganti font
		scrollDown(16, 32, "OFF");
		jamOnDisplay = false;
		countSts = 1;
		errorWifi = true;
	}

	else if (ntp_all == "--no data--")
	{
		if (errorNtp)
			return; // biar nggak berulang animasi nya
		tmr.stop(tmrEvJam);
		clearLeft();
		animIcon("7");
		mx.setFont(font_5X8);
		scrollDown(12, 32, "NTP");
		jamOnDisplay = false;
		countSts = 1;
		errorNtp = true;
	}

	// CUSTOM TEXT____________________________________________________________
	else if (kustomText.length() > 0 && detik % shKustomText == 0)
	{
		tmr.stop(tmrEvJam);
		clearRight();
		mx.setFont(font_ICON);
		scrollDown(13, 8, ":"); // icon text
		delay(400);
		for (uint8_t i = 0; i <= MAX_DEVICES * 8; i++)
		{
			mx.transform(MD_MAX72XX::TSL);
			delay(7);
		}
		mx.setFont(font_default); // default font
		scrollText(kustomText + "           ");
		countSts = 1;
		jamOnDisplay = false;
		changeJamStyle();
	}

	// DAY ___________________________________________________________________________
	else if (detik % shTgl == 0)
	{
		tmr.stop(tmrEvJam);
		clearRight();
		animIcon("1");
		mx.setFont(font_5X8);
		scrollDown(12, 32, ntp_tgl.substring(0, 3));
		shTglAfter = 10;
	}

	// DATE ___________________________________________________________________________
	else if (shTglAfter == 8)
	{
		scrollDown(8, 32, "\0");
		for (uint8_t i = 0; i <= MAX_DEVICES * 8; i++)
		{
			mx.transform(MD_MAX72XX::TSR);
			delay(5);
		}
		mx.setFont(font_5X8);
		printTextRight(ntp_tgl.substring(5, 11), 2);
	}

	// YEAR __________________________________________________________________________
	else if (shTglAfter == 6)
	{
		clearRight();
		animIcon("0");
		delay(25);
		mx.setFont(font_SUHU);
		scrollDown(10, 32, ntp_tgl.substring(12, 16));
		countSts = 2; // 2 detik baru ganti jam
		jamOnDisplay = false;
		changeJamStyle();
	}

	// JWS ___________________________________________________________________________
	else if (detik % shJws == 0)
	{
		tmr.stop(tmrEvJam);
		clearUp();
		mx.setFont(font_ICON);
		scrollDown(12, 10, "2");
		delay(400);
		clearRight();
		mx.setFont(font_4X7); // ganti font
		printTextRight(prayName[prayShow], 2);
		shJwsAfter = 10;
	}
	else if (shJwsAfter == 8)
	{
		clearLeft();
		delay(200);
		animIcon("2");
		delay(200);
		mx.setFont(font_4X7); // ganti font
		scrollDown(10, 32, String(prayTime[prayShow]));
		countSts = 3; // 3 detik baru ganti jam
		jamOnDisplay = false;
		changeJamStyle();
	}

	// CLOCK _________________________________________________________________________
	else if (countSts == 0)
	{
		if (tmr.status(tmrEvJam) == 0)
			tmrEvJam = tmr.every(100, shJam);
	}

	if (countSts >= 0)
		countSts--; // no process status= -1
	if (shTglAfter > 0)
		shTglAfter--; // no process date = 0
	if (shJwsAfter > 0)
		shJwsAfter--; // no process date = 0
}

void setup()
{
	Serial.begin(115200);
	delay(100);
	Serial.println();
	EEPROM.begin(512);

	/**
	 * How to reset EEPROM for simulation first upload sketch to board:
	 * - Enable (un-comment) the code "resetEEPROM()" below and upload sketch to ESP board
	 * - Disable code, and then upload sketch again
	 **/
	// resetEEPROM();

	pinMode(LED_BUILTIN, OUTPUT); // LED BUILTIN = D4
	pinMode(A0, INPUT);						// thermistor ==> temp
	delay(10);
	digitalWrite(LED_BUILTIN, LOW);

	// File system start (file on flash memory --> html,js,css and friends) ----------------------------
	SPIFFS.begin();
	String buf; // buffer

	// Matrix Led -----------------------------------------------------------------------
	mx.begin();
	setIntensity();
	mx.clear();

	// RONI GANTENG SHOW ^___^
	delay(300);
	mx.setFont(font_ICON); // ganti font
	scrollDown(1, 8, "A");
	delay(100);
	scrollDown(9, 8, "B");
	delay(100);
	scrollDown(17, 8, "C");
	delay(100);
	scrollDown(25, 8, "D");
	delay(1000);
	clearLeft();

	// show animasi & tulisan clock
	scrollDown(13, 8, "E");
	delay(400);
	for (uint8_t i = 0; i <= 11; i++)
	{
		mx.transform(MD_MAX72XX::TSL);
		delay(5);
	}
	delay(200);
	mx.setFont(font_3X5); // ganti font
	scrollDown(11, 32, "CLOCK");
	mx.setFont(font_ICON);
	buf = "FGHIJKLEFGH";
	for (uint8_t i = 0; i <= buf.length(); i++)
	{
		printText(String(buf[i]), 1);
		delay(100);
	}

	// Wifi_______________________________________________________
	WiFi.persistent(false);
	Serial.println("Connecting wifi...");
	connectWifi();
	// show jam pasir
	if (!apMode)
	{ // AP mode muncul tulisan AP MODE di diplay
		clearUp();
		mx.setFont(font_ICON);
		scrollDown(13, 8, "V");
	}
	// handle html request
	serverOnHandleClient();

	// start http server
	server.begin();
	Serial.println("HTTP server started");

	// matiin led();
	digitalWrite(LED_BUILTIN, HIGH); // matiin led bawaan (HIGH = mati) --> i dont know why... :D

	// Waktu tampil jam & suhu
	shJws = readEEPROM("shJws").toInt();
	shJws = shJws < 5 ? 30 : shJws; // proteksi pertama load, EEPROM kosong (xx mod 0 = ERROR)
	shTgl = readEEPROM("shTgl").toInt();
	shTgl = shTgl < 5 ? 60 : shTgl; // proteksi pertama load, EEPROM kosong (xx mod 0 = ERROR)

	// Get EEPROM location
	Serial.println("Get Location ");
	buf = readEEPROM("latitude");
	buf = (buf.length() == 15 || buf.length() == 0) ? "-6.0889" : buf; // if eeprom not valid
	location_lat = buf.toFloat();
	buf = readEEPROM("longitude");
	buf = (buf.length() == 15 || buf.length() == 0) ? "106.9967" : buf; // if eeprom not valid
	location_long = buf.toFloat();
	Serial.print(location_lat, 4);
	Serial.print(", ");
	Serial.println(location_long, 4);

	// Get EEPROM Timezone
	Serial.print("Get NTP Offset : GMT ");
	buf = readEEPROM("gmt");
	buf = (buf.length() == 10 || buf.length() == 0) ? "7" : buf; // if eeprom not valid
	timeZone = buf.toFloat();
	timeZone = (timeZone < -12 || timeZone > 12) ? 7 : timeZone; // if timeZone not valid
	Serial.println(timeZone);

	// Butuh dapet key array event dulu buat dapet tmr.status--> soalnya tak modif lib. timer nya... :D
	tmrEvJam = tmr.oscillate(LED_BUILTIN, 50, HIGH, 1);
	delay(100);
	tmr.every(60 * 1000, reConnectWifi); // jika gagal konek wifi, otomatis re-connect tiap 1 menit
	tmr.every(1000, uptime);						 // update uptime

	// NTP
	configNtp();
	delay(1000);
	tmr.every(100, getNtpDate);
	tmr.every(60 * 1000, []() { // force update tiap 1 menit
		timeClient.forceUpdate();
	});

	tmr.every(1000, runApplication);
}

void loop()
{
	tmr.update();
	server.handleClient();
	timeClient.update();
}
