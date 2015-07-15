

# set SAL_WORK_DIR $env(SAL_WORK_DIR)
# set SAL_DIR $env(SAL_DIR)
# source $SAL_DIR/utilities.tcl
# source $SAL_DIR/gencommandtestsjava.tcl
# source $SAL_WORK_DIR/idl-templates/validated/camera_cmddef.tcl
# gencommandtestsjava camera
#


proc gencommandtestsjava { subsys } {
global CMD_ALIASES CMDS EVENT_ALIASES EVTS SAL_WORK_DIR SYSDIC SAL_DIR
 if { [info exists CMD_ALIASES($subsys)] } {
   if { [info exists SYSDIC($subsys,keyedID)] } {
       set initializer "(1)"
   } else {
       set initializer "()"
   }
   foreach alias $CMD_ALIASES($subsys) {
    if { [info exists CMDS($subsys,$alias,param)] } {
      stdlog "	: command test send for = $alias"
      set fcmd [open $SAL_WORK_DIR/$subsys/java/src/[set subsys]Commander_[set alias].java w]
      puts $fcmd "


// This file contains the implementation for the [set subsys]_[set alias] commander test.
import [set subsys].*;
import org.lsst.sal.SAL_[set subsys];

public class [set subsys]Commander_[set alias] \{

	public static void main(String\[\] args) \{

	  SAL_[set subsys] mgr = new SAL_[set subsys][set initializer];

	  // Issue command
	  int count=0;
          int cmdId=0;
          int status=0;

          if (args.length < [expr [llength $CMDS($subsys,$alias,plist)] +1] ) \{

            System.err.println(\"Usage: timeout $CMDS($subsys,$alias,plist)\");
            System.exit(1);

          \} else \{

            int timeout=Integer.parseInt(args\[0\]);

  	    mgr.salCommand(\"[set subsys]_command_[set alias]\");
	    [set subsys].command_[set alias] command  = new [set subsys].command_[set alias]();

	    command.private_revCode = \"LSST TEST COMMAND\";"
  set cpars $CMDS($subsys,$alias)
  puts $fcmd "  command.device   = \"[lindex $cpars 0]\";"
  puts $fcmd "  command.property = \"[lindex $cpars 1]\";"
  puts $fcmd "  command.action   = \"[lindex $cpars 2]\";"
  set narg 1
  foreach p $CMDS($subsys,$alias,param) {
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
	    cmdId = mgr.issueCommand_[set alias](command);

	    try \{Thread.sleep(1000);\} catch (InterruptedException e)  \{ e.printStackTrace(); \}
	    status = mgr.waitForCompletion_[set alias](cmdId, timeout);

	    /* Remove the DataWriters etc */
	    mgr.salShutdown();

	\}
      \}

\}

"
      close $fcmd
      stdlog "	: command test receive for = $alias"
      set fcmd [open $SAL_WORK_DIR/$subsys/java/src/[set subsys]Controller_[set alias].java w]
      puts $fcmd "
import [set subsys].*;
import org.lsst.sal.SAL_[set subsys];

public class [set subsys]Controller_[set alias] \{

	public static void main(String\[\] args) \{
          short aKey   = 1;
	  int status   = SAL_[set subsys].SAL__OK;
	  int cmdId    = 0;
          int timeout  = 0;
          boolean finished=false;

	  // Initialize
	  SAL_[set subsys] cmd = new SAL_[set subsys][set initializer];

          if (args.length > 0) \{
             timeout=Integer.parseInt(args\[0\]) * 1000;
          \}

	  cmd.salProcessor(\"[set subsys]_command_[set alias]\");
	  [set subsys].command_[set alias] command = new [set subsys].command_[set alias]();

	  while (!finished) \{

	     cmdId = cmd.acceptCommand_[set alias](command);
	     if (cmdId > 0) \{
	       if (timeout > 0) \{
	          cmd.ackCommand_[set alias](cmdId, SAL_[set subsys].SAL__CMD_INPROGRESS, timeout, \"Ack : OK\");
 	          try \{Thread.sleep(timeout);\} catch (InterruptedException e)  \{ e.printStackTrace(); \}
	       \}       
	       cmd.ackCommand_[set alias](cmdId, SAL_[set subsys].SAL__CMD_COMPLETE, 0, \"Done : OK\");
	     \}
 	     try \{Thread.sleep(1000);\} catch (InterruptedException e)  \{ e.printStackTrace(); \}
	  \}

	  /* Remove the DataWriters etc */
	  cmd.salShutdown();
       \}
\}

"
         close $fcmd
       }
     }
     puts stdout "Generating commands test Makefile"
     set frep [open /tmp/makerep.sal w]
     foreach alias $CMD_ALIASES($subsys) {
      if { [info exists CMDS($subsys,$alias,param)] } {
        puts stdout "	: for $subsys $alias"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_testcommands.template $SAL_WORK_DIR/$subsys/java/src/Makefile.saj_[set subsys]_[set alias]_test
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

 