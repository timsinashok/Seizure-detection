#include <M5Core2.h> 
using namespace std;
float x, y, z;
void setup(){M5.begin();
M5.IMU.Init( );
Serial.begin(9600);} // initializing serial monitor

#define WINDOW_SIZE 8 // we are using window size of 8 for moving average

int I = 0;
float sumx = 0, sumy = 0, sumz = 0;
float xValues[WINDOW_SIZE];
float yValues[WINDOW_SIZE];
float zValues[WINDOW_SIZE];
float xAveraged = 0;
float yAveraged = 0;
float zAveraged = 0;

void loop(){
  
  // keeping track of the sum of required values for moving average
  sumx = sumx - xValues[I];
  sumy = sumy - yValues[I];
  sumz = sumz - zValues[I];

  M5.IMU.getAccelData(&x, &y, &z); // getting value form IMU
  
  // updating old value with new value for moving average 
  xValues[I] = x;
  yValues[I] = y;
  zValues[I] = z;

  sumx = sumx + x;
  sumy = sumy + y;
  sumz = sumz + z;

  xAveraged = sumx / WINDOW_SIZE;
  yAveraged = sumy / WINDOW_SIZE;
  zAveraged = sumz / WINDOW_SIZE; 

  I = (I+1) % WINDOW_SIZE; // the most important step where we get the index of oldest data which will be removed for new moving average

  Serial.print(xAveraged);
  Serial.print(" ");
  Serial.print(yAveraged);
  Serial.print(" ");
  Serial.print(zAveraged);
  Serial.print("\n");
  
  //Serial.println(x);
  delay(25);
}

