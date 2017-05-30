#ifndef MACROS_H
#define MACROS_H

// local typedefs
typedef unsigned char	uint_8;
typedef signed char	int_8;
typedef unsigned int	uint_16;
typedef signed int	int_16;
typedef unsigned long	uint_32;
typedef signed long	int_32;

//Various Macros by: Ryan Furry 28/03/16

//Mask off lower byte of a value
#define LOW(value)  ((value&0xFF))

//Set a bit pattern in any I/O register
#define SET_BITS(address, mask) ((address) |=(mask))

//Clear a bit pattern in any I/O register
#define CLR_BITS(address, mask) ((address) &= LOW(~(mask)))

//Flip an arbitrary pattern of in any I/O register
#define FLIP_BITS(address, mask)  ((address) ^= (mask))

//Force the bits of any I/O register to a specified value
#define FORCE_BITS(port, mask, value) ((port) = (port) &LOW(~(mask)) | ((value) & (mask)))

//Make a timer channel output compare
#define MAKE_CHNL_OC(chnl)  SET_BITS(TIOS, 1 << (chnl) )

//Make a timer channel input capture
#define MAKE_CHNL_IC(chnl)	CLR_BITS(TIOS, 1 << (chnl) )

//Clear a timer channel flag
#define CLR_TIMER_CHNL_FLAG(chnl)	TFLG1 = (1 << (chnl))

//Enable interupts on specific channel
#define CHNL_EN_INTERRUPTS(chnl)  SET_BITS(TIE,(1<<(chnl)))

//Force subset of a word to specified value
#define FORCE_WORD(port, mask, value)    			\
	(port) = ((port) & ((~(mask)) & 0xFFFF) | ((value) & (mask)))

//Set output compare event action for a specified channel
#define SET_OC_ACTION(chnl, action)     			\
        FORCE_WORD(TCTL_1_2, 				\
			            (0x03 << ((chnl) * 2)), 		\
			            ((action) << ((chnl) * 2)))

//Force an action after next timer tick
#define FORCE_OC_ACTION_NOW(chnl, action )    	\
        	 SET_OC_ACTION((chnl), (action));	 	\
	         SET_BITS( CFORC, 1 << (chnl))

#define BIT_IS_SET( port, bits )			((port) & (bits))
#define BIT_IS_CLR( port, bits )			((~(port))&(bits))

#define LO_NYBBLE( value )  ((value) & 0x0F)
#define HI_NYBBLE( value )  (((value) >> 4) & 0x0F)

#endif
