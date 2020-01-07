#include <NewPing.h>
#include <Stepper.h>
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
#define BUTTON 22

//Dioda
#define LED 3

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP); //Przycisk
  pinMode(LED, OUTPUT); //Dioda
  digitalWrite(LED, LOW);
  
  pinMode(motor1_dir,OUTPUT);
  pinMode(motor1_pwm,OUTPUT);
  pinMode(motor2_dir,OUTPUT);
  pinMode(motor2_pwm,OUTPUT);

  servo.attach(servoPin);
}

void ServoFront()
{
  servo.write(80);
}

void ServoLeft()
{
  servo.write(130);
}

void ServoRight()
{
  servo.write(50);
}

bool CheckIfObstacle()
{
  if (frontSonar.ping_cm() <= 30 && frontSonar.ping_cm() != 0 )
  {
    return false;
  }
  else if (frontSonar.ping_cm() > 30 || frontSonar.ping_cm() == 0) 
  {
    return true;
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
  delay(100);
}

void RunBackward()
{
  digitalWrite(motor1_dir,LOW);
  analogWrite(motor1_pwm,100);
  digitalWrite(motor2_dir,LOW);
  analogWrite(motor2_pwm,100);
  delay(100);
}

void StopMotors()
{
  digitalWrite(motor1_dir,LOW);
  analogWrite(motor1_pwm,0);
  digitalWrite(motor2_dir,LOW);
  analogWrite(motor2_pwm,0);
}


void RunMotor()
{  
  if (CheckIfObstacle() == false)
    {
      ServoLeft();
      RunBackward();
      delay(500);
      ServoRight();
      RunForward();
      delay(100);
      ServoLeft();
      RunBackward();
      delay(500);
    }
  else if (CheckIfObstacle() == true)
    {
      ServoFront();
      RunForward();
    }
}

void LightLed()
{
  if (POWER == 0)
  {
    digitalWrite(LED, LOW);
  }
  else if (POWER == 1)
  {
    digitalWrite(LED, HIGH);
  }
}

void PowerOn()
{
  if (digitalRead(22) == LOW && POWER == 0) //Jeśli przycisk wciśnięty
  { 
      POWER = true;
      delay(500);
  }
}

void loop() {
  LightLed();
  RunMotor();
  CheckDistance();
  Serial.print(POWER);
}
