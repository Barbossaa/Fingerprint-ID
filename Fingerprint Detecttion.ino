
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

SoftwareSerial mySerial(2, 3);
// pin #2 is IN from sensor (TX)
// pin #3 is OUT from arduino  (RX)
// Set up the serial port to use softwareserial..


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  lcd.begin(16,2);
  lcd.backlight();
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Virtual ID");
  lcd.setCursor(0, 1);
  lcd.print("is preparing");
  delay(1000);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
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

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No data!");
    delay(1000);
    
  }
  else {
    Serial.println("Waiting for valid finger...");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place the finger");
    delay(1000);
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop()                     // run over and over again
{
  
  getFingerprintID();
  delay(1000);            //don't need to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
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
      Serial.println("No finger detected");
      Wire.setClock(10000); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Waiting");
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
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      Wire.setClock(10000); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imaging error");
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

  // OK success!

  p = finger.image2Tz();
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
      lcd.print("No feature");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      Wire.setClock(10000); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No feature");
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
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID Found");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communic. error");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    Wire.setClock(10000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No ID");
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

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Name:Ankara Mesi");
  lcd.setCursor(0, 1);
  lcd.print("ID:K12-34-5679");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOB: 09/03/1984");
  lcd.setCursor(0, 1);
  lcd.print("Class: AB");
  delay(3000);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  Wire.setClock(10000); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Name:Ankara Mesi");
  lcd.setCursor(0, 1);
  lcd.print("ID:K12-34-5679");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOB: 09/03/1984");
  lcd.setCursor(0, 1);
  lcd.print("Class: AB");
  delay(3000);
  return finger.fingerID;
}
