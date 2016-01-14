/* Copyright (C) 2003-2016 LiveCode Ltd.
 
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

#ifndef THEME_LCB_H
#define THEME_LCB_H


#include "theme.h"

#include "libscript/script.h"
#include "libscript/script-auto.h"
#include "parsedef.h"


// Theme that is implemented as an LCB module
class MCControlThemeLCB :
public MCControlTheme
{
public:
    
    // Constructor. Takes a handle to an LCB module that implements the theming
    // engine API.
    MCControlThemeLCB(MCScriptModuleRef);
    
    virtual ~MCControlThemeLCB();
    
    virtual bool DrawBackground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite);
    virtual bool DrawForeground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite);
    
    virtual bool GetDefaultThemeBoolean(MCObject*, MCPlatformThemeProperty, bool& r_value);
    virtual bool GetDefaultThemeInteger(MCObject*, MCPlatformThemeProperty, int& r_value);
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCColor& r_color);
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCPlatformControlState p_extra_states, MCColor& r_color);
    virtual bool GetDefaultThemeFont(MCObject*, MCPlatformThemeProperty, MCFontRef& r_font);
    virtual bool GetDefaultThemeRect(MCObject*, MCPlatformThemeProperty, MCRectangle& r_rect);
    
private:
    
    MCAutoScriptInstanceRef m_instance;
    
    // Turns the properties of an object into an array so they can be used by
    // the LCB theme code.
    bool PackProperties(MCObject* p_object, MCPlatformControlState, bool p_effective, MCArrayRef& r_array);
    
    // Dispatches a draw call of the given name
    bool DispatchDraw(MCDC*, MCObject*, const MCRectangle&, MCNameRef p_method);
    
    // Dispatches a property fetch with the given name
    bool DispatchPropertyGet(MCObject*, MCPlatformThemeProperty, MCPlatformControlState, MCValueRef& r_value);
    
    // Adds a colour property to an array
    static bool AddColorProp(MCArrayRef, MCNameRef, MCObject*, Properties);
};


#endif  /* ifndef THEME_LCB_H */
