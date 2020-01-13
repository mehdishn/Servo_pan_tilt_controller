/*
 * Notes on GWS S35/STD countinuous rotation servo:
 *    send 93 for stop
 *    0 is fwd at full speed
 *    180 is backward at full speed
 *    use stop before changing direction
 * Notes on SG-5010 180 rotationa servo:
 *    range = [176, 0]
 *    okay mid with no buzz = 95, 85, 83
*/

#include <Servo.h>
  // global variables:
  const int x_servo_pin = 9, x_servo_min = 0, x_servo_max = 176, default_x_pos = 83;
  const int y_servo_pin = 10, c_servo_stop = 93; 
  int x_pos = default_x_pos, y_pos = c_servo_stop;
  Servo x_servo, y_servo;  
  const int joystick_button = 4; // digital pin 2 has a pushbutton attached to it

  bool check_button_press(int pin)
{
  int buttonState = digitalRead(pin); // read the input pin:
  //  Serial.println(buttonState); // print out the state of the button:
  return buttonState;
}

void blink(int pin)
{
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
}

int which_way_to_turn() // 0 = do_nothing, 1 = right, 2 = left, 3 = up, 4 = down
{
    int x = analogRead(A2); // x axix range (0, 1023) default = 495  - myleft = 1023
    int y = analogRead(A1); // y axix range (0, 1023) default = 520  - mydown = 0

    if(x > 800) 
      return 2; // left
    else if (x < 200)
      return 1;
    else if (y > 800)
      return 3;
    else if (y < 200)
      return 4; // down
    else
      return 0;  
}
void set_servo_x_position(Servo& x_servo, int& pos, const int joystick_in)
{
  const int turning_speed = 1;
  if(joystick_in == 2) // left
    pos = pos + turning_speed;
  else if(joystick_in == 1) // right
    pos = pos - turning_speed;

  if (pos > x_servo_max)
    pos = x_servo_max;
  else if(pos < x_servo_min)
    pos = x_servo_min;

  x_servo.write(pos);   // tell servo to go to position in variable 'pos'
}


void set_servo_y_position(Servo& c_servo, const int c_stop, const int joystick_in)
{
  const int fwd_speed = 73 , bwd_speed = 113;
  if(joystick_in == 4) // down
    c_servo.write(bwd_speed);
  else if(joystick_in == 3) // up
    c_servo.write(fwd_speed);
  else if(joystick_in == 0 || joystick_in == 1 || joystick_in == 2)
    c_servo.write(c_stop);
}

void setup()
{
  pinMode(13, OUTPUT);  // initialize digital pin 13 as an output. -> in case u need to blink built in LED
  pinMode(joystick_button, INPUT); // make the pushbutton's pin an input:
  Serial.begin(9600); // initialize serial communication at 9600 bits per second: for serial print
  y_servo.attach(y_servo_pin); // attaches the continuous rotation servo on pin x to the servo object
  x_servo.attach(x_servo_pin); // attaches the 180 rotation servo on pin x to the servo object
}

void loop()
{
  int joystick_in = which_way_to_turn(); // print out joystick movments
  // Serial.println(joystick_in);
  // Serial.println(x_pos);
  set_servo_x_position(x_servo, x_pos, joystick_in);
  set_servo_y_position(y_servo, c_servo_stop, joystick_in);
  
  delay(5);    // waits 15ms for the servo to reach the position
  
  if(check_button_press(joystick_button))
    blink(13);
}

