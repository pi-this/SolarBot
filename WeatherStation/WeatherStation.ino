#include <Wire.h> // library
#include <LiquidCrystal_I2C.h> // library
using namespace std;LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <BH1750.h> // library
BH1750 lightMeter;

#include <dht11.h> // library
dht11 DHT;
#define DHT11_PIN 7  //define the DHT11 as the digital pin 7

#include <Servo.h> // library
Servo lr_servo; //define the name of the servo rotating right and left
Servo ud_servo; //efine the name of the servo rotating upwards and downwards

const byte button = 2;  //the pin of button

// Define the LEDS AND SPEAKER

int R1 = 8;
int R2 = 3;
int Y1 = 4;
int Y2 = 5;
int G1 = 11;
int G2 = 12;
int SIGNAL = 13;


int lr_angle = 90;//set the initial angle to 90 degree
int ud_angle = 10;//set the initial angle to 10 degree;keep the solar panels upright to detect the strongest light
int l_state = A0;//define the analog voltage input of the photoresistors
int r_state = A1;
int u_state = A2;
int d_state = A3;
const byte buzzer = 6;  //set the pin of the buzzer to digital pin 6
const byte lr_servopin = 9;//define the control signal pin of the servo rotating right and left
const byte ud_servopin = 10;//define the control signal pin of the servo rotating clockwise and anticlockwise 

unsigned int light; //save the variable of light intensity
byte error = 15;//Define the error range to prevent vibration
byte m_speed = 5; //set delay time to adjust the speed of servo; the longer the time, the smaller the speed
byte resolution = 1;   //set the rotation accuracy of the servo, the minimum rotation angle 
int temperature;  //save the variable of temperature
int humidity; //save the variable of humidity
int fahrenheit; //save the variable of fahrenheit

int slide = 1; // three slides start slide, set to 1: Temp.


int LEDpin;

float analogValue;
float input_voltage;

int signalrun(int time1, int time2) { // signal run function
  // signals the run
  digitalWrite(SIGNAL,HIGH);
  delay(time1);
  digitalWrite(SIGNAL,LOW);
  delay(time2);
}


void setup() { // the setup stage

// LED AND SPEAKER SET DIGITAL PINS TO OUTPUT
 pinMode(R1,OUTPUT);
 pinMode(R2,OUTPUT);
 pinMode(Y1,OUTPUT);
 pinMode(Y2,OUTPUT);
 pinMode(G1,OUTPUT);
 pinMode(G2,OUTPUT);
 pinMode(SIGNAL,OUTPUT);

// RUN STARTUP MODE FOR LEDS AND SPEAKER


  signalrun(500,0);
  
  Serial.begin(9600); //define the serial baud rate
  
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  lightMeter.begin();

  lr_servo.attach(lr_servopin);  // set the control pin of servo
  ud_servo.attach(ud_servopin);  // set the control pin of servo
  pinMode(l_state, INPUT); //set the mode of pin
  pinMode(r_state, INPUT);
  pinMode(u_state, INPUT);
  pinMode(d_state, INPUT);

  pinMode(button, INPUT);

  lcd.init();          // initialize the LCD
  
  lcd.backlight();     //set LCD backlight

  lcd.setCursor(5, 0);
  lcd.print("Solar");
  lcd.setCursor(6, 1);
  lcd.print("Bot");

  delay(1000); // wait for one-thousand milliseconds = to 1 second
  
    int numArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int numline[] = {1, 2};

    // use of ranged for loop to display into light show  
    for (int n : numline) {
      for (int n : numArray) {
        if (n == 1) {
          LEDpin = R1;
        }
        else if (n == 2) {
          LEDpin = R2;
        }
        else if (n == 3) {
          LEDpin = Y1;
        }
        else if (n == 4) {
          LEDpin = Y2;
        }
        else if (n == 5) {
          LEDpin = G1;
        }
        else if (n == 6) {
          LEDpin = G2;
        }
        else if (n == 7) {
          signalrun(50,20);
        }
        else if (n == 8) {
          LEDpin = G2;
        }
        else if (n == 9) {
          LEDpin = G1;
        }
        else if (n == 10) {
          LEDpin = Y2;
        }
        else if (n == 11) {
          LEDpin = Y1; //
        }
        else if (n == 12) {
          LEDpin = R2;
        }
        else if (n == 13) {
          LEDpin = R1;
        }
        else if (n == 14) {
          signalrun(100, 75);
        }


      
       digitalWrite(LEDpin,HIGH);
       delay(random(10,100));
       digitalWrite(LEDpin,LOW);
       delay(random(5));
        
    }
    }
    
  lcd.clear();
  
  lcd.setCursor(6, 0);
  lcd.print("By.");
  lcd.setCursor(0, 1);
  lcd.print("Wesley Chastain");
  delay(2500);
  lcd.clear();
  
  signalrun(2, 2);
  tone(buzzer, random(250), random(80)); 
}

void loop() {
  ServoAction();  //servo performs the action
  read_light();   //read the light intensity of bh1750
  read_dht11();   //read the value of temperature and humidity
  LcdShowValue(); //Lcd shows the values of light intensity, temperature and humidity

  /*
  code bellow changes slides:
  */

  if (!digitalRead(button)){
    tone(buzzer, random(200,800), random(2,800));
    lcd.clear();
    if (slide >= 3) {
      slide = 1;
    }
    else{
      slide = slide + 1;
    }
    lcd.clear();
  
  }

}

/**********the function of the servo************/
void ServoAction(){
  int L = analogRead(l_state);//read the analog voltage value of the sensor, 0-1023
  int R = analogRead(r_state);
  int U = analogRead(u_state);
  int D = analogRead(d_state);
  /**********************system adjusting left and right序**********************/
  //  abs() is the absolute value function
  if (abs(L - R) > error && L > R) { //Determine whether the error is within the acceptable range, otherwise adjust the steering gear
    lr_angle -= resolution;//reduce the angle
    //    lr_servo.attach(lr_servopin);  // connect servo
    if (lr_angle < 0) { //limit the rotation angle of the servo
      lr_angle = 0;
    }
    lr_servo.write(lr_angle);  //output the angle of the servooutput the angle of servo
    delay(m_speed);

  }
  else if (abs(L - R) > error && L < R) { //Determine whether the error is within the acceptable range, otherwise adjust the steering gear
    lr_angle += resolution;//increase the angle
    //    lr_servo.attach(lr_servopin);    // connect servo
    if (lr_angle > 180) { //limit the rotation angle of servo
      lr_angle = 180;
    }
    lr_servo.write(lr_angle);  //output the angle of servo
    delay(m_speed);

  }
  else if (abs(L - R) <= error) { //Determine whether the error is within the acceptable range, otherwise adjust the steering gear
    //    lr_servo.detach();  //release the pin of servo
    lr_servo.write(lr_angle); //output the angle of servo
  }
  /**********************system adjusting up and down**********************/
  if (abs(U - D) > error && U >= D) { //Determine whether the error is within the acceptable range, otherwise adjust the steering gear
    ud_angle -= resolution;//reduce the angle
    //    ud_servo.attach(ud_servopin);  // connect servo
    if (ud_angle < 10) { //limit the rotation angle of servo
      ud_angle = 10;
    }
    ud_servo.write(ud_angle);  //output the angle of servo
    delay(m_speed);

  }
  else if (abs(U - D) > error && U < D) { //Determine whether the error is within the acceptable range, otherwise adjust the steering gear
    ud_angle += resolution;//increase the angle
    //    ud_servo.attach(ud_servopin);  // connect servo
    if (ud_angle > 90) { //limit the rotation angle of servo
      ud_angle = 90;
    }
    ud_servo.write(ud_angle);  //output the angle of servo
    delay(m_speed);

  }
  else if (abs(U - D) <= error) { //Determine whether the error is within the acceptable range. If it is, keep it stable and make no change in angle
    //    ud_servo.detach();  //release the pin of servo
    ud_servo.write(ud_angle);  //output the angle of servo
  }
}

void LcdShowValue() {
  char str1[5];
  char str2[2];
  char str3[2];
  dtostrf(light, -5, 0, str1); //Format the light value data as a string, left-aligned
  dtostrf(temperature, -1, 0, str2);
  dtostrf(humidity, -2, 0, str3);
  //LCD1602 display
  //display the value of the light intensity

  if (slide == 1) { 
    fahrenheit = (temperature*1.8)+32;
    int F = fahrenheit;

    lcd.setCursor(8, 0);
    lcd.print(temperature);
    lcd.setCursor(10, 0);
    lcd.print("C");
    lcd.setCursor(0, 0);
    lcd.print("celcius:");
    
    lcd.setCursor(11, 1);
    lcd.print(fahrenheit);
    lcd.setCursor(13, 1);
    lcd.print("F");
    lcd.setCursor(0, 1);
    lcd.print("fahrenheit:");


    if (F <= 32){
      // frezzing or bellow
      digitalWrite(G1,LOW);
      digitalWrite(G2,HIGH);
      digitalWrite(Y1,LOW);
      digitalWrite(Y2,LOW);
      digitalWrite(R1,LOW);
      digitalWrite(R2,LOW);
    }

    else if (F <= 50){
        digitalWrite(G1,HIGH);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }

    else if (F <= 70){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,HIGH);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }

    else if (F <= 100){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,HIGH);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }
    
    else if (F <= 211){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,HIGH);
      }
     else if (F >= 212){
      // boiling or over
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,LOW);
      }
      
  


    
  }

  if (slide == 2) {


    //display the value of temperature and humidity
    lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  lcd.setCursor(12, 0);
  lcd.print(" %");
  lcd.setCursor(10, 0);
  lcd.print(humidity);

  int H = humidity;

  if (H < 30) {
    lcd.setCursor(0, 1);
    lcd.print("low humidity!                     ");
  }
  else if (H <= 50) {
    lcd.setCursor(0, 1);
    lcd.print("good air                  ");
  }
  else if (H < 55) {
    lcd.setCursor(0, 1);
    lcd.print("high humidity!             ");
  }
  else if (H >= 55) {
    lcd.setCursor(0, 1);
    lcd.print("mold can grow!              ");
  }
    
    if (H <= 10){
      digitalWrite(G1,LOW);
      digitalWrite(G2,HIGH);
      digitalWrite(Y1,LOW);
      digitalWrite(Y2,LOW);
      digitalWrite(R1,LOW);
      digitalWrite(R2,LOW);
    }

    else if (H <= 30){
        digitalWrite(G1,HIGH);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }

    else if (H <= 50){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,HIGH);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }

    else if (H <= 60){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,HIGH);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }
    
    else if (H <= 80){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,HIGH);
      }
     else if (H <= 100 or H > 100){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,LOW);
      }


  

  
  }
  if (slide == 3) {
  
  //display the value of temperature and humidity
  lcd.setCursor(0, 0);
  lcd.print("Light:");
  lcd.setCursor(9, 0);
  lcd.print("    lux");
  lcd.setCursor(6, 0);
  lcd.print(light);

  int L = light;

  if (L <= 1) {
    lcd.setCursor(0, 1);
    lcd.print("pitch dark                ");
  }
  else if (L <= 5) {
    lcd.setCursor(0, 1);
    lcd.print("very dark");
  }
  else if (L <= 10) {
    lcd.setCursor(0, 1);
    lcd.print("dark                   ");
  }
  else if (L <= 20) {
    lcd.setCursor(0, 1);
    lcd.print("not much light             ");
  }
  else if (L < 8000){
    lcd.setCursor(0, 1);
    lcd.print("no sun             ");
  }
  else if (L < 50000) {
    lcd.setCursor(0, 1);
    lcd.print("in the sun          ");
  }
  else if (L >= 50000) {
    lcd.setCursor(0, 1);
    lcd.print("sunny day!          ");
  }

  if (L <= 5){
      digitalWrite(G1,HIGH);
      digitalWrite(G2,HIGH);
      digitalWrite(Y1,HIGH);
      digitalWrite(Y2,HIGH);
      digitalWrite(R1,HIGH);
      digitalWrite(R2,HIGH);
    }

    else if (L <= 10){
        digitalWrite(G1,HIGH);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,HIGH);
        digitalWrite(Y2,HIGH);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,HIGH);
      }

    else if (L <= 20){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,HIGH);
        digitalWrite(Y2,HIGH);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,HIGH);
      }

    else if (L <= 30){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,HIGH);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,HIGH);
      }
    
    else if (L <= 40){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,HIGH);
      }
     else if (L <= 50){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,HIGH);
        digitalWrite(R2,LOW);
      }
      else if (L <= 60){
        digitalWrite(G1,LOW);
        digitalWrite(G2,HIGH);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }
      else if (L <= 70){
        digitalWrite(G1,HIGH);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }
      else if (L <= 90 or L > 90){
        digitalWrite(G1,LOW);
        digitalWrite(G2,LOW);
        digitalWrite(Y1,LOW);
        digitalWrite(Y2,LOW);
        digitalWrite(R1,LOW);
        digitalWrite(R2,LOW);
      }
  
  }


}

void read_light(){
  light = lightMeter.readLightLevel();  //read the light intensity detected by BH1750
}

void read_dht11(){
  int chk;
  chk = DHT.read(DHT11_PIN);      // read data
  switch (chk) {
    case DHTLIB_OK:
      break;
    case DHTLIB_ERROR_CHECKSUM:   //check and return error
      break;
    case DHTLIB_ERROR_TIMEOUT:    //Timeout and return error
      break;
    default:
      break;
  }
  temperature = DHT.temperature;
  humidity = DHT.humidity;
}
