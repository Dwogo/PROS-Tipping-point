#include "main.h"

// V5 Parts
	// Controller
	Controller oleana (CONTROLLER_MASTER);
	Controller archie (CONTROLLER_PARTNER);
	// Motors
	Motor LBM(3, MOTOR_GEARSET_18 , false);
	Motor LFM(2, MOTOR_GEARSET_18, false);
	Motor RFM(10, MOTOR_GEARSET_18, true);
	Motor RBM(1, MOTOR_GEARSET_18, true);
	Motor DR4BL(18, MOTOR_GEARSET_36, false);
	Motor DR4BR(19, MOTOR_GEARSET_36, true);
	// Distance
	Distance LIFTO(20);

/// Tri port
	// Piston
	ADIDigitalOut CLAMPY(7);
	// Encoders
	ADIEncoder LYEN(1, 2, false);
	ADIEncoder RYEN(3, 4, false);





// Global Variables
	bool usercontrol = false;
	bool autonGo = false;
	bool sideAuto = false;
	int liftdir = 0;
	int clampState = 0;

// PID desired
	double desired = 0;
	double tesired = 0;