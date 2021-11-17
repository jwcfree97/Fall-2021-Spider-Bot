
class Motor {
  private:
    int pins[6];
    int states[6] = {LOW,LOW,LOW,LOW,LOW,LOW};
    int step_c = 0;

    void setup_pins() {
      for(int i=1; i<5;i++) {
        pinMode(pins[i],OUTPUT);
      }
    }
    
  public:
    Motor(int a, int b, int c, int d) {
      
      pins[0]=d;
      pins[1]=a;
      pins[2]=b;
      pins[3]=c;
      pins[4]=d;
      pins[5]=a;
      setup_pins();
      //pins={d,a,b,c,d,a};
    }


    void reset() {
      for(int i=1; i<5;i++) {
        digitalWrite(pins[i],LOW);
      }
    }

    void write_index(int i,int s) {
      digitalWrite(pins[i],s);
      states[i]=s;
      if(i==1 || i==5) {
        states[5]=s;
        states[1]=s;
      }
      if(i==4 || i==0) {
        states[0]=s;
        states[4]=s;
      }
    }

    void print_states() {
      String message="{";
      for(int i=0; i<sizeof(states);i++) {
        message=message+String(states[i]) +","; 
      }
      message+="}";
      //Serial.println(message);
    }
    
    void next_step() {
      if(step_c==0) {
        reset();
        write_index(1,HIGH);
        step_c++;
        return;
      } else {

      for(int i=1; i<5;i++) {
        if(states[i]==HIGH && states[i+1]==HIGH) {
          write_index(i,LOW);
          break;
        } else if(states[i]==HIGH) {
          write_index(i+1,HIGH);
          break;
        }
      }   

      }

      step_c++;
      if(step_c==8) {
        step_c==0;
      }
    }
};


Motor mot_a(2,3,4,5);
Motor mot_b(6,7,8,9);
Motor mots[2] = {mot_a,mot_b};

int dt=5;

void setup() {
  Serial.begin(9600);

}

void motorStep(int steps, int dt) {
  for(int a=0; a<steps; a++) {
    for(int i=0; i<sizeof(mots);i++) {
      mots[i].next_step();
      //mots[i].print_states();
    }
  }
  delay(dt);
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
      //blinkbuiltin();
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
