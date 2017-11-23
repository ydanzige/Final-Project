#pragma once
#include "stdafx.h"

inline bool UrlPathsEquels( const wchar_t *source, const wchar_t *target )
{
	return 0 == wcscmp( source, target );
}