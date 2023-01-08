#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define Encoder_output_A 2 // pin2 of the Arduino
#define Encoder_output_B 3 // pin 3 of the Arduino



//defining objects
Servo myServo;
RF24 radio(PA4, PA3); // CE, CNS


// values to recieve
const byte address[6] = "00069";
String str_receive;                 
String str_ball;
String str_robots;
String str_robot_red;
String str_robot_green;
String str_ball_x;
String str_ball_y;
String str_robot_red_x;
String str_robot_green_x;
String str_robot_red_y;
String str_robot_green_y;
String str_data;

//speed measurement 
volatile long encoder0Pos=0;
long newposition;
long oldposition = 0;
unsigned long newtime;
unsigned long oldtime = 0;
long vel;

void Rotate_L(){   // default move to left 
   analogWrite(PA8,200);
   analogWrite(PB7,50);
  }
void Rotate_R(){   // default move to right 
   analogWrite(PA8,50);
   analogWrite(PB7,200);
  }
 void forward(){  // default move forward 
   analogWrite(PA8,50);
   analogWrite(PB7,200);
  }
  void back(){  // default move back  
  
    digitalWrite(PB4, LOW);
    digitalWrite(PB5, HIGH);
    digitalWrite(PB11, LOW);
    digitalWrite(PB10, HIGH);
    analogWrite(PA8,200);
    analogWrite(PB7,200);
 }
 
 void grab(){  // default move to grab 
  myservo.write(100);
  }
 
 void shoot(){ // default move to shoot 
  myservo.write(0);
  }



void setup() {
  
  // setting for encoder 
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  attachInterrupt(0, doEncoder, RISING);  // encoder ON PIN 2
  
  //trouble shooting serial out PA9  
  Serial1.begin(9600);
  
  // servo for shooting 
  myservo.attach(PB3);
  
  //setting up NRF
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  // enabling motor driver
  pinMode(PA8, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB5, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB10, OUTPUT);
  pinMode(PB11, OUTPUT);
  pinMode(PC13, OUTPUT);
    
    
  // output pin for motor +-
  digitalWrite(PB5, LOW);
  digitalWrite(PB4, HIGH);
  digitalWrite(PB10, LOW);
  digitalWrite(PB11, HIGH);
  digitalWrite(PC13, LOW);
  
  
  // writing speed 
  analogWrite(PA8,200);
  analogWrite(PB7,200);
  
  // serial config
  Serial1.begin(9600);
  
  // radio config
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // get orientation
  int player_angle = get_orientation();
}
void loop() {
  
if (radio.available()) {
  char text[18] = "";
  radio.read(&text, sizeof(text));
  
  Serial1.println(text);
  
  str_receive = String(text);
  
  Serial1.println(str_receive);
  
  for (int i = 0; i < str_receive.length(); i++)             
  // Split string into two string
  {
    if (str_receive.substring(i, i + 1) == ",")
    {
      str_ball_x = str_receive.substring(0, i);
      str_data = str_receive.substring(i + 1);
      if (str_data.substring(i, i + 1) == ",")
      {
        str_ball_y = str_data.substring(0, i);
        str_robots = str_data.substring(i + 1);
        if (str_robots.substring(i, i + 1) == ",")
        {
          str_robot_red_x = str_robots.substring(0, i);
          str_robot_red = str_robots.substring(i + 1);
          if (str_robot_red.substring(i, i + 1) == ",")
          {
            str_robot_red_y = str_robot_red.substring(0, i);
            str_robot_green = str_robot_red.substring(i + 1);
            if (str_robot_green.substring(i, i + 1) == ",")
            {
              str_robot_green_x = str_robot_green.substring(0, i);
              str_robot_green_y = str_robot_green.substring(i + 1);
            break;
            }
          }
        }
      }
    }
  }
  
  char ball_x[3];
  char ball_y[3];
  char robot_red_x[3];
  char robot_red_y[3];
  char robot_green_x[3];
  char robot_green_y[3];
  
  str_robot_green_x.toCharArray(robot_green_x, 3);
  str_robot_green_y.toCharArray(robot_green_y, 3);
  str_robot_red_x.toCharArray(robot_red_x, 3);
  str_robot_red_y.toCharArray(robot_red_y, 3);
 
  //Convert String into Char Array
  str_ball_x.toCharArray(ball_x, 3);
  str_ball_y.toCharArray(ball_y, 3);
  int robot_redx = atoi(robot_red_x);                   
  
  //Convery Array into integer value
  int robot_redy = atoi(robot_red_y);
  int robot_greenx = atoi(robot_green_x);
  int robot_greeny = atoi(robot_green_y);
  int ballx = atoi(ball_x);
  int bally = atoi(ball_y);

  }
  newposition = encoder0Pos;
  newtime = millis();
  vel = (newposition-oldposition) * 1000 /(newtime-oldtime);
  Serial.print ("speed = ");
  Serial.println (vel);
  oldposition = newposition;
  oldtime = newtime;

  angle = find_ball_angle();

  if (ball_avaiable()){
    get_ball();
    }
   
  
}
