
int a,b,c,d,leddebug;
int e,f,g,h;
int i,j,k,l;
int m,n,o,p;
double dt;
void setup() {
  //Setup 5 output pins. 4 for stepper motor inputs, 1 for debug led.
  Serial.begin(9600);
  dt=5;
  a=2;
  b=3;
  c=4;
  d=5;
  leddebug=13;
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(leddebug,OUTPUT);
  
}

void output_pins(int pina,int pinav,int pinb,int pinbv,int pinc,int pincv,int pind,int pindv, int delaytime) {
  //Set 4 output pins to their specified signal (HIGH or LOW)
  digitalWrite(pina,pinav);
  digitalWrite(pinb,pinbv);
  digitalWrite(pinc,pincv);
  digitalWrite(pind,pindv);
  //Delay a short amount of time
  delay(delaytime);
}

void motorStep(int steps,int dt) {
  if(steps>0) { //If positive direction
  for(int i=0;i<steps;i++) { //Perform 'steps' amount of motor steps
    
    /*
     * 
     * This logic makes the stepper motor move 1 step
     * 
     */
     
    output_pins(a,HIGH,b,LOW,c,LOW,d,LOW,dt);  
    output_pins(a,HIGH,b,HIGH,c,LOW,d,LOW,dt);  
    output_pins(a,LOW,b,HIGH,c,LOW,d,LOW,dt);
    output_pins(a,LOW,b,HIGH,c,HIGH,d,LOW,dt);
    output_pins(a,LOW,b,LOW,c,HIGH,d,LOW,dt);
    output_pins(a,LOW,b,LOW,c,HIGH,d,HIGH,dt);
    output_pins(a,LOW,b,LOW,c,LOW,d,HIGH,dt);
    output_pins(a,HIGH,b,LOW,c,LOW,d,HIGH,dt);
  }
  } else if(steps < 0) { //If negative direction
    for(int i=0;i>steps;i--) { //Perform 'steps' amount of motor steps
    output_pins(a,HIGH,b,LOW,c,LOW,d,HIGH,dt);
    output_pins(a,LOW,b,LOW,c,LOW,d,HIGH,dt);
    output_pins(a,LOW,b,LOW,c,HIGH,d,HIGH,dt);
    output_pins(a,LOW,b,LOW,c,HIGH,d,LOW,dt);
    output_pins(a,LOW,b,HIGH,c,HIGH,d,LOW,dt);
    output_pins(a,LOW,b,HIGH,c,LOW,d,LOW,dt);
    output_pins(a,HIGH,b,HIGH,c,LOW,d,LOW,dt);
    output_pins(a,HIGH,b,LOW,c,LOW,d,LOW,dt);  
    }
  }
}

void blinkbuiltin() {
  digitalWrite(leddebug,HIGH);
  delay(2000);
  digitalWrite(leddebug,LOW);
  delay(100);
}

void loop() {


   //Continuously check if serial port has data
   if(Serial.available()>0) {
    //Command name for moving stepper motor
    String stepCommand = "step=";
    String dtS = "delaytime=";
    //Read command from the serial port
    String returned=Serial.readString();
    int commandSplit = returned.indexOf("=")+1;
    //Gets index of step motor command (checks if step motor command was sent)
    String command = returned.substring(0,commandSplit);
    String value = returned.substring(commandSplit);
    
    
    if(returned=="test"){
      //Blink LED if command sent was 'test'
      blinkbuiltin();
      //Print onto the serial port a return message so the sender knows the command went through
      Serial.println("ABC : " + returned);
    } else if(command==stepCommand) { //If step motor command was sent
      
      value.trim(); //remove spaces
      int stepsCount = value.toInt(); //convert from string to integer
      motorStep(stepsCount,dt); //Move stepper motor however many steps were passed

      //Return message saying how many steps the motor moved (confirmation of command working)
      Serial.println("STEPPED : " + value);
    } else if(command==dtS) { //If step motor command was sent
      
      value.trim(); //remove spaces
      dt = value.toDouble(); //convert from string to integer
      //Return message saying how many steps the motor moved (confirmation of command working)
      Serial.println("DT CHANGED TO : " + value);
    } else {
      //Return error if the sent command was not found
      Serial.println("ERROR : " + returned);
    }
    delay(1000);
   }
}
