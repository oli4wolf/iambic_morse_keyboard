// Turns Iambic-Morsekey into USB keyboard
// Basic Idea comming from a straight key keyboard https://github.com/nomblr/morse Thank you!
#include <Bounce.h> // include de-bounce library

/* Universal constants*/
#define CHARSIZE 45
/* Variables */
const int ledPin = 13; // blinky pin (might connect to a buzzer?)
const int dotPin = 1;  // dot key pin
const int dashPin = 0;  // dash pin key
String morseCode[CHARSIZE] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "..--..", "-....-", ".----.", "---...", ".-..-.", "-..-.", ".--.-."};
char morseChar[CHARSIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '-', '\'', ':', '\'', '/', '@'};
Bounce dotKey = Bounce(dotPin, 10);  // 10 ms debounce
Bounce dashKey = Bounce(dashPin, 10);  // 10 ms debounce

//Reference configuration from nomblr/morse
const unsigned long dashThresh = 150; // time threshold in ms to differentiate dots from dashes
const unsigned long letterThresh = 500; // time threshold in ms to differentiate letter gaps
const unsigned long wordThresh = 3000; // time threshold in ms to differentiate word gaps

String lastInput = ""; // initialise input string
String keyString = ""; // initialise key string

// Variables for the algorithm
unsigned long downTime = 0; // records the start time of state change
unsigned long upTime = 0; // records the end time of state change
unsigned long timeNow = 0; // records the current time
unsigned long changeDuration = 0; // records the duration of state change
unsigned long pauseDuration = 0; // records the duration of the last pause
int pauseFlag = 0; // initilise the flag to indicate whether a pause has already been evaluated

void setup() {
  pinMode(ledPin, OUTPUT); // configure the pin connected to the led as an output
  pinMode(dotPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup
  pinMode(dashPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup


}

// the loop() method runs over and over again,
// as long as the board has power
void loop() {
  //Squeeze
  if (digitalRead(dashPin) == LOW && digitalRead(dotPin) == LOW) {
    if (lastInput == "dash") {
      dot();
    } else {
      dash();
    }
  } else if (digitalRead(dashPin) == LOW) {
    dash();
  } else if (digitalRead(dotPin) == LOW) {
    dot();
  } else {
    //Measure the pause
    if ((millis() - pauseDuration) >= letterThresh) {
      //eval Letter and reset timer
      for (int i = 0; i < CHARSIZE; i++) {
        if (morseCode[i].equals(keyString)) {
          Keyboard.press(morseChar[i]);
          Keyboard.release(morseChar[i]);
          printToConsole(i, morseChar[i]);
          keyString = "";
          pauseFlag = 0;
          pauseDuration = millis();
          break;
        }
      }
      //Debug Serial.println("code not found: " + keyString);
      keyString = "";
    } else if ((millis() - pauseDuration) >= wordThresh && pauseFlag < 1) {
      Keyboard.press(KEY_SPACE);
      Keyboard.release(KEY_SPACE);
      Serial.println("Keypress : Space");
      pauseFlag = 1;
    }
  }

}// end of loop

//Debug console can be easely outcommented in the algorithm
void printToConsole(int i, char c) {
  String str = "keyPress ";
  str.concat(i);
  str.concat(": ");
  str.concat(c);
  Serial.println(str);
}

//Dash method with console output
void dash() {
  lastInput = "dash";
  keyString = keyString + "-";
  Serial.println("Keypress dash :" + keyString);
  pauseDuration = millis(); // reset time for Pause and letter change.
  pauseFlag = 0;
  delay(150);
}

//Dot Method with console output
void dot() {
  lastInput = "dot";
  keyString = keyString + ".";
  Serial.println("Keypress dot :" + keyString);
  pauseDuration = millis();
  pauseFlag = 0;
  delay(150);
}

