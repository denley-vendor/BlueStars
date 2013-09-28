#pragma once
#ifndef WEIBOSTRUCT_H
#define WEIBOSTRUCT_H
#include "..//..//stdafx.h"
class WeiBoStruct
{
public:
	WeiBoStruct(void);
	~WeiBoStruct(void);
};
typedef	struct LONGINUSER_INFO
{
		DWORD		id;
		CString		screenName;
		CString		name;
		DWORD		province;
		DWORD		city;
		CString		location;
		CString		description;
		CString		url;
		CString		profileImageUrl;
		CString		domain;
		BOOL		bGender;
		DWORD		followersCount;
		DWORD		friendsCount;
		DWORD		statusesCount;
		DWORD		favouritesCount;
		CString		createdAt;
		BOOL		bFollowing;
		BOOL		bVerified;
		BOOL		bAllowAllActMsg;
		BOOL		bGeoEnabled;

} LONGINUSER_INFO;


#endif