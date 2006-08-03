/* the Music Player Daemon (MPD)
 * (c)2003-2006 by Warren Dukes (warren.dukes@gmail.com)
 * This project's homepage is: http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "buffer2array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int cstrtok(char *buffer, char *array[], const int max)
{
	int i = 0;
	char *c = buffer;

	while (*c != '\0' && i < max) {
		if (*c == '\"') {
			int escape = 0;
			array[i++] = ++c;
			while (*c != '\0') {
				if (*c == '\"') {
					if (escape)
						memmove(c - 1, c,
							strlen(c) + 1);
					else {
						*(c++) = '\0';
						break;
					}
				} else if (*c == '\\' && escape)
					memmove(c - 1, c, strlen(c) + 1);
				escape = (*(c++) != '\\') ? 0 : !escape;
			}
		} else {
			while (isspace(*c))
				++c;
			array[i++] = c++;
			if (*c == '\0')
				return i;
			while (!isspace(*c) && *c != '\0')
				++c;
		}
		if (*c == '\0')
			return i;
		*(c++) = '\0';
		while (isspace(*c))
			++c;
	}
	return i;
}
