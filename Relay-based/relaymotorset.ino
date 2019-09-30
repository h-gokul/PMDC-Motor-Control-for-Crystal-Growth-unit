#include <EEPROM.h>

const int pmp = 10; // pump pin
//const int Relaysig = 4; // heateron
const int waterlevel = A0; // waterstatusread
const int in =A1 ; // LM35
int motoren1 =  9, swap = 0; // MOTOR_O/P relay terminals 7,8

float pwm = 40, rpm, error, inc = 1, minpwm=45;
String inpt, T1, T2, T3, T4, T5, T6, T7, T8, motorSpeed,motorSpeedd, stemp;
int t1 , t2 , t3 , t4, t5, t6 , t7 , t8 , motorSpeed1 , motorSpeed2, settemp;
int acdr = 1, pmpflag = 0, flag1, r, rev, f = 0;
unsigned long startime, prev, prev1, oldtime, preva, prevd, constime, restime,previsr;
int i = 20;
float acctime, deceltime;
int level;
//------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  attachInterrupt(digitalPinToInterrupt(3),isr, CHANGE); //rpm interrupt
  Serial.begin(9600); 
 pinMode(10,OUTPUT);pinMode(motoren1, OUTPUT);   pinMode(7, OUTPUT); pinMode(8, OUTPUT); digitalWrite(7,HIGH);digitalWrite(8,LOW);
    analogWrite(motoren1,50);startime = millis();
   // analogWrite(pmp,250);

}

//------------------------------------------------------------------------------------------------------------------------------------------------
void isr()
{
 
  int a = digitalRead(3);
  if ((a == 1) ||(f == 1))
  { rev = 0;
  //if ((millis() - previsr) >100){ 
    f = 0;
    //previsr = millis();} 
  }
  
  if ((a == 0)&&(f==0)) {
    rev = 1;
    f =1;
  }
 
}
//-----------------------------------------------------------------------------------------------------------------------------------
void loop()
{ input();
  obtain();
  bathstatus();
 
  if (rev > 0)
  { Serial.println("xx");
  rpm = tach();
    
    if (acdr == 2) {digitalWrite(7,HIGH);digitalWrite(8,LOW);
      constant();
    }
    
    if (acdr == 6) {digitalWrite(8,HIGH);digitalWrite(7,LOW);
      t2 = t6; constant();
    }

  
   if (acdr == 1) {digitalWrite(7,HIGH);digitalWrite(8,LOW);
      accel();
    }
     
   if (acdr == 3) {digitalWrite(7,HIGH);digitalWrite(8,LOW);
      decel();
    }  
 //   Serial.print(acdr); Serial.print(" "); Serial.print(acctime); Serial.print(" "); Serial.print(pwm); Serial.print(" "); Serial.println(rpm);
   if (acdr == 5) {digitalWrite(8,HIGH);digitalWrite(7,LOW);
      t1 = t5; motorSpeed1 = motorSpeed2; accel();
    }
  
   if (acdr == 7) {digitalWrite(8,HIGH);digitalWrite(7,LOW);
      t3 = t7; decel();
    }

   if (acdr == 8) {    t4 = t8; rest();}

  }
  
  
    if (acdr == 4){ rest(); }
    
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
  void testobtain(){
    settemp =1;
t1=20;
t2=1;    
t3=20;
t4=1;
t5=20;
t6=1;
t7=20;  
t8=1;  
motorSpeed1 =33 ;
motorSpeed2=33;
    }
//------------------------------------------------------------------------------------------------------------------------------------------------
int tach()
{
  long  atime = millis()- oldtime; //finds the time
  if (atime > 900)
  {
    r = (rev * 60000 ) / atime ;
  }
  //Serial.print(pwm);Serial.print(" "); Serial.print(rpm);Serial.print(" ");Serial.println(atime);
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
   // analogWrite(motoren2, 0);
    preva = millis();
  }  
  if (abs(rpm-motorSpeed1)<= 1) {
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
  //analogWrite(motoren2, 0);
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
    
    prevd = millis();
  }

  if (pwm <= minpwm) {
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
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  if ((millis() - restime) > t4 * 60000) {
    if (acdr < 5) {
      acdr = 5;
  //    swap1();
analogWrite(motoren1,minpwm);
//analogWrite(motoren2,0);   
}
    else {
      acdr = 1;
 //     swap1();
      analogWrite(motoren1,minpwm);
 
          }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
//void swap1(){
//      swap =  motoren1;
//      motoren1 = motoren2;
//      motoren2 = swap;
//      }

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
        
        level = analogRead(waterlevel);
        Serial.println(level);
        if (level <200 )
        { digitalWrite(pmp, HIGH);
//          Serial.print(ctemp); Serial.print("c   ");
       //     Serial.println("BELOW LEVEL");
        }
        if (level >200)
        { digitalWrite(pmp, 0);
//          Serial.print(ctemp); Serial.print("c   "); 
        //  Serial.println("SAFE");
         }
      prev = millis();      
      }
}
