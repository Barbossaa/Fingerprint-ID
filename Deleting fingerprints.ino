#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

SoftwareSerial mySerial(2, 3);
// pin #2 is IN from sensor (TX)
// pin #3 is OUT from arduino  (TX)
// Set up the serial port to use softwareserial..

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  lcd.begin(16,2);
  lcd.backlight();
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nDelete Finger");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found a sensor");
    delay(1000);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor not exist");
    delay(1000);
    while (1);
  }
}


uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Type in the ID");
  delay(1000);
  uint8_t id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Deleting");
  delay(1000);
  deleteFingerprint(id);
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Deleted!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communic. error");
    delay(1000);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Couldn't delete");
    delay(1000);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error in writing");
    delay(1000);
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}
