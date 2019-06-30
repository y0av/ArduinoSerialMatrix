The arduino expects a binary ascii String with size defined as PACKET_SIZE
after reciveing it completely it uses it to interpret the string as a matrix that correspend the first (PACKET_SIZE/NUM_OF_MOTORS) bits
as the orders for the first digital pin and so on
the total number of seconds to operate is MACHINE_SQN_TIME and the time frame is TIME_FRAME which means if the TIME_FRAME is 500 (half a second) 
then each bit correspond to half a second so each row in the input matrix is expected to be MACHINE_SQN_TIME * 2