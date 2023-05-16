#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTTClient.h>
#include <FirebaseArduino.h>

const char *ssid = "hzrhscn";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "hzrhscn1183";           // bağlanılacak olan kablosuz ağın şifresi

WiFiClient wifi_istemci;                     // wifi istemci nesnesi oluşturuldu
MQTTClient mqtt_istemci;                     // MQTT istemci nesnesi oluşturuldu

#define firebase_erisim_linki "final-one-3269e-default-rtdb.firebaseio.com"   // oluşturduğunuz projenizdeki veritabanınızın erişim linki
#define firebase_sifre "pXUHmkLRXiJdg4i4j0UtOuM13jyhgiinkLPrC1FL"    // veritabanı secret key

int x=0;
int y=2016;
//------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);                        //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  
  WiFi.mode(WIFI_STA);                       //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                //SSID ve şifre girilierek wifi başlatılıyor.

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

  mqtt_istemci.begin("final-ust.cloud.shiftr.io", wifi_istemci); // mqtt istemci nesnesini başlat. ilk parametre web adresi
                                                              // ikinci parametre bağlantı kurulurken kullanılacak wifi nesnesi
  mqtt_istemci.onMessage(mesaj_geldi);                        // abone olunan konudan veri gelince mesaj_geldi fonksiyonuna git
  Firebase.begin(firebase_erisim_linki, firebase_sifre);      // Firebase erişimini başlat
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt_istemci.loop();              // Mesaj göndermek ve almak için bu loop fonksiyonunu her döngü içerisinde çalıştırmak lazım
  delay(100);                       // 100 ms bekleme
  
  if(!mqtt_istemci.connected())     // bağlantı kurulmamış mı?
    baglan();                       // kurulmadıysa baglan fonksiyonuna git

  delay(1000);                      // 1 sn bekleme
  
  
  Firebase.setInt("201913172016/sonuc = ", 2*x+y+89);               // formüldeki islemi yap ve sonucu 201913172016 etiketine yazdır

  if (Firebase.failed()) {                              // yazma hatası oldu mu?
      Serial.print("Hata Var:");                        // evet olmuş, seri porttan bana bildir
      Serial.println(Firebase.error());  
      return;                                           // loop başına dön
  }
  delay(1000); 
  
}
//-------------------------------------------------------------
void baglan()
{
  // mqtt_istemci.connect("xxx", "yyy", "zzz") burada xxx bağlandığınızda görünen adınız, yyy projenin adı, zzz oluşturduğunuz token 
  while (!mqtt_istemci.connect("201913172016", "final-ust", "zrTbgbfClC8b8iWD"))  // projeye bağlanamadı mı?
  {                                                                    // evet bağlanamadı 1sn bekle tekrar kontrol et
    Serial.print(".");
    delay(1000);
  }
  mqtt_istemci.subscribe("amfi/ust");       // amfi/ust konusuna abone ol
}


//----------------- abone olunan konudan veri geldi --------------------------------
void mesaj_geldi(String &konu, String &veri)      // bu fonksiyona 2 parametre geliyor, abone olunan konu bilgisi ve oraya gelen bilgi
{
   Serial.println(konu+":"+veri);    // gelen veri ve konuyu seri porttan da gönder
}
