#define NUM_OF_MOTORS 12 //number of motors we have
#define MACHINE_SQN_TIME 20 //number of seconds for the entire sequence
#define TIME_FRAME 500 //our minimum time frame is half a second which is 500 milliseconds
#define PACKET_SIZE 1000/TIME_FRAME * MACHINE_SQN_TIME * NUM_OF_MOTORS // 480 //number of bits in the expected serial packet

#define MSG_RECEIVING 'R'
#define MSG_STARTSEQUENCE 'S'
#define MSG_PLAYING 'P'
#define MSG_FINISHED 'F'

const int motors[NUM_OF_MOTORS] = { 2,3,4,5,6,7,8,9,10,11,12,13 }; //declaring output motor pins
bool receivedPacket[PACKET_SIZE]; //place to store the recived binary packet in an array
int pIndex = 0; //current index in recivedPacket

void setup() {
  for (int i=0; i<NUM_OF_MOTORS ; i++)
    pinMode(motors[i], OUTPUT);  //initialize all motor pins as outputs
  Serial.begin(9600); //starts serial connection
  //showSignOfLife();
}

void loop() {
  if (Serial.available()) {
    switch(Serial.read()) {
      case '0': validBitRecieved(false);
             break;
      case '1': validBitRecieved(true);
             break;
    }
  }
}

void showSignOfLife() {
  for (int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1);
    digitalWrite(LED_BUILTIN,LOW);
  }
  
}

void validBitRecieved(bool bit) {
  if (pIndex < PACKET_SIZE) {
    serialNotify(MSG_RECEIVING);
    receivedPacket[pIndex++] = bit; //stores received bit and then increment packet current index
  } else { //finished reading entire packet
    pIndex=0;
    startMotorsSequence();
  }
}

void serialNotify(char msg) {
  Serial.write(msg);
}

void startMotorsSequence() {
  serialNotify(MSG_STARTSEQUENCE);
  const int gapJump = MACHINE_SQN_TIME*1000 / TIME_FRAME; //result is 40 with current constants
  for (int timeFrameIndex = 0; timeFrameIndex < gapJump ; timeFrameIndex++) { 
    for (int motorIndex = 0 ; motorIndex < NUM_OF_MOTORS ; motorIndex++) {
      int boolSwitch = (receivedPacket[timeFrameIndex+motorIndex*gapJump]) ? 1 : 0;
      switch (boolSwitch) {
        case 1: digitalWrite(motors[motorIndex],HIGH);
                   break;
        case 0: digitalWrite(motors[motorIndex],LOW);
                   break;
      }
    }
    serialNotify(MSG_PLAYING);
    delay(TIME_FRAME);
  }
  serialNotify(MSG_FINISHED);
  shutDownAll();
}

void shutDownAll() {
  for (int motorIndex = 0 ; motorIndex < NUM_OF_MOTORS ; motorIndex++) {
    digitalWrite(motors[motorIndex],LOW);
  }
}
