/**
 * @file ronnWifi.h
 * @author fahroni|ganteng (fahroniganteng@gmail.com)
 * *************************************************************
 * Fungsi buat handle wifi
 *
 */

const char *ssid = "JAM-JWS";
const char *passphrase = ""; // no pass...
const char *wifiHostName = "Mini-JWS";
IPAddress local_IP(10, 10, 10, 10);
IPAddress gateway(10, 10, 10, 10);
IPAddress subnet(255, 255, 255, 0);

// Variable wifi
String esid;     // eeprom SSID
String epass;    // eeprom password wifi
String listWifi; // buat nyimpan data list wifi yang available (dipanggil di HTTP req)
bool apMode = false;
int softap_stations_cnt; // jumlah client yang konek pada saat jadi access point (softAP)
String locIP;            // local IP (IP jam setelah konek wifi)

// Set web server
ESP8266WebServer server(80);

void scanWifi()
{
   listWifi = "";
   int n = WiFi.scanNetworks();
   Serial.println("scan done");
   if (n == 0)
   {
      Serial.println("no networks found");
   }
   else
   {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
         // Print SSID and RSSI for each network found
         Serial.print(i + 1);
         Serial.print(": ");
         Serial.print(WiFi.SSID(i));
         Serial.print(" (");
         Serial.print(WiFi.RSSI(i));
         Serial.print(")");
         Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
         delay(10);
      }

      listWifi += "<select name=\"ssid\" style=\"margin-bottom:0\" onchange=\"changeSSID(this)\">";
      for (int i = 0; i < n; ++i)
      {
         // Print SSID and RSSI for each network found
         String signalRSSI;
         if (WiFi.RSSI(i) >= -50)
            signalRSSI = "&#9679;&#9679;&#9679;&#9679;&#9679;";
         else if (WiFi.RSSI(i) >= -60)
            signalRSSI = "&#9679;&#9679;&#9679;&#9679;&#9675;";
         else if (WiFi.RSSI(i) >= -67)
            signalRSSI = "&#9679;&#9679;&#9679;&#9675;&#9675;";
         else if (WiFi.RSSI(i) >= -70)
            signalRSSI = "&#9679;&#9679;&#9675;&#9675;&#9675;";
         else
            signalRSSI = "&#9679;&#9675;&#9675;&#9675;&#9675;";

         listWifi += "<option value=\"" + WiFi.SSID(i) + "\">";
         listWifi += WiFi.SSID(i);
         listWifi += "&nbsp;<sup>" + signalRSSI + "</sup>";
         listWifi += "</option>";
      }
      listWifi += "<option value=\"opsi-manual-ssid\">MANUAL SSID</option>";
      listWifi += "</select>";
      listWifi += "<br><small>*lingkaran sebelah SSID menandakan kekuatan signal</small><br>";
      listWifi += "<input style=\"margin-top:5px;display:none\" type=\"text\" name=\"ssid-manual\" id=\"ssid-manual\" placeholder=\"Nama SSID (termasuk hidden SSID)\">";
   }
   Serial.println("");
   delay(100);
}

void setupAP(void)
{ // jadiin access point
   clearRight();
   mx.setFont(font_ICON);
   printText("8", 1);
   mx.setFont(font_3X5); // ganti font
   scrollDown(14, 32, "OFF");

   if (WiFi.status() == WL_CONNECTED)
      WiFi.disconnect();
   WiFi.mode(WIFI_AP);
   WiFi.hostname(wifiHostName);
   delay(10);
   // scanWifi();
   Serial.print("AP Config... ");
   Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
   // WiFi.softAP(ssid, passphrase);
   Serial.print("AP Start... ");
   if (WiFi.softAP(ssid, passphrase))
   {
      Serial.println("Ready");
      apMode = true;
      delay(500);
      clearLeft();
      mx.setFont(font_3X5); // ganti font
      scrollDown(5, 32, "AP MODE");
      delay(1000);
      clearLeft();
      scrollDown(9, 32, "SSID :");
      delay(500);
      clearLeft();
      scrollDown(3, 32, String(ssid));
      delay(1000);
      clearLeft();
      scrollDown(7, 32, "ADMIN :");
      delay(500);
      clearLeft();
      mx.setFont(font_default); // default font
      scrollText("http://10.10.10.10/           ");
      clearLeft();
      animIcon("E");
      mx.setFont(font_3X5); // ganti font
      scrollDown(13, 32, "SET...");
   }
   else
   {
      Serial.println("Failed! Coba ulang...");
      apMode = false;
      setupAP();
   }
}

void connectWifi()
{
   if (WiFi.status() == WL_CONNECTED)
      WiFi.disconnect();

   Serial.print("Reading ssid : ");
   esid = readEEPROM("ssid");
   epass = readEEPROM("pass");
   /**
    * buffer epass & esid = 50
    * so, if length is 50 maybe is broken eeprom data
    **/
   if (esid == "__DEFAULT__" || esid.length() == 50 || epass == "__DEFAULT__" || epass.length() == 50 || epass.length() < 8)
   {
      esid = "__DEFAULT__";
      epass = "__DEFAULT__";
      Serial.println(esid);
      Serial.println("SSID / PASSWORD belum diisi, mode access point..");
      setupAP();
      Serial.print("SoftAP IP: ");
      Serial.println(WiFi.softAPIP());
      tmr.oscillate(LED_BUILTIN, 50, HIGH, 5);
      delay(100);
      return;
   }
   Serial.println(esid);

   // wifi mode : WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA
   WiFi.mode(WIFI_STA);
   WiFi.hostname(wifiHostName);
   int connectLoop = 0;
   WiFi.begin(esid.c_str(), epass.c_str());

   mx.clear();
   mx.setFont(font_3X5); // ganti font
   scrollDown(10, 32, "WAIT...");
   String wifiIcon = "456";
   mx.setFont(font_ICON);
   while (WiFi.status() != WL_CONNECTED && connectLoop <= 50)
   { // 20*500ms= 10 detik max connect time
      printText(String(wifiIcon[connectLoop % 3]), 1);
      delay(200);
      Serial.print(".");
      connectLoop++;
   }
   if (WiFi.status() != WL_CONNECTED)
   {
      WiFi.disconnect();
      Serial.println("Cannot connect WiFi");
      setupAP();
      Serial.print("SoftAP IP: ");
      Serial.println(WiFi.softAPIP());
      tmr.oscillate(LED_BUILTIN, 50, HIGH, 5);
      delay(100);
   }
   else
   {
      Serial.println();
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      locIP = WiFi.localIP().toString();
      Serial.println(locIP);
      tmr.oscillate(LED_BUILTIN, 50, HIGH, 3);
      apMode = false;
      clearLeft();
      mx.setFont(font_3X5); // ganti font
      scrollText("IP : " + locIP + "                               ");
   }
}
void reConnectWifi()
{
   if (
       WiFi.status() != WL_CONNECTED && // wifi nggak konek
       esid != "__DEFAULT__" &&         // ssid (EEPROM) sudah di setting
       softap_stations_cnt < 1          // nggak ada cient yang konek (pas jadi access point), jika ada yang konek diasumsikan lagi di setting
   )
      connectWifi();
}