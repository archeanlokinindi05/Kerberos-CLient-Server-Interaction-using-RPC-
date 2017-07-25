/* SP_proc.c */
#include <rpc/rpc.h>
#include "C2S.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool SP_validator()
{
	return true;
}

strRep replyBuilder(char *targetString){
	strRep reply ;
	int length;
	length = strlen(targetString);
	printf("In reply builder %s \n", targetString);
	memcpy(reply.resultText, (char*)targetString, length) ;
	reply.len = strlen(targetString);
	return (reply);
}

strRep* alpha_1(strReq S)
{
	static strRep reply;
	strRep reply2 ;
	char *text = (char*) malloc(strlen(S.msg));
	int i, j = 0, counter = 0 ;
	int messageLen ;
	char c ;
	char tokenVal[24] ;
	char *targetString ;

	printf("Processing request message in Alpha service \n");

	messageLen = S.length ;

	printf("Input length in Alpha service ----> %d \n", messageLen);

	memcpy(text, S.msg, S.length);
	text[messageLen] = '\0' ;
	memcpy(tokenVal, S.tokenKey, 24);
	tokenVal[24] = '\0' ;

	printf("Input message text in Alpha service ----> %s \n", text);
	printf("Token val in Alpha service ----> %s \n", tokenVal);

	for (i = 0; i < messageLen ; i++)
	{
		c = text[i] ;

	  	if (isalpha(c) > 0)
		{
			counter++ ;
		}
	}

	targetString = (char*) malloc(counter);


	for (i = 0; i < messageLen ; i++)
	{
		c = text[i] ;

	  	if (isalpha(c) > 0)
		{
			memcpy(targetString+j, &c, 1);
			j++ ;
		}
	}


	targetString[j] = '\0' ;
	printf("Target string %s \n", targetString);
	
	reply = replyBuilder(targetString);

	printf("Sending reply with Alpha service. Length = %d Alpha string = %s\n", reply.len, targetString);
	
	return (&reply);
}

strRep* numeric_1(strReq S)
{
	static strRep reply;
	strRep reply2 ;
	char *text = (char*) malloc(strlen(S.msg));
	int i, j = 0, counter = 0 ;
	int messageLen ;
	char c ;
	char tokenVal[24] ;
	char *targetString ;

	printf("Processing request message in Numeric service \n");

	messageLen = S.length ;

	printf("Input length in Numeric service ----> %d \n", messageLen);

	memcpy(text, S.msg, messageLen);
	text[messageLen] = '\0' ;
	memcpy(tokenVal, S.tokenKey, 24);
	tokenVal[24] = '\0' ;

	printf("Input message text in Numeric service ----> %s \n", text);
	printf("Token val in Numeric service ----> %s \n", tokenVal);

	for (i = 0; i < messageLen ; i++)
	{
		c = text[i] ;

	  	if (isdigit(c) > 0)
		{
			counter++ ;
		}
	}

	targetString = (char*) malloc(counter);


	for (i = 0; i < messageLen ; i++)
	{
		c = text[i] ;

	  	if (isdigit(c) > 0)
		{
			memcpy(targetString+j, &c, 1);
			j++ ;
		}
	}


	targetString[j] = '\0' ;
	printf("Target string %s \n", targetString);
	
	reply = replyBuilder(targetString);

	printf("Sending reply with Numeric service. Length = %d Alpha string = %s\n", reply.len, targetString);
	
	return (&reply);
}

