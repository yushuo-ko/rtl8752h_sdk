/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dlps.h
  * @brief   DLPS implementation head file.
  * @author  lory_xu
  * @date    2014-08-05
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __DLPS_H
#define __DLPS_H


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "os_queue.h"
#include "otp.h"

#ifdef __cplusplus
extern "C" {
#endif




/** @defgroup DLPS_PLATFORM DLPS Platform
  * @brief Deep low power state support
  * @{
  */

/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup DLPS_PLATFORM_Exported_Types DLPS Platform Exported Types
  * @{
  */
typedef enum PMCheckResult
{
    PM_CHECK_PEND               = 0,
    PM_CHECK_FAIL               = 1,
    PM_CHECK_PASS               = 2,
} PMCheckResult;

/** @brief This CB is used for any module which needs to be checked before entering DLPS */
typedef PMCheckResult(*DLPSEnterCheckFunc)();

/** @brief PlatformPMStage struct */
typedef enum
{
    PLATFORM_PM_CHECK       = 0,
    PLATFORM_PM_STORE       = 1,
    PLATFORM_PM_ENTER       = 2,
    PLATFORM_PM_EXIT        = 3,
    PLATFORM_PM_RESTORE     = 4,
    PLATFORM_PM_PEND        = 5,
    PLATFORM_PM_STAGE_MAX   = 6,
} PlatformPMStage;


/** @brief PlatformPowerMode struct */
typedef enum
{
    PLATFORM_POWERDOWN      = 1,   /**< Power down */
    PLATFORM_DLPS_PFM       = 2,   /**< DLPS (PFM) */
    PLATFORM_ACTIVE         = 5,   /**< Active     */
} PlatformPowerMode;

typedef enum
{
    PLATFORM_PM_WAKEUP_UNKNOWN              = 0x00000000,
    PLATFORM_PM_WAKEUP_USER                 = 0x00000001,
    PLATFORM_PM_WAKEUP_OS                   = 0x00000002,
    PLATFORM_PM_WAKEUP_PRE_SYSTEM_LEVEL     = 0x00000003,
    PLATFORM_PM_WAKEUP_PF_RTC               = 0x00000100,
    PLATFORM_PM_WAKEUP_RTC                  = 0x00000200,
    PLATFORM_PM_WAKEUP_MAC                  = 0x00000400,
    PLATFORM_PM_WAKEUP_GPIO                 = 0x00000800,
    PLATFORM_PM_WAKEUP_CTC                  = 0x00001000,
    PLATFORM_PM_WAKEUP_DEF_MAX              = 0x7FFFFFFF,
} PlatformWakeupReason;

typedef enum
{
    PLATFORM_PM_ERROR_UNKNOWN               = 0x0,
    PLATFORM_PM_ERROR_POWER_MODE            = 0x1,
    PLATFORM_PM_ERROR_DISABLE_DLPS_TIME     = 0x2,
    PLATFORM_PM_ERROR_32K_CHECK_LOCK        = 0x3,
    PLATFORM_PM_ERROR_LOG_DMA_NOT_IDLE      = 0x4,
    PLATFORM_PM_ERROR_CALLBACK_CHECK        = 0x5,
    PLATFORM_PM_ERROR_INTERRUPT_OCCURRED    = 0x6,
    PLATFORM_PM_ERROR_WAKEUP_TIME           = 0x7,
    PLATFORM_PM_ERROR_DATA_UART             = 0x8,
    PLATFORM_PM_ERROR_OCCUPIED_TIME         = 0x9,
} PlatformPowerModeErrorCode;

/** @} */ /* End of group DLPS_PLATFORM_Exported_Types */

/*============================================================================*
 *                              Internal type
*============================================================================*/
/**
 * \cond        private
 * \brief       internal types.
 * \defgroup    DLPS_PLATFORM_Internal_Types DLPS Platform Internal Types
 * \{
 */

/** @brief This CB is used for any module which needs to control the hw before entering or after exiting from DLPS */
typedef void (*DLPSHWControlFunc)();

typedef PMCheckResult(*PMUnitCheckFunc)(uint32_t, uint32_t, uint32_t *);
typedef void (*PMUnitStoreFunc)(void);
typedef void (*PMUnitRestoreFunc)(void);
typedef void (*PMUnitEnterFunc)(void);
typedef void (*PMUnitExitFunc)(void);

#define PM_SLAVE_PROPRIETARY            PM_SLAVE_0

typedef enum PMUnitStatus
{
    PM_UNIT_UNKNOWN             = 0,
    PM_UNIT_PEND                = 1,
    PM_UNIT_INACTIVE            = 2,
    PM_UNIT_ACTIVE              = 3,
} PMUnitStatus;

typedef enum PMSystemLevel
{
    PM_SYSTEM_LEVEL_0           = 0,
    PM_SYSTEM_LEVEL_1           = 1,
    PM_SYSTEM_LEVEL_PF          = 2,
    PM_SYSTEM_LEVEL_DEF_MAX,
} PMSystemLevel;

typedef enum
{
    PM_UNIT_PROPRIETARY      = (PM_SYSTEM_LEVEL_0 << 2 | 0),
    PM_UNIT_ZIGBEE           = (PM_SYSTEM_LEVEL_0 << 2 | 1),
    PM_UNIT_BTMAC            = (PM_SYSTEM_LEVEL_1 << 2 | 0),
    PM_UNIT_PLATFORM         = (PM_SYSTEM_LEVEL_PF << 2 | 0),
    PM_UNIT_HEADER_ID        = 0x80,
} PMUnitID;

typedef enum
{
    PROFILING_PM_SLAVE_CHECK        = 0,
    PROFILING_PM_SLAVE_STORE        = 1,
    PROFILING_PM_SLAVE_ENTER        = 2,
    PROFILING_PM_SLAVE_EXIT         = 3,
    PROFILING_PM_SLAVE_RESTORE      = 4,
    PROFILING_PM_SLAVE_STAGE_MAX    = 5
} ProfilingPMSlaveStage;

typedef struct
{
    uint32_t stage_start;
    uint32_t stage_end;
    uint32_t stage_time[PROFILING_PM_SLAVE_STAGE_MAX];
    uint32_t stage_time_max[PROFILING_PM_SLAVE_STAGE_MAX];
    uint32_t stage_time_min[PROFILING_PM_SLAVE_STAGE_MAX];
} ProfilingPMSlaveData;

typedef struct
{
    PMUnitCheckFunc check_func;
    PMUnitStoreFunc store_func;
    PMUnitRestoreFunc restore_func;
    PMUnitEnterFunc enter_func;
    PMUnitExitFunc exit_func;

    PMUnitID unit_id;

    ProfilingPMSlaveData *profiling_data;
} PowerManagerSlaveUnit;

typedef enum PMSlave
{
    PM_SLAVE_0                  = 0,
    PM_SLAVE_DEF_MAX,
} PMSlave;

typedef enum
{
    NO_DATARAM_SHUTDOWN,
    LAST_4K_DATARAM_SHUTDOWN,    /**< 4k */
    LAST_8K_DATARAM_SHUTDOWN,    /**< 4k + 4k */
    MIDDLE_32K_DATARAM_SHUTDOWN, /**< 32k, last 8k retention*/
    LAST_40K_DATARAM_SHUTDOWN,   /**< 32k + 4k + 4k */
    LAST_72K_DATARAM_SHUTDOWN    /**< 32k + 32k + 4k + 4k */
} DATARAM_SHUTDOWN_LEVEL;

typedef enum
{
    BTMAC_DEEP_SLEEP        = 0,   /**< Deep sleep */
    BTMAC_ACTIVE            = 1,   /**< Active     */
} BtmacPowerMode;

typedef enum
{
    BTMAC_PM_WAKEUP_UNKNOWN                 = 0x0,
    BTMAC_PM_WAKEUP_LEGACY                  = 0x1,
    BTMAC_PM_WAKEUP_LE                      = 0x2,
    BTMAC_PM_WAKEUP_PRE_SYSTEM_LEVEL        = 0x3,
} BtmacWakeupReason;

typedef enum
{
    BTMAC_PM_ERROR_UNKNOWN                  = 0x0,
    BTMAC_PM_ERROR_POWER_MODE               = 0x1,
    BTMAC_PM_ERROR_LEGACY_SCAN              = 0x2,
    BTMAC_PM_ERROR_ROLE_SWITCH              = 0x3,
    BTMAC_PM_ERROE_BQB                      = 0x4,
    BTMAC_PM_ERROR_PSD                      = 0x5,
    BTMAC_PM_ERROR_CSB_ENABLE               = 0x6,
    BTMAC_PM_ERROR_NOT_EMPTY_QUEUE_OF_LOWER = 0x7,
    BTMAC_PM_ERROR_CONTROLLER_TO_HOST_BUSY  = 0x8,
    BTMAC_PM_ERROR_TX_BUSY                  = 0x9,
    BTMAC_PM_ERROR_LEGACY_NOT_IDLE          = 0xA,
    BTMAC_PM_ERROR_LE_REG_S_INST            = 0xB,
    BTMAC_PM_ERROR_ADV_STATE_NOT_IDLE       = 0xC,
    BTMAC_PM_ERROR_SCAN_STATE_NOT_IDLE      = 0xD,
    BTMAC_PM_ERROR_INITIATOR_UNIT_ENABLE    = 0xE,
    BTMAC_PM_ERROR_CHANNEL_MAP_UPDATE       = 0xF,
    BTMAC_PM_ERROR_CONNECTION_UPDATE        = 0x10,
    BTMAC_PM_ERROR_PHY_UPDATE               = 0x11,
    BTMAC_PM_ERROR_CONN_STATE_NOT_IDLE      = 0x12,
    BTMAC_PM_ERROR_LE_SCHE_NOT_READY        = 0x13,
    BTMAC_PM_ERROR_INTERRUPT_PENDING        = 0x14,
    BTMAC_PM_ERROR_WAKEUP_TIME              = 0x15,
    BTMAC_PM_ERROR_32K_CHECK_LOCK           = 0x16,
    BTMAC_PM_ERROR_HW_TIMER_RUNNING         = 0x17,
    BTMAC_PM_ERROR_LE_ISO_ACTIVE            = 0x18,
    BTMAC_PM_ERROR_OCCUPIED_TIME            = 0x19,
} BtmacPowerModeErrorCode;

/**
 * \}
 * \endcond
 */   /* End of group DLPS_PLATFORM_Internal_Types */

/*============================================================================*
 *                              Variables
*============================================================================*/

/**
 * \cond        private
 * \brief       internal types.
 * \defgroup    DLPS_PLATFORM_Internal_Variables DLPS Platform Internal Variables
 * \{
 */

typedef union _PlatformPMFeatureConfig
{
    uint8_t value[1];
    struct
    {
        uint8_t platform_check_dbg1: 1;
        uint8_t platform_check_dbg2: 1;
        uint8_t platform_enter_dbg: 1;
        uint8_t platform_exit_dbg: 1;
        uint8_t platform_stage_time: 1;
        uint8_t platform_statistics: 1;
        uint8_t rsvd: 2;
    };
} PlatformPMFeatureConfig;

extern PlatformPMFeatureConfig platform_pm_feature_cfg;

typedef union _BtmacPMFeatureConfig
{
    uint8_t value[1];
    struct
    {
        uint8_t btmac_check_dbg: 1;
        uint8_t btmac_enter_dbg: 1;
        uint8_t btmac_exit_dbg: 1;
        uint8_t btmac_stage_time: 1;
        uint8_t btmac_statistics: 1;
        uint8_t rsvd: 3;
    };
} BtmacPMFeatureConfig;

extern BtmacPMFeatureConfig btmac_pm_feature_cfg;

/**
 * \}
 * \endcond
 */   /* End of group DLPS_PLATFORM_Internal_Variables */

/*============================================================================*
 *                         Internal Functions
*============================================================================*/
/**
 * \cond        private
 * \brief       internal types.
 * \defgroup    DLPS_PLATFORM_Internal_Functions DLPS Platform Internal Functions
 * \{
 */

extern void platform_pm_set_power_mode(PlatformPowerMode pf_power_mode_user);
extern PlatformPowerMode platform_pm_get_power_mode(void);
extern void btmac_pm_set_power_mode(BtmacPowerMode bz_power_mode_user);
extern void power_manager_slave_suspend_all(void);
extern void power_manager_slave_resume_all(void);
extern void (*platform_pm_register_callback_func)(void *cb_func, PlatformPMStage pf_pm_stage);

extern void platform_pm_get_statistics(uint32_t *wakeup_count, uint32_t *last_wakeup_clk,
                                       uint32_t *last_sleep_clk);
extern void btmac_pm_get_statistics(uint32_t *wakeup_count, uint32_t *last_wakeup_clk,
                                    uint32_t *last_sleep_clk);
extern void power_manager_slave_register_unit(PMUnitID unit_id, PowerManagerSlaveUnit *p_unit);
extern PMUnitStatus power_manager_interface_get_unit_status(PMSlave slave, PMUnitID unit_id);
extern void (*power_manager_master_initiate_wakeup)(PMSlave slave, PMSystemLevel sys_lvl,
                                                    PMUnitID unit_id);
/**
 * \}
 * \endcond
 */   /* End of group DLPS_PLATFORM_Internal_Functions */

/*============================================================================*
 *                         External Functions
*============================================================================*/

/** @defgroup DLPS_PLATFORM_Exported_Functions DLPS Platform Exported Functions
  * @{
  */

/**
 * @brief  get platform error code which prevent system from entering dlps.
 * @param  none
 * @return platform error code
*/
PlatformPowerModeErrorCode platform_pm_get_error_code(void);

/**
 * @brief  get platform refuse check function which prevent system from entering dlps.
 * @param  none
 * @return platform refuse check function
*/
uint32_t *platform_pm_get_refuse_reason(void);

/**
 * @brief  get platform wake up reason which wake system to exit dlps.
 * @param  none
 * @return platform wake up reason
*/
extern PlatformWakeupReason platform_pm_get_wakeup_reason(void);

/**
 * @brief  stop all sw timer to enter into power down mode.
 * @param  none
 * @return none
*/
extern void platform_pm_stop_all_non_excluded_timer(void);

/**
 * @brief Register Check CB to DlpsPlatform which will call it before entering Dlps.
 * @param  func DLPSEnterCheckFunc
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
static inline bool dlps_check_cb_reg(DLPSEnterCheckFunc func)
{
    platform_pm_register_callback_func(func, PLATFORM_PM_CHECK);
    return true;
}

/**
 * @brief Register HW Control CB to DlpsPlatform which will call it before entering Dlps or after exiting from Dlps (according to dlpsState) .
 * @param  func DLPSHWControlFunc
 * @param  dlpsState tell the DlpsPlatform the CB should be called when DLPS_ENTER or DLPS_EXITx_XXX.
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
static inline bool dlps_hw_control_cb_reg(DLPSHWControlFunc func, PlatformPMStage dlpsState)
{
    platform_pm_register_callback_func(func, dlpsState);
    return true;
}

/**
 * @brief Keep platform in @ref LPM_ACTIVE_MODE which means will stop platform enter any lower power mode.
 * @param  none
 * @return  none
*/
static inline void lps_mode_pause(void)
{
    power_manager_slave_suspend_all();
}

/**
 * @brief Restore to original LPSMode.
 * @param  none
 * @return  none
*/
static inline void lps_mode_resume(void)
{
    power_manager_slave_resume_all();
}

/**
 * @brief Set active time after boot before entering into dlps
 * @param  active_time_ms  time to keep active, unit ms
 * @return  none
*/
void set_boot_active_time(uint32_t active_time_ms);

/**
 * @brief  LPSMode Set .
 * @param  mode LPSMode
 * @return  none
*/
static inline void lps_mode_set(PlatformPowerMode mode)
{
    btmac_pm_set_power_mode(BTMAC_DEEP_SLEEP);
    platform_pm_set_power_mode(mode);
}

/**
 * @brief  LPSMode Get .
 * @param  none
 * @return  @ref LPSMode
*/
static inline PlatformPowerMode lps_mode_get(void)
{
    return platform_pm_get_power_mode();
}

/**
 * @brief  Return dlps wakeup counts .
 * @param  none
 * @return  count value
*/
static inline  uint32_t lps_wakeup_count_get(void)
{
    uint32_t wakeup_count, last_wakeup_clk, last_sleep_clk;
    platform_pm_get_statistics(&wakeup_count, &last_wakeup_clk, &last_sleep_clk);
    return wakeup_count;
}

/**
 * @brief  Set data ram to shut down instead of deep sleep when enter dlps to reduce power consumption.
           Data ram(total 104k) divides into 5 blocks: 32k + 32k + 32k + 4k + 4k.
 * @param  level
 * @return none
  */
static inline void set_dataram_to_shutdown(DATARAM_SHUTDOWN_LEVEL level)
{
    switch (level)
    {
    case LAST_4K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_LOWPOWER].DS_retsram_4_0 = 0xf;
            OTP->ram_cfg[MEMCFG_LOWPOWER].SD_retsram_4_0 = 0x10;
        }
        break;
    case LAST_8K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_LOWPOWER].DS_retsram_4_0 = 0x7;
            OTP->ram_cfg[MEMCFG_LOWPOWER].SD_retsram_4_0 = 0x18;
        }
        break;
    case MIDDLE_32K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_LOWPOWER].DS_retsram_4_0 = 0x1b;
            OTP->ram_cfg[MEMCFG_LOWPOWER].SD_retsram_4_0 = 0x4;
        }
        break;
    case LAST_40K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_LOWPOWER].DS_retsram_4_0 = 0x3;
            OTP->ram_cfg[MEMCFG_LOWPOWER].SD_retsram_4_0 = 0x1c;
        }
        break;
    case LAST_72K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_LOWPOWER].DS_retsram_4_0 = 0x1;
            OTP->ram_cfg[MEMCFG_LOWPOWER].SD_retsram_4_0 = 0x1e;
        }
        break;
    default:
        break;
    }
}

/** @} */ /* End of group DLPS_PLATFORM_Exported_Functions */

/** @} */ /* End of group DLPS_PLATFORM */


#ifdef __cplusplus
}
#endif

#endif  /* __DLPS_PLATFORM_H */
