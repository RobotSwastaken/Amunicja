#include <Adafruit_LIS3DH.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>

TinyGPS gps; // Reprezentacja modułu GPS
Adafruit_LIS3DH ada = Adafruit_LIS3DH(); // Reprezentacja akcelerometru

SoftwareSerial nss(2, 3); // RX, TX Softwarowy port UART wykorzystywany do komunikajci z modułem GPS
SoftwareSerial mySerial(A2, A3); // RX, TX Softwarowy port UART wykorzystywany do komunikajci z modułem GSM

int _pinGSMOn = A1; //Pin do podłącznienia pinu POWER_ON z modułu GSM
int _pin = A0; //Pin umożliwiające podpięcie diody, sygnalizującej niektóre zdrarzenie w urządzeniu
int addr = 0x18; //Adres akcelerometru
int x,y,xl,yl; //Zmienne przechowujące wartości odczytane przez akcelerometr

#define DIFFERENCE 3000 //Ustawienie czulosci akcelerometru
#define X_Axis1 0x28 
#define X_Axis2 0x29


void Blink() // Funkcja migająca diodą podłączoną pod zmienną _pin
{
  digitalWrite(_pin, LOW);
  delay(1000);

  digitalWrite(_pin, HIGH);
  delay(1000);
}

void GSM_G510_init() //Funkcja rozpoczyna działanie modułu GSM
{

  mySerial.begin(9600);
  pinMode(_pinGSMOn, OUTPUT);
  digitalWrite(_pinGSMOn, LOW);

  Serial.println("Laczenie z siecia GSM");

  delay(20000);
}

boolean GSM_G510_isConnecting() // Funkcja sprawdza czy moduł GSM jest połączony z siecią
{

  if (GSM_G510_getSignalStrength() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

float GSM_G510_getSignalStrength() // Funkcja zwraca siłe sygnału sieci GSM
{
  mySerial.flush();
  mySerial.write("\r\n");
  delay(100);
  mySerial.write("AT+CSQ\r\n");
  mySerial.flush();

  return mySerial.parseFloat();
}

boolean GSM_G510_sendSms(char* phoneNumber, char* message) { // Funkcja wysyłająca wiadomości SMS

  mySerial.setTimeout(10000);

  mySerial.flush();
  mySerial.write("\r\n");
  delay(10);

  mySerial.write("AT+CMGF=1\r\n");

  if (mySerial.findUntil("OK", "ERROR\r\n")) 
  {
    mySerial.flush();

    mySerial.write("AT+CMGS=\"");
    mySerial.write(phoneNumber);
    mySerial.write("\"\r\n");
    mySerial.flush();

    if (mySerial.findUntil(">", "ERROR\r\n")) 
    {

      mySerial.write(message);
      mySerial.write("\x1A\r\n");

      mySerial.setTimeout(10000);
      mySerial.flush();

      if (mySerial.findUntil("+CMGS: ", "ERROR\r\n")) 
      {
        mySerial.setTimeout(1000);
        return true;
      }
    }
  }

  mySerial.setTimeout(1000);
  return false;
}

void setup() { //Funkcja wykonywana raz na początku działania urządzenia
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  GSM_G510_init();
  nss.begin(9600);
  
   pinMode(_pin, OUTPUT);
   digitalWrite(_pin, LOW);
   ada.begin();
   ada.read();
   
   xl=abs(ada.x);
   yl=abs(ada.y);
}

void loop() { //Funcja wykonywana w pętli po wykonaniu funkcji setup
  
   long lat, lon;
   unsigned long fix_age;
    
   ada.begin();
   ada.read();
   
   x=abs(ada.x);
   y=abs(ada.y);
  





if(x>xl+DIFFERENCE||x<xl-DIFFERENCE||y>yl+DIFFERENCE||y<yl-DIFFERENCE)
{
  nss.begin(9600);
  digitalWrite(_pin, HIGH);
  while(1){
    if (nss.available()){
      int c = nss.read();
      Serial.println(c);
      if (gps.encode(c)){
         gps.get_position(&lat, &lon, &fix_age);
        break;
      }
    }else{
      Serial.println("not avaliable");
    }
  }
  
  
  char szerokosc [12];
      ltoa(lat,szerokosc, 10);
      char dlugosc [12];
      ltoa(lon,dlugosc, 10);
      
      
      char tresc[200] = "Twoj pojazd znajduje sie na nastepujacych wspolrzednych: Szerokosc= "; 
      strcat(tresc,szerokosc);
      strcat(tresc," Dlugosc= ");
      strcat(tresc,dlugosc);
      
  int _Counter = 0;
  int _CounterMax = 10;
  delay(1000);
   GSM_G510_init();
  while (!GSM_G510_sendSms("+48601941981", tresc))
  {
    _Counter++;
    
    Serial.println("Proba wyslania sms");
    delay(500);
    if(_Counter >= _CounterMax)
    {
      break;
    }
    
  }
  if(_Counter < _CounterMax)
  {
    Serial.println("Wyslano sms");
  }
  else
  {
      Serial.println("Blad wysylania");
  }
  
  delay(300000);
  ada.begin();
   ada.read();
   
   x=abs(ada.x);
   y=abs(ada.y);
  }
else{
   digitalWrite(_pin, LOW);
  }
  xl=x;
  yl=y;

}
