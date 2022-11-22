#include <SoftwareSerial.h>

#define no_dust 0.35

int dustout=A0;
int v_led=7;

float vo_value=0;

float sensor_voltage=0;

float dust_density=0;

char dust [10];

SoftwareSerial s(2,3);   
void setup() {
  Serial.begin(115200);
  s.begin(115200);
  pinMode(v_led,OUTPUT); 
}
void loop() {
 digitalWrite(v_led,LOW); 
 delayMicroseconds(280); 
 vo_value=analogRead(dustout); 
 delayMicroseconds(40); 
 digitalWrite(v_led,HIGH);
 delayMicroseconds(9680); 

 sensor_voltage=get_voltage(vo_value);
 dust_density=get_dust_density(sensor_voltage);

  Serial.print("Dust Density = ");
  Serial.print(dust_density);
  Serial.println(" [ug/m^3]");
  sprintf(dust,"%.f",dust_density);
  //Serial.print(dust);

  s.print(dust_density);

  delay(2000);
}

float get_voltage(float value)
{
 float V= value * 5.0 / 1024; 
 return V;
}

float get_dust_density(float voltage)
{
 float dust=(voltage-no_dust) / 0.005;
 return dust;
}
