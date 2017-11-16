/*
 * =====================================================================================
 *
 *       Filename:  state.c
 *
 *    Description:  state lib
 *
 *        Version:  1.0
 *        Created:  2017/11/07 22时10分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yhao (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "state.h"
#include <string.h>

struct state
{
	uint64_t 			alive:1;
	uint64_t 			resum:1;

	st_id_t 			st;
	ev_id_t 			ev;

	struct state_map 	*map;
	void 				*ctx;
};

static inline
int state_check (struct state *state)
{
	if (!state) {
		return ST_ERR_PARAM;
	}
	if (!state->alive) {
		return ST_ERR_CLOSED;
	}
	if (state->st >= state->map->state_size) {
		return ST_ERR_RANGE;
	}
	return ST_OK;
}

int state_open (struct state *state, struct state_map *map, void *ctx)
{
	if (!state || !map || !map->state_size) {
		return ST_ERR_PARAM;
	}

	memset(state, 0, sizeof(struct state));

	state->map = map;
	state->ctx = ctx;

	state->alive = 1;

	return ST_OK;
}

int state_close (struct state *state)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}

	state->alive = 0;

	return ST_OK;
}

int state_drive (struct state *state, ev_id_t ev)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}

	state->ev = ev;
	do {
		state->resum = 0;
		if (state->map->caller[state->st](state, state->ev)) {
			ret = ST_ERR_CALLER;
			break;
		}
	} while (state->resum);
	
	return ret;
}

int state_resum (struct state *state)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}
	state->resum = 1;
	return ret;
}

int state_enter (struct state *state, st_id_t st)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}

	if (st >= state->map->state_size) {
		return ST_ERR_PARAM;
	}

	state->st = st;
	return ST_OK;
}

int state_enter_resum (struct state *state, st_id_t st)
{
	int ret = state_enter(state, st);
	if (ret != ST_OK) {
		return ret;
	}

	state->resum = 1;
	return ST_OK;
}

int state_emit (struct state *state, ev_id_t ev)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}

	state->ev = ev;
	state->resum = 1;

	return ST_OK;
}

int state_set_ctx (struct state *state, void *ctx)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}
	state->ctx = ctx;
	return ST_OK;
}

int state_get_ctx (struct state *state, void **ctx)
{
	int ret = state_check(state);
	if (ret != ST_OK) {
		return ret;
	}

	*ctx = state->ctx;
	return ST_OK;
}
