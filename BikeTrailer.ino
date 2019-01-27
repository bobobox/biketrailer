/*
  BikeTrailer
*/

// 2D array, where second dimension represents time (in milliseconds) and duty cycle (0-255)

byte rearFlashPin = 11;
byte cabinLightPin = 13;
byte cabinLightInterruptPin = 2;
volatile byte cabinLightState = LOW;
unsigned long cabinLightPinLastChangeTime = 0;

unsigned long switchHold = 30;
volatile bool switchLock = true;

int lowBrightPwm = 30; 
int highBrightPwm = 255;

int rearFlashPattern [][2] = {
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 140, 0 },
    { 70, highBrightPwm },
    { 110, 0 },
    { 160, highBrightPwm },
    { 130, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 120, 0 },
    { 30, lowBrightPwm },
    { 140, 0 },
    { 70, highBrightPwm },
    { 110, 0 },
    { 160, highBrightPwm },
    { 130, 0 }
};

int rearFlashPatternLength = sizeof(rearFlashPattern)/sizeof(rearFlashPattern[0]);


// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(rearFlashPin, OUTPUT);

    pinMode(cabinLightPin, OUTPUT);
    digitalWrite(cabinLightPin, LOW);

    pinMode(cabinLightInterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(cabinLightInterruptPin), toggleCabinLight, CHANGE);

    Serial.begin(9600);
  
  
}

// the loop function runs over and over again forever
void loop() {
    
    int i;
    for (i = 0; i < rearFlashPatternLength; i++) {

        if (!switchLock &&
            (digitalRead(cabinLightInterruptPin) == HIGH) && 
            ((millis() - cabinLightPinLastChangeTime) > switchHold))
        {
            switchLock = true;
            Serial.print("Lock ON");
            Serial.println();
        }

        analogWrite(rearFlashPin, rearFlashPattern[i][1]);
        delay(rearFlashPattern[i][0]); 
    }

}

void toggleCabinLight() {
    // ISR for cabin light button

    if (!switchLock && ((millis() - cabinLightPinLastChangeTime) > switchHold)) {

        Serial.print("Toggling Light");
        Serial.println();
        cabinLightState = !cabinLightState;
        digitalWrite(cabinLightPin, cabinLightState);

    }

    Serial.print((millis() - cabinLightPinLastChangeTime));
    Serial.print("Lock OFF");
    Serial.println();

    cabinLightPinLastChangeTime = millis();
    switchLock = false;

}
