/**
 *
 * \file
 * CGI and SSI helper functions.
 *
 */

//*****************************************************************************
//
// cgifuncs.c - Helper functions related to CGI script parameter parsing.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 6288 of the EK-LM3S6965 Firmware Package.
//
//*****************************************************************************

#include <string.h>
#include "dev.h"																// for snprintf
#include "cgifuncs.h"

#define true 1
#define false 0

//
// \todo TODO: this is mapped out due to preprocessor defs,
// we need to find out why, but for now, use a prototype here
//
//int	 _EXFUN(strcasecmp,(const char *, const char *));

//*****************************************************************************
//!
//! Searches the list of parameters passed to a CGI handler and returns the
//! index of a given parameter within that list.
//!
//! \param pcToFind is a pointer to a string containing the name of the
//! parameter that is to be found.
//! \param pcParam is an array of character pointers, each containing the name
//! of a single parameter as encoded in the URI requesting the CGI.
//! \param iNumParams is the number of elements in the pcParam array.
//!
//! This function searches an array of parameters to find the string passed in
//! \e pcToFind.  If the string is found, the index of that string within the
//! \e pcParam array is returned, otherwise -1 is returned.
//!
//! \return Returns the index of string \e pcToFind within array \e pcParam
//! or -1 if the string does not exist in the array.
//!
//*****************************************************************************
int
FindCGIParameter(const char *pcToFind, char *pcParam[], int iNumParams)
{
    int iLoop;

    //
    // Scan through all the parameters in the array.
    //
    for(iLoop = 0; iLoop < iNumParams; iLoop++)
    {
        //
        // Does the parameter name match the provided string?
        //
        if(strcmp(pcToFind, pcParam[iLoop]) == 0)
        {
            //
            // We found a match - return the index.
            //
            return(iLoop);
        }
    }

    //
    // If we drop out, the parameter was not found.
    //
    return(-1);
}

//*****************************************************************************
//!
//! Determines whether a given character is a valid hexadecimal digit.
//!
//! \param cDigit is the ASCII character code to be checked for validity.
//!
//! This function checks the passed character to determime whether or not it is
//! a valid hexadecimal digit.
//!
//! \return Returns \e true if the passed character is a valid hex digit or \e
//! false otherwise
//!
//*****************************************************************************
int
IsValidHexDigit(const char cDigit)
{
    if(((cDigit >= '0') && (cDigit <= '9')) ||
       ((cDigit >= 'a') && (cDigit <= 'f')) ||
       ((cDigit >= 'A') && (cDigit <= 'F')))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
//!
//! Converts an ASCII character to a hex digit value.
//!
//! \param cDigit is the ASCII character code to convert to a number.
//!
//! This function converts a passed ASCII character to a number assuming that
//! the character is a valid hexadecimal digit.  If the character is not a valid
//! hex digit, the value 0xFF is returned.
//!
//! \return Returns the number represented by hex digit \e cDigit or 0xFF if
//! the character passed is not valid hex.
//!
//*****************************************************************************
unsigned char
HexDigit(const char cDigit)
{
    if((cDigit >= '0') && (cDigit <= '9'))
    {
        return(cDigit - '0');
    }
    else
    {
        if((cDigit >= 'a') && (cDigit <= 'f'))
        {
            return((cDigit - 'a') + 10);
        }
        else
        {
            if((cDigit >= 'A') && (cDigit <= 'F'))
            {
                return((cDigit - 'A') + 10);
            }
        }
    }

    //
    // If we get here, we were passed an invalid hex digit so return 0xFF.
    //
    return(0xFF);
}

//*****************************************************************************
//!
//! Decodes a single %xx escape sequence as an ASCII character.
//!
//! \param pcEncoded points to the ``%'' character at the start of a three
//! character escape sequence which represents a single ASCII character.
//! \param pcDecoded points to a byte which will be written with the decoded
//! character assuming the escape sequence is valid.
//!
//! This function decodes a single escape sequence of the form ``%xy'' where
//! x and y represent hexadecimal digits.  If each digit is a valid hex digit,
//! the function writes the decoded character to the pcDecoded buffer and
//! returns true, else it returns false.
//!
//! \return Returns \b true on success or \b false if pcEncoded does not point
//! to a valid escape sequence.
//!
//*****************************************************************************
int
DecodeHexEscape(const char *pcEncoded, char *pcDecoded)
{
    if((pcEncoded[0] != '%') || !IsValidHexDigit(pcEncoded[1]) ||
       !IsValidHexDigit(pcEncoded[2]))
    {
        return(false);
    }
    else
    {
        *pcDecoded = ((HexDigit(pcEncoded[1]) * 16) +
                      HexDigit(pcEncoded[2]));
        return(true);
    }
}

//*****************************************************************************
//!
//! Encodes a string for use within an HTML tag, escaping non alphanumeric
//! characters.
//!
//! \param pcDecoded is a pointer to a null terminated ASCII string.
//! \param pcEncoded is a pointer to a storage for the encoded string.
//! \param ulLen is the number of bytes of storage pointed to by pcEncoded.
//!
//! This function encodes a string, adding escapes in place of any special,
//! non-alphanumeric characters.  If the encoded string is too long for the
//! provided output buffer, the output will be truncated.
//!
//! \return Returns the number of characters written to the output buffer
//! not including the terminating NULL.
//!
//*****************************************************************************
unsigned long
EncodeFormString(const char *pcDecoded, char *pcEncoded,
                 unsigned long ulLen)
{
    unsigned long ulLoop;
    unsigned long ulCount;

    //
    // Make sure we were not passed a tiny buffer.
    //
    if(ulLen <= 1)
    {
        return(0);
    }

    //
    // Initialize our output character counter.
    //
    ulCount = 0;

    //
    // Step through each character of the input until we run out of data or
    // space to put our output in.
    //
    for(ulLoop = 0; pcDecoded[ulLoop] && (ulCount < (ulLen - 1)); ulLoop++)
    {
        switch(pcDecoded[ulLoop])
        {
            //
            // Pass most characters without modification.
            //
            default:
            {
                pcEncoded[ulCount++] = pcDecoded[ulLoop];
                break;
            }

            case '\'':
            {
                ulCount += snprintf(&pcEncoded[ulCount], (ulLen - ulCount), "&#39;");
                break;
            }
        }
    }

    return(ulCount);
}

//*****************************************************************************
//!
//! Decodes a string encoded as part of an HTTP URI.
//!
//! \param pcEncoded is a pointer to a null terminated string encoded as per
//! RFC1738, section 2.2.
//! \param pcDecoded is a pointer to storage for the decoded, null terminated
//! string.
//! \param ulLen is the number of bytes of storage pointed to by pcDecoded.
//!
//! This function decodes a string which has been encoded using the method
//! described in RFC1738, section 2.2 for URLs.  If the decoded string is too
//! long for the provided output buffer, the output will be truncated.
//!
//! \return Returns the number of character written to the output buffer, not
//! including the terminating NULL.
//!
//*****************************************************************************
unsigned long
DecodeFormString(const  char *pcEncoded, char *pcDecoded,
                 unsigned long ulLen)
{
    unsigned long ulLoop;
    unsigned long ulCount;
    int bValid;

    ulCount = 0;
    ulLoop = 0;

    //
    // Keep going until we run out of input or fill the output buffer.
    //
    while(pcEncoded[ulLoop] && (ulCount < (ulLen - 1)))
    {
        switch(pcEncoded[ulLoop])
        {
            //
            // '+' in the encoded data is decoded as a space.
            //
            case '+':
            {
                pcDecoded[ulCount++] = ' ';
                ulLoop++;
                break;
            }

            //
            // '%' in the encoded data indicates that the following 2
            // characters indicate the hex ASCII code of the decoded character.
            //
            case '%':
            {
                if(pcEncoded[ulLoop + 1] && pcEncoded[ulLoop + 2])
                {
                    //
                    // Decode the escape sequence.
                    //
                    bValid = DecodeHexEscape(&pcEncoded[ulLoop],
                                             &pcDecoded[ulCount]);

                    //
                    // If the escape sequence was valid, move to the next
                    // output character.
                    //
                    if(bValid)
                    {
                        ulCount++;
                    }

                    //
                    // Skip past the escape sequence in the encoded string.
                    //
                    ulLoop += 3;
                }
                else
                {
                    //
                    // We reached the end of the string partway through an
                    // escape sequence so just ignore it and return the number
                    // of decoded characters found so far.
                    //
                    pcDecoded[ulCount] = '\0';
                    return(ulCount);
                }
                break;
            }

            //
            // For all other characters, just copy the input to the output.
            //
            default:
            {
                pcDecoded[ulCount++] = pcEncoded[ulLoop++];
                break;
            }
        }
    }

    //
    // Terminate the string and return the number of characters we decoded.
    //
    pcDecoded[ulCount] = '\0';
    return(ulCount);
}

//*****************************************************************************
//!
//! Ensures that a string passed represents a valid decimal number and,
//! if so, converts that number to a long.
//!
//! \param pcValue points to a null terminated string which should contain an
//! ASCII representation of a decimal number.
//! \param plValue points to storage which will receive the number represented
//! by pcValue assuming the string is a valid decimal number.
//!
//! This function determines whether or not a given string represents a valid
//! decimal number and, if it does, converts the string into a decimal number
//! which is returned to the caller.
//!
//! \return Returns \b true if the string is a valid representation of a
//! decimal number or \b false if not.
//!
//*****************************************************************************
int
CheckDecimalParam(const char *pcValue, long *plValue)
{
    unsigned long ulLoop;
    int bStarted;
    int bFinished;
    int bNeg;
    long lAccum;

    //
    // Check that the string is a valid decimal number.
    //
    bStarted = false;
    bFinished = false;
    bNeg = false;
    ulLoop = 0;
    lAccum = 0;

    while(pcValue[ulLoop])
    {
        //
        // Ignore whitespace before the string.
        //
        if(!bStarted)
        {
            if((pcValue[ulLoop] == ' ') || (pcValue[ulLoop] == '\t'))
            {
                ulLoop++;
                continue;
            }

            //
            // Ignore a + or - character as long as we have not started.
            //
            if((pcValue[ulLoop] == '+') || (pcValue[ulLoop] == '-'))
            {
                //
                // If the string starts with a '-', remember to negate the
                // result.
                //
                bNeg = (pcValue[ulLoop] == '-') ? true : false;
                bStarted = true;
                ulLoop++;
            }
            else
            {
                //
                // We found something other than whitespace or a sign character
                // so we start looking for numerals now.
                //
                bStarted = true;
            }
        }

        if(bStarted)
        {
            if(!bFinished)
            {
                //
                // We expect to see nothing other than valid digit characters
                // here.
                //
                if((pcValue[ulLoop] >= '0') && (pcValue[ulLoop] <= '9'))
                {
                    lAccum = (lAccum * 10) + (pcValue[ulLoop] - '0');
                }
                else
                {
                    //
                    // Have we hit whitespace?  If so, check for no more
                    // characters until the end of the string.
                    //
                    if((pcValue[ulLoop] == ' ') || (pcValue[ulLoop] == '\t'))
                    {
                        bFinished = true;
                    }
                    else
                    {
                        //
                        // We got something other than a digit or whitespace so
                        // this makes the string invalid as a decimal number.
                        //
                        return(false);
                    }
                }
            }
            else
            {
                //
                // We are scanning for whitespace until the end of the string.
                //
                if((pcValue[ulLoop] != ' ') && (pcValue[ulLoop] != '\t'))
                {
                    //
                    // We found something other than whitespace so the string
                    // is not valid.
                    //
                    return(false);
                }
            }

            //
            // Move on to the next character in the string.
            //
            ulLoop++;
        }
    }

    //
    // If we drop out of the loop, the string must be valid.  All we need to do
    // now is negate the accumulated value if the string started with a '-'.
    //
    *plValue =  bNeg ? -lAccum : lAccum;
    return(true);
}

//*****************************************************************************
//!
//! Searches the list of parameters passed to a CGI handler for a parameter
//! with the given name and, if found, reads the parameter value as a decimal
//! number.
//!
//! \param pcName is a pointer to a string containing the name of the
//! parameter that is to be found.
//! \param pcParams is an array of character pointers, each containing the name
//! of a single parameter as encoded in the URI requesting the CGI.
//! \param pcValue is an array of values associated with each parameter from
//! the pcParam array.
//! \param iNumParams is the number of elements in the pcParam array.
//! \param pbError is a pointer that will be written to \b true if there is any
//! error during the parameter parsing process (parameter not found, value is
//! not a valid decimal number).
//!
//! This function searches an array of parameters to find the string passed in
//! \e pcName.  If the string is found, the corresponding parameter value is
//! read from array pcValues and checked to make sure that it is a valid
//! decimal number.  If so, the number is returned.  If any error is detected,
//! parameter \e pbError is written to \b true.  Note that \e pbError is NOT
//! written if the parameter is successfully found and validated.  This is to
//! allow multiple parameters to be parsed without the caller needing to check
//! return codes after each individual call.
//!
//! \return Returns the value of the parameter or 0 if an error is detected (in
//! which case \e *pbError will be \b true).
//!
//*****************************************************************************
long
GetCGIParam(const char *pcName, char *pcParams[], char *pcValue[],
            int iNumParams, int *pbError)
{
    int iParam;
    long lValue;
    int bRetcode;

    //
    // Is the parameter we are looking for in the list?
    //
    iParam = FindCGIParameter(pcName, pcParams, iNumParams);
    if(iParam != -1)
    {
        //
        // We found the parameter so now get its value.
        //
        bRetcode = CheckDecimalParam(pcValue[iParam], &lValue);
        if(bRetcode)
        {
            //
            // All is well - return the parameter value.
            //
            return(lValue);
        }
    }

    //
    // If we reach here, there was a problem so return 0 and set the error
    // flag.
    //
    *pbError = true;
    return(0);
}




//*****************************************************************************
//!
//! Searches the list of parameters passed to a CGI handler for a parameter
//! with the given name and, if found, tries to parse the parameter value as
//! a boolean.
//!
//! \param pcName is a pointer to a string containing the name of the
//! parameter that is to be found.
//! \param pcParams is an array of character pointers, each containing the name
//! of a single parameter as encoded in the URI requesting the CGI.
//! \param pcValue is an array of values associated with each parameter from
//! the pcParam array.
//! \param iNumParams is the number of elements in the pcParam array.
//! \param pbError is not used in this function.
//!
//! This function searches an array of parameters to find the string passed in
//! \e pcName.  If the string is found, the corresponding parameter value is
//! read from array pcValues and parsed as a boolean, so that the values 'true',
//! 'on' or '1' will return true, any other value will return false.
//! As any value is accepted, the parameter \e pbError is never set.
//!
//! \return Returns the result of the string comparison, true or false.
//!
//*****************************************************************************
long GetCGIParamBool(const char *pcName, char *pcParams[], char *pcValue[],
            int iNumParams, int *pbError)
{
    int iParam;
    int bRetcode = false;

    //
    // Is the parameter we are looking for in the list?
    //
    iParam = FindCGIParameter(pcName, pcParams, iNumParams);
    if (iParam != -1)
    {
        //
        // We found the parameter so now get its value.
        //
    	char pcDecodedString[8];
        DecodeFormString(pcValue[iParam], pcDecodedString, 7);

        if ( (strcasecmp(pcDecodedString,"true") == 0) ||
        	 (strcasecmp(pcDecodedString,"on") == 0) ||
        	 (strcasecmp(pcDecodedString,"1") == 0) )
        	bRetcode = true;

        return bRetcode;
    }
    return(0);
    UNUSED(pbError);
}
