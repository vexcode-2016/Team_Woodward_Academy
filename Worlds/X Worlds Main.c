#pragma config(Sensor, in1,    claw,           sensorPotentiometer)
#pragma config(Sensor, in2,    arm,            sensorNone)
#pragma config(Sensor, dgtl5,  liftDown,       sensorTouch)
#pragma config(Sensor, dgtl6,  leftLiftDown,   sensorTouch)
#pragma config(Sensor, dgtl7,  liftEnc,        sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl11, lDriveEnc,      sensorQuadEncoder)
#pragma config(Motor,  port1,           leftClaw,      tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           lDriveFront,   tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           rDriveFront,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           dumpTopY,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           dumpOutsideLeft, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           dumpOutsideRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           dumpBottomY,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           lDriveBack,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           rDriveBack,    tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rightClaw,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//leftLiftDown touch sensor was added later.  To avoid having to change a bunch of references, liftDown is the right lift down sensor

//1/10: claw (2 motors)
//2/3: front drivetrain motors (2 motors)
//4 and 7: 2 Y-offs, 1 motor from each side of lift (4 motors)
//5-6: 1 lift motor each from each side of lift (2 motors) through PE
//8/9: back drivetrain motors (2) through PE

//Competition stuff
// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

//global variables
int LEFT = 1; //note that changing this value could affect gyro rotation function if LEFT is used for the value of the direction parameter in that function
int RIGHT = 2;
int AUTON_SIDE = 0; //either LEFT or RIGHT, as above
int AUTON_PLAY = 0;
int armPotOffset = 0; //The value of the claw potentiometer when the claw is fully closed and touching the physical limit
bool disableLiftComp = false;
bool LCD_CUBE = true;
bool LCD_STARS = true;

int getArmPos() {
	return SensorValue[claw] - armPotOffset;
}


//Our includes
#include "autonomousLib X.c"
#include "Position PID - Tank v2.c"
#include "../Claw Robots/LCD Wizard.c"
//setDumpMotors and setClawMotors are in autonomousLib.c


void pre_auton()
{
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	//bDisplayCompetitionStatusOnLcd = false;

	bLCDBacklight = true;
	bool testLcdWizard = false;
	if (bIfiRobotDisabled || testLcdWizard) { //Only show auton play wizard when the robot is in competition mode & disabled on initial startup
		startTask(LCDSelect);
	}
}



//potentiometer value for lift: 2150
int liftTarget;
int liftTime;
int clawTarget;
int liftgo = 0;
task liftTask()
{
	while(1)
	{
		if(liftgo == 1)
		{
			liftToTargetPIDEnc(liftTarget,liftTime,2,0.00035,.2);
			liftgo = 0;
		}
	}
}

task asyncLiftPID() {
	while(1) {
		if (!bIfiAutonomousMode) { //only do this in driver control
			disableLiftComp = true;
		}
		if (liftgo) {
			liftToTargetPIDEnc(liftTarget,liftTime,2.5,0.00035,.2);
			liftgo = 0;
		}
		if (!bIfiAutonomousMode) { //only do this in driver control
			disableLiftComp = false;
		}
	}
}

task clawTask()
{
	moveClaw(127,clawTarget);
}
void throw()
{
	setClawMotors(-50);
	liftTarget = 125;
	liftTime = 2200;
	liftgo = 1;
	startTask(liftTask);
	while(sensorValue[liftEnc] < 95)
	{
		wait1Msec(5);
	}
	clawTarget = 1750;
	startTask(clawTask);
}
task progSkills() {
	SensorValue[rDriveEnc] = 0;
	SensorValue[lDriveEnc] = 0;
	liftTarget = 135;
	clawTarget = 1750;
	startTask(clawTask);
	driveDistancePID(-250,STRAIGHT,1000);
	waitForClaw(1750,50);
	wait10Msec(250);
	moveClaw(127, 650);
	driveDistancePID(150,STRAIGHT,1000);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-75);
	driveDistancePID(-900,STRAIGHT,1500);
	throw();
	wait10Msec(20);
	driveDistancePID(150,STRAIGHT,1000);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	startTask(clawTask);
	//driveDistancePID(-300,STRAIGHT,1000)
	driveDistancePID(850,STRAIGHT,1300);
	wait10Msec(100);
	moveClaw(127, 675);
	driveDistancePID(150,STRAIGHT,1000);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-75);
	driveDistancePID(-1220,STRAIGHT,1500);
	throw();
	wait10Msec(80);
	driveDistancePID(150,STRAIGHT,1000);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	driveDistancePID(-300,STRAIGHT,1000)
	driveDistancePID(300,STRAIGHT,1000);
	strafeleft(175,127);
	moveClaw(127,820);
	SensorValue[rDriveEnc] = 0;
	driveDistancePID(480,ROTATE_RIGHT,1000);
	/*while(abs(SensorValue[rDriveEnc]) < 390.5)
	{
	setRightDtMotors(-85);
	setLeftDtMotors(85);
	}*/
	setRightDtMotors(0);
	setLeftDtMotors(0);
	driveDistancePID(1250,STRAIGHT,1500);
	setClawMotors(-127);
	wait10Msec(100);
	setClawMotors(-30);
	liftgo = 1;
	liftTarget = 70;
	startTask(liftTask);
	SensorValue[rDriveEnc] = 0;
	//liftToTargetPIDEnc(55,1000,2.5,0.00035,.2);
	driveDistancePID(480,ROTATE_LEFT,1000);
	setRightDtMotors(0);
	setLeftDtMotors(0);
	throw();
	liftgo = 1;
	startTask(liftTask);
	wait10Msec(200);
	driveDistancePID(250,STRAIGHT,1000)
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	/*SensorValue[rDriveEnc] = 0;
	while(SensorValue[rDriveEnc] < 735)
	{
	setRightDtMotors(85);
	setLeftDtMotors(-85);
	}
	setRightDtMotors(0);
	setLeftDtMotors(0);
	liftToPotTarget(3900, -127)
	moveClaw(127, 1900);*/
	driveDistancePID(175,STRAIGHT,1000);
	moveClaw(127, 550);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-25);
	driveDistancePID(-350,STRAIGHT,1000);
	throw();
	wait10Msec(80);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	driveDistancePID(-300,STRAIGHT,1000)
	driveDistancePID(300,STRAIGHT,1000);
	//straight(127, 200)
	driveDistancePID(1075,STRAIGHT,1500);
	moveClaw(127, 550);
	setClawMotors(-50);
	wait10Msec(100);
	driveDistancePID(-1075,STRAIGHT,1500);
	stopTask(liftTask);
	throw();
	wait10Msec(80);
	driveDistancePID(150,STRAIGHT,1000);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	driveDistancePID(-300,STRAIGHT,1000)
	driveDistancePID(300,STRAIGHT,1000);
	driveDistancePID(150,STRAIGHT,1000);
	startTask(clawTask);
	/*SensorValue[rDriveEnc] = 0;
	while(SensorValue[rDriveEnc] < 735)
	{
	setRightDtMotors(85);
	setLeftDtMotors(-85);
	}
	setRightDtMotors(0);
	setLeftDtMotors(0);
	liftToPotTarget(3900, -127)
	moveClaw(127, 1900);*/
	strafeRight(1000, 127);
	driveDistancePID(750,STRAIGHT,1500);
	wait10Msec(100);
	moveClaw(127, 550);
	driveDistancePID(150,STRAIGHT,1000);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-25);
	driveDistancePID(-1220,STRAIGHT,1500);
	throw();
	wait10Msec(80);
	driveDistancePID(150,STRAIGHT,1000);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);

}

int autonClawWait = 0;
task autonBigClawDelay() {
	wait1Msec(autonClawWait);
	startTask(clawTask);
}

void releaseClaw() {
	driveDistancePID(300,STRAIGHT,1000);
	moveClaw(127,850);
	setClawMotors(0);
	while (SensorValue[liftEnc] < 15) {
		setDumpMotors(127);
	}
	while (!SensorValue[liftDown]) {
		setDumpMotors(-127);
	}
	setDumpMotors(0);
	SensorValue[liftEnc] = 0;
}

task liftToBottomAsync() {
	liftToBottom();
}

task autonBig() {
	//releaseClaw();

	//liftToBottom();
	//SensorValue[liftEnc] = 0;
	driveDistancePID(-1700,STRAIGHT,3000);
	driveDistancePID(225,ROTATE_RIGHT,750);
	driveDistancePID(-100,STRAIGHT,500);
	clawTarget = 1250;
	/*
	driveDistancePID(-1600,STRAIGHT,1750);
	driveDistancePID(200,ROTATE_RIGHT,1000);
	clawTarget = 1800;
	*/
	startTask(clawTask);
	waitForClaw(1250,50); //value previously 1850, but 1250 makes more sense given clawTarget above
	setClawMotors(-10);
	wait1Msec(100);
	setClawMotors(0);
	if (SensorValue[claw] < 2000) {
		liftToBottom();
		SensorValue[liftEnc] = 0;
		setDumpMotors(-12);
		driveDistancePID(200,STRAIGHT,500);
		moveClaw(127,700,1500);
		setClawMotors(0);
		driveDistancePID(-200,STRAIGHT,500);
		liftToTop();
		wait1Msec(250);
		clawTarget = 1250;
		startTask(clawTask);
		startTask(liftToBottomAsync);
		waitForClaw(1250,50);
		setClawMotors(0);
		driveDistancePID(1200,STRAIGHT,3000);
		//moveClaw(127,1000,500);
		//setClawMotors(0);
		//while(!SensorValue[liftDown]) {
		//	setDumpMotors(-127);
		//}
		//setDumpMotors(-12);
	}
	//driveDistancePID(200,STRAIGHT,1000);
	//forward version
	/*
	while (!SensorValue[liftDown]) {
		setDumpMotors(-127);
	}
	setDumpMotors(0);
	SensorValue[liftEnc] = 0;
	setDumpMotors(-12);
	moveClaw(127,1290);
	setClawMotors(15);
	driveDistancePID(700, STRAIGHT, 3000);
	moveClaw(127,740,1200);
	setClawMotors(-20);
	liftTarget = 55;
	liftTime = 1000;
  liftgo = 1;
	startTask(asyncLiftPID);
	waitForLift(20,0); //move once lift has gone up some
	driveDistancePID(530,STRAIGHT,1500,50); //TODO: needs to go farther here
	driveDistancePID(400,ROTATE_RIGHT,1000)*/
	//end forward version


	//setClawMotors(-40);
	//waitForClaw(580,20); //430
	//SensorValue[liftEnc] = 0;
	//liftTarget = 69;
	//liftTime = 1000;
 // liftgo = 1;
	//startTask(asyncLiftPID);
	//wait1Msec(250);
	//driveDistancePID(500, STRAIGHT, 900);
	//setClawMotors(-80);
	//driveDistancePID(300, ROTATE_LEFT, 750);
	//driveDistancePID(700, STRAIGHT, 750);
	//moveClaw(127,820);
	//setClawMotors(15);

	//liftTarget = 80;
	//liftTime = 750;
 // liftgo = 1;
	//startTask(asyncLiftPID);

	//driveDistancePID(-300, STRAIGHT, 750);
	//liftTarget = 74;
	//liftTime = 500;
 // liftgo = 1;
	//startTask(asyncLiftPID);
	//moveClaw(127,1600);
	//setClawMotors(15);
	//driveDistancePID(600, STRAIGHT, 1000);
	//driveDistancePID(-300, STRAIGHT, 1000);
}

task autonStars() {
	//releaseClaw();
	//move out of the way of the wall and open claw
	driveDistancePID(-200,STRAIGHT,500);
	driveDistancePID(-300,STRAFE,750);
	moveClaw(127,1000);
	setClawMotors(15);

	//move in line with stars and collect them
	driveDistancePID(300,STRAFE,750);
	driveDistancePID(1400,STRAIGHT,2000);
	moveClaw(127,330);
	setClawMotors(-40);

	//lift to pick up stars
	liftTarget = 50;
	liftTime = 750;
  liftgo = 1;
	startTask(asyncLiftPID);

	//move back behind cube to score
	driveDistancePID(-1400,STRAIGHT,1500);
	setClawMotors(-20);

	//move away from wall and turn to score backwards on fence
	driveDistancePID(-800,STRAFE,750);
	wait1Msec(250);
	driveDistancePID(425,ROTATE_RIGHT,1000);

	//move back to fence
	driveDistancePID(-850,STRAIGHT,1750);

	//SCORE!
	throw();
	setClawMotors(15);
	while(!SensorValue[liftDown]) {
		setDumpMotors(-127);
	}
	setDumpMotors(0);
}

task autonomous() {
	wait10Msec(250);
	startTask(progSkills);
	//Auton plays and their numbers, for reference.  These numbers are set as the value of the AUTON_PLAY variable to control which auton play runs
	//#1 Big
	//#2 Small
	//#3 Cube (score middle cube and block)
	//#4 Fence (3 stars, corner)
	//#5 Nothing
	//#6 Prog skills

	//plays should differentiate between left and right using AUTON_SIDE and changing certain values accordingly (ex: running the right side version of a function)
	if (AUTON_PLAY == 1) { //uncomment line inside this block when task exists
			startTask(autonBig);
		} else if (AUTON_PLAY == 2) {
		//startTask(autonSmall);
		} else if (AUTON_PLAY == 3) {
		//startTask(autonCube);
		} else if (AUTON_PLAY == 4) {
		//startTask(autonFence);
		} else if (AUTON_PLAY == 5) {
		//Do nothing
		} else if (AUTON_PLAY == 6) {
		startTask(progSkills);
		stopTask(autonomous);
	}
	stoptask(autonomous)
	SensorValue[rDriveEnc] = 0;
	SensorValue[lDriveEnc] = 0;
	liftTarget = 800;
	clawTarget = 1750;//A
	//startTask(liftTask);
	//liftgo = 1;
	startTask(clawTask);
	diagonalLeft(-127,50);
	//waitForLift(2000,50);
	waitForClaw(1750,50);//A
	wait1Msec(250);
	straight(-127,1200);
	//wait1Msec(125);
	straight(127,150);
	/*SensorValue[rDriveEnc] = 0;
	while(SensorValue[rDriveEnc] < 735)
	{
	setRightDtMotors(85);
	setLeftDtMotors(-85);
	}
	setRightDtMotors(0);
	setLeftDtMotors(0);
	liftToPotTarget(3900, -127)
	moveClaw(127, 1900);*/
	strafeRight(1020, 127);
	straight(127, 75);
	moveClaw(127, 3200);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-25);
	straight(-127, 350);
	liftgo = 1;
	startTask(liftTask);
	waitForLift(100,50);
	stopTask(liftTask);
	setDumpMotors(127);
	wait1Msec(1100);
	startTask(clawTask);
	waitForClaw(1750,50);
	setDumpMotors(0);
	wait10Msec(80);
	setDumpMotors(-127);
	while(SensorValue[liftDown] == 0)
	{
		wait1Msec(25);
	}
	setDumpMotors(0);
	//straight(127, 200)
	straight(127,1075);
	moveClaw(127, 3200);
	setClawMotors(-50);
	wait10Msec(100);
	straight(-127,1075);
	stopTask(liftTask);
	liftTarget = 800;
	liftgo = 1;
	startTask(liftTask);
	waitForLift(100,50);
	stopTask(liftTask);
	setDumpMotors(127);
	wait1Msec(1250);
	startTask(clawTask);
	waitForClaw(1750,50);
	setDumpMotors(0);
	wait10Msec(80);
	liftToPotTarget(4000,-127);
	//arm down
	//wait
	//drive forward
	//wait
	//close claw
	//wait
	//lift arm
	//drive back
	//wait
	//raise arm
	//open claw
	//wait
}

/*task progSkills() {
	straight(-127,250);
	clawTarget = 2700;//A
	startTask(clawTask);
	waitForClaw(2700,100);
	setClawMotors(15);
	wait1Msec(1000);
	//clawTarget = 4095;
	//startTask(clawTask);
	//waitForClaw(4075,25);
	//setClawMotors(-20);
	//wait1Msec(1000);
	//liftTarget = 3670;
	//startTask(liftTask);
	//straight(-127,700);
	//wait1Msec(250);
	//liftTarget = 2100;
	//startTask(liftTask);
	//straight(-127,500);


	//liftTarget = 1950;

}*/

task clawControl()
{
	int PIDTargetValue;
	float kp = 0.5; //these constants might change down the road, but they are good for now
	float ki = 0.01;
	float kd = 0.00001;
	int error;
	int integral = 0;
	int derivative;
	int lastError = 0; //start this at 0 so the first time `derivative = error - lastError` runs there's no issue
	int PIDDrive;
	while(true)
	{
		if(vexRT[Btn6U]) //opens claw
		{
			setClawMotors(127);
			//do we want to clear the integral and lastError terms here?
			PIDTargetValue = SensorValue[claw];
		}
		else if(vexRT[Btn6D]) //closes claw
		{
			setClawMotors(-127);
			//do we want to clear the integral and lastError terms here?
			PIDTargetValue = SensorValue[claw];
		}
		else //holds position with PID
		{
			error = PIDTargetValue - SensorValue[claw];
			integral += error;
			derivative = error - lastError;
			PIDDrive = kp*error + ki*integral + kd*derivative;
			setClawMotors(PIDDrive);
			lastError = error;
		}
		wait1Msec(25); //prevents cpu hogging
	}
}


bool holdDown = false;
bool liftCompStarted = false;

//lift to 77
//claw to 1869

task liftComp() {
	setDumpMotors(0);
	wait1Msec(400);
	int target = SensorValue[liftEnc];
	liftToTargetPIDEnc(target,1000,3,0,0.4);
	while(1) {
		wait1Msec(500); //keep this task alive until it stops; the wait time here doesn't really matter, since the task will be stopped when it is no longer needed
	}
}
task usercontrol()
{
	bool ENABLE_DEBUG_MODE = true;
	bool DEBUG_RUN_AUTON = false;
	displayLCDCenteredString(0,"Debug - center");
	displayLCDCenteredString(1,"drvr mv joystck");
	while(ENABLE_DEBUG_MODE && abs(vexRT[Ch1]) < 30 && abs(vexRT[Ch2]) < 30 && abs(vexRT[Ch3]) < 30 && abs(vexRT[Ch4]) < 30) {
		wait1Msec(25);
		if (nPgmTime > 15000) {
			break;
		}
		if (nLCDButtons == 2) {
			DEBUG_RUN_AUTON = true;
			break;
		}
	}
	clearLCD();
	if (DEBUG_RUN_AUTON) {
		wait1Msec(1000);
		startTask(autonBig);
		//liftToTop();
		stopTask(usercontrol);
	}

	int LY = 0;
	int RY = 0;
	int threshold = 15;
	int armEncMaxLimit = 97; //software limit for potentiometer to limit arm movement from going over the top (protects potentiometer)
	bool enableSoftwareArmPosLimit = true; //experimental software limit for arm, see above
	int clawCompPower = 15;
	bool btn8UPressed = false;
  while(1)
  {
  	if(vexRT[Btn7D]){
  		//throw();
  		startTask(autonomous);
  		stopTask(usercontrol);
  	}
  	//for deadzones; when the joystick value for an axis is below the threshold, the motors controlled by that joystick will not move in that direction
  	LY = (abs(vexRT[Ch3]) > threshold) ? vexRT[Ch3] : 0;
  	RY = (abs(vexRT[Ch2]) > threshold) ? vexRT[Ch2] : 0;
   	setLeftDtMotors(LY);
   	setRightDtMotors(RY);

    if(vexRT[Btn7U])
		{
			clawTarget = 1750;
			startTask(clawTask);
			setClawMotors(18);
			liftToTargetPIDEnc(75,1000,2.5,0.00035,.2);
			setDumpMotors(15);
		}

  	if (vexRT[Btn7R]) { //don't uncomment this until releaseClaw is updated
  		//releaseClaw();
  	}

    if (vexRT[Btn5U] && (SensorValue[liftEnc] < armEncMaxLimit || !SensorValue[liftDown] || !enableSoftwareArmPosLimit)) {
	  	stopTask(liftComp);
	  	liftCompStarted = false;
	  	setDumpMotors(127);
	  	holdDown = false;
		} else if (vexRT[Btn5D] && (!SensorValue[liftDown] || SensorValue[liftEnc] > 10)) { //second part of condition is to prevent motors from jittering if 5U and 5D are pressed down
			stopTask(liftComp);
			liftCompStarted = false;
			setDumpMotors(-127);
		} else {
			//vertical at 117
			/*if (SensorValue[arm] > 3890) { //arm is all the way down; no compensation power
				setDumpMotors(0);
			} else if (SensorValue[arm] > 1350) { *///arm is up but has not gone past vertical (behind back of robot).  Positive compensation power
				if (SensorValue[liftDown]) {
					holdDown = true;
				}
				if (vexRT[Btn8U] && !btn8UPressed) {
					stopTask(liftComp);
					holdDown = false;
					liftTarget = 77;
					liftTime = 750;
					clawTarget = 1869;
					stopTask(asyncLiftPID);
					startTask(asyncLiftPID);
					liftgo = 1;
					startTask(clawTask);
					btn8UPressed = true;
					liftCompStarted = true; //so that when the lifting finishes, the driver control compensation code doesn't raise the lift even higher
				} else if ((holdDown || SensorValue[liftEnc] >= 117) && !disableLiftComp) {
					stopTask(liftComp);
					liftCompStarted = false;
					setDumpMotors(-12);
				} else if (!liftCompStarted && !disableLiftComp) { //don't restart this task unless the lift has moved and unless the midfence height macro isn't running
					startTask(liftComp);
					liftCompStarted = true;
				}
				if (!vexRT[Btn8U] && btn8UPressed) {
					btn8UPressed = false;
				}
			/*} else { //arm is up and behind the back of the robot.  Negative compensation power (and increased compensation power to protect potentiometer from crossing its physical limit and counter momentum)
				setDumpMotors(-armCompPower - 5);
			}*/
		}

  	if (vexRT[Btn6U]) {
			setClawMotors(127);
			clawCompPower = 0;
		} else if (vexRT[Btn6D]) {
			setClawMotors(-127);
			clawCompPower = -20;
		}	else {
			setClawMotors(clawCompPower);
		}

		if (SensorValue[liftDown] && (SensorValue[liftEnc] < 95 || SensorValue[liftEnc] > 110)) {
				SensorValue[liftEnc] = 0;
		}

  	wait1Msec(25);
	}
}
