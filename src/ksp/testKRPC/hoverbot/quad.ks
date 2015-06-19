// console initialize/reset
function console_init {
	clearscreen.
	
	
	print "HOVERBOT alpha - suicidal edition".
	horizontalLine(-1,"_"). 
	// left
	print "                    ". //2
	print "Mode: " + mode. //3								
	print " ". horizontalLineTo(4,0,19,"_").
	print " ". //5
	if mode = "hover" { print "Maintaining height". }
	else if mode = "free" { print "Heading: ". print "Speedlimit:". }
	else {
		if mode = "follow" {
			set s to tarVeh:name. print "TGT: " + s. //6
			if tarVeh:loaded { set s to tarPart:name. print "Part: " + s. } //7
			else print " ". //7
		}
		else {
			set s to vecs[markDestination]:label.
			print "TGT: " + s. //6
			print " ". //7
		}
		print "Distance:". //8
		print "Height D:".
		print " ".
		print "LAT:".
		print "LNG:".
		print "Heading:".
	}
	
	//print 
	verticalLineTo(20,2,terminal:height,"|").
	// right
	set c2 to 22.
	print "Radar alt:" at (c2,3).
	print "Hover alt:" at (c2,4).
	print "Alt error:" at (c2,5).
	horizontalLineTo(6,c2-1,terminal:width-1,"_").
	horizontalLineTo(7,c2-1,terminal:width-1," ").
	print "Vel limit: " at (c2,8).
	print "Hor vel:" at (c2,9).
	print "Hor acc:" at (c2,10).
	print "Vert vel:" at (c2,11).
	print "Vert acc:" at (c2,12).
	print "Pitch:" at (c2,13).
	print "TWR:" at (c2,15).
	print "Max V Acc:" at (c2,16).
	print "Neutral Thr:" at (c2,17).
	print "Eng Radius:  " + round(engDist,2) + "m" at (c2,18).
	print "Mass (drone):" at (c2,19).
	horizontalLineTo(20,c2-1,terminal:width-1,"_").
	print "Calibr Mass:" at (c2,22).
	print "Acc Error:" at (c2,23).
	print "Running at:" at (c2,24).
}

set terminal:width to 44.
set terminal:height to 25.
print "Booting up program..".
local torques is list().
list engines in engs.
ag1 off. ag2 off. ag3 off. ag4 off. ag5 off. ag6 off. ag7 off. ag8 off. ag9 off.
if MaxShipThrust() = 0 stage.

vecs_clear().
set engDist to 0.
local i is 0.
for eng in engs {
	vecs_add(eng:position,eng:facing:vector * eng:thrust,red,"").
	set vecs[i]:show to false.
	set eng:thrustlimit to 50.
	
	if vdot(facing:starvector,eng:position) < -0.5 { set eng_yaw_pos to eng.  }
	else if vdot(facing:starvector,eng:position) > 0.5 { set eng_yaw_neg to eng. }
	else if vdot(facing:topvector,eng:position) < -0.5 set eng_pitch_pos to eng.
	else if vdot(facing:topvector,eng:position) > 0.5 set eng_pitch_neg to eng.
	
	set engDist to engDist + vxcl(facing:vector,eng:position):mag.
	set i to i + 1.
}
set engDist to engDist/4.

//camera stuff --------------------------------------------------------
set hasCamera to false.
set cam to ship:partstagged("camera").
set camRotH to ship:partstagged("horizontal").
set camRotV to ship:partstagged("vertical").
if cam:length > 0 and camRotH:length > 0 and camRotV:length > 0 {
	set hasCamera to true.
	set cam to cam[0].
	set camMod to cam:getmodule("MuMechModuleHullCameraZoom").
	set camRotH to camRotH[0].
	set rotHMod to camRotH:getmodule("MuMechToggle").
	set camRotV to camRotV[0].
	set rotVMod to camRotV:getmodule("MuMechToggle").
}
//cam end -------------------------------------------------------------

// Vecdraws -----------------------------------------------------------
//local markThrustAcc is vecs_add(v(0,0,0),v(0,0,0),red,"Thr").
//local markStar is vecs_add(v(0,0,0),facing:starvector*5,rgba(1,1,0,0.1),"stb").
//local markTop is vecs_add(v(0,0,0),facing:topvector*5,rgba(1,0.8,0,0.12),"top").
//local markFwd is vecs_add(v(0,0,0),facing:forevector*5,rgba(1,0.6,0,0.14),"fwd").
local targetVec is up:forevector.
local targetVecStar is v(0,0,0).
local targetVecTop is v(0,0,0).
local markTar is vecs_add(v(0,0,0),targetVec*5,rgba(0.3,1,0,0.9),"tgt"). set vecs[markTar]:show to false.
//local markTarP is vecs_add(v(0,0,0),v(0,0,0),cyan,"TP").
//local markTarY is vecs_add(v(0,0,0),v(0,0,0),cyan,"TY").
local markAcc is vecs_add(v(0,0,0),v(0,0,0),green,"acc"). set vecs[markAcc]:show to false.
local markHorV is vecs_add(v(0,0,0),v(0,0,0),blue,"HV"). set vecs[markHorV]:show to false.
local markDesired is vecs_add(v(0,0,0),v(0,0,0),yellow,""). set vecs[markDesired]:show to false.
local markDestination is vecs_add(v(0,0,0),-up:vector * 3,rgb(1,0.8,0),"").

local pList is list().
//pList:add(0).
//pList:add(vecs_add(v(0,0,0),up:vector * 3,rgb(1,0,0.0),"")).
//pList:add(vecs_add(v(0,0,0),up:vector * 3,rgb(1,0.2,0.0),"")).
//pList:add(vecs_add(v(0,0,0),up:vector * 3,rgb(1,0.4,0.0),"")).
//pList:add(vecs_add(v(0,0,0),up:vector * 3,rgb(1,0.6,0.0),"")).
//pList:add(vecs_add(v(0,0,0),up:vector * 3,rgb(1,0.8,0.0),"")).

set stMark to false. ag8 off.
set thMark to false. ag9 off.
set miscMark to false. ag10 off.

function updateVec {
	parameter targetVec.
	set targetVecStar to vxcl(facing:topvector, targetVec).
	set targetVecTop to vxcl(facing:starvector, targetVec).
	set vecs[markTar]:vec to targetVec*5.
	//set vecs[markTarP]:vec to targetVecTop*5.
	//set vecs[markTarY]:vec to targetVecStar*5.
}

//function that checks for user key input
function inputs {
	if hasCamera {
		if ship:control:pilottop > 0 camMod:doevent("Activate Camera").
	}
	if mode = "free" { //manual direction
		if ship:control:pilotyaw <> 0 { // S+D
			set countHeading to countHeading + 1.
			if countHeading <= 10 set freeHeading to freeHeading + ship:control:pilotyaw/5.
			else set freeHeading to freeHeading + ship:control:pilotyaw * min(40,countHeading)/10.
			if freeHeading > 360 set freeHeading to freeHeading - 360.
			else if freeHeading < 0 set freeHeading to freeHeading + 360.
		}
		else {
			set countHeading to 0.
		}
		if ship:control:pilotpitch <> 0 { // W+S
			set freeSpeed to freeSpeed - ship:control:pilotpitch * min(5,max(0.5,abs(freeSpeed*0.1))).
			set freeSpeed to min(150,max(-150,freeSpeed)).
		}
	}
	else if mode = "pos" { //position shift
		if ship:control:pilotyaw <> 0 {
			set countE to countE + 1.
			if countE <= 10 set eastShift to -ship:control:pilotyaw * 0.05.
			else set eastShift to -ship:control:pilotyaw * (0.1 * min((countE-9)*2,80)).
			set targetGeoPosP to targetGeoPos:position + vcrs(north:vector,up:vector):normalized * eastShift.
			set targetGeoPos to body:geopositionof(targetGeoPosP).
		}
		else {
			set countE to 0.
		}
		if ship:control:pilotpitch <> 0 { 
			set countN to countN + 1.
			if countN <= 10 set northShift to -ship:control:pilotpitch * 0.05.
			else set northShift to -ship:control:pilotpitch * (0.1 * min((countN-9)*2,80)).
			set targetGeoPosP to targetGeoPos:position + vxcl(up:vector,north:vector):normalized * northShift.
			set targetGeoPos to body:geopositionof(targetGeoPosP).
			
		}
		else {
			set countN to 0.
		}
	}
	else if mode = "follow" {
		if ship:control:pilotpitch <> 0 {
			set followDist to max(0,followDist + ship:control:pilotpitch*0.5).
			console_init().
		}
	}
	if ship:control:pilotfore <> 0 { //target hover height
		set countH to countH + 1.
		set heightShift to ship:control:pilotfore * (0.05 * min(countH,20)).
		set tHeight to round(tHeight + heightShift,2).
	}
	else {
		set countH to 0.
	}
		
	//ui
	if ag9 { //thrusters
		ag9 off.
		local new is true.
		if thMark { set new to false. set thMark to false. }
		else set thMark to true.
		
		set i to 0.
		for eng in engs {
			set vecs[i]:show to new.
			set i to i + 1.
		}
		HUDTEXT("Thrust balance vector display toggled", 5, 2, 15, yellow, false).
		//set vecs[markThrustAcc]:show to true.
	}
	else if ag8 { //steering stuff
		ag8 off.
		local new is true.
		if stMark { set new to false. set stMark to false. }
		else set stMark to true.
		
		set vecs[markHorV]:show to new.
		set vecs[markDesired]:show to new.
		HUDTEXT("Velocity and target velocity vector display toggled", 5, 2, 15, yellow, false).
	}
	else if ag10 { //misc
		ag10 off.
		local new is true.
		if miscMark { set new to false. set miscMark to false. }
		else set miscMark to true.
		set vecs[markTar]:show to new.
		set vecs[markAcc]:show to new.
		
		HUDTEXT("Steering and acceleration vector display toggled", 5, 2, 15, yellow, false).
	}
	//modes
	else if ag2 { 
		ag2 off.
		set mode to "hover".
		console_init().
		set vecs[markDestination]:show to false.
		HUDTEXT("Canceling velocity", 5, 2, 15, yellow, false).
	}
	else if ag3 {
		ag3 off.
		set mode to "land".
		console_init().
		set vecs[markDestination]:show to false.
		HUDTEXT("Landing", 5, 2, 15, yellow, false).
	}
	else if ag4 { //free
		ag4 off.
		set mode to "free".
		set freeSpeed to 0.
		set freeHeading to 90.
		console_init().
		set vecs[markDestination]:show to false.
		HUDTEXT("Freeroam mode", 5, 2, 15, yellow, false).
		ag8 on.
	}
	else if ag5 {
		ag5 off.
		if targetString = "POOL" { set targetGeoPos to geo_bookmark("LAUNCHPAD"). set targetString to "LAUNCHPAD". }
		else if targetString = "LAUNCHPAD" { set targetGeoPos to geo_bookmark("VAB"). set targetString to "VAB". }
		else if targetString = "VAB" { set targetGeoPos to geo_bookmark("RUNWAY E"). set targetString to "RUNWAY E". }
		else if targetString = "RUNWAY E" { set targetGeoPos to geo_bookmark("RUNWAY W"). set targetString to "RUNWAY W". }
		else if targetString = "RUNWAY W" { set targetGeoPos to geo_bookmark("ISLAND W"). set targetString to "ISLAND W". }
		else if targetString = "ISLAND W" { set targetGeoPos to geo_bookmark("POOL"). set targetString to "POOL". }
		else { set targetGeoPos to geo_bookmark("LAUNCHPAD"). set targetString to "LAUNCHPAD". }
		set mode to "pos".
		set vecs[markDestination]:show to true.
		set vecs[markDestination]:label to targetString.
		HUDTEXT("Bookmark location: " + targetString, 5, 4, 15, yellow, false).
		console_init().
	}
	else if ag6 {
		ag6 off.
		set targetGeoPos to ship:geoposition.
		set targetString to "LOCAL".
		set mode to "pos".
		set vecs[markDestination]:label to targetString.
		set vecs[markDestination]:show to true.
		console_init().
		HUDTEXT("Location mode", 5, 2, 15, yellow, false).
	}
	else if ag7 { //target vehicles
		ag7 off.
		list targets in targs2.
		if targs:length <> targs2:length list targets in targs. //update target list
		local done is false.
		until done {
			if targs[target_i]:position:mag < 50000 {
				set done to true.
				set mode to "follow".
				set tarVeh to targs[target_i].
				
				if tarVeh:loaded { taggedPart(). }
				else { set tarPart to ship:rootpart. set vecs[markDestination]:label to tarVeh:name. }
				HUDTEXT("Following " + tarVeh:name, 5, 2, 15, yellow, false).
			}
			set target_i to target_i + 1.
			if target_i = targs:length set target_i to 0.
		}
		set vecs[markDestination]:show to true.
		console_init().

	}
}

function taggedPart {
	set tagged to tarVeh:PARTSTAGGED("attach").
	if tagged:length > 0 { 
		set tarPart to tagged[0].
		set vecs[markDestination]:label to tarVeh:name + " - " + tarPart:name.
	}
	else { set tarPart to tarVeh:rootpart. set vecs[markDestination]:label to tarVeh:name. }
}

list targets in targs.
set target_i to 0.
set tarPart to 0.
set adjustedMass to mass.
set localTWR to (MaxShipThrust() / adjustedMass)/(body:mu / body:position:mag^2).
global PID_pitch is PID_init(50.0,0,0,-100,100).
global PID_yaw is PID_init(50.0,0,0,-100,100).
global PID_th is PID_init((0.6/localTWR),0.0002,0.1,-1,1). //(0.5,0.0002,0.1,-1,1)
set v_acc_e_old to 0.
local tOld is time:seconds. local velold is velocity:surface.
global tHeight is round(min(50,alt:radar),2).
local th is 0.
local posI is 0.
local accI is 0.
local throtOld is 0.
local lastT is time:seconds - 1000.
local acc_list is list().
set i to 0. until i = 5 { acc_list:add(0). set i to i + 1. }
local posList is list().
set i to 0. until i = 10 { posList:add(ship:geoposition:terrainheight). set i to i + 1. }
lock throttle to th.
global thrust_toggle is true. 
set targetGeoPos to ship:geoposition.
set targetString to "LOCAL".
set mode to "pos".
set massOffset to 0.

set speedlimitmax to 100.
set consoleTimer to time:seconds.
set desiredV to v(0,0,0).
set v_acc_dif_average to 0.
set followDist to 0.

run quad_loop.
console_init().

//main controller loop
until ag1 {
	flightcontroller().
}

vecs_clear().
set th to throt.
unlock throttle.
set ship:control:pilotmainthrottle to throt.