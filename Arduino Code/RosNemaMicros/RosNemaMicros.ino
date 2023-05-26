#include "ros.h"                //Library has to be installed following process mentioned in README File
#include "geometry_msgs/Twist.h"

/* V1 Of Arduino Code For Project RVAC VOL:1 by Scinix 
Takes velocity information from ROS , calculates and commands A4988 stepper
motor drivers to move the Robot*/



const int dirPinl = 2;  //Pin determining the Direction of rotation of Left motor
const int stepPinl = 3; //Pin sending the step signals to the driver
const int dirPinr = 6;  //Same For Right
const int stepPinr = 7;
const int enablePin = 52; //Pin turning on or off both drivers incase of sleep

float vlin;
float vang;
float d = 0.29; //Distance between the 2 wheels
float vl;          // Left and Right Velocities 
float stepdelay1;  //    and Step Delays
float stepdelay2;
float vr;


ros::NodeHandle nh;

void velCallBack(  const geometry_msgs::Twist& vel){
  
  vlin = vel.linear.x;
  vang = vel.angular.z;

  vlin = constrain(vlin,-0.48,0.48); //Limiting Velocities to controlled range
  vang = constrain(vang,-3.3 , 3.3);

}



ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallBack); //Setup Subscriber to cmd_vel
unsigned int prevmicros1 = micros();
unsigned int prevmicros2 = micros();


void setup() {
  
  pinMode(stepPinl, OUTPUT); //Setting up all the pins
  pinMode(dirPinl, OUTPUT);
  pinMode(stepPinr, OUTPUT);
  pinMode(dirPinr, OUTPUT);
  pinMode(enablePin , OUTPUT);
  digitalWrite(enablePin , LOW);
  digitalWrite(dirPinr , LOW);
  digitalWrite(dirPinl , LOW);
  digitalWrite(stepPinl , LOW);
  digitalWrite(stepPinr , LOW);
  nh.initNode();
  nh.subscribe(sub);
 

  while (!nh.connected()){
    nh.spinOnce(); //Loops to check for updates on cmd_vel topic
  }

}

void loop() {
  
  
  nh.spinOnce();
  
  vl = vlin - (vang * d/2);  //Calculating Left and right velocities 
  vr = vlin + (vang * d/2);
  

  if(vl > 0){
    digitalWrite(dirPinl , HIGH);  //Setting direction of rotation for both
  }
  else if (vl < 0) {
    digitalWrite(dirPinl , LOW);
    vl = -1 * vl;
  }
  if(vr > 0){
    digitalWrite(dirPinr , LOW);
  }
  else if (vr < 0) {
    digitalWrite(dirPinr , HIGH);
    vr = -1 * vr;
  }
  
  if(vr == 0 && vl == 0){
    digitalWrite(enablePin , HIGH);  //driver turned off only if both velocities are 0
  }
  else digitalWrite(enablePin , LOW); // if any one of them is non zero then both are turned on
  


  if(vl!=0){
    unsigned int currentmicros1 = micros();
   
    stepdelay1 = 0.4814*2200/(vl*1); 
    if(currentmicros1 - prevmicros1 >= stepdelay1){ //If the difference in time is greater then said step delay then send a step signal for left
      digitalWrite(stepPinl,HIGH);
      digitalWrite(stepPinl,LOW);
      prevmicros1 = currentmicros1;
    
    }
  }

  if(vr!=0){
    unsigned int currentmicros2 = micros();
    stepdelay2 = 0.4814*2200/(vr*1);
    if(currentmicros2 - prevmicros2 >= stepdelay2){ ////If the difference in time is greater then said step delay then send a step signal for Right
      digitalWrite(stepPinr,HIGH);
      digitalWrite(stepPinr,LOW);
      prevmicros2 = currentmicros2;
      
    }
  }
  
}

