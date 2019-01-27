/*
  BikeTrailer

  Arduino microcontroller code to control rear flashing lights and interior
  'cabin' reading light for Burley bike trailer.

  Rear light pattern is similar to Planet Bike's 'SuperFlash' pattern, which is
  very eye catching.

*/

// General setup
byte rearFlashPin = 11;
byte cabinLightPin = 13;
byte cabinLightInterruptPin = 2;
volatile byte cabinLightState = LOW;
unsigned long cabinLightPinLastChangeTime = 0;

unsigned long switchHold = 30;
volatile bool switchLock = true;

// PWM duty cycle for low and high brightness (there is also off, which is 0)
int lowBrightPwm = 30; 
int highBrightPwm = 255;

// 2D array, where second dimension represents time (in milliseconds) and duty cycle (0-255)
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


void setup() {

    pinMode(rearFlashPin, OUTPUT);

    pinMode(cabinLightPin, OUTPUT);
    digitalWrite(cabinLightPin, LOW);

    pinMode(cabinLightInterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(cabinLightInterruptPin), toggleCabinLight, CHANGE);

    Serial.begin(9600);
  
}


void loop() {
    
    int i;
    for (i = 0; i < rearFlashPatternLength; i++) {
        
        // This is part of the interrupt based debounceing
        if (!switchLock &&
            (digitalRead(cabinLightInterruptPin) == HIGH) && 
            ((millis() - cabinLightPinLastChangeTime) > switchHold))
        {
            switchLock = true;

            // DEBUG
            Serial.print("Lock ON");
            Serial.println();
        }

        analogWrite(rearFlashPin, rearFlashPattern[i][1]);
        delay(rearFlashPattern[i][0]); 
    }

}

void toggleCabinLight() {
    // ISR for cabin light button. This uses a debounce scheme I cooked up that
    // requires no waiting in the ISR (but the main loop has a role to play).
    // Light state is toggled when button is released.

    if (!switchLock && ((millis() - cabinLightPinLastChangeTime) > switchHold)) {

        // DEBUG
        Serial.print("Toggling Light");
        Serial.println();

        cabinLightState = !cabinLightState;
        digitalWrite(cabinLightPin, cabinLightState);

    }

    // DEBUG
    Serial.print((millis() - cabinLightPinLastChangeTime));
    Serial.print("Lock OFF");
    Serial.println();

    cabinLightPinLastChangeTime = millis();
    switchLock = false;

}
