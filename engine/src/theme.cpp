
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

#include "theme.h"
#include "theme-lcb.h"

#include "object.h"
#include "mccontrol.h"
#include "card.h"
#include "platform.h"
#include "module-canvas.h"
#include "graphicscontext.h"
#include "uidc.h"
#include "button.h"
#include "exec-interface.h"


MCControlThemeLegacyNative* MCControlThemeLegacyNative::s_Instance = NULL;
MCControlThemeLegacy* MCControlThemeLegacy::s_Instance = NULL;

MCControlThemeLegacyNative* MCControlThemeLegacyNative::GetInstance()
{
    if (s_Instance == NULL)
        s_Instance = new MCControlThemeLegacyNative;
    return s_Instance;
}

MCControlThemeLegacy* MCControlThemeLegacy::GetInstance()
{
    if (s_Instance == NULL)
        s_Instance = new MCControlThemeLegacy;
    return s_Instance;
}


bool MCControlTheme::IsLegacyNativeTheme()
{
    return this == MCControlThemeLegacyNative::GetInstance();
}

bool MCControlTheme::IsLegacyTheme()
{
    return this == MCControlThemeLegacy::GetInstance();
}


MCPlatformControlState MCControlThemeLegacyNative::GetObjectState(MCObject* p_object)
{
    return p_object->getcontrolstate();
}

MCPlatformControlState MCControlThemeLegacy::GetObjectState(MCObject* p_object)
{
    return p_object->getcontrolstate() | kMCPlatformControlStateCompatibility;
}


MCControlTheme::~MCControlTheme()
{
    ;
}


bool MCControlThemeLegacyNative::DrawBackground(MCDC* p_dc, MCObject* p_object, const MCRectangle& p_rect, bool p_isolated, bool p_sprite)
{
    Chunk_term t_type = p_object->gettype();
    if (t_type >= CT_FIRST_CONTROL && t_type <= CT_LAST_CONTROL)
    {
        static_cast<MCControl*> (p_object)->DrawBackgroundLegacy(p_dc, p_rect, p_isolated, p_sprite);
        return true;
    }
    else if (t_type == CT_CARD)
    {
        static_cast<MCCard*> (p_object)->DrawBackgroundLegacy(p_dc, p_rect, p_isolated);
        return true;
    }
    
    return false;
}

bool MCControlThemeLegacyNative::DrawForeground(MCDC* p_dc, MCObject* p_object, const MCRectangle& p_rect, bool p_isolated, bool p_sprite)
{
    Chunk_term t_type = p_object->gettype();
    if (t_type >= CT_FIRST_CONTROL && t_type <= CT_LAST_CONTROL)
    {
        static_cast<MCControl*> (p_object)->DrawForegroundLegacy(p_dc, p_rect, p_isolated, p_sprite);
        return true;
    }
    else if (t_type == CT_CARD)
    {
        static_cast<MCCard*> (p_object)->DrawForegroundLegacy(p_dc, p_rect, p_isolated);
        return true;
    }
    
    return false;
}

bool MCControlThemeLegacyNative::GetDefaultThemeBoolean(MCObject* p_object, MCPlatformThemeProperty p_prop, bool& r_bool)
{
    return MCPlatformGetControlThemePropBool(p_object->getcontroltype(), p_object->getcontrolsubpart(), GetObjectState(p_object), p_prop, r_bool);
}

bool MCControlThemeLegacyNative::GetDefaultThemeInteger(MCObject* p_object, MCPlatformThemeProperty p_prop, int& r_int)
{
    return MCPlatformGetControlThemePropInteger(p_object->getcontroltype(), p_object->getcontrolsubpart(), GetObjectState(p_object), p_prop, r_int);
}

bool MCControlThemeLegacyNative::GetDefaultThemeColor(MCObject* p_object, MCPlatformThemeProperty p_prop, MCColor& r_color)
{
    return MCPlatformGetControlThemePropColor(p_object->getcontroltype(), p_object->getcontrolsubpart(), GetObjectState(p_object), p_prop, r_color);
}

bool MCControlThemeLegacyNative::GetDefaultThemeColor(MCObject* p_object, MCPlatformThemeProperty p_prop, MCPlatformControlState p_extra_states, MCColor& r_color)
{
    return MCPlatformGetControlThemePropColor(p_object->getcontroltype(), p_object->getcontrolsubpart(), GetObjectState(p_object)|p_extra_states, p_prop, r_color);
}

bool MCControlThemeLegacyNative::GetDefaultThemeFont(MCObject* p_object, MCPlatformThemeProperty p_prop, MCFontRef& r_font)
{
    return MCPlatformGetControlThemePropFont(p_object->getcontroltype(), p_object->getcontrolsubpart(), GetObjectState(p_object), p_prop, r_font);
}

bool MCControlThemeLegacyNative::GetDefaultThemeRect(MCObject* p_object, MCPlatformThemeProperty p_prop, MCRectangle& r_rect)
{
    return false;
}


MCControlThemeLCB::MCControlThemeLCB(MCScriptModuleRef p_module) :
  m_instance()
{
    MCScriptCreateInstanceOfModule(p_module, &m_instance);
}

MCControlThemeLCB::~MCControlThemeLCB()
{
    ;
}


bool MCControlThemeLCB::DrawBackground(MCDC* p_dc, MCObject* p_object, const MCRectangle& p_dirty, bool p_isolated, bool p_sprite)
{
    // Attempt to draw using the theming engine and fall back if it fails
    if (!DispatchDraw(p_dc, p_object, p_dirty, MCNAME("themeDrawBackground")))
        MCControlThemeLegacy::GetInstance()->DrawBackground(p_dc, p_object, p_dirty, p_isolated, p_sprite);
    return true;
}

bool MCControlThemeLCB::DrawForeground(MCDC* p_dc, MCObject* p_object, const MCRectangle& p_dirty, bool p_isolated, bool p_sprite)
{
    // Attempt to draw using the theming engine and fall back if it fails
    if (!DispatchDraw(p_dc, p_object, p_dirty, MCNAME("themeDrawForeground")))
        MCControlThemeLegacy::GetInstance()->DrawForeground(p_dc, p_object, p_dirty, p_isolated, p_sprite);
    return true;
}

bool MCControlThemeLCB::DispatchDraw(MCDC* p_dc, MCObject* p_object, const MCRectangle& p_dirty, MCNameRef p_method)
{
    // Generate the properties array for the object
    MCAutoArrayRef t_properties;
    if (!PackProperties(p_object, 0, true, &t_properties))
        return false;
    
    // Turn the rectangle into a form LCB can process
    MCAutoValueRefBase<MCCanvasRectangleRef> t_rectangle;
    if (!MCCanvasRectangleCreateWithMCGRectangle(MCGRectangleMake(p_dirty.x, p_dirty.y, p_dirty.width, p_dirty.height), &t_rectangle))
        return false;
    
    // Parameter array. Push the properties array, then the rectangle
    MCAutoValueRefArray t_parameters;
    if (!t_parameters.Push(*t_properties))
        return false;
    if (!t_parameters.Push(*t_rectangle))
        return false;
    
    // Make the given DC the active graphics context for LCB
    uintptr_t t_cookie;
    MCAutoValueRefBase<MCCanvasRef> t_canvas;
    MCCanvasPush(((MCGraphicsContext*)p_dc)->getgcontextref(), t_cookie);
    MCCanvasThisCanvas(&t_canvas);
    
    // Save the canvas state
    MCCanvasSaveState(*t_canvas);
    
    // Set the clip to the whole card (to allow drawing outside the bounds)
    p_dc->setclip(p_object->getcard()->getrect());
    
    // Make the call
    MCAutoValueRef t_result;
    bool t_success = MCScriptCallHandlerOfInstance(*m_instance, p_method, t_parameters.Ptr(), t_parameters.Size(), &t_result);
    
    // Restore the canvas state
    MCCanvasRestoreState(*t_canvas);
    
    // Restore the previous graphics context
    MCCanvasPop(t_cookie);
    
    // Check the result
    if (!t_success || !MCValueIsEqualTo(*t_result, kMCTrue))
        return false;
    
    // All done
    return true;
}

bool MCControlThemeLCB::GetDefaultThemeBoolean(MCObject* p_object, MCPlatformThemeProperty p_prop, bool& r_bool)
{
    // Get the property
    MCAutoValueRef t_result;
    if (!DispatchPropertyGet(p_object, p_prop, 0, &t_result))
        return false;
    
    // Extract the result
    if (MCValueGetTypeCode(*t_result) != kMCValueTypeCodeBoolean)
        return false;
    r_bool = MCValueIsEqualTo(*t_result, kMCTrue);
    return true;
}

bool MCControlThemeLCB::GetDefaultThemeInteger(MCObject* p_object, MCPlatformThemeProperty p_prop, int& r_int)
{
    // Get the property
    MCAutoValueRef t_result;
    if (!DispatchPropertyGet(p_object, p_prop, 0, &t_result))
        return false;
    
    // Extract the result
    if (MCValueGetTypeCode(*t_result) != kMCValueTypeCodeNumber)
        return false;
    r_int = MCNumberFetchAsInteger((MCNumberRef)*t_result);
    return true;
}

bool MCControlThemeLCB::GetDefaultThemeColor(MCObject* p_object, MCPlatformThemeProperty p_prop, MCColor& r_color)
{
    return GetDefaultThemeColor(p_object, p_prop, 0, r_color);
}

bool MCControlThemeLCB::GetDefaultThemeColor(MCObject* p_object, MCPlatformThemeProperty p_prop, MCPlatformControlState p_extra_state, MCColor& r_color)
{
    // Get the property
    MCAutoValueRef t_result;
    if (!DispatchPropertyGet(p_object, p_prop, p_extra_state, &t_result))
        return false;
    
    // Extract the result. Note that we aren't able to preserve the alpha.
    MCCanvasFloat t_r, t_g, t_b;
    if (MCValueGetTypeInfo(*t_result) != kMCCanvasColorTypeInfo)
        return false;
    MCCanvasColorGetRed((MCCanvasColorRef)*t_result, t_r);
    MCCanvasColorGetGreen((MCCanvasColorRef)*t_result, t_g);
    MCCanvasColorGetBlue((MCCanvasColorRef)*t_result, t_b);
    
    r_color.red = uint16_t(t_r * 65535);
    r_color.green = uint16_t(t_g * 65535);
    r_color.blue = uint16_t(t_b * 65535);
    MCscreen->alloccolor(r_color);
    return true;
}

bool MCControlThemeLCB::GetDefaultThemeFont(MCObject* p_object, MCPlatformThemeProperty p_prop, MCFontRef& r_font)
{
    // Get the property
    MCAutoValueRef t_result;
    if (!DispatchPropertyGet(p_object, p_prop, 0, &t_result))
        return false;
    
    // Extract the result
    if (MCValueGetTypeInfo(*t_result) != kMCCanvasFontTypeInfo)
        return false;
    r_font = MCCanvasFontGetMCFont((MCCanvasFontRef)*t_result);
    return true;
}

bool MCControlThemeLCB::GetDefaultThemeRect(MCObject* p_object, MCPlatformThemeProperty p_prop, MCRectangle& r_rect)
{
    // Get the property
    MCAutoValueRef t_result;
    if (!DispatchPropertyGet(p_object, p_prop, 0, &t_result))
        return false;
    
    // Extract the result. We lose precision as MCRectangle uses int16_t.
    MCGRectangle t_grect;
    if (MCValueGetTypeInfo(*t_result) != kMCCanvasFontTypeInfo)
        return false;
    MCCanvasRectangleGetMCGRectangle((MCCanvasRectangleRef)*t_result, t_grect);
    r_rect.x = t_grect.origin.x;
    r_rect.y = t_grect.origin.y;
    r_rect.width = t_grect.size.width;
    r_rect.height = t_grect.size.height;
    return true;
}

bool MCControlThemeLCB::DispatchPropertyGet(MCObject* p_object, MCPlatformThemeProperty p_prop, MCPlatformControlState p_extra_state, MCValueRef& r_result)
{
    // Convert the property to a string representation
    MCAutoStringRef t_prop_name;
    switch (p_prop)
    {
        case kMCPlatformThemePropertyAlpha:
            t_prop_name = MCSTR("alpha");
            break;
            
        case kMCPlatformThemePropertyForegroundColor:
            t_prop_name = MCSTR("foregroundColor");
            break;
            
        case kMCPlatformThemePropertyBackgroundColor:
            t_prop_name = MCSTR("backgroundColor");
            break;
            
        case kMCPlatformThemePropertyBorderColor:
            t_prop_name = MCSTR("borderColor");
            break;
            
        case kMCPlatformThemePropertyBottomEdgeColor:
            t_prop_name = MCSTR("bottomEdgeColor");
            break;
            
        case kMCPlatformThemePropertyFocusColor:
            t_prop_name = MCSTR("focusColor");
            break;
            
        case kMCPlatformThemePropertyLeftEdgeColor:
            t_prop_name = MCSTR("leftEdgeColor");
            break;
            
        case kMCPlatformThemePropertyRightEdgeColor:
            t_prop_name = MCSTR("rightEdgeColor");
            break;
            
        case kMCPlatformThemePropertyShadowColor:
            t_prop_name = MCSTR("shadowColor");
            break;
            
        case kMCPlatformThemePropertyTextColor:
            t_prop_name = MCSTR("textColor");
            break;
            
        case kMCPlatformThemePropertyTextFont:
            t_prop_name = MCSTR("textFont");
            break;
            
        case kMCPlatformThemePropertyTextSize:
            t_prop_name = MCSTR("textSize");
            break;
            
        case kMCPlatformThemePropertyTopEdgeColor:
            t_prop_name = MCSTR("topEdgeColor");
            break;
            
        default:
            return false;
    }
    
    // Generate the properties array for the object
    MCAutoArrayRef t_properties;
    if (!PackProperties(p_object, p_extra_state, false, &t_properties))
        return false;
    
    // Parameter array. Push the properties array and then the property name
    MCAutoValueRefArray t_parameters;
    if (!t_parameters.Push(*t_properties))
        return false;
    if (!t_parameters.Push(*t_prop_name))
        return false;
    
    // Make the call
    return MCScriptCallHandlerOfInstance(*m_instance, MCNAME("themeGetDefaultProperty"), t_parameters.Ptr(), t_parameters.Size(), r_result);
}

bool MCControlThemeLCB::PackProperties(MCObject* p_object, MCPlatformControlState p_extra_state, bool p_effective, MCArrayRef& r_properties)
{
    // Accumulated object properties
    MCAutoArrayRef t_props;
    if (!MCArrayCreateMutable(&t_props))
        return false;
    
    // Base control type
    MCAutoStringRef t_base_type;
    if (!MCStringCreateWithCString(p_object->gettypestring(), &t_base_type))
        return false;
    if (!MCArrayStoreValue(*t_props, false, MCNAME("baseType"), *t_base_type))
        return false;
    
    // Derived control type
    if (p_object->gettype() == CT_BUTTON)
    {
        MCButton* t_button = static_cast<MCButton*> (p_object);
        switch (getstyleint(t_button->getflags()))
        {
            case F_STANDARD:
            case F_ROUNDRECT:
            case F_RECTANGLE:
            default:
                if (!MCArrayStoreValue(*t_props, false, MCNAME("buttonType"), MCSTR("pushbutton")))
                    return false;
                break;
            
            case F_CHECK:
                if (!MCArrayStoreValue(*t_props, false, MCNAME("buttonType"), MCSTR("checkbox")))
                    return false;
                break;
                
            case F_RADIO:
                if (!MCArrayStoreValue(*t_props, false, MCNAME("buttonType"), MCSTR("radiobutton")))
                    return false;
                break;
                
            case F_MENU:
                if (!MCArrayStoreValue(*t_props, false, MCNAME("buttonType"), MCSTR("menu")))
                    return false;
                break;
                
            case F_OVAL_BUTTON:
                if (!MCArrayStoreValue(*t_props, false, MCNAME("buttonType"), MCSTR("oval")))
                    return false;
                break;
        }
    }
    
    // Control state
    MCAutoProperListRef t_states;
    MCPlatformControlState t_state = p_object->getcontrolstate() | p_extra_state;
    if (!MCProperListCreateMutable(&t_states))
        return false;
    if ((t_state & kMCPlatformControlStateDefault) && !MCProperListPushElementOntoBack(*t_states, MCSTR("default")))
        return false;
    if ((t_state & kMCPlatformControlStateDisabled) && !MCProperListPushElementOntoBack(*t_states, MCSTR("disabled")))
        return false;
    if ((t_state & kMCPlatformControlStateMouseFocus) && !MCProperListPushElementOntoBack(*t_states, MCSTR("mouseFocus")))
        return false;
    if ((t_state & kMCPlatformControlStateMouseOver) && !MCProperListPushElementOntoBack(*t_states, MCSTR("mouseHover")))
        return false;
    if ((t_state & kMCPlatformControlStateOn) && !MCProperListPushElementOntoBack(*t_states, MCSTR("on")))
        return false;
    if ((t_state & kMCPlatformControlStatePressed) && !MCProperListPushElementOntoBack(*t_states, MCSTR("pressed")))
        return false;
    if ((t_state & kMCPlatformControlStateReadOnly) && !MCProperListPushElementOntoBack(*t_states, MCSTR("readOnly")))
        return false;
    if ((t_state & kMCPlatformControlStateSelected) && !MCProperListPushElementOntoBack(*t_states, MCSTR("selected")))
        return false;
    if ((t_state & kMCPlatformControlStateWindowActive) && !MCProperListPushElementOntoBack(*t_states, MCSTR("windowActive")))
        return false;
    if (!t_states.MakeImmutable())
        return false;
    if (!MCArrayStoreValue(*t_props, false, MCNAME("state"), *t_states))
        return false;
    
    // Control rectangle
    MCRectangle t_rect = p_object->getrect();
    MCAutoValueRefBase<MCCanvasRectangleRef> t_canvas_rect;
    if (!MCCanvasRectangleCreateWithMCGRectangle(MCGRectangleMake(t_rect.x, t_rect.y, t_rect.width, t_rect.height), &t_canvas_rect))
        return false;
    if (!MCArrayStoreValue(*t_props, false, MCNAME("rect"), *t_canvas_rect))
        return false;
    
    // Various boolean properties
    if ((p_object->getflags() & F_3D) && !MCArrayStoreValue(*t_props, false, MCNAME("3d"), kMCTrue))
        return false;
    if ((p_object->getflags() & F_SHOW_BORDER) && !MCArrayStoreValue(*t_props, false, MCNAME("showBorder"), kMCTrue))
        return false;
    if ((p_object->getflags() & F_OPAQUE) && !MCArrayStoreValue(*t_props, false, MCNAME("opaque"), kMCTrue))
        return false;
    if ((p_object->getflags() & F_SHADOW) && !MCArrayStoreValue(*t_props, false, MCNAME("shadow"), kMCTrue))
        return false;
    if ((p_object->getflags() & F_VISIBLE) && !MCArrayStoreValue(*t_props, false, MCNAME("visible"), kMCTrue))
        return false;
    if ((p_object->getflags() & F_DISABLED) && !MCArrayStoreValue(*t_props, false, MCNAME("disabled"), kMCTrue))
        return false;
    
    // Colour properties
    if (p_effective)
    {
        if (!AddColorProp(*t_props, MCNAME("backgroundColor"), p_object, P_BACK_COLOR))
            return false;
        if (!AddColorProp(*t_props, MCNAME("hilightColor"), p_object, P_HILITE_COLOR))
            return false;
        if (!AddColorProp(*t_props, MCNAME("textColor"), p_object, P_TEXT_COLOR))
            return false;
    }
    
    return MCArrayCopy(*t_props, r_properties);
}

bool MCControlThemeLCB::AddColorProp(MCArrayRef p_props, MCNameRef p_prop_name, MCObject* p_object, Properties p_which)
{
    MCExecContext ctxt;
    MCInterfaceNamedColor t_color;
    p_object->GetColor(ctxt, p_which, true, t_color);
    if (ctxt.HasError())
        return false;
    
    MCAutoValueRefBase<MCCanvasColorRef> t_color_value;
    if (!MCCanvasColorCreateWithRGBA(t_color.color.red/65535.0f, t_color.color.green/65535.0f, t_color.color.blue/65535.0f, 1.0f, &t_color_value))
        return false;
    if (!MCArrayStoreValue(p_props, false, p_prop_name, *t_color_value))
        return false;
    return true;
}
