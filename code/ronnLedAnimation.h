/**
 * @file roniGantengLedAnimation.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * *******************************************************************
 * Fungsi buat handle matrix led
 */

#include "MD_MAX72xx.h"
#include <SPI.h>
#include "ronnFont.h"

/**
 * SET MATRIX latexinclude
 * *******************************************************************
 */
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5	// or SCK
#define DATA_PIN D7 // or MOSI
#define CS_PIN D8		// or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void scrollDown(uint8_t st, uint8_t col, String txt)
{ // posisi start dari kiri 1-xx, jumlah kolom, String pengganti
	uint8_t ln = txt.length();
	uint8_t buf[col];

	// Scroll down data lama
	for (uint8_t no = 0; no < COL_SIZE; no++)
	{
		uint8_t pos = (MAX_DEVICES * 8) - st;
		mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // stop perubahan display
		for (uint8_t i = 0; i < col; i++)
		{
			buf[i] = mx.getColumn(pos); // Get old karakter di display
			buf[i] = buf[i] << 1;
			mx.setColumn(pos, buf[i]);
			pos--;
		}
		mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON); // jalankan perubahan display
		delay(7);
	}

	// Bikin all new text jadi binari
	uint8_t buf_txt[ln][10]; // ane bikin icon sampe 10 kolom... hahaha....
	uint8_t fontCol[ln];
	uint8_t allCol = 0; // total kolom text setelah digabung
	for (uint8_t i = 0; i < ln; i++)
	{
		fontCol[i] = mx.getChar(txt[i], 10, buf_txt[i]);
		allCol += fontCol[i];
		allCol++; // tambah 1 tiap huruf buat jarak antar text;
	}

	// gabung all new text
	uint8_t buf_new[allCol];
	uint8_t pos = 0;
	for (uint8_t no = 0; no < ln; no++)
	{
		for (uint8_t i = 0; i < fontCol[no]; i++)
		{
			buf_new[pos] = buf_txt[no][i];
			// Serial.println(buf_new[pos],BIN);
			pos++;
		}
		// nambah jarak antar text
		buf_new[pos] = '\0';
		pos++;
	}
	// Scroll down data baru
	uint8_t buf_show[allCol];
	for (uint8_t no = 0; no < COL_SIZE; no++)
	{
		uint8_t pos = (MAX_DEVICES * 8) - st;
		mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // stop perubahan display
		for (uint8_t i = 0; i < allCol; i++)
		{
			if (i > col)
				break; // text over
			buf_show[i] = buf_new[i] >> (COL_SIZE - no - 1);
			mx.setColumn(pos, buf_show[i]);
			pos--;
		}
		mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON); // jalankan perubahan display
		delay(10);
	}
}

void scrollText(String txt, int speedRun = 40)
{ // speed normal = 40, cepet bingit  15
	// PERHATIAN : PROSES INI men-DELAY TIMER!!
	// SEMUA PROSES LAIN AKAN BERHENTI!!
	const char *p = txt.c_str();
	uint8_t charWidth;
	uint8_t cBuf[8]; // this should be ok for all built-in fonts
	// mx.setFont(false);// balikin font built-in
	mx.clear();
	while (*p != '\0')
	{
		charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
		for (uint8_t i = 0; i < charWidth + 1; i++)
		{ // allow space between characters
			mx.transform(MD_MAX72XX::TSL);
			if (i < charWidth)
				mx.setColumn(0, cBuf[i]);
			delay(speedRun);
		}
	}
}

void printText(String txt, int pos = 0)
{
	const char *p = txt.c_str();
	uint8_t charWidth;
	uint8_t cBuf[8]; // this should be ok for all built-in fonts
	int kolom = (MAX_DEVICES * 8) - pos;
	// mx.clear();
	// mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); --> jika lebih dari ukuran malah nggak muncul
	while (*p != '\0')
	{
		charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
		for (uint8_t i = 0; i < charWidth + 1; i++)
		{
			if (i < charWidth)
				mx.setColumn(kolom, cBuf[i]);
			if (kolom < 1)
				return;
			kolom--;
			// delay(7);
		}
	}
	// mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void printTextRight(String txt, int pos = 0)
{
	const char *p = txt.c_str();
	uint8_t charWidth;
	uint8_t cBuf[8]; // this should be ok for all built-in fonts
	int kolom = (MAX_DEVICES * 8) - pos;
	// mx.clear();
	while (*p != '\0')
	{
		charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
		for (uint8_t i = 0; i < charWidth + 1; i++)
		{
			if (i < charWidth)
				mx.setColumn(kolom, cBuf[i]);
			if (kolom < 1)
				return;
			kolom--;
			delay(7);
		}
	}
}

void animIcon(String icon)
{
	mx.setFont(font_ICON); // ganti font
	scrollDown(13, 8, icon);
	delay(200);
	for (uint8_t i = 0; i <= 11; i++)
	{
		mx.transform(MD_MAX72XX::TSL);
		delay(5);
	}
}

void clearLeft()
{
	for (uint8_t i = 0; i <= MAX_DEVICES * 8; i++)
	{
		mx.transform(MD_MAX72XX::TSL);
		delay(3);
	}
}

void clearRight()
{
	for (uint8_t i = 0; i <= MAX_DEVICES * 8; i++)
	{
		mx.transform(MD_MAX72XX::TSR);
		delay(3);
	}
}

void clearDown()
{
	for (uint8_t i = 0; i <= 8; i++)
	{
		mx.transform(MD_MAX72XX::TSD);
		delay(3);
	}
}

void clearUp()
{
	for (uint8_t i = 0; i <= 8; i++)
	{
		mx.transform(MD_MAX72XX::TSU);
		delay(3);
	}
}
