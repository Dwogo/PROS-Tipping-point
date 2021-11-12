// 2976G Tipping Point Code

#include "main.h"

// Stating Components
	// V5 Parts
		// Controller
			Controller master (E_CONTROLLER_MASTER);
		// Motors
			Motor	LBM(4 , E_MOTOR_GEARSET_18 , true);
			Motor	LFM(8, E_MOTOR_GEARSET_18, true);
			Motor RFM(7, E_MOTOR_GEARSET_18, false);
			Motor RBM(5, E_MOTOR_GEARSET_18, false);
			Motor DR4BL(6, E_MOTOR_GEARSET_36, false);
			Motor	DR4BR(11, E_MOTOR_GEARSET_36, true);
	// Sensors
		// Piston
			ADIDigitalOut CLAMPY(7);
		// Encoders
			ADIEncoder LYEN(1, 2, false);
			ADIEncoder RYEN(3, 4, false);
			ADIEncoder XEN(5, 6, false);

// Global Variables
	bool usercontrol = false;

	// PID desired
		double desired = 0;
		double tesired = 0;

// LLEMU's center button
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		lcd::set_text(2, "I was pressed!");
	} else {
		lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	lcd::set_text(1, "Hello PROS User!");

	lcd::register_btn1_cb(on_center_button);

}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

}



// PID system for autonomous that hopefully gets implemented
int drovePID() {

	// Encoders
		//
			double yPos = LYEN.get_value() + RYEN.get_value();
			double xPos = XEN.get_value();
			double tspoon  = LYEN.get_value() - RYEN.get_value();
	// PID system
    // y Position
      double error = 0;
      double prevError = 0;
      double deriv = 0;
    // x Pos
      double serror = 0;
      double sPrevError = 0;
      double seriv = 0;
    // Turning
      float terror = 0;
      float tPrevError = 0;
      float teriv = 0;

	// Konstants
    // Going forward
      double kP = 0;
      double kI = 0;
      double kD = 0;
    // Turning
      double tkP = 0;
      double tkI = 0;
      double tkD = 0;

	while (!usercontrol) {
		// PID yPos
      error = yPos - desired;
      prevError += error;
      deriv = error - prevError;
    // PID Turning
      terror = tspoon - tesired;
      tPrevError += terror;
      teriv = terror - tPrevError;

		// Lateral movement
      double proton = (error * kP) + (prevError * kI) + (deriv * kD);
      double electron = (terror * tkP) + (tPrevError * tkI) + (teriv * tkD);

    // Motor Asignment
			LBM.move_velocity(proton + electron);
      LFM.move_velocity(proton + electron);
      RBM.move_velocity(proton - electron);
      RFM.move_velocity(proton - electron);

			delay(20);
		}
	return 1;
}

// Autonomous code
void autonomous() {
Task drivePD(drovePID);


}

void opcontrol() {
	usercontrol = true;
	while (usercontrol) {
	// Driving
		// CONTROLLER
			float mpwr = master.get_analog(ANALOG_LEFT_Y);
			float turn = master.get_analog(ANALOG_RIGHT_X);
		// Motors
			LBM.move(mpwr + turn);
			LFM.move(mpwr + turn);
			RBM.move(mpwr - turn);
			RFM.move(mpwr - turn);

	// Special Stuff
		// Clampy
			/*
				Declareing the clampState variable to specify what state we want the clamp's piston to be in
				We can also read this to determine what state the piston is currently in
			*/
			int clampState = 0;
			/*
				The Solinoid valve uses just a boolean 1 and 0 for opening and closing the valve
				if the controller button R2 is pressed the piston will fire down and clamp on the MOGO
				if button R1 is pressed the piston releases and the clamp opens
				the variable clampState is used the in "if" statement so it doesn't fire when it already is clamped
			*/
			if (master.get_digital(DIGITAL_R2) && clampState == 0) {
				clampState = 1;
			} else if (master.get_digital(DIGITAL_R1) && clampState == 1){
				clampState = 0;
			}
			// Setting the state to either high or low for the piston to fire or retract
			CLAMPY.set_value(clampState);

		// Lifting
			/*
				Declared variable for lifting the DR4B
			  127 is because the (motor).move function uses volts
				times the boolean value of Button R1 minus R2 being 1,0, or -1
				R1 meaning going up and R2 going down
			*/
			double liftPwr = 127 * (master.get_digital(DIGITAL_R1) - master.get_digital(DIGITAL_R2));
			/*
			Move the motors for the DR4B proportional to the variable liftPwr
			The value can be 127 (up) 0 (Stop) or -127 (Reverse)
			*/
			DR4BL.move(liftPwr);
			DR4BR.move(liftPwr);

		delay(20);
	}
}
