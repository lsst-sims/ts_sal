/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "p_rtAmpState.h"


bool_t
xdr_rtAmpStateStruct(XDR *xdrs, rtAmpStateStruct *objp)
{
	if (!xdr_char(xdrs, &objp->rtAmpEnables)) {
		return (FALSE);
	}
	if (!xdr_char(xdrs, &objp->rtAmpFaults)) {
		return (FALSE);
	}
	if (!xdr_char(xdrs, &objp->rtCommand)) {
		return (FALSE);
	}
	return (TRUE);
}


