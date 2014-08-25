/*
 * Please so not edit this file by hand!
 * This file was automatically generated by: nddsgen
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include "navPlanUpdateSet.h"


bool_t
xdr_navPlanUpdateSetStruct(XDR *xdrs, navPlanUpdateSetStruct *objp)
{
	if (!xdr_int(xdrs, &objp->taskMode)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->minTurningRadius)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->cameraFOV)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->rowWidth)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->speed)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->planningMode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->startNewPattern)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->patternType)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->useAbsGPSCoord)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->coords_dx, 20, sizeof(float), xdr_float)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->coords_dy, 20, sizeof(float), xdr_float)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->coords_dz, 20, sizeof(float), xdr_float)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->numCoords)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->startyaw)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->startroll)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->startdate)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->relFarmingLength)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->relFarmingWidth)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->spiralMaxRadius)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->spiralClockWise)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->sunFollowingRowLength)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->waypointOrientation)) {
		return (FALSE);
	}
	if (!xdr_float(xdrs, &objp->waypointTolerance)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->targetID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->sensorID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->planNewPlan)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->manplan, 40, sizeof(char), xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}


