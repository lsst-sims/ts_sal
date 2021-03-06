/*
 *  $Id: orte.h,v 0.0.0.1               2003/08/21 
 *
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 */

#ifndef _ORTE_H
#define _ORTE_H

#ifdef __cplusplus
extern "C" {
#endif

#define orte_assert(x)
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/if.h"      /* For struct ifreq, required by SIO* */
#include "net/if_types.h" /* For IFT_ETHER */
#include "net/if_dl.h"    /* For sockaddr_dl */
#include "sys/sockio.h"  /* For SIO* */

#include "orte_headers.h"
#include "orte/cdr.h"
#include "ul_gavl.h"
#include "ul_gavlcust.h"
#include "ul_gavlflesint.h"
#include "ul_gavlrepcust.h"
#include "ul_list.h"
#include "orte/typedefs_defines_rtps.h"
#include "ul_htimer.h"          //after typedefs_defines_rtps.h 
#include "defines.h"
#include "orte/defines_api.h"
#include "orte/typedefs_api.h"
#include "typedefs.h"
#include "protos.h"
#include "orte/protos_api.h"
#include "globals.h"
#include "rtps_endian.h"


#ifdef __cplusplus

} /* extern "C"*/
#endif

#endif /* _ORTE_H */
