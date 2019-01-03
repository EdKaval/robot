class cylinder {
    int pinUp, pinDown, pinVal, dBound, uBound;
    float refValue;
    bool cf;
  public:
    cylinder(int, int, int, int, int);
    void setAngle(float);
    void Update();
};

cylinder::cylinder(int pUp, int pDown, int pVal, int downBound, int upBound) {
  pinUp = pUp;
  pinDown = pDown;
  pinVal = pVal;
  dBound = downBound;
  uBound = upBound;
  refValue = 50; //percents
  cf = false;

  pinMode(pinUp, 1);
  pinMode(pinDown, 1);
}

void cylinder::Update() {
  int cur = analogRead(pinVal);
  int needed = dBound + round((uBound - dBound) * (refValue/100));
  int perc = round((uBound - dBound) * 0.05);

  //Serial.print(cur);
  //Serial.print("; need = ");
  //Serial.println(cf);

  if (cf) {
    if (cur < (needed - perc)) {
      cf = true;
    } else if (cur > (needed + perc)) {
      cf = true;
    } else {
      return;
    }
  }

  if (cur < needed) {
    digitalWrite(pinUp, 1);
    digitalWrite(pinDown, 0);
    //Serial.println("Ja eblan");
  } else if (cur > needed) {
    digitalWrite(pinUp, 0);
    digitalWrite(pinDown, 1);
  } else {
    digitalWrite(pinUp, 0);
    digitalWrite(pinDown, 0);
    cf = false;
  }
}

void cylinder::setAngle(float newAngle) {
  refValue = max(min(newAngle, 100), 0);  //in range 0-100%
}

//pinUp, pinDown, pinValue, lowBound, highBound
cylinder cyl1(3, 9, A0, 730, 915);
//cylinder cyl2(3, 7, A0, 730, 915);

void setup() {
  Serial.begin(9600);
}

void loop() {
  cyl1.Update();
  Serial.println(analogRead(A0));
}
