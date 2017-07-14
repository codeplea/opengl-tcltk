# OpenGL example with C and Tcl/Tk
# Copyright (c) 2015-2017 Lewis Van Winkle
#
# http://CodePlea.com
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.


package require Tk

update idletasks

proc bgerror {message} {
    tk_messageBox -message "Background error: $message\n\n$::errorInfo"
}


wm title . "OpenGL Example"


#Add in a basic menu, because what program doesn't have a menu?
option add *tearOff 0
menu .menubar
. configure -menu .menubar
menu .menubar.file
.menubar add cascade -menu .menubar.file -label File
.menubar.file add command -label "Exit" -command {exit 0}


#We'll use a frame control to draw our OpenGL.
set display [frame .opengl -width 300 -height 200 -background white -takefocus 1]
pack $display -fill both -expand 1


#Just showing that we can add other OpenGL controls too.
set bar [label .results -text {Super Basic OpenGL Example}]
pack $bar -fill x


#Tell C where to put the OpenGL
SetRenderWindow [winfo id $display]

#Tell C when the window resizes
bind $display <Configure> {Resize %w %h}

