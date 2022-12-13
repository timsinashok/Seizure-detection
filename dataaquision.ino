#include <M5Core2.h>

#define WINDOW_SIZE 8

float x(0), y(0), z(0);

float sumx = 0, sumy = 0, sumz = 0;

int n(0),m(0);

void setup(){
     M5.begin();
     Serial.begin(9600);
     M5.IMU.Init();
}
void loop(){
  M5.Lcd.print("hi");
    while(m < 16){
        while(n<10){
            M5.IMU.getAccelData(&x, &y, &z);
            sumx = sumx + x;
            sumy = sumy + y;
            sumz = sumz + z;
            delay(25);
            n++;
        }
        n = 0;
        Serial.print(sumx/10);
        Serial.print(", ");
        Serial.print(sumy/10);
        Serial.print(", ");
        Serial.print(sumz/10);
        Serial.print(", ");
        sumx = 0;
        sumy = 0;
        sumz = 0;
        m++;
    }
    m = 0;
    Serial.print("\n");

}