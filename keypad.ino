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
    
    bool correctCode=false;
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
Door newDoor;
//Set proper pins the proper way properly
int Button = D0, speakerPin = A4, ButtonLast= LOW;



void setup(){
    Serial.begin(9600);
    pinMode(Button, INPUT_PULLDOWN);
}
  
  
  
void loop(){
    newDoor.getPress();
    getButton();
}

// - - - - START CLASS FUNCTION - - - -
void Door::checkCode() {
    if (correctCode) {
        // add instructions to do something if secret code entered correctly
		Serial.println("Correct code entered");
		Particle.publish("Door(tm)code","CORRECT");
		correctCode=false;
    } else {
        Serial.println("NOPE FRIENDO");
        Particle.publish("Door(tm)code","INCORRECT");
    }
}

void Door::getPress() {
    static int correctCount = 0;
    char key = keypad.getKey();
    if (key) {
        Serial.println(key);
        // optional: play sound as feedback when key pressed
        tone(speakerPin, 3000, 50);
        // if key matches next key in secret code, add one to correct count
        if (!correctCode) {
           if (key == secretCode[correctCount]) {
                correctCount++; 
           } else {
                correctCode=false;
                // else wrong key was entered (reset correct count)
                correctCount = 0; 
            }   
       } else {
            correctCode=false;
        }
    }
    if (correctCount == codeLength) {
        correctCode = true;
        correctCount = 0;
    }
}
// - - - - END CLASS FUNCTIONS - - - -


void getButton() {
    int ButtonNow = digitalRead(Button);
    if(ButtonNow == HIGH && ButtonLast == LOW) {;
        newDoor.checkCode();
        ButtonLast = HIGH;
    } else if (ButtonNow == LOW) {
        ButtonLast = LOW;
    }
}
