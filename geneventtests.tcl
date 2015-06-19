proc geneventtestscpp { subsys } {
global EVENT_ALIASES EVTS SAL_WORK_DIR SYSDIC
 if { [info exists EVENT_ALIASES($subsys)] } {
   foreach alias $EVENT_ALIASES($subsys) {
    if { [info exists EVTS($subsys,$i,param)] } {
      stdlog "	: log event test send for = $alias"
      set fevt [open $SAL_WORK_DIR/$subsys/cpp/src/sacpp_[set subsys]_[set alias]_send.cpp w]
      puts $fevt "

/*
 * This file contains the implementation for the [set subsys]_[set alias] send test.
 *
 ***/

#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include \"SAL_[set subsys].h\"
#include \"ccpp_sal_[set subsys].h\"
#include \"os.h\"
using namespace DDS;
using namespace [set subsys];

int main (int argc, char *argv\[\])
\{ 
  int priority = SAL__EVENT_INFO;
  [set subsys]_logevent_[set alias]C myData;
  if (argc < 2) \{
     printf(\"Usage :  input parameters...\\n\");
     printf(\"      int priority\\n\");
"
   set fidl [open $SAL_WORK_DIR/idl-templates/validated/[set subsys]_logevent_[set alias].idl r]
   gets $fidl rec ; gets $fidl rec ;gets $fidl rec ;gets $fidl rec ;gets $fidl rec ;gets $fidl rec ;gets $fidl rec ;gets $fidl rec
   while { [gets $fidl rec] > -1 } {
      if { [lindex $rec 0] != "#pragma" && [lindex $rec 0]!= "\};" } {
         puts $fevt "     printf(\"$rec\\n\");"
      }
   }
   close $fidl
   puts $fevt "     exit(1);
  \}"
   if { [info exists SYSDIC($subsys,keyedID)] } {
      puts $fcmd "
  int [set subsys]ID = 1;
  SAL_[set subsys] mgr = SAL_[set subsys]([set subsys]ID);"
   } else {
      puts $fcmd "  SAL_[set subsys] mgr = SAL_[set subsys]();"
   }
   puts $fcmd "
  mgr.salEvent(\"[set subsys]_logevent_[set alias]\");
"
  set fin [open $SAL_WORK_DIR/include/SAL_[set subsys]_logevent_[set alias]Cargs.tmp r]
  while { [gets $fin rec] > -1 } {
     if { [lindex [split $rec ",)"] 2] != " &myData.priority" } {
       puts $fevt $rec
     } else {
       puts $fevt "    sscanf(argv\[1\], \"%d\", &priority);"
     }
  }
  close $fin
  puts $fevt "
  // generate event
  mgr.logEvent_[set alias](&myData, priority);
  cout << \"=== Event $alias generated = \" << endl;
  sleep(1);
  /* Remove the DataWriters etc */
  mgr.salShutdown();

  return 0;
\}

"
     close $fevt
      stdlog "	: log event test receive for = $alias"
      set fevt [open $SAL_WORK_DIR/$subsys/cpp/src/sacpp_[set subsys]_[set alias]_log.cpp w]
      puts $fevt "

/*
 * This file contains the implementation for the [set subsys]_[set alias] receive test.
 *
 ***/

#include <string>
#include <sstream>
#include <iostream>
#include \"SAL_[set subsys].h\"
#include \"ccpp_sal_[set subsys].h\"
#include \"os.h\"
using namespace DDS;
using namespace [set subsys];

/* entry point exported and demangled so symbol can be found in shared library */
extern \"C\"
\{
  OS_API_EXPORT
  int test_[set subsys]_[set alias]_Log();
\}

int test_[set subsys]_[set alias]_Log()
\{ 
  os_time delay_10ms = \{ 0, 10000000 \};
  int status = -1;

  [set subsys]_logevent_[set alias]C SALInstance;"
   if { [info exists SYSDIC($subsys,keyedID)] } {
      puts $fcmd "
  int [set subsys]ID = 1;
  SAL_[set subsys] mgr = SAL_[set subsys]([set subsys]ID);"
   } else {
      puts $fcmd "  SAL_[set subsys] mgr = SAL_[set subsys]();"
   }
   puts $fcmd "
  mgr.salEvent(\"[set subsys]_logevent_[set alias]\");
  while (1) \{
  // receive event
    status = mgr.getEvent_[set alias](&SALInstance);
    if (status == SAL__OK) \{
      cout << \"=== Event $alias received = \" << endl;
"
  set fin [open $SAL_WORK_DIR/include/SAL_[set subsys]_logevent_[set alias]Csub.tmp r]
  while { [gets $fin rec] > -1 } {
     puts $fevt $rec
  }
  close $fin
  puts $fevt "
    \}
    os_nanoSleep(delay_10ms);
  \}

  /* Remove the DataWriters etc */
  mgr.salShutdown();

  return 0;
\}

int main (int argc, char *argv\[\])
\{
  return test_[set subsys]_[set alias]_Log();
\}
"
     close $fevt
   }
  }
   puts stdout "Generating events test Makefile"
   set fin [open $SAL_WORK_DIR/$subsys/cpp/src/Makefile.sacpp_[set subsys]_testevents r]
   set fout [open /tmp/Makefile.sacpp_[set subsys]_testevents w]
   while { [gets $fin rec] > -1 } {
      if { [string range $rec 0 24] == "## INSERT EVENTS TEST SRC" } {
         set n 2
         set extrasrc "		"
         set allbin "all: \$\(BIN1\) \$\(BIN2\)"
         foreach alias $EVENT_ALIASES($subsys) {
           if { [info exists EVTS($subsys,$i,param)] } {
             incr n 1
             puts $fout "
BIN$n           = \$(BTARGETDIR)sacpp_[set subsys]_[set alias]_send
OBJS$n          = .obj/CheckStatus.o .obj/SAL_[set subsys].o .obj/sacpp_[set subsys]_[set alias]_send.o
"
             set allbin "$allbin \$\(BIN$n\)"
             incr n 1
             puts $fout "
BIN$n           = \$(BTARGETDIR)sacpp_[set subsys]_[set alias]_log
OBJS$n          = .obj/CheckStatus.o .obj/SAL_[set subsys].o .obj/sacpp_[set subsys]_[set alias]_log.o
"
             set extrasrc "$extrasrc sacpp_[set subsys]_[set alias]_send.cpp sacpp_[set subsys]_[set alias]_log.cpp"
             set allbin "$allbin \$\(BIN$n\)"
           }
         }
         set nbin $n
         puts $fout "
SRC           = ../src/CheckStatus.cpp ../src/SAL_[set subsys].cpp ../src/[set subsys]Event.cpp ../src/[set subsys]EventLogger.cpp $extrasrc"
      }
      if { [string range $rec 0 24] == "## INSERT EVENTS TEST BIN" } {
         set n 2
         puts $fout "$allbin"
         while { $n <= $nbin } {
            incr n 1
            puts $fout "
\$\(BIN$n\): \$(OBJS$n\)
	@\$\(TESTDIRSTART\) \"\$\(BTARGETDIR\)\" \$\(TESTDIREND\) \$\(MKDIR\) \"\$\(BTARGETDIR\)\"
	\$\(LINK.cc\) \$\(OBJS$n\) \$\(LDLIBS\) \$\(OUTPUT_OPTION\)

"
         }
         foreach alias $EVENT_ALIASES($subsys) {
          if { [info exists EVTS($subsys,$i,param)] } {
            incr n 1
            puts $fout "
.obj/sacpp_[set subsys]_[set alias]_send.o: ../src/sacpp_[set subsys]_[set alias]_send.cpp
	@\$\(TESTDIRSTART\) \".obj/../src\" \$\(TESTDIREND\) \$\(MKDIR\) \".obj/../src\"
	\$\(COMPILE.cc\) \$\(EXPORTFLAGS\) \$\(OUTPUT_OPTION\) ../src/sacpp_[set subsys]_[set alias]_send.cpp
.obj/sacpp_[set subsys]_[set alias]_log.o: ../src/sacpp_[set subsys]_[set alias]_log.cpp
	@\$\(TESTDIRSTART\) \".obj/../src\" \$\(TESTDIREND\) \$\(MKDIR\) \".obj/../src\"
	\$\(COMPILE.cc\) \$\(EXPORTFLAGS\) \$\(OUTPUT_OPTION\) ../src/sacpp_[set subsys]_[set alias]_log.cpp
"
          }
         }
      }
      puts $fout $rec
   }
   close $fin
   close $fout
   exec mv /tmp/Makefile.sacpp_[set subsys]_testevents $SAL_WORK_DIR/$subsys/cpp/src/Makefile.sacpp_[set subsys]_testevents
 }
}

