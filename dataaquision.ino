// this scirpt basically prints continuous stream of data on the LCD of m5 stack screen that can be copied to creat a csv file.

#include <M5Core2.h>

#define WINDOW_SIZE 8 // window size for moving average 

float x(0), y(0), z(0);

float sumx = 0, sumy = 0, sumz = 0;

int n(0),m(0);

void setup(){
     M5.begin(); // initializing m5
     Serial.begin(9600); // initializing the serial monitor
     M5.IMU.Init(); // initializing the imu sensor
}
void loop(){
    while(m < 16){ // running the loop 16 times, taking 4 readings every second
        while(n<10){ // taking average of 10 values that are taken in duration of 25 milliseconds 
            M5.IMU.getAccelData(&x, &y, &z);
            sumx = sumx + x;
            sumy = sumy + y;
            sumz = sumz + z;
            delay(25); // 25 millisecond delay
            n++;
        }
        n = 0;
         // printing the values
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
