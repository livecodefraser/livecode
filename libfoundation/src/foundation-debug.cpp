/* Copyright (C) 2003-2015 LiveCode Ltd.

This file is part of LiveCode.

LiveCode is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License v3 as published by the Free
Software Foundation.

LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with LiveCode.  If not see <http://www.gnu.org/licenses/>.  */

#include <foundation.h>
#include <foundation-auto.h>

#include "foundation-private.h"

////////////////////////////////////////////////////////////////////////////////

#if defined(DEBUG_LOG)

#if defined(_MACOSX) || defined(_LINUX) || defined(TARGET_SUBPLATFORM_IPHONE) || defined(_WINDOWS) || defined(_WINDOWS_SERVER)

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef WIN32
inline int getpid() { return 0; }
#else
#  include <unistd.h>
#endif

void __MCUnreachable(void)
{
	fprintf(stderr, "**** UNREACHABLE CODE EXECUTED ****\n");
	abort();
}

void __MCAssert(const char *p_file, uint32_t p_line, const char *p_message)
{
    fprintf(stderr, "MCAssert failed: %s:%u \"%s\"\n", p_file, p_line, p_message);
    abort();
}

void __MCLog(const char *p_file, uint32_t p_line, const char *p_format, ...)
{
	MCAutoStringRef t_string;
	
	va_list t_args;
	va_start(t_args, p_format);
	MCStringFormatV(&t_string, p_format, t_args);
	va_end(t_args);
	
	char *t_cstring;
	if (MCStringConvertToCString(*t_string, t_cstring))
	{
		fprintf(stderr, "[%d] %s\n", getpid(), t_cstring);
		MCMemoryDeallocate(t_cstring);
	}
}

void __MCLogWithTrace(const char *p_file, uint32_t p_line, const char *p_format, ...)
{
	MCAutoStringRef t_string;
	
	va_list t_args;
	va_start(t_args, p_format);
	MCStringFormatV(&t_string, p_format, t_args);
	va_end(t_args);
	
	char *t_cstring;
	if (MCStringConvertToCString(*t_string, t_cstring))
	{
		fprintf(stderr, "[%d] %s\n", getpid(), t_cstring);
		MCMemoryDeallocate(t_cstring);
	}
}

#elif defined(TARGET_SUBPLATFORM_ANDROID)
#include <android/log.h>

void __MCAssert(const char *p_file, uint32_t p_line, const char *p_message)
{
}

void __MCLog(const char *p_file, uint32_t p_line, const char *p_format, ...)
{
    MCAutoStringRef t_string;

    va_list t_args;
    va_start(t_args, p_format);
    MCStringFormatV(&t_string, p_format, t_args);
    va_end(t_args);
	
	char *t_cstring;
	if (MCStringConvertToCString(*t_string, t_cstring))
	{
		__android_log_print(ANDROID_LOG_INFO, "revandroid", "%s", t_cstring);
		MCMemoryDeallocate(t_cstring);
    }
}

void __MCUnreachable(void)
{
	fprintf(stderr, "**** UNREACHABLE CODE EXECUTED ****\n");
	abort();
}

#elif defined(__EMSCRIPTEN__)

void __MCUnreachable(void)
{
	fprintf(stderr, "**** UNREACHABLE CODE EXECUTED ****\n");
	abort();
}

void __MCAssert(const char *p_file, uint32_t p_line, const char *p_message)
{
    fprintf(stderr, "MCAssert failed: %s:%u \"%s\"", p_file, p_line, p_message);
    abort();
}

void __MCLog(const char *p_file, uint32_t p_line, const char *p_format, ...)
{
	MCAutoStringRef t_string;
	
	va_list t_args;
	va_start(t_args, p_format);
	MCStringFormatV(&t_string, p_format, t_args);
	va_end(t_args);
	
	char *t_cstring;
	if (MCStringConvertToCString(*t_string, t_cstring))
	{
		fprintf(stderr, "%s\n", t_cstring);
		MCMemoryDeallocate(t_cstring);
	}
}

void __MCLogWithTrace(const char *p_file, uint32_t p_line, const char *p_format, ...)
{
	__MCLog(p_file, p_line, p_format);
}

#endif

#endif

////////////////////////////////////////////////////////////////////////////////
