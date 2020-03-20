#ifndef _LCOM_VIDEO_GR_MACROS_H_
#define _LCOM_VIDEO_GR_MACROS_H_

#include <lcom/lcf.h>


#define LINEAR_FRAME_BUFFER     BIT(14)
#define BIOS_VIDEO_SERVICES     0x10  /* BIOS video services */
#define VBE_FUNCTION            0x4F /*function call */
#define VBE_MODE                0x02 /* Set VBE Mode*/
#define VBE_GRAPHICS_MODE       0x105 /*the VBE graphics mode*/
#define VBE_CALL                0x4F02 /*VBE call, function 02--set VBE mode*/




#endif /* _LCOM_VIDEO_GR_MACROS_H_ */

