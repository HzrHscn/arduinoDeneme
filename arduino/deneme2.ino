#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ThingSpeak.h"
#include <FirebaseArduino.h>

const char *ssid = "hzrhscn";          // bağlanılacak olan kablosuz ağın SSID si
const char *password = "hzrhscn1183";           // bağlanılacak olan kablosuz ağın şifresi

unsigned long channel_ID = 1629499;           // thingspeak sitesindeki Channel numaranız
const char * myReadAPIKey = "30X0WJQW1G0JFDJ6";  // thingspeak sitesindeki read api keyiniz

#define firebase_erisim_linki "final-two-default-rtdb.firebaseio.com"   // oluşturduğunuz projenizdeki veritabanınızın erişim linki
#define firebase_sifre "tAFTQaKABFkyyPKaOcHy3yWeo24nKzWm3F8aa2zR"    // veritabanı secret key

int x=0; //x'i tanımladım.
int y=2016; //ögrenci numara son 4 hanesi

WiFiClient wifi_istemci;                       // istemci nesnesi oluşturuldu


//------------------------------------------------------------------------------------
void setup() 
{
  pinMode(D0,OUTPUT);     // D0 pinini çıkış yap
  digitalWrite(D0,LOW);   // D0 ı başlangıç için low yap
  
  Serial.begin(9600);                       //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  
  WiFi.mode(WIFI_STA);                        //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                 //SSID ve şifre girilierek wifi başlatılıyor.

  //------- Wifi ağına bağlanıncaya kadar beklenilen kısım ------------
  Serial.print("Bağlantı kuruluyor");
  while (WiFi.status() != WL_CONNECTED) 
  { // Wifi bağlantısı sağlanana kadar bekleniyor.
    delay(500);                               // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                        //görsellik sağlanıyor.
  }
  //-------------------------------------------------------------------
  
  Serial.println("");                         //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");      //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.

  Serial.print("Alınan IP addresi: ");        // kablosuz ağdan alınan IP adresi
  Serial.println(WiFi.localIP());             // kablosuz ağdan alınan IP adresi
  ThingSpeak.begin(wifi_istemci);             // Thingspeak bağlantısını başlat
  Firebase.begin(firebase_erisim_linki, firebase_sifre);      // Firebase erişimini başlat
}

void loop() {
  // put your main code here, to run repeatedly:

  x=ThingSpeak.readIntField(channel_ID, 5, myReadAPIKey); // alan 5 i oku
  delay (5000);//her 5 saniyede bir veri okuma
  
  Firebase.setInt("201913172016/sonuc = ", 3*x+y);               // formüldeki islemi yap ve sonucu yazdır

  if (Firebase.failed()) {                              // yazma hatası oldu mu?
      Serial.print("Hata Var:");                        // evet olmuş, seri porttan bana bildir
      Serial.println(Firebase.error());  
      return;                                           // loop başına dön
  }
  delay(1000); 
  
}
