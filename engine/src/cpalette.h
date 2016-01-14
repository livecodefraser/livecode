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

//
// MCColors class declarations
//
#ifndef	COLORS_H
#define	COLORS_H

#include "mccontrol.h"

class MCColors : public MCControl
{
	uint4 selectedcolor;
public:
	MCColors();
	MCColors(const MCColors &cref);
	// virtual functions from MCObject
	virtual ~MCColors();
	virtual Chunk_term gettype() const;
	virtual const char *gettypestring();
	virtual Boolean mfocus(int2 x, int2 y);
	virtual Boolean mdown(uint2 which);
	virtual Boolean mup(uint2 which, bool p_release);

#ifdef LEGACY_EXEC
	virtual Exec_stat getprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective, bool recursive = false);
	virtual Exec_stat setprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective);
#endif

	virtual Boolean count(Chunk_term type, MCObject *stop, uint2 &num);
	// virtual functions from MCControl
	IO_stat load(IO_handle stream, uint32_t version);
	IO_stat extendedload(MCObjectInputStream& p_stream, uint32_t version, uint4 p_length);
	IO_stat save(IO_handle stream, uint4 p_part, bool p_force_ext, uint32_t p_version);
	IO_stat extendedsave(MCObjectOutputStream& p_stream, uint4 p_part, uint32_t p_version);

	virtual MCControl *clone(Boolean attach, Object_pos p, bool invisible);

	// MW-2011-09-06: [[ Redraw ]] Added 'sprite' option - if true, ink and opacity are not set.
    virtual void DrawPrepare(MCDC *dc, MCRectangle &x_dirty, bool p_isolated, bool p_sprite);
    virtual void DrawFinish(MCDC *dc, const MCRectangle &p_dirty, bool p_isolated, bool p_sprite);
    virtual void DrawBackgroundLegacy(MCDC *dc, const MCRectangle &p_dirty, bool p_isolated, bool p_sprite);
    virtual void DrawContentsLegacy(MCDC *dc, const MCRectangle &p_dirty, bool p_isolated, bool p_sprite);
    virtual void DrawForegroundLegacy(MCDC *dc, const MCRectangle &p_dirty, bool p_isolated, bool p_sprite);
};
#endif
