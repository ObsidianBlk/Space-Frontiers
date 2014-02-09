/*
* The MIT License (MIT)
*
* Copyright (c) 2014 Bryan Miller
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "EventManager.h"

namespace engine{


EventManagerPtr EventManager::mInstance;

EventManager::EventManager(){}

EventManagerPtr EventManager::getInstance(){
    if (mInstance.get() == 0){
        mInstance = EventManagerPtr(new EventManager());
    }
    return mInstance;
}


void EventManager::QueueEvent(const std::string eventName, const EventDict &eventDict){
    // First, check if a signal exists for this event.
    EventSignalMap::iterator iterFind = mEventSignalMap.find(eventName);
    if (iterFind != mEventSignalMap.end()){
        EventSignal &sig = *iterFind->second;

        // One thread at a time!
        {
            boost::recursive_mutex::scoped_lock lock(mManagerProtection);
            mNotificationQueue.push_back(NamedNotification(eventName, boost::bind(boost::ref(sig), eventDict)));
        }
    }
}


void EventManager::FlushQueue(){
    // Will hold a copy of all existing notifications within the main vector.
    NotificationVector vNotifications;

    // Open a protected scope to modify the notification list.
    {
        // Lock for only one thread at a time.
        boost::recursive_mutex::scoped_lock lock(mManagerProtection);
        // Move the main notification vector to the local vector. This will effectively clear the main notification
        // vector.
        std::swap(vNotifications, mNotificationQueue);
    }
    // Out of the locked scope, and therefore the mNotificationQueue can continue storing new events, even if we're
    // still processing this batch.

    BOOST_FOREACH(const NamedNotification &i, vNotifications){
    //for (NotificationVector::iterator i = vNotifications.begin(); i != vNotifications.end(); i++){
        // Debugging output
        std::cout << "Flushing " << i.first << std::endl;

        try{
            i.second();
        } catch (const boost::bad_any_cast &) {
            std::cout << "*** Invalid any_cast ***" << std::endl;
        }
    }
    // vNotifications will now go out of scope and therefore clear all of the old queued events.
}


boost::signals2::connection EventManager::Subscribe(const std::string &eventName, const HandlerFunction &fn){
    if (mEventSignalMap.find(eventName) == mEventSignalMap.end()){
        // Create signal since it doesn't yet exist.
        mEventSignalMap[eventName].reset(new EventSignal);
    }
    return mEventSignalMap[eventName]->connect(fn);
}



} // End namespace "engine"




