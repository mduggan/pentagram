/*
Copyright (C) 2003 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "pent_include.h"

#include "DisasmProcess.h"
#include "CoreApp.h"

// p_dynamic_cast stuff
DEFINE_RUNTIME_CLASSTYPE_CODE(DisasmProcess,Process);

DisasmProcess::DisasmProcess() : termCounter(120)
{
}


DisasmProcess::~DisasmProcess()
{
}

void DisasmProcess::run()
{
	if (flags & PROC_SUSPENDED)
		return;

	if(termCounter==0)
		CoreApp::get_instance()->ForceQuit();

	termCounter--;
}
