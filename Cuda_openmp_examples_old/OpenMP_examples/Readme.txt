This file will guide you throu this folder.

Cpu tile example with openmp is in maintile.c

We wrote a ticket abaut a bug with clang12 and 14 mainslow.c and the Ticket_szoveg.txt

The code was optimized and implemented 1d and 3d indexing in mainoptim.c use with build script

previous code was tested with a trimed version mainoptim_difftest.c with build2 and Optim_testing 

there was a indexing fail due to the bug in openmp and was tested in mainoptim_3dindexfail.c build3
and continued with a shorter example in targetpointererror.c

mainbase and main1d was the base came from gputest folder