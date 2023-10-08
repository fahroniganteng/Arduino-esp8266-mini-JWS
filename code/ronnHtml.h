/**
 * @file ronnHtml.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * ***************************************************************
 * HTML Request Handle
 */

String header = "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
  <title>Jam NTP & JWS</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
  <style>\
    body {margin:0; font-family: Arial, Helvetica, Sans-Serif;zoom:1.2}\
    .header{background:#000; color:#FFF; padding:7px 10px;margin:0}\
    .content{padding:5px 10px;}\
    .menu{background:#c4e0ff;padding:5px 10px;}\
    .menu a{color:#000;font-size: 14px;text-decoration:none}\
    input,select{width: 400px; max-width:80%; margin: 0 0 10px;}\
    input[type='text'],input[type='number'],select{border:0.9px solid #999;border-radius:4px;padding:6px 4px}\
    input[type='range']{float:left;}\
		output{margin:1px 10px;float:left;font-weight: bold;}\
    ul{margin:0;padding-left:18px}\
    h4{margin-bottom:2px;color:#357cfd;font-size:14px}\
		small{color:#999;font-weight:normal;font-size:8pt;width:100%}\
    .info{color:#426dff;font-size:10pt;background:#e4f2ff;padding:10px;border-radius:4px;box-shadow:1px 1px 4px -2px rgba(0,0,0,0.5);}\
    .info label{font-weight:bold;width:100%;float:left}\
		.text-label{margin-bottom: 4px;padding-left: 5px;}\
		.row{width:100%;clear:left}\
		button{ padding: 10px 14px;border-radius: 4px;background-color: #3789fd;color: #FFF;border: none;}\
  </style>\
  <script>\
    function restartBoard(){\
      if(confirm('Konfirmasi restart?'))\
        window.location.href = \"/restart\";\
    }\
  </script>\
</head>";

String menu = "<div class=\"menu\">\
  <a href=\"/\">Home | </a>\
  <a href=\"/setting\">Setting</a> | \
  <a href=\"/wifi\">Wifi</a> | \
  <a href=\"/jws\">JWS</a> | \
  <a href=\"/reset\">Reset</a>\
</div>";

String iconGit = "<svg style=\"float:left\" width=\"30\" height=\"25\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 98 96\">\
    <path fill=\"#24292f\" d=\"M48.854 0C21.839 0 0 22 0 49.217c0 21.756 13.993 40.172 33.405 46.69 2.427.49 3.316-1.059 3.316-2.362 0-1.141-.08-5.052-.08-9.127-13.59 2.934-16.42-5.867-16.42-5.867-2.184-5.704-5.42-7.17-5.42-7.17-4.448-3.015.324-3.015.324-3.015 4.934.326 7.523 5.052 7.523 5.052 4.367 7.496 11.404 5.378 14.235 4.074.404-3.178 1.699-5.378 3.074-6.6-10.839-1.141-22.243-5.378-22.243-24.283 0-5.378 1.94-9.778 5.014-13.2-.485-1.222-2.184-6.275.486-13.038 0 0 4.125-1.304 13.426 5.052a46.97 46.97 0 0 1 12.214-1.63c4.125 0 8.33.571 12.213 1.63 9.302-6.356 13.427-5.052 13.427-5.052 2.67 6.763.97 11.816.485 13.038 3.155 3.422 5.015 7.822 5.015 13.2 0 18.905-11.404 23.06-22.324 24.283 1.78 1.548 3.316 4.481 3.316 9.126 0 6.6-.08 11.897-.08 13.526 0 1.304.89 2.853 3.316 2.364 19.412-6.52 33.405-24.935 33.405-46.691C97.707 22 75.788 0 48.854 0z\"></path>\
  </svg>\
";

String iconYoutube = "<svg style=\"float:left\" width=\"30\" height=\"30\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 48 48\">\
    <path fill=\"#FF3D00\" d=\"M43.2,33.9c-0.4,2.1-2.1,3.7-4.2,4c-3.3,0.5-8.8,1.1-15,1.1c-6.1,0-11.6-0.6-15-1.1c-2.1-0.3-3.8-1.9-4.2-4C4.4,31.6,4,28.2,4,24c0-4.2,0.4-7.6,0.8-9.9c0.4-2.1,2.1-3.7,4.2-4C12.3,9.6,17.8,9,24,9c6.2,0,11.6,0.6,15,1.1c2.1,0.3,3.8,1.9,4.2,4c0.4,2.3,0.9,5.7,0.9,9.9C44,28.2,43.6,31.6,43.2,33.9z\"></path>\
    <path fill=\"#FFF\" d=\"M20 31L20 17 32 24z\"></path>\
  </svg>\
";

void restartBoard()
{
	clearLeft();
	animIcon("U");
	delay(200);
	mx.setFont(font_3X5); // ganti font
	printTextRight("REBOOT", 10);
	delay(1000);
	// ESP.reset();  // register saved
	ESP.restart(); // clean reboot
}

void handleSetting()
{
	String html = "";
	html = header;
	html += "<body>";
	html += "<h4 class=\"header\">SETTING</h4>";
	html += menu;
	html += "<div class=\"content\">";
	html += "<form class=\"info\" action=\"/simpan\" method=\"post\">";
	html += "<div class=\"row\">";
	html += "<label>NTP Server</label><br>";
	html += "<input style=\"margin-bottom:0\" type=\"text\" name=\"ntp\" value=\"" + ntpServer + "\" step=\"0.0001\" placeholder=\"time.google.com\" required><br>";
	html += "<small style=\"display: inline-block\">*perubahan NTP mungkin akan berefek setelah restart</small>";
	html += "</div>";
	html += "<label>Latitude</label><br>";
	html += "<input type=\"number\" name=\"latitude\" value=\"" + String(location_lat, 4) + "\" step=\"0.0001\" required><br>";
	html += "<label>Longitude</label><br>";
	html += "<input type=\"number\" name=\"longitude\" value=\"" + String(location_long, 4) + "\" step=\"0.0001\" required><br>";
	html += "<div class=\"row\">";
	html += "<label>Timezone</label><br>";
	html += "<input type=\"range\" name=\"timezone\" value=\"" + String(timeZone) + "\" step=\"0.5\" min=\"-12\" max=\"12\" oninput=\"this.nextElementSibling.value = this.value\" required>";
	html += "<output>" + String(timeZone) + "</output><br>";
	html += "</div>";
	html += "<div class=\"row\">";
	html += "<label>Kecerahan Led</label><br> ";
	html += "<input style=\"margin-bottom:0\" type =\"range\" name=\"intensity\" value=\"" + String(ledIntensity) + "\" min=\"0\" max=\"10\" step=\"1\" oninput=\"this.nextElementSibling.value = this.value\" required>";
	html += "<output>" + String(ledIntensity) + "</output>";
	html += "<small style=\"display: inline-block;margin-bottom: 10px;\">*kecerahan tinggi dapat mengurangi umur led</small><br>";
	html += "</div>";
	html += "<div class=\"row\">";
	html += "<label>Tampilkan tanggal setiap... detik</label><br>";
	html += "<input type =\"range\" name=\"showTanggal\" value=\"" + String(shTgl) + "\" min=\"10\" max=\"60\" step=\"5\" oninput=\"this.nextElementSibling.value = this.value\" required>";
	html += "<output> " + String(shTgl) + "</output><br>";
	html += "</div>";
	html += "<div class=\"row\">";
	html += "<label>Tampilkan waktu sholat setiap... detik</label><br>";
	html += "<input type =\"range\" name=\"showJws\" value=\"" + String(shJws) + "\" min=\"10\" max=\"60\" step=\"5\" oninput=\"this.nextElementSibling.value = this.value\" required>";
	html += "<output>" + String(shJws) + "</output><br>";
	html += "</div>";
	html += "<div class=\"row\" style=\"margin-top:20px\">";
	html += "<button type =\"submit\">SIMPAN</button>&nbsp;";
	html += "<button type =\"button\" onclick=\"restartBoard();return false;\">RESTART</button>&nbsp;";
	html += "</div>";
	html += "</form>";
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void handleSimpan()
{
	String buf;
	String err = "";

	buf = server.arg("ntp");
	if (buf.length() > 0)
		writeEEPROM("ntp", buf);
	else
		err += "Error : NTP Server tidak valid<br>";

	buf = server.arg("latitude");
	if (buf.length() > 0)
	{
		writeEEPROM("latitude", buf);
		location_lat = buf.toFloat();
	}
	else
		err += "Error : Latitude tidak valid<br>";

	buf = server.arg("longitude");
	if (buf.length() > 0)
	{
		writeEEPROM("longitude", buf);
		location_long = buf.toFloat();
	}
	else
		err += "Error : Longitude tidak valid<br>";

	buf = server.arg("timezone");
	if (buf.length() > 0)
	{
		writeEEPROM("gmt", buf);
		timeZone = buf.toFloat();
		configNtp();
	}
	else
		err += "Error : Timezone tidak valid<br>";

	buf = server.arg("intensity");
	if (buf.length() > 0)
	{
		writeEEPROM("intensity", buf);
		setIntensity();
	}
	else
		err += "Error : Kecerahan led tidak valid<br>";

	buf = server.arg("showTanggal");
	if (buf.length() > 0)
	{
		writeEEPROM("shTgl", buf);
		shTgl = buf.toInt();
	}
	else
		err += "Error : Durasi tanggal tampil tidak valid<br>";

	buf = server.arg("showJws");
	if (buf.length() > 0)
	{
		writeEEPROM("shJws", buf);
		shJws = buf.toInt();
	}
	else
		err += "Error : Durasi JWS tampil tidak valid<br>";

	String html = header;
	html += "<body>";
	html += "<h4 class=\"header\">Menyimpan</h4>";
	html += menu;
	html += "<div class=\"content\">";
	if (err.length() < 1)
	{
		html += "<br>Data telah disimpan.<br>Beberapa perubahan setting mungkin memerlukan restart perangkat...<br>";
		html += "<script type=\"text/javascript\">";
		html += "setTimeout(function(){ window.location.href = \"../setting\"},2000);";
		html += "</script>";
	}
	else
	{
		html += "<br>Ditemukan beberapa kesalahan : <br>";
		html += err;
	}
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void handleRestart()
{
	String html = header;
	html += "<body>\
    <div class=\"content\">\
      <br>Harap tunggu, device akan restart...\
    </div>\
    <script type=\"text/javascript\">\
      setTimeout(function(){ window.location.href = \"../\"},10000);\
    </script>\
  </body>\
  </html>";
	server.send(200, "text/html", html);
	delay(1000);
	restartBoard();
}

void handleReset()
{
	String html = "";
	html = header;
	html += "<body>";
	html += "<h4 class=\"header\">RESET PERANGKAT</h4>";
	html += menu;
	html += "<div class=\"content\">";
	html += "<form class=\"info\" action=\"/simpan_reset\" method=\"post\" onsubmit=\"return confirm('Konfirmasi reset?');\">";
	html += "<h4 style=\"margin: 0 0 10px;padding-bottom: 2px;border-bottom: 0.9px solid #c7dbeb;\">";
	html += "Reset ke pengaturan awal";
	html += "</h4>";
	html += "<label>SSID Wifi :</label><div class=\"text-label\">Tidak ada</div>";
	html += "<label>Password Wifi :</label><div class=\"text-label\">Tidak ada</div>";
	html += "<label>NTP Server :</label><div class=\"text-label\">time.google.com</div>";
	html += "<label>Lokasi :</label>";
	html += "<div class=\"text-label\">";
	html += "Bekasi<br>";
	html += "- Latitude : -6.0889<br>";
	html += "- Longitude : 106.9967</div>";
	html += "<label>Timezone :</label><div class=\"text-label\">GMT+7</div>";
	html += "<label>Kecerahan led :</label><div class=\"text-label\">Level 0</div>";
	html += "<label>Tampilkan tanggal setiap :</label><div class=\"text-label\">60 detik</div>";
	html += "<label>Tampilkan JWS setiap :</label><div class=\"text-label\">30 detik</div>";
	html += "<input type=\"hidden\" name=\"konfirmasi\" value=\"setuju\">";
	html += "<button style=\"margin-top:20px\" type =\"submit\">RESET PERANGKAT</button>&nbsp;";
	html += "</form>";
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void handleSimpanReset()
{
	String buf = server.arg("konfirmasi");
	String html = header;
	if (buf == "setuju")
	{
		html += "<body>";
		html += "<h4 class=\"header\">Reset Perangkat</h4>";
		html += "<div class=\"content\">";
		html += "<br>Berhasil di reset, perangkat akan restart...";
		html += "</div>";
		html += "</body>";
		html += "</html>";
		server.send(200, "text/html", html);
		delay(1000);
		resetEEPROM();
		delay(1000);
		restartBoard();
	}
	else
	{
		html += "<body>";
		html += "<h4 class=\"header\">Reset Perangkat</h4>";
		html += menu;
		html += "<div class=\"content\">";
		html += "<br>Gagal!<br>Permintaan tidak valid...<br>";
		html += "</div>";
		html += "</body>";
		html += "</html>";
		server.send(200, "text/html", html);
	}
}

void handleJws()
{
	String html = header;
	html += "<body>";
	html += "<h4 class=\"header\">JAM WAKTU SHOLAT (JWS)</h4>";
	html += menu;
	html += "<div class=\"content\">";
	if (WiFi.status() == WL_CONNECTED)
	{
		String li = "";
		for (int i = 0; i < 7; i++)
		{
			li += "<li>" + prayName1[i] + " <b>" + prayTime[i] + "</b></li>";
		}
		html += "<div class=\"info\">";
		html += "<h4 style=\"margin:0 0 10px;padding-bottom:2px;border-bottom:0.9px solid #c7dbeb\">";
		html += ntp_tgl;
		html += "</h4>";
		html += "<label>Metode</label>";
		html += "<div class=\"text-label\">Kemenag<br>(subuh : 20 deg, Asar : Shafi'i, Isya : 18 deg)</div>";
		html += "<label>Koordinat</label>";
		html += "<div class=\"text-label\">" + String(location_lat, 4) + ", " + String(location_long, 4) + "</div>";
		html += "<label>Timezone</label>";
		html += "<div class=\"text-label\">GMT";
		html += timeZone < 0 ? "" : "+";
		html += String(timeZone) + "</div>";
		html += "<label>Waktu Sholat</label>";
		html += "<ul>" + li + "</ul>";
		html += "</div>";
	}
	else
		html += "Jam tidak terkoneksi internet";
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void handleWifi()
{
	scanWifi();
	String html = "";
	for (int i = 0; i < 7; i++)
	{
		html += "<li>" + prayName1[i] + " " + prayTime[i] + "</li>";
	}

	html = header;
	html += "<body>";
	html += "<h4 class=\"header\">WIFI</h4>";
	html += menu;
	html += "<div class=\"content\">";
	html += "<form class=\"info\" action=\"/simpan_wifi\" method=\"post\">";
	html += "<label>Koneksi saat ini</label><br>";
	html += "<div style=\"margin-bottom:10px\">" + (WiFi.status() != WL_CONNECTED ? "-- tidak ada --" : esid) + "</div>";
	html += "<label>SSID Tersedia</label><br>";
	html += listWifi;
	html += "<label>Password</label><br>";
	html += "<input type=\"text\" name=\"password\" minlength=\"8\" placeholder=\"Kosongkan jika tidak ada password\"><br>";
	html += "<button type =\"button\" onclick=\"location.reload()\">REFRESH</button>&nbsp;";
	html += "<button type =\"submit\">SIMPAN</button>&nbsp;";
	html += "</form>";
	html += "</div>";
	html += "<script>";
	html += "function changeSSID(self){";
	html += "if(self.value == 'opsi-manual-ssid')";
	html += "{document.getElementById('ssid-manual').style.display = 'block';}";
	html += "else";
	html += "{document.getElementById('ssid-manual').style.display = 'none';}";
	html += "}";
	html += "</script>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void handleSimpanWifi()
{
	String err = "";
	String buf = server.arg("ssid");
	if (buf == "opsi-manual-ssid")
		buf = server.arg("ssid-manual");
	if (buf.length() > 0)
		writeEEPROM("ssid", buf);
	else
		err += "Error : SSID tidak valid";

	buf = server.arg("password");
	if (buf.length() >= 8)
		writeEEPROM("pass", buf);
	else if (buf.length() == 0)
		writeEEPROM("pass", "");
	else
		err += "Password : SSID tidak valid";

	String html = header;
	html += "<body>";
	html += "<h4 class=\"header\">Menyimpan</h4>";
	html += "<div class=\"content\">";
	html += "<br>Data telah disimpan.<br>Harap tunggu, device akan restart...<br>";
	html += err;
	html += "<script type=\"text/javascript\">";
	html += "setTimeout(function(){ window.location.href = \"../\"},10000);";
	html += "</script>";
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
	delay(1000);
	restartBoard();
}

void handleHome()
{
	String html = header;
	html += "<body>";
	html += "<h4 class=\"header\">HOME</h4>";
	html += menu;
	html += "<div class=\"content\">";
	html += "<div class=\"info\">";
	html += "<h4 style=\"margin:0 0 10px;padding-bottom:2px;border-bottom:0.9px solid #c7dbeb\">Mini JWS</h4>";
	html += "<div style=\"margin-bottom: 8px;\">Mini JWS (Jam Waktu Sholat) dengan NTP Client</div>";
	html += "<label>Version</label>";
	html += "<div class=\"text-label\">1.0.0 (Sept 2023)</div>";
	html += "<label>Hardware</label>";
	html += "<div class=\"text-label\">Wemos (ESP8266) & Matrix Led MAX7219</div>";
	html += "<label>Programmer</label>";
	html += "<div class=\"text-label\"><a href=\"mailto:fahroniganteng@gmail.com\">fahroni|ganteng</a></div>";
	html += "<label>License</label>";
	html += "<div class=\"text-label\">MIT (cek github saya untuk mendapatkan source code)</div>";
	html += "<div style=\"height: 35px;margin-top: 25px;\">";
	html += iconGit;
	html += "<div style=\"padding: 6px 10px 0;float:left\"><a href=\"https://github.com/fahroniganteng\" target=\"_blank\">Fork me on GitHub</a></div>";
	html += "</div>";
	html += "<div style=\"height: 35px;\">";
	html += iconYoutube;
	html += "<div style=\"padding: 8px 10px 0;float:left\"><a href=\"https://www.youtube.com/c/FahroniGanteng\" target=\"_blank\">YouTube Channel</a></div>";
	html += "</div>";
	html += "</div>";
	html += "</div>";
	html += "</body>";
	html += "</html>";
	server.send(200, "text/html", html);
}

void serverOnHandleClient()
{
	server.on("/", handleHome);
	server.on("/restart", handleRestart);
	server.on("/reset", handleReset);
	server.on("/simpan_reset", handleSimpanReset);
	server.on("/simpan", handleSimpan);
	server.on("/simpan_wifi", handleSimpanWifi);
	server.on("/jws", handleJws);
	server.on("/wifi", handleWifi);
	server.on("/setting", handleSetting);

	server.onNotFound([]()
										{
		String message = "Address Not Found\n\n";
		message += "URI: ";
		message += server.uri();
		message += "\nMethod: ";
		message += (server.method() == HTTP_GET) ? "GET" : "POST";
		message += "\nArguments: ";
		message += server.args();
		message += "\n";
		for (uint8_t i = 0; i < server.args(); i++){
			message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
		}
		Serial.println(message);
		server.send(404, "text/plain", message); });
}
