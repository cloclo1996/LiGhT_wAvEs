enum ledStates {INCREASE, DECREASE, STAY, WAVE, OFF, ON}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

int brightness = 0; // our main variable for setting the brightness of the LED
float velocity = 1.0; // the speed at which we change the brightness.
int ledPin = 9; // we use pin 9 for PWM
int TRIG = 10;
int ECHO = 11;
int DISTANCE;
int DURATION;
int i;
int distanceArray[20];
int filteredValue;

int p = 0; // use to keep track how often we plot
int plotFrequency = 3; // how often we plot, every Nth time.


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG, HIGH);
  compose();
  delay(10);
  //digitalWrite(TRIG, LOW);
  DURATION = pulseIn(ECHO, HIGH);
  DISTANCE = DURATION / 58.2;
  
analogWrite(ledPin, brightness);
  currentMillis = millis(); //store the current time since the program started
  for(i = 0; i < 20; i++){
    if(i <= 20){
      distanceArray[i] = DISTANCE; 
    }
    if (i > 20){
      distanceArray[i] = distanceArray[i-1];
    }
    filteredValue += distanceArray[i];
  }
  brightness = map((filteredValue/200), 200, 0, 255, 0);
}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops. 
  // instead we just switch from state to state when particular conditions are met.
  // we switch states by calling the changeState() function.

  
  switch (ledState){

  /*if (currentMillis - startMillis >= 5000){ //change state after 5 secs by comparing the time elapsed since we last change state
      changeState(INCREASE);
      }*/
  


  case INCREASE:
   if(brightness <= 255) {
    brightness = increase_brightness(brightness, 1);
   }

   plot("INCREASING", brightness);
        
    /*if (DISTANCE >= 40){
      //ledState = WAVE;
      changeState(DECREASE);
      }

    if (DISTANCE <= 15){ 
      changeState(INCREASE);
      }*/
    break;
   
  case DECREASE:
     if(brightness > 256) {
      brightness = decrease_brightness(brightness, 1);
     }
     plot("DECREASING", brightness);
     
      /*if (DISTANCE <= 41){
        changeState(INCREASE);
      }*/
     break;

  /*case WAVE:
    plot("WAVE", brightness);
    
    brightness = sinewave(1000,255,0); // you can tweak the parameters of the sinewave
    analogWrite(ledPin, brightness);
    
    if (currentMillis - startMillis >= 5000){ //change state after 5 secs by comparing the time elapsed since we last change state
      changeState(DECREASE);
      }
    break;
    
 case STAY:
    plot("STAY", brightness);
    brightness = brightness;
    break;

  case ON:
    plot("ON", brightness);
    brightness = 255;
    break;

  case OFF:
    plot("OFF", brightness);
    brightness = 0;
    if (currentMillis - startMillis >= 1000){
      changeState(INCREASE);
      }
    break;*/
  }
}

void changeState(ledStates newState){
    // call to change state, will keep track of time since last state
    startMillis = millis();
    ledState = newState;
  }
  
void plot(char *state, int brightness){
    // use this function to plot a graph.
    // it will normalize the auto-scaling plotter

    if ((p % plotFrequency) == 0){
      Serial.println(state);
      //Serial.print(", ");
      Serial.println(brightness);
      //Serial.println(", 0, 300");
      Serial.println(DISTANCE);
    }
    p++;
  }

int increase_brightness (int brightness, float velocity){
    return brightness = brightness + 1 * velocity;
  }

int decrease_brightness (int brightness, float velocity){
    return brightness = brightness - 1 * velocity;
  }

int sinewave(float duration, float amplitude, int offset){
    // Generate a sine oscillation, return a number.
    // In case you are using this for analogWrite, make sure the amplitude does not exceed 256
    float period = millis()/duration; // Duration in ms determines the wavelength.
    float midpoint = amplitude / 2; // set the midpoint of the wave at half the amplitude so there are no negative numbers
    int value = midpoint + midpoint * sin ( period * 2.0 * PI );
    value = value + offset; //offset allows you to move the wave up and down on the Y-axis. Should not exceed the value of amplitude to prevent clipping.
    return value;
  }
