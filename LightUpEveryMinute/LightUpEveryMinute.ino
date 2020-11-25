
//first we must define which pin will be used to power the relay.
const int RELAY_PIN = 4;

//this function runs only once at the start.
void setup() {

  //pinMode() configures the pin to be used either for input or output.
  pinMode(RELAY_PIN, OUTPUT);

}

//loop() runs infinitely, here we write our code.
void loop() {

  //when using a relay, to make it light up, we must set digitalWrite() to LOW.
  digitalWrite(RELAY_PIN, LOW);

  //this function pauses the program for the specified milliseconds.
  delay(1000);

  //to turn off the relay, we must set digitalWrite() to HIGH.
  digitalWrite(RELAY_PIN, HIGH);

  //this code pauses our program for 1 minute.
  for (int i = 0; i < 60; i++)
  {
    delay(1000);
  }

}
