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

#include "prefix.h"

#include "context.h"

#include "graphics.h"

////////////////////////////////////////////////////////////////////////////////

/*bool MCThemeDraw(MCGContextRef p_context, MCThemeDrawType p_type, MCThemeDrawInfo *p_info_ptr)
{
	return false;
}*/

////////////////////////////////////////////////////////////////////////////////

bool MCPlatformGetControlThemePropBool(MCPlatformControlType, MCPlatformControlPart, MCPlatformControlState, MCPlatformThemeProperty, bool&)
{
    return false;
}

bool MCPlatformGetControlThemePropInteger(MCPlatformControlType, MCPlatformControlPart, MCPlatformControlState, MCPlatformThemeProperty, int&)
{
    return false;
}

bool MCPlatformGetControlThemePropColor(MCPlatformControlType p_type, MCPlatformControlPart, MCPlatformControlState, MCPlatformThemeProperty p_prop, MCColor& r_color)
{
    MCColor t_color;
    switch (p_prop)
    {
        case kMCPlatformThemePropertyTextColor:
            if (p_type == kMCPlatformControlTypeInputField
                || p_type == kMCPlatformControlTypeLabel
                || p_type == kMCPlatformControlTypeRichText
                || p_type == kMCPlatformControlTypeCheckbox
                || p_type == kMCPlatformControlTypeRadioButton)
                t_color.red = t_color.green = t_color.blue = 0x0000;
            else
                t_color.red = t_color.green = t_color.blue = 0xFFFF;
            break;
        
        case kMCPlatformThemePropertyBackgroundColor:
            if (p_type == kMCPlatformControlTypeWindow
                || p_type == kMCPlatformControlTypeInputField
                || p_type == kMCPlatformControlTypeRichText)
            {
                t_color.red = t_color.green = t_color.blue = 0xFFFF;
            }
            else
            {
                t_color.red = 0x2222;
                t_color.green = 0x9696;
                t_color.blue = 0xF3F3;
            }
            break;
            
        case kMCPlatformThemePropertyShadowColor:
            t_color.red = 0xFFFF;
            t_color.green = t_color.blue = 0;
            break;
            
        case kMCPlatformThemePropertyBorderColor:
            t_color.red = 0;
            t_color.green = 0xFFFF;
            t_color.blue = 0;
            break;
            
        case kMCPlatformThemePropertyFocusColor:
            t_color.red = 0x4F4F;
            t_color.green = 0xA1A1;
            t_color.blue = 0xE3E3;
            break;
            
        case kMCPlatformThemePropertyTopEdgeColor:
            if (p_type == kMCPlatformControlTypeInputField)
            {
                // Really the bottom edge for fields
                t_color.red = 0x2222;
                t_color.green = 0x9696;
                t_color.blue = 0xF3F3;
            }
            else
                t_color.red = t_color.green = t_color.blue = 0xFFFF;
            break;
            
        case kMCPlatformThemePropertyLeftEdgeColor:
            t_color.red = t_color.green = t_color.blue = 0xFFFF;
            break;
            
        case kMCPlatformThemePropertyRightEdgeColor:
        case kMCPlatformThemePropertyBottomEdgeColor:
            if (p_type == kMCPlatformControlTypeInputField)
                t_color.red = t_color.green = t_color.blue = 0xFFFF;
            else
                t_color.red = t_color.green = t_color.blue = 0x0000;
            break;
            
        default:
            return false;
    }
    
    r_color = t_color;
    MCscreen->alloccolor(r_color);
    return true;
}

bool MCPlatformGetControlThemePropFont(MCPlatformControlType, MCPlatformControlPart, MCPlatformControlState, MCPlatformThemeProperty, MCFontRef& r_font)
{
    return MCFontCreate(MCNAME("Arial"), 0, 12, r_font);
}