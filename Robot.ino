#include <NewPing.h>
#include <Servo.h>

//Silniki
int motor1_dir = 12;
int motor1_pwm = 13;
int motor2_dir = 9;
int motor2_pwm = 8;
bool POWER = false;

//Czujnik zbliżeniowy
#define TRIGGER_PIN  7
#define ECHO_PIN  6
#define MAX_DISTANCE  200
NewPing frontSonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Servo
int servoPin = 5;
int servoAngle = 0;
Servo servo;

//Przycisk
#define BUTTON 2


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  
  pinMode(motor1_dir,OUTPUT);
  pinMode(motor1_pwm,OUTPUT);
  pinMode(motor2_dir,OUTPUT);
  pinMode(motor2_pwm,OUTPUT);

  servo.attach(servoPin);
}

void ServoFront()
{
  delay(20);
  servo.write(80);
  delay(20);
}

void ServoLeft()
{
  delay(20);
  servo.write(50);
  delay(20);
}

void ServoRight()
{
  delay(20);
  servo.write(110);
  delay(20);
}

bool CheckIfObstacle()
{
  if (frontSonar.ping_cm() <= 30 && frontSonar.ping_cm() != 0 )
  {
    return true;
  }
  else if (frontSonar.ping_cm() > 30 || frontSonar.ping_cm() == 0) 
  {
    return false;
  }
}

void CheckDistance()
{
  Serial.print("Ping: ");
  Serial.print(frontSonar.ping_cm());
  Serial.println("cm\n");
  delay(100);
}

void RunForward()
{
  digitalWrite(motor1_dir,HIGH);
  analogWrite(motor1_pwm,0);
  digitalWrite(motor2_dir,HIGH);
  analogWrite(motor2_pwm,0);
}

void RunBackward()
{
  digitalWrite(motor1_dir,LOW);
  analogWrite(motor1_pwm,255);
  digitalWrite(motor2_dir,LOW);
  analogWrite(motor2_pwm,255);
}

void StopMotors()
{
  digitalWrite(motor1_dir,LOW);
  analogWrite(motor1_pwm,0);
  digitalWrite(motor2_dir,LOW);
  analogWrite(motor2_pwm,0);
}

void BypassObstacle()
{
  StopMotors();
  delay(20);
  ServoRight();
  RunBackward();
  delay(1000);
}

void RunMotor()
{ 
  if (POWER == false)
  {
    StopMotors();
  }
  else if (POWER == true)
  {
      if (CheckIfObstacle() == true)
      {
        BypassObstacle();
      }
      else if (CheckIfObstacle() == false)
      {
        ServoFront();
        RunForward();
      }
   }
}


void PowerOn()
{
  if (digitalRead(BUTTON) == LOW && POWER == false)
  { 
      ServoFront();
      POWER = true;
      delay(20);
  }
}

void loop() {
  PowerOn();
  RunMotor();
}
