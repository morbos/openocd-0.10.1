/***************************************************************************
 *   Copyright (C) 2015  Paul Fertser <fercerpav@gmail.com>                *
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

#ifndef OPENOCD_TARGET_ARMV8M_TRACE_H
#define OPENOCD_TARGET_ARMV8M_TRACE_H

#include <target/target.h>
#include <command.h>

/**
 * @file
 * Holds the interface to TPIU, ITM and DWT configuration functions.
 */

enum trace_config_type_v8m {
	DISABLED_V8M,	/**< tracing is disabled */
	EXTERNAL_V8M,	/**< trace output is captured externally */
	INTERNAL_V8M	/**< trace output is handled by OpenOCD adapter driver */
};

enum tpio_pin_protocol_v8m {
	SYNC_V8M,			/**< synchronous trace output */
	ASYNC_MANCHESTER_V8M,	/**< asynchronous output with Manchester coding */
	ASYNC_UART_V8M		/**< asynchronous output with NRZ coding */
};

enum itm_ts_prescaler_v8m {
	ITM_TS_PRESCALE1_V8M,	/**< no prescaling for the timestamp counter */
	ITM_TS_PRESCALE4_V8M,	/**< refclock divided by 4 for the timestamp counter */
	ITM_TS_PRESCALE16_V8M,	/**< refclock divided by 16 for the timestamp counter */
	ITM_TS_PRESCALE64_V8M	/**< refclock divided by 64 for the timestamp counter */
};

struct armv8m_trace_config {
	/** Currently active trace capture mode */
	enum trace_config_type_v8m config_type;

	/** Currently active trace output mode */
	enum tpio_pin_protocol_v8m pin_protocol;
	/** TPIU formatter enable/disable (in async mode) */
	bool formatter;
	/** Synchronous output port width */
	uint32_t port_size;

	/** Bitmask of currenty enabled ITM stimuli */
	uint32_t itm_ter[8];
	/** Identifier for multi-source trace stream formatting */
	unsigned int trace_bus_id;
	/** Prescaler for the timestamp counter */
	enum itm_ts_prescaler_v8m itm_ts_prescale;
	/** Enable differential timestamps */
	bool itm_diff_timestamps;
	/** Enable async timestamps model */
	bool itm_async_timestamps;
	/** Enable synchronisation packet transmission (for sync port only) */
	bool itm_synchro_packets;

	/** Current frequency of TRACECLKIN (usually matches HCLK) */
	unsigned int traceclkin_freq;
	/** Current frequency of trace port */
	unsigned int trace_freq;
	/** Handle to output trace data in INTERNAL capture mode */
	FILE *trace_file;
};

extern const struct command_registration armv8m_trace_command_handlers[];

/**
 * Configure hardware accordingly to the current TPIU target settings
 */
int armv8m_trace_tpiu_config(struct target *target);
/**
 * Configure hardware accordingly to the current ITM target settings
 */
int armv8m_trace_itm_config(struct target *target);

#endif /* OPENOCD_TARGET_ARMV8M_TRACE_H */
