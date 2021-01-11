// Button pins - 2, 3, 4, 5
// LED pins - 8, 9, 10, 11

// Buttons
int R_BUTTON = 2;
int G_BUTTON = 3;
int B_BUTTON = 4;
int Y_BUTTON = 5;

// LEDs
int RLED = 8;
int GLED = 9;
int BLED = 10;
int YLED = 11;

// Booleans

//to control flow
bool acceptingInput, waitingForInput, newIteration, gameOver;
//for buttons
bool lastR, currR, lastG, currG,
     lastB, currB, lastY, currY = false;
int RledOn, GledOn, BledOn, YledOn = 0;
bool Rchanged, Gchanged, Bchanged, Ychanged = false;
bool buttonPressed;


// ints
int score = 0; 
int inputCounter = 0;
unsigned long currTime;
int resizeFactor = 2; // Used when resizing the arrays (don't change value)

// Can change if u want
const int restTime = 350; // deadzone (in milliseconds) since last button press 
const int resizeAmount = 5; // Intial array size and amount of spaces added each time it's resized

 

// Arrays
int *commands = calloc(resizeAmount, sizeof(int)); // color sequence
int *input = calloc(resizeAmount, sizeof(int));// buttons the user presses
const char *colors[] = {"Red", "Green", "Blue", "Yellow"}; // map numbers to colors
unsigned long prevTime[] = {millis(), millis(), millis(), millis()}; // previous button press times 




// ------------------ Functions --------------------------------

// Turns all led's off
void allOff(){
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, LOW);
  digitalWrite(BLED, LOW);
  digitalWrite(YLED, LOW);
}

// Turns all led's on
void allOn(){
  digitalWrite(RLED, HIGH);
  digitalWrite(GLED, HIGH);
  digitalWrite(BLED, HIGH);
  digitalWrite(YLED, HIGH);
}


// Debouncing
boolean debounce(int buttonPin, boolean last)
{
  boolean current = digitalRead(buttonPin);       // Read the button state
  if (last != current)                         // If it's different...
  {
    delay(5);                                  // Wait 5ms
    current = digitalRead(buttonPin);             // Read it again
  }
  return current;                              // Return the current value
}



void setup()
{
  // Buttons already initialized as inputs
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  Serial.begin(9600);

  // Lighting Sequence - Game automatically starts after 4 seconds;
  digitalWrite(RLED, HIGH);
  delay(750);
  allOff();
  digitalWrite(GLED, HIGH);
  delay(750);
  allOff();
  digitalWrite(BLED, HIGH);
  delay(750);
  allOff();
  digitalWrite(YLED, HIGH);
  delay(750);
  allOff();
  delay(750);
  
  Serial.print("Game starting now...\n");

  // Initialize flow control variables
  acceptingInput = false;
  newIteration = true;
  gameOver = false;


}

void loop()
{
    





  
  if (acceptingInput){
    currTime = millis(); // prev and curr time are used to eliminate inputs within [restTime] time 
                         // of last button press to avoid repeated press detections from the same press
    
    // Red Button State
    currR = debounce(R_BUTTON, lastR);
    if ( (currTime - prevTime[0] >= restTime) && lastR == HIGH && currR == HIGH) { // If button pressed
      digitalWrite(RLED, HIGH);
      delay(100); // Keep LED on for 100ms otherwise it's not visible
      Rchanged = true;
      buttonPressed = true;
      prevTime[0] = currTime;
    }
    else{
      digitalWrite(RLED, LOW);
    }
    lastR = currR;
  
    // Green Button State
    currG = debounce(G_BUTTON, lastG);
    if ( (currTime - prevTime[1] >= restTime) && lastG == HIGH && currG == HIGH) {
      digitalWrite(GLED, HIGH);
      delay(100);
      Gchanged = true;
      buttonPressed = true;
      prevTime[1] = currTime;
    }
    else{
      digitalWrite(GLED, LOW);
    }
    lastG = currG;
  
    // Blue Button State
    currB = debounce(B_BUTTON, lastB);
    if ( (currTime - prevTime[2] >= restTime) && lastB == HIGH && currB == HIGH) {
      digitalWrite(BLED, HIGH);
      delay(100);
      Bchanged = true;
      buttonPressed = true;
      prevTime[2] = currTime;
    }
    else{
      digitalWrite(BLED, LOW);
    }
    lastB = currB;
  
    // Yellow Button State
    currY = debounce(Y_BUTTON, lastY);
    if ( (currTime - prevTime[3] >= restTime) && lastY == HIGH && currY == HIGH) {
      digitalWrite(YLED, HIGH);
      delay(100);
      Ychanged = true;
      buttonPressed = true;
      prevTime[3] = currTime;
    }
    else{
      digitalWrite(YLED, LOW);
    }
    lastY = currY;

    // --------------------------- Processing Player Input ----------------------------------------
    if (Rchanged){
      input[inputCounter] = 8; // Add led value to input array
      Rchanged = false; // Button can now be triggered again
      Serial.println("Red Pressed");
    }
    if (Gchanged){
      input[inputCounter] = 9;
      Gchanged = false;
      Serial.println("Green Pressed");
    }
    if (Bchanged){
      input[inputCounter] = 10;
      Bchanged = false;
      Serial.println("Blue Pressed");
    }
    if (Ychanged){
      input[inputCounter] = 11;
      Ychanged = false;
      Serial.println("Yellow Pressed");
    }

    if (buttonPressed){
      if (input[inputCounter] != commands[inputCounter]){ // input does not match sequence
        Serial.println("\nWrong button pressed!");
        Serial.print("Pressed: "); Serial.println(colors[input[inputCounter]-8]); 
        Serial.print("Supposed to press: "); Serial.println(colors[commands[inputCounter]-8]); 
        gameOver = true; // You lost because you pressed the wrong button
        acceptingInput = false;
        newIteration = false;
      }
      else if (inputCounter == (score-1)){ // successfully replicated all commands
        acceptingInput = false;
        newIteration = true; 
        allOff();
        
      }
      buttonPressed = false; // ready to accept user input again
      inputCounter += 1;
    }
    
  }











  if (newIteration){
    delay(800); //Don't want new iter to start immediately
    
    // resize array every [resizeAmount] iterations
    if ((score != 0) && (score % resizeAmount == 0)){
      commands = realloc(commands, resizeFactor * resizeAmount * sizeof(int));
      input = realloc(input, resizeFactor * resizeAmount * sizeof(int));
      Serial.print("Resized Array to size: "); Serial.println(resizeAmount * resizeFactor);
      resizeFactor += 1;
    }

    // add random light to sequence
    randomSeed(millis());
    int newLED = random(8, 12);
    commands[score] = newLED;

    // Printing Sequence for debugging
    Serial.println("-----------------------------------------------");
    Serial.print("\n");
    Serial.println("Sequence");
    for (int j=0; j<(score+1); j++){
      Serial.println(colors[commands[j]-8]); // The color of the led added
    }
    Serial.print("\n");
    
    
    for (int i=0; i<(score+1); i++){
      digitalWrite( commands[i], HIGH );
      delay(750);
      allOff();
      delay (150);
    }
    score += 1;
    newIteration = false;
    acceptingInput = true;
    inputCounter = 0;
  }






  
  if (gameOver){
    //Serial.println("Game Over!");
    Serial.print("Your score was: "); Serial.println(score);
    gameOver = false; // all flow control variables are false now
    
    // lighting sequence
    allOn();
    delay(500);
    allOff();
    delay(500);
    allOn();
    delay(500);
    allOff();
  }


}
