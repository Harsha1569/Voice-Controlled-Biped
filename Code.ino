//The following code has four functions to control the car:
//MoveForward – this moves the car forward
//Movebackward – this moves the car in reverse
//TurRight – this spins the car right
//TurnLeft – this spins the car left
//Each of the functions has the same input parameters:
//Steps – the number of steps in the encoder disk to move the wheel
//mSpeed – the speed of the motors, from 0 to 255


#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE
#include <Dabble.h>
// Constants for interrupt pins
const byte MOTOR_A = 3; // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_B = 2; // Motor 1 Interrupt Pin - INT 0 - Left Motor
// Constant for steps in encoder disc
const float stepcount = 20.00; // 20 Slots in disk

// Constant for wheel diameter
const float wheeldiameter = 65.00; // Wheel diameter in millimeters

// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;

// Motor A
int enA = 10; //connected to ENABLE 1(pinout 1) of LM293D motor driver
int in1 = 9;
int in2 = 8;

// Motor B
int enB = 5; //connected to ENABLE 2(pinout 11) of LM293D motor driver
int in3 = 7;
int in4 = 6;

void setup() {
  Serial.begin(9600);       
  Dabble.begin(9600,11,12); // Connect bluetooth module to digital pin 11(RX) and digital pin 12(TX)
  pinMode(MOTOR_A, INPUT_PULLUP);
  pinMode(MOTOR_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING); //Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), ISR_countB, RISING); //Increase counter B when speed sensor pin goes High
}


void loop() {
Dabble.processInput();       //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.    
String voiceinput;           //this string variable takes in the voice command from the dabble app on the phone and sends the string in the format "XX YYYYYY" where XX represents the distance in cm and YYYYYY represents the direction of motion
float distance;             // the distance to be covered in cm is stored in this floating point variable
String Direction;            // this string variable stores the direction(forward, backward, left or right) in which the robot has to move/turn
int index;                  // this variable holds the index of the "space" that appears in the voiceinput string so as the split the input into a number(distance) and a string(direcion)
int i;
String x;                   // this variable is used to hold the charecter int the ith index of the voice input string while interrating through it
String space=" ";           // this String constant is used as a part of a logical operation that ends the for loop when a "space" is encountered
unsigned int stringlength;  // this variable stores the length of the voiceinput string
if(Terminal.available())   //This function tells if there is new data present in the terminal queue or not. It returns 0 if there is not data, and 1 if there is data present.
  {
    Serial.println("hi");
    voiceinput=Terminal.readString();
     stringlength=voiceinput.length();
     for(i=0;i<stringlength;i++){
      x=voiceinput.substring(i,i+1);
      if(x==space){
        index=i;
        break;
    }
  }
  distance=(float)(voiceinput.substring(0,index)).toInt();  // typecasting into floatiinng point variable after converting the string input into an integer
  Direction=voiceinput.substring(index+1);
  if(Direction=="forward"){
    MoveForward(CMtoSteps(distance),255); // here the speed of the DC motor(mspeed) is set to max,i.e, 255
  }
  if(Direction=="backward"){
    MoveBackward(CMtoSteps(distance),255);
  }
  if(Direction=="left"){
    TurnLeft(CMtoSteps(distance),255);
  }
  if(Direction=="right"){
    TurnRight(CMtoSteps(distance),255);
  }  
}
}


// Interrupt Service Routines
// Motor A pulse count ISR
void ISR_countA()
{
counter_A++; // increment Motor A counter value
}


// Motor B pulse count ISR
void ISR_countB()
{
counter_B++; // increment Motor B counter value
}


// Function to convert input distance in centimeters to steps(on the encoder disc)
int CMtoSteps(float cm) {
int result; // Final calculation result
float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
float cm_step = circumference / stepcount; // CM per Step
float f_result = cm / cm_step; // Calculate result as a float
result = (int) f_result; // Convert to an integer (note this is NOT rounded)
return result; // End and return result
}


// Function to Move Forward
void MoveForward(int steps, int mspeed)
{
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
// Set Motor A to move forward
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
// Set Motor B to move forward
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
// Go forward until step value is reached
while (steps > counter_A && steps > counter_B) {
if (steps > counter_A) {
analogWrite(enA, mspeed);
} else {
analogWrite(enA, 0);
}
if (steps > counter_B) {
analogWrite(enB, mspeed);
} else {
analogWrite(enB, 0);
}
}
// Stop when done
analogWrite(enA, 0);
analogWrite(enB, 0);
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
}


// Function to Move in Reverse
void MoveBackward(int steps, int mspeed)
{
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
// Set Motor A to move backward
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
// Set Motor B to move backward
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
// Go in reverse until step value is reached
while (steps > counter_A && steps > counter_B) {
if (steps > counter_A) {
analogWrite(enA, mspeed);
} else {
analogWrite(enA, 0);
}
if (steps > counter_B) {
analogWrite(enB, mspeed);
} else {
analogWrite(enB, 0);
}
}
// Stop when done
analogWrite(enA, 0);
analogWrite(enB, 0);
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
}



// Function to Turn Right
void TurnRight(int steps, int mspeed)
{
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
// Set Motor A to move backward
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
// Set Motor B to move forward
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
// Turn until step value is reached
while (steps > counter_A && steps > counter_B) {
if (steps > counter_A) {
analogWrite(enA, mspeed);
} else {
analogWrite(enA, 0);
}
if (steps > counter_B) {
analogWrite(enB, mspeed);
} else {
analogWrite(enB, 0);
}
}
// Stop when done
analogWrite(enA, 0);
analogWrite(enB, 0);
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
}



// Function to Turn Left
void TurnLeft(int steps, int mspeed)
{
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
// Set Motor A to move forward
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
// Set Motor B to move backward
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
// Turn until step value is reached
while (steps > counter_A && steps > counter_B) {
if (steps > counter_A) {
analogWrite(enA, mspeed);
} else {
analogWrite(enA, 0);
}
if (steps > counter_B) {
analogWrite(enB, mspeed);
} else {
analogWrite(enB, 0);
}
}
// Stop when done
analogWrite(enA, 0);
analogWrite(enB, 0);
counter_A = 0; // reset counter A to zero
counter_B = 0; // reset counter B to zero
}
