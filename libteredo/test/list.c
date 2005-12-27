/*
 * list.c - Libteredo peer list tests
 * $Id$
 */

/***********************************************************************
 *  Copyright (C) 2005 Remi Denis-Courmont.                            *
 *  This program is free software; you can redistribute and/or modify  *
 *  it under the terms of the GNU General Public License as published  *
 *  by the Free Software Foundation; version 2 of the license.         *
 *                                                                     *
 *  This program is distributed in the hope that it will be useful,    *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               *
 *  See the GNU General Public License for more details.               *
 *                                                                     *
 *  You should have received a copy of the GNU General Public License  *
 *  along with this program; if not, you can get it from:              *
 *  http://www.gnu.org/copyleft/gpl.html                               *
 ***********************************************************************/

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdbool.h>

#if HAVE_STDINT_H
# include <stdint.h> /* Mac OS X needs that */
#endif
#include <sys/types.h>
#include <netinet/in.h>

#include "teredo.h"
#include "relay.h"
#include "peerlist.h"

int main (void)
{
	teredo_peerlist *l;
	struct in6_addr addr = { };
	unsigned i;

	// test empty list
	l = teredo_list_create (0);
	if (l == NULL)
		return -1;
	else
	{
		bool create;

		if (teredo_list_lookup (l, &addr, &create) != NULL)
			return -1;

		teredo_list_destroy (l);
	}

	// test real list
	l = teredo_list_create (255);
	if (l == NULL)
		return -1;

	for (i = 0; i < 256; i++)
	{
		teredo_peer *p;
		bool create;

		addr.s6_addr[12] = i;
		p = teredo_list_lookup (l, &addr, i & 1 ? &create : NULL);
		if (i & 1)
		{
			// item should have been created
			if ((!create) || (p == NULL))
				return -1;
			teredo_list_release (l);
		}
		else
		{
			// item did not exist and should not have been found
			if (p != NULL)
				return -1;
		}
	}

	/* FIXME can't test lookup until IsExpired() / Touch*() are handled
	* properly inside the peer list */
	teredo_list_destroy (l);

	return 0;
}