//--------------------CYLINDER CLASS--------------------------------------------
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
	cf       = false;
	pinUp    = pUp;
	pinVal   = pVal;
	dBound   = downBound;
	uBound   = upBound;
	pinDown  = pDown;
	refValue = 50; //percents

	pinMode(pinUp, 1);
	pinMode(pinDown, 1);
}

void cylinder::Update() {
	int cur    = analogRead(pinVal);
	int needed = dBound + round((uBound - dBound) * (refValue/100));
	int perc   = round((uBound - dBound) * 0.05);

	if (!cf) {
		if (cur < (needed - perc)) {
			cf = true;
		} else if (cur > (needed + perc)) {
			cf = true;
		} else {
			return;
		}
	}

	if (cur < needed - 1) {
		digitalWrite(pinUp, 1);
		digitalWrite(pinDown, 0);
	} else if (cur > needed + 1) {
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

//------------------------------------------------------------------------------

//pinUp, pinDown, pinValue, lowBound, highBound
cylinder cyl1(3, 7, A7, 730, 915);  //lowBound and highBound are min and max values of angle got from potenciometer
cylinder cyl2(4, 8, A6, 375, 710);
cylinder cyl3(5, 9, A5, 780, 1023);
//cylinder cyl1(2, 6, A5, 730, 915);  //for kleshi

//-----------------COMMAND PROMPT-----------------------------------------------

void serialFlush(){
	char t;
	while(Serial.available() > 0) {
		t = Serial.read();
	}
}

void cmdParse() {
	if (Serial.available() > 0) {
		String cur   = Serial.readStringUntil(';');
		int dotplace = cur.indexOf(':');
		String comnd = cur.substring(0, dotplace);
		String value = cur.substring(dotplace+1);
		cmd(comnd, value);
		serialFlush();
	}
}

void cmd(String comnd, String value) {
	char cm[20];

	value.toCharArray(cm, 20);

	if (comnd == "bottom") {
		cyl1.setAngle(atof(cm));
	} else if (comnd == "upper") {
		cyl2.setAngle(atof(cm));
	} else if (comnd == "hook") {
		switch (cm[0]) {
			case '=': {
				digitalWrite(2, 0);
				digitalWrite(6, 0);
				break;
			}
			case '+': {
				digitalWrite(2, 1);
				digitalWrite(6, 0);
				break;
			}
			case '-': {
				digitalWrite(2, 0);
				digitalWrite(6, 1);
				break;
			}
		}
	} else if (comnd == "hookCyl") {
		cyl3.setAngle(atof(cm));
	} else if (comnd == "rot") {
		switch (cm[0]) {
			case '=': {
				digitalWrite(A2, 0);
				digitalWrite(A3, 0);
				break;
			}
			case '+': {
				digitalWrite(A2, 0);
				delayMicroseconds(300);
				digitalWrite(A3, 1);
				break;
			}
			case '-': {
				digitalWrite(A3, 0);
				delayMicroseconds(300);
				digitalWrite(A2, 1);
				break;
			}
		}
	} else {
		Serial.println("Unknown command");
	}
}

//--------------------MAIN CODE-------------------------------------------------

void setup() {
	Serial.begin(115200);
	pinMode(2, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
}

void loop() {
	cyl1.Update();
	cyl2.Update();
	cyl3.Update();

	/*
  	Serial.print("A5:");
	Serial.print(analogRead(A5));
	Serial.print("  A6:");
	Serial.print(analogRead(A6));
	Serial.print("  A7:");
	Serial.print(analogRead(A7));
	Serial.println("  ");*/
	cmdParse();
}
