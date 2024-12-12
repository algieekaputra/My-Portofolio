#define kp 1
#define ki 0.1
#define kd 0.0

long int enc = 0;
int value; // store digital sensor input value
int sensorInput = 6;
float w;
float rpm;
float sampling_time = 50; //ms
float setpoint = 50;

float error, P, I, D, prev_error, pwm_motor;
int PID;

unsigned long time_prev;
void setup() {

  Serial.begin(9600);

  attachInterrupt(1,sub,RISING); // interrupt 1 pin3

  pinMode(8,OUTPUT); // motor driving
  pinMode(9,OUTPUT); // motor driving

  pinMode(sensorInput,INPUT);

  time_prev = millis();
}

void loop() {
  
  if(millis()-time_prev >= sampling_time){
    w = enc/(sampling_time/1000)*(2*3.141592/100); //kecepatan sudut rad/s
    rpm = enc/(sampling_time/1000)*60/100; //kecepatan rpm
    enc = 0;
    
    //////////kontroler///////////////
    error = setpoint - rpm;
    P = kp*error;
    I += ki*error;
    D = kd*(error-prev_error);
    prev_error = error;

    PID = (int)P + (int)I + (int)D;

    ///arah putar motor
    if(PID > 255){
      PID = 255;
    }
    else if(PID < -255){
      PID = -255;
    }

    if(PID > 0){
      digitalWrite(8,LOW);
      digitalWrite(9,HIGH);
    }
    else{
      digitalWrite(8,HIGH);
      digitalWrite(9,LOW);
    }
    ///kecepatan motor
    pwm_motor = abs(PID);
    analogWrite(5, pwm_motor);

//    Serial.print("error: ");
//    Serial.println(error);
//    Serial.print("P: ");
//    Serial.println(P);
//    Serial.print("I: ");
//    Serial.println(I);
//    Serial.print("D: ");
//    Serial.println(D);
//    Serial.print("PID: ");
//    Serial.println(PID);
//    Serial.print("W = ");
//    Serial.print(w);
//    Serial.print(",  ");
    Serial.print("RPM = ");
    Serial.println(rpm);
//    Serial.print(rpm);
//    Serial.print(",");
    time_prev = millis();
  }
  
}

void sub(){ //interrupt 1 occured ( pin 3 )
  value = digitalRead(sensorInput);
  if(value==1){ // button not pressed
    enc++;
  }
  else // button pressed
  {
    enc--;
  }
}
