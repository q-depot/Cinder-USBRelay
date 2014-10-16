/*
 *  USBRelay.h
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd. Copyright 2014 . All rights reserved.
 *
 *  www.nocte.co.uk
 *
 *  Cinder-Block for the robot-electronics USB Relay
 *
 *  USB Relay documentation: http://www.robot-electronics.co.uk/htm/usb_rly16ltech.htm
 *
 *  Command         Action
 *  -----------     ------
 *  dec     hex
 *  90      5A      Get software version - returns 2 bytes, the first being the Module ID which is 9, followed by the software version
 *  91      5B      Get relay states - sends a single byte back to the controller, bit high meaning the corresponding relay is powered
 *  92      5C      Set relay states - the next single byte will set all relays states, All on = 255 (11111111) All off = 0
 *  93      5D      Get DC input voltage - returns relay supply voltage as byte, 125 being 12.5V DC
 *  100     64      All relays on
 *  101     65      Turn relay 1 on
 *  102     66      Turn relay 2 on
 *  103     67      Turn relay 3 on
 *  104     68      Turn relay 4 on
 *  105     69      Turn relay 5 on
 *  106     6A      Turn relay 6 on
 *  107     6B      Turn relay 7 on
 *  108     6C      Turn relay 8 on
 *  110     6E      All relays off
 *  111     6F      Turn relay 1 off
 *  112     70      Turn relay 2 off
 *  113     71      Turn relay 3 off
 *  114     72      Turn relay 4 off
 *  115     73      Turn relay 5 off
 *  116     74      Turn relay 6 off
 *  117     75      Turn relay 7 off
 *  118     76      Turn relay 8 off
 *
 */

#ifndef USB_RELAY
#define USB_RELAY

#define USB_RELAY_BAUD_RATE             57600       // baudrate, this is not actually used by the device
#define USB_RELAY_MAX_OUTPUTS           8           // maximun number of outputs
#define USB_RELAY_THREAD_SLEEP_MS		16			// thread sleep for in milliseconds

#pragma once

#include "cinder/Serial.h"
#include "cinder/Thread.h"

class USBRelay;
typedef std::shared_ptr<USBRelay> USBRelayRef;

class USBRelay {

public:
  
    static USBRelayRef create( const std::string &serialDevicePath, size_t numOutputs )
    {
        return USBRelayRef( new USBRelay( serialDevicePath, numOutputs ) );
    }
    
	~USBRelay();

	static void listDevices()
    {
		const std::vector<ci::Serial::Device> &devices( ci::Serial::getDevices(true) );
		
        for( std::vector<ci::Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt )
            ci::app::console() << "USBRelay > List serial devices: " + deviceIt->getPath() << std::endl;
	}
    
    static std::vector<std::string> getDevicesList()
    {
        std::vector<std::string> devicesList;

		const std::vector<ci::Serial::Device> &devices( ci::Serial::getDevices(true) );
		
        for( std::vector<ci::Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt )
            devicesList.push_back( deviceIt->getPath() );
        
        return devicesList;
	}
	
    static ci::Serial::Device findDeviceByPathContains( const std::string &searchString )
    {
        const std::vector<ci::Serial::Device> &devices = ci::Serial::getDevices();
        for( std::vector<ci::Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
            if( deviceIt->getPath().find( searchString ) != std::string::npos )
                return *deviceIt;
        }
        return ci::Serial::Device();
    }
    
	bool        isConnected() { return mSerial != NULL; }
	
    std::string getDeviceName() { return mSerialDevicePath; }
	
    void        setOutput( size_t outputN, bool value );
    bool        getOutput( size_t outputN );
    
    size_t      getNumOutputs() { return mOutputs.size(); }
    
    void        setAllOff();
    
private:
    
    USBRelay( const std::string &serialDevicePath, size_t numOutputs );

	void initSerial();

    void sendData();
    
private:
    
    struct RelayOutput {
        bool value;
        bool doUpdate;
    };
    
	ci::Serial                  *mSerial;				// serial interface
	std::string                 mSerialDevicePath;		// usb serial device path
    std::vector<RelayOutput>    mOutputs;
    
    bool                        mRunSendDataThread;
	std::mutex                  mDataMutex;             // mutex unique lock
    std::thread                 mSendDataThread;
    
};


#endif