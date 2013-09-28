#pragma once
#ifndef MAINTITLEIMAGE_H
#define MAINTITLEIMAGE_H
#include <afxwin.h>

class MainTitleImage : public CButton
{
public:
	MainTitleImage(void);
	~MainTitleImage(void);
public:
	afx_msg		void		OnMouseHover(UINT nFlags, CPoint point);
	afx_msg		void		OnMouseLeave();
	afx_msg		void		OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif