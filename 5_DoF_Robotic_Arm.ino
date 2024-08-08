#include <Servo.h>
#include <Encoder.h>

Encoder myEnc(2, 4);

Servo base_servo;
Servo shoulder_servo;
Servo elbow_servo;
Servo wrist_servo;
Servo gripper_rotation_servo;
Servo gripper_servo;

int base;
int shoulder;
int elbow;
int wrist;
int gripper_rotation;
int gripper;

int homeAngle= 90;
int stepValue = 3;

static int Gripper_Button_State = 0;


void setup() {

Serial.begin(9600);

   base_servo.attach(3);
   shoulder_servo.attach(5);
   elbow_servo.attach(6);
   wrist_servo.attach(9);
   gripper_rotation_servo.attach(10);
   gripper_servo.attach(11);

  pinMode(8,INPUT);
  pinMode(7,INPUT);
   shoulder = analogRead(0);
   elbow = analogRead(1);
   wrist = analogRead(2);
   gripper_rotation = analogRead(3);
   gripper = analogRead(4);

}
long oldPosition_Base =-999;
long oldPosition_Shoulder =-999;
long oldPosition_Elbow =-999;
long oldPosition_Wrist =-999;
long oldPosition_GrRot =-999;

static int Emergency_STOP_Button_State = 0;


void loop() {

 long newPos_Base =  myEnc.read();
 long newPos_Shoulder = analogRead(0);
 long newPos_Elbow = analogRead(1);
 long newPos_Wrist = analogRead(2);
 long newPos_GrRot = analogRead(3);
 

 //Έλεγχος Κίνησης Βάσης (Πρώτος Βαθμός Ελευθερίας)
 if (newPos_Base != oldPosition_Base) {

    if(newPos_Base >  oldPosition_Base){
      int newStep = abs(newPos_Base - oldPosition_Base);
      homeAngle -= stepValue;
      base_servo.write(homeAngle);
      if(homeAngle <81){
          homeAngle =81;      
          base_servo.write(homeAngle);
      }
    }

    if(newPos_Base <  oldPosition_Base ){
      int newStep = abs(newPos_Base - oldPosition_Base);      
      homeAngle += stepValue;
      base_servo.write(homeAngle);
      if(homeAngle >108)
          homeAngle =108;
          base_servo.write(homeAngle);
     }
    oldPosition_Base = newPos_Base;//remember the new position
  }
 if( digitalRead(12) == HIGH){
     homeAngle =homeAngle;
     base_servo.write(homeAngle);
  }
   
   
   // Έλεγχος Κίνησης Ώμου (Δεύτερος Βαθμός Ελευθερίας)

   if (newPos_Shoulder > oldPosition_Shoulder){
    newPos_Shoulder = map(newPos_Shoulder, 0, 1023, 0, 180);
    delay(15);
    shoulder_servo.write(newPos_Shoulder);
    //Serial.println(newPos_Shoulder);
   }

   // Έλεγχος Κίνησης Αγκώνα (Τρίτος Βαθμός Ελευθερίας)

   if (newPos_Elbow > oldPosition_Elbow){
    newPos_Elbow = map(newPos_Elbow, 0, 1023, 70, 120);
    delay(15);
    elbow_servo.write(newPos_Elbow);
    
   }
 // Έλεγχος Οριζόντιας Κίνησης Καρπού (Τέταρτος Βαθμός Ελευθερίας)
   if  (newPos_Wrist > oldPosition_Wrist){
    newPos_Wrist = map(newPos_Wrist, 0, 1023, 0, 180);
    delay(15);
    wrist_servo.write(newPos_Wrist);
   
  }
   
 // Έλεγχος Κατακόρυφης Κίνησης Καρπού (Πέμπτος Βαθμός Ελευθερίας)
  if (newPos_GrRot > oldPosition_GrRot){
    newPos_GrRot = map(newPos_GrRot, 0, 1023, 0, 180);
    delay(15);
    gripper_rotation_servo.write(newPos_GrRot);
    
  }
   // Έλεγχος Κίνησης Αρπάγης
  
  if(digitalRead(7)==HIGH){
    delay(100);
    if(Gripper_Button_State==0){
      Gripper_Button_State = 1;
      }
    else{
      Gripper_Button_State = 0;
      
    }
   }
   if (Gripper_Button_State == 0){
    gripper_servo.write(90);
    delay(15);
    //Serial.println("GRIPPER: OPEN");
    delay(100);
    }
   else{
    gripper_servo.write(0);
    delay(15);
    // Serial.println("GRIPPER: CLOSED");
     delay(100);
   } 
 
 Serial.println("Base: "+String(homeAngle)+" "+"Shoulder: "+String(newPos_Shoulder)+" "+"Elbow: "+String(newPos_Elbow)+" "+"Wrist Horizontal: "+String(newPos_Wrist)+" "+"Wrist Vertical: "+String(newPos_GrRot));

 //ΟΤΑΝ ΠΑΤΗΘΕΙ ΤΟ EMERGENCY STOP ΣΤΑΜΑΤΑΕΙ Η ΛΕΙΤΟΥΡΓΊΑ ΤΩΝ ΣΕΡΒΟΚΙΝΗΤΗΡΩΝ 
 while(digitalRead(8)== HIGH){
   if (Emergency_STOP_Button_State == 1){   
    Emergency_STOP_Button_State = 0;  
    }
   else {
    Emergency_STOP_Button_State = 1;
   }
    
    while (Emergency_STOP_Button_State == 1)
    {base_servo.write(90);
    elbow_servo.write(95); 
    delay(100);
    
    }
 }

}
