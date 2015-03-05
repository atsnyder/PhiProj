#pragma offload_attribute (push,target(mic))
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <iostream>
#pragma offload_attribute (pop)
#include <fstream>
#include "cilktime.h"
#include <vector>

using namespace std;
using namespace tbb;

__declspec(target(mic)) struct testStruct{
	float* numArr;
} tS;

int main(){

	//File Input Begin
	vector <float> numVect(0);	
	fstream reader("../input/testString.txt", ios_base::in);

	if(!reader){
		cout << "Error opening file" << endl;
		return -1;
	} else {
		cout << "File Opened" << endl;
		float temp;
		while (reader >> temp){
			numVect.push_back(temp);
		}
		cout << "File Copied to Vector" << endl;
		reader.close();
	}
	//File Input End

	int size = numVect.size(); 
	tS.numArr = &numVect[0];
	cout << "Vector reassigned to array" << endl;
	cout << "Beginning calculation" << endl;
	unsigned long long start_tick = cilk_getticks();

#pragma offload target(mic) in(size) inout(tS.numArr: length(size))
	parallel_for(0,size,1, [&](int i){
				tS.numArr[i] = tS.numArr[i]*7;
			});

	unsigned long long end_tick = cilk_getticks();
	long total_time = (long) (end_tick-start_tick)/1000.f;
	cout << total_time << " milliseconds" << endl;	

	cout << "Calculation Completed!" << endl;
	cout << "Outputing to File" << endl;

	//File Output Begin
	ofstream writer("../output/testStringOut.txt");
	if(!writer){
		cout << "Error opening file" << endl;
		return -1;
	} else {
		for(int i=0; i<size; i++){
			writer << tS.numArr[i] << endl;
		}
	}
	writer.close();
	cout << "Output Complete!" << endl;
	//File Output End
}	
