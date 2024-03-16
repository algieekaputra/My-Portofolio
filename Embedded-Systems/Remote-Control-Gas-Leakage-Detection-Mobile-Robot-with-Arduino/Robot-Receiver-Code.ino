int sensorMQ2 = A0; //Buat sensor MQ-2
int sensorMQ135 = A1; //Buat sensor MQ-135
int ledR = A2;
int ledG = A3;

//MOTOR SPEED
int enA = 3;
int enB = 5;
//LEFT MOTOR
int in1 = 7;
int in2 = 8;
//RIGHT MOTOR
int in3 = 12;
int in4 = 13;

char val = 0;

void maju()
{
       //MOTOR KIRI
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, LOW);
       analogWrite(enA, 255);
      //MOTOR KANAN
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enB,255); 
}
void kiri()
{
        //MOTOR KIRI
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH);
      analogWrite(enB, 198);
      //MOTOR KANAN
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enB,200);
}
void kanan()
{
      //MOTOR KIRI
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, LOW);
      analogWrite(enA, 198);
      //MOTOR KANAN
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, 200);
}
void berhenti()
{
      //MOTOR KIRI
      digitalWrite(in1, LOW); 
      digitalWrite(in2, LOW);
      //MOTOR KANAN
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
}
void mundur()
{
        //MOTOR KIRI
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH);
      analogWrite(enA, 127);
      //MOTOR KANAN
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, 180);
}
void majukiri()
{
          //MOTOR KIRI
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, LOW);
      analogWrite(enA, 0);
      //MOTOR KANAN
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enB, 255);
}
void majukanan()
{
          //MOTOR KIRI
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, LOW);
      analogWrite(enA, 255);
      //MOTOR KANAN
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enB, 0);
}
void mundurkiri()
{
          //MOTOR KIRI
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH);  
      analogWrite(enA, 0);
      //MOTOR KANAN
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, 180);
}
void mundurkanan()
{
          //MOTOR KIRI
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH);
      analogWrite(enA, 127);
      //MOTOR KANAN
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, 0);
}

void leakage ()
{
    analogWrite(ledG, 0);
    analogWrite(ledR, 1023);
    delay(100);
    analogWrite(ledR, 0);
    delay(100);
}
void setup() {
  // put your setup code here, to run once:
  //MOTOR DRIVER
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(sensorMQ2, INPUT); 
  pinMode(sensorMQ135, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  Serial.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue1 = analogRead(sensorMQ2);
  int sensorValue2 = analogRead(sensorMQ135);
 /* Serial.println(sensorValue1);
  Serial.println(sensorValue2);
  Serial.println("\n"); */
  
  if (sensorValue1 > 400 || sensorValue2 > 500) 
  {                  
    leakage(); 
  } 
  else {
    analogWrite(ledG, 1023);
    analogWrite(ledR, 0);               
  }

  while (Serial.available() > 0)
  {
    val = Serial.read();
    Serial.println(val);
  }
  if (val == 'F')
  {
    maju();
  }
  else if (val == 'L')
  {
    kiri();
  }
  else if (val == 'R')
  {
    kanan();
  }
  else if (val == 'B')
  {
    mundur();
  }
  else if (val == 'G')
  {
    majukiri();
  }
  else if (val == 'I')
  {
    majukanan();
  }
  else if (val == 'H')
  {
    mundurkiri();
  }
  else if (val == 'J')
  {
    mundurkanan();
  }
  else
  {
    berhenti();
  } 
}
