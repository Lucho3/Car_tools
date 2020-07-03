#include <LiquidCrystal.h> // includes the LiquidCrystal Library

//----------------------------------

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd1(12, 10, 5, 4, 3, 2);

//----------------------------------

int Li          = 0;  //Variables for LCD left scroll.
int Lii         = 16; 

//----------------------------------

const int trigPin = 6;
const int echoPin = 7;
const int buzzerPin1 = 8;
const int buzzerPin2 = 9;
const int buzzerPin3 = 13;    //Mapping the pins.
const int greenLEDPin=A0;
const int yellowLEDPin=A1;
const int redLEDPin=A2;
const int gasPin=A3;
const int tmpPin=A4;
const int switchPin = A5;


//----------------------------------

bool firstState=false;
bool secondState=false;
bool thirdState=false;
bool fourthState=false;
long duration;
int distanceCm;  //Variables for the parctronic method.

//----------------------------------

String Scroll_LCD_Left(String StrDisplay)
{
  String result;
  String StrProcess =StrDisplay + "                ";  //Method that scrolls row from LCD from left to right.
  result = StrProcess.substring(Li,Lii);
  Li++;
  Lii++;
  if (Li>StrDisplay.length()-16){
    Li=0;
    Lii=16;
  }
  return result;
}

//----------------------------------


void setup() 
{
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(greenLEDPin, OUTPUT);
   pinMode(yellowLEDPin, OUTPUT);
   pinMode(redLEDPin, OUTPUT);
   pinMode(buzzerPin1, OUTPUT);
   pinMode(buzzerPin2, OUTPUT);
   pinMode(buzzerPin3, OUTPUT);
   pinMode(gasPin,INPUT);
   pinMode(tmpPin,INPUT);
   pinMode(switchPin, INPUT); 
}

//----------------------------------
bool bootedSystem=false;
void BootSystem()
{
   lcd.begin(16,2);
   lcd1.begin(16,2);
   lcd.display();
   lcd1.display();
   bootedSystem=true;
   lcd.print("System Is");
   lcd.setCursor(0,1);
   lcd.print("Booting!");
   lcd1.setCursor(0,0);
   lcd1.print("Pleace Wait 10 s.");
   lcd1.setCursor(0,1);
   lcd1.print("Seconds: ");
  
   for(int i=1; i<=10;i++)
   {
     lcd1.setCursor(9,1);
     lcd1.print(i);
     delay(1000);
   }
   lcd.clear();
   lcd1.clear();
}

//----------------------------------

void loop() 
{
  if (digitalRead(switchPin) == HIGH ) 
  {
     if(bootedSystem==false)
     {
       BootSystem();
     }

     Parctronic();
     GasDetecting();
     Temperature();
  }
  else
  {
    lcd.clear();
    lcd1.clear();
    lcd.noDisplay();
    lcd1.noDisplay();
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    digitalWrite(trigPin, LOW);   
    digitalWrite(buzzerPin1, LOW);
    digitalWrite(buzzerPin2, LOW);
    digitalWrite(buzzerPin3, LOW);
    digitalWrite(gasPin, LOW);
    digitalWrite(tmpPin, LOW);
    bootedSystem=false;
  }
}

//----------------------------------

void Parctronic()
{
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   distanceCm= duration*0.034/2;
  
   if(distanceCm>=332)
   {
     if(secondState==true || thirdState==true || fourthState==true)
     {
     	lcd.clear();
     }
   	 digitalWrite(greenLEDPin, LOW);
     digitalWrite(yellowLEDPin, LOW);
     digitalWrite(redLEDPin, LOW);
     firstState=true;
     secondState=false;
     thirdState=false;
     fourthState=false;
     lcd.setCursor(0,0);
	 lcd.print("Parktronic:");
     lcd.setCursor(0,1);
     lcd.print("Clear Sight!");
     delay(10);
   }
  
  else
   if(distanceCm>=75 && distanceCm<332)
   {
     if(firstState==true || thirdState==true || fourthState==true)
     {
     	lcd.clear();
     }
   	 digitalWrite(greenLEDPin, HIGH);
     digitalWrite(yellowLEDPin, LOW);
     digitalWrite(redLEDPin, LOW);
     secondState=true;
     firstState=false;
     thirdState=false;
     fourthState=false;
     lcd.setCursor(0,0);
	 lcd.print("Objects nearby!");
     lcd.setCursor(0,1);
     lcd.print("Distance: ");
     lcd.print(distanceCm);
     lcd.print(" Cm");
     delay(10);
   }
   
   else 
     if(distanceCm<75 && distanceCm>=40)
  		{
          if(firstState==true || secondState==true || fourthState==true)
          {
              lcd.clear();
    	  }
 
  	  	  digitalWrite(greenLEDPin, LOW);
  		  digitalWrite(yellowLEDPin, HIGH);
  		  digitalWrite(redLEDPin, LOW);
          secondState=false;
     	  firstState=false;
          thirdState=true;
          fourthState=false;
    	  lcd.setCursor(0,0);
		  lcd.print("Close Objects!");
    	  lcd.setCursor(0,1);												//Parctronic system.
    	  lcd.print("Distance: ");
    	  lcd.print(distanceCm);
    	  lcd.print(" Cm");
     	  Buzzer(400,100,buzzerPin1);
          delay(10);
  		}
   else
     if(distanceCm<40 && distanceCm!=332)
   		{
     		if(firstState==true || secondState==true || thirdState==true)
     		{
     			lcd.clear();
     		}
     
   		  digitalWrite(greenLEDPin, LOW);
  		  digitalWrite(yellowLEDPin, LOW);
  		  digitalWrite(redLEDPin, HIGH);
          secondState=false;
     	  firstState=false;
          thirdState=false;
          fourthState=true;
    	  lcd.setCursor(0,0);
    	  lcd.print("Too close!");  
     	  lcd.setCursor(0,1);
     	  lcd.print("Distance: ");
     	  lcd.print(distanceCm);
     	  lcd.print(" Cm");
          Buzzer(700,10,buzzerPin1);
          delay(10);
  		 }
}

//----------------------------------

void Buzzer(int frequency,int del, int pin)
{
  tone(pin, frequency); 
  delay(del);    
  noTone(pin);      //Method that controls the buzzers.
  delay(del);
}

//----------------------------------

void GasDetecting()
{
  int gasDetecting=analogRead(gasPin);
  if(gasDetecting>487)
  {
    clearLCD1Line(0);
  	Buzzer(1500,10,buzzerPin2);
    lcd1.setCursor(0,0);
    lcd1.print("Gas leaks!!!");  //Method that controls gas leaking.
    delay(500);    
  }
  else
  {  
    lcd1.setCursor(0,0);
    lcd1.print("No Gas Leaks!");
    delay(10);
  }
}

//----------------------------------

void clearLCD1Line(int line)
{               
   lcd1.setCursor(0,line);
   for(int n = 0; n < 17; n++)   //Method that clears one row from LCD1.
   {
     lcd1.print("  ");
   }
}

//----------------------------------

void Temperature()
{
  int reading = analogRead(tmpPin);  
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperatureC = (voltage - 0.5) * 100 ;  
                                               
 
  if(temperatureC>85)
  {
    clearLCD1Line(1);
   	Buzzer(1200,10,buzzerPin3);
    
    for (int i = 0; i <= 5; i++) 
    {
       lcd1.setCursor(0, 1);
       lcd1.print(Scroll_LCD_Left("High T Of The Engine!"));
       delay(30);
    } 
  
    clearLCD1Line(1);											//Method that controls engine temperature.
    lcd1.setCursor(0,1);
    lcd1.print("T: ");
    lcd1.print(temperatureC);
    lcd1.print(" C");
    delay(500);
    clearLCD1Line(1);
    lcd1.setCursor(0,1);
    lcd1.print("STOP THE CAR!");
    delay(500);
  }
  else
  {
    lcd1.setCursor(0,1);
    lcd1.print("Engine T:");
    lcd1.print(temperatureC);
    lcd1.print(" C");
    delay(10);
  }
}

//----------------------------------