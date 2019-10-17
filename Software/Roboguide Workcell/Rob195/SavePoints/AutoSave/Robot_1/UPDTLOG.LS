19/01/10 15:38:32

Version:  V8.30P/41/None
Build ID: V8.30258      6/20/2018

1A05B-2500-H510
1A05B-2500-H521
1A05B-2500-H552
1A05B-2500-R651
1A05B-2500-H768
1A05B-2500-H930
1A05B-2500-J618
1A05B-2500-J567
1A05B-2500-J568
1A05B-2500-J508
1A05B-2500-J771
1A05B-2500-R648
1A05B-2500-J841
1A05B-2500-J902
1A05B-2500-J901
1A05B-2500-J871
1A05B-2500-J873
1A05B-2500-R792
1A05B-2500-R632
1A05B-2500-R558
1A05B-2500-R834
Z

 ***At the beginning of Auto Update.***

Img.Backup Yes     * must have one ' ' then 'yes' or 'no'
! Next what devices should be backed up:
! Each line needs the word "BackupDevice", then space then 
! the device name with ":"
BackupDevice FR:
BackupDevice FRA:
!BackupDevice RD:
!BackupDevice FR1:
! Next is whether saving the backup (all and image) after 
! auto update or not. Select 'YES' or 'NO' after 
! "AfterBackup" and one space. YES: save, NO: not save.
! When MC: does not have enough capacity, backup is not
! saved even if "AfterBackup YES".
AfterBackup NO
!
! Restore I/O status,  must have one ' ' then 'yes' or 'no'
SetUpIOstat Yes
MED:\00\TEMP\SYSVARS.SV file missing from backup
MED:\00\TEMP\SYSPASS.SV file missing from backup

 ***At Controlled Start.***

F Number: F00000    
VERSION : HandlingTool         
$VERSION: V8.30261      5/21/2018
DATE:     14-JAN-19 08:38 

MEMORY USAGE::

MEMORY DETAIL (MAIN):
Pools        TOTAL     AVAILABLE      LARGEST
TPP          700.0KB     656.2KB     656.2KB
PERM        2040.0KB     426.9KB     423.9KB
SYSTEM          .0KB       0.0KB       0.0KB
SHADOW      6698.3KB    6066.8KB      32.0KB
TEMP       65458.3KB   43672.8KB   43419.4KB
FROM       64751.0KB   42153.0KB        .0KB
HARDWARE
FROM      64MB
DRAM      64MB
SRAM       2MB

860 memconfig skipped for virtual.


 ***At Cold Start.***

ERRALL.LS      Robot Name ROBOT 14-JAN-19 08:38:28  

1" 14-JAN-19 08:38:16 " SYST-305 System startup in progress               " " SERVO                         00110110" act"
2" 14-JAN-19 08:38:16 " SYST-322 Auto status check time out               " " WARN                          00000000"    "
3" 14-JAN-19 08:38:16 " SYST-305 System startup in progress               " " STOP.G                        00100110" act"
4" 14-JAN-19 08:38:16 " R E S E T                                         " "                               00000000"    "
5" 14-JAN-19 08:38:16 " PRIO-397 PMIO FW Update Needed 1030100 0          " " WARN                          00000000"    "
6" 14-JAN-19 08:38:14 " SYST-026 System normal power up                   " " WARN                          00000000"    "
7" 14-JAN-19 08:38:14 " RIPE-011 This robot is not in the ring            " " WARN                          00000000"    "
8" 14-JAN-19 08:38:14 " PRIO-370 PMIO No Board Found                      " " WARN                          00000000"    "
9" 14-JAN-19 08:38:04 " OPTN-017 No UPDATES on this media                 " " WARN                          00000000"    "
10" 14-JAN-19 08:38:04 " SYST-290 Cycle power to use new DCS parameter     " " STOP.G                        00100110"    "
11" 14-JAN-19 08:38:02 " TPIF-218 MED:\00\TEMP\SENDSYSV.TP failed to load  " MEMO-006 Protection error occurred                " WARN                          00000000"    "
12" 14-JAN-19 08:38:02 " TPIF-218 MED:\00\TEMP\SENDEVNT.TP failed to load  " MEMO-006 Protection error occurred                " WARN                          00000000"    "
13" 14-JAN-19 08:38:02 " TPIF-218 MED:\00\TEMP\SENDDATA.TP failed to load  " MEMO-006 Protection error occurred                " WARN                          00000000"    "
14" 14-JAN-19 08:38:02 " TPIF-218 MED:\00\TEMP\REQMENU.TP failed to load   " MEMO-006 Protection error occurred                " WARN                          00000000"    "
15" 14-JAN-19 08:38:02 " TPIF-218 MED:\00\TEMP\GETDATA.TP failed to load   " MEMO-006 Protection error occurred                " WARN                          00000000"    "
16" 14-JAN-19 08:38:00 " FILE-063 (MED:\00\TEMP\SYSPASS.SV) is not loaded  " FILE-014 File not found                           " STOP.G                        00100110"    "
17" 14-JAN-19 08:38:00 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
18" 14-JAN-19 08:38:00 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
19" 14-JAN-19 08:38:00 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
20" 14-JAN-19 08:38:00 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
21" 14-JAN-19 08:38:00 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
22" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
23" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
24" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
25" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
26" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
27" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
28" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
29" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
30" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
31" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
32" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
33" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
34" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
35" 14-JAN-19 08:37:58 " VARS-054 Sysvars - V8.30258      6/20/2018 loaded " " WARN                          00000000"    "
36" 14-JAN-19 08:37:56 " FILE-063 (MED:\00\TEMP\SYSVARS.SV) is not loaded  " FILE-014 File not found                           " STOP.G                        00100110"    "
37" 14-JAN-19 08:37:50 " OPTN-017 No UPDATES on this media                 " " WARN                          00000000"    "
38" 14-JAN-19 08:37:48 " OPTN-018 No CUSTO additions on this media         " " WARN                          00000000"    "
39" 14-JAN-19 08:37:48 " OPTN-018 No Language additions on this media      " " WARN                          00000000"    "
40" 14-JAN-19 08:37:46 " PRIO-370 PMIO No Board Found                      " " WARN                          00000000"    "

 ***At Cold Start.***

F Number: F00000    
VERSION : HandlingTool         
$VERSION: V8.30261      5/21/2018
DATE:     14-JAN-19 08:38 

MEMORY USAGE::

MEMORY DETAIL (MAIN):
Pools        TOTAL     AVAILABLE      LARGEST
TPP          700.0KB     656.2KB     656.2KB
PERM        2040.0KB     361.6KB     360.9KB
SYSTEM          .0KB       0.0KB       0.0KB
SHADOW      6698.3KB    6060.8KB      32.0KB
TEMP       65458.3KB   15697.1KB   15608.5KB
FROM       64751.0KB   42104.0KB        .0KB
HARDWARE
FROM      64MB
DRAM      64MB
SRAM       2MB

860 memconfig skipped for virtual.

** Update Complete 14-JAN-19 08:38 

19/05/16 10:19:48

Version:  V8.30P/40/None
Build ID: V8.30261      5/21/2018

1A05B-2500-H552
1A05B-2500-H521
1A05B-2500-R796
1A05B-2500-J618
1A05B-2500-J508
1A05B-2500-J567
1A05B-2500-J568
1A05B-2500-R834
1A05B-2500-R651
1A05B-2500-J841
1A05B-2500-R558
1A05B-2500-R632
1A05B-2500-J771
1A05B-2500-R641
1A05B-2500-R792
1A05B-2500-R648
1A05B-2500-J873
1A05B-2500-J901
1A05B-2500-J902
1A05B-2500-J871
1A05B-2500-FVRC
1A05B-2500-H768
Z
