wire trace_valid;
        assign trace_valid=(!clock_cross_kernel_mem1_m0_waitrequest) && (clock_cross_kernel_mem1_m0_write || clock_cross_kernel_mem1_m0_read);
avalonst_sink_valid          (trace_valid),    //        in.valid
                .avalonst_sink_data           (clock_cross_kernel_mem1_m0_address),                                      //          .data

