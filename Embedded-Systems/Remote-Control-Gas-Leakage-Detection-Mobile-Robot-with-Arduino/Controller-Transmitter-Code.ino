const int X1 = A0; // analog pin connected to X output
const int Y1 = A1; // analog pin connected to Y output 
const int X2 = A2;
const int Y2 = A3;
void setup() {
  // put your setup code here, to run once:
  pinMode(X1, INPUT);
  pinMode(Y1, INPUT);
  pinMode(X2, INPUT);
  pinMode(Y2, INPUT);
  Serial.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x1 = analogRead(X1);
  int y1 = analogRead(Y1);
  int x2 = analogRead(X2);
  int y2 = analogRead(Y2);

  if(x1 < 50 /*  &&  505 < y2 < 515 */)
  {
    //FORWARD
    Serial.write('F');
    delay(5);
  }

  else if (x1 >= 900  /*&&  505 < y2 < 515*/)
  {
    //REVERSE
    Serial.write('B');
    delay(10);
  } 
  else if (y2 > 900  /*&&   505 <= x1 <= 515*/)
  {
    //LEFT
    Serial.write('L');
    delay(10);
  }
  else if (y2 < 384  /*&&   505 <= x1 <= 515 */)
  {
    //RIGHT
    Serial.write('R');
    delay(10);
  }
  else if(x1 >= 900  && y2 > 900)
  {
    Serial.write('G');
    delay(10);
  }
  else
  {
    //STOP
    Serial.write('S');
    delay(10);
  }


}
