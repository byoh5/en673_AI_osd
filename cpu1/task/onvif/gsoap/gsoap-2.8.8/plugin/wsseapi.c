/*
	wsseapi.c

	WS-Security plugin

gSOAP XML Web services tools
Copyright (C) 2000-2010, Robert van Engelen, Genivia Inc., All Rights Reserved.
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
Copyright (C) 2000-2010, Robert van Engelen, Genivia Inc., All Rights Reserved.
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

/**

@mainpage

- @ref wsse documents the wsse plugin for WS-Security 1.0 support.
- @ref smdevp documents the smdevp signature/digest engine.
- @ref mecevp documents the mecevp encryption engine.
- @ref threads documents a portable threads and locking API.

*/

/**

@page wsse The wsse WS-Security plugin

@section wsse_5 Security Header

The material in this section relates to the WS-Security specification section 5.

To use the wsse plugin:
-# Run wsdl2h -t typemap.dat on a WSDL of a service that requires WS-Security
   headers. The typemap.dat file is used to recognize and translate Security
   header blocks for XML signature and encryption.
-# Run soapcpp2 on the header file produced by wsdl2h.
-# (Re-)compile stdsoap2.c/pp, dom.c/pp, smdevp.c, mecevp.c, wsseapi.c and the
   generated source files with the -DWITH_DOM and -DWITH_OPENSSL compile flags
   set. The smdevp.c, mecevp.c, and wsseapi.c files are located in the 'plugin'
   directory.
-# Use the wsse plugin API functions described below to add and verify
   Security headers, sign and verify messages, and to encrypt/decrypt messages.

An example wsse client/server application can be found in gsoap/samples/wsse.

The wsse engine is thread safe. However, if HTTPS is required please follow the
instructions in Section @ref wsse_11 to ensure thread-safety of WS-Security
with HTTPS.

The wsse plugin API consists of a set of functions to populate and verify
WS-Security headers and message body content. For more details, we refer to the
following sections that correspond to the WS-Security specification sections:

- Section 6 @ref wsse_6
- Section 7 @ref wsse_7
- Section 8 @ref wsse_8
- Section 9 @ref wsse_9
- Section 10 @ref wsse_10
- @ref wsse_11
- @ref wsse_12
- @ref wsse_13

The basic API is introduced below.

To add an empty Security header block to the SOAP header, use:

@code
    soap_wsse_add_Security(soap);
@endcode

To delete a Security header, use:

@code
    soap_wsse_delete_Security(soap);
@endcode

Adding an empty Security header block is not very useful. In the following, we
present the higher-level functions of the wsse plugin to populate and verify
Security header content.

Note: The soap context includes an actor value soap.actor that is populated and
rendered as the env:actor (SOAP 1.1) or env:role (SOAP 1.2) attribute
in XML within the generic SOAP Header. The attribute is optional, but should be
used to target a recipient such as an intermediate node to process the SOAP
header.  In contrast, actor or role attributes within Security header blocks
target specific recipients to process the Security header block. The gSOAP
implementation does not automate this feature and application should set and
check the actor/role attribute when necessary. In addition, the current
implementation supports the inclusion of a single Security header block in the
SOAP header.

To populate the env:actor or env:role attribute within the Security
header, use:

@code
    soap_wsse_add_Security_actor(soap, "recipient");
@endcode

To obtain the actor or role value (e.g. after receiving a message), use:

@code
    _wsse__Security *security = soap_wsse_Security(soap);
    if (security)
    { ... = security->SOAP_ENV__actor; // SOAP 1.1
      ... = security->SOAP_ENV__role;  // SOAP 1.2
@endcode

The env:mustUnderstand attribute is automatically added and checked by the
gSOAP engine. A gSOAP application compiled without Security support will reject
Security headers.

Security header blocks are attached to the soap context, which means that the
information will be automatically kept to support multiple invocations.

@section wsse_6 Security Tokens

The material in this section relates to the WS-Security specification section 6.

@subsection wsse_6_2 User Name Token

To add a user name token to the Security header block, use:

@code
    soap_wsse_add_UsernameTokenText(soap, "Id", "username", NULL);
@endcode

The Id value is optional. When non-NULL the user name token is included in the
digital signature to protect its integrity. It is common for the wsse plugin
functions to accept such Ids, which are serialized as wsu:Id identifiers for
cross-referencing XML elements. The signature engine of the wsse plugin is
designed to automatically sign all wsu:Id attributed elements to simplify the
code you need to write to implement the signing process.

To add a user name token with clear text password, use:

@code
    soap_wsse_add_UsernameTokenText(soap, "Id", "username", "password");
@endcode

It is strongly recommended to use @ref soap_wsse_add_UsernameTokenText only in
combination with HTTPS encrypted transmission or not at all. A better
alternative is to use password digests. With password digest authentication,
the digest value of a password (with message creation time and a random nonce)
is compared on both sides, thus eliminating the need to exchange a password
over the wire.

To add a user name token with password digest, use:

@code
    soap_wsse_add_UsernameTokenDigest(soap, "Id", "username", "password");
@endcode

Although the password string is passed to this function, it is not rendered in
XML or stored in a message log. It has been argued that this approach adopted
by the WS-Security protocol is still vulnerable since the application retrieves
the password in text form requiring a database to store passwords in clear
text. However, a digest algorithm can be used to hash the passwords and store
their digests instead, which eliminates the need to store clear-text passwords.
Note that this is a common approach adopted by Unix for decades.

By setting the Id value to a unique string, the user name token is also
digitally signed by the signature engine further preventing tampering with its
value.

You must use @ref soap_wsse_add_UsernameTokenDigest for each message exchange
to refresh the password digest even when the user name and password are not
changed. Otherwise, the receiver might flag the message as a replay attack.

Clear-text passwords and password digests are verified with
@ref soap_wsse_verify_Password. To verify a password at the receiving side to
authorize a request (e.g. within a Web service operation), use:

@code
    int ns__myMethod(struct soap *soap, ...)
    { const char *username = soap_wsse_get_Username(soap);
      const char *password;
      if (!username)
        return soap->error; // no username: return FailedAuthentication
      password = ...; // lookup password of username
      if (soap_wsse_verify_Password(soap, password))
        return soap->error; // password verification failed: return FailedAuthentication
      ... // process request, then sign the response message:
      if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
       || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
       || soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0)
        return soap->error;
      return SOAP_OK;
    }
@endcode

Note that the @ref soap_wsse_get_Username functions sets the
wsse:FailedAuthentication fault. It is common for the wsse plugin functions to
return SOAP_OK or a wsse fault that should be passed to the sender by returning
soap->error from service operations. The fault is displayed with the
soap_print_fault() function.

Password digest authentication prevents message replay attacks. The wsse plugin
keeps a database of password digests to thwart replay attacks. This is the
only part in the plugin code that requires mutex provided by threads.h.  Of
course, this only works correctly if the server is persistent, such as a
stand-alone service. Note that CGI-based services do not keep state. Machine
clocks must be synchronized and clock skew should not exceed @ref
SOAP_WSSE_CLKSKEW at the server side.

@subsection wsse_6_3 Binary Security Tokens

X509 certificates are commonly included in Security header blocks as binary
security tokens. A certificate is used to verify the digital signature of a
digitally signed message using the public key embedded within the certificate.
The certificate itself is signed by a certificate authority (CA) that vouches
for the authenticity of the certificate, i.e. to prove the identify of the
message originator. This verification process is important, because digital
signatures are useless without verification: an attacker could simply replace
the message, sign it, and replace the certificate.

Certificates are automatically verified by the wsse plugin signature engine
when received and accessed, which means that the certificates of the CAs must
be made accessible to the wsse plugin as follows:

@code
    soap->cafile = "cacerts.pem";  // use this
    soap->capath = "dir/to/certs"; // and/or point to CA certs
    soap->crlfile = "revoked.pem"; // use CRL (optional)
@endcode

The @ref soap_wsse_verify_X509 function checks the validity of a certificate.
The check is automatically performed. The check is also performed when
retrieving the certificate from a Security header block, either automatically
by the wsse plugin's signature verification engine or manually as follows:

@code
    X509 *cert = soap_wsse_get_BinarySecurityTokenX509(soap, "Id");
@endcode

where Id is the identification string of the binary security token or NULL.

The verification is an expensive process that will be optimized in future
releases by caching the certificate chain.

To attach a binary security token stored in a PEM file to a Security header
block for transmission, use:

@code
    soap_wsse_add_BinarySecurityTokenPEM(soap, NULL, "mycert.pem")
@endcode

A binary security token can be automatically signed by setting its Id
attribute:

@code
    soap_wsse_add_BinarySecurityTokenPEM(soap, "X509Token", "mycert.pem")
@endcode

Repeatedly loading a certificate from a PEM file is inefficient. To reuse a
certificate loaded from a PEM file for multiple invocations, use:

@code
    FILE *fd = fopen("mycert.pem", "r");
    X509 *cert = PEM_read_X509(fd, NULL, NULL, NULL);
    fclose(fd);
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert))
      ... // an error occurred
@endcode

Other types of binary security tokens can be added to the Security header block using:

@code
    soap_wsse_add_BinarySecurityToken(soap, "Id", "valueType", data, datalen);
@endcode

@section wsse_6_4 XML Tokens

The use and processing rules for XML tokens such as SAML assertions is specific
to an application.  The wsse plugin does not automate the use of XML tokens.
The developer is encouraged to generate code for the SAML schema with wsdl2h
and add the necessary assertions to the Security header block:

@code
typedef struct _wsse__Security
{       struct _wsu__Timestamp*                 wsu__Timestamp;
        struct _wsse__UsernameToken*            UsernameToken;
        struct _wsse__BinarySecurityToken*      BinarySecurityToken;
        struct _saml__Assertion*		saml__Assertion; // added
	struct xenc__EncryptedKeyType*          xenc__EncryptedKey;
        struct ds__SignatureType*               ds__Signature;
        @char*                                  SOAP_ENV__actor;
        @char*                                  SOAP_ENV__role;
} _wsse__Security;
@endcode

Alternatively, a DOM can be used to store and retrieve XML tokens:

@code
#import "dom.h"
typedef struct _wsse__Security
{       struct _wsu__Timestamp*                 wsu__Timestamp;
        struct _wsse__UsernameToken*            UsernameToken;
        struct _wsse__BinarySecurityToken*      BinarySecurityToken;
	struct xenc__EncryptedKeyType*          xenc__EncryptedKey;
        struct ds__SignatureType*               ds__Signature;
        int					__size;
        xsd__anyType*				any;
        @char*                                  SOAP_ENV__actor;
        @char*                                  SOAP_ENV__role;
} _wsse__Security;
@endcode

@section wsse_7 Token References

The material in this section relates to the WS-Security specification section 7.

To use a certificate for signature verification, add a direct security token
reference URI for the token to the KeyInfo, for example:

@code
    soap_wsse_add_KeyInfo_SecurityTokenReferenceURI(soap, "URI", "valueType");
@endcode

and:

@code
    soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "URI");
@endcode

For X509 certificates we use this to add a binary security token with the
certificate and a reference to the local token:

@code
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token"))
      ... // an error occurred
@endcode

This follows the recommended practice to place Security token references in
the KeyInfo element of a Signature. The KeyInfo is used to verify the validity
of a signature value.

Key identifiers can be used as well:

@code
    soap_wsse_add_KeyInfo_SecurityTokenReferenceKeyIdentifier(soap, "Id", "valueType", data, datalen);
@endcode

Embedded references are added with:

@code
    soap_wsse_add_KeyInfo_SecurityTokenReferenceEmbedded(soap, "Id", "valueType");
@endcode

Full support for embedded references requires coding to add tokens and
assertions, as well as to consume embedded references at the receiving side.
There is no automated mechanism to take the embedded references and process
them accordingly.

The use of key names is not recommended, but in case they are required they can
be added with:

@code
    soap_wsse_add_KeyInfo_KeyName(soap, "name");
@endcode

@section wsse_8 Signatures

The material in this section relates to the WS-Security specification section 8.

The wsse plugin must be registered to sign and verify messages:

@code
    soap_register_plugin(soap, soap_wsse);
@endcode

XML signatures are usually computed over normalized XML (to ensure the XML
processors of intermediate nodes can accurately reproduce the XML). To this
end, the exclusive canonical XML standard (exc-c14n) is required, which is set
using the SOAP_XML_CANONICAL flag:

@code
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL);
    soap_register_plugin(soap, soap_wsse);
@endcode

If you prefer XML indentation, use:

@code
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
@endcode

Other flags to consider:

- SOAP_IO_CHUNK for HTTP chunked content to stream messages.
- SOAP_ENC_GZIP for HTTP compression (also enables HTTP chunking).

Next, we decide which signature algorithm is appropriate to use:
- HMAC-SHA1 uses a secret key (also known as a shared key in symmetric
  cryptography) to sign the SHA1 digest of the SignedInfo element.
- DSA-SHA1 uses a DSA private key to sign the SHA1 digest of the SignedInfo
  element.
- RSA-SHA1 uses a RSA private key to sign the SHA1 digest of the SignedInfo
  element.

HMAC-SHA1 is the simplest method, but relies on the fact that you have to make
absolutely sure the key is kept secret on both the sending and receiving side.
As long as the secret key is confidential, messages are securely signed.
However, this is virtually impossible when exchanging messages with untrusted
disparate parties. The advantage of HMAC-SHA1 is the speed by which messages
are signed and verified.

DSA-SHA1 and RSA-SHA1 rely on public key cryptography. In simplified terms, a
message is signed using the (confidential!) private key. The public key is used
to verify the signature. Since only the originating party could have used its
private key to sign the message, the integrity of the message is guaranteed. Of
course, we must trust the public key came from the originator (it is often
included as an X509 certificate in the message). To this end, a trusted
certificate authority should have signed the public key, thereby creating a
X509 certificate that contains the public key and the identity of the message
originator.

An optional callback function can be passed to the plugin that is responsible
for providing a certificate to the wsse engine to verify a signed message. For
example, when a security token is absent from an DSA-SHA1 or RSA-SHA1 signed
message then the only mechanism to automatically verify the signature is to let
the callback produce a certificate:

@code
    soap_register_plugin_arg(soap, soap_wsse, security_token_handler);

    const void *security_token_handler(struct soap *soap, int alg, const char *keyname, int *keylen)
    { // Note: 'keyname' argument is only used with shared secret key decryption
      // Get the user name from UsernameToken in message
      const char *uid = soap_wsse_get_Username(soap);
      switch (alg)
      { case SOAP_SMD_VRFY_DSA_SHA1:
        case SOAP_SMD_VRFY_RSA_SHA1:
          if (uid)
          { // Lookup uid to retrieve the X509 certificate to verify the signature
            const X509 *cert = ...; 
            return (const void*)cert;
          }
          return NULL; // no certificate: fail
        case SOAP_SMD_HMAC_SHA1:
          if (uid)
          { // Lookup uid to retrieve the HMAC key to verify the signature
            const void *key = ...; 
	    *keylen = ...;
            return key;
          }
          return NULL; // no certificate: fail
        case SOAP_MEC_ENV_DEC_DES_CBC:
        case SOAP_MEC_ENV_DEC_AES128:
        case SOAP_MEC_ENV_DEC_AES256:
        case SOAP_MEC_ENV_DEC_AES512:
	  // return decryption private key associated with keyname
        case SOAP_MEC_DEC_DES_CBC:
        case SOAP_MEC_DEC_AES128:
        case SOAP_MEC_DEC_AES256:
        case SOAP_MEC_DEC_AES512:
	  // *keylen = ...
	  // return decryption shared secret key associated with keyname
      }
      return NULL; // fail
    }
@endcode

@subsection wsse_8_2a Signing Messages

After the plugin is registered and a signature algorithm selected, the
@ref soap_wsse_sign function or the @ref soap_wsse_sign_body function is used
to initiate the signature engine to automatically sign outbound messages.

The code to sign the SOAP Body of a message using HMAC-SHA1 is:

@code
    static char hmac_key[16] =
    { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    if (soap_wsse_sign_body(soap, SOAP_SMD_HMAC_SHA1, hmac_key, sizeof(hmac_key))
      ... // an error occurred
    else if (soap_call_ns__myMethod(soap, ...))
      ... // a transmission error occurred
@endcode

The hmac_key is some secret key you generated for the sending side and
receiving side (don't use the one shown here).

As always, use soap_print_fault() to display the error message.

To sign the body of an outbound SOAP message using RSA-SHA1 (DSA-SHA1 is
similar), we include the X509 certificate with the public key as a
BinarySecurityToken in the header and a KeyInfo reference to the token to let
receivers use the public key in the certificate to verify the authenticity of
the message:

@code
    FILE *fd;
    EVP_PKEY *rsa_private_key;
    X509 *cert;
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    fd = fopen("privkey.pem", "r");
    rsa_private_key = PEM_read_PrivateKey(fd, NULL, NULL, "password");
    fclose(fd);
    fd = fopen("cert.pem", "r");
    X509 *cert = PEM_read_X509(fd, NULL, NULL, NULL);
    fclose(fd);
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
     || soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0))
      ... // an error occurred
    else if (soap_call_ns__myMethod(soap, ...))
      ... // a transmission error occurred
@endcode

The private key and its certificate are often placed in the same file, see e.g.
server.pem in the package.

To summarize the signing process:
-# Register the wsse plugin.
-# Obtain an HMAC secret key or a DSA or RSA private key.
-# For DSA or RSA, obtain the X509 certificate with the public key signed by a
   certificate authority.
-# Add the X509 certificate as a BinarySecurityToken to the header.
-# Add a KeyInfo BinarySecurityTokenReference.
-# Invoke @ref soap_wsse_sign or @ref soap_wsse_sign_body to sign the message.
-# Always check the function return values for errors. You don't want to
   produce and accept messages with an invalid Security headers.

@subsection wsse_8_2b Signing Message Parts

The @ref soap_wsse_sign_body function signs the entire SOAP body. If it is
desirable to sign individual parts of a message the @ref soap_wsse_sign
function should be used. All message parts with wsu:Id attributes are signed.
These message parts should not be nested (nested elements will not be
separately signed). By default, all and only those XML elements with wsu:Id
attributes are signed. Therefore, the wsu:Id attribute values used in a message
must be unique within the message. Although usually not required, the default
signing rule can be overridden with the @ref soap_wsse_sign_only function, see
@ref wsse_8_3.

For example, consider a transaction in which we only want to sign a contract in
the SOAP Body. This allows us to modify the rest of the message or extract the
contract in XML and pass it on with the signature.

The gSOAP header file includes a myContract declaration:

@code
    struct ns__myContract
    { @char* wsu__Id = "Contract";
      char* name;
      char* title;
      char* terms;
    };
    int ns__myMethod(struct ns__myContract agreement, bool* accepted);
@endcode

The default value of the wsu:Id is "Contract" so that we can instantiate the
struct, automatically sign it, and send it as follows:

@code
    struct ns__myContract contract;
    bool accept;
    soap_default_ns__myContract(soap, &contract);
    contract.name = ...;
    contract.title = ...;
    contract.terms = ...;
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
     || soap_wsse_sign(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0))
      ... // an error occurred
    else if (soap_call_ns__myMethod(soap, contract, &accept))
      ... // a transmission error occurred
@endcode

The above example shows a wsu:Id attribute embedded (hardcoded) in a struct.
When it is not possible to add the wsu__Id member, for example when the type is
a string instead of a struct, it is suggested to specify the XML element to be
signed with the @ref soap_wsse_set_wsu_id(soap, "space-separated string of
element names") function. Use it before each call or in the server operation
(when returning XML data from a service operation). This lets the engine add
wsu:Id="tag" attribute-value pair to the element's tag name. For example:

@code
    soap_wsse_set_wsu_id(soap, "ns:myContract"); // <ns:myContract wsu:Id="ns:myContract">...
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
     || soap_wsse_sign(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0))
      ... // an error occurred
    soap_wsse_set_wsu_id(soap, NULL);
@endcode

This code adds the wsu:Id="ns-myContract" to the ns:myContract element. Here,
the wsu__Id value in the struct MUST NOT be set. Otherwise, two wsu:Id
attributes are present which is invalid. Also, the element signed must be
unique in the message. That is, there cannot be more than one matching element,
otherwise the resulting signature is invalid.

Note: to reset the automatic wsu:Id attributes addition, pass NULL to
@ref soap_wsse_set_wsu_id as shown above. This is automatically performed when
a new message is received (but not automatically in a sequence of one-way sends
for example).

WARNING:
When signing parts of the body as outlined above, use @ref soap_wsse_sign
(do NOT use @ref soap_wsse_sign_body).

WARNING:
Do not attempt to sign an element with a wsu:Id that is a subelement of another
element with a wsu:Id, that is, do not sign inner nested wsu:Id elements. The
element that you will try to sign will not be canonicalized and will lead to a
failure of the signature verification. When elements with wsu:Id are nested,
sign the outermost element.

We recommend to sign the entire SOAP Body using soap_wsse_sign_body and
reserve the use of soap_wsse_set_wsu_id for SOAP Header elements, such as
WS-Addressing elements. For example:

@code
    #include "wsaapi.h"
    ...
    soap_wsa_request(soap, RequestMessageID, ToAddress, RequestAction);
    soap_wsse_set_wsu_id(soap, "wsa5:To wsa5:From wsa5:ReplyTo wsa5:Action");
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
     || soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0))
      ... // an error occurred
    else if (soap_call_ns__myMethod(soap, ...))
      ... // a transmission error occurred
    soap_wsse_set_wsu_id(soap, NULL);
@endcode

This code signs the wsa5:To and wsa5:Action SOAP header elements (set with
soap_wsa_request, see the WS-Addressing "wsa" API in the gSOAP documentation
for more information on the use of WS-Addressing). It is fine to specify more
elements with @ref soap_wsse_set_wsu_id than actually present in the XML
payload. The other WS-Addressing headers are not present and are not signed.

WARNING:
@ref soap_wsse_set_wsu_id should only be set once for each @ref soap_wsse_sign
or @ref soap_wsse_sign_body. Each new call overrides the previous setting.

WARNING:
Never use @ref soap_wsse_set_wsu_id to set the wsu:Id for an element that
occurs more than once in the payload, since each will have the same wsu:Id
attribute that may lead to a WS-Signature failure.

@subsection wsse_8_3 Signing Tokens

To sign security tokens such as user names, passwords, and binary security
tokens, just assign their Id values with a unique string, such as "Time" for
timestamps and "User" for user names. For example:

@code
    soap_wsse_add_Timestamp(soap, "Time", 600);
    soap_wsse_add_UsernameTokenDigest(soap, "User", "username", "password");
    ... // the rest of the signing code
@endcode

Note that by default all wsu:Id-attributed elements are signed. To filter a
subset of wsu:Id-attributed elements for signatures, use the
@ref soap_wsse_sign_only function as follows:

@code
    soap_wsse_add_UsernameTokenDigest(soap, "User", "username", "password");
    soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert);
    soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token");
    soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0);
    soap_wsse_sign_only(soap, "User Body");
@endcode

Note that in the above we MUST set the X509Token name for cross-referencing
with a wsu:Id, which normally results in automatically signing that token
unless filtered out with @ref soap_wsse_sign_only. The SOAP Body wsu:Id is
always "Body" and should be part of the @ref soap_wsse_sign_only set of wsu:Id
names to sign.

When using @ref soap_wsse_set_wsu_id we need to use the tag name with
@ref soap_wsse_sign_only. For example:

@code
    soap_wsa_request(soap, RequestMessageID, ToAddress, RequestAction);
    soap_wsse_set_wsu_id(soap, "wsa5:To wsa5:From wsa5:ReplyTo wsa5:Action");
    soap_wsse_add_UsernameTokenDigest(soap, "User", "username", "password");
    soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert);
    soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token");
    soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0);
    soap_wsse_sign_only(soap, "wsa5:To wsa5:From wsa5:ReplyTo wsa5:Action User Body");
@endcode

Note: @ref soap_wsse_sign_only should only be set once for each @ref
soap_wsse_sign or @ref soap_wsse_sign_body. Each new call overrides the
previous.

Note: to reset the filtering of signed tokens and elements, pass NULL to
@ref soap_wsse_sign_only. This is automatically performed when a new message is
received (but not automatically in a sequence of one-way sends for example).

@subsection wsse_8_4 Signature Validation

To automatically verify the signature of an inbound message signed with DSA or
RSA algorithms, assuming the message contains the X509 certificate as a binary
security token, use:

@code
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    soap_wsse_verify_auto(soap, SOAP_SMD_NONE, NULL, 0);
    soap->cafile = "cacerts.pem";  // file with CA certs of peers
    soap->capath = "dir/to/certs"; // and/or point to CA certs
    soap->crlfile = "revoked.pem"; // use CRL (optional)

    // server:
    if (soap_serve(soap))
      ... // an error occurred

    // client:
    if (soap_call_ns__myMethod(soap, ...))
      ... // an error occurred
@endcode

All locally referenced and signed elements in the signed message will be
verified. Elements that are not signed cannot be verified. Also elements
referenced with absolute URIs that are not part of the message are not
automatically verified. The received message is stored in a DOM accessible with
soap->dom. This enables further analysis of the message content.

For a post-parsing check to verify if an XML element was signed in an inbound
message, use:
@code
    soap_wsse_verify_auto(soap, SOAP_SMD_NONE, NULL, 0);
    ... // client call
    if (soap_wsse_verify_element(soap, "namespaceURI", "tag") > 0)
      ... // at least one element with matching tag and namespace is signed
@endcode
The signed element nesting rules are obeyed, so if the matching element is a
descendent of a signed element, it is signed as well.

Because it is a post check, a client should invoke @ref soap_wsse_verify_element
after the call completed. A service should invoke this function within the
service operation routine, i.e. when the message request is accepted and about
to be processed.

For example, to check whether the wsu:Timestamp element was signed (assuming it is present and message expiration checked with @ref soap_wsse_verify_Timestamp), use @ref soap_wsse_verify_element(soap, "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd", "Timestamp").

To check the SOAP Body (either using SOAP 1.1 or 1.2), simply use @ref
soap_wsse_verify_body.

The @ref soap_wsse_verify_auto function keeps processing signed (and unsigned)
messages as they arrive. For unsigned messages this can be expensive and the
verification engine should be shut down using @ref soap_wsse_verify_done.

To verify the HMAC signature of an inbound message, the HMAC key must be
supplied:

@code
    static char hmac_key[16] = // the same secret key that was used to sign
    { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    soap_wsse_verify_auto(soap, SOAP_SMD_HMAC_SHA1, hmac_key, sizeof(hmac_key));

    // server:
    if (soap_serve(soap))
      ... // an error occurred

    // client:
    if (soap_call_ns__myMethod(soap, ...))
      ... // an error occurred
@endcode

To summarize the signature verification process:
-# Register the wsse plugin.
-# For HMAC, obtain the HMAC secret key
-# Use @ref soap_wsse_verify_auto to verify inbound messages.
-# Set the cafile (or capath) to verify certificates of the peers and crlfile
   (optional)
-# After receiving a message, the DOM in soap->dom can be traversed for further    analysis.
-# Always check the function return values for errors. You don't want to accept
   a request or response message with an invalid Security header.
-# Use @ref soap_wsse_verify_done to terminate verification, e.g. to consume
   plain messages more efficiently.

@section wsse_9 Encryption

The material in this section relates to the WS-Security specification section 9.

The wsse plugin must be registered:

@code
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
@endcode

Other flags to consider:

- SOAP_IO_CHUNK for HTTP chunked content to stream messages.
- SOAP_ENC_GZIP for HTTP compression (also enables HTTP chunking).

@subsection wsse_9_1 Encrypting Messages

Encryption should be used in combination with signing. A signature ensures
message integrity while encryption ensures confidentially. Encrypted messages
can be tampered with unless integrity is ensured. Therefore, Section
@ref wsse_8 should be followed to sign and verify message content.

Messages are encrypted using either public key cryptography or a symmetric
secret key. A symmetric secret key should only be shared between the sender and
receiver (or any trusted communicating peer).

Encryption with public key cryptography uses an "envelope" process, where the
public key of the recipient is used to encrypt a temporary (ephemeral) secret
key that is sent together with the secret key-encrypted message to the
recipient. The recipient decrypts the ephemeral key and uses it to decrypt the
message. The public key is usually part of a X509 certificate. The public key
(containing the subject information) is added to the WS-Security header and
used for encryption of the SOAP Body as follows:

@code
    X509 *cert = ...; //
    if (soap_wsse_add_EncryptedKey(soap, "Cert", cert, NULL))
      soap_print_fault(soap, stderr);
@endcode

The "Cert" parameter is a unique URI to reference the key from the encrypted
SOAP Body. The above enables the encryption engine for the next message to be
sent, either at the client or server side. The server should use this withing a
server operation (before returning) to enable the service operation response to
be encrypted.

To include a subject key ID in the WS-Security header instead of the entire
public key, specify the subject key ID parameter:

@code
    X509 *cert = ...; //
    if (soap_wsse_add_EncryptedKey(soap, "Cert", cert, "Subject Key ID"))
      soap_print_fault(soap, stderr);
@endcode

The difference with the previous example where no subject key ID was specified
is that the WS-Security header only contains the subject key ID and no longer
the public key in base64 format.

To encrypt specific elements of the SOAP Body rather than the entire SOAP Body,
use @ref soap_wsse_add_EncryptedKey_encrypt_only in combination with
@ref soap_wsse_set_wsu_id as follows:

@code
    X509 *cert = ...; //
    // the SOAP Body contains one <ns:myContract> and one <ns:myPIN> (not nested)
    soap_wsse_set_wsu_id(soap, "ns:myContract ns:myPIN");
    if (soap_wsse_add_EncryptedKey_encrypt_only(soap, "Cert", cert, "Subject Key ID", "ns:myContract ns:myPIN"))
      soap_print_fault(soap, stderr);
@endcode

WARNING:
- The @ref soap_wsse_set_wsu_id MUST be used to specify all element tag names
  to encrypt. Additional elements MAY be specified in @ref soap_wsse_set_wsu_id
  (for example elements to digitally sign).
- The elements to encrypt MUST occur EXACTLY ONCE in the SOAP Body.

For symmetric encryption with a shared secret key, generate a 160-bit triple
DES key and make sure both the sender and reciever can use the key without it
being shared by any other party (key exchange problem). Then use the
@ref soap_wsse_encrypt_body function to encrypt the SOAP Body as follows:

@code
    char des_key[20] = ...; // 20-byte (160-bit) DES shared secret key
    if (soap_wsse_encrypt_body(soap, SOAP_MEC_ENC_DES_CBC, des_key, sizeof(des_key)))
      soap_print_fault(soap, stderr);
@endcode

Triple DES can be selected as above, but also AES128, AES256, or AES512, for
example:

@code
    char aes256_key[32] = ...; // 32-byte (256-bit) AES256 shared secret key
    if (soap_wsse_encrypt_body(soap, SOAP_MEC_ENC_AES256, aes256_key, sizeof(aes256_key)))
      soap_print_fault(soap, stderr);
@endcode

To symmetrically encrypt specific elements of the SOAP Body rather than the
entire SOAP Body, use @ref soap_wsse_encrypt_only in combination with
@ref soap_wsse_set_wsu_id as follows:

@code
    char des_key[20] = ...; // 20-byte (160-bit) secret key
    // the SOAP Body contains one <ns:myContract> and one <ns:myPIN> (not nested)
    soap_wsse_set_wsu_id(soap, "ns:myContract ns:myPIN");
    if (soap_wsse_encrypt_only(soap, SOAP_MEC_ENC_DES_CBC, des_key, sizeof(des_key), "ns:myContract ns:myPIN"))
      soap_print_fault(soap, stderr);
@endcode

WARNING:
- The @ref soap_wsse_set_wsu_id MUST be used to specify all element tag names
  to encrypt. Additional elements MAY be specified in @ref soap_wsse_set_wsu_id
  (for example elements to digitally sign).
- The elements to encrypt MUST occur EXACTLY ONCE in the SOAP Body.

@subsection wsse_9_2 Decrypting Message Parts

The wsse engine automatically decrypts message parts, but requires a private
key or secret shared key to do so. A default key can be given to enable
decryption, but it will fail if a non-compatible key was used for encryption.
In that case a token handler callback should be defined by the user to select a
proper decryption key based on the available subject key name or identifier
embedded in the encrypted message.

Here is an example of a token handler callback:

@code
    soap_register_plugin_arg(soap, soap_wsse, security_token_handler);

    const void *security_token_handler(struct soap *soap, int alg, const char *keyname, int *keylen)
    { // Note: 'keyname' argument is only used with shared secret key decryption
      // Get the user name from UsernameToken in message
      const char *uid = soap_wsse_get_Username(soap);
      switch (alg)
      { case SOAP_SMD_VRFY_DSA_SHA1:
        case SOAP_SMD_VRFY_RSA_SHA1:
          if (uid)
          { // Lookup uid to retrieve the X509 certificate to verify the signature
            const X509 *cert = ...; 
            return (const void*)cert;
          }
          return NULL; // no certificate: fail
        case SOAP_SMD_HMAC_SHA1:
          if (uid)
          { // Lookup uid to retrieve the HMAC key to verify the signature
            const void *key = ...; 
	    *keylen = ...;
            return key;
          }
          return NULL; // no certificate: fail
        case SOAP_MEC_ENV_DEC_DES_CBC
        case SOAP_MEC_ENV_DEC_AES128
        case SOAP_MEC_ENV_DEC_AES256
        case SOAP_MEC_ENV_DEC_AES256
	  // return decryption private key associated with keyname
        case SOAP_MEC_DEC_DES_CBC
        case SOAP_MEC_DEC_AES128
        case SOAP_MEC_DEC_AES256
        case SOAP_MEC_DEC_AES512
	  // *keylen = ...
	  // return decryption shared secret key associated with keyname
      }
      return NULL; // fail
    }
@endcode

The last two arms are used to return a private key associated with the keyname
paramater, which is a string that contains the subject key id from the public
key information in an encrypted message or the subject key ID string that was
set with @ref soap_wsse_add_EncryptedKey at the sender side.

To set the default private key for envelope decryption, use:

@code
    EVP_PKEY *rsa_private_key = ...;
    soap_wsse_decrypt_auto(soap, SOAP_MEC_ENV_DEC_DES_CBC, rsa_private_key, 0);
@endcode

Or to set the default shared secret key for symmetric decryption, use:

@code
    char des_key[20] = ...; // 20-byte (160-bit) triple DES key
    soap_wsse_decrypt_auto(soap, SOAP_MEC_DEC_DES_CBC, des_key, sizeof(des_key));
@endcode

The above assumes that triple DES is used, but you can select AES128, AES256,
or AES512:

@code
    char aes256_key[32] = ...; // 32-byte (256-bit) AES256 key
    soap_wsse_decrypt_auto(soap, SOAP_MEC_DEC_AES256, aes256_key, sizeof(aes256_key));
@endcode

If a default key is not set, the token handler callback should be used as
discussed above in this section. Do NOT set a default key if a token handler is
used to handle multiple different keys. The default key mechanism is simpler to
use when only one decryption key is used to decrypt all encrypted messages.

To remove the default key, use:

@code
    soap_wsse_decrypt_auto(soap, SOAP_MEC_NONE, NULL, 0);
@endcode

@subsection wsse_9_3 Example Combining Signing with Encryption/Decryption

Here is an client-side example to use signatures and encryption for the
outbound service request message and verification and decryption of the inbound
response message:

@code
    FILE *fd;
    EVP_PKEY *rsa_private_key;
    X509 *cert;
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    soap_wsse_verify_auto(soap, SOAP_SMD_NONE, NULL, 0);
    fd = fopen("privkey.pem", "r");
    rsa_private_key = PEM_read_PrivateKey(fd, NULL, NULL, "password");
    fclose(fd);
    soap_wsse_decrypt_auto(soap, SOAP_MEC_ENV_DEC_DES_CBC, rsa_private_key, 0);
    fd = fopen("cert.pem", "r");
    X509 *cert = PEM_read_X509(fd, NULL, NULL, NULL);
    fclose(fd);
    if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
     || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
     || soap_wsse_add_EncryptedKey(soap, "Cert", cert, NULL)
     || soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0)
      ... // an error occurred
    else if (soap_call_ns__myMethod(soap, ...))
      ... // a transmission error occurred
    ...
    EVP_PKEY_free(rsa_private_key);
    X509_free(cert);
@endcode

The server-side service operation is as follows:

@code
    FILE *fd;
    EVP_PKEY *rsa_private_key;
    X509 *cert;
    struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    soap_wsse_verify_auto(soap, SOAP_SMD_NONE, NULL, 0);
    fd = fopen("privkey.pem", "r");
    rsa_private_key = PEM_read_PrivateKey(fd, NULL, NULL, "password");
    fclose(fd);
    soap_wsse_decrypt_auto(soap, SOAP_MEC_ENV_DEC_DES_CBC, rsa_private_key, 0);
    fd = fopen("cert.pem", "r");
    X509 *cert = PEM_read_X509(fd, NULL, NULL, NULL);
    fclose(fd);
    ...
    if (soap_serve(soap))
    { soap_wsse_delete_Security(soap);
      soap_print_fault(soap, stderr);
    }
    ...
    EVP_PKEY_free(rsa_private_key);
    X509_free(cert);
@endcode

where an example service operation could be:

@code
    int ns__myMethod(struct soap *soap, ...)
    { ...
      soap_wsse_delete_Security(soap);
      if (soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert)
       || soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token")
       || soap_wsse_add_EncryptedKey(soap, "Cert", cert, NULL)
       || soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_private_key, 0)
        return soap->error;
      return SOAP_OK;
    }
@endcode

The service operation signs the message using a private key and encrypts the
response message using a public key (from the certificate).

@section wsse_10 Security Timestamps

The material in this section relates to the WS-Security specification section
10.

To add a timestamp with the creation time to the Security header, use:

@code
    soap_wsse_add_Timestamp(soap, NULL, 0); // no expiration
@endcode

The lifetime of a message (in seconds) is passed as the third argument, which
will be displayed as the timestamp expiration time:

@code
    soap_wsse_add_Timestamp(soap, NULL, 10); // 10 seconds lifetime
@endcode

Timestamps, like other header elements, are not automatically secured with a
digital signature. To secure a timestamp, we add an identifier (wsu:Id) to each
element we want the WS-Security plugin to sign thereby making it impossible for
someone to tamper with that part of the message. To do this for the timestamp,
we simply pass a unique identification string as the second argument:

@code
    soap_wsse_add_Timestamp(soap, "Time", 10); // timestamp will be signed
@endcode

@section wsse_11 WS-Security and HTTPS

HTTPS is used at the client side with the usual "https:" URL addressing, shown
here with the registration of the wsse plugin and setting up locks for
thread-safe use of SSL for HTTPS:

@code
    #include "wsseapi.h"
    #include "threads.h"
    struct soap *soap;
    if (CRYPTO_thread_setup())
      ... // error
    soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    if (soap_ssl_client_context(&soap,
      SOAP_SSL_DEFAULT, // requires server authentication
      NULL,             // keyfile for client authentication to server
      NULL,             // the keyfile password
      "cacerts.pem",    // cafile CA certificates to authenticate the server
      NULL,             // capath CA directory path to certificates
      NULL
    ))
      ... // error
    soap->cafile = "cacerts.pem";  // same as above (or overrides the above)
    soap->capath = "dir/to/certs"; // and/or point to CA certs
    soap->crlfile = "revoked.pem"; // use CRL (optional)
    ... // set up WS-Security for signatures/encryption etc
    if (soap_call_ns__myMethod(soap, "https://...", ...))
      ... // error
    ... // process response results
    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    CRYPTO_thread_cleanup();
@endcode

The CRYPTO threads should be set up before any threads are created.

The soap_ssl_client_context only needs to be set up once. Use the following
flags:

- SOAP_SSL_DEFAULT requires server authentication, CA certs should be used
- SOAP_SSL_NO_AUTHENTICATION disables server authentication
- SOAP_SSL_SKIP_HOST_CHECK disables server authentication host check

The server uses the following:

@code
    #include "wsseapi.h"
    #include "threads.h"
    SOAP_SOCKET m, s;
    int port = 443;
    struct soap *soap;
    if (CRYPTO_thread_setup())
      ... // error
    soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
    soap_register_plugin(soap, soap_wsse);
    if (soap_ssl_server_context(&soap,
      SOAP_SSL_DEFAULT, // requires server to authenticate, but not the client
      server.pem,       // keyfile for authentication to client
      "password",       // the keyfile password
      NULL,             // CA certificates to authenticate the client
      NULL,             // CA directory path to certificates
      NULL,             // use RSA 2048 bits (or give file name with DH param)
      NULL,
      NULL
    ))
      ... // error
    if (!soap_valid_socket(m = soap_bind(soap, NULL, port, 100))
      ... // error
    for (;;)
    { if (!soap_valid_socket(s = soap_accept(soap)))
        ... // error
      THREAD_CREATE(&tid, (void*(*)(void*))&process_request, soap_copy(soap));
    }
    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    CRYPTO_thread_cleanup();
@endcode

where we define a process_request function that is executed by the thread to
process the request (on a copy of the soap context struct):

@code
  void *process_request(struct soap *soap)
  { ... // set up WS-Security for signatures/encryption etc
    if (soap_ssl_accept(soap)
     || soap_serve(soap))
      ... // error
    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
  }
@endcode

The soap_ssl_server_context only needs to be set up once. Use the following
flags:

- SOAP_SSL_DEFAULT requires server authentication, but no client authentication
- SOAP_SSL_REQUIRE_CLIENT_AUTHENTICATION requires client authentication

We need to define the thread set up and clean up operations as follows:

@code
  struct CRYPTO_dynlock_value
  { MUTEX_TYPE mutex;
  };

  static MUTEX_TYPE *mutex_buf;

  static struct CRYPTO_dynlock_value *dyn_create_function(const char *file, int line)
  { struct CRYPTO_dynlock_value *value;
    value = (struct CRYPTO_dynlock_value*)malloc(sizeof(struct CRYPTO_dynlock_value));
    if (value)
      MUTEX_SETUP(value->mutex);
    return value;
  }

  static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line)
  { if (mode & CRYPTO_LOCK)
      MUTEX_LOCK(l->mutex);
    else
      MUTEX_UNLOCK(l->mutex);
  }

  static void dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line)
  { MUTEX_CLEANUP(l->mutex);
    free(l);
  }

  void locking_function(int mode, int n, const char *file, int line)
  { if (mode & CRYPTO_LOCK)
      MUTEX_LOCK(mutex_buf[n]);
    else
      MUTEX_UNLOCK(mutex_buf[n]);
  }

  unsigned long id_function()
  { return (unsigned long)THREAD_ID;
  }

  int CRYPTO_thread_setup()
  { int i;
    mutex_buf = (MUTEX_TYPE*)malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
    if (!mutex_buf)
      return SOAP_EOM;
    for (i = 0; i < CRYPTO_num_locks(); i++)
      MUTEX_SETUP(mutex_buf[i]);
    CRYPTO_set_id_callback(id_function);
    CRYPTO_set_locking_callback(locking_function);
    CRYPTO_set_dynlock_create_callback(dyn_create_function);
    CRYPTO_set_dynlock_lock_callback(dyn_lock_function);
    CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);
    return SOAP_OK;
  }

  void CRYPTO_thread_cleanup()
  { int i;
    if (!mutex_buf)
      return;
    CRYPTO_set_id_callback(NULL);
    CRYPTO_set_locking_callback(NULL);
    CRYPTO_set_dynlock_create_callback(NULL);
    CRYPTO_set_dynlock_lock_callback(NULL);
    CRYPTO_set_dynlock_destroy_callback(NULL);
    for (i = 0; i < CRYPTO_num_locks(); i++)
      MUTEX_CLEANUP(mutex_buf[i]);
    free(mutex_buf);
    mutex_buf = NULL;
  }
@endcode

For additional details and examples, see the user guide and examples in the
gSOAP package directory gsoap/samples/ssl.

@section wsse_12 Miscellaneous

The Security header block was generated from the WS-Security schema with the
wsdl2h tool and WS/WS-typemap.dat:

@code
    > wsdl2h -cegxy -o wsse.h -t WS/WS-typemap.dat WS/wsse.xsd
@endcode

The same process was used to generate the header file ds.h from the XML digital
signatures core schema, and the xenc.h encryption schema:

@code
    > wsdl2h -cuxy -o ds.h -t WS/WS-typemap.dat WS/ds.xsd
    > wsdl2h -cuxy -o xenc.h -t WS/WS-typemap.dat WS/xenc.xsd
@endcode

The import/wsse.h file has the following definition for the Security header
block:

@code
typedef struct _wsse__Security
{       struct _wsu__Timestamp*                 wsu__Timestamp;
        struct _wsse__UsernameToken*            UsernameToken;
        struct _wsse__BinarySecurityToken*      BinarySecurityToken;
        struct xenc__EncryptedKeyType*          xenc__EncryptedKey;
        struct _xenc__ReferenceList*            xenc__ReferenceList;
        struct ds__SignatureType*               ds__Signature;
        @char*                                  SOAP_ENV__actor;
        @char*                                  SOAP_ENV__role;
} _wsse__Security;
@endcode

The _wsse__Security header is modified by a WS/WS-typemap.dat mapping rule to
include additional details.

@section wsse_13 Encryption Limitations

- Individual encryption/decryption of simple content (CDATA content) with @ref
  soap_wsse_add_EncryptedKey_encrypt_only IS NOT SUPPORTED. Encrypt the entire
  SOAP Body or encrypt elements with complex content (complexType and
  complexContent elements that have sub elements).

- Encryption is performed after signing (likewise, signatures are verified
  after decryption). Signing after encryption is not supported.

*/
#include <stdio.h>
#include "wsseapi.h"
#include "smdevp.h"
//#include "mecevp.h"
#include "threads.h"	/* only need threads to enable mutex for MT */
#include "enx_gsoap_http.h"

#if defined(SOAP_WSA_2003) || defined(SOAP_WSA_2004) || defined(SOAP_WSA_200408) || defined(SOAP_WSA_2005)
//#include "wsaapi.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** Plugin identification for plugin registry */
const char soap_wsse_id[14] = SOAP_WSSE_ID;

/** Maximum number of SignedInfo References */
#define SOAP_WSSE_MAX_REF	(100)

/** Clock skew between machines (in sec) to fit message expiration in window */
#define SOAP_WSSE_CLKSKEW	(300)

/** Size of the random nonce */
#define SOAP_WSSE_NONCELEN	(20)
/** Digest authentication accepts messages that are not older than creation time + SOAP_WSSE_NONCETIME */
#define SOAP_WSSE_NONCETIME	(SOAP_WSSE_CLKSKEW + 240)

/******************************************************************************\
 *
 * Common URIs
 *
\******************************************************************************/

//const char *wsse_PasswordTextURI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText";
const char *wsse_PasswordDigestURI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
//const char *wsse_Base64BinaryURI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary";
//const char *wsse_X509v3URI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-x509-token-profile-1.0#X509v3";
//const char *wsse_X509v3SubjectKeyIdentifierURI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-x509-token-profile-1.0#X509SubjectKeyIdentifier";
//
//const char *ds_sha1URI = "http://www.w3.org/2000/09/xmldsig#sha1";
//const char *ds_hmac_sha1URI = "http://www.w3.org/2000/09/xmldsig#hmac-sha1";
//const char *ds_dsa_sha1URI = "http://www.w3.org/2000/09/xmldsig#dsa-sha1";
//const char *ds_rsa_sha1URI = "http://www.w3.org/2000/09/xmldsig#rsa-sha1";
//
//const char *xenc_rsa15URI = "http://www.w3.org/2001/04/xmlenc#rsa-1_5";
//const char *xenc_3desURI = "http://www.w3.org/2001/04/xmlenc#tripledes-cbc";
//
//const char *ds_URI = "http://www.w3.org/2000/09/xmldsig#";
//const char *c14n_URI = "http://www.w3.org/2001/10/xml-exc-c14n#";
//const char *wsu_URI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd";

/******************************************************************************\
 *
 * Digest authentication session
 *
\******************************************************************************/

/**
@struct soap_wsse_session
@brief Digest authentication session data
*/
struct soap_wsse_session
{ struct soap_wsse_session *next;	/**< Next session in list */
  time_t expired;			/**< Session expiration */
  char hash[SOAP_SMD_SHA1_SIZE];	/**< SHA1 digest */
  char nonce[1]; /**< Nonce string flows into region below this struct */
};

/** The digest authentication session database */
static struct soap_wsse_session *soap_wsse_session = NULL;
static int soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce);
static void soap_wsse_session_cleanup(struct soap *soap);
static void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE]);

/**
@fn void soap_wsse_delete_Security(struct soap *soap)
@brief Deletes Security header element.
@param soap context
*/
void
soap_wsse_delete_Security(struct soap *soap)
{ 
	dvprintf("soap_wsse_delete_Security\n");
	if(soap)
		if(soap->header)
			if(soap->header->wsse__Security)
				soap->header->wsse__Security = NULL;
}

/**
@fn _wsse__Security* soap_wsse_Security(struct soap *soap)
@brief Returns Security header element if present.
@param soap context
@return _wsse__Security object or NULL
*/
struct _wsse__Security*
soap_wsse_Security(struct soap *soap)
{ 
	if(soap->header)
		return soap->header->wsse__Security;
	return NULL;
}

/**
@fn int soap_wsse_CheckSecurity(struct soap *soap)
@brief Returns 1 if security passed.
@param soap context
@return 1 object or 0
*/
int
soap_wsse_CheckSecurity(struct soap *soap)
{ 	
	int ret = -1;
	dvprintf("soap_wsse_CheckSecurity\n");
	if(soap)
	{
		onvif_http_t *onvif_http = soap->user;
		if(onvif_http) {		
			if(soap->header)
				if(soap->header->wsse__Security)
				{		
					if(onvif_http->SecurityClear == -1)
						onvif_http->SecurityClear = 0;	
					dvprintf("soap_wsse_CheckSecurity %d\n",ret);
				}
			ret = onvif_http->SecurityClear;
		}
	}
	return ret;
}

/**
@fn void soap_wsse_ClearSecurity(struct soap *soap)
@param soap context
*/
void
soap_wsse_ClearSecurity(struct soap *soap)
{ 
	onvif_http_t *onvif_http = soap->user;
	if(onvif_http)
		onvif_http->SecurityClear = 1;
}

/**
@fn void soap_wsse_DefaultSecurity(struct soap *soap)
@param soap context
*/
void
soap_wsse_DefaultSecurity(struct soap *soap)
{ 
	onvif_http_t *onvif_http = soap->user;
	onvif_http->SecurityClear = -1;
}

/**
@fn _wsse__UsernameToken* soap_wsse_UsernameToken(struct soap *soap, const char *id)
@brief Returns UsernameToken element if present.
@param soap context
@param[in] id string of UsernameToken or NULL
@return _wsse__UsernameToken object or NULL

Note: this release supports the use of at most one UsernameToken in the header.
*/
struct _wsse__UsernameToken*
soap_wsse_UsernameToken(struct soap *soap, const char *id)
{ 
  _wsse__Security *security = soap_wsse_Security(soap);

  if (security
   && security->UsernameToken
   && (!id || (security->UsernameToken->wsu__Id
            && !strcmp(security->UsernameToken->wsu__Id, id))))
    return security->UsernameToken;
  return NULL;
}

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
soap_wsse_get_Username(struct soap *soap)
{ 
  _wsse__UsernameToken *token = soap_wsse_UsernameToken(soap, NULL);

  DBGFUN("soap_wsse_get_Username");
  if (token)
    return token->Username;
  soap_wsse_fault(soap, wsse__FailedAuthentication, "Username authentication required");
  return NULL;
}

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
soap_wsse_verify_Password(struct soap *soap, const char *password)
{ 
  _wsse__UsernameToken *token = soap_wsse_UsernameToken(soap, NULL);
 // DBGFUN("soap_wsse_verify_Password");
  /* if we have a UsernameToken with a Password, check it */
  if (token && token->Password)
  { /* password digest or text? */
    if (token->Password->Type
     && !strcmp(token->Password->Type, wsse_PasswordDigestURI))
    { /* check password digest: compute SHA1(created, nonce, password) */
      if (token->Nonce
       && token->wsu__Created
       && strlen(token->Password->__item) == 28)	/* digest pw len = 28 */
      { 
      	char HA1[SOAP_SMD_SHA1_SIZE], HA2[SOAP_SMD_SHA1_SIZE];
        /* The specs are not clear: compute digest over binary nonce or base64 nonce? The formet appears to be the case: */
        int noncelen;
        const char *nonce = soap_base642s(soap, token->Nonce, NULL, 0, &noncelen);
        /* compute HA1 = SHA1(created, nonce, password) */
        calc_digest(soap, token->wsu__Created, nonce, noncelen, password, HA1);
        /*
        calc_digest(soap, token->wsu__Created, token->Nonce, strlen(token->Nonce), password, HA1);
        */
        /* get HA2 = supplied digest from base64 Password */
        soap_base642s(soap, token->Password->__item, HA2, SOAP_SMD_SHA1_SIZE, NULL);
        /* compare HA1 to HA2 */
        if (!memcmp(HA1, HA2, SOAP_SMD_SHA1_SIZE))
        { /* authorize if HA1 and HA2 identical and not replay attack */
          //dvprintf("%s, token->wsu__Created %s, token->Nonce %s \n",__func__,token->wsu__Created, token->Nonce);
          if (!soap_wsse_session_verify(soap, HA1, token->wsu__Created, token->Nonce))
          {	
            //dvprintf("%s %d \n",__func__,__LINE__);
            return SOAP_OK;
          }
          return soap->error; 
        }
      }
    }
    else
    { 
      /* check password text */
      if (!strcmp(token->Password->__item, password))
        return SOAP_OK;
    }
  }
  
  return soap_wsse_fault(soap, wsse__FailedAuthentication, NULL);
}

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
soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail)
{
//#if defined(SOAP_WSA_2003) || defined(SOAP_WSA_2004) || defined(SOAP_WSA_200408) || defined(SOAP_WSA_2005)
//  return soap_wsa_sender_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
//#else
  return soap_sender_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
//#endif
}



/**
@fn int soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail)
@brief Sets SOAP Fault (sub)code for server response.
@param soap context
@param[in] fault is one of wsse:FaultcodeEnum
@param[in] detail string with optional text message
@return SOAP_FAULT
*/
int
soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail)
{ const char *code = soap_wsse__FaultcodeEnum2s(soap, fault);
  DBGFUN2("soap_wsse_fault", "fault=%s", code?code:"", "detail=%s", detail?detail:"");
  /* remove incorrect or incomplete Security header */
  soap_wsse_delete_Security(soap);
  /* populate the SOAP Fault as per WS-Security spec */
  /* detail = NULL; */ /* uncomment when detail text not recommended */
  /* use WSA to populate the SOAP Header when WSA is used */
  switch (fault)
  { case wsse__UnsupportedSecurityToken:
      return soap_wsse_sender_fault_subcode(soap, code, "An unsupported token was provided", detail);
    case wsse__UnsupportedAlgorithm:
      return soap_wsse_sender_fault_subcode(soap, code, "An unsupported signature or encryption algorithm was used", detail);
    case wsse__InvalidSecurity:
      return soap_wsse_sender_fault_subcode(soap, code, "An error was discovered processing the <wsse:Security> header", detail);
    case wsse__InvalidSecurityToken:
      return soap_wsse_sender_fault_subcode(soap, code, "An invalid security token was provided", detail);
    case wsse__FailedAuthentication:
      return soap_wsse_sender_fault_subcode(soap, code, "The security token could not be authenticated or authorized", detail);
    case wsse__FailedCheck:
      return soap_wsse_sender_fault_subcode(soap, code, "The signature or decryption was invalid", detail);
    case wsse__SecurityTokenUnavailable:
      return soap_wsse_sender_fault_subcode(soap, code, "Referenced security token could not be retrieved", detail);
  }
  return SOAP_FAULT;
}



/******************************************************************************\
 *
 * Digest authentication session management
 *
\******************************************************************************/

/**
@fn static int soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce)
@brief Verifies and updates the digest, nonce, and creation time against the digest authentication session database to prevent replay attacks.
@param soap context
@param[in] hash binary digest value of PasswordDigest
@param[in] created string
@param[in] nonce string (base64)
@return SOAP_OK or SOAP_FAULT
*/
#include <time.h>

static int
soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce)
{ struct soap_wsse_session *session;
  time_t expired, now = time(NULL)+32400;
  //struct tm *tm_ptr, *local_time;
  return SOAP_OK;
  //tm_ptr = gmtime(&now);
  //local_time = localtime(&now);
  //printf("local time %s \n", asctime(local_time));
  //printf("current time : yy %d mm %d, dd %d, hh %d, min %d \n",
  //  tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);

  DBGFUN("soap_wsse_session_verify");
  soap_s2dateTime(soap, created, &expired);
  /* creation time in the future? */
  //printf("%s, expired %lu, now %lu dff %lu \n",__func__, expired, now, expired - now);
  if (expired > now + SOAP_WSSE_CLKSKEW)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, "Authorization request in future");
  //printf("%s %d \n",__func__,__LINE__);   
  expired += SOAP_WSSE_NONCETIME;
  /* expired? */
  if (expired <= now)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, "Authentication expired");
  //printf("%s %d \n",__func__,__LINE__);    
  /* purge expired messages, but don't do this all the time to improve efficiency */
  if (now % 10 == 0)
    soap_wsse_session_cleanup(soap);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Verifying session nonce=%s\n", nonce));
  //printf("%s %d \n",__func__,__LINE__);  
  /* enter mutex to check and update session */
//  MUTEX_LOCK(soap_wsse_session_lock);
  for (session = soap_wsse_session; session; session = session->next)
  { if (!memcmp(session->hash, hash, SOAP_SMD_SHA1_SIZE) && !strcmp(session->nonce, nonce))
      break;
  }
  //printf("%s %d \n",__func__,__LINE__);  
  /* if not found, allocate new session data */
  if (!session)
  { session = (struct soap_wsse_session*)malloc(sizeof(struct soap_wsse_session) + strlen(nonce));
    if (session)
    { session->next = soap_wsse_session;
      session->expired = expired;
      memcpy(session->hash, hash, SOAP_SMD_SHA1_SIZE);
      strcpy(session->nonce, nonce);
      soap_wsse_session = session;
    }
    session = NULL;
  }
  //printf("%s %d \n",__func__,__LINE__);
  /* exit mutex */
//  MUTEX_UNLOCK(soap_wsse_session_lock);
  /* if replay attack, return non-descript failure */
  if (session)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, NULL);
  //printf("%s %d \n",__func__,__LINE__);
  return SOAP_OK;
}

/**
@fn static void soap_wsse_session_cleanup(struct soap *soap)
@brief Removes expired authentication data from the digest authentication session database.
@param soap context
*/
static void
soap_wsse_session_cleanup(struct soap *soap)
{ struct soap_wsse_session **session;
  time_t now = time(NULL);
  DBGFUN("soap_wsse_session_cleanup");
  /* enter mutex to purge expired session data */
//  MUTEX_LOCK(soap_wsse_session_lock);
  session = &soap_wsse_session;
  while (*session)
  { if ((*session)->expired < now)
    { struct soap_wsse_session *p = *session;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Deleting session nonce=%s\n", p->nonce));
      *session = p->next;
      free(p);
    }
    else
      session = &(*session)->next;
  }
  /* exit mutex */
//  MUTEX_UNLOCK(soap_wsse_session_lock);
}

/******************************************************************************\
 *
 * Calculate SHA1(created, nonce, password) digest
 *
\******************************************************************************/

/**
@fn static void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE])
@brief Calculates digest value SHA1(created, nonce, password)
@param soap context
@param[in] created string (XSD dateTime format)
@param[in] nonce value
@param[in] noncelen length of nonce value
@param[in] password string
@param[out] hash SHA1 digest
*/
static void
calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE])
{ struct soap_smd_data context;
  /* use smdevp engine */
  soap_smd_init(soap, &context, SOAP_SMD_DGST_SHA1, NULL, 0);
  soap_smd_update(soap, &context, nonce, noncelen);
  soap_smd_update(soap, &context, created, strlen(created));
  soap_smd_update(soap, &context, password, strlen(password));
  soap_smd_final(soap, &context, hash, NULL);
}

#ifdef __cplusplus
}
#endif

