#include "soapH.h"  
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "FreeRTOS.h" 
#include <stdio.h>
#include "onvifSystemInfo.h"
#include "onvifUtil.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

int __tad__GetServiceCapabilities(struct soap *soap, 
	struct _tad__GetServiceCapabilities *tad__GetServiceCapabilities, 
	struct _tad__GetServiceCapabilitiesResponse *tad__GetServiceCapabilitiesResponse)
{
	ENTER();

	tad__GetServiceCapabilitiesResponse->Capabilities = (struct tad__Capabilities *)soap_malloc(soap, sizeof(struct tad__Capabilities));
	memset(tad__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct tad__Capabilities));
	struct tad__Capabilities *Cap = tad__GetServiceCapabilitiesResponse->Capabilities;

	Cap->__size = 0;
	Cap->__any = NULL;
	Cap->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tad__GetServiceCapabilities);
}

int __tad__DeleteAnalyticsEngineControl(struct soap *soap, 
	struct _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl, 
	struct _tad__DeleteAnalyticsEngineControlResponse *tad__DeleteAnalyticsEngineControlResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__DeleteAnalyticsEngineControl);
	UNUSED(tad__DeleteAnalyticsEngineControlResponse);
}

int __tad__CreateAnalyticsEngineControl(struct soap *soap, 
	struct _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl, 
	struct _tad__CreateAnalyticsEngineControlResponse *tad__CreateAnalyticsEngineControlResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__CreateAnalyticsEngineControl);
	UNUSED(tad__CreateAnalyticsEngineControlResponse);
}

int __tad__SetAnalyticsEngineControl(struct soap *soap, 
	struct _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl, 
	struct _tad__SetAnalyticsEngineControlResponse *tad__SetAnalyticsEngineControlResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__SetAnalyticsEngineControl);
	UNUSED(tad__SetAnalyticsEngineControlResponse);
}

int __tad__GetAnalyticsEngineControl(struct soap *soap, 
	struct _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl, 
	struct _tad__GetAnalyticsEngineControlResponse *tad__GetAnalyticsEngineControlResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsEngineControl);
	UNUSED(tad__GetAnalyticsEngineControlResponse);
}

int __tad__GetAnalyticsEngineControls(struct soap *soap, 
	struct _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls, 
	struct _tad__GetAnalyticsEngineControlsResponse *tad__GetAnalyticsEngineControlsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsEngineControls);
	UNUSED(tad__GetAnalyticsEngineControlsResponse);
}

int __tad__GetAnalyticsEngine(struct soap *soap, 
	struct _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine, 
	struct _tad__GetAnalyticsEngineResponse *tad__GetAnalyticsEngineResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsEngine);
	UNUSED(tad__GetAnalyticsEngineResponse);
}

int __tad__GetAnalyticsEngines(struct soap *soap, 
	struct _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines, 
	struct _tad__GetAnalyticsEnginesResponse *tad__GetAnalyticsEnginesResponse)
{
	ENTER();

	tad__GetAnalyticsEnginesResponse->__sizeConfiguration = 1;

	tad__GetAnalyticsEnginesResponse->Configuration = (struct tt__AnalyticsEngine *)soap_malloc(soap, sizeof(struct tt__AnalyticsEngine));
	memset(tad__GetAnalyticsEnginesResponse->Configuration, 0x00, sizeof(struct tt__AnalyticsEngine));
	struct tt__AnalyticsEngine *AnaCfg = tad__GetAnalyticsEnginesResponse->Configuration;

	AnaCfg->Name = soap_strdup(soap, "test name");
	AnaCfg->UseCount = 2;
	AnaCfg->token = soap_strdup(soap, "testToken");
	AnaCfg->AnalyticsEngineConfiguration = NULL;

	AnaCfg->__size = 0;
	AnaCfg->__any = NULL;
	AnaCfg->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tad__GetAnalyticsEngines);
}

int __tad__SetVideoAnalyticsConfiguration(struct soap *soap, 
	struct _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration, 
	struct _tad__SetVideoAnalyticsConfigurationResponse *tad__SetVideoAnalyticsConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__SetVideoAnalyticsConfiguration);
	UNUSED(tad__SetVideoAnalyticsConfigurationResponse);
}

int __tad__SetAnalyticsEngineInput(struct soap *soap, 
	struct _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput, 
	struct _tad__SetAnalyticsEngineInputResponse *tad__SetAnalyticsEngineInputResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__SetAnalyticsEngineInput);
	UNUSED(tad__SetAnalyticsEngineInputResponse);
}

int __tad__GetAnalyticsEngineInput(struct soap *soap, 
	struct _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput, 
	struct _tad__GetAnalyticsEngineInputResponse *tad__GetAnalyticsEngineInputResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsEngineInput);
	UNUSED(tad__GetAnalyticsEngineInputResponse);
}

int 
__tad__GetAnalyticsEngineInputs(struct soap *soap, 
	struct _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs, 
	struct _tad__GetAnalyticsEngineInputsResponse *tad__GetAnalyticsEngineInputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsEngineInputs);
	UNUSED(tad__GetAnalyticsEngineInputsResponse);
}

int __tad__GetAnalyticsDeviceStreamUri(struct soap *soap, 
	struct _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri, 
	struct _tad__GetAnalyticsDeviceStreamUriResponse *tad__GetAnalyticsDeviceStreamUriResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsDeviceStreamUri);
	UNUSED(tad__GetAnalyticsDeviceStreamUriResponse);
}

int __tad__GetVideoAnalyticsConfiguration(struct soap *soap, 
	struct _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration, 
	struct _tad__GetVideoAnalyticsConfigurationResponse *tad__GetVideoAnalyticsConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__SetAnalyticsEngineControl);
	UNUSED(tad__SetAnalyticsEngineControlResponse);
}

int __tad__CreateAnalyticsEngineInputs(struct soap *soap, 
	struct _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs, 
	struct _tad__CreateAnalyticsEngineInputsResponse *tad__CreateAnalyticsEngineInputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__CreateAnalyticsEngineInputs);
	UNUSED(tad__CreateAnalyticsEngineInputsResponse);
}

int __tad__DeleteAnalyticsEngineInputs(struct soap *soap, 
	struct _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs, 
	struct _tad__DeleteAnalyticsEngineInputsResponse *tad__DeleteAnalyticsEngineInputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__DeleteAnalyticsEngineInputs);
	UNUSED(tad__DeleteAnalyticsEngineInputsResponse);
}

int __tad__GetAnalyticsState(struct soap *soap, 
	struct _tad__GetAnalyticsState *tad__GetAnalyticsState, 
	struct _tad__GetAnalyticsStateResponse *tad__GetAnalyticsStateResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tad__GetAnalyticsState);
	UNUSED(tad__GetAnalyticsStateResponse);
}

