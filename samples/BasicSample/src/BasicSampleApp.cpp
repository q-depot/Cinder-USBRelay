#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "USBRelay.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicSampleApp : public AppNative {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void draw();
    
    USBRelayRef     mUSBRelay;
};

void BasicSampleApp::setup()
{
    mUSBRelay = USBRelay::create( "tty.usbmodem00015481", 2 );
}

void BasicSampleApp::keyDown( KeyEvent event )
{
    int code = event.getCode() - 49;
    
    if ( code < 0 || code > 8 )
        return;
    
    bool valueOn = !event.isAltDown();

    mUSBRelay->setOutput( code, valueOn );
    
    console() << "relay " << code << " " << ( valueOn ? "on" : "off" ) << endl;
}


void BasicSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();
    
    gl::drawString( "KEYS 1 to 8 to turn the relay ON", Vec2i( 25, 35 ) );
    gl::drawString( "Hold ALT + KEY to turn it OFF", Vec2i( 25, 55 ) );
}


CINDER_APP_NATIVE( BasicSampleApp, RendererGl )
