#ifndef __QUANG_DEMO_H
#define __QUANG_DEMO_H

#include <ugw_proto.h>
#include <uglogging.h>
#include "sl_hook.h"
#include "Quang_Demo_control.h"

/*  This macro defines the list of objects sample sl depends on  */
OBJS_DEPENDS = {
	"Device.PTM.",
	"Device.PPP.",
	"Device.IP.",
	"Device.ATM."
};

/*  This macro defines the list of notification events that this Demo sl wants to get. 
	Whenever any of below events occurs, sl interface function is called with the 
	event id and list of affected parameters */
OBJS_NOTIFY = {
	NOTIFY_DEFAULT_LAN_UP,
	NOTIFY_DEFAULT_LAN_DOWN,
	NOTIFY_WAN_UP,
	NOTIFY_WAN_DOWN,
	NOTIFY_DSL_DATARATE_STATUS,
	NOTIFY_INTERFACE_UP,
	NOTIFY_INTERFACE_DOWN
};

/* 	This macro defines the name of the sl and preferred order in init sequence. 
	servd uses this name to frame interface function for the sl (sl_Quang_Demo_handler), 
	this function will be called with appropricate operation flags for any communication 
	from servd to sl. */
SL_IDENTIFY = {
	.name = "sl_Quang_Demo",
	.start = 20,
	OBJS_FILL
};

/* This is the prototype of servd interface function to demo sl */
void sl_Quang_Demo_handler ( SERVDEXPORTS, INOUT ServdData *pxSlData);
static void sl_Quang_Demo_init (ServdData *pxSlData);
static void sl_Quang_Demo_uninit (ServdData *pxSlData);
static void sl_Quang_Demo_modify (ServdData *pxSlData);
static void sl_Quang_Demo_notify (ServdData *pxSlData);
static void sl_Quang_Demo_get (ServdData *pxSlData);

#endif //#ifndef __QUANG_DEMO_H
