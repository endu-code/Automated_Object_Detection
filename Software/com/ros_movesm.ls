/PROG  ROS_MOVESM
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "r2";
PROG_SIZE	= 232;
CREATE		= DATE 12-10-02  TIME 12:08:46;
MODIFIED	= DATE 00-06-06  TIME 01:07:46;
FILE_NAME	= ;
VERSION		= 0;
LINE_COUNT	= 6;
MEMORY_SIZE	= 580;
PROTECT		= READ_WRITE;
TCD:  STACK_SIZE	= 0,
      TASK_PRIORITY	= 50,
      TIME_SLICE	= 0,
      BUSY_LAMP_OFF	= 0,
      ABORT_REQUEST	= 0,
      PAUSE_REQUEST	= 0;
DEFAULT_GROUP	= 1,*,*,*,*;
CONTROL_CODE	= 00000000 00000000;
/MN
   1:  UTOOL_NUM=3 ;
   2:  UFRAME_NUM=8 ;
   3:L PR[90] 250mm/sec FINE    ;
   4:  !done, repeat ;
   5:  R[200]=(1) ;
   6:  R[199]=(0) ;
/POS
/END