#pragma config(UserModel, "C:/Users/rstudent/code/robot-configs/35A.c")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Always open the debug stream window when the program starts
#pragma DebuggerWindows("debugStream")


#include <CKGeneral.h>
#include <CKVexMotors.h>
#include <CKMovingAverage.h>



float powerForSpeed( float speed ){
	return 0.0744 * exp( 0.0816 * speed );
}


float speedDialValue(){
	return 20*potentiometer(pot); //rad/sec of motor output shaft
}


task main(){

	//tMotor motorPorts[] = { mFlyLT, mFlyLB, mFlyRT, mFlyRB };//32
	//tMotor motorPorts[] = { mFlyR, mFlyL };//32A
	tMotor motorPorts[] = { mFlyT, mFlyB };//35A

	IMEMotorSet imems;
	IMEMotorSetInit( imems, motorPorts, 2, M393Turbo );

	//MovingAverage maAcceleration;
	//MovingAverageInit( maAcceleration, 5 );

	MovingAverage maVelocity;
	MovingAverageInit( maVelocity, 6 );

	float targetV, measV, vError, cruisePower, power;
	while(true){
		targetV = speedDialValue();
		measure( imems );
		nextSample( maVelocity, imems.ime.velocity );
		measV = getAverage(maVelocity);
		vError = measV - targetV;
		cruisePower = powerForSpeed(targetV);
		power = cruisePower + -0.03*sgn(vError)*vError*vError;
		setPower( imems, bound(power, 0, 1) );
		writeDebugStreamLine( "%.2f\t%.2f\t%.2f", MainBatteryVoltage(), targetV, measV );
		delay( 50 );
	}
}
