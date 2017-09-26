#
#  Generate Generic SAL Client instances, based on list of Telemetry/Events
#  to be monitored
#




proc genericclientfragment { fout id ctype } {
global ACTORTYPE SAL_WORK_DIR SALCLIENTS
   set ptypes $SALCLIENTS($id,telemetry)
   foreach j $ptypes {
     set subsys  [lindex [split $j _] 0]
     set type [lindex [split $j _] 1]
     set topic [join [lrange [split $j _] 1 end] _]
      if { $ctype == "subscriber" } {
         puts $fout "  mgr_[set subsys].salTelemetrySub(\"[set j]\");"
         if { [info exists SALCLIENTS($id,[set topic])] } {
            if { [lindex $SALCLIENTS($id,[set topic]) 0] == "publish" } {
               puts $fout "  mgr_[set subsys].salTelemetryPub(\"[lindex $SALCLIENTS($id,[set topic]) 1]\");"
            }
         }
      }
      if { $ctype == "getsamples" } {
         if { [info exists SALCLIENTS($id,[set j])] == 0 } {
           puts $fout "    status = getSample_[set topic](client_[set id]_buffer.instanceOf_[set topic]);"
         }
      }
      if { $ctype == "onevent" } {
         if { [info exists SALCLIENTS($id,[set j])] } {
           puts $fout "  
    status = getSample_[set topic](client_[set id]_buffer.instanceOf_[set topic]);
    if (status == SAL__OK) \{"
           if { [lindex $SALCLIENTS($id,[set j]) 0] == "publish" } {
              puts $fout "      status = publish_[lindex $SALCLIENTS($id,[set j]) 1]();"
           } else {
              puts $fout "      status = [set SALCLIENTS($id,[set j])]();"
           }
           puts $fout "    \}"
         }
      }
   }
}

proc copyincludefiles { id } {
global SAL_WORK_DIR SALCLIENTS
   set ptypes $SALCLIENTS($id,telemetry)
   foreach j $ptypes {
     set s [lindex [split $j _] 0]
     exec cp $SAL_WORK_DIR/$s/cpp/src/SAL_[set s].h $SAL_WORK_DIR/salClient/cpp/src/.
     exec cp $SAL_WORK_DIR/$s/cpp/src/SAL_[set s]C.h $SAL_WORK_DIR/salClient/cpp/src/.
     exec cp $SAL_WORK_DIR/$s/cpp/sal_[set s]Dcps.h $SAL_WORK_DIR/salClient/cpp/src/.
     exec cp $SAL_WORK_DIR/$s/cpp/ccpp_sal_[set s].h $SAL_WORK_DIR/salClient/cpp/src/.
     exec cp $SAL_WORK_DIR/$s/cpp/sal_[set s]Dcps_impl.h $SAL_WORK_DIR/salClient/cpp/src/.
   }
}




proc gensalclientmake { id } {
global SAL_DIR SAL_WORK_DIR env
  set frep [open /tmp/sreplace6.sal w]
  puts $frep "#!/bin/sh"
  exec touch $SAL_WORK_DIR/salClient/cpp/src/.depend.Makefile.sacpp_salClient_[set id]
  exec cp  $SAL_DIR/code/templates/Makefile.sacpp_SAL_client.template $SAL_WORK_DIR/salClient/cpp/src/Makefile.sacpp_salClient_[set id]
  puts $frep "perl -pi -w -e 's/_SALCLIENT/_salClient_[set id]/g;' $SAL_WORK_DIR/salClient/cpp/src/Makefile.sacpp_salClient_[set id]"
  close $frep
  exec chmod 755 /tmp/sreplace6.sal
  catch { set result [exec /tmp/sreplace6.sal] } bad
  if { $bad != "" } {puts stdout $bad}
}


proc gentelemetryclient { id } {
global SAL_WORK_DIR SYSDIC SALCLIENTS
   exec mkdir -p $SAL_WORK_DIR/salClient/cpp/src
   set fout [open $SAL_WORK_DIR/salClient/cpp/src/sacpp_salClient_[set id].cpp w]
   puts $fout "
/*
 * This file contains the implementation for the salClient generic Telemetry client.
 *
 ***/

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <mysql.h>
#include \"SAL_actors.h\"
#include \"os.h\"
using namespace DDS;"
   foreach s $SALCLIENTS($id,subsystems) {
       puts $fout "
#include \"SAL_[set s].h\"
#include \"ccpp_sal_[set s].h\"
"
   }
   puts $fout "
struct SAL_client_$id \{"
   foreach t $SALCLIENTS($id,telemetry) {
       puts $fout "  [set t]C instanceOf_[set t];"
   }
   puts $fout "\}

SAL_client_$id client_[set id]_buffer;

/* entry point exported and demangled so symbol can be found in shared library */
extern \"C\"
\{
  OS_API_EXPORT
  int test_salClient_telemetry_salclient_[set id]();
\}

int test_salClient_telemetry_salclient_[set id]()
\{   
  os_time delay_1ms = \{ 0, 1000000 \};
  int numsamp = 0;
  int actorIdx = 0;
  int mstatus = 0;
  int status = 0;
"
  foreach subsys $SALCLIENTS($id,subsystems) {
     puts $fout "  SAL_[set subsys] mgr_[set subsys] = SAL_[set subsys]();"
  }
  genericclientfragment $fout $id subscriber
  puts $fout "
  while (1) \{
"
  genericclientfragment $fout $id getsamples
  genericclientfragment $fout $id onevent
  puts $fout "
    os_nanoSleep(delay_1ms);
  \}

  /* Remove the DataWriters etc */
  return 0;
\}

int main (int argc, char **argv[])
\{
  return test_salClient_telemetry_salclient_[set id]();
\}
"
   close $fout
}


proc genclientpublisher { fout id } {
global SAL_WORK_DIR SALCLIENTS
   set ptypes $SALCLIENTS($id,telemetry)
   foreach j $ptypes {
      set subsys  [lindex [split $j _] 0]
      if { [info exists SALCLIENTS($id,[set j]) ] } {
       if { [lindex $SALCLIENTS($id,[set j]) 0] == "publish" } {
         puts $fout "int publish_[lindex $SALCLIENTS($id,[set j]) 1]()
\{
  int status;
  int priority;
  [set subsys]_logevent_LargeFileObjectAvailableC LFOA_sample;

  strcpy(LFOA_sample.URL,\"http://lsst.summit.efd/2017/12/12/1234567/sample_header.fits\");
  strcpy(LFOA_sample.Generator,\"DM Header Service\");
  LFO_sample.Version = 1.0;
//  md5sum(&LFOA_sample.Checksum,\"file:///2017/12/12/1234567/sample_header.fits\");
  strcpy(LFOA_sample.Mime_Type,\"application/fits\");
//  LFOA_sample.Byte_Size = getFileSize(\"file:///2017/12/12/1234567/sample_header.fits\");
  SAL_[set subsys] mgr = SAL_[set subsys]();
  mgr.salEvent(\"[set subsys]_logevent_LargeFileObjectAvailable\");
  priority=1;
  mgr.logEvent_LargeFileObjectAvailable(&LFOA_sample,priority);
  cout << \"=== Event [set subsys]_logevent_LargeFileObjectAvailable generated = \" << endl;
  sleep(1);

  /* Remove the DataWriters etc */
  mgr.salShutdown();

  return 0;
\}
"
       } else {
         puts $fout "int [set SALCLIENTS($id,[set j])]()
\{
  int status;
\}
"
       }
     }
   }
}


proc startsalclients { clients } { 
global SAL_WORK_DIR
   foreach id $clients {
      if { [file exists $SAL_WORK_DIR/salClients/cpp/src/sacpp_telemetry_salclient_[set id]] == 0 } {
         puts sdout "ERROR : No telemetry client available for $id"
         exit
      }
      set tid($s) [exec $SAL_WORK_DIR/salClients/cpp/src/sacpp_telemetry_salclient_[set id] >& [set id]_telemetry_[clock seconds].log &]
   }
   while { 1 } {
     foreach id $clients {
       set bad [catch {exec ps -F --pid $tid($s)} res]
       if { $bad } {
          set tid($s) [exec $SAL_WORK_DIR/salClients/cpp/src/sacpp_telemetry_salclient_[set id] >& [set id]_telemetry_[clock seconds].log &]
       }
     }
   }
}

proc parsesalclients { clientlist } {
global SALCLIENTS
   set client ""
   set fin [open $clientlist r]
   while { [gets $fin rec] > -1 } {
       if { [lindex $rec 0] == "client" } {
          if { $client != "" } {
             set SALCLIENTS($client,subsystems) [lsort [array names subsystems]]
             set SALCLIENTS($client,telemetry) [lsort [array names telemetry]]
          }
          catch {unset subsystems;unset telemetry}
          set client [lindex $rec 1]
          lappend SALCLIENTS(list) $client
       }
       if { [lindex $rec 0] == "subscribe" } {
          set s [lindex [split [lindex $rec 1] _] 0]
          set subsystems($s) 1
          set telemetry([lindex $rec 1]) 1
       }
       if { [lindex $rec 0] == "onevent" } {
          set s [lindex [split [lindex $rec 1] _] 0]
          set subsystems($s) 1
          set telemetry([lindex $rec 1]) 1
          set SALCLIENTS($client,[lindex $rec 1]) [lrange $rec 2 end]
          set s [lindex [split [lindex $rec 3] _] 0]
          set subsystems($s) 1
       }
   }
   if { $client != "" } {
       set SALCLIENTS($client,subsystems) [lsort [array names subsystems]]
       set SALCLIENTS($client,telemetry) [lsort [array names telemetry]]
   }
   close $fin 
}

proc exampleclient { } {
   set fout [open exampleClient.def w]
   puts $fout "# Example SAL generic client
client anExample
subscribe MTMount_MotionParameters
subscribe camera_Filter
onevent camera_logevent_endReadout publish dmHeaderService_logevent_LargeFileObjectAvailable
"
   close $fout
   parsesalclients exampleClient.def
   foreach c $SALCLIENTS(list) {
      genetelemetryclient $c
      gensalclientmake $c
   }
}



set SAL_DIR $env(SAL_DIR)
set SAL_WORK_DIR $env(SAL_WORK_DIR)


source $SAL_DIR/add_system_dictionary.tcl
source $SAL_DIR/revCodes.tcl
source $SAL_WORK_DIR/.salwork/revCodes.tcl
source $SAL_DIR/managetypes.tcl

