
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/date.h"
#include "world/scheduler/eventscheduler.h"
#include "world/scheduler/warningevent.h"

#include "mmgr.h"

//#define VERBOSESCHEDULER


EventScheduler::EventScheduler ()
{
}

EventScheduler::~EventScheduler ()
{

    DeleteLListData ( (LList <UplinkObject *> *) &events );

}

void EventScheduler::ScheduleEvent ( UplinkEvent *event )
{

	UplinkAssert (event);


	// Insert this event into the list of events
	
	bool inserted = false;

	for ( int i = 0; i < events.size (); ++i ) {

		UplinkEvent *ue = events.at (i);
		UplinkAssert ( ue );

		if ( event->rundate.Before ( &(ue->rundate) ) ) {

			events.insert( i, event);
			inserted = true;
			break;

		}
	
	}

	if ( !inserted ) {

		// Put it at the back of the queue
		events.push_back ( event );

	}

}

void EventScheduler::ScheduleWarning ( UplinkEvent *event, Date *date )
{

	UplinkAssert (event);
	UplinkAssert (date);

	WarningEvent *wevent = new WarningEvent ();
	wevent->SetEvent ( event );
	wevent->SetRunDate ( date );

	ScheduleEvent ( wevent );

}

Date *EventScheduler::GetDateOfNextEvent ()
{

	UplinkEvent *event = events.at (0);
	
	if ( event ) 
		return &(event->rundate);

	else
		return NULL;
	
}


bool EventScheduler::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !LoadLList ( (LList <UplinkObject *> *) &events, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void EventScheduler::Save  ( FILE *file )
{

	SaveID ( file );

	SaveLList ( (LList <UplinkObject *> *) &events, file );

	SaveID_END ( file );

}

void EventScheduler::Print ()
{

	printf ( "==== Event Scheduler : ===============================\n" );
	PrintLList ( (LList <UplinkObject *> *) &events );

}

void EventScheduler::Update ()
{

	// Run every event that should have been run so far

	LList <UplinkEvent *> eventsToRun;

	if ( events.size () > 0 ) {

		UplinkEvent *event = events.at (0);

		while ( event && game->GetWorld ()->date.After ( &(event->rundate) ) ) {

			eventsToRun.push_back( event );

			events.erase (0);

			if ( events.ValidIndex ( 0 ) )
				event = events.at (0);
			else
				event = NULL;

		}

	}


	if ( eventsToRun.size () > 0 ) {

		UplinkEvent *event = eventsToRun.at (0);

		while ( event ) {

#ifdef VERBOSESCHEDULER
			printf ( "Running EVENT : %s\n", event->GetLongString () );
#endif

			event->Run ();
			
			eventsToRun.erase (0);

			delete event;
			if ( eventsToRun.ValidIndex ( 0 ) )
				event = eventsToRun.at (0);
			else
				event = NULL;

		}

	}

}

char *EventScheduler::GetID ()
{

	return "SCHED";

}

