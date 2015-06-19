// parameter 1: A string or index number based on the list below.
// returns: a geoposition
function geo_bookmark {
	parameter bookmark.
	
	
	if bookmark = 1 or bookmark = "LAUNCHPAD" or bookmark = "KSC" return LATLNG(-0.0972078822701718, -74.5576864391954). //Kerbal space center
	else if bookmark = 2 or bookmark = "RUNWAY E" return LATLNG(-0.0502131096942382, -74.4951289901873). //East
	else if bookmark = 3 or bookmark = "RUNWAY W" return LATLNG(-0.0486697432694389, -74.7220377114077). //West
	else if bookmark = 4 or bookmark = "VAB" return LATLNG(-0.0967646955755359, -74.6187122587352). //VAB Roof
	
	else if bookmark = 5 or bookmark = "IKSC" return latlng(20.3926,-146.2514). //inland kerbal space center
	else if bookmark = 6 or bookmark = "ISLAND W" return LATLNG(-1.5173500701556, -71.9623911214353). //Island/airfield runway west
	else if bookmark = 7 or bookmark = "ISLAND E" return LATLNG(-1.51573303823027, -71.8571463011229).//Island/airfield runway east
	else if bookmark = 8 or bookmark = "POOL" return LATLNG(-0.0867719193943464, -74.6609773699654).
	//else if bookmark = "" return .
	
	
	else { print "ERROR: geolocation bookmark " + bookmark + " not found!". return latlng(90,0). }
}

//////////////////// User Interface stuff /////////////////// 
function horizontalLine {
	parameter line,char.
	local i is 0.
	local s is "".
	until i = terminal:width {
		set s to char + s.
		set i to i + 1.
	}
	if line < 0 print s. //print to next line
	else print s at (0,line).
}
function horizontalLineTo {
	parameter line,colStart,colEnd,char.
	local column is colStart.
	local s is "".
	until column > colEnd {
		set s to char + s.
		set column to column + 1.
	}
	print s at (colStart,line).
}
function verticalLineTo {
	parameter column,lineStart,lineEnd,char.
	local line is lineStart.
	until line > lineEnd {
		print char at (column,line).
		set line to line + 1.
	}
}

function clearLine {
	parameter line.
	local i is 0.
	local s is "".
	until i = terminal:width {
		set s to " " + s.
		set i to i + 1.
	}
	print s at (0,line).
}



/////////////////////// VECTORS ///////////////////////////

// vecs_clear().
function vecs_clear {
	if vecs:length > 0 {
		for vd in vecs {
			set vd:SHOW TO false.
		}
		vecs:clear.
	}
}

// set [variable] to vecs_add([position],[vector],[color],[string]).
// returns: list index. 
// example: 
//  Create a vecdraw:
//  set velocityVec to vecs_add(ship:position,velocity:orbit,blue,round(velocity:orbit:mag) + " m/s").
//  Update it's starting position:
//  set vecs[velocityVec]:start to ship:position.
function vecs_add {
	parameter p,v,c,descr.
	vecs:add(VECDRAWARGS(p, v, c, descr, 1, true)).
	return vecs:length - 1.
}

global vecs is list().
if vecs:length > 0 vecs_clear().

//////////////////////////////////////////////////////////

function MaxShipThrust
{
    LIST engines IN engs.
    local mth to 0.
    for eng in engs {
        if eng:SLISP > 0 and eng:IGNITION = TRUE {
            set mth to mth + eng:MAXTHRUST* (eng:ISP/eng:VISP).
        }
    }
    return mth.
}
function nz { //"not zero" , NaN protection
	parameter float.
	if abs(float) < 0.001 {
		set float to 0.001.
	}
	return float.
}

///////////////////////// ksLIB team's PID controller ////////////////////////////
// This file is distributed under the terms of the MIT license, (c) the KSLib team

@LAZYGLOBAL off.

function PID_init {
  parameter
    Kp,      // gain of position
    Ki,      // gain of integral
    Kd,      // gain of derivative
    cMin,  // the bottom limit of the control range (to protect against integral windup)
    cMax.  // the the upper limit of the control range (to protect against integral windup)

  local SeekP is 0. // desired value for P (will get set later).
  local P is 0.     // phenomenon P being affected.
  local I is 0.     // crude approximation of Integral of P.
  local D is 0.     // crude approximation of Derivative of P.
  local oldT is -1. // (old time) start value flags the fact that it hasn't been calculated
  local oldInput is 0. // previous return value of PID controller.

  // Because we don't have proper user structures in kOS (yet?)
  // I'll store the PID tracking values in a list like so:
  //
  local PID_array is list(Kp, Ki, Kd, cMin, cMax, SeekP, P, I, D, oldT, oldInput).

  return PID_array.
}.

function PID_seek {
  parameter
    PID_array, // array built with PID_init.
    seekVal,   // value we want.
    curVal.    // value we currently have.

  // Using LIST() as a poor-man's struct.

  local Kp   is PID_array[0].
  local Ki   is PID_array[1].
  local Kd   is PID_array[2].
  local cMin is PID_array[3].
  local cMax is PID_array[4].
  local oldS   is PID_array[5].
  local oldP   is PID_array[6].
  local oldI   is PID_array[7].
  local oldD   is PID_array[8].
  local oldT   is PID_array[9]. // Old Time
  local oldInput is PID_array[10]. // prev return value, just in case we have to do nothing and return it again.

  local P is seekVal - curVal.
  local D is oldD. // default if we do no work this time.
  local I is oldI. // default if we do no work this time.
  local newInput is oldInput. // default if we do no work this time.

  local t is time:seconds.
  local dT is t - oldT.

  if oldT < 0 {
    // I have never been called yet - so don't trust any
    // of the settings yet.
  } else {
    if dT > 0 { // Do nothing if no physics tick has passed from prev call to now.
     set D to (P - oldP)/dT. // crude fake derivative of P
     local onlyPD is Kp*P + Kd*D.
     if (oldI > 0 or onlyPD > cMin) and (oldI < 0 or onlyPD < cMax) { // only do the I turm when within the control range
      set I to oldI + P*dT. // crude fake integral of P
     }.
     set newInput to onlyPD + Ki*I.
    }.
  }.

  set newInput to max(cMin,min(cMax,newInput)).

  // remember old values for next time.
  set PID_array[5] to seekVal.
  set PID_array[6] to P.
  set PID_array[7] to I.
  set PID_array[8] to D.
  set PID_array[9] to t.
  set PID_array[10] to newInput.

  return newInput.
}.