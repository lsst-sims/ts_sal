/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "sysState.h"


bool_t
xdr_sysStateStruct(XDR *xdrs, sysStateStruct *objp)
{
	if (!xdr_u_int(xdrs, &objp->sysStateFlag)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->msecs)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->week)) {
		return (FALSE);
	}
	return (TRUE);
}


