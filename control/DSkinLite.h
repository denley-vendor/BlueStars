/*!
* DSkinLite.h 
* 
* Copyright (c) 2008 by alex ding / uieasy.com
*/


#ifndef _DSKINLITE_H__
#define _DSKINLITE_H__


///////////////////////////////////////////////////////////////////////////
//Function: dsInitKeys( char* pRegName, char* plicenkey )
//
//Parameters: 
//		pRegName 
//			[in] Its type is ascii, the register name. 
//      pLicenkey
//			[in] Its type is ascii, the license key.
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	dsInitKeys must be called before you call other function, or there will run as a trial version.
//  You can get a key from uieasy by contacting to support@uieasy.com.

 BOOL  __stdcall dsInitKeys( char* pRegName, char* plicenkey);



///////////////////////////////////////////////////////////////////////////
//Function: dsLoadSkin( LPCTSTR strSkinPath, int nLoadType );
//
//Parameters: 
//		strSkinPath 
//			[in] Specifies the skin to load, 
//				 if the nLoadType is LOAD_FROM_FOLDER, the strSkinPath is the skin folder name.
//               if the nLoadType is LOAD_FROM_DB, the strSkinPath is the skin db file name.
//				 The path can be  integrated,or relative to the application directory. 
//      nLoadType
//			[in] Specifies the loading type, this parameter can be the following value: 
//
//				LOAD_FROM_FOLDER 
//					Load the skin files from the folder.
//				LOAD_FROM_DB 
//					Load the skin files from the skin database, which is builder by DSKinLite SkinDB Builder. 

//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The dsLoadSkin function set the window's skin.



 BOOL  __stdcall dsLoadSkin( LPCTSTR strSkinPath, int nLoadType );


///////////////////////////////////////////////////////////////////////////
//Function: dsExitSkin()
//
//Parameters: 
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The dsExitSkin function must be called when your application terminates


 BOOL  __stdcall dsExitSkin();


///////////////////////////////////////////////////////////////////////////
//Function: dsSkinWindow(HWND hWnd, UINT uCtrlType, LPCTSTR szXmlLabel, BOOL bChildSkin  );
//
//Parameters: 
//			hWnd
//			[in] Handle of the window
//			uCtrlType
//			[in] The type of the window,defined in following enumWindowType:

//enum enumWindowType
//{
//	SKIN_TYPE_UNKNOWN = 1,
//	SKIN_TYPE_DIALOG = 100,
//	SKIN_TYPE_DIALOGEX ,
//	SKIN_TYPE_CHILDDIALOG,
//	SKIN_TYPE_BUTTON,
//	SKIN_TYPE_CHECKBUTTON,
//	SKIN_TYPE_RADIOBUTTON,
//	SKIN_TYPE_STATIC,
//	SKIN_TYPE_EDIT,
//	SKIN_TYPE_LISTBOX,
//	SKIN_TYPE_LISTCTRL,
//	SKIN_TYPE_COMBOBOX,
//	SKIN_TYPE_SCROLLWIN,
//	SKIN_TYPE_HEADER,
//	SKIN_TYPE_GROUPBOX,
//	SKIN_TYPE_SCROLLBAR,
//	SKIN_TYPE_SLIDER,
//	SKIN_TYPE_SPIN,
//	SKIN_TYPE_TAB,
//	SKIN_TYPE_MENU,
//	SKIN_TYPE_PROGRESS,
//	SKIN_TYPE_GIFWND,
//	SKIN_TYPE_TREECTRL,
//	SKIN_TYPE_TOOLBARCTRL,
//	SKIN_TYPE_SPLIT_BUTTON,
//	SKIN_TYPE_RICHEDIT,
//	SKIN_TYPE_COMMON,
//};

//			szXmlLabel
//			[in] Specifies the label name of window, the label name is defined in the configuration xml.
//				if the parameter is NULL, the skin will use the default value to set the window's skin.
//			bChildSkin
//			[in]Specifies whether set the children's skin of the window.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The dsExitSkin function must be called when your application terminates

 BOOL  __stdcall dsSkinWindow(HWND hWnd, UINT uCtrlType, LPCTSTR szXmlLabel , BOOL bChildSkin  );


///////////////////////////////////////////////////////////////////////////
//Function: dsAdjustColor( int nHue, int nSaturation);
//
//Parameters: 
//     nHue
//		[in] Specifies the hue of skin. The range is from -180 to 180, and the default value is 0.

//     nSaturation  
//		[in] Specifies the saturation of the skin. The range is from 0 to 200, and the default value is 100.
//
//		nLightness
//		[in] Specifies the lightness of the skin. The range is from -100 to 100, and the default value is 0.

//Return Values:
//If the function succeeds, the return value is nonzero.
//If the function fails, the return value is zero. To get extended error information, call GetLastError.

//Remarks:
//	dsAdjustColor function adjusts the skin's hue , saturation , and lightness.
//  If setting the hue ,saturation, lightness to (0,100,0), the skin will return to default color.
//  In the application, maybe you need to limit the ranges of hue, saturation, lightness according to your ui look and feel; 
// because for some image, a large value will cause the appearance to look badly.





 BOOL __stdcall dsAdjustColor( int nHue, int nSaturation, int lightness);


///////////////////////////////////////////////////////////////////////////
//Function: dsEnableMenuNoBorder( BOOL bEable);
//
//Parameters: 
//     bEable
//		[in] Specifies whether drawing the border of menu.If this parameter is TRUE, 
//			 the shadow of menu window will be removed.
//
//Return Values:
//If the function succeeds, the return value is nonzero.
//If the function fails, the return value is zero. To get extended error information, call GetLastError.

//Remarks:
// dsEnableMenuNoBorder used to remove the shadow of the menu window, if the shadow is removed, you can draw the 
// border of menu window.

 BOOL __stdcall dsEnableMenuNoBorder( BOOL bEable);


///////////////////////////////////////////////////////////////////////////
//Function: dsGetLastError(void);
//
//Parameters: 
//
//Return Values:
//    The return value is the calling dskin's last-error code value 
//

//Remarks:
//  The GetLastError function retrieves the dskin's last-error code value.

 DWORD __stdcall dsGetLastError(void);



///////////////////////////////////////////////////////////////////////////
//Function: dsTrackPopupMenu( HMENU hMenu, UINT  uFlags, int   x, int y, HWND  hWnd, LPCTSTR lpLabel);
//
//Parameters: 
//		hMenu 
//		[in] Handle of the shortcut menu.
//		uFlags
//		[in] Function options,use the flag in TrackPopupMenu's uFlags parameter. 
//		x
//		[in] Specifies the horizontal location of the shortcut menu, in screen coordinates. 
//		y
//		[in] Specifies the vertical location of the shortcut menu, in screen coordinates. 
//		hWnd
//		[in] Handle of the window that owns the shortcut menu. This window receives all messages from the menu
//		lpLabel
//		[in] Specifies the label name of menu, the label name is defined in the config xml.

//Return Values:
//If the function succeeds, the return value is nonzero.
//If the function fails, the return value is zero. To get extended error information, call GetLastError.
//

//Remarks:
//  The dsTrackPopupMenu function displays a skinned shortcut menu at the specified location, like the win32 api TrackPopupMenu



 BOOL __stdcall  dsTrackPopupMenu( HMENU hMenu, UINT  uFlags, int   x, int y, HWND  hWnd, LPCTSTR lpLabel);



///////////////////////////////////////////////////////////////////////////
//Function: dsSetDrawItemVisible( HWND hwnd, int nMinIndex, int nMaxIndex, BOOL bVisible , BOOL bRepaint);
//
//Parameters: 
//		hwnd 
//		[in] Handle of the window.
//		nMinIndex
//		[in] The min index of item.the value according to the parameter "index" in xml; 
//		nMaxIndex
//		[in] The max index of item.the value according to the parameter "index" in xml; 
//		bVisible
//		[in] Specifies whether the items to be visible,  TURE for visible, FALSE for hide. 
//		bRepaint
//		[in] Specifies whether the window is to be repainted. If this parameter is TRUE,
//			the window receives a message. If the parameter is FALSE, no repainting of any kind occurs.
//
//Return Values:
//If the function succeeds, the return value is nonzero.
//If the function fails, the return value is zero. To get extended error information, call GetLastError.
//

//Remarks:
//  Set the drawing item in xml to be visible or hide



 BOOL __stdcall dsSetDrawItemVisible( HWND hwnd, int nMinIndex, int nMaxIndex, BOOL bVisible , BOOL bRepaint );



///////////////////////////////////////////////////////////////////////////
//Function: dsAddMenuBar( HWND hwnd, HWND* pToolBar, HMENU hMenu, LPCTSTR szToolBarLabel, LPCTSTR szMenuLabel, BOOL bAsMainToolbar );
//
//Parameters: 
//		hwnd 
//		[in] Handle of the window.
//		pToolBar
//		[in,out] return the handle of toolbar created to load menu
//		hMenu
//		[in] The hMenu of menu to be insterted.
//		szToolBarLabel
//		[in] The style of Toolbarctrl defined int the xml.
//		szMenuLabel
//		[in] Specifies the style of the menu defined in the xml.
//		bAsMainToolbar
//		[in] Specifies whether set the toolbar as the main toolbar of the window.The main toolbar will be moved the top of the window.
//
//Return Values:
//If the function succeeds, the return value is nonzero.
//If the function fails, the return value is zero. To get extended error information, call GetLastError.
//

//Remarks:
// The dsAddMenuBar function Add menu bar to window; It will create a toolbar control to contain the menu. Like the CommandBar in WTL.


 BOOL __stdcall  dsAddMenuBar( HWND hwnd, HWND* pToolBar, HMENU hMenu, LPCTSTR szToolBarLabel, LPCTSTR szMenuLabel, BOOL bAsMainToolbar);

///////////////////////////////////////////////////////////////////////////
//Function: dsGetIcon( LPCTSTR strIconLabel);
//
//Parameters: 
//		strIconLabel
//		[in]  strIconLabel is the "id" of icon image, defined in xml.For example:
//		 <image id="standard.icon" value="All_logo16-xp.ico" />
//
//Return Values:
//Return the handle of icon. It can be used in DrawIcon or DrawIconEx
//
//Remarks:
// dsGetIcon return the handle HICON according the parameter strIconLabel.
//When the skin changes, the HICON is invalid, you should re-call the function to get new HICON

 HICON __stdcall  dsGetIcon( LPCTSTR strIconLabel);


///////////////////////////////////////////////////////////////////////////
//Function: dsGetColor( LPCTSTR strColorLabel);
//
//Parameters: 
//		strColorLabel
//		[in] The id of color defined in xml. for example:
//			 "<color id="system.border" value="RGB(84,165,213)"/>"
//			 You can get the color value by passing the id "system.border";
//
//Return Values:
//return the color value.
//
//Remarks:
// Get the color value according to the color id defined in the xml.


 COLORREF __stdcall dsGetColor( LPCTSTR strColorLabel);



///////////////////////////////////////////////////////////////////////////
//Function: dsDrawImage( LPCTSTR strBmp, HDC hdc, LPRECT rtDest, LPRECT rtImage);
//
//Parameters: 
//		strImageLabel 
//			[in]strImageLabel is the "id" of image, defined in xml. For example: <image id="standard.window.bk" value="window\windowBkg.bmp" /> 
//		hdc 
//			[in] specifies the HDC to be drawn 
//		rtDest 
//			[in] Specifies the bounding rectangle 
//		rtImage 
//			[in] Specifies the part of image used to be drawn. 
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE.
//
//Remarks:
// The dsDrawImage function uses the specified image to draw on the hdc. 
//Sometimes, user may need to draw some images by self. 
//To keep the consistency, dskinlite need to manager all the drawing images, 
//when the theme color changes, all the images will change color. So if the image is loaded by user, this will break the consistency. 
//If it cann't find the handle of image according to the "id", it will return FALSE. 


 BOOL __stdcall dsDrawImage( LPCTSTR strBmp, HDC hdc, LPRECT rtDest, LPRECT rtImage);



///////////////////////////////////////////////////////////////////////////
//Function: dsGetFont( LPCTSTR strFont);
//
//Parameters: 
//		strColorLabel
//		[in] TThe id of font defined in xml. For example:
//			 "<font id="system.default" face="tahoma" size="11"/>"
//			
//
//Return Values:
// The return value is a handle to the font, or NULL if not find the font. 
//
//Remarks:
// Get the handle of font according to the font id defined in the xml.
//You should deletes these resources by calling DeleteObject  when you are finished using the HFONT

 HFONT __stdcall dsGetFont( LPCTSTR strFont);


///////////////////////////////////////////////////////////////////////////
//Function: dsGetClientRect( HWND hwnd, LPRECT lpRect);
//
//Parameters: 
//		hwnd
//		[in] The handle of dialog window
//		lpRect		
//		[in,out] Pointer to a RECT structure that receives the client coordinates.
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
//
//Remarks:
// The GetClientRect function retrieves the coordinates of a window's client area.Diffrent from the window api GetClientRect,
// The lef and top of the lpRect are not zero, they are the left and top value of the client area in the window.
//
// Use dsGetClientRect to replace GetClientRect API.
// If you use the SKIN_TYPE_DIALOG to skin the window, and user can set caption, border width,
// so after skin the dialog, its window style and client area changed,you should use the dsGetClientRect to get the right area.


 void __stdcall dsGetClientRect( HWND hwnd, LPRECT lpRect);



///////////////////////////////////////////////////////////////////////////
//Function: dsInvalidateRect( HWND hWnd, CONST RECT* lpRect, BOOL bErase);
//
//Parameters: 
//		hWnd
//		[in] handle to the window.
//
//		lpRect
//		[in] Pointer to a RECT structure that contains the client coordinates of the rectangle to be added to the update region. 
//			 If this parameter is NULL, the entire client area is added to the update region. 
//
//		bErase
//			[in] Specifies whether the background within the update region is to be erased when the update region is processed. 
//				 If this parameter is TRUE, the background is erased when the BeginPaint function is called. 
//				If this parameter is FALSE, the background remains unchanged.
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
//
//Remarks:
// Use the dsInvalidateRect to redraw the windows. The Win32 API Invalidate will cause a WM_PAINT
// message, but in dskinlite, there is a memory dc of window, and  the WM_PAINT may not cause a real memory dc rebuild, if the size of window doesn't 
// changed and the state of window doesn't changed. This is more efficient. So DSkinlite provides a function dsInvalidateRect to
// make a real redraw of window.

 BOOL __stdcall dsInvalidateRect( HWND hWnd, CONST RECT* lpRect, BOOL bErase);


///////////////////////////////////////////////////////////////////////////
//Function: dsSetSelfPaint( HWND hwnd , BOOL bSelfPaint);
//
//Parameters: 
//		hWnd
//		[in] handle to the window.
//
//		bErase
//			[in] specified whether self paint. TRUE to start self paint, FALSE to cancel self paint 
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
//
//Remarks:
//     Sometimes, there are some dynamic images that can't be described in xml, 
// and you need to draw them according to program logic.You can refer to function dsSetSelfPaint.
//    After calling dsSkinWindow to skin window, the application can't receive the WM_PAINT and WM_ERASEBKGND message.
//DSkinlite captures the messages and draws the window. 
//    If you want to draw the window , you can call this function , then you will receive the WM_ERASEBKGND message, 
// and the WPARAM parameter is the window memory dc in DSkinlite, so you can draw it at will.


 BOOL __stdcall dsSetSelfPaint( HWND hwnd , BOOL bSelfPaint);

///////////////////////////////////////////////////////////////////////////
//Function: dsSetAnimatePlay( HWND hwnd, BOOL bPlay);
//
//Parameters: 
//		hWnd
//		[in] handle to the window.
//
//		bErase
//			[in] if set to TRUE, then the animate start to play, or the animate will stop.
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
//
//Remarks:
//  Start or stop the animation.


 BOOL __stdcall dsSetAnimatePlay( HWND hwnd, BOOL bPlay);


///////////////////////////////////////////////////////////////////////////
//Function: dsResetAnimate( HWND hwnd);
//
//Parameters: 
//		hWnd
//		[in] handle to the window.
//
//Return Values:
// If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
//
//Remarks:
//  If the animation is not repeat, you can use this function to reset the animation, then it will restart. 


 BOOL __stdcall dsResetAnimate( HWND hwnd);

 ///////////////////////////////////////////////////////////////////////////
 //Function: dsSetAnimateFrame(  HWND hwnd, int nFrame );
 //
 //Parameters: 
 //		hWnd
 //		[in] handle to the window.
 //		nFrame
 //		[in] the index of frame to show
 //Return Values:
 // If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
 //
 //Remarks:
 // The function will stop animation and set nFrame as current frame. 
 // then animation will start ( if calling dsSetAnimatePlay(TRUE)) from this frame. 

 BOOL __stdcall dsSetAnimateFrame( HWND hwnd, int nFrame);


 ///////////////////////////////////////////////////////////////////////////
 //Function: dsSetHighLight( HWND hwnd,  BOOL bHightLight );
 //
 //Parameters: 
 //		hWnd
 //		[in] handle to the window.

 //		bHightLight
 //		[in] Specifies whether the button is to be highlighted. DSkinlite will use the down state to draw the highlight state 


 //
 //Return Values:
 // If the function succeeds, the return TRUE . If the function fails, the return value is FALSE. 
 //
 //Remarks:
 //Setting the state of control, you can use this function to set the control to be highlight. 
 //This function is valid for Static, Button control.

 BOOL __stdcall dsSetHighLight( HWND hwnd,  BOOL bHightLight);


 ///////////////////////////////////////////////////////////////////////////
 //Function: dsLoadSkinFromRes( LPCTSTR szResourceName, LPCTSTR szResourceType);
 //
 //Parameters: 
 //		szResourceName 
 //			[in] Specifies the name of the resource.
 //				 Use the MAKEINTRESOURCE( resource id ) to get the value.
 //      szResourceType
 //			[in] Specifies the resource type. 
 //


 //Return Values:
 //   If the function success, the return value is nonzero
 //   If the function fail, the return value is zero

 //Remarks:
 //	The function will load skin from resource of exe module.
 // For example: dsLoadSkinFromRes( MAKEINTRESOURCE(IDR_SKINDB1), _T("SKINDB"));

 // TIPS: This function is supported by the enterprise version. If you need, please contact us :support@uieasy.com

BOOL __stdcall dsLoadSkinFromRes( LPCTSTR szResourceName, LPCTSTR szResourceType);


///////////////////////////////////////////////////////////////////////////
//Function: dsSetDrawItemValue( HWND hwnd , LPCTSTR strID, LPCTSTR strValue, BOOL bRedraw);
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of the window
//			
//		strID
//			[in] Specifies the id of the draw item which is defined in the xml file.
//
//		strValue
//			[in] Specifies the value of the draw item.
//				 If the draw item type is text, this value is the text content.
//				 If the draw item type is image, this value is the image file path.
//
//		bRedraw
//			[in] Specifies whether redrawing the window after changing the draw item value.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is available for all the controls.

BOOL __stdcall dsSetDrawItemValue(  HWND hwnd, LPCTSTR strID, LPCTSTR strValue, BOOL bRedraw);


///////////////////////////////////////////////////////////////////////////
//Function: dsSetItemStyle( HWND hwnd, int nItem, int nSubItem,, LPCTSTR strStyle, BOOL bRedrawItem);
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of listbox , listCtrl or tree control
//			
//      nItem
//			[in] Specifies the item index 
//				  For listbox, it is the item index. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//
//		strStyle
//			[in] Specifies the id of style for item which is defined in the xml file.
//
//
//		bRedrawItem
//			[in] Specifies whether redrawing the item after changing the draw item value.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is available for listbox, list control, tree control now, used to change the style of  control's item.

BOOL __stdcall dsSetItemStyle(  HWND hwnd, int nItem, int nSubItem, LPCTSTR strStyle, BOOL bRedrawItem);

///////////////////////////////////////////////////////////////////////////
//Function: dsSetDrawItemValueEx( HWND hwnd, int nItem, int nSubItem,LPCTSTR strID, LPCTSTR strValue, BOOL bRedrawItem);
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of list control or tree control
//			
//      nItem
//			[in] Specifies the item index.
//				  For listbox, it is the index of listbox item. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//				 
//		strID
//			[in] Specifies the id of the draw item which is defined in the xml file.
//
//		strValue
//			[in] Specifies the value of the draw item.
//				 If the draw item type is text, this value is the text content.
//				 If the draw item type is image, this value is the image file path.
//
//		bRedrawItem
//			[in] Specifies whether redrawing the item after changing the draw item value.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is used to configure the  control's draw item value. It can be used to change the text item
// or the image item.


BOOL __stdcall dsSetDrawItemValueEx(  HWND hwnd, int nItem, int nSubItem,LPCTSTR strID, LPCTSTR strValue, BOOL bRedrawItem);


///////////////////////////////////////////////////////////////////////////
//Function: dsSetDrawItemVisibleEx( HWND hwnd, int nItem, int nSubItem, LPCTSTR strID, BOOL bVisible,  BOOL bRedrawItem);
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of list control listbox or tree control
//			
//      nItem
//			[in] Specifies the item index.
//				  For listbox, it is the index of listbox item. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//
//		strID
//			[in] Specifies the id of the draw item.
//
//		bVisible
//			[in] Specifies  the draw item  to be visible or not.
//
//
//		bRedrawItem
//			[in] Specifies whether redrawing the item after changing the draw item value.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	This function is used to set the draw item of the item to be visible or not. It is available for ListBox,listCtrl or TreeCtrl.

BOOL __stdcall dsSetDrawItemVisibleEx( HWND hwnd, int nItem, int nSubItem, LPCTSTR strID, BOOL bVisible,  BOOL bRedrawItem);



///////////////////////////////////////////////////////////////////////////
//Function: dsSetDrawItemVisibleEx2( HWND hwnd, int nItem, int nSubItem, int nMinIndex, int nMaxIndex, BOOL bVisible , BOOL bRedrawItem );
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of list control listbox or tree control
//			
//      nItem
//			[in] Specifies the item index.
//				  For listbox, it is the index of listbox item. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//
//		nMinIndex
//			[in] Specifies the min index of draw item to be visible or not.
//
//		nMaxIndex
//			[in] Specifies the max index of draw item to be visible or not.
//
//
//		bRedrawItem
//			[in] Specifies whether redrawing the item after changing the draw item value.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	This function is used to set the draw item of the item to be visible or not. It is available for ListBox,listCtrl or TreeCtrl.


BOOL __stdcall dsSetDrawItemVisibleEx2( HWND hwnd, int nItem, int nSubItem, int nMinIndex, int nMaxIndex, BOOL bVisible , BOOL bRedrawItem );


///////////////////////////////////////////////////////////////////////////
//Function: dsSetProgressItemRange( HWND hwnd, int nItem, int nSubItem, int nLower, int nUpper );
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of list control listbox or tree control
//			
//      nItem
//			[in] Specifies the item index.
//				  For listbox, it is the index of listbox item. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//
//		nLower
//			[in] Sets the lower limits of the progress item's range 
//
//		nUpper
//			[in] Sets the upper limits of the progress item's range 
//
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	This function is used to set limits of the progress item. It is available for ListBox,listCtrl or TreeCtrl.


BOOL __stdcall dsSetProgressItemRange( HWND hwnd, int nItem, int nSubItem, int nLower, int nUpper );


///////////////////////////////////////////////////////////////////////////
//Function: dsSetProgressItemPos( HWND hwnd, int nItem, int nSubItem, int nPos );
//
//Parameters: 
//		hwnd
//			[in] Specifies the handle of list control listbox or tree control
//			
//      nItem
//			[in] Specifies the item index.
//				  For listbox, it is the index of listbox item. 
//				  For the tree control, it is the handle(HTREEITEM) of  tree item.
//				  For the listctrl, it is the index of the list control item.
//		nSubItem
//			[in] Specifies the sub item index of the list control
//				 This parameter is only valid for list control.
//
//		nPos
//			[in] New position of the progress item.
//
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	Sets the progress item's current position as specified by nPos and redraws the item to reflect the new position.
//  It is available for ListBox,listCtrl or TreeCtrl.

BOOL __stdcall dsSetProgressItemPos( HWND hwnd, int nItem, int nSubItem, int nPos);

///////////////////////////////////////////////////////////////////////////
//Function: dsAddStyleFile( LPCTSTR strFile);
//
//Parameters: 
//		 strFile
//			[in] Specifies the file path of xml.
//			
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is used to add a new xml file to dskinlite dynamically.


BOOL __stdcall dsAddStyleFile( LPCTSTR strFile);


///////////////////////////////////////////////////////////////////////////
//Function:  dsMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption,UINT uType)
//
//Parameters: 
//		hWnd
//			[in] Handle to the owner window of the message box to be created. 
//
//	    lpText
//			[in] Pointer to a null-terminated string that contains the message to be displayed. 	
//
//		lpCaption
//			[in] Pointer to a null-terminated string that contains the dialog box title.
//
//		uType
//			[in] Specifies the contents and behavior of the dialog box. Please refer to windows api MessageBox to get the flags.
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is used to show the messagebox as the windows api MessageBox.

int __stdcall dsMessageBox( HWND hWnd,LPCTSTR lpText, LPCTSTR lpCaption,UINT uType);


///////////////////////////////////////////////////////////////////////////
//Function:  dsLockState( HWND hwnd, BOOL bLocked);
//
//Parameters: 
//		hWnd
//			[in] Handle of the window. 
//
//	    bLocked
//			[in] TRUE to lock the state of window, FALSE to release the lock. 	

//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero

//Remarks:
//	The function is used to lock the state of window, after setting the parameter bLocked to TRUE, the window will keep 
//  current state until calling the dsLockState( hwnd, FALSE);
//  For more detail, you can refer to the example in ButtonDlg.cpp in the example folder.


int __stdcall dsLockState( HWND hwnd, BOOL bLocked);


#endif