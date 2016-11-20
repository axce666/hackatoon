#include <SoftwareSerial.h>
#include <Servo.h>
#include <NewPing.h>



#define TRIGGER_PIN  15  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     16  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.



Servo servos[13];

int pos=1;
int plastico=0;
int papel=90;
int cascara=180;

int abrir=120;
int cerrar=165;

unsigned int distancia;
int porcentaje=0;

String tipoDesecho="plastico";

boolean lleno=false;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

char aux_str[50];
char direccion[200]="";
String pet;


void setup()
{
  pinMode(2,OUTPUT);
  servos[9].attach(9);
  digitalWrite(2,HIGH);
  servos[9].write(cerrar);
  delay(500);

  pinMode(A0,INPUT);
  pinMode(7,INPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);

  Serial.begin(9600);
  servos[8].attach(8);
  pinMode(6,OUTPUT);
  

}


void loop()
{
    int jo=analogRead(A0);
    int bo=digitalRead(7);
    if ((jo > 600) || (jo < 400)) {
      if (jo > 600) {
        if (pos < 3) {
          pos=pos + 1;
         }else {
          pos=1;
         }
       }else if (jo < 400) {
        if (pos > 1) {
          pos=pos - 1;
         }else {
          pos=3;
         }
       }
      if (pos == 1) {
        digitalWrite(2,HIGH);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        tipoDesecho = "plastico";
        
       }else if (pos == 2) {
        digitalWrite(3,HIGH);
        digitalWrite(2,LOW);
        digitalWrite(4,LOW);
        tipoDesecho = "papel";
       }else if (pos == 3) {
        digitalWrite(4,HIGH);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        tipoDesecho = "organico";
       }
      delay(250);
     }
    if (bo == 0) {
      delay(250);
      //Serial.println(pos);
      if (pos == 1) {
        servos[8].write(plastico);
        delay(2000);
       }else if (pos == 2) {
        servos[8].write(papel);
        delay(2000);
       }else if (pos == 3) {
        servos[8].write(cascara);
        delay(2000);
       }

      lleno = llenado();

      if(lleno){
        ///api/groups/reciclabot/send.json?papel=12&organico=12&plastico=12
        //Serial.println("/api/groups/reciclabot/send.json?"+tipoDesecho+"=100");
        //Serial.println("/api/groups/reciclabot/send.json?"+tipoDesecho+"=100");
        Serial.println("io.adafruit.com/api/groups/reciclabot/send.json?x-aio-key=228225a0f0d4481a92238d4c4c557971&"+tipoDesecho+"=100");
        digitalWrite(5, HIGH);
        delay(3000);
        digitalWrite(5, LOW);
      }
      else{
        //Serial.println("/api/groups/reciclabot/send.json?"+tipoDesecho+"="+String(porcentaje));
        //Serial.println("/api/groups/reciclabot/send.json?"+tipoDesecho+"="+String(porcentaje));
        Serial.println("io.adafruit.com/api/groups/reciclabot/send.json?x-aio-key=228225a0f0d4481a92238d4c4c557971&"+tipoDesecho+"="+String(porcentaje));
        
        digitalWrite(6,HIGH);
        servos[9].write(abrir);
        delay(4000);
        servos[9].write(cerrar);
        delay(500);
        digitalWrite(6,LOW);
      
      }
       
      
     }

}

boolean llenado(){
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  distancia = uS / US_ROUNDTRIP_CM;
  porcentaje = map(distancia, 44, 19, 0, 100);

  if( distancia < 19)
    return true;

  else
    return false;
}


