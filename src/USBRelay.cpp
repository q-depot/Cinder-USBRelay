/*
 *  USBRelay.cpp
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd. Copyright 2014 . All rights reserved.
 *
 *  www.nocte.co.uk
 *
 */


#include "USBRelay.h"

using namespace ci;
using namespace ci::app;
using namespace std;


USBRelay::USBRelay( const string &serialDevicePath, size_t numOuputs ) : mSerialDevicePath(serialDevicePath), mSerial(NULL)
{
    numOuputs = std::min( numOuputs, (size_t)USB_RELAY_MAX_OUTPUTS );
    
    for( size_t k=0; k < numOuputs; k++ )
    {
        RelayOutput ro = { false, false };      // value OFF, no need for update, initSerial already set all the outputs off
        mOutputs.push_back( ro );
    }
    
    USBRelay::listDevices();
    
	initSerial();
}


USBRelay::~USBRelay()
{
    // set all output OFF
    setAllOff();
    
    // stop the thread
    mRunSendDataThread = false;
    if ( mSendDataThread.joinable() )
        mSendDataThread.join();
    
    // close the serial port
    if ( mSerial )
    {
        mSerial->flush();
        delete mSerial;
        mSerial = NULL;
    }
    
    console() << "shutdown USBRelay" << endl;
}


void USBRelay::initSerial()
{
	if ( mSerial )
	{
		mSerial->flush();
		delete mSerial;
		mSerial = NULL;
	}
	
	try 
    {
		Serial::Device dev = findDeviceByPathContains(mSerialDevicePath);
		mSerial = new Serial( dev, USB_RELAY_BAUD_RATE );
        
        console() << "USBRelay > Connected to usb USB interface: " << dev.getName() << endl;
	}
	catch( ... ) 
    {
        console() << "USBRelay > There was an error initializing the USB device" << endl;
		mSerial = NULL;
	}
    
    setAllOff();
    
    mSendDataThread = std::thread( &USBRelay::sendData, this );
}


void USBRelay::setOutput( size_t outputN, bool valueOn )
{
    if ( outputN >= mOutputs.size() )
        return;

    std::unique_lock<std::mutex> dataLock( mDataMutex );    // get data UNIQUE lock
    
    mOutputs[outputN].value     = valueOn;                  // set the value
    mOutputs[outputN].doUpdate  = true;                     // always update even if the value doesn't change
    
    dataLock.unlock();                                      // unlock
}


bool USBRelay::getOutput( size_t outputN )
{
    if ( !mSerial || outputN >= mOutputs.size() )
        return false;
    
    return mOutputs[outputN].value;
}


void USBRelay::setAllOff()
{
    mSerial->writeByte( 110 );
}


void USBRelay::sendData()
{
    mRunSendDataThread = true;
    
	while( mSerial && mRunSendDataThread )
    {
		std::unique_lock<std::mutex> dataLock( mDataMutex );                            // get data UNIQUE lock
		
        for( size_t k=0; k < mOutputs.size(); k++ )
        {
            if ( mOutputs[k].doUpdate )
            {
                if ( mOutputs[k].value )
                    mSerial->writeByte( 101 + k );
                else
                    mSerial->writeByte( 111 + k );
                
                mOutputs[k].doUpdate = false;
            }
        }

		dataLock.unlock();                                                              // unlock data
        
        std::this_thread::sleep_for( std::chrono::milliseconds( USB_RELAY_THREAD_SLEEP_MS ) );
	}
    
    console() << "USBRelay > sendData() thread exited!" << endl;
}

