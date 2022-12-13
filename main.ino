#include <M5Core2.h> // For using functions specific to m5stack core 2
#include "WiFi.h" // Wifi library
#include "time.h" // Time library
#include <EMailSender.h> // sending email
#include <string> // string libary is required to print time through sprite

using namespace std;


#define WIDTH  320
#define HEIGHT 240

TFT_eSprite spr = TFT_eSprite(&M5.Lcd); // creating a sprite for the Mt core 2 Lcd

// function prototypes
void get_time(int& hour, int& minute, int& second);
void detector();
void alert();

// float sumx(0), sumy(0), sumz(0);

char buttonPressed = 'B'; // default button



// setting up credentials for wifi
const char* ssid = "NETGEAR25";
const char* pass = "happyearth088";

#include "time.h" // Time library
RTC_TimeTypeDef TimeStruct; // structure to store time data
// setup for getting it
const char* time_server = "time1.aliyun.com";  //time server to get time in UTC
const long gmtOffset_sec = 14400; // offset to get time of Abu Dhabi
const int daylightOffset_sec = 0;  

void get_time(int& hour, int& minute, int& second){
  struct tm time; // using time structure defined in time.h for accessing data from  the timeserver
  getLocalTime(&time); // This code gets time form the time-server we defined earlier and stores it to the structure time
  // dropping the value form time structure to specific variable
  delay(2000);
  second = time.tm_sec;
  minute = time.tm_min;
  hour = time.tm_hour;
}

// variables to store the accelermeter data
float ax;
float ay;
float az;

// variables to keep track of time
int i_time, f_time;

// Email setup
EMailSender emailSend("melearner9@gmail.com", "ghmuaoemvwtoasth");


// window size for moving average fileter
#define WINDOW_SIZE 8

// initialization for the moving average filter
int I = 0;
float sumx = 0, sumy = 0, sumz = 0;
float xValues[WINDOW_SIZE];// array to store values that are relevant for the moving average
float yValues[WINDOW_SIZE];
float zValues[WINDOW_SIZE];
float xAveraged = 0;
float yAveraged = 0;
float zAveraged = 0;




void setup() {

  M5.begin();
  SD.begin(); // initializing sd card
    // initializing motion sensors
  M5.IMU.Init();
// serial is used to debug
  Serial.begin(9600);
  WiFi.begin(ssid, pass); // connecting to the wifi
  delay(2000);

  configTime(gmtOffset_sec, daylightOffset_sec, time_server); // initializing and configuring time according to our variables
  int hour, minute, second;

  get_time(hour,minute,second); // passing variables by reference to get_time function that will store current time in the variables

  // dropping real time we get into the TimeStruct i.e. the strucutre we defined earlier in the code  
  TimeStruct.Hours   = hour;
  TimeStruct.Minutes = minute;
  TimeStruct.Seconds = second;

  // updating the real time we got to the RTC module of M5 stack
  M5.Rtc.SetTime(&TimeStruct); 
  
  M5.Rtc.GetTime(&TimeStruct); // getting the current time and setting it as initial time
  i_time = TimeStruct.Seconds;  
  
  spr.createSprite(WIDTH, HEIGHT);
}

void loop() {
  
  M5.update(); // updating to get the current buttion states
  // checking for the buttion press
  if(M5.BtnA.isPressed()== 1){
    buttonPressed = 'A';
  }
  if(M5.BtnB.isPressed() == 1){
    buttonPressed = 'B';
    }
  if(M5.BtnC.isPresse d() == 1){
    buttonPressed = 'C';
  }

 M5.Lcd.setCursor(20, 20);
  
  // Schedule
  if (buttonPressed == 'A') {    //If the key is pressed. 
    M5.Lcd.setTextSize(2);
  File F =SD.open("/Schedule.txt", FILE_READ); // opening the file to read the schedule
  if(F){
    while (F.available()) { // looping until the file has data
      M5.Lcd.write(F.read()); // printing the data on the screen
    }
    // spr.pushSprite(0,0);
  }
  else{
    M5.Lcd.print("Error opening the file.");
  }
    // checking for new button press 
    if(M5.BtnB.isPressed() == 1){
      buttonPressed = 'B';
      
    }
     if(M5.BtnC.isPressed() == 1){
      buttonPressed = 'C';  
    }
  // getting the initial time every time so that the dector gets accurate initial time every time
  M5.Rtc.GetTime(&TimeStruct); // getting the current time and setting it as initial time
  i_time = TimeStruct.Seconds;
  }

  // clock/ main screen
  if (buttonPressed == 'B') {    //If the key is pressed. 
  spr.fillSprite(BLACK); // making screen black
  // Printing the time
  M5.Rtc.GetTime(&TimeStruct);// accessing hour minute and second value respectively from the structure
  string h = to_string(TimeStruct.Hours); // converting to string
  char const *h_char = h.c_str(); // using pointer to print the value using pointer 
  //printing hour and colon
  spr.drawString(h_char, 35, 80, 6);
  spr.drawString(": ", 90,  80, 6);
  string m = to_string(TimeStruct.Minutes);
  char const *m_char = m.c_str();
  spr.drawString(m_char, 110, 80, 6);
  spr.drawString(": ", 170, 80, 6);
  
  string s = to_string(TimeStruct.Seconds);
  char const *s_char = s.c_str();
  spr.drawString(s_char, 190, 80, 6);
  
  spr.drawString("Schedule",10, 220, 4);
  spr.drawString("Sleep mode", 190, 220, 4);

  spr.pushSprite(0, 0); // pusing the sprite;
    if(M5.BtnA.isPressed() == 1){ // checking for other buttion press
      buttonPressed = 'A';
    }
     if(M5.BtnC.isPressed() == 1){
      buttonPressed = 'C';
    }
    // again updating the intitial time 
    M5.Rtc.GetTime(&TimeStruct); // getting the current time and setting it as initial time
  i_time = TimeStruct.Seconds;
  }
 // Serial.println(buttonPressed);
  
  // seizure dtection
  if (buttonPressed == 'C') {    //If the key is pressed.
  spr.fillSprite(BLACK); 
    spr.drawString("Sleep Mode is on", WIDTH/4, HEIGHT / 3, 4);
    spr.pushSprite(0, 0);
    M5.Lcd.setTextSize(15); 
    

/// implementing moving average filter

// getting sum of values only associated with the current window
  sumx = sumx - xValues[I];
  sumy = sumy - yValues[I];
  sumz = sumz - zValues[I];

  M5.IMU.getAccelData(&ax, &ay, &az); // getting acceleratoin data from IMU
  
  // Adding new value to the Ith index
  xValues[I] = ax;
  yValues[I] = ay;
  zValues[I] = az;

// getting hte sum
  sumx = sumx + ax;
  sumy = sumy + ay;
  sumz = sumz + az;

// calculating the average
  xAveraged = sumx / WINDOW_SIZE;
  yAveraged = sumy / WINDOW_SIZE;
  zAveraged = sumz / WINDOW_SIZE; 

  I = (I+1) % WINDOW_SIZE;  // getting the index which is 0 to windowsize-1, this will make possible to only take value associated with current moving average

    // checking if acceleration in any direction is gretaer than 1.1
    if(abs(xAveraged) > 0.22 || abs(yAveraged) > 0.22 || abs(zAveraged) > 1.22){
      // looking at the current time 
      M5.Rtc.GetTime(&TimeStruct); 
      f_time = TimeStruct.Seconds;
      // delay(1000);
      // if the acceleration in any direction is greater than 1.1 for more than 4 seconds, we consider it a seizure
      if(f_time-i_time>4){
        spr.fillSprite(TFT_RED);
        spr.drawString("Detected", WIDTH / 3, HEIGHT / 3, 4);
        spr.pushSprite(0, 0);
        M5.Lcd.setTextSize(20); 
        M5.Spk.DingDong(); // playing the alert sound
        delay(2000);
        EMailSender::EMailMessage message; // creating instance of message
        // message detail
        message.subject = "Emergency";
        message.message = "Dear Serene your dearest friend xyz is having seizure. "; 
        EMailSender::Response resp = emailSend.send("ssd9756@nyu.edu", message); 
        Serial.println("Sending status: ");
        Serial.println(resp.code);
        Serial.println(resp.desc); 
        Serial.println(resp.status);
        buttonPressed = 'B';
      }
      }
    else{
      // if there is no significant acceleration in any direction, we reset the initial time and continue with the loop
      M5.Rtc.GetTime(&TimeStruct); 
      i_time = TimeStruct.Seconds;

    if(M5.BtnA.isPressed() == 1){t
      buttonPressed = 'A';
    }
     if(M5.BtnB.isPressed() == 1){
      buttonPressed = 'B';
     }
  }
}}


