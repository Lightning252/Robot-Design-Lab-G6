# GDB startup script for debugging program in STM32F103 flash ROM

# $Id: debug.gdb,v 1.1 2008/03/07 20:34:47 cvs Exp $

# Connect to the OpenOCD server


target remote localhost:3333 


# Reset target & gain control

monitor soft_reset_halt
monitor wait_halt

# We can't use software breakpoints because we are running from ROM

symbol-file main.out

#monitor arm7_9 force_hw_bkpts enable

# Break at beginning of main()

#hbreak main
#hb I2C1_EV_IRQHandler
#hb I2C1_ER_IRQHandler
hb HardFaultException
hb _default_handler
continue
