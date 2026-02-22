/*
  This Library is written for Stepper Motor that uses ULN2003
  Author: Bonezegei (Jofel Batutay)
  Editor:  Oleoleg (Oleg Shiryaev)
  Date:  February 2026
*/

#include "Bonezegei_ULN2003_Stepper.h"

Bonezegei_ULN2003_Stepper::Bonezegei_ULN2003_Stepper(int in1, int in2, int in3, int in4) {
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;
  _speed = 3;
  _mode = MODE_FULL_STEP;
}

bool Bonezegei_ULN2003_Stepper::begin() {
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  return 1;
}

void Bonezegei_ULN2003_Stepper::setSpeed(int speed) {
  _speed = speed;
}

void Bonezegei_ULN2003_Stepper::step(int dir, int steps) {
  isBusy = true;     
  currentDir = dir;   // Remember the direction (1 or 0)

  int cnt = 0;

  //FULL Step
  if (_mode == MODE_FULL_STEP) {

    //Direction FOrward
    if (dir) {
      for (int a = 0; a < steps; a++) {
		  
		if (stopFlag) {
			_releaseCoils();
			return;
		}
		 
        if (cnt == 0) {
          digitalWrite(_in1, HIGH);
          digitalWrite(_in2, HIGH);
          digitalWrite(_in3, LOW);
          digitalWrite(_in4, LOW);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 1) {
          digitalWrite(_in1, LOW);
          digitalWrite(_in2, HIGH);
          digitalWrite(_in3, HIGH);
          digitalWrite(_in4, LOW);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 2) {
          digitalWrite(_in1, LOW);
          digitalWrite(_in2, LOW);
          digitalWrite(_in3, HIGH);
          digitalWrite(_in4, HIGH);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 3) {
          digitalWrite(_in1, HIGH);
          digitalWrite(_in2, LOW);
          digitalWrite(_in3, LOW);
          digitalWrite(_in4, HIGH);
          delay(_speed);
          //delayMicroseconds(_speed);
        }
        cnt++;
        if (cnt == 4) {
          cnt = 0;
        }
      }
    } else {
      for (int a = 0; a < steps; a++) {
		  
		if (stopFlag) {
			_releaseCoils();
			return;
		}
		  
        if (cnt == 0) {
          digitalWrite(_in1, HIGH);
          digitalWrite(_in2, LOW);
          digitalWrite(_in3, LOW);
          digitalWrite(_in4, HIGH);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 1) {
          digitalWrite(_in1, LOW);
          digitalWrite(_in2, LOW);
          digitalWrite(_in3, HIGH);
          digitalWrite(_in4, HIGH);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 2) {
          digitalWrite(_in1, LOW);
          digitalWrite(_in2, HIGH);
          digitalWrite(_in3, HIGH);
          digitalWrite(_in4, LOW);
          delay(_speed);
          //delayMicroseconds(_speed);
        } else if (cnt == 3) {
          digitalWrite(_in1, HIGH);
          digitalWrite(_in2, HIGH);
          digitalWrite(_in3, LOW);
          digitalWrite(_in4, LOW);
          delay(_speed);
          //delayMicroseconds(_speed);
        }
        cnt++;
        if (cnt == 4) {
          cnt = 0;
        }
      }
    }
  } else {
  }

	_releaseCoils();
}

void Bonezegei_ULN2003_Stepper::stepAngle(int dir, float angle){
  int stp=0;
  if(_mode == MODE_FULL_STEP){
    stp= (int) (angle * 5.68);
    step(dir, stp);
  }
}  
  
void Bonezegei_ULN2003_Stepper::_releaseCoils() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, LOW);
	isBusy = false;
	currentDir = -1; // Reset direction when stopped
}
