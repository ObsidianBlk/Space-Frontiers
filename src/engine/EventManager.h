#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

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


#include <vector>
#include <string>
#include <map>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>

//#include "EventDict.h"

namespace engine{

/** \typedef
* \brief A std::map type.
*
* Used to pass event specific arguments of any type to event handlers without knowing the argument types.
* It is assumed event handlers know the argument types they expect and the callers know what the handlers expect.
*/
typedef std::map<const std::string, boost::any> EventDict;

// I prequality the name so as to create the typedef.
class EventManager;
/** \typedef
* \brief boost::shared_ptr<EventManager>
*/
typedef boost::shared_ptr<EventManager> EventManagerPtr;

/** \class
* \brief [SINGLETON] Primary event manager.
*
* This class handles event handle function/method subscriptions, queing of event calls, and flushing of the queued events
* to be handled by all subscribed handlers.
* This class knows nothing of what events exist throughout the software, or how they are handled. It simply stores and
* passes those events along.
*
* \author Bryan Miller
* \version 1.0.0
* \date January, 2014
*/
class EventManager{
    public:
        // Callback Signatures
        typedef void EventNotificationFuncSignature();
        typedef boost::function<EventNotificationFuncSignature> EventNotificationFunction;
        /**
        * Adds an EventDict object to the event queue by the given event name.
        * The EventDict object will not be stored, however, if there are no handlers subscribed to the given event name.
        * This method is thread-safe.
        *
        * \warning It is up to the caller to affirm the EventDict object has the parameters for the event being queued for!
        *
        * @param eventName - [const] The string name of the event being queued for.
        * @param eventDict - [const] An EventDict object containing all of the parameters expected by the event being queued for.
        */
        void QueueEvent(const std::string eventName, const EventDict &eventDict);

        /**
        * Flushes the event queue by passing all stored EventDict objects to their requested event handlers.
        * This method is thread-safe.
        */
        void FlushQueue();


        /** \typedef A void(const EventDict) function signature */
        typedef void SignalSignature(const EventDict);
        /** \typedef boost::function<SignalSignature> */
        typedef boost::function<SignalSignature> HandlerFunction;
        /**
        * Subscribes a function/method handler reference to a given event name.
        *
        * @param eventName - [const] A string name of an event to attach to.
        * @param fn - [const] A reference to a void(const EventDict) function/method to handle the event.
        */
        boost::signals2::connection Subscribe(const std::string &eventName, const HandlerFunction &fn);

        /**
        * Returns the instance of the EventManager class.
        *
        * @return Instance of EventManager
        */
        static EventManagerPtr getInstance();

    private:
        static EventManagerPtr mInstance;

        typedef std::pair<std::string, EventNotificationFunction> NamedNotification;
        typedef std::vector<NamedNotification> NotificationVector;
        NotificationVector mNotificationQueue;

        // Mutex used to ensure one-at-a-time access if needed.
        boost::recursive_mutex mManagerProtection;

        typedef boost::signals2::signal<SignalSignature> EventSignal;
        typedef boost::shared_ptr<EventSignal> EventSignalPtr;
        typedef std::map<std::string, EventSignalPtr> EventSignalMap;
        EventSignalMap mEventSignalMap;

        // Constructor.
        EventManager();
};


} // End namespace "engine"
#endif // EVENTMANAGER_H
