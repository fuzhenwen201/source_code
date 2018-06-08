/*******************************************************************************************

*Department:	TP

*Decrible:  	FIX SiganlEngine
				
				


*Auhor:			Savin

*Createdtime:	2017-06-07

*ModifyTime:	


********************************************************************************************/


#ifndef QUOTEREQUEST44_PUBLIC_HEAD_H
#define QUOTEREQUEST44_PUBLIC_HEAD_H


typedef struct _STQuoteRequest44
{
	char 	BeginString[8];
	char 	BodyLength[8];
	char	MsgType[8];
	char	SenderCompID[64];
	char	TargetCompID[64];
	char	MsgSeqNum[20];
	char	SendingTime[32];
	char	Checksum[8];
	
	char	QuoteReqID[12];
	char	Symbol[8];
	char	BidPx[20];
	char	BidSize[20];
	char	OfferPx[20];
	char	OfferSize[20];

	unsigned long int  RecvTime;
	
}STQuoteRequest44;

extern char fortexUserName[128];
extern char fortexPassword[128];


#endif //QUOTEREQUEST44_PUBLIC_HEAD_H

