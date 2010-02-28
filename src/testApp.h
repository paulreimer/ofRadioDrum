#pragma once

#include "ofMain.h"
#include "RadioDrumListener.h"
#include "ofxMultiTouch.h"

class testApp
: public ofBaseApp
, public ofxMultiTouchListener
{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);

	void audioReceived(float* input, int bufferSize, int channels);
	void audioRequested(float* input, int bufferSize, int channels);

	RadioDrumListener radioDrumListener;

	ofTrueTypeFont font;
};
