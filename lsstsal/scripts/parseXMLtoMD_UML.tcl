#!/usr/bin/tclsh

source $env(SAL_DIR)/add_system_dictionary.tcl
set TCODE(string) '_rC9ncFqtEeiGoKJ3GcWAXw'
set TCODE(short) '_rC9ncVqtEeiGoKJ3GcWAXw'
set TCODE(char) '_rC9nclqtEeiGoKJ3GcWAXw'
set TCODE(unsignedshort) '_rC9nc1qtEeiGoKJ3GcWAXw'
set TCODE(octet) '_rC9ndFqtEeiGoKJ3GcWAXw'
set TCODE(boolean) '_rC9ndVqtEeiGoKJ3GcWAXw'
set TCODE(long) '_rC9ndlqtEeiGoKJ3GcWAXw'
set TCODE(unsignedlong) '_rC9nd1qtEeiGoKJ3GcWAXw'
set TCODE(int) '_rC9ndlqtEejGoKJ3GcWAXw'
set TCODE(longlong) '_rC9neFqtEeiGoKJ3GcWAXw'
set TCODE(unsignedint) '_rC9ndlqtEekGoKJ3GcWAXw'
set TCODE(unsignedlonglong) '_rC9neVqtEeiGoKJ3GcWAXw'
set TCODE(float) '_rC-OgFqtEeiGoKJ3GcWAXw'
set TCODE(double) '_rC-OgVqtEeiGoKJ3GcWAXw'
set TCODE(byte) '_rC-OglqtEeiGoKJ3GcWAXw'

proc parseXMLtoUML { } { 
global IDLRESERVED SAL_WORK_DIR SAL_DIR CMDS CMD_ALIASES EVTS EVENT_ALIASES SYSDIC TCODE
global TLMS TLM_ALIASES EVENT_ENUM
   set fout [open SALXML_to_UML.xml w]
   puts $fout "<?xml version='1.0' encoding='UTF-8'?>
<xmi:XMI xmlns:uml='http://www.omg.org/spec/UML/20131001' xmlns:xmi='http://www.omg.org/spec/XMI/20131001' xmlns:StandardProfile='http://www.omg.org/spec/UML/20131001/StandardProfile' xmlns:MD_Customization_for_SysML__additional_stereotypes='http://www.magicdraw.com/spec/Customization/180/SysML' xmlns:DSL_Customization='http://www.magicdraw.com/schemas/DSL_Customization.xmi' xmlns:sysml='http://www.omg.org/spec/SysML/20150709/SysML' xmlns:MagicDraw_Profile='http://www.omg.org/spec/UML/20131001/MagicDrawProfile' xmlns:Units_and_DataTypes_Profile='http://www.magicdraw.com/schemas/Units_and_DataTypes_Profile.xmi' xmlns:MD_Customization_for_Requirements__additional_stereotypes='http://www.magicdraw.com/spec/Customization/180/Requirements' xmlns:Validation_Profile='http://www.magicdraw.com/schemas/Validation_Profile.xmi'>
	<xmi:Documentation>
		<xmi:exporter>MagicDraw UML</xmi:exporter>
		<xmi:exporterVersion>18.5</xmi:exporterVersion>
	</xmi:Documentation>
	<xmi:Extension extender='MagicDraw UML 18.5'>
		<plugin pluginName='SysML' pluginVersion='18.5'/>
		<plugin pluginName='Cameo Requirements Modeler' pluginVersion='18.5'/>
		<req_resource resourceID='1480' resourceName='Cameo Requirements Modeler' resourceValueName='Cameo Requirements Modeler'/>
		<req_resource resourceID='1440' resourceName='SysML' resourceValueName='SysML'/>
	</xmi:Extension>
		<uml:Model xmi:type='uml:Model' name='SAL_Model'>
			<packagedElement xmi:type='uml:Package' name='SAL_Interface'>"
   foreach subsys $SYSDIC(systems) {
    set ctype ""
    set eaid 1
    set donefirst 0
    set tname none
    set itemid 0
    set EANUM 1
    puts $fout "				    <packagedElement xmi:type='uml:Package' name='[set subsys]'>"
    foreach stype "Commands Events Telemetry" {
    if { [file exists [set subsys]_[set stype].xml] } {
     set fin [open [set subsys]_[set stype].xml r]
     puts $fout "					<packagedElement xmi:type='uml:Package' name='[set stype]'>"
     while { [gets $fin rec] > -1 } {
##puts stdout $rec
      set tag   [lindex [split $rec "<>"] 1]
      set value [lindex [split $rec "<>"] 2]
      if { $tag == "SALTelemetry" }    {set ctype "telemetry"}
      if { $tag == "SALEvent" }        {set ctype "event"}
      if { $tag == "SALTelemetrySet" } {
          set ctype "telemetry"
      }
      if { $tag == "SALCommandSet" }   {
          set ctype "command" 
      }
      if { $tag == "SALEventSet" }     {
          set ctype "event"
      }
      if { $tag == "Alias" }           {
          set alias $value
          set shouldbe [join [lrange [split $tname _] 2 end] "_"]
          if { $alias != $shouldbe } {
             puts stdout "ERROR - Alias does not match EFDB_Topic declaration for $alias"
             exit
          }
      }
      if { $tag == "Subsystem" }       {set subsys $value}
      if { $tag == "Enumeration" }     {set EVENT_ENUM($alias) "$item:$value" ; set ENUM($item) $value}
      if { $tag == "/SALTelemetry" || $tag == "/SALEvent" || $tag == "/SALCommand" } {
         puts $fout "						</packagedElement>"
      }
      if { $tag == "EFDB_Topic" } {
        set alias ""
        set itemid 0
        if { [info exists topics($value)] } { 
           puts stdout "ERROR - duplicate EFDB_Topic = $value"
           exit
        }
        set topics($value) 1
        set tname $value
        puts stdout "Translating $tname"
        puts $fout "						<packagedElement xmi:type='uml:Class' name='[set tname]'>"
      }
      if { $tag == "EFDB_Name" } {
        set item $value ; set unit "" ; set ENUM($item) ""
        incr itemid 1
        set desc "" ; set range "" ; set location ""
        set freq 0.054 ; set sdim 1
        if { [lsearch $IDLRESERVED [string tolower $item]] > -1 } {
           puts stdout "Invalid use of IDL reserved token $item"
           exit 1
        }
      }
      if { $tag == "IDL_Type"} {
         set type $value
         if { $type == "long long" } {set type "longlong"}
         if { $type == "unsigned long long" } {set type "unsigned longlong"}
         set ctype $TCODE([join $value ""])
      }
      if { $tag == "IDL_Size"}        {set sdim $value}
      if { $tag == "Description"}     {set desc [join [split $value "'<>"]]}
      if { $tag == "Frequency"}       {set freq $value}
      if { $tag == "Range"}           {set range $value}
      if { $tag == "Count"}           {set idim $value}
      if { $tag == "Units"}           {set unit $value}
      if { $tag == "/item" && $item != "priority" } {
         if { [info exists EATYPE($type)] == 0 } {
            set EAIDS($EANUM) $type
            set EATYPE($type) $EANUM
            incr EANUM 1
         }
         puts $fout "
								<ownedAttribute xmi:type='uml:Property' name=\"[set item]\" type=$ctype>
								<xmi:Extension extender='MagicDraw UML 18.5'>
									<modelExtension>
										<lowerValue xmi:type='uml:LiteralInteger' value='1'/>
									</modelExtension>
								</xmi:Extension>
								<xmi:Extension extender='MagicDraw UML 18.5'>
									<modelExtension>
									<upperValue xmi:type='uml:LiteralUnlimitedNatural' value='1'/>
								</modelExtension>
								</xmi:Extension>
								<xmi:Extension extender='MagicDraw UML 18.5'>
									<modelExtension>
									<lowerValue xmi:type='uml:Literal[set EATYPE($type)]' value='1'/>
									</modelExtension>
								</xmi:Extension>
								</ownedAttribute>
								<ownedComment xmi:type='uml:Comment'  body='[set desc]'>
									<annotatedElement />
								</ownedComment>
"
         if { $ENUM($item) != "" } {
            puts $fout "					<packagedElement xmi:type='uml:Enumeration'  name='[set item]'>"
            set ei 1
            foreach e [split $ENUM($item) ,] { 
                puts $fout "									<ownedLiteral xmi:type='uml:EnumerationLiteral' name='[set e]'/>"
                incr ei 1
            }
            puts $fout "					</packagedElement>"
         }
         incr eaid 1
      }
     }
     puts $fout "					</packagedElement>"
     close $fin
      }
     }
     puts $fout "				    </packagedElement>"
    }
    puts $fout "			</packagedElement>
			<packagedElement xmi:type='uml:Package' xmi:id='_rC9AbVqtEeiGoKJ3GcWAXw' name='IDL_Datatypes'>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9ncFqtEeiGoKJ3GcWAXw' name='string'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9ncVqtEeiGoKJ3GcWAXw' name='short'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9nclqtEeiGoKJ3GcWAXw' name='char'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9nc1qtEeiGoKJ3GcWAXw' name='unsigned short'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9ndFqtEeiGoKJ3GcWAXw' name='octet'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9ndVqtEeiGoKJ3GcWAXw' name='boolean'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9ndlqtEeiGoKJ3GcWAXw' name='long'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9nd1qtEeiGoKJ3GcWAXw' name='unsigned long'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9neFqtEeiGoKJ3GcWAXw' name='long long'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC9neVqtEeiGoKJ3GcWAXw' name='unsigned long long'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC-OgFqtEeiGoKJ3GcWAXw' name='float'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC-OgVqtEeiGoKJ3GcWAXw' name='double'/>
				<packagedElement xmi:type='uml:DataType' xmi:id='_rC-OglqtEeiGoKJ3GcWAXw' name='byte'/>
			</packagedElement>
		</uml:Model>
</xmi:XMI>
"
   close $fout
}


set IDLRESERVED "abstract any attribute boolean case char component const consumes context custom default double emits enum eventtype exception factory false finder fixed float getraises home import in inout interface local long module multiple native object octet oneway out primarykey private provides public publishes raises readonly sequence setraises short string struct supports switch true truncatable typedef typeid typeprefix union unsigned uses valuebase valuetype void wchar wstring"
set SAL_DIR $env(SAL_DIR)
set SAL_WORK_DIR $env(SAL_WORK_DIR)
source $SAL_DIR/add_private_idl.tcl
source $SAL_DIR/checkidl.tcl


