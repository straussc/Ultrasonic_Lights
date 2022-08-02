#include <Wire.h>  // Wire Bibliothek hochladen
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek hochladen
LiquidCrystal_I2C lcd1(0x27, 16, 2);  //Hier wird das erste Display benannt "lcd1" (Adresse/Zeichen pro Zeile/Anzahl Zeilen)

// this constant won't change:
//const int buttonPin = 2;    // the pin that the pushbutton is attached to

// Definiere Ausgänge
const int LightPin1 = 3;
const int LightPin2 = 5;
const int LightPin3 = 6;
const int LightPin4 = 10;
const int LightPin5 = 11;

// Einstellung Ultrasonic
long dauer=0; // Das Wort dauer ist jetzt eine Variable, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt. Startwert ist hier 0.
long entfernung=0; // Das Wort „entfernung“ ist jetzt die variable, unter der die berechnete Entfernung gespeichert wird. Info: Anstelle von „int“ steht hier vor den beiden Variablen „long“. Das hat den Vorteil, dass eine größere Zahl gespeichert werden kann. Nachteil: Die Variable benötigt mehr Platz im Speicher.

float cm;
float inches;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(10);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  // initialize the button pin as a input:
  //pinMode(buttonPin, INPUT_PULLUP);
  
  // Ausgänge initialisieren (für PWM Ausgabe) 
  pinMode(LightPin1, OUTPUT) ; //Ampellicht links
  pinMode(LightPin2, OUTPUT) ; //Ampellicht links mitte       
  pinMode(LightPin3, OUTPUT) ; //Ampellicht mitte        
  pinMode(LightPin4, OUTPUT) ; //Ampellicht rechts mitte        
  pinMode(LightPin5, OUTPUT) ; //Ampellicht rechts 
  
  // LCD1 Display initialisieren 
  lcd1.init(); //Im Setup wird das LCD1 gestartet
  lcd1.backlight(); //Hintergrundbeleuchtung von LCD1 einschalten (0 schaltet die Beleuchtung aus).  
  lcd1.clear();
  
  lcd1.setCursor(0,0); //Der folgende Text soll beim LCD1 beim ersten Zeichen in der ersten Zeile beginnen.
  lcd1.print("Schlaubis Garage"); //In der ersten Zeile soll der Text „Display1 Zeile 1“ angezeigt werden    
  lcd1.setCursor(3,1); //Der folgende Text soll beim LCD1 beim ersten Zeichen in der zweiten Zeile beginnen.
  lcd1.print("V1.0 by CS"); //In der zweiten Zeile soll der Text „Display1 Zeile 2“ angezeigt werden  
  ResetLights20();
  delay(2500); //2,500 Sekunden
  
  lcd1.clear();
  lcd1.setCursor(0,0); //Der folgende Text soll beim LCD1 beim ersten Zeichen in der ersten Zeile beginnen.
  lcd1.print("Loading Mode:"); //In der ersten Zeile soll der Text „Display1 Zeile 1“ angezeigt werden                 
  lcd1.setCursor(0,1); //Der folgende Text soll beim LCD1 beim ersten Zeichen in der ersten Zeile beginnen.
  lcd1.print("Ultrasonic"); //In der ersten Zeile soll der Text „Display1 Zeile 1“ angezeigt werden                 

  ResetLights0();
  delay(1250); //1,2500 Sekunden
  lcd1.clear();
    
  // initialize serial communication:
  Serial.begin(9600);
  
  }

void loop() {

  cm = 0.01722 * readUltrasonicDistance(7, 8);
  
  if (cm >= 300 || cm <= 30)
  {
    Serial.println("Messwert ausserhalb des Bereichs! (30cm bis 300cm)");
    lcd1.clear();	// Display bereiningen
    //clearLCDLine;
  	lcd1.setCursor(3,0);	//Schreibe Zeile 1
  	lcd1.print("Messfehler");
  	lcd1.setCursor(0,1);	//Schreibe Zeile 2
  	lcd1.print("Warte 3 Sekunden");
    
    // Aktivieren der LEDs im Stoerungsfall (Efffekt)    
    FailsafeLightsFAST();
    
    delay(3000);	// Erneute Abfrage nach 3 Sekunden
    lcd1.clear();	// Display bereiningen (Fehlerkorrektur der Ueberlagerung nach gueltigem Messwert)
    
  }
  else
  {

  // Ultrasonic Lichtsteuerung (Abfolge Mitte nach Außen)
    
  // Ampel 3 (Mitte)
  if ((cm >=235) && (cm<= 289.9))    
    {   
    //Serial.println("AMPEL 3 EIN");
    analogWrite(LightPin3, 255);
    delay(500);
    //Serial.println("AMPEL 3 AUS");
    analogWrite(LightPin3, 0);
    delay(500);
    }  
  else if (cm >= 290)  
    {
    //Serial.println("AMPEL 3 AUS");  
    analogWrite(LightPin3, 0);
    delay(50);
    }
  else if (cm <= 235)  
    {
    //Serial.println("AMPEL 3 EIN");
    analogWrite(LightPin3, 255);
    delay(50);
    }    
    
  // Ampel 2&4 (Aussermittig)
  if ((cm >=185) && (cm<= 234.9))    
    {   
    //Serial.println("AMPEL 2&4 EIN");
    analogWrite(LightPin2, 255);
    analogWrite(LightPin4, 255);    
    delay(500);
    //Serial.println("AMPEL 2&4 AUS");
    analogWrite(LightPin2, 0);
    analogWrite(LightPin4, 0);   
    delay(500);
    }  
  else if (cm >= 245)  
    {
    //Serial.println("AMPEL 2&4 AUS");  
    analogWrite(LightPin2, 0);
    analogWrite(LightPin4, 0);    
    delay(50);
    }
  else if (cm <= 185)
    {
    //Serial.println("AMPEL 2&4 EIN");
    analogWrite(LightPin2, 255);
    analogWrite(LightPin4, 255);
    delay(50);
    }    
  
  // Ampel 1&5 (Aussermittig)
  if ((cm >=140) && (cm<= 184.9))    
    {   
    //Serial.println("AMPEL 1&5 EIN");
    analogWrite(LightPin1, 255);
    analogWrite(LightPin5, 255);    
    delay(500);
    //Serial.println("AMPEL 1&5 AUS");
    analogWrite(LightPin1, 0);
    analogWrite(LightPin5, 0);   
    delay(500);
    }  
  else if (cm >= 185)  
    {
    //Serial.println("AMPEL 1&5 AUS");  
    analogWrite(LightPin1, 0);
    analogWrite(LightPin5, 0);    
    delay(50);
    }
  else if (cm <= 140)  
    {
    //Serial.println("AMPEL 1&5 EIN");
    analogWrite(LightPin1, 255);
    analogWrite(LightPin5, 255);
    delay(50);
    }    
        
// Display SerialPrint Funktionen    
    
  Serial.print(cm , 1);
  Serial.println(" cm");
  
  //lcd1.print("Inches");
  //lcd1.setCursor(0,0);
  lcd1.setCursor(0,0);
  lcd1.print("Schlaubis Garage");
  
  //lcd1.setCursor(1,1);
  //lcd1.print(inches, 1);
  lcd1.setCursor(3,1);
  lcd1.print(cm);
  
  lcd1.setCursor(10,1);
  lcd1.print("cm");
  
  //lcd1.setCursor(14,1);
  delay(500);
  clearLCDLine;
    
  }
}  

// Ende Programmablauf
// Funktionen

void FailsafeLightsFAST() { //0,176 Sekunden
  
  		//Ablauf 1 (Links nach rechts)
		analogWrite(LightPin1, 255); //Ausgabe als PWM Wert 255=100%
		delay(176);
		analogWrite(LightPin1, 0); //Ausgabe als PWM Wert 0=0%
  
		analogWrite(LightPin2, 255);
		delay(176);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin3, 255);
		delay(176);
		analogWrite(LightPin3, 0);
  
		analogWrite(LightPin4, 255);
		delay(176);
		analogWrite(LightPin4, 0);
  
		analogWrite(LightPin5, 255);
		delay(176);
		analogWrite(LightPin5, 0);
  
		//Ablauf 2 Rechts nach Links  
		analogWrite(LightPin4, 255);
		delay(176);
		analogWrite(LightPin4, 0);
  
		analogWrite(LightPin3, 255);
		delay(176);
		analogWrite(LightPin3, 0);
  
		analogWrite(LightPin2, 255);
		delay(176);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin1, 255);
		delay(176);
		analogWrite(LightPin1, 0);
  
  		// Ablauf 3 (Links zur Mitte(bleibt erstmal EIN))
  
		analogWrite(LightPin2, 255);
		delay(176);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin3, 255);
		delay(176);  
  
		// Ablauf 4 (Mitte nach Außen)  
  
		analogWrite(LightPin2, 255);
		analogWrite(LightPin4, 255);
		delay(176);  
		analogWrite(LightPin1, 255);  
		analogWrite(LightPin5, 255);
		delay(176);  

  		//Ablauf 5 (Rechts nach Links AUS)
		analogWrite(LightPin5, 20);  
		delay(176);
		analogWrite(LightPin4, 20);  
		delay(176);
		analogWrite(LightPin3, 20);  
		delay(176);
		analogWrite(LightPin2, 20);
		delay(176);
		analogWrite(LightPin1, 20); 
  
  
}


void FailsafeLightsSLOW() { //15,000 Sekunden
  
  		//Ablauf 1 (Links nach rechts)
		analogWrite(LightPin1, 255); //Ausgabe als PWM Wert 255=100%
		delay(15000);
		analogWrite(LightPin1, 0); //Ausgabe als PWM Wert 0=0%
  
		analogWrite(LightPin2, 255);
		delay(15000);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin3, 255);
		delay(15000);
		analogWrite(LightPin3, 0);
  
		analogWrite(LightPin4, 255);
		delay(15000);
		analogWrite(LightPin4, 0);
  
		analogWrite(LightPin5, 255);
		delay(15000);
		analogWrite(LightPin5, 0);
  
		//Ablauf 2 Rechts nach Links  
		analogWrite(LightPin4, 255);
		delay(15000);
		analogWrite(LightPin4, 0);
  
		analogWrite(LightPin3, 255);
		delay(15000);
		analogWrite(LightPin3, 0);
  
		analogWrite(LightPin2, 255);
		delay(15000);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin1, 255);
		delay(15000);
		analogWrite(LightPin1, 0);
  
  		// Ablauf 3 (Links zur Mitte(bleibt erstmal EIN))
  
		analogWrite(LightPin2, 255);
		delay(15000);
		analogWrite(LightPin2, 0);
  
		analogWrite(LightPin3, 255);
		delay(15000);  
  
		// Ablauf 4 (Mitte nach Außen)  
  
		analogWrite(LightPin2, 255);
		analogWrite(LightPin4, 255);
		delay(15000);  
		analogWrite(LightPin1, 255);  
		analogWrite(LightPin5, 255);
		delay(15000);  

  		//Ablauf 5 (Rechts nach Links AUS)
		analogWrite(LightPin5, 20);  
		delay(15000);
		analogWrite(LightPin4, 20);  
		delay(15000);
		analogWrite(LightPin3, 20);  
		delay(15000);
		analogWrite(LightPin2, 20);
		delay(15000);
		analogWrite(LightPin1, 20); 
  
  
}

void ResetLights20() {
 		analogWrite(LightPin1, 20); //Ausgabe als PWM Wert 255=100%
  		analogWrite(LightPin2, 20);
  		analogWrite(LightPin3, 20);
  		analogWrite(LightPin4, 20);
  		analogWrite(LightPin5, 20);
}  

void ResetLights0() {
 		analogWrite(LightPin1, 0); //Ausgabe als PWM Wert 0=0%
  		analogWrite(LightPin2, 0);
  		analogWrite(LightPin3, 0);
  		analogWrite(LightPin4, 0);
  		analogWrite(LightPin5, 0);
}  

void clearLCDLine(int line) {               
        lcd1.setCursor(0,line);
        for(int n = 0; n < 16; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
        {
                lcd1.print(" ");
        }
}
