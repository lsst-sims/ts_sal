proc calcshmid { subsys } {
  set fout [open /tmp/subsys.tmp w]
  puts $fout "$subsys"
  close $fout
  set id [string range [exec md5sum /tmp/subsys.tmp] 0 3]
  return $id
}

#
#  Define SYSDIC(name,hasGenerics) when full UML -> XML generation is available
#  ts_xml MUST be updated in concert as each CSC is updated 
#
#  e.g. set SYSDIC(m2,hasGenerics) 1
#


set SYSDIC(systems) "accl archiver atMonochromator atWhiteLight auxscope calibrationSpectrometer calibrationElectrometer camera 
catchuparchiver dome domeADB domeAPS domeLouvers domeLWS domeMONCS domeTHCS eec environment EXA hexapod lasercal m1m3 m2ms MTMount network ocs operations power processingcluster rotator sequencer scheduler seeing skycam system tcs"
foreach s $SYSDIC(systems) {set SYSDIC($s,type) system}

set SYSDIC(datatypes) "byte short int long float string int64 double ubyte ushort uint ulong"
set SYSDIC(hexapod,keyedID) 1

set SYSDIC(hexapod,1) "m2mshexapod"
set SYSDIC(hexapod,2) "camerahexapod"

set TSYSDIC(subsystems) "<H2>Appendix A - System Dictionary</H2><P>
<H2>Contents : </H2>
<UL>
<H3>Introduction</H3>
<H3>Systems and Subsystems</H3>
<H3>Devices</H3>
<H3>Properties</H3>
<H3>Actions</H3>
<H3>Units</H3>
</UL><P><HR><P>
<H2>Introduction</H3>
<P>
This document describes the keywords used to refer to items in the
Service Abstraction Layer (SAL) mediated services. e.g. Subsystem Telemetry. 
<P>
The approved names for all subsystems, components, devices, actions, 
are defined in this dictionary. SAL generated code is validated against
the dictionary to ensure system wide consistency.
<P>
The dictionary will evolve as the individual subsystem descriptions
are completed to increasing levels of detail.
<P>
<H2>Systems and Subsystems</H2><P>"

set SYSDIC(subsystems) "accl.m1m3 - Main mirror accellerometers
accl.m2ms - M2 mirror accellerometers
accl.tma - Mount accellerometers
auxscope - Auxiliary telescope
auxscope.Application - Derived metadata
auxscope.Electrical - Electrical system monitoring
auxscope.Metrology - Positional information
auxscope.TC - Thermal monitoring
auxscope.Camera - Imaging system
auxscope.dome - Dome and vents
auxscope.Environment - Environmental monitoring
auxscope.Optics - Mirror support system
auxscope.Mount - Alt/az/rotr control
auxscope.Power - Power conditioning and monitoring
auxscope.Spectrometer - Spectroscopic instruments
auxscope.TCS - Pointing and control system
calibration - Calibration subsystem
calibration.Application - Derived metadata
calibration.Electrical - Electrical system monitoring
calibration.Metrology - Positional information
calibration.TC - Thermal monitoring
calibration.imager - Test/Commissioning camera
calibration.sources - Vis/IR calibration devices
calibration.mirror - Calibration source selection
camera - Camera sub-systems (* items not confirmed)
camera.BEE - Back-end electronics
camera.CALSYS - Calibration system
camera.CCC - Camera control system
camera.CCS - Camera control system
camera.Cluster - Camera control system
camera.GAS - Camera Guider acquisition system
camera.Prot - Lens protection system*
camera.FCS - Filter control system
camera.Filter - Filter control system
camera.FEE - Front-end electronics
camera.PCMS - Front-end electronics
camera.FEE - Front-end electronics
camera.PWR - Power supplies
camera.Power - Power supplies
camera.GDS - Guide sensors
camera.LASERCAL - Lens monitoring and metrology*
camera.QA - Data quality
camera.RAS - Raft Alignment system
camera.RNA - Raft controller
camera.SAS - Science Array
camera.SDS - Science Data System
camera.SCS - Shutter controller
camera.Shutter - Shutter controller
camera.TC - Temperature management
camera.Cold - Temperature management, BEE cold plate
camera.Cryo - Temperature management, Camera body
camera.T4U - Temperature management, Utility trunk
camera.T5S - Temperature management, cooling
camera.T6U - Temperature management, transport
camera.TCM - Timing and control module
camera.TC - Thermal control
camera.Purge - Vacuum control
camera.VCS - Vacuum control
camera.UTIL - Utility trunk
camera.WAS - Wavefront data acquisition
camera.WDS - Wavefront data system
camera.WFS - Wavefront sensing system
camera.WTCM - Wavefront sensors timing control
camera.XCAL - Sensor calibration system*
dm - Data management subsystems
dm.derived - Derived data quality
dm.ALERT - Alert events
dm.DB - Database statistics
dm.DQR - Raw data quality assessments
dm.DQS - Science data quality assessments
dm.pointing - Astrometry calibrations
dm.WCS - Astrometry calibrations
dm.raw - Raw metrics
dm.PIPE - Pipeline statuses
dome - Dome mechanisms
domeMOCS - Electrical system monitoring
domeADB - Dome rotation
dome.CapacitorBank - Capacitor bank
dome.Crane - Camera handling system
dome.Environment - Dome environmental sensing
dome.Liftplatform - Mirror handling system
domeLWS - Calibration screen
domeAPS - Dome aperture control
domeTHCS - Temperature control systems
domeLouvers - Dome vents and air-flow management
environment - Environmental monitoring
environment.Dust - Particulate monitoring
environment.Electrical - Electrical system monitoring
environment.Lightning - Electrical disturbance monitoring
environment.Seismometer - Earthquake monitoring
environment.TC - Temperature control systems
environment.Weather - Local weather sensing
environment.Video - Local video cameras
hexapod - Hexapod/Rotator subsystem
hexapod.Application - Derived metadata
hexapod.Actuators - Position control systems
hexapod.Electrical - Power monitoring
hexapod.LimitSensors - Position monitoring/control
hexapod.Metrology - Position monitoring/control
hexapod.TC - Thermal monitoring/control
lasercal - Laser calibration system
lasercal.Application - Derived metadata
lasercal.Electrical - Electrical system monitoring
lasercal.TC - Temperature control systems
m1m3 - Primary/Tertiary mirror
m1m3.Application - Derived metadata
m1m3.Actuators - Figure control systems
m1m3.ActuatorSample - Figure control systems
m1m3.Electrical - Power monitoring
m1m3.LimitSensors - Position monitoring/control
m1m3.LUT - Look-up tables
m1m3.LimitSensors - Limit sensors
m1m3.Metrology - Position monitoring/control
m1m3.Support - Mirror/Mount interface
m1m3.Supports - Mirror/Mount interface
m1m3.Surface - Figure monitoring
m1m3.TC - Thermal monitoring/control
m2ms - Secondary mirror
m2ms.Application - Derived metadata
m2ms.Actuators - Figure control systems
m2ms.Electrical - Power monitoring
m2ms.Hexapod - Hexapod monitoring
m2ms.LimitSensors - Position monitoring/control
m2ms.LUT - Look-up tables
m2ms.Metrology - Position monitoring/control
m2ms.Supports - Mirror/Mount interface
m2ms.Surface - Figure monitoring
m2ms.TC - Thermal monitoring/control
m2ms.Temps - Thermal monitoring/control
MTMount - Telescope mount
MTMount.Az - Azimuth axis monitoring/control
MTMount.Alt - Altitude axis monitoring/control
MTMount.Balance - Balance tuning mechanisms
MTMount.Hexapod - Camera mount monitoring/control
MTMount.Rotator - Camera mount monitoring/control
MTMount.Metrology - Configuration/Position/Velocity monitoring
MTMount.Application - Derived metadata
MTMount.Electrical - Electrical system monitoring
MTMount.Mirror_cover - Mirror covers
MTMount.Power - Monitoring/control of power management
MTMount.Services - Monitoring/control of N2/Vac/Coolant etc
MTMount.TC - Thermal monitoring/control
network - Computer network hardware
network.Application - Derived metadata
network.Camera - Camera specific
network.External - Links off-mountain
network.Noncritical - Non mission critical
network.Science - Science data networks
network.Telescope - On-site telescope and site
ocs - observatory control system
ocs.activity - observatory operational modes
ocs.database - observatory facility database
ocs.operator - Operator interfaces management
ocs.pointing - Telescope pointing
ocs.scheduler - observatory scheduling
ocs.staticanalysis - Debugging information
ocs.system - system overview
operations - Operator interfaces management
operations.main - Main telescope control
operations.base - Base control room
operations.cameralab - Camera checkout area
operations.dome - Dome floor
operations.mirrorlab - Mirror Lab and coating
operations.remote - WAN access
power - Power supply systems
power.Electrical - Electrical monitoring
power.Noncritical - Other power systems
power.Main - Main power monitoring/control , mission critical
power.TC - Thermal monitoring/control
power.UPSs - Uninterruptible power supplies
rotator - Camera rotator system
scheduler - Observation scheduling subsystem
scheduler.Application - Derived metadata
seeing.dimm - DIMM Seeing monitor system
seeing.dimm.Application - Derived metadata
seeing.dimm.Electrical - Electrical system monitoring
seeing.dimm.Metrology - Positional information
seeing.dimm.TC - Thermal monitoring
seeing.dimm.Imager - Sensor configuration and readout
seeing.dimm.Policy - Configuration and operations
seeing.dimm.Status - Summary data
seeing.dimm.Telescope - Telescope mount control
seeing.mass - MASS Seeing monitor system
seeing.mass - DIMM Seeing monitor system
seeing.mass.Application - Derived metadata
seeing.mass.Electrical - Electrical system monitoring
seeing.mass.Metrology - Positional information
seeing.mass.TC - Thermal monitoring
seeing.mass.Imager - Sensor configuration and readout
seeing.mass.Policy - Configuration and operations
seeing.mass.Status - Summary data
seeing.mass.Telescope - Telescope mount control
skycam - All-sky monitoring cameras
skycam.Application - Derived metadata
skycam.Electrical - Electrical system monitoring
skycam.Metrology - Positional information
skycam.TC - Thermal monitoring
skycam.Imager - Sensor configuration and readout
skycam.Policy - Configuration and operations
skycam.Status - Summary data
system - System configuration/history/status interfaces
system.Command  - Comprehensive command log
system.Computer - Per-computer status
system.Hardware - Modification logs
system.Software - Modification logs
tcs - Telescope control system
tcs.kernel - Telescope pointing kernel
tcs.MTMount - Mount interface
tcs.optics - Active optics interface"

foreach i [split $SYSDIC(subsystems) "\n" ] {
   set t [split $i "-"]
   set ss [string trim [lindex $t 0]]
   set title [string trim [lindex $t 1]]
   set SYSDIC($ss,title) $title
}

set TSYSDIC(devices) "<P><HR><P>
<H2>Devices</H2><P>	
Devices may occur in multiple instances in a particular system or subsystem, <P>
e.g.	camera.Raft.RNA5 , <BR>
	m1m3.actuators.actuator200
<P>"

set SYSDIC(devices) "Actuator - Mirror support structures
Accelerometer - Motion detector
Altitude - Altitude axis motion
A2D - Analog-digital converter
Application - Software component
Azimuth - Azimuth axis motion
Bogie - Dome bogie truck
Brake - Motion damping mechanism
CalibLamp - Calibration lamps
Clamp - Motion prevention/protection mechanism
CommandHistory - Software component
ComputerStatus - Computer hardware components
CO2Sensor - Carbon dioxide sensor
DomeAzimuth - Dome azimuth axis motion
DomeShutter - Dome slit position motion
DustMonitor - Dust monitoring devices
Electrical - Voltage and power consumption monitors
Fan - Air circulation mechanism
Filter - Filter positioning devices
Firmware - On-board flash memory device / EPROM
FuseSensor - Overcurrent sensor mechanism
GPS - GPS satellite receiver
GuideStars - Camera guide sensor devices
HandPaddle - Operator hand paddle devices
Hardware_revision - Hardware component
HexapodStrut - Hexapod active component
HumiditySensor - Humidity sensor
HVAC - Air conditioning unit
Image - CCD Camera or imaging sensor
LensCover - Camera lens protection mechanisms
LightningDetector - Lightning detection devices
LimitSensor - Limit of motion detection mechanism
Louver - Dome louver mechanism
Metrology - Position monitoring devices
Mirror - Beam steering / fold mirrors
MirrorCover - Mirror cover actuators and monitors
NetworkSwitch - Computer network switch/router/hub
O2Sensor - Oxygen sensor
PressureSensor - Pressure sensor
Raft - Camera sensor raft subsystems
RNA - Camera sensor raft network adaptor
Rotator - Camera rotator mechanism
SensorMetadata - Camera sensor performance
Seismometer - Seismic activity monitor
ServoMotor - Servo motor
Shutter - Dome shutter mechanism
SkyCamera - All sky coverage camera
SoftwareRevision - Software component
Spectrometer - Calibration telescope spectrograph
Status - Generic virtual device to query status
StepperMotor - Stepper motor
Support - Device support structures
Surface - Mirror surface control and monitoring
TempSensor - Temperature monitoring devices
UPS - Power management device
VacuumPump - Vacuum pump
Vents - Dome vent control
VideoCamera - Video camera subsystems
Weather - Weather monitoring devices and services
WFSRaft - Wavefront sensor raft
Wiper - Camera calibration source positioner"

set TSYSDIC(properties) " <P><HR><P>
<H2>Properties</H2>
<P>
Each device in a system or subsystem may have many different
properties associated with it. Properties are used for both
control and monitoring purposes.  ie Some properties may be
controlled; others may only monitored. In general, if a property can
be controlled there should also be another  item to allow monitoring so
that both the target and actual properties are logged.<P>"

set SYSDIC(properties) "acceleration - meters/second/second or steps/second/second
active - boolean state true/false
aborted - boolean state true/false
algorithm - software algorithm name
angle - in degrees
author - Name of a software author
bandwidth - of network links (bps, Mbps)
biasvoltage - ccd bias
blobtype - datatype for large vector/image/other objects
brightness - in lumens (?)
certificate - sofware security certificate
checksum - software check value
clamped - boolean state true/false
clockfreq - ccd clock frequency
cpu - manufacturer-architecture-revision
credential - software security credential
current - in amps, milli/micro
deadband -
declination - in ddd:mm:ss.sss
deceleration - meters/second/second or steps/second/second
delay - in seconds, milli/micro
demand - in amps, milli/micro
diq - Delivered image quality (metric TBD)
distance - in meters , centi/milli/micro
email - an email address
encoder - in raw counts
encoderConstant - in raw counts
epoch - yyyy.xxxxxxxx
equinox - yyyy.xxxxxxxx
errcode - a compact error code
errtext - a human readable error message
failure - boolean state true/false
filterName - Comprehensive descriptor
filterResponse - Filter response curve
focus - in microns
force - in Newtons
function - F(x) - Name of function
frequency - in Hz
fwhm - in arcseonds / pixels
gain - dimensionless multiplier
ha - Hour angle in hh:mm:ss.sss
index - integer enumerator
interface - Standard name
interlock - boolean state true/false
interpolator - Name of function
interval - integer value
ip4addr- an IP4 address
ip6addr - an IP6 address
jogspeed - arcsec per sec
loadCell - pressure in psi
loadLimit - in psi
loadRange - in psi
lvdt - load cell reading, raw data
macaddr - a network device MAC address
maximum - any numeric type
md5sum - software check value
memory - in kb, Mb, Gb
minimum - any numeric type
model - name of model (mathematical)
moving - boolean state true/false
motionProfile - vector tracking state of a moving device
motorRatio - dimensionless multiplier
motorSteps - steps per revolution
name - Descriptive
offset - in arcseconds
paraClock - ccd parallel clocking
pager - a pager number
period - in seconds
permission - software access credentials (r/w/e/d/c etc)
phone - a telephone number
position - Descriptive, may be numerical
power - boolean state true/false
pressure - in psi
ra - Right Ascension in hh:mm:ss.sss
regulated - boolean state true/false
resolution - in milli/micro/nano meters
revision - Software/Hardware code
retries - retry count, number of attempts allowed
retry - boolean state true/false
scale - dimensionless multiplier
serclock - ccd serial clocking
severity - a severity indication for a warning/error condition
simulator - boolean state true/false
speed - in meter/second
state - Descriptive (may be limited to a set)
status - Descriptive (may be limited to a set)
stepper - stepper motor count
stepperAccel - acceleration in steps/sec/sec
stepperPower - current in amps
stepperSpeed - steps/second
storageCapacity - in kb/Mb/Gb/Tb
success - boolean state true/false
temperature - in degrees C/K
threshold - dimensionless numeric
tolerance - dimensionless numeric
torque -
track - boolean state true/false
update - boolean state true/false
uri - universal resource identifier
username - login credential
vacuum - in tor (?)
velocity - in meters/second
voltage - in volts, milli/micro
wavelength - in Ang.,
width - dimensionless numeric
x - dimensionless numeric
y - dimensionless numeric
z - dimensionless numeric
zn - zernicke polynomial coefficients
zd - zenith distance - in degrees"


set TSYSDIC(actions) "<P><HR><P>
<H2>Actions</H2><P>
Every subsystem supports a set of commands which can be
issued by clients. Each command may optionally include one of the 
following actions, and may optionally include a set of action
specific operators to refine the request."

set SYSDIC(actions) "abort - immediate cessation of a specific command
ack - acknowlege receipt of external command
adjust - modify numeric parameter
arm - prepare a mechanism for motion
backup - save subsystem state
begin - start a cyclical activity
busy - an action state
cancel - cancel a command before it executes
capture - initiate data capture
center - move a mechanism to mid-range
clear - cancel all queued commands
close - close a mechanism (cover, shutter etc)
configure - loads a predefined configuration
continue - resume a paused command
delete - remove a queued command
disable - accept no more commands (except enable)
disarm - cancel an arming command
disengage - unlock a mechanism
done - an action state
dump - dump memory
enable - enable regular commanding
end - terminate a cyclical operation
engage - lock a mechanism
erase - clear memory
error - an action state
execute - execute a queued command
failed - an action state
free - free allocated buffers
go - initiate a prepared motion
index - move to an index mark
initialize - prepare a mechanism for use
insert - move component into operational location
install - software update
kill - immediately power down a component
mark - remember current location
maximize - maximize a tunable parameter
minimize - minimize a tunable parameter
move - generic motion command
off - generic state command
on - generic state command
open - open a mechanism (cover, shutter etc)
pause - temporarily pause queued commands
query - inquire command status
quit - exit from a series of queued commands (loop)
reboot - restart a computer/embedded system
remove - remove a command from a queue
reset - generic state command
restart - generic state command
restore - recover memory contents
resume - resume a paused command queue
save - snapshot subsystem state
select - generic state command
send - transmit data packet
set - generic state command
show - generic state command
start - generic state command
status - inquire component status
stop - generic state command
suspend - interrupt a command (may be resumed)
trigger - launch a previously armed operation"

set TSYSDIC(units) "<P><HR><P>
<H2>Units</H2><P>"

set SYSDIC(units) "Amps - amperes of electrical current (SI unit)
   Angstroms - wavelength 1A = 0.1 nanometers
   arcseconds - 1/3600 of a degree
   BTUs - British thermal unit of energy
   centimeters - 1/100 meter
   Coulombs - amount of charge displaced by a 1 Amp current in 1 second
   counts - dimensionless
   cycles - no of repeats of an operation
   day - time
   degrees - in Centigrade or Kelvin
   dynes - unit of force , 1 dyne = 10 micronewtons (cgs unit)
   Gauss - magnetic field of 1 maxwell per sq centimeter (cgs unit)
   Gb - Gigabytes 1024^3 for memory (1e9 bytes otherwise)
   Gbps - Gigabits per second
   grams - 1/1000 Kilogram
   Hertz - cycles per second
   hours - time
   Joules - unit of force/energy , 1 Joule = 1 Newton
   Kelvin - temperature (SI unit)
   Kilograms - mass (SU unit)
   Kilometers - 1000 meters
   Mb - Megabytes 1024^2 for memory (1e6 otherwise)
   Mbps - Megabits per second
   meters - length (SI unit)
   microns - 1/1000000 meter
   milliarcseconds - 1/3600000 of a degree
   millimeters - 1/000 meter
   minutes - time
   months - time
   Newtons - 1N = force required to acc 1 Kg mass at a rate of 1 m/s/s
   Ohms - unit of electrical resistance
   Pascals - 1 Pascal = 1 Newton per square meter or one Joule per cubic meter.
   radians - ratio between the length of an arc and its radius.
   RPM - revolutions per minute
   seconds - time
   steps - of a stepper motor
   Tb - Terabytes 1024^4 for memory (1e12 bytes otherwise)
   Volts - 1V = one Joule of energy per Coulomb of charge (SI derived)
   Watts - 1W = one Joule of energy per second (SI derived)
   years - time
"

proc add_system_dictionary_appendix { fout } {
global TSYSDIC SYSDIC
   foreach section "subsystems devices properties actions units" {
      puts $fout $TSYSDIC($section)
      puts $fout "<P><UL>"
      foreach i [split $SYSDIC($section) \n] {
         puts $fout "<B>[lindex $i 0]</B>[lrange $i 1 end]<BR>"
      }
      puts $fout "</UL><P>"
   }
}

proc updateSYSDICxml { } {
global SAL_DIR SYSDIC
  set fout [open $SAL_DIR/xml/SALSystemDictionary.xml w]
  puts $fout "
 <simpleType name=\"DateIdType\">
  <restriction base=\"string\">
   <pattern value=\"\\d\{4\}-\\d\{2\}-\\d\{2\}T\\d\{2\}:\\d\{2\}:\\d\{2\}:\\d\{2\}.\\d\{3\}\"/>
  </restriction>
 </simpleType>
 <simpleType name=\"anyURI\">                   <restriction base=\"string\"/>          </simpleType>
 <simpleType name=\"FrequencyType\">            <restriction base=\"decimal\"/>         </simpleType>
 <simpleType name=\"PublishersType\">           <restriction base=\"positiveInteger\"/> </simpleType>
 <simpleType name=\"ValuesPerPublisherType\">   <restriction base=\"positiveInteger\"/> </simpleType>
 <simpleType name=\"SizeInBytesType\">          <restriction base=\"positiveInteger\"/> </simpleType>
 <simpleType name=\"CountType\">                <restriction base=\"positiveInteger\"/> </simpleType>
 <simpleType name=\"InstancesPerNightType\">    <restriction base=\"positiveInteger\"/> </simpleType>
 <simpleType name=\"BytesPerNightType\">        <restriction base=\"positiveInteger\"/> </simpleType>
"
  puts $fout "
 <simpleType name=\"systemsEnum\">
  <restriction base=\"string\">"
  foreach i $SYSDIC(systems) {
    puts $fout "   <enumeration value=\"[string trim [lindex $i 0]]\"/>"
  }
  puts $fout "  </restriction>
 </simpleType>

"
  foreach category "subsystems devices properties actions units" {
    set all [split $SYSDIC($category) "\n"]
    puts $fout "
 <simpleType name=\"[set category]Enum\">
  <restriction base=\"string\">"
    foreach i $all {
      set cmd [string trim [lindex [split $i "-"] 1]]
      puts $fout "   <enumeration value=\"[string trim [lindex $i 0]]\"/><!-- $cmd -->" 
    }
    puts $fout "  </restriction>
 </simpleType>

"
  }
  close $fout
}





