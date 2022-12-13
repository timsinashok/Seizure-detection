#include <M5Core2.h> 
using namespace std;
float x, y, z;
void setup(){M5.begin();
M5.IMU.Init( );
Serial.begin(9600);}

#define WINDOW_SIZE 8

int I = 0;
float sumx = 0, sumy = 0, sumz = 0;
float xValues[WINDOW_SIZE];
float yValues[WINDOW_SIZE];
float zValues[WINDOW_SIZE];
float xAveraged = 0;
float yAveraged = 0;
float zAveraged = 0;

void loop(){
  
  sumx = sumx - xValues[I];
  sumy = sumy - yValues[I];
  sumz = sumz - zValues[I];

  M5.IMU.getAccelData(&x, &y, &z);
  
  xValues[I] = x;
  yValues[I] = y;
  zValues[I] = z;

  sumx = sumx + x;
  sumy = sumy + y;
  sumz = sumz + z;

  xAveraged = sumx / WINDOW_SIZE;
  yAveraged = sumy / WINDOW_SIZE;
  zAveraged = sumz / WINDOW_SIZE; 

  I = (I+1) % WINDOW_SIZE;

  Serial.print(xAveraged);
  Serial.print(" ");
  Serial.print(yAveraged);
  Serial.print(" ");
  Serial.print(zAveraged);
  Serial.print("\n");
  
  //Serial.println(x);
  delay(25);
}

