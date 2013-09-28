#pragma once
#ifndef IMAGESHOW_H
#define IMAGESHOW_H
#include "..//StdAfx.h"
class ImageShow : public CMFCButton
{
public:
	ImageShow(WCHAR *pImagePath = NULL);
	~ImageShow(void);
public:
		afx_msg		void		OnPaint();
		afx_msg		void		OnMouseLeave();
		afx_msg		void		OnMouseMove(UINT nFlags, CPoint point);
		afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
		afx_msg		void		OnSize(UINT nType, int cx, int cy);

		virtual		void		OnFillBackground(CDC* pDC, const CRect& rectClient);
		virtual		void		OnDrawFocusRect(CDC* pDC, const CRect& rectClient);
		virtual		void		OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
		virtual		void		OnDraw(CDC* pDC, const CRect& rect, UINT uiState);
		virtual		void		DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

					void		SetImage(WCHAR *pImagePath);
		inline		void		SetImageSize(BOOL	b){m_bImageSize = b;}
		DECLARE_MESSAGE_MAP()
private:
		CImage		m_image;
		CPen		m_pen;
		CPen		m_framePen;
		BOOL		m_bImageSize;
		CPoint		m_pt;
		DWORD		m_frameColor;
		BOOL		m_bMouseMove;
};

#endif