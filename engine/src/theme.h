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

#ifndef THEME_H
#define THEME_H


#include "sysdefs.h"
#include "platform.h"


class MCControlTheme
{
public:
    
    virtual ~MCControlTheme();
    
    // Themed controls are drawn in three passes:
    //  1. the background. For most controls, the majority of the drawing is
    //      done here.
    //  2. the contents. This includes things like the label or other text
    //      within a control. This isn't drawn by the theme code but rather by
    //      the control (but using some of the theming parameters).
    //  3. the foreground. Few controls use this but it provides the theming
    //      engine with an opportunity to draw on top of the contents.
    virtual bool DrawBackground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite) = 0;
    virtual bool DrawForeground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite) = 0;
    
    virtual bool GetDefaultThemeBoolean(MCObject*, MCPlatformThemeProperty, bool& r_value) = 0;
    virtual bool GetDefaultThemeInteger(MCObject*, MCPlatformThemeProperty, int& r_value) = 0;
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCColor& r_color) = 0;
    virtual bool GetDefaultThemeFont(MCObject*, MCPlatformThemeProperty, MCFontRef& r_font) = 0;
    virtual bool GetDefaultThemeRect(MCObject*, MCPlatformThemeProperty, MCRectangle& r_rect) = 0;
    
    // Variant required due to the structure of our colour-handling code
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCPlatformControlState p_extra_states, MCColor& r_color) = 0;
    
    void GetEffectiveBoolean(MCObject*, MCPlatformThemeProperty, bool& r_value);
    void GetEffectiveInteger(MCObject*, MCPlatformThemeProperty, int& r_value);
    void GetEffectiveColor(MCObject*, MCPlatformThemeProperty, MCColor& r_color);
    void GetEffectiveFont(MCObject*, MCPlatformThemeProperty, MCFontRef& r_font);
    void GetEffectiveRect(MCObject*, MCPlatformThemeProperty, MCRectangle& r_rect);
    
    // Returns true if this is an instance of MCControlThemeLegacyNative
    bool IsLegacyNativeTheme();
    
    // Returns true if this is an instance of MCControlThemeLegacy
    bool IsLegacyTheme();
};

// Theme that uses the legacy rendering methods but native theme properties
class MCControlThemeLegacyNative :
  public MCControlTheme
{
public:
    
    virtual bool DrawBackground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite);
    virtual bool DrawForeground(MCDC* p_dc, MCObject* p_control, const MCRectangle& p_rect, bool p_isolated, bool p_sprite);
    
    virtual bool GetDefaultThemeBoolean(MCObject*, MCPlatformThemeProperty, bool& r_value);
    virtual bool GetDefaultThemeInteger(MCObject*, MCPlatformThemeProperty, int& r_value);
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCColor& r_color);
    virtual bool GetDefaultThemeColor(MCObject*, MCPlatformThemeProperty, MCPlatformControlState p_extra_states, MCColor& r_color);
    virtual bool GetDefaultThemeFont(MCObject*, MCPlatformThemeProperty, MCFontRef& r_font);
    virtual bool GetDefaultThemeRect(MCObject*, MCPlatformThemeProperty, MCRectangle& r_rect);
    
    static MCControlThemeLegacyNative* GetInstance();
    
protected:
    
    virtual MCPlatformControlState GetObjectState(MCObject*);
    
private:
    
    static MCControlThemeLegacyNative* s_Instance;
};

// Theme that uses the legacy rendering methods and legacy theme properties
class MCControlThemeLegacy :
public MCControlThemeLegacyNative
{
public:
    
    static MCControlThemeLegacy* GetInstance();
    
protected:
    
    virtual MCPlatformControlState GetObjectState(MCObject*);
    
private:
    
    static MCControlThemeLegacy* s_Instance;
};


#endif
