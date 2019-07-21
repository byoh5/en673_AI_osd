/*
	wsseapi.h

	WS-Security plugin.

	See wsseapi.c for documentation and details.

gSOAP XML Web services tools
Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc., All Rights Reserved.
This part of the software is released under one of the following licenses:
GPL, the gSOAP public license, or Genivia's license for commercial use.
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc., All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef WSSEAPI_H
#define WSSEAPI_H

#include "soapH.h"	/* replace with soapcpp2-generated *H.h file */
//#include "smdevp.h"
//#include "mecevp.h"

#ifdef __cplusplus
extern "C" {
#endif

/** plugin identification for plugin registry */
#define SOAP_WSSE_ID "SOAP-WSSE-1.2"

/** plugin identification for plugin registry */
extern const char soap_wsse_id[];

/**
@fn void soap_wsse_delete_Security(struct soap *soap)
@brief Deletes Security header element.
@param soap context
*/
void
soap_wsse_delete_Security(struct soap *soap);

/**
@fn _wsse__Security* soap_wsse_Security(struct soap *soap)
@brief Returns Security header element if present.
@param soap context
@return _wsse__Security object or NULL
*/
struct _wsse__Security*
soap_wsse_Security(struct soap *soap);

/**
@fn int soap_wsse_CheckSecurity(struct soap *soap)
@brief Returns 1 if security passed.
@param soap context
@return 1 object or 0
*/
int
soap_wsse_CheckSecurity(struct soap *soap);

/**
@fn void soap_wsse_ClearSecurity(struct soap *soap)
@param soap context
*/
void
soap_wsse_ClearSecurity(struct soap *soap);


/**
@fn void soap_wsse_DefaultSecurity(struct soap *soap)
@param soap context
*/
void
soap_wsse_DefaultSecurity(struct soap *soap);

/**
@fn _wsse__UsernameToken* soap_wsse_UsernameToken(struct soap *soap, const char *id)
@brief Returns UsernameToken element if present.
@param soap context
@param[in] id string of UsernameToken or NULL
@return _wsse__UsernameToken object or NULL

Note: this release supports the use of at most one UsernameToken in the header.
*/
struct _wsse__UsernameToken*
soap_wsse_UsernameToken(struct soap *soap, const char *id);

/**
@fn const char* soap_wsse_get_Username(struct soap *soap)
@brief Returns UsernameToken/username string or wsse:FailedAuthentication fault.
@param soap context
@return UsernameToken/username string or NULL with wsse:FailedAuthentication fault error set
@see soap_wsse_verify_Password

The returned username should be used to lookup the user's password in a
dictionary or database for server-side authentication with
soap_wsse_verify_Password.
*/
const char*
soap_wsse_get_Username(struct soap *soap);

/**
@fn int soap_wsse_verify_Password(struct soap *soap, const char *password)
@brief Verifies the supplied password or sets wsse:FailedAuthentication fault.
@param soap context
@param[in] password string to verify against
@return SOAP_OK (authorized) or SOAP_FAULT with wsse:FailedAuthentication fault

The verification supports both clear-text password verification and digest
password authentication. For digest authentication a history mechanism with a
digest authentication session database ensures protection against replay
attacks.
Note: this release supports the use of at most one UsernameToken in the header.
*/
int
soap_wsse_verify_Password(struct soap *soap, const char *password);

/******************************************************************************\
 *
 * Faults
 *
\******************************************************************************/

/**
@fn int soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail)
@brief Sets sender SOAP Fault (sub)code for server fault response.
@param soap context
@param[in] faultsubcode sub code string
@param[in] faultstring fault string
@param[in] faultdetail detail string
@return SOAP_FAULT
*/
int
soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail);

/**
@fn int soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail)
@brief Sets SOAP Fault (sub)code for server response.
@param soap context
@param[in] fault is one of wsse:FaultcodeEnum
@param[in] detail string with optional text message
@return SOAP_FAULT
*/
int
soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail);


#ifdef __cplusplus
}
#endif

SOAP_FMAC1 void SOAP_FMAC2 soap_default_xsd__anyType(struct soap*, struct soap_dom_element *node);
SOAP_FMAC1 int SOAP_FMAC2 soap_out_xsd__anyType(struct soap *soap, const char *tag, int id, const struct soap_dom_element *node, const char *type);
SOAP_FMAC1 struct soap_dom_element * SOAP_FMAC2 soap_in_xsd__anyType(struct soap *soap, const char *tag, struct soap_dom_element *node, const char *type);

#endif
