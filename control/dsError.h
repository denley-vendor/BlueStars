#ifndef _DSKINERROR_
#define _DSKINERROR_

enum em_ErrorCode
{
  DS_ERROR_PATH_INVALID = 100, //the file path is invalid
  DS_ERROR_XML_INVALID  = 101, //the xml file path is invalid
  DS_ERROR_PARSE_XML_FAULT = 102, //Parse XML error    
  DS_ERROR_LOAD_MSIMG32_FAIL = 103,  //load msimage32 dll error
  DS_ERROR_WINDOW_ALREADY_SKIN = 104, //The window is skinned already.
  DS_ERROR_NOT_BITMAP_FORMAT = 105, //not a bitmap format, event the file suffix is ".bmp"

  DS_ERROR_LOAD_COMTRL32_FAIL = 106, 
  DS_ERROR_LOAD_CREATETOOLBAREX_FAIL = 107, 
  DS_ERROR_COMMON_TYPE_REQUIRE_STYLE,  //for the common type control, you must specify the xml style, 
                                       //there is no default style name for common type control.
  DS_ERROR_SKINDB_INVALID,
  DS_ERROR_SKINDB_OPEN_FAIL,
  DS_ERROR_SKINDB_FINDRESOURCE,
  DS_ERROR_LOCKDATA_FAIL,
  DS_ERROR_VERSION_NO_SUPPORT,

};

#endif