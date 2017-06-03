//  Catapult University Version 8.2b/244325 (Production Release) Fri Aug 28 15:37:28 PDT 2015
//  
//  Copyright (c) Calypto Design Systems, Inc., 1996-2015, All Rights Reserved.
//                       UNPUBLISHED, LICENSED SOFTWARE.
//            CONFIDENTIAL AND PROPRIETARY INFORMATION WHICH IS THE
//          PROPERTY OF CALYPTO DESIGN SYSTEMS OR ITS LICENSORS
//  
//  Running on Windows 7 Fitkit@FITKIT-PC Service Pack 1 6.01 i686
//  
//  Package information: SIFLIBS v21.2_2.0, HLS_PKGS v21.2_2.0, 
//                       DesignPad v2.78_1.0
//  
//  This version may only be used for academic purposes.  Some optimizations 
//  are disabled, so results obtained from this version may be sub-optimal.
//  
solution new -state initial
solution options defaults
solution options set /Flows/Precision/addio false
solution file add {./src_filter/addr_space.h} -type CHEADER
solution file add {./src_filter/filter.h} -type CHEADER
solution file add {./src_filter/filter.cpp} -type C++
directive set -IO_MODE super
directive set -STALL_FLAG false
directive set -BLOCK_SYNC none
directive set -TRANSACTION_SYNC ready
directive set -DATA_SYNC none
directive set -CHARACTERIZE_ROM false
directive set -PROTOTYPE_ROM true
directive set -ROM_THRESHOLD 64
directive set -CLUSTER_INPUT_WIDTH_THRESHOLD 0
directive set -CLUSTER_OPT_CONSTANT_INPUTS true
directive set -CLUSTER_RTL_SYN false
directive set -CLUSTER_FAST_MODE false
directive set -CLUSTER_TYPE combinational
directive set -REGISTER_IDLE_SIGNAL false
directive set -IDLE_SIGNAL {}
directive set -TRANSACTION_DONE_SIGNAL false
directive set -DONE_FLAG {}
directive set -START_FLAG {}
directive set -FSM_ENCODING none
directive set -REG_MAX_FANOUT 0
directive set -NO_X_ASSIGNMENTS true
directive set -SAFE_FSM false
directive set -RESET_CLEARS_ALL_REGS true
directive set -ASSIGN_OVERHEAD 0
directive set -DESIGN_GOAL area
directive set -OLD_SCHED false
directive set -PIPELINE_RAMP_UP true
directive set -COMPGRADE fast
directive set -SPECULATE true
directive set -MERGEABLE true
directive set -REGISTER_THRESHOLD 256
directive set -MEM_MAP_THRESHOLD 32
directive set -UNROLL no
directive set -CLOCK_OVERHEAD 50.000000
directive set -OPT_CONST_MULTS -1
go analyze
solution library add mgc_Xilinx-SPARTAN3-4_beh_psr -- -rtlsyntool {Precision 2014a} -manufacturer Xilinx -family SPARTAN3 -speed -4 -part 3s50tq144
solution library add ram_Xilinx-SPARTAN3-4_RAMSB
directive set -DESIGN_HIERARCHY filter
directive set -CLOCK_NAME clk
directive set -CLOCKS {clk {-CLOCK_PERIOD 40.0 -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME 20.0 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_ACTIVE high}}
go compile
directive set /filter/core/main -PIPELINE_INIT_INTERVAL 4
directive set /filter/out_data:rsc -MAP_TO_MODULE mgc_ioport.mgc_out_stdreg_en
directive set /filter/in_data:rsc -MAP_TO_MODULE mgc_ioport.mgc_in_wire_en
go libraries
go assembly
directive set /filter/core/L1a -UNROLL yes
directive set /filter/core/L1b -UNROLL yes
directive set /filter/core/Linit -UNROLL yes
directive set /filter/core/L2 -UNROLL yes
go architect
go extract
flow run /Precision/precision -shell -file ./rtl.vhdl.psr -run_state mapped
