// Cod arduino proiect


#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN 10
byte readCard[4];
String MasterTag = "23DEE516";
String tagID = "";
Servo myservo;  // create servo object to control a servo
// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //Parameters: (rs, enable, d4, d5, d6, d7) 
void setup() 
{
  // Initiating
  pinMode(A0, OUTPUT); //LED VERDE
  pinMode(A1, OUTPUT);  //LED ROSU
  pinMode(A2, OUTPUT);  //Buzzer
  myservo.attach(A3);  // ServoMotor
  myservo.write(0); //Initializare servomotor in pozitia usa inchisa
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  lcd.begin(16, 2); // LCD screen
  lcd.clear();
  lcd.print(" Access Control ");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card>>");
}
void loop() 
{
  
  //Wait until new tag is available
  while (getID()) 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (tagID == MasterTag) 
    {
      
      lcd.print(" Access Granted!");
      digitalWrite(A0, HIGH);
       myservo.write(180); //servomotor in pozitia usa deschisa
      delay(2000);
      digitalWrite(A0, LOW);
      myservo.write(0);   //servomotor in pozitia usa inchisa
    }
    else
    {
      lcd.print(" Access Denied!");

      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      delay(1000);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
    }
    
      lcd.setCursor(0, 1);
      lcd.print(" ID : ");
      lcd.print(tagID);
      
    delay(1000);
    lcd.clear();
    lcd.print(" Access Control ");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Card>>");
  }
}
//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}