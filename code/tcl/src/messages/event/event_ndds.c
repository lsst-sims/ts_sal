/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "event.h"


bool_t
xdr_eventStruct(XDR *xdrs, eventStruct *objp)
{
	if (!xdr_char(xdrs, &objp->dummyFiller)) {
		return (FALSE);
	}
	return (TRUE);
}


