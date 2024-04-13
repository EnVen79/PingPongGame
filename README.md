# PingPongGame
PingPong game with two players. It is controlled using the Arduino Uno (2 buttons). The image is output to the matrix(8x8). The matrix is made of a smart LED strip. The ID of the LEDs starts from 0.
## The rules of the game:
- At the beginning: the ball appears on a random side in the upper or lower part of the matrix field and has a random direction of movement towards the nearest player
- If the ball touches the side wall, it bounces
- If a player misses the ball, then a goal is scored to the opposite participant, the arduino must output the current score via usb once every 1 second
- The ball is moving at a constant speed
- The length of the platform that you control can be 2 or 3 sections
