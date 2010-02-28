#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(true);
	
	font.loadFont("frabk.ttf",6);

	ofSoundStreamSetup(radioDrumListener.nSticks,
					   radioDrumListener.nSignals,
					   this,
					   radioDrumListener.sampleRate,
					   radioDrumListener.bufferSize,
					   radioDrumListener.nBuffers);

	ofxMultiTouch.addListener(this);
	radioDrumListener.setup();
}

//--------------------------------------------------------------
void testApp::update()
{}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	ofSetColor(0x00aa00);

	radioDrumListener.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	//	switch (key){
	//	}	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{}

//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{}

//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{}

//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{}

//--------------------------------------------------------------
void testApp::audioReceived(float* input, int bufferSize, int channels)
{
	radioDrumListener.audioReceived(input, bufferSize, channels);
}

//--------------------------------------------------------------
void testApp::audioRequested(float* input, int bufferSize, int channels)
{
	radioDrumListener.audioRequested(input, bufferSize, channels);
}

