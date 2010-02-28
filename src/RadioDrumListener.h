#pragma once
#include "ofTypes.h"

class RadioDrumListener
{
public:
	enum signals
	{
		Z_SIGNAL	= 0,
		dZ_SIGNAL	= 1,
		XZ_SIGNAL	= 2,
		YZ_SIGNAL	= 3,
	};	

	RadioDrumListener();

	void setup();
	void draw();
	void drawSignalInRect(float *s, ofRectangle frame);

	void audioReceived(float* input, int bufferSize, int channels);
	void audioRequested(float* input, int bufferSize, int channels);

	int sampleRate;
	int bufferSize;
	int windowSize;
	int nBuffers;
	int nSignals;
	int nSticks;

	int	order;

	vector<float*>	signal;
	vector<float*>	carrier;
	
	vector<int>		Fm;
	vector<float>	phase;
	vector<float>	phaseAdder;
	vector<float>	ini_phase;
	vector<float>	carr_amp;

	vector<float*>	num;
	vector<float*>	den;
	vector<float*>	Z;
	vector<float*>	dZ;
	vector<float*>	X;
	vector<float*>	Y;
	
	//	std::map<int, ofPoint> touches;
	
private:
	float *temp;
};
