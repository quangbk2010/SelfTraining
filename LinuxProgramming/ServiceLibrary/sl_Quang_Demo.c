#include "sl_Quang_Demo.h"

/* 	This is the prototype of servd interface function to sample sl.
	This function will be called with appropriate operation flags
	for any communication from servd to sl. Sl can invoke suitable
	internal API based on operation flag. */
void sl_Quang_Demo_handler (SERVDEXPORTS, INTOUT ServdData *pxSlData)
{
	LOCAL_EXPORT_SERVDEXPORTS;
	struct timeval startTv, endTv;
	
	memset (&startTv, 0, sizeof (struct timeval));
	memset (&endTv, 0, sizeof (struct timeval));
	
	gettimeofday (&startTv, NULL);
	
	/* Switch to init operation */
	if (IS_MOPT_INIT (pxSlData -> unMainOper)) {
		sl_Quang_Demo_init (pxSlData);
	}
	
	/* switch to Modify operation */
	else if (IS_MOPT_MODIFY (pxSlData -> unMainOper)) {
		sl_Quang_Demo_modify (pxSlData);
	}
	
	/* Switch to Notify operation */
	else if (IS_MOPT_NOTIFY (	pxSlData -> unMainOper) {
		sl_Quang_Demo_notify (pxSlData);
	}
	
	/* switch to Modify operation */
	else if (IS_MOPT_GET (pxSlData -> unMainOper)) {
		sl_Quang_Demo_get (pxSlData);
	}
	
	/* Switch to Notify operation */
	else if (IS_MOPT_UNINIT (	pxSlData -> unMainOper) {
		sl_Quang_Demo_uninit (pxSlData);
	}
	
	/**/
	else {
		pxSlData -> nStatus = SL_FUNC_NOT_SUPPORTED;
		printf ("Operation is not implemented by this Demo\n");
	}
	
	/* Get time when requested operation is done */
	gettimeofday (&endTv, NULL);
	printf ("Time taken by Quang Demo SL for [%d] operation is: %u s and %u ms\n", pxSlData -> unMainOper, (int32_t) (endTv.tv_sec - startTv.tv_sec), (int32_t) (endTv.tv_sec - startTv.tv_sec) );
}

/* Internal API to initialize required configuration to service subsequent from servd */
static void sl_Quang_Demo_init (ServdData *pxSlData)
{
	printf ("sl_Quang_Demo_init\n");
	pxSlData -> nStatus = UGW_SUCCESS;
	return;
}

/* Internal API to un-initialize configuration. All resources taken by SL during init or other requests can be freed here */
static void sl_Quang_Demo_uninit (ServdData *pxSlData) 
{
	printf ("sl_Quang_Demo_uninit\n");
	pxSlData -> nStatus = UGW_SUCCESS;
	return;
}

/* 	Internal API to service modify request that includes modification/addition/deletion of objects 
	managed by Sample SL. pxSlData contains list of one or more objects of same/different type 
	supported by Sample SL and each object entry contains list	of the affected parameters. */ 
static void sl_Quang_Demo_modify (ServdData *pxSlData)
{
	ObjList *pxObjEntry = NULL;
	PramList *pxParamEntry = NULL;
	
	printf ("sl_Quang_Demo_modify\n");
	
	/* Loop macro to go through the list of objects passed in this request */
	FOR_EACH_OBJ (pxSlData -> pxObjList, pxObjEntry) {
		/* Loop macro to go through the list of parameters for pxObjEntry object */
		FOR_EACH_PARAM (pxObjEntry, pxParamEntry) {
			/* Print names and values for understanding */
			printf ("ParamName: %s, ParamVal: %s\n", pxParamEntry -> sParamName, pxParamEntry -> sParamvalue);
		}
		
		/* 	For add object operation, object is added to database with default values then
			request is sent to SL. Therefore, operation has to be changed to modify in order to 
			update database with requested values*/
		if (IS_OBJOPT_ADD (pxObjEntry -> unObjOper) {
			pxObjEntry -> unObjOper = OBJOPT_MODIFY;
		}
	}
	
	/* 	Return UGW_SUCCESS if operation is successfully and data has to be updated in database.
		UGW_FAILURE to be returned if operation is NOT successfully*/
	
	pxSlData -> nStatus = UGW_SUCCESS;
	
	return;
}

/* 	Internal API to handle various notification events from servd. servd sends notification 
	for the events SL has registered with it by filling OBJS_NOTIFY object */
static void sl_Quang_Demo_Notify (ServdData *pxSlData) 
{
	ParamList *pxParamEntry = NULL;
	ParamList *pxNotifyParamList = NULL;
	
	printf ("sl_Quang_Demo_Notify\n");
	pxNotifyParamList = pxSlData -> xNotify.pxParamList;
}








