#!/usr/bin/tclsh
#
#  Generate simple pub/sub programs for each data type
#
proc makesaldirs { base name } {
global SAL_WORK_DIR
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/python
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/cpp/src
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/cpp/standalone
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/isocpp
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/java/src/org/lsst/sal
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/java/src/org/lsst/sal/$base/$name
   exec mkdir -p $SAL_WORK_DIR/SALDDS/java/src/org/lsst/sal
   exec mkdir -p $SAL_WORK_DIR/[set base]/java/src
   exec mkdir -p $SAL_WORK_DIR/[set base]/cpp/src
   exec mkdir -p $SAL_WORK_DIR/[set base]/isocpp/src
   exec mkdir -p $SAL_WORK_DIR/[set base]_[set name]/java/standalone
   exec touch $SAL_WORK_DIR/[set base]_[set name]/java/standalone/.depend.Makefile.saj_[set base]_[set name]_pub
   exec touch $SAL_WORK_DIR/[set base]_[set name]/java/standalone/.depend.Makefile.saj_[set base]_[set name]_sub
   exec touch $SAL_WORK_DIR/[set base]_[set name]/cpp/standalone/.depend.Makefile.sacpp_[set base]_[set name]_pub
   exec touch $SAL_WORK_DIR/[set base]_[set name]/cpp/standalone/.depend.Makefile.sacpp_[set base]_[set name]_sub
###   exec mkdir -p $SAL_WORK_DIR/SALDDS/cpp/src
   exec ln -sf $SAL_WORK_DIR/idl-templates/validated/sal/sal_$base.idl $SAL_WORK_DIR/$base/cpp/.
   exec ln -sf $SAL_WORK_DIR/idl-templates/validated/sal/sal_$base.idl $SAL_WORK_DIR/$base/isocpp/.
   exec ln -sf $SAL_WORK_DIR/idl-templates/validated/sal/sal_$base.idl $SAL_WORK_DIR/$base/java/.
   exec touch $SAL_WORK_DIR/[set base]/cpp/.depend.Makefile.sacpp_[set base]_types
   exec touch $SAL_WORK_DIR/[set base]/java/.depend.Makefile.saj_[set base]_types
   exec touch $SAL_WORK_DIR/[set base]/isocpp/.depend.Makefile.ISO_Cxx_[set base]_types
}

proc makesalidl { subsys } {
global SAL_DIR SAL_WORK_DIR
   set all [lsort [glob $SAL_WORK_DIR/idl-templates/validated/$subsys*.idl]]
   exec mkdir -p $SAL_WORK_DIR/idl-templates/validated/sal
   set fout [open $SAL_WORK_DIR/idl-templates/validated/sal/sal_$subsys.idl w]
   puts $fout "module $subsys
\{"
   foreach i $all {
      set fin [open $i r]
      gets $fin rec
      set name [join [lrange [split [file rootname [file tail $i]] _] 1 end] _]
      puts $fout "	struct $name \{"
      while { [gets $fin rec] > -1 } {
         if { [string range $rec 0 6] == "#pragma" } {
            puts $fout "	#pragma keylist $name"
         } else {
            puts $fout "	$rec"
         }
      }
      close $fin
   }
   puts $fout "   struct command
   \{
      string<32>	private_revCode;
      double		private_sndStamp;
      double		private_rcvStamp;
      long		private_origin;
      long		private_seqNum;
      string<32>	device;
      string<32>	property;
      string<32>	action;
      string<32>	value;
      string<128>	modifiers;
   \};
   #pragma keylist command
   struct response
   \{
      string<32>	private_revCode;
      double		private_sndStamp;
      double		private_rcvStamp;
      long		private_origin;
      long		private_seqNum;
      long 		ack;
      long 		error;
      string<32>	result;
   \};
   #pragma keylist response
   struct logevent
   \{
      string<32>	private_revCode;
      double		private_sndStamp;
      double		private_rcvStamp;
      long		private_origin;
      string<128>	message;
   \};
   #pragma keylist logevent"
   puts $fout "\};
"
   close $fout
   return $SAL_WORK_DIR/idl-templates/validated/sal/sal_$subsys.idl
}


proc makesalcode { idlfile base name lang } {
global SAL_DIR
      puts stdout "Processing $base $name"
      catch {makesaldirs $base $name}
      set id [set base]_[set name]
      set frep [open /tmp/sreplace.sal w]
      puts $frep "#!/bin/sh"
      if { $lang == "cpp" } {
        exec cp $SAL_DIR/code/templates/Makefile-cpp.template [set id]/cpp/standalone/Makefile
        puts $frep "perl -pi -w -e 's/sacpp_SAL_types/sacpp_[set base]_types/g;' [set id]/cpp/standalone/Makefile"
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/cpp/standalone/Makefile"
        exec cp $SAL_DIR/code/templates/Makefile.sacpp_SAL_types.template [set base]/cpp/Makefile.sacpp_[set base]_types
        puts $frep "perl -pi -w -e 's/SALDATA.idl/[file tail $idlfile]/g;' [set base]/cpp/Makefile.sacpp_[set base]_types"
        puts $frep "perl -pi -w -e 's/SALData/sal_[set base]/g;' [set base]/cpp/Makefile.sacpp_[set base]_types"
        puts $frep "perl -pi -w -e 's/sacpp_SAL_types/sacpp_[set base]_types/g;' [set base]/cpp/Makefile.sacpp_[set base]_types"
        exec cp $SAL_DIR/code/templates/Makefile.sacpp_SAL_sub.template [set id]/cpp/standalone/Makefile.sacpp_[set id]_sub
        puts $frep "perl -pi -w -e 's/_SAL_/_[set base]_/g;' [set id]/cpp/standalone/Makefile.sacpp_[set id]_sub"
        puts $frep "perl -pi -w -e 's/SALData/[set id]/g;' [set id]/cpp/standalone/Makefile.sacpp_[set id]_sub"
        exec cp $SAL_DIR/code/templates/Makefile.sacpp_SAL_pub.template [set id]/cpp/standalone/Makefile.sacpp_[set id]_pub
        puts $frep "perl -pi -w -e 's/_SAL_/_[set base]_/g;' [set id]/cpp/standalone/Makefile.sacpp_[set id]_pub"
        puts $frep "perl -pi -w -e 's/SALData/[set id]/g;' [set id]/cpp/standalone/Makefile.sacpp_[set id]_pub"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_pub.template [set id]/java/standalone/Makefile.saj_[set id]_pub
        exec cp $SAL_DIR/code/templates/SALDataPublisher.cpp.template [set id]/cpp/src/[set id]DataPublisher.cpp
        puts $frep "perl -pi -w -e 's/SALTopic/[set base]\:\:[set name]/g;' [set id]/cpp/src/[set id]DataPublisher.cpp"
        puts $frep "perl -pi -w -e 's/SALNAMESTRING/[set base]_[set name]/g;' [set id]/cpp/src/[set id]DataPublisher.cpp"
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/cpp/src/[set id]DataPublisher.cpp"
        exec cp $SAL_DIR/code/templates/SALDataSubscriber.cpp.template [set id]/cpp/src/[set id]DataSubscriber.cpp
        puts $frep "perl -pi -w -e 's/SALTopic/[set base]\:\:[set name]/g;' [set id]/cpp/src/[set id]DataSubscriber.cpp"
        puts $frep "perl -pi -w -e 's/SALNAMESTRING/[set base]_[set name]/g;' [set id]/cpp/src/[set id]DataSubscriber.cpp"
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/cpp/src/[set id]DataSubscriber.cpp"
        foreach f "CheckStatus.cpp CheckStatus.h" {
##### These should be generated by idlpp
##### SALData.cpp SALDataDcps.cpp SALDataDcps.h SALDataDcps_impl.cpp SALDataDcps_impl.h 
##### SALData.h  SALDataSplDcps.cpp SALDataSplDcps.h ccpp_SALData.h"
           exec cp $SAL_DIR/code/templates/$f [set id]/cpp/src/.
        }
      }
      if { $lang == "java"}  {
        exec cp $SAL_DIR/code/templates/Makefile-java.template [set id]/java/standalone/Makefile
        puts $frep "perl -pi -w -e 's/saj_SAL_types/saj_[set base]_types/g;' [set id]/java/standalone/Makefile"
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/java/standalone/Makefile"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_types.template [set base]/java/Makefile.saj_[set base]_types
        puts $frep "perl -pi -w -e 's/SALDATA.idl/[file tail $idlfile]/g;' [set base]/java/Makefile.saj_[set base]_types"
        puts $frep "perl -pi -w -e 's/SALData/[set base]/g;' [set base]/java/Makefile.saj_[set base]_types"
        puts $frep "perl -pi -w -e 's/_SAL_/_[set base]_/g;' [set base]/java/Makefile.saj_[set base]_types"
        exec cp $SAL_DIR/code/templates/Makefile-java.template [set id]/java/Makefile
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/java/Makefile"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_pub.template [set id]/java/standalone/Makefile.saj_[set id]_pub
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/java/standalone/Makefile.saj_[set id]_pub"
        puts $frep "perl -pi -w -e 's/SALTopic/[set id]/g;' [set id]/java/standalone/Makefile.saj_[set id]_pub"
        puts $frep "perl -pi -w -e 's/SALData/[set base]/g;' [set id]/java/standalone/Makefile.saj_[set id]_pub"
        exec cp $SAL_DIR/code/templates/Makefile.saj_SAL_sub.template [set id]/java/standalone/Makefile.saj_[set id]_sub
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/java/standalone/Makefile.saj_[set id]_sub"
        puts $frep "perl -pi -w -e 's/SALTopic/[set id]/g;' [set id]/java/standalone/Makefile.saj_[set id]_sub"
        puts $frep "perl -pi -w -e 's/SALData/[set base]/g;' [set id]/java/standalone/Makefile.saj_[set id]_sub"
        exec cp $SAL_DIR/code/templates/SALTopicDataPublisher.java.template [set id]/java/src/[set id]DataPublisher.java
        puts $frep "perl -pi -w -e 's/SALTopic/[set name]/g;' [set id]/java/src/[set id]DataPublisher.java"
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/java/src/[set id]DataPublisher.java"
        puts $frep "perl -pi -w -e 's/SALNAMESTRING/[set id]/g;' [set id]/java/src/[set id]DataPublisher.java"
        exec cp $SAL_DIR/code/templates/SALTopicDataSubscriber.java.template [set id]/java/src/[set id]DataSubscriber.java
        puts $frep "perl -pi -w -e 's/SALTopic/[set name]/g;' [set id]/java/src/[set id]DataSubscriber.java"
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/java/src/[set id]DataSubscriber.java"
        puts $frep "perl -pi -w -e 's/SALNAMESTRING/[set id]/g;' [set id]/java/src/[set id]DataSubscriber.java"
###        exec cp -r $SAL_DIR/code/templates/java/standalone/SALData [set id]/java/standalone/.
        exec cp $SAL_DIR/code/templates/runsample.template [set id]/java/standalone/[set id].run
        puts $frep "perl -pi -w -e 's/SALTopic/[set id]/g;' [set id]/java/standalone/[set id].run"
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/java/standalone/[set id].run"
      }
      if { $lang == "isocpp" } {
        exec cp $SAL_DIR/code/templates/Makefile-isocpp.template [set id]/isocpp/Makefile
        puts $frep "perl -pi -w -e 's/SAL_Typesupport/[set base]_TypeSupport/g;' [set id]/isocpp/Makefile"
        puts $frep "perl -pi -w -e 's/_SAL_/_[set id]_/g;' [set id]/isocpp/Makefile"
        exec cp $SAL_DIR/code/templates/Makefile.ISO_Cxx_SAL_Typesupport.template [set base]/isocpp/Makefile.ISO_Cxx_[set base]_Typesupport
        puts $frep "perl -pi -w -e 's/SALDATA.idl/[file tail $idlfile]/g;' [set base]/isocpp/Makefile.ISO_Cxx_[set base]_Typesupport"
        puts $frep "perl -pi -w -e 's/SALData/sal_[set base]/g;' [set base]/isocpp/Makefile.ISO_Cxx_[set base]_Typesupport"
        puts $frep "perl -pi -w -e 's/_SAL_/_[set base]_/g;' [set base]/isocpp/Makefile.ISO_Cxx_[set base]_Typesupport"
        exec cp $SAL_DIR/code/templates/implementation.hpp.template [set id]/isocpp/implementation.hpp
        exec cp $SAL_DIR/code/templates/publisher.cpp.template [set id]/isocpp/publisher.cpp
        exec cp $SAL_DIR/code/templates/subscriber.cpp.template [set id]/isocpp/subscriber.cpp
        exec cp $SAL_DIR/code/templates/implementation.cpp.template [set id]/isocpp/implementation.cpp
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/isocpp/implementation.cpp"
        puts $frep "perl -pi -w -e 's/SALTopic/$name/g;' [set id]/isocpp/implementation.cpp"
        puts $frep "perl -pi -w -e 's/SALNAMESTRING/$name/g;' [set id]/isocpp/implementation.cpp"
      }
      if { $lang == "python" } {
        exec cp $SAL_DIR/code/templates/SALTopicPublisher.py.template [set id]/python/[set id]Publisher.py
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/python/[set id]Publisher.py"
        puts $frep "perl -pi -w -e 's/SALTopic/$name/g;' [set id]/python/[set id]Publisher.py"
        puts $frep "perl -pi -w -e 's/SALDATA.idl/[file tail $idlfile]/g;' [set id]/python/[set id]Publisher.py"
        exec cp $SAL_DIR/code/templates/SALTopicSubscriber.py.template [set id]/python/[set id]Subscriber.py
        puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set id]/python/[set id]Subscriber.py"
        puts $frep "perl -pi -w -e 's/SALTopic/$name/g;' [set id]/python/[set id]Subscriber.py"
        puts $frep "perl -pi -w -e 's/SALDATA.idl/[file tail $idlfile]/g;' [set id]/python/[set id]Subscriber.py"
      }
      close $frep
      exec chmod 755 /tmp/sreplace.sal
      catch { set result [exec /tmp/sreplace.sal] } bad
      if { $bad != "" } {puts stdout $bad}
puts stdout "calling addSALDDStypes $idlfile $id $lang"
      addSALDDStypes $idlfile $id $lang $base
      set frep [open /tmp/sreplace2.sal w]
      puts $frep "#!/bin/sh"
      puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set base]/cpp/src/SALDDS.h"
      puts $frep "perl -pi -w -e 's/SALData/$base/g;' [set base]/cpp/src/SALDDS.cpp"
      puts $frep "perl -pi -w -e 's/SALCommand/$base\:\:command/g;' [set base]/cpp/src/SALDDS.cpp"
      close $frep
      exec chmod 755 /tmp/sreplace2.sal
      catch { set result [exec /tmp/sreplace2.sal] } bad
puts stdout "calling salidlgen $base $lang"
      salidlgen $base $lang
      if { $lang == "cpp" } { 
         set incfiles [glob [set base]/cpp/*.h]
         puts stdout "Updating include files : $incfiles"
         foreach i $incfiles {  exec cp $i /opt/lsstsal/include/. }
         exec cp [set base]/cpp/lib_sacpp_[set base]_types.so /opt/lsstsal/lib/.
      }
      if { $lang == "java" } {
         exec cp [set base]/java/saj_[set base]_types.jar /opt/lsstsal/lib/.
         saljavaclassgen $id
      }
}


proc unittest { } {
global SAL_WORK_DIR
      makesalidl tcs
      makesalcode $SAL_WORK_DIR/idl-templates/validated/sal/sal_tcs.idl tcs kernel_TrackingTarget cpp
      makesalcode $SAL_WORK_DIR/idl-templates/validated/sal/sal_tcs.idl tcs kernel_TrackingTarget java
      makesalcode $SAL_WORK_DIR/idl-templates/validated/sal/sal_tcs.idl tcs kernel_TrackingTarget isocpp
      makesalcode $SAL_WORK_DIR/idl-templates/validated/sal/sal_tcs.idl tcs kernel_TrackingTarget python
}


proc salfullgen { } {
global SAL_WORK_DIR OPTIONS
  set atypes [lsort [glob $SAL_WORK_DIR/idl-temmplates/validated/sal/*.idl]]
  foreach i $atypes {
    set base [lindex [exec grep module $i] 1]
    set ptypes [split [exec grep pragma $i] \n]
    foreach j $ptypes {
      set name [lindex $j 2]
      makesalcode $idlfile $base $name cpp
      salidlgen cpp
      makesalcode $idlfile $base $name java
      salidlgen java
      makesalcode $idlfile $base $name isocpp
      salidlgen isocpp
      makesalcode $idlfile $base $name python
    }
  }
}

proc salidlgen { base lang } {
global SAL_WORK_DIR OPTIONS
   cd $SAL_WORK_DIR/$base/$lang
   puts stdout "Generating $lang type support for $base"
   if { $lang == "cpp" }     {catch { set result [exec make -f Makefile.sacpp_[set base]_types] } bad}
   if { $lang == "isocpp" }  {catch { set result [exec make -f Makefile.ISO_Cxx_[set base]_Typesupport] } bad}
   if { $lang == "java"}     {catch { set result [exec make -f Makefile.saj_[set base]_types] } bad}
   puts stdout "idl : $result"
   cd $SAL_WORK_DIR
}

proc saljavaclassgen { id } {
global SAL_WORK_DIR OPTIONS
   cd $SAL_WORK_DIR/$id/java/standalone
   catch { set result [exec make -f Makefile.saj_[set id]_pub] } bad
   puts stdout "javac : $result"
   catch { set result [exec make -f Makefile.saj_[set id]_sub] } bad
   puts stdout "javac : $result"
   cd $SAL_WORK_DIR
}


source /opt/lsstsal/scripts/gensalgetput.tcl


