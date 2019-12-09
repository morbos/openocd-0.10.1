/***************************************************************************
 *   Copyright (C) 2005 by Dominic Rath                                    *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   Copyright (C) 2006 by Magnus Lundin                                   *
 *   lundin@mlu.mine.nu                                                    *
 *                                                                         *
 *   Copyright (C) 2008 by Spencer Oliver                                  *
 *   spen@spen-soft.co.uk                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef OPENOCD_TARGET_ARMV8M_H
#define OPENOCD_TARGET_ARMV8M_H

#include "arm_adi_v5.h"
#include "arm.h"
#include "armv8m_trace.h"

extern const int armv8m_psp_reg_map[];
extern const int armv8m_msp_reg_map[];

const char *armv8m_exception_string(int number);

/* offsets into armv8m core register cache */
enum {
	/* for convenience, the first set of indices match
	 * the Cortex-M3/-M4 DCRSR selectors
	 */
	ARMV8M_R0,
	ARMV8M_R1,
	ARMV8M_R2,
	ARMV8M_R3,

	ARMV8M_R4,
	ARMV8M_R5,
	ARMV8M_R6,
	ARMV8M_R7,

	ARMV8M_R8,
	ARMV8M_R9,
	ARMV8M_R10,
	ARMV8M_R11,

	ARMV8M_R12,
	ARMV8M_R13,
	ARMV8M_R14,
	ARMV8M_PC = 15,

	ARMV8M_xPSR = 16,
	ARMV8M_MSP,
	ARMV8M_PSP,

	/* this next set of indices is arbitrary */
	ARMV8M_PRIMASK, 
	ARMV8M_BASEPRI,
	ARMV8M_FAULTMASK,
	ARMV8M_CONTROL,

	ARMV8M_MSP_NS = 24,
	ARMV8M_PSP_NS,
        ARMV8M_MSP_S,
	ARMV8M_PSP_S,
	ARMV8M_MSPLIM_S, 
	ARMV8M_PSPLIM_S,
        ARMV8M_MSPLIM_NS,
	ARMV8M_PSPLIM_NS,

	ARMV8M_FPSCR = 33,

	ARMV8M_PRIMASK_S, 
	ARMV8M_BASEPRI_S,
	ARMV8M_FAULTMASK_S,
	ARMV8M_CONTROL_S,

	ARMV8M_PRIMASK_NS, 
	ARMV8M_BASEPRI_NS,
	ARMV8M_FAULTMASK_NS,
	ARMV8M_CONTROL_NS,
	
	ARMV8M_VPR = 36,
	
	/* 32bit Floating-point registers */
	ARMV8M_S0 = 64,
	ARMV8M_S1,
	ARMV8M_S2,
	ARMV8M_S3,
	ARMV8M_S4,
	ARMV8M_S5,
	ARMV8M_S6,
	ARMV8M_S7,
	ARMV8M_S8,
	ARMV8M_S9,
	ARMV8M_S10,
	ARMV8M_S11,
	ARMV8M_S12,
	ARMV8M_S13,
	ARMV8M_S14,
	ARMV8M_S15,
	ARMV8M_S16,
	ARMV8M_S17,
	ARMV8M_S18,
	ARMV8M_S19,
	ARMV8M_S20,
	ARMV8M_S21,
	ARMV8M_S22,
	ARMV8M_S23,
	ARMV8M_S24,
	ARMV8M_S25,
	ARMV8M_S26,
	ARMV8M_S27,
	ARMV8M_S28,
	ARMV8M_S29,
	ARMV8M_S30,
	ARMV8M_S31,

	/* 64bit Floating-point registers */
	ARMV8M_D0,
	ARMV8M_D1,
	ARMV8M_D2,
	ARMV8M_D3,
	ARMV8M_D4,
	ARMV8M_D5,
	ARMV8M_D6,
	ARMV8M_D7,
	ARMV8M_D8,
	ARMV8M_D9,
	ARMV8M_D10,
	ARMV8M_D11,
	ARMV8M_D12,
	ARMV8M_D13,
	ARMV8M_D14,
	ARMV8M_D15,

	/* Floating-point status registers */
	ARMV8M_FPSID,
	ARMV8M_FPEXC,

	ARMV8M_LAST_REG,
};

enum {
	FP_NONE = 0,
	FPv4_SP,
	FPv5_SP,
	FPv5_DP,
};

//#define ARMV8M_NUM_CORE_REGS (ARMV8M_VPR + 1)
//#define ARMV8M_NUM_CORE_REGS (ARMV8M_xPSR + 18)

#if 0
#define ARMV8M_NUM_CORE_REGS (ARMV8M_xPSR + 1)
#define ARMV8M_NUM_CORE_REGS_NOFP (ARMV8M_NUM_CORE_REGS + 6)
#else
#define ARMV8M_NUM_CORE_REGS (ARMV8M_xPSR + 1)
#define ARMV8M_NUM_CORE_REGS_NOFP (ARMV8M_NUM_CORE_REGS + 22)
#endif


#define ARMV8M_COMMON_MAGIC 0x2A452A45

struct armv8m_common {
	struct arm	arm;

	int common_magic;
	int exception_number;

	/* AP this processor is connected to in the DAP */
	struct adiv5_ap *debug_ap;

	int fp_feature;
	uint32_t demcr;

	/* stlink is a high level adapter, does not support all functions */
	bool stlink;

	struct armv8m_trace_config trace_config;

	/* Direct processor core register read and writes */
	int (*load_core_reg_u32)(struct target *target, uint32_t num, uint32_t *value);
	int (*store_core_reg_u32)(struct target *target, uint32_t num, uint32_t value);

	int (*examine_debug_reason)(struct target *target);
	int (*post_debug_entry)(struct target *target);

	void (*pre_restore_context)(struct target *target);
};

static inline struct armv8m_common *
target_to_armv8m(struct target *target)
{
	return container_of(target->arch_info, struct armv8m_common, arm);
}

static inline bool is_armv8m(struct armv8m_common *armv8m)
{
	return armv8m->common_magic == ARMV8M_COMMON_MAGIC;
}

struct armv8m_algorithm {
	int common_magic;

	enum arm_mode core_mode;

	uint32_t context[ARMV8M_LAST_REG]; /* ARMV8M_NUM_REGS */
};

struct reg_cache *armv8m_build_reg_cache(struct target *target);
void armv8m_free_reg_cache(struct target *target);

enum armv8m_mode armv8m_number_to_mode(int number);
int armv8m_mode_to_number(enum armv8m_mode mode);

int armv8m_arch_state(struct target *target);
int armv8m_get_gdb_reg_list(struct target *target,
		struct reg **reg_list[], int *reg_list_size,
		enum target_register_class reg_class);

int armv8m_init_arch_info(struct target *target, struct armv8m_common *armv8m);

int armv8m_run_algorithm(struct target *target,
		int num_mem_params, struct mem_param *mem_params,
		int num_reg_params, struct reg_param *reg_params,
		uint32_t entry_point, uint32_t exit_point,
		int timeout_ms, void *arch_info);

int armv8m_start_algorithm(struct target *target,
		int num_mem_params, struct mem_param *mem_params,
		int num_reg_params, struct reg_param *reg_params,
		uint32_t entry_point, uint32_t exit_point,
		void *arch_info);

int armv8m_wait_algorithm(struct target *target,
		int num_mem_params, struct mem_param *mem_params,
		int num_reg_params, struct reg_param *reg_params,
		uint32_t exit_point, int timeout_ms,
		void *arch_info);

int armv8m_invalidate_core_regs(struct target *target);

int armv8m_restore_context(struct target *target);

int armv8m_checksum_memory(struct target *target,
		uint32_t address, uint32_t count, uint32_t *checksum);
int armv8m_blank_check_memory(struct target *target,
		uint32_t address, uint32_t count, uint32_t *blank, uint8_t erased_value);

int armv8m_maybe_skip_bkpt_inst(struct target *target, bool *inst_found);

extern const struct command_registration armv8m_command_handlers[];

#endif /* OPENOCD_TARGET_ARMV8M_H */
