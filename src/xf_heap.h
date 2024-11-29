/**
 * @file xf_heap.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 堆内存接口。
 * @version 0.1
 * @date 2023-11-14
 *
 * @copyright Copyright (c) 2023, CorAL. All rights reserved.
 *
 * @note
 * - 此封装仅用于隔离内存管理算法，方便用户替换。
 * - 通过 xf_heap_redirect() 函数，用户重定向内存管理实现。
 * - 重定向内存管理实现时，必须在自动初始化早期阶段重定向。
 *   (太早)可能没有重定向成功，从而调用默认的内存管理算法 xf_alloc.c.
 *   (太晚)或者已经有别的应用使用了堆内存，之后再重定向的话可能导致内存泄漏等错误。
 */
#ifndef __XF_HEAP_H__
#define __XF_HEAP_H__

/**
 * @defgroup group_xf_heap xf_heap
 * @brief xf_heap 堆内存接口。
 *
 * 通常直接使用即可，需要时可通过 xf_heap_redirect() 函数重定向内存管理实现。
 *
 */

/**
 * @ingroup group_xf_heap
 * @defgroup group_xf_heap_user 用户接口
 * @brief 如 xf_malloc, xf_free 等。
 *
 * xfusion 用户使用 xf_heap 时只需 `#include "xf_heap.h"` 即可。
 *
 */

/**
 * @ingroup group_xf_heap
 * @defgroup group_xf_heap_port 移植接口
 * @brief 注册堆内存管理实现操作集，以及注册内存区域。
 *
 * 对接 xf_heap 时只需 `#include "xf_heap_port.h"` 即可。
 *
 */

/**
 * @ingroup group_xf_heap
 * @defgroup group_xf_heap_internal 内部接口
 * @brief 内部的内存管理实现的相关 api。用户最好不要直接使用。
 */

/* ==================== [Includes] ========================================== */

#include "xf_heap_internal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] =========================================== */

/**
 * @ingroup group_xf_heap_port
 * @{
 */

/**
 * @brief 堆内存块结构体。
 */
typedef struct _xf_heap_region_t {
    unsigned char *stat_address;  /*!< 内存块起始地址 */
    unsigned int size_in_bytes;   /*!< 内存块大小 */
} xf_heap_region_t;

/**
 * @brief 内存分配操作集。
 */
typedef struct _xf_alloc_func_t {
    void *(*malloc)(unsigned int size);
    void (*free)(void *pv);
    unsigned int (*init)(const xf_heap_region_t *const regions);
    unsigned int (*get_block_size)(void *pv); /*!< 获取内存块的大小 */
} xf_alloc_func_t;

/**
 * End of group_xf_heap_port
 * @}
 */

/* ==================== [Global Prototypes] ================================= */

/**
 * @ingroup group_xf_heap_user
 * @{
 */

/**
 * @brief 申请内存。
 *
 * @param size 申请内存大小。
 * @return void*
 *      - NULL 失败
 *      - OTHER 成功
 */
void *xf_malloc(unsigned int size);

/**
 * @brief 释放内存。
 *
 * @param pv 释放内存的大小。
 *
 * @note 释放内存后记得将指针指向 NULL，以防出现访问未申请的内存。
 */
void xf_free(void *pv);

/**
 * End of group_xf_heap_port
 * @}
 */

/**
 * @ingroup group_xf_heap_port
 * @{
 */

/**
 * @brief 相关申请的函数重定向。
 *
 * @param func 重定向的函数。
 *
 * @note 该函数只能在未初始化之前调用。
 * @return int
 *      - 0 设置成功
 *      - -1 设置失败
 */
xf_heap_err_t xf_heap_redirect(xf_alloc_func_t func);

/**
 * @brief 内存初始化。
 *
 * @param regions 注册不同内存区域。
 *
 * @note regions 数组最后一个元素必须是 {}(`0`数组)。内部以此作为数组结束标志。
 * @note 该函数只能在 xf_malloc 之前调用。
 *
 * @return int
 *      - 0 设置成功
 *      - -1 设置失败
 */
int xf_heap_init(const xf_heap_region_t *const regions);

/**
 * @brief 内存反初始化。
 *
 * @return int
 *      - 0 设置成功
 *      - -1 设置失败
 */
int xf_heap_uninit(void);

/**
 * End of group_xf_heap_port
 * @}
 */

/**
 * @ingroup group_xf_heap_user
 * @{
 */

/**
 * @brief 获取内存总空闲大小。
 *
 * @return unsigned int 内存总空闲大小。
 */
unsigned int xf_heap_get_free_size(void);

/**
 * @brief 获取曾经最小的空闲内存块。
 *
 * @return unsigned int 内存块大小。
 */
unsigned int xf_heap_get_min_ever_free_size(void);

/**
 * End of group_xf_heap_port
 * @}
 */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HEAP_H__
