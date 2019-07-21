#include <string.h>

#include "dev.h"
#include "dev_inline.h"

#include "http_authorization.h"
#include "md5.h"

#include "user.h"

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION
#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_BASIC
#include "Base64.h"
static int HttpAuthorization_BASIC(char *datacheck)
{
	size_t nonce_len, index;
	int nRes = DEF_FAIL;
	char *datacpy = NULL;
	char *baseDecodeString = NULL;

	index = strcspn(datacheck, CRLF) + 1;
	datacpy = (char *)pvPortMalloc(index);
	if(datacpy == NULL)
	{
		printf("%04d%s : malloc fail\r\n", __LINE__, __func__);
		return nRes;
	}
	strlcpy(datacpy, datacheck+HTTP11_AUTORIZATION_LEN, index-HTTP11_AUTORIZATION_LEN);
	baseDecodeString = (char *)base64_decode(datacpy, strlen(datacpy), &nonce_len);
	if(baseDecodeString == NULL)
	{
		printf("%04d%s : malloc fail\r\n", __LINE__, __func__);
		if(datacpy)
			vPortFree(datacpy);
		return nRes;
	}

	index = strcspn(baseDecodeString, ":");
	if(strncmp((const char *)gtUser.strUserId, baseDecodeString, index) == 0)
	{
		if(strncmp((const char *)gtUser.strUserPw, &(baseDecodeString[index]) + 1, nonce_len - index - 1) == 0)
		{
			nRes = DEF_OK;
		}
	}

	if(datacpy)
		vPortFree(datacpy);
	if(baseDecodeString)
		vPortFree(baseDecodeString);

	return nRes;
}
#endif

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_DIGEST
static void Hex2Str(unsigned char* digest, char* rchar)
{
	int i, j;
	unsigned char ctemp;

	for(j=0,i=0;i<16;i++)
	{
		ctemp = (digest[i] & 0xf0) >> 4;
		if(ctemp <= 0x9)		rchar[j] = ctemp + 0x30;
		else if(ctemp <= 0xF)	rchar[j] = (ctemp - 0xA) + 0x61;
		else					rchar[j] = 0x30;
		j++;

		ctemp = (digest[i] & 0x0f);
		if(ctemp <= 0x9)		rchar[j] = ctemp + 0x30;
		else if(ctemp <= 0xF)	rchar[j] = (ctemp - 0xA) + 0x61;
		else					rchar[j] = 0x30;
		j++;
	}
	rchar[32] = 0;
}

static int HttpAuthorization_DIGEST(char *datacheck, int isPost)
{
	int index;
	int methodlen;
	char *datacpy = NULL;
	char *pMD5EncyptData = NULL;
	char *realm = NULL;
	char *uri = NULL;
	char *nonce = NULL;
	char *qop = NULL;
	char *nc = NULL;
	char *cnonce = NULL;
	char *opaque = NULL;
	char *response = NULL;
	char id[16+1] = {0,};
	char pass[16+1] = {0,};
	char *temp = NULL;
	char hsMethod[6] = {0,};

//	UINT t_St = timer_get_ms();

	if(isPost)	strcpy(hsMethod, "POST");
	else		strcpy(hsMethod, "GET");

	index = strcspn(datacheck, CRLF);
	//printf("authorization datacheck CRLF index %d\r\n", index);

	datacpy = (char *)pvPortMalloc((size_t)index + 1);
	strncpy(datacpy, datacheck, index);
	datacpy[index] = 0;
	//printf("authorization datacpy %s\r\n", datacpy);

	index = index - HTTP11_AUTORIZATION_LEN;
	pMD5EncyptData = (char *)pvPortMalloc((size_t)index);
	strcpy(pMD5EncyptData, datacpy + HTTP11_AUTORIZATION_LEN);

	///////////////////////////////////////////////////////////
	strcpy(id, (const char *)gtUser.strUserId);
	strcpy(pass, (const char *)gtUser.strUserPw);

	//printf("Cam ID(%s) PW(%s)\r\n", gtUser.strUserId, gtUser.strUserPw);

	///////////////////////////////////////////////////////////
	// parser start
	temp = strstr(pMD5EncyptData,"realm=");
	if(temp)
	{
		temp = temp+7;
		index = strcspn(temp, "\"");
		realm = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(realm, temp, index);
		realm[index] = 0;
//		printf(" realm [%d][%s]\r\n", index, realm);
	}

	temp = strstr(pMD5EncyptData, "nonce=");
	if(temp)
	{
		temp = temp+7;
		index = strcspn(temp, "\"");
		nonce = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(nonce, temp, index);
		nonce[index] = 0;
//		printf(" nonce [%d][%s]\r\n", index, nonce);
	}

	temp = strstr(pMD5EncyptData, "uri=");
	if(temp)
	{
		temp = temp+5;
		index = strcspn(temp, "\"");
		strcat(hsMethod, ":");
		methodlen = strlen(hsMethod);
		uri = (char *)pvPortMalloc((size_t)index + methodlen + 1);
		strncpy(uri, hsMethod, methodlen);
		strncpy(uri+methodlen, temp, index);
		uri[index+methodlen] = 0;
//		printf(" uri [%d][%s]\r\n", index, uri);
	}

	temp = strstr(pMD5EncyptData, "cnonce=");
	if(temp)
	{
		temp = temp+8;
		index = strcspn(temp, "\"");
		cnonce = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(cnonce, temp, index);
		cnonce[index] = 0;
//		printf(" cnonce [%d][%s]\r\n", index, cnonce);
	}

	temp = strstr(pMD5EncyptData, "nc=");
	if(temp)
	{
		temp = temp+3;
		index = strcspn(temp, ",");
		nc = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(nc, temp, index);
		nc[index] = 0;
//		printf(" nc [%d][%s]\r\n", index, nc);
	}

	temp = strstr(pMD5EncyptData, "response=");
	if(temp)
	{
		temp = temp+10;
		index = strcspn(temp, "\"");
		response = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(response, temp, index);
		response[index] = 0;
//		printf(" response [%d][%s]\r\n", index, response);
	}

	temp = strstr(pMD5EncyptData, "qop=");
	if(temp)
	{
		//printf(" temp [%s]\r\n", temp);
		if(strncmp(temp+4,"\"",1)==0)
		{
			temp = temp + 5;
			index = strcspn(temp, "\"");
		}
		else
		{
			temp = temp + 4;
			index = strcspn(temp, ",");
		}
		qop = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(qop, temp, index);
		qop[index] = 0;
//		printf(" qop [%d][%s]\r\n", index, qop);
	}

	temp = strstr(pMD5EncyptData, "opaque=");
	if(temp)
	{
		temp = temp + 8;
		index = strcspn(temp, "\"");
		opaque = (char *)pvPortMalloc((size_t)index + 1);
		strncpy(opaque, temp, index);
		opaque[index] = 0;
//		printf(" opaque [%d][%s]\r\n", index, opaque);
	}
	// Client측에서 보내온 결과 끝====== 결과물 response
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//HA1 = MD5(id:realm:password);
	//HA2 = MD5(uri);
	//response = MD5(HA1:nonce:nc:cnonce:qop:HA2)

	char cHA1[33] = {0};
	char HA2[33] = {0};
	char HAresponse[33] = {0};
	unsigned char hash[16];
	MD5_CTX mdContext;
	MD5Init(&mdContext);
#if 0
	MD5Update(&mdContext, (u_char*) id, strlen(id));
	MD5Update(&mdContext, (u_char*) ":", strlen(":"));
	MD5Update(&mdContext, (u_char*) realm, strlen(realm));
	MD5Update(&mdContext, (u_char*) ":", strlen(":"));
	MD5Update(&mdContext, (u_char*) pass, strlen(pass));
#else
	//char encryptstr[200];
	char *encryptstr = (char *)pvPortCalloc(1, strlen(id) + strlen(pass) + strlen(realm) + 2 + 1);
	strcpy(encryptstr, id);
	strcat(encryptstr, ":");
	strcat(encryptstr, realm);	// realm이 한없이 길어질 수 있다!!
	strcat(encryptstr, ":");
	strcat(encryptstr, pass);
	MD5Update(&mdContext, (unsigned char *)encryptstr, strlen(encryptstr));
	vPortFree(encryptstr);
#endif
	MD5Final(hash, &mdContext);
	Hex2Str(mdContext.digest, cHA1);
	//printf("HA1 [%s]\r\n", cHA1);

	MD5_CTX mdContext2;
	MD5Init(&mdContext2);
	MD5Update(&mdContext2, (unsigned char *)uri, strlen(uri));
	MD5Final(hash, &mdContext2);
	Hex2Str(mdContext2.digest, HA2);
	//printf("HA2 [%s]\r\n", HA2);

	MD5_CTX mdContext3;
	MD5Init(&mdContext3);
	char encryptstr2[200] = {0};
	//printf("HA1 %s strlen %d\r\n" ,cHA1, strlen(cHA1));
	strncpy(encryptstr2, cHA1, strlen(cHA1));
	strcat(encryptstr2, ":");
	strcat(encryptstr2, nonce);
	strcat(encryptstr2, ":");
	strcat(encryptstr2, nc);
	strcat(encryptstr2, ":");
	strcat(encryptstr2, cnonce);
	strcat(encryptstr2, ":");
	strcat(encryptstr2, qop);
	strcat(encryptstr2, ":");
	strcat(encryptstr2, HA2);
	//printf("encryptstr2 %s strlen %d\r\n",encryptstr2, strlen(encryptstr2));
	MD5Update(&mdContext3, (unsigned char *)encryptstr2, strlen(encryptstr2));
	MD5Final(hash, &mdContext3);
	Hex2Str(mdContext3.digest, HAresponse);
	//printf("HAresponse [%s]\r\n",HAresponse);

	//////////////////////////////////////////////////
	if(datacpy)
		vPortFree(datacpy);
	if(pMD5EncyptData)
		vPortFree(pMD5EncyptData);

	if(realm)
		vPortFree(realm);
	if(uri)
		vPortFree(uri);
	if(nonce)
		vPortFree(nonce);
	if(qop)
		vPortFree(qop);
	if(nc)
		vPortFree(nc);
	if(cnonce)
		vPortFree(cnonce);
	if(opaque)
		vPortFree(opaque);

	if(strcmp(HAresponse, response) == 0)
	{
//		UINT t_En = timer_get_ms();
//		printf("authorization Time(%dms)\r\n", t_En - t_St);
		if(response)
			vPortFree(response);
		return 1;
	}
	else
	{
		if(response)
			vPortFree(response);
		return 0;
	}
}
#endif

int HttpAuthorization(int isPost, char *data, int data_len)
{
	char *datacheck;

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_BASIC
	if((datacheck = strnstr(data, HTTP11_BASIC_AUTORIZATION, data_len)) != NULL)
	{
		return HttpAuthorization_BASIC(datacheck);
		UNUSED(isPost);
	}
#endif	//LWIP_HTTPD_SUPPORT_AUTHORIZATION_BASIC
#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_DIGEST
	if((datacheck = strnstr(data, HTTP11_DIGEST_AUTORIZATION, data_len))!=NULL)
	{
		return HttpAuthorization_DIGEST(datacheck, isPost);
	}
#endif	//LWIP_HTTPD_SUPPORT_AUTHORIZATION_DIGEST
//	printf("HTTP : Unauthorization(401.html)\r\n");
	return DEF_FAIL;
}
#endif	//LWIP_HTTPD_SUPPORT_AUTHORIZATION
