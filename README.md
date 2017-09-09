# fpga-hammer
An OpenCL program to rowhammer Linux from the FPGA side.

This project aims at reproducing/characterizing rowhammer attack on recent SDRAM
chips (~40nm). We use a popular SoCFPGA platform (DE1SoC). The SDRAM DDR3 memory 
on board is hammered from the FPGA. There is a configurable hardware block programmed 
in FPGA capable of hamering the SDRAM. The following parameters can be configured

# Address Range
# Refresh Interval
# Activation Interval.
# Data Pattern(RowStripe, Chekered adn Random).

The rowhammer fault injection is achieved by repeatedly opening and closing a SDRAM
row (agressor) within the same bank. The neighbouring rows are discharged in this process. And if
done sufficient number of times within the refresh interval it can flip bits in the 
neighbouring rows (victim). The agressor row itself is unchanged as its charge content is 
reinforced through read operation. The DDR chip tested is the ISSI IS43/46TR16256A, which 
is organised as two 256M x 16 chips with a total memory size of 1GB. The page size is 4KB, 
and the hammering addresses are calculated acocrdingly. We tried threee different patterns

RowStripe: Alternate rows of zero and one
Chekered: 0s and 1s organised as in chessboard.
Random:  random strings of 0s and 1s.

As noted by the rowhammer paper(~smith) teh Rowstripe pattern gives the maximum number of 
errors. We vary the refresh Interval from 32ms to 128ms. The refresh interval in DDR3 standard is 
64ms. The activation interval, i.e the time between two hammers is ~50 ns on average. This is the 
minimum due to DDR3 row cycle time constraint.
