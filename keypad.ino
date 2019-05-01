// This #include statement was automatically added by the Particle IDE.
#include <Keypad_Particle.h>

class Door {
public:
    void checkCode();
    void getPress();
private:
    // change to any secret code or command of any length
    const static int codeLength=4;
    char secretCode[codeLength]={'5','6','7','8'}; // 8324 = TECH
    
    bool correctCode=false;     int correctCount=0;
    const static byte ROWS=4, COLS=3;
    byte rowPins[ROWS] = {D1,D2,D3,D4};
    byte colPins[COLS] = {D5,D6,D7};
    char keys[ROWS][COLS] = {
     {'1','2','3'},
     {'4','5','6'},
     {'7','8','9'},
     {'*','0','#'}
    };
    // create Keypad object variable called "keypad"
    Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
};

//Make a door
Door door;
//Set proper pins the proper way properly
int Button = D0, speakerPin = A4;



void setup() {
    Serial.begin(9600);
    pinMode(Button, INPUT_PULLDOWN);
    Particle.subscribe("doorBellRing", doorBell, "1c002a000347363333343435");
    Particle.subscribe("doorTMmotionDetected", motionDetected, "1c002a000347363333343435");
}
  
  
  
void loop() {
    door.getPress();
    getButton();
}

// - - - - START CLASS FUNCTION - - - -

void Door::getPress() {
    char key = keypad.getKey();
    if (key) {
        Serial.println(key);
        // optional: play sound as feedback when key pressed
        tone(speakerPin, 3000, 50);
        // if key matches next key in secret code, add one to correct count
        if (!correctCode) {
           if (key == secretCode[correctCount]) {
            //key is correct
                correctCount++; 
           } else {
            //Key is incorrect
                correctCode=false;
                correctCount = 0; 
            }  
       }    //Else keycode is too long, make it false. 
       else {
            correctCode=false;  correctCount=0;
        }
    }
    if (correctCount == codeLength) {
        correctCode = true; correctCount = 0;
    }
}


void Door::checkCode() {
    if (correctCode) {
        //Keycode is correct, unlock door
        tone(speakerPin, 4000, 100);
        delay(75);
        tone(speakerPin, 5000, 150);
		Serial.println("Correct code entered");
		//Publish both a private and public event for the occasion
		Particle.publish("Door(tm)code","CORRECT", PUBLIC);
		Particle.publish("CodeEntered", "CORRECT");
		correctCode=false;
    } else {
        //Code is wrong.
        correctCount=0;
        tone(speakerPin, 1250, 250);
        Serial.println("NOPE FRIENDO");
        //Publish both a private and public event for the occasion
        Particle.publish("Door(tm)code","INCORRECT", PUBLIC);
        Particle.publish("CodeEntered", "INCORRECT");
    }
}

// - - - - END CLASS FUNCTIONS - - - -


void getButton() {
    static int ButtonLast;
    int ButtonNow = digitalRead(Button);
    if(ButtonNow == HIGH && ButtonLast == LOW) {;
        door.checkCode();
        ButtonLast = HIGH;
    } else if (ButtonNow == LOW) {
        ButtonLast = LOW;
    }
}


void motionDetected(const char *event, const char *data) {
 static unsigned long long Time = millis();
 //static unsigned long long 
 Particle.publish("MotionDetection","DETECTED MOTION");
 
}

void doorBell(const char *event, const char *data) {
    tone(speakerPin,4200,420);
}
