/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file      rtl876x_io_dlps.h
* \brief
* \details
* \author    tifnan_ge
* \date      2024-01-22
* \version   v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL876X_IO_DLPS_H_
#define _RTL876X_IO_DLPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl876x_bitfields.h"
#include "board.h"

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    IO_DLPS     IO DLPS
 *
 * \brief       Manage the IO DLPS peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    IO_DLPS_Exported_Types IO DLPS Exported Types
 * \{
 * \ingroup     IO_DLPS
 */

typedef void (*DLPS_IO_ExitDlpsCB)(void);
typedef void (*DLPS_IO_EnterDlpsCB)(void);

/** End of group IO_DLPS_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    IO_DLPS_Exported_Functions IO DLPS Exported Functions
 * \{
 * \ingroup     IO_DLPS
 */

/**
  * \brief  Register io restore function in dlps mode
  * \param  None
  * \return None
  */
extern void DLPS_IORegister(void);
#if USE_USER_DEFINE_DLPS_EXIT_CB

extern DLPS_IO_ExitDlpsCB User_IO_ExitDlpsCB;

/**
  * \brief  Rrgister user-defined exit dlps callback function
  * \param  func: user-defined callback functon.
  * \return None
  */
__STATIC_INLINE void DLPS_IORegUserDlpsExitCb(DLPS_IO_ExitDlpsCB func)
{
    User_IO_ExitDlpsCB = func;
}

#endif /* USE_USER_DEFINE_DLPS_EXIT_CB */

#if USE_USER_DEFINE_DLPS_ENTER_CB

extern DLPS_IO_EnterDlpsCB User_IO_EnterDlpsCB;

/**
  * \brief  Rrgister user-defined enter dlps callback function
  * \param  func: user-defined callback functon.
  * \return None
  */
__STATIC_INLINE void DLPS_IORegUserDlpsEnterCb(DLPS_IO_EnterDlpsCB func)
{
    User_IO_EnterDlpsCB = func;
}

#endif

/** End of IO_DLPS_Exported_Functions
  * \}
  */

/** End of IO_DLPS
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_IO_DLPS_H_ */



/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/
