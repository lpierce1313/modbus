#ifndef AEConstantsH
#define AEConstantsH



/****************************************************************************
 *    AE Bus Related Constants                                              *
 ****************************************************************************/

      ///////////////////////////////////////////////
      // AE Bus Command Constants                  //
      ///////////////////////////////////////////////
#define NO_CSR_ERROR_HANDLING (1)
// Regmode Codes
#define REGMODE_DI_VOLTAGE  5
#define REGMODE_FWD         6
#define REGMODE_LOAD        7
#define REGMODE_EXT_DCBIAS  8
#define REGMODE_VALIMIT     9

// EEPROM Command Constants
    //NAME                              //HEX             //DEC
#define EEPROM_COUPLER_DATA             0x0001
#define EEPROM_FWD_INDEX                0x0003
#define EEPROM_RFL_INDEX                0x0004


#define EEPROM_LIMIT_PAIMBAL_TRIP       0x0051          //    81
#define EEPROM_LIMIT_PATEMP             0x0053          //    83
#define EEPROM_LIMIT_RFOFF_BCT          0x060F
#define EEPROM_PWRCTRL_CLG              0x00C1          //   192
#define EEPROM_PWRCTRL_DRIVE_GAIN       0x0090          //   144
#define EEPROM_PWRCTRL_DRIVE_OFFSET     0x0091          //   145
#define EEPROM_FWD_CAL_OFFSET           0x0094          //
#define EEPROM_RFL_CAL_OFFSET           0x0095          //

#define EEPROM_ERROR_MASK               0x00A3          //   163

#define EEPROM_LIMIT_FWDPWR             0x0054
#define EEPROM_LIMIT_RFLPWR             0x0055

#define EEPROM_PULSING_ENABLED          0x0200
#define EEPROM_PULSING_DURATION_LOW     0x0201
#define EEPROM_PULSING_DURATION_HIGH    0x0202
#define EEPROM_PULSING_START_DELAY      0x0203
#define EEPROM_PULSING_SELECT           0x0204
#define EEPROM_PULSING_START_DURATION   0x0205
#define EEPROM_PULSING_SETPOINT_HIGH    0x0206
#define EEPROM_PULSING_SETPOINT_LOW     0x0207
#define EEPROM_PULSING_OPERATION        0x0208
#define EEPROM_PULSING_ENV_DUR_HIGH     0x0209
#define EEPROM_PULSING_ENV_DUR_LOW      0x020A
#define EEPROM_PULSING_TRANSITION       0x020B

#define EEPROM_TUNUNG_ENABLED           0x0100     // 1
#define EEPROM_START_FREQ               0x0101
#define EEPROM_MIN_FREQ                 0x0102
#define EEPROM_MAX_FREQ                 0x0103
#define EEPROM_MINLOCK_FREQ             0x0104
#define EEPROM_MAXLOCK_FREQ             0x0105
#define EEPROM_TUNUNG_START_STEP        0x0106     // 2
#define EEPROM_TUNUNG_END_STEP          0x0107     // 2
#define EEPROM_TUNUNG_START_DELAY       0x0108     // 2
#define EEPROM_TUNUNG_MAX_REFLECTED     0x0109     // 2
#define EEPROM_TUNUNG_REVERSALS         0x010A     // 2
#define EEPROM_TUNUNG_THRESHOLD         0x010B     // 2
#define EEPROM_TUNUNG_NOT_FOUND         0x010C     // 2
#define EEPROM_TUNUNG_DECREASE_STEP     0x010D     // 1
#define EEPROM_TUNUNG_RESTART_OPTION    0x010E     // 1
#define EEPROM_TUNUNG_DITHERING         0x010F     // 1

#define EEPROM_FREQ_ABSOLUTE_MIN        0x0110          //   272
#define EEPROM_FREQ_ABSOLUTE_MAX        0x0111          //   273
#define EEPROM_DUMP                     0xFFF1          //
#define EEPROM_ERASE                    0xC001          //
#define EEPROM_COMMIT                   0xC002          //
#define EEPROM_RESTORE                  0xC003          //


#define EEPROM_REFL_TIMER               0x006A
#define EEPROM_GAMMA                    0x0093

#define EEPROM_DIRECT                   0xFFFE      // Undocumented

// VI Sensor IDs
#define EEPROM_VIS_D1F1                 0x0102     // 1
#define EEPROM_VIS_D1F2                 0x0103     // 1
#define EEPROM_VIS_D2F1                 0x0104     // 1
#define EEPROM_VIS_D2F2                 0x0105     // 1
#define EEPROM_VIS_IT                   0x010C     // 1
#define EEPROM_VIS_DF                   0x010D     // 1
#define EEPROM_VIS_RR                   0x010F     // 1
#define EEPROM_VIS_IRD                  0x060C     // 1

#define EEPROM_UIAO_1                   0x0801
#define EEPROM_UIAO_2                   0x0802
#define EEPROM_UIAO_3                   0x0803

#define EEPROM_SERIAL_NUMBER            0x0702
#define EEPROM_TOP_LEVEL_ASSY           0x0700

#define EEPROM_FPR_DRIVE_OFFSET         0x060F
#define EEPROM_LO_CORR_LIMIT            0x0611
#define EEPROM_HI_CORR_LIMIT            0x0612
#define EEPROM_MEMO                     0x0703
#define EEPROM_MEMO_SIZE                100


// Process Status Constants
  //NAME                          //HEX          //DEC
#define PROCESS_MATCH_FOUND       0x00000001      //         1
#define PROCESS_RFON              0x00000020      //        32
#define PROCESS_RFON_REQUEST      0x00000040      //        64
#define PROCESS_SETPT             0x00000080      //       128
#define PROCESS_OVERTEMP          0x00000800      //      2048
#define PROCESS_INTERLOCK         0x00008000      //
#define PROCESS_BUS_FAULT         0x00020000      //    131072
#define PROCESS_BUS_HIV           0x00040000      //    262144
#define PROCESS_BUS_LOV           0x00100000      //   1048576
#define PROCESS_INTERRUPT         0x00400000      //   4194304
#define PROCESS_CURRENT_LIMIT     0x01000000      //  16777216
#define PROCESS_INVRTR_NOT_READY  0x02000000      //
#define PROCESS_EXTENDED_FAULT    0x08000000      // 134217728
#define PROCESS_FAULT_PRESENT     0x20000000
#define PROCESS_WARNING_PRESENT   0x40000000


// PIN ELEMENT INDEXES
#define PIN_OUTPUT_POWER        1
#define PIN_OPERATING_FREQ      2
#define PIN_PRIMARY_COMM        3
#define PIN_SECONDARY_COMM      4
#define PIN_OUTPUT_CIRCULATOR   5
#define PIN_CEX                 6
#define PIN_RF_MEASUREMENT      7
#define PIN_PULSING             8
#define PIN_CTRL_LOOP_TIMING    9
#define PIN_ANA_IN_ENABLED      11
#define PIN_ANA_OUT_ASSIGN      12
#define PIN_DIG_IN_ENABLED      13
#define PIN_DIG_IN_POLARITY     14
#define PIN_DIG_OUT_ENABLED     15
#define PIN_DIG_OUT_POLARITY    16
#define PIN_DUAL_DC             17
#define PIN_SWEEP_TUNING        18
#define PIN_REF_FOLDBACK_TIMER  19
#define PIN_BAUD_RATE           20
#define PIN_AUTOBIAS            21
#define PIN_CONTROL_BOARD       22


// CONTROL NODES
#define CONTROL_VFP        2
#define CONTROL_USER       4

#define AEBUSW32_OK				0
#define AEBUSW32_WRITE_ERROR	(-1)
#define AEBUSW32_READ_ERROR		(-2)
#define AEBUSW32_TIMEOUT		(-3)
#define AEBUSW32_CKSUM			(-4)
#define AEBUSW32_NAK			(-5)
#define AEBUSW32_BAD_ACK        (-6)

#define ACK		0x06
#define NAK		0x15

#define AEBUS_DATA_SIZE         1024

#define AEBUS_REMOTE_RESERVED       0
#define AEBUS_REMOTE_GETVER         1
#define AEBUS_REMOTE_GETPORTS       2
#define AEBUS_REMOTE_CONNECT        3
#define AEBUS_REMOTE_DISCONNECT     4
#define AEBUS_REMOTE_LOCK           5
#define AEBUS_REMOTE_UNLOCK         6
#define AEBUS_REMOTE_GETSERVERINFO  7
#define AEBUS_REMOTE_FLUSH          8
#define AEBUS_REMOTE_SETTIMEOUT     9

#define AEBUS_ERROR         (-100)  // Any other error
#define AEBUS_WSERROR       (-101)  // Winsock error
#define AEBUS_DISCONNECTED  (-102)  // Disconnected from COM port
#define AEBUS_LOCKED        (-103)  // Server locked
#define AEBUS_FC100_NOT_AVAILABLE (-104)  // FC100 not implemented

#define AEBUS_REMOTE_TIMEOUT    10 // 10  seconds
#define AEBUS_LOCK_TIMEOUT      3   // Server lock timeout, s

#define AEBUS_EX_ERR            5   // Server exception: any error
#define AEBUS_EX_LOCKED         6   // Server exception: locked
#define MODBUS_EX_ILLEGAL_FUNCTION 1 // Illegal function code

//#define MY_COMPUTER     ""
#define LOCAL_HOST      "Local gateway"  //Host"
#define LOCAL_HOST_IP   "127.0.0.1"


//VFP_NAME:
// 1. Caption of the application
// 2. 'Product' value in the license.dat file
// 3. Registry key
// 4. Section name in the settings (vfp) file
// 5. Header in the log file

#define VFP_DATE            "07/24/2009"           // Displayed in About window

#define VFP_NAME            "Paramount VFP"
#define VFP_PRODUCT_NAME    "Paramount ®  Virtual Front Panel"
#define VFP_COPYRIGHT       "Copyright © 2009  by  Advanced Energy Industries, Inc."
#define VFP_ORGANIZATION    "Advanced Energy"
#define VFP_SW              "78000004"      //Full version SW #
#define VFP_VERSION          "B.00" //emsys

//emsys
#define VFP_KEY             "Software\\Advanced Energy Industries, Inc.\\"
#define VFP_MASTER_PASS     "AEAdmin"               // Password for everything
#define HELP_FILE       "help\\index.htm"      // Help file name



//emsys
#define COMM_ON    0
#define COMM_COMM   1
#define COMM_ERR    2
#define COMM_OFF    3
#define COMM_GLOBE  4


//emsys
#define CFG_SERIAL      0
#define CFG_PWRLIMS     1
#define CFG_TIMERS      2
#define CFG_RAMP        3
#define CFG_NETWORK     4
#define CFG_PULSING     5
#define CFG_SWEEP_FREQ  6
#define CFG_GAMMA_FREQ  7
#define CFG_DI          8
#define CFG_CEX         9
#define CFG_FIXED_FREQ  10
#define CFG_TUNING_LAM  11
#define CFG_ARC         12
#define CFG_RTC         13

#define CFG_SIZE        14
#define LOCAL_HOST      "Local gateway"  //Host"

#endif
