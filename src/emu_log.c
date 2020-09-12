/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/emu_log.h"

struct emu_logging
{
	enum emu_log_level loglevel;
	void (*emu_log_fn)(struct emu *e, enum emu_log_level level, const char *msg);
};


struct emu_logging *emu_log_new()
{
	struct emu_logging *el = (struct emu_logging *)malloc(sizeof(struct emu_logging));
	if( el == NULL )
	{
		return NULL;
	}
	memset(el, 0, sizeof(struct emu_logging));
	
	return el;
}

void emu_log_free(struct emu_logging *el)
{
	free(el);
}

void emu_log_level_set(struct emu_logging *el, enum emu_log_level level)
{
	el->loglevel = level;
}

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...)
{
	struct emu_logging *el = emu_logging_get(e);

	if ( el->loglevel == EMU_LOG_NONE )
		return;

	if ( el->loglevel < level )
		return;


	va_list         ap;
	char            *message;

	va_start(ap, format);
	vasprintf(&message, format, ap);
	va_end(ap);

	if ( el->emu_log_fn == NULL )
	{
		const char *lev[] = {"none","\033[32;1minfo\033[0m","\033[31;1mdebug\033[0m"};
		fprintf(stdout,"[emu 0x%08x %s ] ",(unsigned int)e, lev[level]);
		fprintf(stdout,"%s", message);
	}
	else
		el->emu_log_fn(e, level, message);

	free(message);
}
