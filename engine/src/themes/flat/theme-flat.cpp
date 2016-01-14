/*
 Copyright (C) 2016 LiveCode Ltd.
 
 This file is part of LiveCode.
 
 LiveCode is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License v3 as published by the Free
 Software Foundation.
 
 LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with LiveCode.  If not see <http://www.gnu.org/licenses/>.
 */

#include "themes/flat/theme-flat.h"


#include "libscript/script.h"
#include "theme-lcb.h"


// Handle to the LCB module implementing the flat theme
static MCScriptModuleRef s_module = NULL;

// Instance of the "flat" theme implemented in LCB
static MCControlThemeLCB* s_theme = NULL;


extern "C" bool com_livecode_themes_flat_Initialize()
{
    return true;
}

extern "C" void com_livecode_themes_flat_Finalize()
{
    delete s_theme;
    if (s_module != NULL)
        MCScriptReleaseModule(s_module);
    
    s_theme = NULL;
    s_module = NULL;
}


MCControlTheme* MCThemeGetFlat()
{
    // Create the theme object if it hasn't already been done
    if (s_theme == NULL)
    {
        // Get the handle of the LCB module implementing the theme
        if (!MCScriptLookupModule(MCNAME("com.livecode.themes.flat"), s_module))
            return NULL;
        
        if (!MCScriptEnsureModuleIsUsable(s_module))
            return NULL;
        
        // Create a theme from this module
        s_theme = new MCControlThemeLCB(s_module);
    }
    
    return s_theme;
}
