// Define the LEDS AND SPEAKER

int R1 = 0;
int R2 = 1;
int Y1 = 3;
int Y2 = 4;
int G1 = 5;
int G2 = 11;
int SPK = 12;
int TIME1 = 500;
int TIME2 = 250;

void setup() {

// LED AND SPEAKER SET DIGITAL PINS TO OUTPUT
 pinMode(R1,OUTPUT);
 pinMode(R2,OUTPUT);
 pinMode(Y1,OUTPUT);
 pinMode(Y2,OUTPUT);
 pinMode(G1,OUTPUT);
 pinMode(G2,OUTPUT);
 pinMode(SPK,OUTPUT);

// RUN STARTUP MODE FOR LEDS AND SPEAKER
digitalWrite(R1,HIGH);
delay(TIME1);
digitalWrite(R1,LOW);
delay(TIME2);

digitalWrite(R2,HIGH);
delay(TIME1);
digitalWrite(R2,LOW);
delay(TIME2);

digitalWrite(Y1,HIGH);
delay(TIME1);
digitalWrite(Y1,LOW);
delay(TIME2);

digitalWrite(Y2,HIGH);
delay(TIME1);
digitalWrite(Y2,LOW);
delay(TIME2);

digitalWrite(G1,HIGH);
delay(TIME1);
digitalWrite(G1,LOW);
delay(TIME2);

digitalWrite(G2,HIGH);
delay(TIME1);
digitalWrite(G2,LOW);
delay(TIME2);

digitalWrite(SPK,HIGH);
delay(TIME1);
digitalWrite(SPK,LOW);
delay(TIME2);

digitalWrite(SPK,HIGH);
delay(TIME1);
digitalWrite(SPK,LOW);
delay(TIME2);

digitalWrite(R1,HIGH);
delay(TIME1);
digitalWrite(R1,LOW);
delay(TIME2);

digitalWrite(R2,HIGH);
delay(TIME1);
digitalWrite(R2,LOW);
delay(TIME2);

digitalWrite(Y1,HIGH);
delay(TIME1);
digitalWrite(Y1,LOW);
delay(TIME2);

digitalWrite(Y2,HIGH);
delay(TIME1);
digitalWrite(Y2,LOW);
delay(TIME2);

digitalWrite(G1,HIGH);
delay(TIME1);
digitalWrite(G1,LOW);
delay(TIME2);

digitalWrite(G2,HIGH);
delay(TIME1);
digitalWrite(G2,LOW);
delay(TIME2);

digitalWrite(SPK,HIGH);
delay(TIME1);
digitalWrite(SPK,LOW);
delay(TIME2);

digitalWrite(SPK,HIGH);
delay(TIME1);
digitalWrite(SPK,LOW);
delay(TIME2);
}

void loop() {






}
