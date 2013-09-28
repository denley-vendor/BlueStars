#ifndef _DSKIN_DEFINE_FILE_
#define _DSKIN_DEFINE_FILE_

#define dskin_prop_str   _T("#2008_dskin_set_this_hwnd_no_skin")

class dSkin_Set_Prop
{
public:
	dSkin_Set_Prop(HWND hwnd)
	{
		if (!::GetProp(hwnd, dskin_prop_str))
		{
			::SetProp(hwnd, dskin_prop_str, hwnd);
			m_hWnd = hwnd;
		}
	}

	dSkin_Set_Prop()
	{
	}

	BOOL IsSet( )
	{
		return ::GetProp( m_hWnd, dskin_prop_str) == NULL ?  FALSE : TRUE;
	}

	void SetProp(HWND hwnd)
	{
		if (!::GetProp(hwnd, dskin_prop_str))
		{
			::SetProp(hwnd, dskin_prop_str, hwnd);
			m_hWnd = hwnd;
		}
	}

	~dSkin_Set_Prop()
	{
		::RemoveProp(m_hWnd, dskin_prop_str);
		m_hWnd = NULL;
	}

private:
	HWND m_hWnd;
};

#define SET_NOSKIN(id, hwnd) dSkin_Set_Prop _skinSetNoSkinProp##id(hwnd)


enum enumWindowType
{
	SKIN_TYPE_UNKNOWN = 1,
	SKIN_TYPE_DIALOG = 100,
	SKIN_TYPE_DIALOGEX ,
	SKIN_TYPE_CHILDDIALOG,
	SKIN_TYPE_BUTTON,
	SKIN_TYPE_CHECKBUTTON,
	SKIN_TYPE_RADIOBUTTON,
	SKIN_TYPE_STATIC,
	SKIN_TYPE_EDIT,
	SKIN_TYPE_LISTBOX,
	SKIN_TYPE_LISTCTRL,
	SKIN_TYPE_COMBOBOX,
	SKIN_TYPE_SCROLLWIN,
	SKIN_TYPE_HEADER,
	SKIN_TYPE_GROUPBOX,
	SKIN_TYPE_SCROLLBAR,
	SKIN_TYPE_SLIDER,
	SKIN_TYPE_SPIN,
	SKIN_TYPE_TAB,
	SKIN_TYPE_MENU,
	SKIN_TYPE_PROGRESS,
	SKIN_TYPE_GIFWND,
	SKIN_TYPE_TREECTRL,
	SKIN_TYPE_TOOLBARCTRL,
	SKIN_TYPE_SPLIT_BUTTON,
	SKIN_TYPE_RICHEDIT,
	SKIN_TYPE_COMMON,
	SKIN_TYPE_DATETIMECTRL
};

//when you skin a button as a split button, if you press the split part, the parent window will receive this message.

#define WM_LBUTTONDOWN_SPLITBTN ( WM_USER + 1982) 
#define WM_LISTCTRL_EVENT_LBTNDOWN ( WM_USER + 1983)
#define WM_LISTBOX_EVENT_LBTNDOWN ( WM_USER + 1984)
#define WM_TREECTRL_EVENT_LBTNDOWN ( WM_USER + 1984)


enum enumSkinType
{
     LOAD_FROM_FOLDER,
	 LOAD_FROM_DB,
	 LOAD_FROM_RES
};

enum _tagenumControlState
{
	CONTROL_STATE_NORMAL,
	CONTROL_STATE_OVER,
	CONTROL_STATE_DOWN,
};

typedef BOOL ( WINAPI *DRAWITEMCALLBACK) ( LPVOID lpParameter, DRAWITEMSTRUCT* pDrawItemStruct);

#endif