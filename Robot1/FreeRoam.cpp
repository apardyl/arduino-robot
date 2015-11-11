// 
// 
// 

#include "FreeRoam.h"
#include "Robot.h"
#include "utilities.h"

uint16_t getDist() {
	uint16_t readings[5];
	for (uint8_t i = 0; i < 5;i++) {
		delay(50);
		robot.sensors.sonar.read();
		Serial.println(robot.sensors.sonar.dist());
		readings[i] =  robot.sensors.sonar.dist();
	}
	sort(readings, 5);
	return readings[2];
}

int findWay() {
	const int8_t directions[] = { -60, -30, 30, 60, 0 };
	int best = 0;
	uint16_t max = 0;

	char buff[21];

	for (uint8_t i = 0; i < 5; i++) {
		robot.sonarServo.wakeup();
		delay(100);
		robot.sonarServo.set(180 - (directions[i] + 90));
		delay(150);
		robot.sonarServo.sleep();
		delay(200);
		uint16_t dist = getDist();
		sprintf(buff, "dist %u at %d", dist, directions[i]);
		robot.logger.log(buff);
		if(dist > max) {
			best = directions[i];
			max = dist;
		}
	}

	if (max < 20) {
		best = 180;
	}
	if (best == 30) best = 50;
	if (best == -30) best = -50;
	if (best == 60) best = 100; //sam si? dziwi?, ?e to pisz?.
	if (best == -60) best = -100; //TODO: Unfuck this.

	sprintf(buff, "I turn %d", best);
	robot.logger.log(buff);
	return best;
}

void freeRoam() {
	static uint32_t nextMove;
	static int heading;

	enum Stage {START, MOVE, ROTATE, FINDWAY};

	static Stage stage;
	static int turn;

	static uint16_t distt[3];
	static uint8_t disti;
	
	if (nextMove < millis()) {
		switch (stage) {
			case MOVE: 
				disti = (disti + 1) % 3;
				robot.sensors.sonar.read();
				distt[disti] = robot.sensors.sonar.dist();
				//Serial.println(distt[disti]);
				if (distt[0] + distt[1] + distt[2] < 80) {
					robot.motors.motorBreak();
					robot.logger.log("Obstacle detected!");
					stage = FINDWAY;
				}
				nextMove = millis() + 100;
				break;
			case START:
				robot.motors.motorBreak();
				robot.logger.log("Moving forward");
				delay(100);
				robot.motors.setEngines(63, 63);
				stage = MOVE;
				for (uint8_t i = 0; i < 3;i++) {
					distt[i] = 9999;
				}
				break;
			case ROTATE:
				robot.sensors.lsm303.read();
				static int currentheading;
				currentheading = int(robot.sensors.lsm303.heading());
				//Serial.println(currentheading);
				//Serial.println(heading);
				//Serial.println((heading + 360 - currentheading) % 360);

				if ((heading + 360 - currentheading) % 360 < 10) stage = START;

				nextMove = millis() + 10;
				break;
			case FINDWAY: 
				delay(200);
				
				turn = findWay();

				robot.sensors.lsm303.read();
				heading = (int(robot.sensors.lsm303.heading()) + turn + 360) % 360;

				//Serial.println(turn);

				if (turn > 0) {
					robot.motors.setEngines(63, -63);
				}
				else if (turn < 0) {
					robot.motors.setEngines(-63, 63);
				}
				else {
					stage = START;
					break;
				}

				stage = ROTATE;
				break;
			default: break;
		}
	}
}