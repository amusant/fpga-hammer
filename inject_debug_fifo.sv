wire trace_valid;
        assign trace_valid=(!clock_cross_kernel_mem1_m0_waitrequest) && (clock_cross_kernel_mem1_m0_write || clock_cross_kernel_mem1_m0_read);
avalonst_sink_valid          (trace_valid),    //        in.valid
                .avalonst_sink_data           (clock_cross_kernel_mem1_m0_address),                                      //          .data

.f2h_AWCACHE                   (4'b1011),      //                    .awcache
.f2h_AWUSER                    (5'b11111),       //                    .awuser
.f2h_ARCACHE                   (4'b1011),      //                    .arcache
.f2h_ARUSER                    (5'b11111),       //                    .aruser

