#pragma once

namespace libapt
{
	enum Error
	{
		NO_ERROR		= 0,
		CONST_NF		= 1,
		APT_NF			= 2,
		DAT_NF			= 3,
		INVALID_APT		= 4,
		INVALID_CONST	= 5,
		INVALID_DAT		= 6,
		INVALID_GEOM	= 7,
		MISSING_MOVIE	= 8,
		INVALID_SHADER	= 9
	};
}