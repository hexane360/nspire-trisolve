#include "Triangle.h"
using namespace std;

#define STATE(s) (_state == State::s)

Triangle::Triangle() {
	//cout << "Triangle::Triangle()\n";
	for (int i = 0; i < 3; i++) {
		_angles[i].setSuffix((char)248);
		_sides[i].setSuffix('\'');
	}
	_sides[0].setOffsetDir(Dimension::OffsetDown);
	_angles[0].setOffsetDir(Dimension::OffsetUp);
	_angles[1].setOffsetDir(Dimension::OffsetDown);
	_angles[2].setOffsetDir(Dimension::OffsetDown);
	reset();
}

/*
       ^  -  -  -
      /A\ B
     /   \
    b     c
   /       \
  /C       B\
  -----a-----
  note: clockwise is + rotation (because +y is down)
*/

void debugSolveState(double sideArray[3], double angleArray[3]) {
	cout << "a: " << sideArray[0] << endl;
	cout << "b: " << sideArray[1] << endl;
	cout << "c: " << sideArray[2] << endl;
	cout << "A: " << angleArray[0] << endl;
	cout << "B: " << angleArray[1] << endl;
	cout << "C: " << angleArray[2] << endl;
}

void Triangle::update() {
	int angleCount = 0;
	int sideCount = 0;
	double angle[3] = {0,0,0};
	double side[3] = {0,0,0};
	cout << "\nBeginning solve:\n";
	if (_checkConstraints(side, angle, sideCount, angleCount)) return; //constrains as necessary, returns on error
	if (_lawOfCosines(side, angle, sideCount, angleCount)) return;

	double sineLaw = 0;
	double ambiguous = 0;
	int solveIteration = 0;
	while (angleCount < 3 || sideCount < 3) { //while there's still solving left
		if (solveIteration > 2) {
			message("ERROR: Solve loop unending");
			return;
		}
		solveIteration++;
		if (angleCount == 2) { //use 180 = A + B + C
			cout << "Using interior angle sum\n";
			double sum = 0;
			int solveFor = 0;
			for (int i = 0; i < 3; i++) {
				if (!angle[i])
					solveFor = i;
				sum += angle[i];
			}
			if (180 - sum <= 0.001) { //solved angle would be invalid
				message("The given values are impossible");
				return;
			}
			angle[solveFor] = 180 - sum;
			cout << "angle[" << solveFor << "]: " << angle[solveFor] << endl;
			angleCount++;
		}
		if (!sineLaw) { //find angle/side pair to use with law of sines
			for (int i = 0; i < 3; i++) {
				if (side[i] && angle[i]) {
					cout << "Got Law of Sines pair from " << i << endl;
					sineLaw = side[i]/sin(angle[i]*TO_RADS);
					break;
				}
			}
		}
		if (sineLaw) { // a/sin(A) = b/sin(B) = c/sin(C)
			for (int i = 0; i < 3; i++) {
				if (!side[i] && angle[i]) { //find side given opposite angle
					side[i] = sin(angle[i]*TO_RADS)*sineLaw;
					cout << "side[" << i << "]: " << side[i] << endl;
					sideCount++;
				}
				if (side[i] && !angle[i]) { //find angle given opposite side
					double sum = 0;
					for (int j = 0; j < 3; j++) {
						sum += angle[j];
					}
					angle[i] = asin(side[i]/sineLaw)*TO_DEGS;
					if (std::isnan(angle[i])) {
						message("The given values are impossible");
						return;
					}

					if (angle[i] - sum > 1.0) { //ambiguous case (determined using angle sum)
						if (!ambiguous) { //first time here, try to solve other places first
							ambiguous = angle[i];
							cout << "Skipping ambiguious case angle[" << i << "]: " << angle[i] << " or " << 180 - angle[i] << endl;
							angle[i] = 0;
						} else if (STATE(RESOLVED)) { //we've resolved this ambiguity, apply the user's choice
							cout << "Confirmed ambiguous triangle. Choosing " << (_ambiguous.choice ? "obtuse" : "acute") << " solution\n";
							if (_ambiguous.choice)
								angle[i] = 180 - angle[i];
							cout << "angle[" << i << "]: " << angle[i] << endl;
							angleCount++;
						} else { //we've determined the triangle is ambiguous, choose acute (larger) version for now
							cout << "Confirmed ambiguious triangle\n";
							angleCount++;
							_state = State::AMBIGUOUS;

							_ambiguous.angleIndex = i;
							//calculate alternate dimensions
							for (int j = 0; j < 3; j++) {
								if (side[j] == 0) { //this side is ambiguous
									_ambiguous.sideIndex = j;
									double oppositeAngle = angle[i] - sum;
									cout << "Second alternate angle: " << oppositeAngle << endl;
									_ambiguous.side = sin(oppositeAngle*TO_RADS)*sineLaw;
									cout << "Alternate side[" << j << "]: " << _ambiguous.side << endl;
									break;
								}
							}
						}
					} else {
						angleCount++;
						cout << "angle[" << i << "]: " << angle[i] << endl;
					}
				}
			}
		}
	}
	cout << "Solve finished\n";
	_drawSolved(side, angle);
}

bool Triangle::_checkConstraints(double side[3], double angle[3], int &sideCount, int &angleCount) {
	double maxSide = 0;
	for (int i = 0; i < 3; i++) {
		angle[i] = _angles[i].isDriving() ? _angles[i].getValue() : 0;
		if (angle[i]) {
			cout << "angle[" << i << "] given as " << angle[i] << endl;
			if (angle[i] > 180) {
				message("Angle out of range");
				return true;
			}
			angleCount++;
		}
		side[i] = _sides[i].isDriving() ? _sides[i].getValue() : 0;
		if (side[i]) {
			if (side[i] > maxSide) maxSide = side[i];
			cout << "side[" << i << "] given as " << side[i] << endl;
			sideCount++;
		}
	}
	int constraints = angleCount + sideCount;
	cout << constraints << " constraints.\n";
	if (constraints < 3) { //underconstrained, just make up some constraints
		//if just AA, make an S
		if (!sideCount) {
			side[0] = _sides[0].getValue();
			cout << "Constraining side[0]: " << side[0] << endl;
			sideCount++; constraints++;
		}
		if (sideCount == 2 && angleCount == 0) { //if just SS, make an S to avoid invalid triangle
			for (int i = 0; i < 3; i++) {
				if (!side[i]) {
					cout << "Constraining side[" << i << "]: " << maxSide << endl;
					side[i] = maxSide;
					sideCount++; constraints++;
					return false;
				}
			}
		}
		double anglesLeft = 3 - angleCount; //unconstrained angles
		double angleSumLeft = 180;
		for (int i = 0; i < 3; i++) {
			angleSumLeft -= angle[i]; //finds remaining angles to make triangle
		}
		double idealAngle = angleSumLeft / anglesLeft;
		//guaranteed to make a valid (hopefully pretty) triangle
		//splits up remaining interior angle sum equally between unconstrained angles
		for (int i = 0; i < 3 && constraints < 3; i++) {
			if (!angle[i]) {
				angle[i] = idealAngle;
				cout << "Constraining angle[" << i << "]: " << angle[i] << endl;
				angleCount++; constraints++;
			}
		}
	}
	if (angleCount+sideCount > 3) { //overconstrained
		message("Too many set values");
		return true;
	}
	if (angleCount == 3) {
		message("Three angles are unsolvable");
		return true;
	} else if (angleCount == 2 && angle[0] + angle[1] + angle[2] > 179.9999) {
		message("The entered angles are impossible");
		return true;
	}
	return false;
}

bool Triangle::_lawOfCosines(double side[3], double angle[3], int &sideCount, int &angleCount) {
	//law of cosines if it applies
	//a^2 = b^2 + c^2 - 2*b*c*cos(A)
	if (sideCount == 2 && angleCount == 1) { //SSA or SAS
		if (!side[0] && angle[0]) { //SAS: find a using A, b and c
			cout << "Using SAS Law of Cosines\n";
			side[0] = sqrt(side[1]*side[1] + side[2]*side[2] - 2*side[1]*side[2]*cos(angle[0]*TO_RADS));
			cout << "side[0]: " << side[0] << endl;
			sideCount++;
		} else if (!side[1] && angle[1]) { // SAS: find b using a, B and c
			cout << "Using SAS Law of Cosines\n";
			side[1] = sqrt(side[0]*side[0] + side[2]*side[2] - 2*side[0]*side[2]*cos(angle[1]*TO_RADS));
			cout << "side[1]: " << side[1] << endl;
			sideCount++;
		} else if (!side[2] && angle[2]) { //SAS: find c using a, b and C
			cout << "Using SAS Law of Cosines\n";
			side[2] = sqrt(side[1]*side[1] + side[0]*side[0] - 2*side[1]*side[0]*cos(angle[2]*TO_RADS));
			cout << "side[2]: " << side[2] << endl;
			sideCount++;
		} else {
			cout << "SSA. Using Law of Sines\n";
		}
	}
	if (sideCount == 3) { //SSS
		cout << "Using SSS Law of Cosines\n";
		double maxSide = side[0];
		double perim = side[0];
		for (int i = 1; i < 3; i++) {
			perim += side[i];
			if (side[i] > maxSide)
				maxSide = side[i];
		}
		if (perim < 2*maxSide) { //c > a + b
			message("The entered values are impossible");
			return true;
		}
		for (int i = 0; i < 3; i++) {
			if (!angle[i]) {
				angle[i] = TO_DEGS*acos((side[(i+1)%3]*side[(i+1)%3] + side[(i+2)%3]*side[(i+2)%3] - side[i]*side[i]) / (2*side[(i+1)%3]*side[(i+2)%3]));
				angleCount++;
				cout << "angle[" << i << "]: " << angle[i] << endl;
			}
		}
	}
	return false;
}

void Triangle::_drawSolved(double side[3], double angle[3]) {
	//find new positions
	//side a will be flat
	//triangle will be centered in y
	//triangle height is then b*sin(C)
	double triHeight = side[1]*sin(angle[2]*TO_RADS);
	double triLength = side[0]; //length of base leg
	bool leftCornerOffset = false;
	if (angle[1] > 90.0) {
		triLength += -side[2]*cos(angle[1]*TO_RADS); //side c sticks past base, offset to compensate
	} else if (angle[2] > 90.0) {
		triLength += -side[1]*cos(angle[2]*TO_RADS); //side b sticks past base
		leftCornerOffset = true; //left corner offset by b, use right instead
	}

	double xFactor = triLength / static_cast<double>(WINDOW_WIDTH);
	//if ambiguous, leave top 10% of window for message
	double yFactor = triHeight / static_cast<double>(WINDOW_HEIGHT) / (STATE(AMBIGUOUS) ? 0.9 : 1.0);
	double scale = 0.8 / max(xFactor, yFactor); //fill 80% of screen

	for (int i = 0; i < 3; i++) {
		//set all driven (solved for) dimensions
		_sides[i].setValue(side[i]);
		_angles[i].setValue(angle[i]);
		side[i] *= scale; //scale all our solved values
	}
	triHeight *= scale; //update length and height with scale
	triLength *= scale;
	if (STATE(AMBIGUOUS)) {
		_ambiguous.side *= scale;
	}

	double leftCornerX = (WINDOW_WIDTH - triLength)/2.0; //center bottom in x
	if (leftCornerOffset) { //left corner inset in
		leftCornerX += triLength - side[0]; //right corner - length = left corner
	}
	double bottomY = WINDOW_HEIGHT - ((STATE(AMBIGUOUS) ? 0.9 : 1.0)*WINDOW_HEIGHT-triHeight)/2.0; //and in y

	_drawPos[2] = Vector2f(leftCornerX, bottomY); //bottom left corner
	//A = C + components of b = C + <b*cos(C), -height>
	_drawPos[0] = _drawPos[2] + Vector2f(side[1]*cos(angle[2]*TO_RADS), -triHeight);
	//B directly right from C
	_drawPos[1] = _drawPos[2] + Vector2f(side[0], 0);

	if (STATE(AMBIGUOUS)) { //calculate alternate side position
		//index of non-ambiguous angle adjacent ambiguous side
		size_t thirdSide = 3 - _ambiguous.sideIndex - _ambiguous.angleIndex;
		//unit vector of ambiguous side
		Vector2f ambigSideDir = (_drawPos[_ambiguous.angleIndex] - _drawPos[thirdSide]) / side[_ambiguous.sideIndex];
		//project alternate side onto vector
		_ambiguous.pos = _drawPos[thirdSide] + _ambiguous.side * ambigSideDir;
	}

	_drawAng[0] = 180 + angle[2]; //180 to parallel + corresponding angle of C
	_drawAng[1] = 180 - angle[1]; //supplement of B
	_drawAng[2] = 0.0;

	double insideGap = side[0]/2.0; //half sized similar triangle
	double leftGap = (_drawPos[2].x + _drawPos[0].x)/2.0; //average of A and C
	double rightGap = WINDOW_WIDTH - (_drawPos[0].x + _drawPos[1].x)/2.0; //average of pos B and A
	if (insideGap > leftGap || insideGap > rightGap) { //use inside of triangle, it's better
		if (leftGap < rightGap) {
			_sides[1].setOffsetDir(Dimension::OffsetRight); //left side take inside
			_sides[2].setOffsetDir(Dimension::OffsetRight);
		} else {
			_sides[1].setOffsetDir(Dimension::OffsetLeft);
			_sides[2].setOffsetDir(Dimension::OffsetLeft); //right side take inside
		}
	} else {
		_sides[1].setOffsetDir(Dimension::OffsetLeft);
		_sides[2].setOffsetDir(Dimension::OffsetRight);
	}
	for (int i = 0; i < 3; i++) {
		_angles[i].setPos(_drawPos[i]);
		_sides[i].setPos((_drawPos[(i+1)%3] + _drawPos[(i+2)%3])/2.0); //draw side dims at midpoints
	}
}

void Triangle::sendChar(char c) {
	if (STATE(MESSAGE)) {
		_state = State::DEFAULT;
		_messageBox.displayed = false;
	} else if (!STATE(AMBIGUOUS)) {
		bool selected = false;
		for (int i = 0; i < 3; i++) {
			if (_sides[i].selected()) {
				_sides[i].sendChar(c);
				selected = true;
				break;
			}
			if (_angles[i].selected()) {
				_angles[i].sendChar(c);
				selected = true;
				break;
			}
		}
		if (c == '\n' && selected) { //values have changed, recalculate
			_state = State::DEFAULT;
			update();
		}
	}
}

void Triangle::clickAt(const Vector2f &pos) { //select one dimension and deselect all others
	if (STATE(MESSAGE)) {
		if (_messageBox.contains(pos)) {
			_state = State::DEFAULT;
			_messageBox.displayed = false;
		}
	} else if (!STATE(AMBIGUOUS)) {
		for (int i = 0; i < 3; i++) {
			_angles[i].cancelSelection();
			_sides[i].cancelSelection();
		}
		for (int i = 0; i < 3; i++) {
			if (_sides[i].contains(pos)) {
				_sides[i].select();
				break;
			}
			if (_angles[i].contains(pos)) {
				_angles[i].select();
				break;
			}
		}
	}
}

void Triangle::_drawArc(SDL_Surface *window, const Vector2f &pos, double radius, double start, double angle, uint32_t color) const {
	arcColor(window, pos.x, pos.y, radius, -start - angle, -start, color);
}

void Triangle::drawOn(SDL_Surface *window) const { //fast draw loop
	if (STATE(AMBIGUOUS)) {
		size_t thirdPoint = 3 - _ambiguous.sideIndex - _ambiguous.angleIndex;
		//draw acute version that doesn't overlap
		aalineColor(window,
					_drawPos[_ambiguous.angleIndex].x, _drawPos[_ambiguous.angleIndex].y,
					_drawPos[_ambiguous.sideIndex].x, _drawPos[_ambiguous.sideIndex].y, SHAPE_COLOR);
		aalineColor(window,
					_drawPos[_ambiguous.angleIndex].x, _drawPos[_ambiguous.angleIndex].y,
					_ambiguous.pos.x, _ambiguous.pos.y, SHAPE_COLOR);
		//draw obtuse alternative
		aatrigonColor(window, _ambiguous.pos.x, _ambiguous.pos.y,
		              _drawPos[thirdPoint].x, _drawPos[thirdPoint].y,
		              _drawPos[_ambiguous.sideIndex].x, _drawPos[_ambiguous.sideIndex].y, AMBIG_COLOR);
		//second try to bypass horizontal line bug
		aatrigonColor(window, _drawPos[_ambiguous.sideIndex].x, _drawPos[_ambiguous.sideIndex].y,
		              _drawPos[thirdPoint].x, _drawPos[thirdPoint].y,
					  _ambiguous.pos.x, _ambiguous.pos.y, AMBIG_COLOR);

		Vector2u textPt = Vector2u((WINDOW_WIDTH - 37*FONT_WIDTH)/2, 8);
		drawString(window, textPt, "Use acute (a) or            solution?", SHAPE_COLOR);
		drawString(window, textPt + Vector2u(17*FONT_WIDTH,0), "obtuse (o)", AMBIG_COLOR);
	} else {
		aatrigonColor(window, _drawPos[2].x, _drawPos[2].y,
		              _drawPos[1].x, _drawPos[1].y,
		              _drawPos[0].x, _drawPos[0].y, SHAPE_COLOR);
	}
	for (int i = 0; i < 3; i++) {
		//angles negated so positive is CCW
		arcColor(window, _drawPos[i].x, _drawPos[i].y, 15,
		         -_drawAng[i] - _angles[i].getValue(), -_drawAng[i], SHAPE_COLOR);
	}
	for (int i = 0; i < 3; i++) {
		_angles[i].drawOn(window);
		_sides[i].drawOn(window);
	}
	if (STATE(MESSAGE))
		_messageBox.drawOn(window);
}

void Triangle::chooseSolution(bool choice) {
	if (STATE(MESSAGE)) {
		_messageBox.displayed = false;
		_state = State::DEFAULT;
	} else if (STATE(AMBIGUOUS)) {
		_ambiguous.choice = choice;
		_state = State::RESOLVED;
		update();
	}
}

void Triangle::clear() { //clears constraints
	if (STATE(MESSAGE))
		_messageBox.displayed = false;
	_state = State::DEFAULT;
	for (int i = 0; i < 3; i++) {
		_sides[i].makeDriven();
		_sides[i].cancelSelection();
		_angles[i].makeDriven();
		_angles[i].cancelSelection();
	}
	update();
}

void Triangle::reset() { //resets all values to default
	if (STATE(MESSAGE))
		_messageBox.displayed = false;
	_state = State::DEFAULT;
	for (int i = 0; i < 3; i++) {
		_sides[i].makeDriven();
		_sides[i].cancelSelection();
		_sides[i].setValue(400);
		_angles[i].makeDriven();
		_angles[i].cancelSelection();
		_angles[i].setValue(60);
	}
	update();
}

void Triangle::cancelSelection() { //resets current selection to driven
	if (STATE(MESSAGE)) {
		_messageBox.displayed = false;
		return;
	}
	_state = State::DEFAULT;
	for (int i = 0; i < 3; i++) {
		if (_sides[i].selected()) {
			_sides[i].cancelSelection();
			_sides[i].makeDriven();
		}
		if (_angles[i].selected()) {
			_angles[i].cancelSelection();
			_angles[i].makeDriven();
		}
	}
	update();
}

void Triangle::message(const string &s) {
	_state = State::MESSAGE;
	_messageBox.display(s);
}
