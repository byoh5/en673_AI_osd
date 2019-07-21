#include "soapH.h"  
#include "soapStub.h"
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "enx_gsoap_http.h"
#include "FreeRTOS.h" 
#include <stdio.h>
#include "onvifProfile.h"
#include "notify.nsmap"
#include "msg.h"
#include "lwip/sys.h"	// sys_thread_new
#include "timer_task.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#ifdef USE_EVENT_INTERFACE	
int canSubscribe_pullmessages = 1;
char eventstatus[3] = "";
time_t t_out; /*global variable for termination time*/
time_t time_out; /*global variable for termination time*/
char MOTION_DETECTED_MASK = 1;
int canSubscribe = 1;
char notify_endpoint[100];

void notify_thr(void *arg);
void eventtracker_thr(void *arg);
static BYTE topic_type = 0;
static BYTE pull_msg_start = 0;

#endif

int 
__tev__PullMessages(struct soap *soap, 
	struct _tev__PullMessages *tev__PullMessages, 
	struct _tev__PullMessagesResponse *tev__PullMessagesResponse)
{
	ENTER();

#ifdef USE_EVENT_INTERFACE
	char StateMsg[SMALL_INFO_LENGTH];

	if(pull_msg_start == 0)
	{
		strcpy(StateMsg, "Initialized");
	}
	else
	{
		strcpy(StateMsg, "Changed");
	}
	soap->header->wsa5__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesResponse");

//	char macaddr[MACH_ADDR_LENGTH];
//	SysInfo *oSysInfo = GetSysInfo();
//	NET_IPV4 ip;
	int limitMsg;
	time_t tnow;
	time_t tter;
	time_t time_out;
	struct tm timeinfo;
	int i;
	SysInfo *oSysInfo = GetSysInfo();
	char msg[LARGE_INFO_LENGTH];	
	char strtime[100];
	
	limitMsg = tev__PullMessages->MessageLimit;
	
	tnow = time(&gptMsgShare.TIME);	
	tter = time(&gptMsgShare.TIME);	
	
	dvprintf("tev__PullMessages->Timeout %lld %x\r\n",tev__PullMessages->Timeout,tev__PullMessages->Timeout);
	//time_out = time(&tev__PullMessages->Timeout);	//periodtol("PT20S");//
	
	if(tev__PullMessages->Timeout == 0)
		tter += 1;
	else
		tter += tev__PullMessages->Timeout;
	
	tev__PullMessagesResponse->CurrentTime = tnow;
	tev__PullMessagesResponse->TerminationTime = tter;
	
	tev__PullMessagesResponse->__sizeNotificationMessage = 1;//limitMsg;
			
	tev__PullMessagesResponse->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *) soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType)* tev__PullMessagesResponse->__sizeNotificationMessage);
	memset(tev__PullMessagesResponse->wsnt__NotificationMessage, 0x00, sizeof(struct wsnt__NotificationMessageHolderType) * tev__PullMessagesResponse->__sizeNotificationMessage);

	enx_get_tmtime(gptMsgShare.TIME, &timeinfo, DEF_YES);

	sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02d", timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	
	dvprintf ( "The current date/time is: %s\r\n", strtime );

	for(i = 0; i<tev__PullMessagesResponse->__sizeNotificationMessage; i++)
	{
	
		tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic = (struct wsnt__TopicExpressionType *) soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		memset(tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic, 0x00, sizeof(struct wsnt__TopicExpressionType));	

		tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
		tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic->__any = soap_strdup(soap, "tns1:VideoSource/MotionAlarm" /*"wsnt:VideoAnalytics/wsnt:MotionDetection/wsnt:Motion"*//*"tns1:VideoSource/MotionAlarm"*/);
		tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic->__anyAttribute = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage[i].wsnt__Topic->__mixed = NULL;


		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x\r\n",eventstatus[0],MOTION_DETECTED_MASK);
			eventstatus[0]=0;
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"%s\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, StateMsg, oSysInfo->VSC[0].Vtoken);
			tev__PullMessagesResponse->wsnt__NotificationMessage[i].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x\r\n",eventstatus[0],MOTION_DETECTED_MASK);
			eventstatus[0]=0;
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"%s\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime, StateMsg, oSysInfo->VSC[0].Vtoken);
			tev__PullMessagesResponse->wsnt__NotificationMessage[i].Message.__any = soap_strdup(soap,msg);
		}
#if 0	
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference = (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->Address = soap_strdup(soap, );

		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->__size = 0;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->__any = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__SubscriptionReference->__anyAttribute = NULL;
		
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference = (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->Address = soap_strdup(soap, );

		
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->__size = 0;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->__any = NULL;
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__ProducerReference->__anyAttribute = NULL;

		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__Topic->__any = soap_strdup(soap, "tnsvendor:VideoAnalytics/MotionDetection/Detected");

		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__Topic->__anyAttribute = NULL; 
		tev__PullMessagesResponse->wsnt__NotificationMessage->wsnt__Topic->__mixed = NULL; 

		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{
			tev__PullMessagesResponse->wsnt__NotificationMessage->Message.__any = soap_strdup(soap,"<tt:Message UtcTime=\"2008-10-10T12:24:57.321\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\"Value=\"1\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"2\"/><tt:SimpleItem Value=\"Motion\" Name=\"MotionDetection\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"window\" Value=\"1\" /></tt:Data></tt:Message>"); 
		}
		else
		{
			tev__PullMessagesResponse->wsnt__NotificationMessage->Message.__any = NULL;
		}
#endif		
	}
#endif
	LEAVE();
	return SOAP_OK;
}

int 
__tev__Seek(struct soap *soap, 
	struct _tev__Seek *tev__Seek, 
	struct _tev__SeekResponse *tev__SeekResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tev__Seek);
	UNUSED(tev__SeekResponse);
}

int 
__tev__SetSynchronizationPoint(struct soap *soap, 
struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, 
struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse)
{
	ENTER();

	soap->header->wsa5__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/SetSynchronizationPointResponse");

	LEAVE();
	return SOAP_OK;

	UNUSED(tev__SetSynchronizationPoint);
	UNUSED(tev__SetSynchronizationPointResponse);
}

int 
__tev__GetServiceCapabilities(struct soap *soap, 
struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, 
struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse)
{
	ENTER();

	tev__GetServiceCapabilitiesResponse->Capabilities = (struct tev__Capabilities *)soap_malloc(soap, sizeof(struct tev__Capabilities));
	memset(tev__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct tev__Capabilities));
	struct tev__Capabilities *Cap = tev__GetServiceCapabilitiesResponse->Capabilities;

	Cap->WSSubscriptionPolicySupport = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	Cap->WSSubscriptionPolicySupport[0] = xsd__boolean__true_;

	Cap->WSPullPointSupport = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	Cap->WSPullPointSupport[0] = xsd__boolean__true_;

	Cap->WSPausableSubscriptionManagerInterfaceSupport = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	Cap->WSPausableSubscriptionManagerInterfaceSupport[0] = xsd__boolean__false_;

	Cap->MaxNotificationProducers = (int *)soap_malloc(soap, sizeof(int));
	Cap->MaxNotificationProducers[0] = 1;

	Cap->MaxPullPoints = (int *)soap_malloc(soap, sizeof(int));
	Cap->MaxPullPoints[0] = 1;

	Cap->PersistentNotificationStorage = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	Cap->PersistentNotificationStorage[0] = xsd__boolean__false_;

	Cap->__size = 0;
	Cap->__any = NULL;
	Cap->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tev__GetServiceCapabilities);
}

int 
__tev__CreatePullPointSubscription(struct soap *soap, 
struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription,
struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse)
{
	ENTER();

#ifdef USE_EVENT_INTERFACE	
#include "time.h"
//	SysInfo *oSysInfo = GetSysInfo();
	int i=0;
//	NET_IPV4 ip;
	char macaddr[MACH_ADDR_LENGTH] = {0};
//	char *temp1;
//	char *temp2;
	char _IPAddr[LARGE_INFO_LENGTH];	
	time_t tnow;
	time_t tter;

	soap->header->wsa5__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse");

//	long t_out = periodtol(tev__CreatePullPointSubscription->InitialTerminationTime); // <- null!!

#if 1
	if (tev__CreatePullPointSubscription->Filter != NULL) {
		if (tev__CreatePullPointSubscription->Filter->__size > 0) {
			if (tev__CreatePullPointSubscription->Filter->__any != NULL) {
				for (i = 0; i < tev__CreatePullPointSubscription->Filter->__size; i++) {
					flprintf("__any[%d] = [%s]\r\n", i, tev__CreatePullPointSubscription->Filter->__any[i]);
					if (tev__CreatePullPointSubscription->Filter->__any[i] != NULL) {
						// anyparser processing
						flprintf("Code add!!\r\n");
//						temp1 = soap_strdup(soap, anyparser(tev__CreatePullPointSubscription->Filter->__any[i]));
//						if (temp1 == NULL) {
//							onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidFilter","","");
//							return SOAP_FAULT;
//						}
					} else {
						flprintf("Code add!!\r\n");
						// __any[i] == NULL error processing
					}
				}
			} else {
				flprintf("size(%d) __any NULL!\r\n", tev__CreatePullPointSubscription->Filter->__size);
				onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidFilter","","");
				LEAVE();
				return SOAP_FAULT;
			}
		}
	}
#else
	if (tev__CreatePullPointSubscription->Filter != NULL) {
		if (tev__CreatePullPointSubscription->Filter->__size == 1) {
			if (tev__CreatePullPointSubscription->Filter->__any[0] != NULL) {
				temp1 = soap_strdup(soap, anyparser(tev__CreatePullPointSubscription->Filter->__any[0]));
				if (temp1 == NULL) {
					onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidFilter","","");
					return SOAP_FAULT;
				}
			}
		} else {
			if (tev__CreatePullPointSubscription->Filter != NULL) {
				if (tev__CreatePullPointSubscription->Filter->__any[1] != NULL) {
					temp2 = soap_strdup(soap, anyparser(tev__CreatePullPointSubscription->Filter->__any[1]));
					if (temp2 == NULL) {
						onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidFilter","","");
						return SOAP_FAULT;
					}
				}
			}
		}
	}
#endif
	
	net_get_hwaddr(ETH_IF_NAME, macaddr);

	tev__CreatePullPointSubscriptionResponse->SubscriptionReference = (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
	memset(tev__CreatePullPointSubscriptionResponse->SubscriptionReference, 0x00, sizeof(struct wsa5__EndpointReferenceType));	
	sprintf(_IPAddr,"http://%s/onvif/Events/PullSubManager", getIpAddress(HTTP_ONVIF_PORT)); 

	tev__CreatePullPointSubscriptionResponse->SubscriptionReference->Address = soap_strdup(soap, _IPAddr);

//	flprintf("TEST : gptMsgShare.TIME(%d)\r\n", gptMsgShare.TIME);
//	flprintf("TEST : %d - %s\r\n", t_out, tev__CreatePullPointSubscription->InitialTerminationTime);

	if (tev__CreatePullPointSubscription->InitialTerminationTime != NULL) {
		t_out = periodtol(tev__CreatePullPointSubscription->InitialTerminationTime); // <- null!!!!!!!!!!!!!!!
	} else {
		t_out = gptMsgShare.TIME;
		t_out +=3600;
	}
//	flprintf("TEST : %d\r\n", t_out);


	tnow = time(&gptMsgShare.TIME);
	tter = time(&gptMsgShare.TIME);
	tter += 3600;

	tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime = tnow;
	tev__CreatePullPointSubscriptionResponse->wsnt__TerminationTime = tter;

	tev__CreatePullPointSubscriptionResponse->__any = NULL;
	tev__CreatePullPointSubscriptionResponse->__size = 0;	
	time_out = t_out;
	if(canSubscribe_pullmessages)
	{
//		flprintf("TEST\r\n");
		sys_thread_new("event",eventtracker_thr, NULL, 32*1024, DISCOVERY_TASK_PRIO);	
	}
//	flprintf("TEST\r\n");
#endif

	LEAVE();
	return SOAP_OK;
}

int 
__tev__GetEventProperties(struct soap *soap, 
struct _tev__GetEventProperties *tev__GetEventProperties, 
struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{
	ENTER();

	printf("%s s_addr %IP:%d soap->omode %x\r\n", __func__, soap->peer.sin_addr.s_addr, soap->peer.sin_port, soap->omode);

#ifdef USE_EVENT_INTERFACE		
	soap->header->wsa5__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesResponse");

	tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation = 1;
	tev__GetEventPropertiesResponse->TopicNamespaceLocation = (char **)soap_malloc(soap, sizeof(char *));

	*tev__GetEventPropertiesResponse->TopicNamespaceLocation = soap_strdup(soap, "http://www.onvif.org/onvif/ver10/topics/topicns.xml");
	
	tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = 1;
	tev__GetEventPropertiesResponse->wstop__TopicSet = (struct wstop__TopicSetType *)soap_malloc(soap, sizeof(struct wstop__TopicSetType)); 
	tev__GetEventPropertiesResponse->wstop__TopicSet->documentation = NULL;//(struct wstop__Documentation *)soap_malloc(soap,sizeof(struct wstop__Documentation));
	
	tev__GetEventPropertiesResponse->wstop__TopicSet->__size = 1;	
	tev__GetEventPropertiesResponse->wstop__TopicSet->__any = (char **)soap_malloc(soap, sizeof(char *) * tev__GetEventPropertiesResponse->wstop__TopicSet->__size);
	
	tev__GetEventPropertiesResponse->wstop__TopicSet->__any[0] = soap_strdup(soap, "<tns1:VideoSource wstop:topic=\"true\"><MotionAlarm wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"Source\" Type=\"tt:ReferenceToken\"/></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xsd:boolean\"/></tt:Data></tt:MessageDescription></MotionAlarm></tns1:VideoSource>");//soap_strdup(soap, "<wsnt:VideoAnalytics><wsnt:MotionDetection><wsnt:Motion wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"VideoAnalyticsConfigurationToken\" Type=\"tt:ReferenceToken\"/></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"ObjectId\" Type=\"tt:ObjectRefType\"/></tt:Data></tt:MessageDescription></wsnt:Motion></wsnt:MotionDetection></wsnt:VideoAnalytics>");

	tev__GetEventPropertiesResponse->__sizeTopicExpressionDialect = 2;//3;
	tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect = (char **)soap_malloc(soap, sizeof(char *) * tev__GetEventPropertiesResponse->__sizeTopicExpressionDialect); 
	tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect[0] = soap_strdup(soap, 			 "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
	tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect[1] = soap_strdup(soap, "http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete");	
	//tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect[2] = soap_strdup(soap, 			 "tns1:VideoSource");
	
	tev__GetEventPropertiesResponse->__sizeMessageContentFilterDialect = 1;
	tev__GetEventPropertiesResponse->MessageContentFilterDialect = (char **)soap_malloc(soap, sizeof(char *));

	tev__GetEventPropertiesResponse->MessageContentFilterDialect[0] = soap_strdup(soap, "http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter");
	
	tev__GetEventPropertiesResponse->__sizeProducerPropertiesFilterDialect = 0;
	tev__GetEventPropertiesResponse->ProducerPropertiesFilterDialect = NULL;
	
	tev__GetEventPropertiesResponse->__sizeMessageContentSchemaLocation = 1;
	tev__GetEventPropertiesResponse->MessageContentSchemaLocation =  (char **)soap_malloc(soap, sizeof(char *));
	tev__GetEventPropertiesResponse->MessageContentSchemaLocation[0] = soap_strdup(soap, "http://www.onvif.org/ver10/schema/onvif.xsd");
	tev__GetEventPropertiesResponse->__size = 0;
	tev__GetEventPropertiesResponse->__any = NULL;
#endif

	LEAVE();
	return SOAP_OK;

	UNUSED(tev__GetEventProperties);
}

int 
__tev__Renew(struct soap *soap, 
struct _wsnt__Renew *wsnt__Renew, 
struct _wsnt__RenewResponse *wsnt__RenewResponse)
{
	ENTER();

	time_t time_tm;
	time_t termtime_tm;
	time_t timeout;

	soap->header->wsa5__Action = soap_strdup(soap, "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/RenewResponse");

	timeout = periodtol(wsnt__Renew->TerminationTime);
	time_out += timeout;
	t_out += timeout;
	time_tm = time(&gptMsgShare.TIME);
	termtime_tm = time_tm + timeout;
	wsnt__RenewResponse->wsnt__TerminationTime = termtime_tm;
	wsnt__RenewResponse->wsnt__CurrentTime = (time_t *) soap_malloc(soap, sizeof(time_t)); 
	memset(wsnt__RenewResponse->wsnt__CurrentTime, 0x00, sizeof(time_t));
	wsnt__RenewResponse->wsnt__CurrentTime[0] = time_tm; 
	wsnt__RenewResponse->__size = 0;
	wsnt__RenewResponse->__any = NULL;

	LEAVE();
	return SOAP_OK;
}

int 
__tev__Unsubscribe(struct soap *soap, 
struct _wsnt__Unsubscribe *wsnt__Unsubscribe, 
struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
	ENTER();

	soap->header->wsa5__Action = soap_strdup(soap, "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeResponse");
	time_out = 0;
	t_out = 0;
	wsnt__UnsubscribeResponse->__size = 0;
	wsnt__UnsubscribeResponse->__any = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(wsnt__Unsubscribe);
}

//struct tm *notyfy_time_tm = NULL;
static UINT notycount = 0;

int __tev__Subscribe(struct soap *soap, 
struct _wsnt__Subscribe *wsnt__Subscribe, 
struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{
	ENTER();

	memset(notify_endpoint,0x00,sizeof(char)*100);

	soap->header->wsa5__Action = soap_strdup(soap, "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/SubscribeResponse");
		
	char macaddr[MACH_ADDR_LENGTH] = {0};
	char _IPAddr[LARGE_INFO_LENGTH] = {0};
	char initial_time[SMALL_INFO_LENGTH] = "PT10S";
///	char LOC_time[SMALL_INFO_LENGTH] = "PT9H";
	char strtime[100];	
	time_t t_time;

#ifdef ONVIF_NOTIFY_ENABLED
	int temp_sock;
#endif

//	notyfy_time_tm = localtime(&gptMsgShare.TIME);//gmtime
	struct tm timeinfo;
	enx_get_tmtime(gptMsgShare.TIME, &timeinfo, DEF_YES);
	sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	printf("localtime : [%s]\r\n", strtime);

	time_t time_tm = time(&gptMsgShare.TIME);
	net_get_hwaddr(ETH_IF_NAME, macaddr);
#ifdef ONVIF_NOTIFY_ENABLED

	if(wsnt__Subscribe->ConsumerReference.Address != NULL)
	{
		strcpy(notify_endpoint, wsnt__Subscribe->ConsumerReference.Address);
		printf("notify_endpoint [[ %s ]]\r\n", notify_endpoint);
	}

 	temp_sock = SOAP_INVALID_SOCKET; //SOAP_INVALID_SOCKET;//socket(AF_INET, SOCK_STREAM, 0); //create_socket_tcp();
	/* Check if previous Subscription is terminated */	
	if(canSubscribe)
	{
		/* Notification thread Initialization */
		//sys_thread_new("notify",notify_thr, (void *)&temp_sock, 32*1024, CAMERA_TASK_PRIO);	
	}
#endif

	if(0)//wsnt__Subscribe->Filter != NULL)
	{
		onvif_fault(soap,"","ter:InvalidArgVal","ter:InvalidFilterFault","","");
		LEAVE();
		return SOAP_FAULT;
	}

	if(wsnt__Subscribe->InitialTerminationTime != NULL)
	{
		dvprintf("wsnt__Subscribe->InitialTerminationTime %s\r\n",wsnt__Subscribe->InitialTerminationTime);
		t_time = periodtol(wsnt__Subscribe->InitialTerminationTime);
	}
	else
	{
		t_time = periodtol(initial_time);
	}
	dvprintf("t_time %ld\r\n",t_time);
	sprintf(_IPAddr,"http://%s/onvif/Events/SubManager_%s_%d", getIpAddress(HTTP_ONVIF_PORT),strtime,notycount++); 

	wsnt__SubscribeResponse->SubscriptionReference.Address = soap_strdup(soap, _IPAddr);
	wsnt__SubscribeResponse->SubscriptionReference.ReferenceParameters = NULL;
	wsnt__SubscribeResponse->SubscriptionReference.Metadata = NULL;
	wsnt__SubscribeResponse->SubscriptionReference.__size = 0;
	wsnt__SubscribeResponse->SubscriptionReference.__any = NULL;
	wsnt__SubscribeResponse->SubscriptionReference.__anyAttribute = NULL;
	wsnt__SubscribeResponse->wsnt__CurrentTime = (time_t *) soap_malloc(soap, sizeof(time_t));
	memset(wsnt__SubscribeResponse->wsnt__CurrentTime, 0x00, sizeof(time_t));
	wsnt__SubscribeResponse->wsnt__CurrentTime[0] = time_tm;
	t_time += time_tm;
	wsnt__SubscribeResponse->wsnt__TerminationTime = (time_t *) soap_malloc(soap, sizeof(time_t));
	memset(wsnt__SubscribeResponse->wsnt__TerminationTime, 0x00, sizeof(time_t));
	wsnt__SubscribeResponse->wsnt__TerminationTime[0] = t_time;
	wsnt__SubscribeResponse->__size = 0;
	wsnt__SubscribeResponse->__any = NULL;

	t_out = time_out = t_time;
	
	LEAVE();
	return SOAP_OK;
}

int __tev__GetCurrentMessage(struct soap *soap, 
struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage,
struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__GetCurrentMessage);
	UNUSED(wsnt__GetCurrentMessageResponse);
}

#if 1
int __tev__Notify(struct soap *soap, 
struct _wsnt__Notify *wsnt__Notify)
{
	ENTER();

#if 1
	if(topic_type == 0)
	{
		topic_type = 1;
		SysInfo *oSysInfo = GetSysInfo();
		struct tm timeinfo;
		enx_get_tmtime(gptMsgShare.TIME, &timeinfo, DEF_YES);
		char strtime[100];	
		char msg[LARGE_INFO_LENGTH*2];	
///		char macaddr[MACH_ADDR_LENGTH];
///		char _HwId[LARGE_INFO_LENGTH];

#if 0	
		wsnt__Notify->__sizeNotificationMessage = 2;
#else
		wsnt__Notify->__sizeNotificationMessage = 1;
#endif
		wsnt__Notify->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType)*wsnt__Notify->__sizeNotificationMessage);
		memset(wsnt__Notify->wsnt__NotificationMessage, 0x00, sizeof(struct wsnt__NotificationMessageHolderType)*wsnt__Notify->__sizeNotificationMessage);

		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__SubscriptionReference	= NULL;//= (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		memset(wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic, 0x00, sizeof(wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic));
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
#if 0		
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__any =  soap_strdup(soap, "tns1:RuleEngine/CellMotionDetector/Motion");
#else
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__any =  soap_strdup(soap, "tns1:VideoSource/MotionAlarm");
#endif
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__anyAttribute = NULL;
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__mixed = NULL;

		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__ProducerReference = NULL;//(struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

		dvprintf ( "The current date/time is: %s", strtime );
#if 0
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
#else
	if(eventstatus[0] & MOTION_DETECTED_MASK)
	{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
		eventstatus[0]=0;
		sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
		wsnt__Notify->wsnt__NotificationMessage->Message.__any = soap_strdup(soap,msg);
	}
	else			
	{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
		eventstatus[0]=0;
		sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
		wsnt__Notify->wsnt__NotificationMessage->Message.__any = soap_strdup(soap,msg);
	}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__SubscriptionReference	= NULL;//= (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		memset(wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic, 0x00, sizeof(wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic));
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
#if 1		
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__any =  soap_strdup(soap, "tns1:RuleEngine/CellMotionDetector/Motion");
#else
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__any =  soap_strdup(soap, "tns1:VideoSource/MotionAlarm");
#endif
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__anyAttribute = NULL;
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__mixed = NULL;
		
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__ProducerReference = NULL;//(struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	
		dvprintf ( "The current date/time is: %s", strtime );
#if 1
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
#else
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}

#endif	
#endif
	wsnt__Notify->__size = 0;
	wsnt__Notify->__any = NULL;
	}
	else
	{
		SysInfo *oSysInfo = GetSysInfo();
		struct tm timeinfo;
		enx_get_tmtime(gptMsgShare.TIME, &timeinfo, DEF_YES);
		char strtime[100];	
		char msg[LARGE_INFO_LENGTH*2];	
///		char macaddr[MACH_ADDR_LENGTH];
///		char _HwId[LARGE_INFO_LENGTH];

#if 0
		wsnt__Notify->__sizeNotificationMessage = 2;
#else
		wsnt__Notify->__sizeNotificationMessage = 1;
#endif
		wsnt__Notify->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType));
		memset(wsnt__Notify->wsnt__NotificationMessage, 0x00, sizeof(struct wsnt__NotificationMessageHolderType));

		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__SubscriptionReference	= NULL;//= (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
		
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		memset(wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic, 0x00, sizeof(wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic));
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
#if 0
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__any =  soap_strdup(soap, "tns1:RuleEngine/CellMotionDetector/Motion");
#else
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__any =  soap_strdup(soap, "tns1:VideoSource/MotionAlarm");
#endif
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__anyAttribute = NULL;
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__Topic->__mixed = NULL;
		
		wsnt__Notify->wsnt__NotificationMessage[0].wsnt__ProducerReference = NULL;//(struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

		dvprintf ( "The current date/time is: %s", strtime );

#if 0
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
#else
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			eventstatus[0]=0;
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			eventstatus[0]=0;
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
			wsnt__Notify->wsnt__NotificationMessage[0].Message.__any = soap_strdup(soap,msg);
		}
#endif		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__SubscriptionReference = NULL;//= (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
		
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
		memset(wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic, 0x00, sizeof(wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic));
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
#if 1
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__any =  soap_strdup(soap, "tns1:RuleEngine/CellMotionDetector/Motion");
#else
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__any =  soap_strdup(soap, "tns1:VideoSource/MotionAlarm");
#endif
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__anyAttribute = NULL;
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__Topic->__mixed = NULL;
		
		wsnt__Notify->wsnt__NotificationMessage[1].wsnt__ProducerReference = NULL;//(struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));

		sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		
		dvprintf ( "The current date/time is: %s", strtime );

#if 1
		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"VideoSourceToken\"/><tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/><tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
#else

		if(eventstatus[0] & MOTION_DETECTED_MASK)
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"VideoSource_1\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
		else			
		{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x",eventstatus[0],MOTION_DETECTED_MASK);
			sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Changed\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"VideoSource_1\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime);
			wsnt__Notify->wsnt__NotificationMessage[1].Message.__any = soap_strdup(soap,msg);
		}
#endif		
#endif
		wsnt__Notify->__size = 0;
		wsnt__Notify->__any = NULL;	
	}
#endif	

	LEAVE();
	return SOAP_OK;
}

#else
int 
__tev__Notify(struct soap *soap, 
struct _wsnt__Notify *wsnt__Notify)
{
	dvprintf("%s   \r\n",__func__);	
	
	time_t tnow;	
	struct tm * timeinfo;	
	char strtime[100];	
	char msg[LARGE_INFO_LENGTH];	
	tnow = time(NULL);		
	timeinfo = localtime ( &tnow );

	char macaddr[MACH_ADDR_LENGTH];
	char _HwId[LARGE_INFO_LENGTH];

	wsnt__Notify->__sizeNotificationMessage = 1;

	wsnt__Notify->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType));
	memset(wsnt__Notify->wsnt__NotificationMessage, 0x00, sizeof(struct wsnt__NotificationMessageHolderType));
	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference	= NULL;//= (struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference, 0x00, sizeof(struct wsa5__EndpointReferenceType));
//	net_get_hwaddr(ETH_IF_NAME, macaddr);
//	sprintf(_HwId,TEST_ONVIF_DEVICE_SCOPE_UUID,macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Address = soap_strdup(soap, _HwId);
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters = (struct wsa5__ReferenceParametersType *)soap_malloc(soap, sizeof(struct wsa5__ReferenceParametersType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters, 0x00, sizeof(struct wsa5__ReferenceParametersType));
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters->__size = 0;	/* sequence of elements <-any> */
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters->__any = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->ReferenceParameters->__anyAttribute = NULL;	/* optional attribute of type xsd:anyType */
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata = (struct wsa5__MetadataType *)soap_malloc(soap, sizeof(struct wsa5__MetadataType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata, 0x00, sizeof(struct wsa5__MetadataType));
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata->__size = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata->__any = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->Metadata->__anyAttribute = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->__size = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->__any = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__SubscriptionReference->__anyAttribute = NULL;
	
	wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic = (struct wsnt__TopicExpressionType *)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic, 0x00, sizeof(wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic));
	wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
	wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic->__any =  soap_strdup(soap, "tns1:VideoSource" /*"wsnt:VideoAnalytics/wsnt:MotionDetection/wsnt:Motion"*//*"tns1:VideoSource/MotionAlarm"*/);
	wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic->__anyAttribute = NULL;
	wsnt__Notify->wsnt__NotificationMessage->wsnt__Topic->__mixed = NULL;
	
	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference = NULL;//(struct wsa5__EndpointReferenceType *)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference, 0x00, sizeof(struct wsa5__EndpointReferenceType));
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Address = soap_strdup(soap,notify_endpoint);
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata  = (struct wsa5__MetadataType *)soap_malloc(soap, sizeof(struct wsa5__MetadataType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata, 0x00, sizeof(struct wsa5__MetadataType));
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata->__size = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata->__any = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->Metadata->__anyAttribute = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters = (struct wsa5__ReferenceParametersType *)soap_malloc(soap, sizeof(struct wsa5__ReferenceParametersType));
//	memset(wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters, 0x00, sizeof(struct wsa5__ReferenceParametersType));
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters->__size = 0;	/* sequence of elements <-any> */
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters->__any = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->ReferenceParameters->__anyAttribute = NULL;	/* optional attribute of type xsd:anyType */
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->__size = 0;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->__any = NULL;
//	wsnt__Notify->wsnt__NotificationMessage->wsnt__ProducerReference->__anyAttribute = NULL;



	sprintf(strtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour+1, timeinfo->tm_min, timeinfo->tm_sec);
	
	dvprintf ( "The current date/time is: %s\r\n", strtime );
	if(eventstatus[0] & MOTION_DETECTED_MASK)
	{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x\r\n",eventstatus[0],MOTION_DETECTED_MASK);
		sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"true\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
		wsnt__Notify->wsnt__NotificationMessage->Message.__any = soap_strdup(soap,msg);
	}
	else			
	{dvprintf("pull eventstatus %x MOTION_DETECTED_MASK %x\r\n",eventstatus[0],MOTION_DETECTED_MASK);
		sprintf(msg,"<tt:Message UtcTime=\"%s\" PropertyOperation=\"Initialized\"><tt:Source><tt:SimpleItem Name=\"Source\" Value=\"%s\"/></tt:Source><tt:Data><tt:SimpleItem Name=\"State\" Value=\"false\"/></tt:Data></tt:Message>", strtime, oSysInfo->VSC[0].Vtoken);
		wsnt__Notify->wsnt__NotificationMessage->Message.__any = soap_strdup(soap,msg);
	}

	
	wsnt__Notify->__size = 0;
	wsnt__Notify->__any = NULL;
	
	dvprintf("%s   E]]\r\n",__func__); 

	return SOAP_OK;
}
#endif

int __tev__GetMessages(struct soap *soap, 
struct _wsnt__GetMessages *wsnt__GetMessages, 
struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__GetMessages);
	UNUSED(wsnt__GetMessagesResponse);
}

int __tev__DestroyPullPoint(struct soap *soap, 
struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, 
struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__DestroyPullPoint);
	UNUSED(wsnt__DestroyPullPointResponse);
}

int __tev__CreatePullPoint(struct soap *soap, 
struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, 
struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__CreatePullPoint);
	UNUSED(wsnt__CreatePullPointResponse);
}

int __tev__PauseSubscription(struct soap *soap, 
struct _wsnt__PauseSubscription *wsnt__PauseSubscription, 
struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__PauseSubscription);
	UNUSED(wsnt__PauseSubscriptionResponse);
}

int __tev__ResumeSubscription(struct soap *soap, 
struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, 
struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsnt__ResumeSubscription);
	UNUSED(wsnt__ResumeSubscriptionResponse);
}


#ifdef USE_EVENT_INTERFACE	

//#include "gpio.h"

/**
 * @brief eventtracker_thr
 * @This thread records the events within the specified termination time.
 * (This thread is initialized for every 'CreatePullPointSubscription' request)
 * Operations Done
 *	1. Loop till 'Termination Time' (Sent as a part of Subscription Request)
 *	2. Intialize appropriate event handling mechanism
 * 	3. Wait for events
 *	4. Send Notify Packet whenever an event Occurs
 */
void eventtracker_thr(void *arg)
{
	ENTER();

///	int sem_value = 0;
///	int id;
	time_t current;
	time_t time_tm;
	int value = 1;
	int ret;
//	flprintf("TEST\r\n");
//	time_out = *(long *) arg;
	canSubscribe_pullmessages = 0;

	/* Saving current Time */
	current = time(&gptMsgShare.TIME);

	/* Initialize Motion Detection */
	ret = ControlSystemData(SFIELD_SET_MOTIONENABLE, (void *)&value, sizeof(value));
	if (ret < 0)
	{
		//ONVIF_DBG("Failed to enable motion.");
	}
//	flprintf("TEST\r\n");
	/* TODO:: Send Initialization Notification */	

	/* Loop till Termination Time */
	while(time_out > 0)
	{	
		/* semaphore get value */
		/* Semaphore value set in proc_alarm.c when motion is detected */
		/* Check if any motion is detected */
//		if(sem_value & MOTION_DETECTED_MASK)
		{
//			argument.val = 0;
//			semctl(id, 0, SETVAL, argument);
		}
//		else if(sem_value & NO_MOTION_MASK)
//		{
//			argument.val = 0;
//			semctl(id, 0, SETVAL, argument); 
//		}
		
	       	time_tm = time(&gptMsgShare.TIME);
		if((time_tm - current) >= time_out)
		{
			time_out = 0;
		}
		
		vTaskDelay(100);
	}
	canSubscribe_pullmessages = 1;
	LEAVE();
	vTaskDelete(xTaskGetCurrentTaskHandle());

//	vTaskDelete(NULL);
	UNUSED(arg);
}


#ifdef ONVIF_NOTIFY_ENABLED

/**
 * @brief notify_thr
 * @This thread sends the notify message when an event occurs.
 * (This thread is initialized for every 'Subscribe' request)
 * Operations Done
 *	1. Loop till 'Termination Time' (Sent as a part of Subscription Request)
 *	2. Intialize appropriate event handling mechanism
 * 	3. Wait for events
 *	4. Send Notify Packet whenever an event Occurs
 */
#if 1
void enx_motion_check(void *ctx)
{
	if(gptMsgISPTbl.ISP_MOTION)
	{
		int i;
		for(i=0;i<3;i++)
			eventstatus[i] = 1;
		gptMsgISPTbl.ISP_MOTION = 0;
	}
	UNUSED(ctx);
}

void notify_thr(void *arg)
{
	dvprintf("notify_thr s \r\n");

	vTaskDelay(1000);

	time_t time_tm;
///	int value = 1;
///	int ret;
	canSubscribe = 0;

	/* Initialize Motion Detection */
//	ret = ControlSystemData(SFIELD_SET_MOTIONENABLE, (void *)&value, sizeof(value));
	struct soap *soap_notify;	
///	int server_tcp; 	
///	SOAP_SOCKET m, s;
///	int retval = 0;
	soap_notify = soap_new();
	if(soap_notify == NULL)
	{
		printf("soap_notify == NULL \r\n");
		return;//break;
	}
///	int ret2;
	soap_init(soap_notify);
	soap_set_namespaces(soap_notify, notify_namespaces/*namespaces*/); 
	
	soap_notify->socket = SOAP_INVALID_SOCKET;//server_tcp;
	soap_notify->master = SOAP_INVALID_SOCKET;
	soap_notify->fopen = NULL;
	soap_notify->fconnect = enx_tcp_connect;
	soap_notify->fsend = enx_fsendhttp;
	soap_notify->faccept = enx_tcp_accept;
	soap_notify->fclose = enx_tcp_disconnect;
	//soap_notify->fdisconnect = enx_tcp_disconnect;
	soap_notify->fclosesocket = enx_tcp_closesocket;
	soap_notify->fshutdownsocket = enx_tcp_shutdownsocket;
	
	soap_notify->frecv = enx_frecv;
	soap_notify->fpoll = NULL;
	soap_notify->fmalloc = NULL;//enx_soap_malloc; 
	soap_set_endpoint(soap_notify,notify_endpoint);
	//enx_soap_bind(soap_notify,soap_notify->port, 0);
	soap_notify->errmode = 0;

	soap_notify->keep_alive = (((soap_notify->imode | soap_notify->omode) & SOAP_IO_KEEPALIVE) != 0); 
	soap_notify->user = NULL;
	soap_notify->bind_flags = 0;	

	timer_sched_timeout_cb(1000, TIMEOUT_PERIODIC, enx_motion_check, NULL);

	while(1)
	{	
			
		time_tm = time(&gptMsgShare.TIME);//gmtime
		//printf("time_out %x time_tm %x",time_out, time_tm);
		if(time_tm > time_out)
		{
			topic_type = 0;
			t_out = 0;
			dvprintf("notify_thr end \r\n");
			canSubscribe = 1;
		}
		else if((time_tm+2) >= time_out)
		{
			topic_type = 0;
			t_out = 0;
			dvprintf("notify_thr end \r\n");
			canSubscribe = 1;
			
			if(soap_notify != NULL)
			{

				if(strlen(notify_endpoint) > 0)
				{
					soap_set_endpoint(soap_notify,notify_endpoint);
					//enx_soap_bind(soap_notify,soap_notify->port, 0);
					soap_notify->errmode = 0;
					
					soap_notify->keep_alive = (((soap_notify->imode | soap_notify->omode) & SOAP_IO_KEEPALIVE) != 0); 
					soap_notify->user = NULL;
					soap_notify->bind_flags = 0;		
					soap_send___tev__Notify(soap_notify, notify_endpoint, "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/Notify");
					
					soap_destroy(soap_notify);		 
					soap_end(soap_notify);
					dvprintf("soap_notify e\r\n");

				}
				//soap_serve(soap_notify);
			}
		}
		else
		{
			canSubscribe = 0;	
			dvprintf("notify_thr ! \r\n");

			if(soap_notify != NULL)
			{

				if(strlen(notify_endpoint) > 0)
				{
					soap_set_endpoint(soap_notify,notify_endpoint);
					//enx_soap_bind(soap_notify,soap_notify->port, 0);
					soap_notify->errmode = 0;
					
					soap_notify->keep_alive = (((soap_notify->imode | soap_notify->omode) & SOAP_IO_KEEPALIVE) != 0); 
					soap_notify->user = NULL;
					soap_notify->bind_flags = 0;		
					soap_send___tev__Notify(soap_notify, notify_endpoint, "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/Notify");
					
					soap_destroy(soap_notify);		 
					soap_end(soap_notify);
					dvprintf("soap_notify e\r\n");

				}
				//soap_serve(soap_notify);
			}
			
			dvprintf("notify_thr e\r\n");
		}
		vTaskDelay(50);
	}
	
	soap_destroy(soap_notify);		 
	soap_end(soap_notify);
	//soap_done(soap_notify);
	soap_free(soap_notify); 
	//soap_notify = NULL;			
	
	vTaskDelete(NULL);

	UNUSED(arg);
}
#else
void notify_thr(void *arg)
{
	dvprintf("notify_thr s\r\n");
	
//	gMotionDetect = 0;

#if 1


	/* soap structure */
	//struct soap *soap_notify;
	int sem_value = 0;
	union semun{
                int val;
                struct semid_ds *buf;
                short *array;
        }argument;
	int id;
	time_t current;
	time_t time_tm;
	int value = 1;
	int ret;
	int temp_sock; /* DUP variable */
	canSubscribe = 0;

	/* Saving current Time */
	current = time(NULL);
	

	/* Get semaphore descriptor */
//	id = semget(KEY_NOTIFY_SEM, 1, 0666);
//	if(id < 0)
//	{
//		ONVIF_DBG( "Unable to obtain semaphore.");
//        }
	
	/* Initialize Motion Detection */
	ret = ControlSystemData(SFIELD_SET_MOTIONENABLE, (void *)&value, sizeof(value));
	if (ret < 0)
	{
		dvprintf("Failed to enable motion.\r\n");
	}

	/* Ignore any previous motion */
	argument.val = 0;
//	semctl(id, 0, SETVAL, argument);

	/* TODO:: Send Initialization Notification */	

#if 0
	temp_sock = *((int *)arg);
	/* soap initialization */
	soap_notify = soap_new();
	soap_init(soap_notify);

	/* soap binding */
	soap_notify->master = temp_sock;
	soap_notify->socket = temp_sock;
	soap_notify->errmode = 0;
	soap_notify->bind_flags = 1;
	soap_notify->error = SOAP_OK;
	soap_notify->keep_alive = 0;
	memset((void *)&soap_notify->peer, 0, sizeof(soap_notify->peer));
	
	soap_set_endpoint(soap_notify,notify_endpoint);
	printf("soap_notify->host %s,soap_notify->port %d\r\n",soap_notify->host,soap_notify->port);
	soap_notify->peer.sin_addr.s_addr = inet_addr(soap_notify->host);
	soap_notify->peer.sin_port = soap_notify->port;
	printf(" soap_set_endpoint s_addr %u.%u.%u.%u port %d soap->omode %x\r\n",(int)(soap_notify->peer.sin_addr.s_addr>>24)&0xFF, (int)(soap_notify->peer.sin_addr.s_addr>>16)&0xFF, (int)(soap_notify->peer.sin_addr.s_addr>>8)&0xFF, (int)soap_notify->peer.sin_addr.s_addr&0xFF,soap_notify->peer.sin_port,soap_notify->omode);

	soap_set_namespaces(soap_notify, namespaces);

#endif


	int server_tcp; 	
	SOAP_SOCKET m, s;
	int retval = 0;
	struct soap *soap_notify;	
	soap_notify = soap_new();

	int ret2;
	soap_init(soap_notify);
	soap_set_namespaces(soap_notify, namespaces);	

	soap_notify->fopen = NULL;//enx_tcp_connect;
	soap_notify->fconnect = enx_tcp_connect;
	soap_notify->fsend = enx_fsendhttp;//enx_fsend;
	soap_notify->faccept = NULL;//enx_tcp_accept;
	soap_notify->fclose  = enx_tcp_disconnect;
	soap_notify->fclosesocket = enx_tcp_closesocket;
	soap_notify->fshutdownsocket = enx_tcp_shutdownsocket;
	soap_notify->frecv = enx_frecv;
	soap_notify->fpoll = NULL;
	soap_notify->fmalloc = NULL;//enx_soap_malloc; 
	//server_tcp = create_connect_socket_udp();
	soap_set_endpoint(soap_notify,notify_endpoint);
	enx_soap_bind(soap_notify,soap_notify->port, 0);
	soap_notify->errmode = 0;

	soap_notify->socket = 0;//server_tcp;
	soap_notify->master = 0;//server_tcp;



#if 0	
//	soap_notify->faccept = enx_tcp_accept;
	soap_notify->fconnect   =  enx_tcp_connect;
	soap_notify->fclose  = enx_tcp_disconnect;
	soap_notify->fclosesocket = enx_tcp_closesocket;
	soap_notify->fshutdownsocket = enx_tcp_shutdownsocket;
//	soap_notify->fsend = enx_fsend;
//	soap_notify->frecv = enx_frecv;
//	soap_notify->fopen = enx_soapsend;

	soap_notify->fsend 			= enx_fsend;
	soap_notify->frecv 			= enx_soaprecv;

	soap_notify->fpoll = NULL;
	soap_notify->fmalloc = NULL;//enx_soap_malloc;

	if (soap_valid_socket(soap_notify->socket))
	{
		soap_notify->ip = ntohl(soap_notify->peer.sin_addr.s_addr);
		soap_notify->port = soap_notify->peer.sin_port;//(int)ntohs(soap_notify->peer.sin_port);
		
	}
#endif		
	soap_notify->keep_alive = (((soap_notify->imode | soap_notify->omode) & SOAP_IO_KEEPALIVE) != 0);	

	/* Loop till Termination Time */
	while(t_out > 0)
	{	
		/* semaphore get value */
		/* Semaphore value set in proc_alarm.c when motion is detected */
//		sem_value = semctl(id, 0, GETVAL, 0);
		
		/* Check if any motion is detected */
//		if(	gMotionDetect & MOTION_DETECTED_MASK)
		{
//			gMotionDetect = 0;
			/* Send Change Notification */
			//ret = soap_serve___tev__Notify(soap_notify);
			soap_send___tev__Notify(soap_notify, notify_endpoint, "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/Notify");
			argument.val = 0;
			printf("soap_send___tev__Notify ]] \r\n");	
//			semctl(id, 0, SETVAL, argument);
		}
//		else if(sem_value & NO_MOTION_MASK)
		{
			argument.val = 0;
//			semctl(id, 0, SETVAL, argument); 
			/* TODO:: Send Delete Notification */
		}
		
	       	time_tm = time(NULL);
		if((time_tm - current) > t_out)
		{
			t_out = 0;
		}
		vTaskDelay(500);
	}
	//soap_dealloc(soap_notify, NULL); 
	soap_destroy(soap_notify);       
	soap_end(soap_notify);
	soap_done(soap_notify);
	soap_free(soap_notify); 
	canSubscribe = 1;
//	pthread_exit(NULL);
#endif
printf("notify_thr e\r\n");

	vTaskDelete(NULL);

}
#endif
#endif
#endif
