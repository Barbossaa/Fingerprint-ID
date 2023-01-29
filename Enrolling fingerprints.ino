#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

SoftwareSerial mySerial(2, 3);
// pin #2 is IN from sensor (TX)
// pin #3 is OUT from arduino  (RX)
// Set up the serial port to use softwareserial..

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()
{
  lcd.begin(16,2);
  lcd.backlight();
  Serial.begin(9600);
  delay(100);
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enrolment starts");
  delay(1000);

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found a sensor!");
    delay(1000);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor not exist");
    delay(1000);
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
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
  Serial.println("Ready to enroll a fingerprint!");
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to enroll");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Type in the ID");
  delay(1000);
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enrolling ID");
  delay(1000);
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting finger");
  delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image taken");
      delay(1000);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(".");
      delay(1000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communic. error");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imaging error");
      delay(1000);
      break;
    default:
      Serial.println("Unknown error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image converted");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image too messy");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communic. error");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feature not find");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feature not find");
      delay(1000);
      return p;
    default:
      Serial.println("Unknown error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      delay(1000);
      return p;
  }

  Serial.println("Remove finger");
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remove finger");
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  Wire.setClock(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place it again");
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image taken");
      delay(1000);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(".");
      delay(1000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communic. error");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imaging error");
      delay(1000);
      break;
    default:
      Serial.println("Unknown error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image converted");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image too messy");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communic. error");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feature not find");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feature not find");
      delay(1000);
      return p;
    default:
      Serial.println("Unknown error");
      Wire.setClock(10000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      delay(1000);
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Prints matched!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communic. error");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FPs not match");
    delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    delay(1000);
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stored!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communic. error");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Could not store");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error in writin");
    delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    Wire.setClock(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    delay(1000);
    return p;
  }

  return true;
}
