import processing.serial.*;

// Set the necessary variables.
Serial myPort;
String val = "0";
int count = 0; // 
int state = 1; // current status: up(1), down(0)
int pstate = 0; // previous status
float distance=100;  
int std = 25; // state reference distance 
int rest_time = 25;

void setup()
{
  // Default screen and text settings
  String portName = Serial.list()[6];
  myPort = new Serial(this, portName, 9600);
  size(1280,720);
  fill(0);
  textSize(min(width,height)/5);
  textAlign(CENTER,CENTER);
}


void draw()
{
  // After counting 20,40 times, switches to the rest screen
  if (count==20){
    background(255,255,255); 
    text("Rest Time: "+rest_time, width/2,height/2);
    rest_time -= 1;
    delay(1000);
    if (rest_time == 0){
      count = 21;
      rest_time = 25;
    }
  }
  else if (count==40){
    background(255,255,255); 
    text("Rest Time: "+rest_time, width/2,height/2);
    rest_time -= 1;
    delay(1000);
    if (rest_time == 0){
      count= 41;
      rest_time = 25;
    }
  } 
  // When counting 60 times, the finished screen is displayed.
  else if (count==60){
    background(255,255,255); 
    text("FINISHED!", width/2,height/2);
  }
  else{
    background(255,255,255);
    // Receive distance information through serial communication with Arduino. 
    if ( myPort.available() > 0) val = myPort.readStringUntil('\n'); 
     try{
        distance = float(val);
      } catch(NumberFormatException e){} catch(Exception e){}
    // Push-up counting algorithm & output display
    if (distance >= std) state = 1;
    else if (distance <std) state = 0;
    if (state == 1 && pstate ==0) count += 1;
    text("Count: "+count,width/2,height/2);
    pstate = state;
  }
}
