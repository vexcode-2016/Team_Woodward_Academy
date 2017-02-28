#pragma config(Sensor, in1,    claw,           sensorPotentiometer)
#pragma config(Sensor, in2,    arm,            sensorNone)
#pragma config(Sensor, in3,    gyro,           sensorNone)
#pragma config(Sensor, dgtl1,  lDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armDown,        sensorTouch)
#pragma config(Sensor, dgtl6,  liftEnc,        sensorQuadEncoder)
#pragma config(Motor,  port1,           leftClaw,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           lDriveFront,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           lDriveBack,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           lDump12,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           lDump3,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rDump3,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           rDump12,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           rDriveBack,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           rDriveFront,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rightClaw,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
int armPotOffset = 230; //The value of the claw potentiometer when the claw is fully closed and touching the physical limit
bool disableLiftComp = false;

int getArmPos() {
	return SensorValue[claw] - armPotOffset;
}

//Our includes
#include "autonomousLib A.c"
#include "../State/Position PID - 4 Motor - X Drive.c"
#include "LCD Wizard.c"
//setDumpMotors and setClawMotors are in autonomousLib.c

void pre_auton()
{
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	bDisplayCompetitionStatusOnLcd = false;

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
	liftToPotTarget(liftTarget,127);
}
task clawTask()
{
	moveClaw(127,clawTarget);
}
task asyncLiftPID() {
	while(1) {
		if (!bIfiAutonomousMode) { //only do this in driver control
			disableLiftComp = true;
		}
		if (liftgo) {
			liftToTargetPIDEnc(liftTarget,liftTime,3.25,0.00050,.2);
			liftgo = 0;
		}
		if (!bIfiAutonomousMode) { //only do this in driver control
			disableLiftComp = false;
		}
	}
}
int autonClawWait = 0;
task autonBigClawDelay() {
	wait1Msec(autonClawWait);
	startTask(clawTask)
}

task autonBig() {
	//releaseClaw();
	clawTarget = 585; //420
	autonClawWait = 750;
	startTask(autonBigClawDelay);
	//moveClaw(127,560);
	driveDistancePID(750, STRAIGHT, 1000);
	//moveClaw(127,560);
	setClawMotors(-40);
	waitForClaw(550,20); //430
	SensorValue[liftEnc] = 0;
	liftTarget = 69;
	liftTime = 1000;
  liftgo = 1;
	startTask(asyncLiftPID);
	wait1Msec(250);
	driveDistancePID(500, STRAIGHT, 900);
	setClawMotors(-80);
	driveDistancePID(350, ROTATE_RIGHT, 750);
	driveDistancePID(800, STRAIGHT, 750);
	moveClaw(127,840);
	setClawMotors(15);

	liftTarget = 80;
	liftTime = 750;
  liftgo = 1;
	startTask(asyncLiftPID);

	driveDistancePID(-300, STRAIGHT, 750);
	//driveDistancePID(200, ROTATE_RIGHT, 400);
	liftTarget = 74;
	liftTime = 500;
  liftgo = 1;
	startTask(asyncLiftPID);
	moveClaw(127,1620);
	setClawMotors(15);
	driveDistancePID(700, STRAIGHT, 1000);
	driveDistancePID(-300, STRAIGHT, 1000);




	//clawTarget = 560;
	//autonClawWait = 400;
	//startTask(autonBigClawDelay);
	////moveClaw(127,560);
	//driveDistancePID(600, STRAIGHT, 1000);
	////moveClaw(127,560);
	//setClawMotors(-20);
	//waitForClaw(540,20);
	//wait1Msec(500);
	//liftTarget = 45;
	//liftTime = 1500;
	//liftgo = 1;
	//startTask(asyncLiftPID);
	//wait1Msec(250);
	//driveDistancePID(600, STRAIGHT, 750);
	//wait1Msec(250);
	//driveDistancePID(200, ROTATE_RIGHT, 1000);
	//liftToTargetPIDEnc(35,1000,2.5,0.00035,.2);
	//wait1Msec(250);

	//driveDistancePID(330, ROTATE_RIGHT, 1000);
	//driveDistancePID(600,STRAIGHT,1000);
	//moveClaw(127,1600);
	//setClawMotors(15);
}



task autonomous() {
	//Auton plays and their numbers, for reference.  These numbers are set as the value of the AUTON_PLAY variable to control which auton play runs
	//#1 Big
	//#2 Small
	//#3 Cube (score middle cube and block)
	//#4 Fence (3 stars, corner)
	//#5 Nothing
	//#6 Prog skills

	//plays should differentiate between left and right using AUTON_SIDE and changing certain values accordingly (ex: running the right side version of a function)
	if (AUTON_PLAY == 1) { //uncomment line inside this block when task exists
		//startTask(autonBig);
	} else if (AUTON_PLAY == 2) {
		//startTask(autonSmall);
	} else if (AUTON_PLAY == 3) {
		//startTask(autonCube);
	} else if (AUTON_PLAY == 4) {
		//startTask(autonFence);
	} else if (AUTON_PLAY == 5) {
		//Do nothing
	} else if (AUTON_PLAY == 6) {
		//startTask(progSkills);
	}

	SensorValue[rDriveEnc] = 0;
	SensorValue[lDriveEnc] = 0;
	liftTarget = 2000;
	clawTarget = 1111;//A
	startTask(liftTask);
	startTask(clawTask);
	diagonalLeft(127,50);
	waitForLift(2000,50);
	waitForClaw(1111,150);//A
	wait1Msec(250);
	straight(127,1700);
	wait1Msec(125);
	straight(-127,310);
	SensorValue[rDriveEnc] = 0;
	while(abs(SensorValue[rDriveEnc]) < 795)
	{
		setRightDtMotors(85);
		setLeftDtMotors(-85);
	}
	setRightDtMotors(0);

	setLeftDtMotors(0);
	SensorValue[rDriveEnc] = 0;
	liftToPotTarget(3800, -127);
	strafeRight(1120,127);
	straight(127, 75);
	moveClaw(127, 0);
	setClawMotors(-127);
	wait1Msec(500);
	setClawMotors(-20);
	stopTask(liftTask);
	liftTarget = 1650;
	startTask(liftTask);
	wait1Msec(1450);
	setClawMotors(127);
	wait1Msec(1000);
	setClawMotors(15);
	wait1Msec(500);
	liftToPotTarget(3800, -127);
	//straight(127,1075);
	//moveClaw(127, 0);
	//setClawMotors(-50);
	//straight(-127,1075);
	//stopTask(liftTask);
	//liftTarget = 1150;
	//startTask(liftTask);
	//wait1Msec(1000);
	//stopTask(clawTask);
	//startTask(clawTask);
}

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
task liftComp() {
	int target = SensorValue[liftEnc];
	liftToTargetPIDEnc(SensorValue[liftEnc]+5,1000,2,0.00035,.2);
}

task usercontrol()
{
	startTask(autonBig);
	stopTask(usercontrol);
	int LY = 0;
	int LX = 0;
	int RY = 0;
	int RX = 0;
	int threshold = 15;
	int armCompPower = 12; //compensation power for arm/lift
	int armEncMaxLimit = 118; //software limit for potentiometer to limit arm movement from going over the top (protects potentiometer)
	bool enableSoftwareArmPosLimit = true; //software limit for arm, see above
	int clawCompPower = 15;

  while(1)
  {
 // 	if(vexRT[Btn8U])
	//{
	// startTask(autonomous);
	// stopTask(usercontrol);
	//}
  	//for deadzones; when the joystick value for an axis is below the threshold, the motors controlled by that joystick will not move in that direction
  	LY = (abs(vexRT[Ch3]) > threshold) ? vexRT[Ch3] : 0;
  	LX = (abs(vexRT[Ch4]) > threshold) ? vexRT[Ch4] : 0;
  	RY = (abs(vexRT[Ch2]) > threshold) ? vexRT[Ch2] : 0;
  	RX = (abs(vexRT[Ch1]) > threshold) ? vexRT[Ch1] : 0;
    motor[lDriveFront] = LY + LX;
  	motor[lDriveBack] = LY - LX;
  	motor[rDriveFront] = RY - RX;
  	motor[rDriveBack] = RY + RX;

  	if (SensorValue[armDown]) {
  		SensorValue[liftEnc] = 0;
  	}

	  if (vexRT[Btn5U] && (SensorValue[liftEnc] < armEncMaxLimit || !enableSoftwareArmPosLimit || vexRT[Btn8U])) {
	  	stopTask(liftComp);
	  	liftCompStarted = false;
	  	setDumpMotors(127);
	  	holdDown = false;
		} else if (vexRT[Btn5D] && !SensorValue[armDown]) { //second part of condition is to prevent motors from jittering if 5U and 5D are pressed down
			stopTask(liftComp);
			liftCompStarted = false;
			setDumpMotors(-127);
		} else {

			//vertical at 117
			/*if (SensorValue[arm] > 3890) { //arm is all the way down; no compensation power
				setDumpMotors(0);
			} else if (SensorValue[arm] > 1350) { *///arm is up but has not gone past vertical (behind back of robot).  Positive compensation power
				if (SensorValue[armDown]) {
					holdDown = true;
				}
				if (holdDown || SensorValue[liftEnc] >= 117) {
					stopTask(liftComp);
					liftCompStarted = false;
					setDumpMotors(-12);
				} else if (!liftCompStarted) { //don't restart this task unless the lift has moved
					startTask(liftComp);
					liftCompStarted = true;
				}
			/*} else { //arm is up and behind the back of the robot.  Negative compensation power (and increased compensation power to protect potentiometer from crossing its physical limit and counter momentum)
				setDumpMotors(-armCompPower - 5);
			}*/
		}

		if (vexRT[Btn6U]) {
			setClawMotors(127);
			clawCompPower = 15;
		} else if (vexRT[Btn6D]) {
			setClawMotors(-127);
			clawCompPower = -20;
		}	else {
			setClawMotors(clawCompPower);
		}

  	wait1Msec(25);
	}
}
