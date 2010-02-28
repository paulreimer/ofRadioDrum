#include "RadioDrumListener.h"
#include "ofMain.h"

#include "dsp.h"

//--------------------------------------------------------------
RadioDrumListener::RadioDrumListener()
{
	sampleRate	= 44100;
	nBuffers	= 4;
	bufferSize	= 256;
	// TODO: Hamming window
	windowSize	= bufferSize;
	nSignals	= 4;
	nSticks		= 2;
	order		= 5;
}

//--------------------------------------------------------------
void
RadioDrumListener::setup()
{
	signal	.resize(nSticks);
	carrier	.resize(nSticks);

	Z	.resize(nSticks);
	dZ	.resize(nSticks);
	X	.resize(nSticks);
	Y	.resize(nSticks);

	Fm	.resize(nSticks);
	num	.resize(nSticks);
	den	.resize(nSticks);

	ini_phase			.resize(nSticks, 0.);
	carr_amp			.resize(nSticks, 0.);
	phase				.resize(nSticks, 0.);
	phaseAdder			.resize(nSticks, 0.);

	for (int i=0; i<nSticks; i++)
	{
		signal[i]	= new float[bufferSize];
		carrier[i]	= new float[bufferSize];
		Z[i]	= new float[bufferSize];
		dZ[i]	= new float[bufferSize];
		X[i]	= new float[bufferSize];
		Y[i]	= new float[bufferSize];	

		num[i]	= new float[order+1];
		den[i]	= new float[order+1];
	}

	for (int i=0; i<nSignals; i++)
		signal[i] = new float[bufferSize];

	temp = new float[bufferSize];

	Fm[0] = 1800;//17000;
	Fm[1] = 2200;//19000;

	// Fc = 17000
	num[0][0]=0.3022;
	num[0][1]=1.5111;
	num[0][2]=3.0223;
	num[0][3]=3.0223;
	num[0][4]=1.5111;
	num[0][5]=0.0913;

	den[0][0]=1.0000;
	den[0][1]=2.6843;
	den[0][2]=3.2037;
	den[0][3]=2.0236;
	den[0][4]=0.6683;
	den[0][5]=0.1495;
	//

	// Fc = 19000
	num[1][0]=0.4916;
	num[1][1]=2.4581;
	num[1][2]=4.9161;
	num[1][3]=4.9161;
	num[1][4]=2.4581;
	num[1][5]=0.4916;

	den[1][0]=1.0000;
	den[1][1]=3.5968;
	den[1][2]=5.3224;
	den[1][3]=4.0232;
	den[1][4]=1.5475;
	den[1][5]=0.2417;
	//
}

//--------------------------------------------------------------
void
RadioDrumListener::drawSignalInRect(float *s, ofRectangle frame)
{
	ofSetColor(0x333333);
	ofRect(frame.x,frame.y,frame.width,frame.height);
	ofSetColor(0xFFFFFF);
	for (int i=0; i<frame.width; i++)
		ofLine(frame.x+i,frame.y+(frame.height/2),
			   frame.x+i,frame.y+(frame.height/2)+(s[i]*frame.height));
}

//--------------------------------------------------------------
void
RadioDrumListener::draw()
{
	float scale = 100.;

	ofTranslate(20., 20., 0.);
	for (int i=0; i<nSticks; i++)
	{
		drawSignalInRect(X[i],	ofRectangle(i*(bufferSize+20),0*(scale+20),bufferSize,scale));
		drawSignalInRect(Y[i],	ofRectangle(i*(bufferSize+20),1*(scale+20),bufferSize,scale));
		drawSignalInRect(Z[i],	ofRectangle(i*(bufferSize+20),2*(scale+20),bufferSize,scale));
		drawSignalInRect(dZ[i],	ofRectangle(i*(bufferSize+20),3*(scale+20),bufferSize,scale));

		drawSignalInRect(carrier[i], ofRectangle((i+1)*(bufferSize+20),scale+20,bufferSize,scale));
	}
	for (int i=0; i<nSignals; i++)
	{
		drawSignalInRect(signal[i],	ofRectangle((i+2)*(bufferSize+20),scale+20,bufferSize,scale));
	}
	ofTranslate(-20., -20., 0.);
}

//--------------------------------------------------------------
void
RadioDrumListener::audioReceived(float* input, int bufferSize, int channels)
{
	int n;

	if (!nSignals || !nSticks || signal.empty())
		return;

	// samples are "interleaved"
	for (int i=0; i<channels; i++)
		for (int j=0; j<bufferSize; j++)
			signal[i][j] = input[j*channels+i];

	for (int i=0; i<nSticks; i++)
	{
		amdemod(bufferSize,signal[Z_SIGNAL],Z[i],temp,
				Fm[i],sampleRate,
				ini_phase[i],carr_amp[i],
				order,num[i],den[i]);

		amdemod(bufferSize,signal[dZ_SIGNAL],dZ[i],temp,
				Fm[i],sampleRate,
				ini_phase[i],carr_amp[i],
				order,num[i],den[i]);

		amdemod(bufferSize,signal[XZ_SIGNAL],X[i],temp,
				Fm[i],sampleRate,
				ini_phase[i],carr_amp[i],
				order,num[i],den[i]);

		amdemod(bufferSize,signal[YZ_SIGNAL],Y[i],temp,
				Fm[i],sampleRate,
				ini_phase[i],carr_amp[i],
				order,num[i],den[i]);

		for (int j=0; j<bufferSize; j++)
		{
			X[i][j] = X[i][j] / Z[i][j];
			Y[i][j] = Y[i][j] / Z[i][j];
		}
	}
}

//--------------------------------------------------------------
void
RadioDrumListener::audioRequested(float * output, int bufferSize, int channels)
{
	if (nSticks<=0 || carrier.empty())
		return;

	for (int i=0; i<nSticks; i++)
	{
		// sin (n) seems to have trouble when n is very large, so we
		// keep phase in the range of 0-TWO_PI like this:
		while (phase[i] > TWO_PI)
			phase[i] -= TWO_PI;

		float phaseAdderTarget = (Fm[i] / (float)sampleRate) * TWO_PI;
		phaseAdder[i] = 0.95f * phaseAdder[i] + 0.05f * phaseAdderTarget;

		for (int j=0; j<bufferSize; j++)
		{
			phase[i] += phaseAdder[i];
			carrier[i][j] = output[j*channels+i] = sin(phase[i]);
		}
	}
}
