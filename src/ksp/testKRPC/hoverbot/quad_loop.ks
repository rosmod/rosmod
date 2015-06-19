function flightcontroller {
	set totalThrust to v(0,0,0).
	set i to 0. until i = engs:length {
		set thrust to engs[i]:facing:vector:normalized * engs[i]:thrust.
		set totalThrust to totalThrust + thrust.
		set vecs[i]:VEC to engs[i]:facing:vector:normalized * (engs[i]:thrustlimit / 20).
		set vecs[i]:START to engs[i]:position.
		set i to i + 1.
	}
	//set vecs[markThrustAcc]:VEC to totalThrust/mass.
	//engs along starvec: yaw
	//engs along topvec:  pitch
	
	inputs(). //check for user input
	
	set gravity to -up:vector:normalized * (body:mu / body:position:mag^2).
	set maxThr to MaxShipThrust().
	set maxTWR to maxThr / adjustedMass.
	set maxTWRVec to facing:vector * maxTWR.
	set weightRatio to gravity:mag/9.81.
	set throt to totalThrust:mag/maxThr.
	
	// height control ----------------------------------------------------------
	
	if mode = "follow" {
		if tarVeh:loaded {
			if tarPart = ship:rootpart { taggedPart(). console_init(). }  //target just got into range, look for part
			set targetPart to tarPart.
		}
		else { set targetPart to tarVeh. }
		set targetGeoPos to body:geopositionof(targetPart:position).
		set targetPos to targetPart:position.
	}
	else if mode = "hover" or mode = "free" or mode = "land" set targetGeoPos to ship:geoposition.
	else set targetPos to targetGeoPos:position.
	set targetPosXcl to vxcl(up:vector, targetPos).
	
	set v_vel to vdot(up:vector,velocity:surface).
	set h_vel to vxcl(up:vector,velocity:surface).
	
	set acc_expected to totalThrust/adjustedMass + gravity.
	set v_acc_expected to vdot(up:vector:normalized, acc_expected).
	set v_acc_e_old to v_acc_expected.
	
	
	set dT to time:seconds - tOld.
	set tOld to time:seconds.
	set v_dif to (velocity:surface - velold)/dT.
	set velold to velocity:surface.
	set vecs[markAcc]:VEC to v_dif.
	set h_acc to vxcl(up:vector, v_dif).
	set v_acc to vdot(up:vector, v_dif).
	
	// mass calibration
	if velocity:surface:mag < 0.5 and abs(throt - throtOld) < 0.01 and not(mode = "land") {
		set v_acc_difference to  v_acc_e_old - v_acc.
		set acc_list[accI] to v_acc_difference.
		if accI = 4 set accI to 0.
		else set accI to accI + 1.
		set acc_sum to 0. for acc_dif in acc_list {
			set acc_sum to acc_sum + acc_dif.
		}
		set v_acc_dif_average to acc_sum/20.
		//set adjustedMass to adjustedMass + 0.01 * v_acc_dif_average.
		set massOffset to max(-mass*0.05,massOffset + mass * 0.04 * v_acc_dif_average).
		
	}
	set adjustedMass to mass + massOffset.
	set throtOld to throt.
	
	//find highest point close to vehicle
	set posCheckGeo to ship:geoposition. 
	set posCheckHeight to posCheckGeo:terrainheight.
	for eng in engs {
		set curEngPos to eng:position.
		set curEngPos:mag to curEngPos:mag +4.
		set curGeo to body:geopositionof(curEngPos).
		if curGeo:terrainheight > posCheckGeo:terrainheight {
			set posCheckGeo to curGeo.
			set posCheckHeight to curGeo:terrainheight.
		}
	}
	
	//find highest point in predicted future position
	set predictedPos to v(0,0,0).
	set vel_at_pos to h_vel.
	set dVorig to desiredV - vel_at_pos.
	set i to 1. until i > 5 {
		
		set dV to desiredV - vel_at_pos.
		set predictedAcc to (dV:normalized * h_acc:mag) * min(1,max(0.01,(dV:mag/dVorig:mag))).
		set predictedPos to predictedPos + vel_at_pos + 0.5 * predictedAcc.
		set vel_at_pos to vel_at_pos + predictedAcc.
		
		set curGeo to body:geopositionof(predictedPos).
		
		//set terPos to curGeo:position + up:vector * tHeight.
		//set pm to pList[i].
		//set vecs[pm]:start to terPos.
		
		if curGeo:terrainheight > posCheckGeo:terrainheight {
			set posCheckGeo to curGeo.
			set posCheckHeight to curGeo:terrainheight.
		}
		if i = 5 and hasCamera set focusPos to curGeo:position.
		set i to i + 1.
	}
	
	// store / access last 10 high positions - use highest
	if lastT + 0.1 < time:seconds {
		set lastT to time:seconds.
		set posList[posI] to posCheckHeight.
		set highPos to posCheckHeight.
		for p in posList {
			set highPos to max(highPos,p).
		}
		set posI to posI + 1.
		if posI = 10 set posI to 0.
	}
	
	if targetPosXcl:mag < 300 set posCheckHeight to max(posCheckHeight,targetGeoPos:terrainheight).
	
	set tAlt to max(0,max(posCheckHeight, highPos)).
	if mode = "follow" and targetPosXcl:mag < 120 set tAlt to max(tAlt,tarVeh:altitude).
	set tAlt to tAlt + tHeight.
	set altErr to tAlt - altitude.
	
	// Suicide "burn" calculations ---------
	set acc_freefall to (vdot(up:vector,maxTWRVec * 0.05) - gravity:mag).//*(1 + weightRatio/10). //(1.1 on kerbin) the larger the modifier the more overshoot & steeper climb
	set acc_maxthr to (vdot(up:vector, maxTWRVec) - gravity:mag)*0.80. //being generous, lower expected maxacc
	
	if verticalspeed < 0 set max_acc to acc_maxthr.
	else set max_acc to acc_freefall.
	
	set burn_duration to abs(verticalspeed)/abs(max_acc).
	set burn_distance to (verticalspeed * burn_duration) + (0.5 * max_acc * (burn_duration^2)).
	
	set thMid to gravity:mag / (vdot(up:vector, maxTWRVec)).	
	if mode = "land" { 
		set vv to 0.
		if h_vel:mag < 1 set vv to -1 * max(0.5, alt:radar/6).
		set thOffset to PID_seek(PID_th, vv, verticalspeed)*0.2.
		//set burn_distance to 0. set altErr to 0.
	}
	else set thOffset to PID_seek(PID_th, altErr, burn_distance).
	set th to min(1,max(0.05*((1 + weightRatio)/2),thMid + thOffset)).
	if th < 0.06 {
		if burn_distance - (5*weightRatio) > altErr and altErr < (-2 * weightRatio) set th to max(0.05,min(0.9,stVec:mag/maxTWR)).
		set sensitivity to 4.
	}
	else set sensitivity to 1.
	// -------------------------------------------
	// max tilt to achieve desired vertical acceleration
	
	if burn_distance - altErr > 15 set maxAngNeut to 91.
	else set maxAngNeut to arccos(gravity:mag / max(gravity:mag*1.1,maxTWR)) - 5.
	if burn_distance - altErr < - 1 and verticalspeed < -0.5 { //ship will likely drop below target height
		if not(mode = "land" and altErr > -0.5) {
			set maxAngNeut to 0.
			set th to 1.
		}
	}
	//else if burn_distance - altErr > 3 and altErr > 5 set maxAngNeut to 90.
	set tilt to vang(up:vector,targetVec).
	//cap tilt
	if tilt > maxAngNeut { 
		set rotAxis to -vcrs(targetVec,up:vector).
		set targetVecCap to up:vector.
		set targetVecCap to angleaxis(maxAngNeut,rotAxis) * targetVecCap.
		updateVec(targetVecCap).
		if tilt > 92 set th to 1.
	}
	
	else updateVec(targetVec).
	
	// ----------------------------------------------
	// engine balancing
	
	set pitch_err to vdot(facing:topvector, targetVecTop).
	set yaw_err to vdot(facing:starvector, targetVecStar).
	
	set pitch_vel_target to pitch_err * 2.
	set yaw_vel_target to yaw_err * 2.
	set pitch_vel to -vdot(facing:starvector, ship:angularvel).
	set yaw_vel to vdot(facing:topvector, ship:angularvel).
	//set pitch_err to vang(targetVec, targetVecTop).
	//set yaw_err to vang(targetVec, targetVecStar).
	set throt to max(0.05,throt).
	set pitch_distr to PID_seek(PID_pitch, pitch_vel_target, pitch_vel) / throt. //returns 0-100
	//set pitch_distr to pitch_distr / ship:control:mainthrottle.
	set yaw_distr to PID_seek(PID_yaw, yaw_vel_target, yaw_vel) / throt.
	//set yaw_distr to yaw_distr / ship:control:mainthrottle.
	
	set eng_pitch_pos:thrustlimit to min(100, 100 + pitch_distr).
	set eng_pitch_neg:thrustlimit to min(100, 100 - pitch_distr).
	set eng_yaw_pos:thrustlimit to min(100, 100 + yaw_distr).
	set eng_yaw_neg:thrustlimit to min(100, 100 - yaw_distr).
	
	
	// -----------------------------------
	// Heading & pitch
	set TWR to maxTWR/9.81.
	set speedlimit to min(speedlimitmax,30*TWR).
	if mode = "free" {
		set targetPosXcl to heading(freeHeading,0):vector * 10000.
		set speedlimit to freeSpeed.
	}
	else if mode = "follow" set targetPosXcl:mag to targetPosXcl:mag - followDist.
	
	set vecs[markDestination]:start to targetPos + up:vector * 3.
	set vecs[markHorV]:vec to h_vel.
	if mode = "follow" set targetV to tarVeh:velocity:surface.
	else set targetV to v(0,0,0).
	
	if targetPosXcl:mag > 20 set desiredV to targetV + targetPosXcl:normalized * min(speedlimit,(targetPosXcl:mag^0.9)/(8/TWR)).
	else set desiredV to targetV + targetPosXcl:normalized * min(speedlimit,(targetPosXcl:mag^0.8) / (8/TWR)).
	
	if burn_distance - altErr < -5 and altErr > 0 {
		if (vdot(h_vel:normalized,desiredV) - vdot(h_vel:normalized,h_vel)) > 0 { set desiredV:mag to h_vel:mag. }
	}
	set vecs[markDesired]:vec to desiredV.
	
	if mode = "land" set targetVec to (up:vector * 5 + (up:vector - vxcl(up:vector, velocity:surface/2))):normalized.
	else if mode = "hover" set targetVec to up:vector. 
	else {
		set stVec to desiredV - h_vel.
		set rotAx to -vcrs(stVec, up:vector).
		set targetVec to up:vector.
		
		set weightMod to 1/((1 + weightRatio)/2).
		set targetVec to angleaxis(min(90,stVec:mag * 4 * sensitivity * weightMod), rotAx) * targetVec.
	}

	
	//-------------------------------------------
	
	// console
	//
	if time:seconds > consoleTimer + 0.1 {
		set base to 10.
		if mode = "hover" { }
		else if mode = "free" {
			print round(freeHeading,1) + "    " at (base,6).
			print round(freeSpeed,1) + " m/s " at (base,7).
			
		}
		else {
			print round(targetPosXcl:mag,2) + "m   " at (base,8).
			print round(vdot(up:vector,targetPos),2) + "m   " at (base,9).
			print round(targetGeoPos:lat,3) + " " at (base,11).
			print round(targetGeoPos:lng,3) + " " at (base,12).
			print round(targetGeoPos:heading,1) + "    " at (base,13).
			if mode = "follow" print "Follow dist: " + round(followDist,1) + "m  " at (0,15).
		}
		set base to c2 + 13.
		print round(alt:radar,1) + "m    " at (base,3).
		print tHeight + "m    " at (base,4).
		print round(altErr,1) + "m    " at (base,5).
		
		
		print round(speedlimit) + "m/s  " at (base,8).
		print round(h_vel:mag,1) + "m/s  " at (base,9).
		print round(h_acc:mag) + "   " at (base,10).
		print round(v_vel) + "m/s   " at (base,11).
		print round(v_acc) + "   " at (base,12).
		print round(tilt) + "  " at (base,13).
		
		print round(TWR,2) + "  " at (base,15).
		
		print round(thMid,2) + "  " at (base,17).
		print round(mass,3) + "t  " at (base,19).
		print round(adjustedMass,3) + "t   " at (base,22).
		print round(v_acc_dif_average,4) at (base,23).
		print round(1/dT,1) + "hz " at (base,24).
		
		set consoleTimer to time:seconds.
		
		//if ship:rootpart:tag = "v" {
		//	run q_var.
		//	set ship:rootpart:tag to "".
		//}
	}
	
	//set vecs[markStar]:vec to facing:starvector*4.
	//set vecs[markTop]:vec to facing:topvector*4.
	//set vecs[markFwd]:vec to facing:forevector*4.
	
	// Camera ------------------------------
	if hasCamera {
		//by default focusPos = expected position in 5s
		if mode = "follow" set focusPos to tarVeh:position.
		set vertError to vang(cam:facing:vector, vxcl(cam:facing:starvector,focusPos)).
		if vdot(cam:facing:topvector, focusPos) < 0 set vertError to -vertError.
		set horError to vang(cam:facing:vector, vxcl(cam:facing:topvector,focusPos)).
		if vdot(cam:facing:starvector, focusPos) < 0 set horError to -horError.
		
		print "vAng err: " + round(vertError,1) + "  " at (0,terminal:height-4).
		print "hAng err: " + round(horError,1) + "  " at (0,terminal:height-3).
		
		rotHMod:setfield("speed",abs(horError/8)).
		rotVMod:setfield("speed",abs(vertError/8)).
		if round(vertError,1) < 0 rotVMod:doaction("move +",true).
		else if round(vertError,1) > 0 rotVMod:doaction("move -",true).
		else { rotVMod:doaction("move +",false). rotVMod:doaction("move -",false). }
		
		if round(horError) < 0 rotHMod:doaction("move -",true).
		else if round(horError) > 0 rotHMod:doaction("move +",true).
		else { rotHMod:doaction("move +",false). rotHMod:doaction("move -",false). }
	}
	
	wait 0.
}