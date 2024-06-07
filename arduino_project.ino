#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>


const int bluewire = 22;
const int greenwire = 24;
const int orangewire = 25;
const int yellowwire = 26;
const int redwire = 27;
// Define the pins for the RGB LED


#define PHOTO_PIN A1
#define TOUCH_THRESHOLD 100 // Adjust this threshold based on your photoresistor sensitivity

#define SWITCH_1 10
#define SWITCH_2 11
#define SWITCH_3 12
#define SWITCH_4 13

#define CLK 2
#define DIO 3
#define COUNTDOWN_DURATION 240 // 4 minutes in seconds


#define NOTE_C 261.63
#define BUZZER_PIN 35

const int redPin1 = 9;
const int greenPin1 = 8;
const int bluePin1 = 7;

const int redPin2 = 6;
const int greenPin2 = 5;
const int bluePin2 = 4;

const int redPin3 = 38;
const int greenPin3 = 37;
const int bluePin3 = 36;

const int redPin4 = 41;
const int greenPin4 = 40;
const int bluePin4 = 39;

const int redPin5 = 44;
const int greenPin5 = 43;
const int bluePin5 = 42;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {52, 51, 50, 49}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {48, 47, 46, 45}; // Connect to the column pinouts of the keypad

// Define global variables
LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // Declare keypad as a global variable


// TM1637Display display(CLK, DIO);
// unsigned long startTime;

bool morseCodeCompleted = false;
TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(9600);
  TM1637Display display(CLK, DIO);
  display.setBrightness(7);
  // lcd.print(Morse Code);
  // delay(5000);
  // lcd.clear();

  // display.setBrightness(0x0a); // Set the brightness level (0x00 to 0x0f)
  // startTime = millis();

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(bluewire, INPUT);
  pinMode(greenwire, INPUT);
  pinMode(yellowwire, INPUT);
  pinMode(orangewire, INPUT);
  pinMode(redwire, INPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);

  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  pinMode(redPin3, OUTPUT);
  pinMode(greenPin3, OUTPUT);
  pinMode(bluePin3, OUTPUT);

  pinMode(redPin4, OUTPUT);
  pinMode(greenPin4, OUTPUT);
  pinMode(bluePin4, OUTPUT);

  pinMode(redPin5, OUTPUT);
  pinMode(greenPin5, OUTPUT);
  pinMode(bluePin5, OUTPUT);

  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on backlight
  
  // Run Morse code function
  morsecode();
  
  keycode();
  //toggleswitch();
  //photoresistor();
}

void loop() {

  //  unsigned long currentTime = millis();
  // unsigned long elapsedTime = (currentTime - startTime) / 1000; // Convert to seconds
  // unsigned long remainingTime = COUNTDOWN_DURATION - elapsedTime;

  // if (remainingTime <= 0) {
  //   startTime = currentTime; // Reset the timer
  //   remainingTime = COUNTDOWN_DURATION;
  // }

  // // Calculate minutes and seconds
  // unsigned int minutes = remainingTime / 60;
  // unsigned int seconds = remainingTime % 60;

  // // Update the display
  // display.showNumberDecEx(minutes * 100 + seconds, 0b01000000, true); // Display minutes and seconds

  // // Print remaining time to Serial monitor for debugging
  // Serial.print("Remaining Time: ");
  // Serial.print(minutes);
  // Serial.print("m ");
  // Serial.print(seconds);
  // Serial.println("s");

  // delay(1000); 

  if (!morseCodeCompleted) {
    
    // Morse code is still running
    // Check if it has completed
    // In the morsecode function, set morseCodeCompleted to true when done
    return;
  }

  static bool checkCombination = false;

  if (!checkCombination) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      lcd.clear(); // Clear the LCD screen
      lcd.setCursor(0, 0); // Set cursor to the first row
      lcd.print("Key: "); // Display "Key: "
      lcd.print(key); // Display the pressed key

      if (key == '3') {
        lcd.setCursor(0, 1); // Set cursor to the second row
        lcd.print("Correct key"); 
        delay(2000);
        lcd.clear();
        checkCombination = true; // Move to checking combination
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Incorrect Key");
        playNote(NOTE_C, 1000);

      }
    }
  } else {
    // Now we check the combination
    keycode(); // Call the function to handle combination input
    delay(100); // Delay to stabilize keypad reading
    
  }
}


void morsecode() {
  // Define morse code functionality
  digitalWrite(bluePin1, HIGH);
  delay(3000);
  digitalWrite(bluePin1, LOW);
  delay(2000);

  // H
  digitalWrite(redPin1, HIGH);
  digitalWrite(redPin2, HIGH);
  digitalWrite(redPin3, HIGH);
  digitalWrite(redPin4, HIGH);
  delay(3000);
  digitalWrite(redPin1, LOW);
  digitalWrite(redPin2, LOW);
  digitalWrite(redPin3, LOW);
  digitalWrite(redPin4, LOW);
  delay(2000);

  // R
  digitalWrite(redPin1, HIGH);
  digitalWrite(bluePin2, HIGH);
  digitalWrite(redPin3, HIGH);
  delay(3000);
  digitalWrite(redPin1, LOW);
  digitalWrite(bluePin2, LOW);
  digitalWrite(redPin3, LOW);
  delay(2000);

  // E
  digitalWrite(redPin1, HIGH);
  delay(3000);
  digitalWrite(redPin1, LOW);
  delay(2000);

  // E
  digitalWrite(redPin1, HIGH);
  delay(3000);
  digitalWrite(redPin1, LOW);
  delay(2000);

  // Set morseCodeCompleted to true to indicate morse code is done
  morseCodeCompleted = true;
}

void keycode() {
  

    static char combination[] = "B403"; // Define the correct combination
    static int index = 0; // Index to keep track of the current position in the combination array
      
    char code = keypad.getKey();

    if (code != NO_KEY) {
        lcd.clear(); // Clear the LCD screen
        lcd.setCursor(0, 0); // Set cursor to the first row
        lcd.print("Code: "); // Display "Code: "
        lcd.print(code); // Display the pressed key

        if (code == combination[index]) {
            index++; // Move to the next position in the combination array

            if (index == 4) { // Check if the entire combination is entered correctly
                lcd.clear();
                lcd.print("Correct Combination!");
                // Reset index for the next input
                index = 0;
              
                checkSteps(); // Call the function to check steps
            }
        } else {
            // If the entered key doesn't match the expected one, display "Wrong Combination!"
            lcd.clear();
            lcd.print("Wrong Combination!");
             playNote(NOTE_C, 1000);

            // Reset index for the next input
            index = 0;
            
        }
        
    }
    
}
void checkSteps() {
    bool potentiometerStepCompleted = false;
    bool switch10StepCompleted = false;
    bool potentiometerBackStepCompleted = false;
    bool switch13StepCompleted = false;

    while (true) {
        // Check if potentiometer is at 1000
        if (!potentiometerStepCompleted) {
            int potValue = analogRead(A0);
            if (potValue >= 1000) {
                lcd.clear();
                lcd.print("Minimised");
                potentiometerStepCompleted = true;
                delay(2000);
            }
        }

        // Check if switch 10 is on
        if (potentiometerStepCompleted && !switch10StepCompleted) {
            if (digitalRead(SWITCH_1) == HIGH) {
                lcd.clear();
                lcd.print("Switch 1 ON");
                switch10StepCompleted = true;
                delay(2000);
            }
        }

        // Check if potentiometer value is down till 10
        if (potentiometerStepCompleted && switch10StepCompleted && !potentiometerBackStepCompleted) {
            int potValue = analogRead(A0);
            if (potValue <= 10) {
                lcd.clear();
                lcd.print("Maximised");
                potentiometerBackStepCompleted = true;
                delay(2000);
            }
        }

        // Check if switch 13 is on
        if (potentiometerStepCompleted && switch10StepCompleted && potentiometerBackStepCompleted && !switch13StepCompleted) {
            if (digitalRead(SWITCH_4) == HIGH) {
                lcd.clear();
                lcd.print("Switch 4 ON");
                switch13StepCompleted = true;
                delay(2000);
            }
        }

        // Check if any step was completed in the wrong order or without the previous step
        if ((potentiometerStepCompleted && !switch10StepCompleted) ||
            (switch10StepCompleted && !potentiometerStepCompleted) ||
            (potentiometerBackStepCompleted && !switch10StepCompleted) ||
            (switch13StepCompleted && (!potentiometerBackStepCompleted || !switch10StepCompleted))) {
            // If any step is done incorrectly, reset all step completion flags and retry from the beginning
            lcd.clear();
            lcd.print("Incorrect steps. Retry.");
            playNote(NOTE_C, 1000);

            potentiometerStepCompleted = false;
            switch10StepCompleted = false;
            potentiometerBackStepCompleted = false;
            switch13StepCompleted = false;
            delay(2000);
        }

        // Check if all steps are completed
        if (potentiometerStepCompleted && switch10StepCompleted && potentiometerBackStepCompleted && switch13StepCompleted) {
            // If all steps are completed, display success message and call the function to check the photoresistor
            lcd.clear();
            lcd.print("All steps completed!");
            delay(2000);
            photoresistor(); // Call the function to check the photoresistor
            break; // Exit the while loop
        }
    }
}

void photoresistor() {
    // Function to handle photoresistor behavior
    int photoValue = analogRead(PHOTO_PIN); // Read the photoresistor value
    if (photoValue > 50) {
        // If the photoresistor is covered (value greater than 250), turn on the green LED
        digitalWrite(greenPin1, HIGH);
        lcd.clear();
        lcd.print("Correct");
        delay(2000);
        digitalWrite(greenPin1,LOW);
        checkWireInputs(); // Call the function to check the wire inputs
    } else {
        // If the photoresistor is not covered, turn off the green LED
        digitalWrite(greenPin1, LOW);
        lcd.clear();
        lcd.print("Detecting Light !");
    }
}


void checkWireInputs() {
    lcd.clear();
    lcd.print("Checking wire...");
    delay(1000);

    lcd.clear();
    lcd.print("15");
    playNote(NOTE_C, 1000);
    lcd.clear();
    lcd.print("14");
    playNote(NOTE_C, 900);
    delay(100);
    lcd.clear();
    lcd.print("13");
    playNote(NOTE_C, 900);
    delay(100);
    lcd.clear();
    lcd.print("12");
    playNote(NOTE_C, 900);
    delay(100);
    lcd.clear();
    lcd.print("11");
    playNote(NOTE_C, 900);
    delay(100);
    lcd.clear();
    lcd.print("10");
    playNote(NOTE_C, 900);
    delay(100);
    lcd.clear();
    lcd.print("9");
    playNote(NOTE_C, 400);
    delay(200);
    playNote(NOTE_C, 400);
    lcd.clear();
    lcd.print("8");
     playNote(NOTE_C, 400);
    delay(200);
    playNote(NOTE_C, 400);
    lcd.clear();
    lcd.print("7");
     playNote(NOTE_C, 400);
    delay(200);
    playNote(NOTE_C, 400);
    lcd.clear();
    lcd.print("6");
     playNote(NOTE_C, 400);
    delay(200);
    playNote(NOTE_C, 400);
    lcd.clear();
    lcd.print("5");
     playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    lcd.clear();
    lcd.print("4");
     playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    lcd.clear();
    lcd.print("3");
     playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    lcd.clear();
    lcd.print("2");
     playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    lcd.clear();
    lcd.print("1");
     playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);
    playNote(NOTE_C, 200);
    delay(50);

    if (digitalRead(greenwire) == HIGH) {
        // Green wire connected, indicating bomb blast
        lcd.clear();
        lcd.print("Bomb blast!!");
        // digitalWrite(redPin2,HIGH);
        // digitalWrite(redPin1,HIGH);
        // digitalWrite(redPin1,HIGH);

        playNote(NOTE_C, 1000);
        delay(100);
        playNote(NOTE_C, 1000);
        delay(100);
        playNote(NOTE_C, 1000);


    } else if (digitalRead(greenwire)==LOW) {
        // Green wire not connected, indicating bomb defused
        lcd.clear();
        lcd.print("Bomb defused");
    }
}


void playNote(float frequency, int duration) {
  if (frequency == 0) {
    // If frequency is 0, pause for the duration
    delay(duration);
  } else {
    // Play the note at the specified frequency for the duration
    tone(BUZZER_PIN, frequency, duration);
    delay(duration + 50); // Add a small delay to prevent overlap
  }
}
