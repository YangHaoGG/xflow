/*
 * =====================================================================================
 *
 *       Filename:  state.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017/11/07 22时12分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yhao (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __STATE_H__
#define __STATE_H__

#include <stdint.h>

struct state;

typedef uint64_t st_id_t;
typedef uint64_t ev_id_t;
typedef int (*state_caller)(struct state *state, ev_id_t ev);

struct state_map
{
	uint64_t 		state_size;
	state_caller 	caller[0];
};

enum state_error
{
	ST_OK = 0,
	ST_ERR_PARAM,
	ST_ERR_CLOSED,
	ST_ERR_RANGE,
	ST_ERR_CALLER,
	ST_ERR_MAX
};

int state_open (struct state *state, struct state_map *map, void *ctx);

int state_close (struct state *state);

int state_drive (struct state *state, ev_id_t ev);

int state_resum (struct state *state);

int state_enter (struct state *state, st_id_t st);

int state_enter_resum (struct state *state, st_id_t st);

int state_emit (struct state *state, ev_id_t ev);

int state_emit_resum (struct state *state, ev_id_t ev);

int state_set_ctx (struct state *state, void *ctx);

int state_get_ctx (struct state *state, void **ctx);

#endif
