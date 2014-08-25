/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "sensManStatus.h"


bool_t
xdr_sensManStatusStruct(XDR *xdrs, sensManStatusStruct *objp)
{
	if (!xdr_int(xdrs, &objp->sensorID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->status)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->extendedStatus)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->timeCost)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->powerCost)) {
		return (FALSE);
	}
	return (TRUE);
}


