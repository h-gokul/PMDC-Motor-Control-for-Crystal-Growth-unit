#include <EEPROM.h>

const int pmp = 9; // pump pin
const int Relaysig = 4; // heateron
const int waterlevel = 7; // waterstatusread
const int in = A0; // LM35
int motoren1 = 5, motoren2 = 6, swap = 0; // MOTOR_O/P

float pwm = 45, rpm, error, inc = 2;
String inpt, T1, T2, T3, T4, T5, T6, T7, T8, motorSpeed,motorSpeedd, stemp;
int t1 , t2 , t3 , t4, t5, t6 , t7 , t8 , motorSpeed1 , motorSpeed2, settemp;
int acdr = 1, pmpflag = 0, flag1, r, rev, f = 0;
unsigned long prev, prev1, oldtime, preva, prevd, constime, restime;
int i = 20;
float acctime, deceltime;
int  level;
//------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{digitalWrite(10,LOW);
  attachInterrupt(digitalPinToInterrupt(3), isr, CHANGE);
  Serial.begin(9600); 
  pinMode(motoren1, OUTPUT); pinMode(motoren2, OUTPUT);
    analogWrite(motoren1,45);
    analogWrite(motoren2, 0);
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void isr()
{
  int a = digitalRead(3);
  if ((a == 1) && (f == 1))
  { rev = 0;
    f = 0;
  }
  if (a == 0) {
    rev = 1;
    f = 1;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
void loop()
{ input();
  obtain();
  
  if (rev > 0)
  { rpm = tach();
    if (acdr == 2) {
      constant();
    }
    
    if (acdr == 6) {
      t2 = t6; constant();
    }
    
   if (acdr == 1) {
      accel();
    }
    if (acdr == 7) {
      t3 = t7; decel();
    }

 if (acdr == 3) {
      decel();
    }
  if (acdr == 5) {
      t1 = t5; motorSpeed1 = motorSpeed2; accel();
    }

    //Serial.print(acdr); Serial.print(" "); Serial.print(inc); Serial.print(" "); Serial.print(pwm); Serial.print(" "); Serial.print(rpm); Serial.print(" "); Serial.print(millis()); Serial.println(" ");
  }
   
  if (acdr == 4) {
    rest();
  }
  if (acdr == 8) {
    t4 = t8;
    rest();
  }
  
  bathstatus();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void input()
{
  if (Serial.available() )
  {
    inpt = Serial.readString();
    if (inpt.startsWith("tmp")) {
      stemp = inpt.substring(3); 
    
     EEPROM[0] = stemp.toInt();
    }
    if (inpt.startsWith("t1")) {
      T1 = inpt.substring(2);
      
  EEPROM[1] =  T1.toInt();
    }
    if (inpt.startsWith("t2")) {
      T2 = inpt.substring(2);
    
    EEPROM[2]= T2.toInt(); 
    }
    if (inpt.startsWith("t3")) {
      T3 = inpt.substring(2);
 EEPROM[3] = T3.toInt();    }
 
    if (inpt.startsWith("t4")) {
      T4 = inpt.substring(2);
      
 EEPROM[4] = T4.toInt();    }
 
    if (inpt.startsWith("t5")) {
      T5 = inpt.substring(2);
      
 EEPROM[5] = T5.toInt();  
 }
    if (inpt.startsWith("t6")) {
      T6 = inpt.substring(2);
      
  EEPROM[6] =  T6.toInt();
  }
    if (inpt.startsWith("t7")) {
      T7 = inpt.substring(2);
      
   EEPROM[7] =  T7.toInt();
    }
    if (inpt.startsWith("t8")) {
      T8 = inpt.substring(2);
    EEPROM[8] = T8.toInt();
     
    }
    if (inpt.startsWith("spd")) {
      motorSpeed = inpt.substring(3);
   EEPROM[9] = motorSpeed.toInt();
    }
     if (inpt.startsWith("sed")) {
      motorSpeedd = inpt.substring(3);
   EEPROM[10]= motorSpeedd.toInt();
    }
   
    if (inpt == "pmp") {
      pmpflag = 1;
    }
   
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void obtain(){  
settemp =EEPROM[0];  
t1=EEPROM[1];
t2=EEPROM[2];    
t3=EEPROM[3];
t4=EEPROM[4];
t5=EEPROM[5];
t6=EEPROM[6];
t7=EEPROM[7];  
t8=EEPROM[8];  
motorSpeed1 =EEPROM[9] ;
motorSpeed2=EEPROM[10];
  }
//------------------------------------------------------------------------------------------------------------------------------------------------
int tach()
{
  long  atime = millis() - oldtime; //finds the time
  if (atime > 900)
  {
    r = (rev * 60000 ) / atime ;
  }
  //calculates rpm
  oldtime = millis();//saves the current time
  rev = 0;
  if (r < 60)
  {
    return r;
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void accel() {
  acctime = ((t1 * 1000)  / motorSpeed1);
  if (millis() - preva > acctime  )
  {
    pwm = pwm + inc ;
    analogWrite(motoren1, pwm);
    analogWrite(motoren2, 0);
    preva = millis();
  }
  if (rpm == motorSpeed1) {
    if (acdr < 5) {
      acdr = 2;
    }
    else {
      acdr = 6;
    }
    constime = millis();
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void constant()
{
  if (rpm != motorSpeed1)
  { float k = error / 55;
    pwm += (pwm * k ) ;
    if (pwm > 255) {
      pwm = 255;
    }
  }
  if ((motorSpeed1 != 0) && (pwm <= 42)) {
    pwm += 20 ;
  }
  analogWrite(motoren1, pwm);
  analogWrite(motoren2, 0);
  if ((millis() - constime) > t2 * 60000)
  {
    if (acdr < 5) {
      acdr = 3;
    }
    else {
      acdr = 7;
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void decel()
{
  deceltime = ((t3 * 1000)  / motorSpeed1);
  if (millis() - prevd > deceltime  )
  {
    pwm = pwm - inc ;
    analogWrite(motoren1, pwm);
    analogWrite(motoren2, 0);
    prevd = millis();
  }

  if (pwm <= 50) {
    if (acdr < 5) {
      acdr = 4;
    }
    else {
      acdr = 8;
    }
    restime = millis();
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void rest()
{
  digitalWrite(motoren1, LOW);
  digitalWrite(motoren2, LOW);
  if ((millis() - restime) > t4 * 60000) {
    if (acdr < 5) {
      acdr = 5;
      swap1();
analogWrite(motoren1,50);
analogWrite(motoren2,0);    }
    else {
      acdr = 1;
      swap1();
      analogWrite(motoren1,50);
      analogWrite(motoren2,0);
          }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void swap1(){
      swap =  motoren1;
      motoren1 = motoren2;
      motoren2 = swap;
      }

//------------------------------------------------------------------------------------------------------------------------------------------------      
void bathstatus() 
{
              if (pmpflag == 1) {
                digitalWrite( pmp, HIGH);
                if (millis() - prev1 > 10000) {
                  pmpflag = 0;
                  prev1 = millis;
                }
              }
              
 
     if (millis() - prev > 500)   {//Check  for every half a second 
//-----temperature control code-------------------------------------------------------------------------------------------------------------------------------------------
//settemp = 35;
//     
//            int ctemp = analogRead(in);
//            ctemp = ctemp * 0.48828128 ;
//            //    boolean level = digitalRead(waterlevel);
//            int error = settemp - ctemp;
//            if (error > 1) {
//              digitalWrite(Relaysig, HIGH); Serial.print(ctemp); Serial.print("c   "); Serial.println("on");
//            }
//            if (error < 1) {
//              digitalWrite(Relaysig, LOW); Serial.print(ctemp); Serial.print("c   "); Serial.println("off");
//            }
//          // reset time.
//         }
 //------------------------------------------------------------------------------------------------------------------------------------------------------       
        
        level = digitalRead(waterlevel);
        if (level <200)
        { digitalWrite(pmp, HIGH);
//          Serial.print(ctemp); Serial.print("c   ");
            Serial.println("BELOW LEVEL");
        }
        if (level>200)
        { digitalWrite(pmp, LOW);
//          Serial.print(ctemp); Serial.print("c   "); 
          Serial.println("SAFE");
         }
      prev = millis();      
      }
}
