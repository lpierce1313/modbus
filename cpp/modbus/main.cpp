#include <iostream>
#include <string>
#include <vector>
#include "mbrtu.h"
using namespace std;

int main() {


	//Set Arguments

		//Set Variables
		string comPort, parity, speed, code, unitAdd, startReg, numReg;
		string arr[7];
		int argc1 = 12;
		string p_arr[7] = { "Comport: ", "Parity: ", "Speed: ", "Function Code: ", "Unit Address: ", "Start Register: ",  "Number Register: " };
		vector<int> result;

		//Ask for Inputs
		for (int i = 0; i < 7; i++) {
			cout << p_arr[i] << endl;
			cin >> arr[i];
		}

	//Call function

		//Call subfunction
		send_arr(arr, argc1, result);
		std::cout << std::endl;
		for (int i = 0; i < result.size(); i++) {
			std::cout << result[i] << std::endl;
		}

	//End Program

		//Call pause
		system("pause");
		return 0;
}

// V8  Javascript world
