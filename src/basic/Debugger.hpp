#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#ifndef DEBUGGER_H
#define DEBUGGER_H

using namespace std;
static const class Debugger
{
public:
	//static Debugger Debug;
	static const void Log(glm::mat4 &m) {
		cout << "[" << endl;
		for (int i = 0; i < 4; i++) {
			cout << m[i][0] << "," << m[i][1] << "," << m[i][2] << "," << m[i][3] << endl;
		}
		cout << "]" << endl;
	};

	static const void Log(glm::vec3& m) {
		cout << "[" ;
			cout << m[0] << "," << m[1] << "," << m[2] ;
		
		cout << "]" << endl;
	};

private:

};

#endif // !DEBUGGER_H
