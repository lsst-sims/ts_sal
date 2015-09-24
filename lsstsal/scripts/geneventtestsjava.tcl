
# set SAL_WORK_DIR $env(SAL_WORK_DIR)
# set SAL_DIR $env(SAL_DIR)
# source $SAL_DIR/utilities.tcl
# source $SAL_DIR/geneventtestsjava.tcl
# source $SAL_WORK_DIR/idl-templates/validated/camera_evtdef.tcl
# geneventtestsjava camera
#



proc geneventtestsjava { subsys } {
global EVENT_ALIASES EVTS EVENT_ALIASES EVTS SAL_WORK_DIR SYSDIC SAL_DIR
 if { [info exists EVENT_ALIASES($subsys)] } {
  if { [info exists SYSDIC($subsys,keyedID)] } {
       set initializer "(1)"
  } else {
       set initializer "()"
  }
  foreach alias $EVENT_ALIASES($subsys) {
    if { [info exists EVTS($subsys,$alias,param)] } {
      stdlog "	: event test send for = $alias"
      set fcmd [open $SAL_WORK_DIR/$subsys/java/src/[set subsys]Event_[set alias].java w]
      puts $fcmd "


// This file contains the implementation for the [set subsys]_[set alias] event generator test.
import [set subsys].*;
import org.lsst.sal.SAL_[set subsys];

public class [set subsys]Event_[set alias] \{

	public static void main(String\[\] args) \{

          short aKey=1;
	  SAL_[set subsys] mgr = new SAL_[set subsys][set initializer];

	  // Issue Event
          int status=0;

          if (args.length < [expr [llength $EVTS($subsys,$alias,plist)] +1] ) \{

            System.err.println(\"Usage: priority $EVTS($subsys,$alias,plist)\");
            System.exit(1);

          \} else \{

            int priority=Integer.parseInt(args\[0\])
	    [set subsys].logevent_[set alias] event  = new [set subsys].logevent_[set alias]();
	    event.private_revCode = \"LSST TEST EVENT\";"
     set narg 1
     foreach p $EVTS($subsys,$alias,param) {
       set pname [lindex $p 1]
       set ptype [lindex $p 0]
       if { [llength [split $pname "()"]] > 1 } {
        set l 0
        set pspl [split $pname "()"]
        set pname [lindex $pspl 0]
        set pdim  [lindex $pspl 1]
        while { $l < $pdim } {
         switch $ptype {
          boolean { puts $fcmd "  command.[set pname]\[$l\] = Boolean.valueOf(args\[$narg\]);" }
          double  { puts $fcmd "  command.[set pname]\[$l\] = Double.valueOf(args\[$narg\]);" }
          int     { puts $fcmd "  command.[set pname]\[$l\] = Integer.valueOf(args\[$narg\]);" }
          long    { puts $fcmd "  command.[set pname]\[$l\] = Integer.valueOf(args\[$narg\]);" }
         }
         incr l 1
        }
       } else {
        switch $ptype {
          boolean { puts $fcmd "  command.[set pname] = Boolean.valueOf(args\[$narg\]);" }
          double  { puts $fcmd "  command.[set pname] = Double.valueOf(args\[$narg\]);" }
          int     { puts $fcmd "  command.[set pname] = Integer.valueOf(args\[$narg\]);" }
          long    { puts $fcmd "  command.[set pname] = Integer.valueOf(args\[$narg\]);" }
          string  { puts $fcmd "  command.[set pname] = args\[$narg\];" }
       }
      }
      incr narg 1
     }
     puts $fcmd "
	    status = mgr.logEvent_[set alias](event,priority);

	    try \{Thread.sleep(1000);\} catch (InterruptedException e)  \{ e.printStackTrace(); \}

	    /* Remove the DataWriters etc */
	    mgr.salShutdown();

	\}
      \}

\}

"
      close $fcmd
      stdlog "	: event logger for = $alias"
      set fcmd [open $SAL_WORK_DIR/$subsys/java/src/[set subsys]EventLogger_[set alias].java w]
      puts $fcmd "
import [set subsys].*;
import org.lsst.sal.SAL_[set subsys];

public class [set subsys]EventLogger \{

	public static void main(String\[\] args) \{
	  int status   = SAL_[set subsys].SAL__OK;
	  boolean finished=false;

	  // Initialize
	  SAL_[set subsys] evt = new SAL_[set subsys][set initializer];
	  [set subsys].logevent_[set alias] event = new [set subsys].logevent_[set alias]();

	  while (!finished) \{
	     status = evt.getEvent(event);
	     if (status == SAL_[set subsys].SAL__OK) \{
                System.out.println(\"=== Event Logged : \" + event);
             \}
 	     try \{Thread.sleep(100);\} catch (InterruptedException e)  \{ e.printStackTrace(); \}
	  \}

	  /* Remove the DataWriters etc */
	  evt.salShutdown();
       \}
\}
"
       close $fcmd
     }
   }
   puts stdout "Generating events test Makefile"
   set frep [open /tmp/makerep.sal w]
   foreach alias $EVENT_ALIASES($subsys) {
     if { [info exists EVTS($subsys,$alias,param)] } {
        puts stdout "	: for $subsys $alias"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_testevents.template $SAL_WORK_DIR/$subsys/java/src/Makefile.saj_[set subsys]_[set alias]_test
        puts $frep "perl -pi -w -e 's/SALData/[set subsys]/g;' [set subsys]/java/src/Makefile.saj_[set subsys]_[set alias]_test"
        puts $frep "perl -pi -w -e 's/SALALIAS/[set alias]/g;' [set subsys]/java/src/Makefile.saj_[set subsys]_[set alias]_test"
      }
   }
   close $frep
   exec chmod 755 /tmp/makerep.sal
   catch { set result [exec /tmp/makerep.sal] } bad
   if { $bad != "" } {puts stdout $bad}
  }
}

 