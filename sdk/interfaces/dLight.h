#pragma once
#include "../misc/Color.hpp"
#include "../math/Vector.hpp"

enum dlight_flags {
	dlight_no_world_illumination = 0x1,
	dlight_no_model_illumination = 0x2,
	dlight_add_displacement_alpha = 0x4,
	dlight_subtract_displacement_alpha = 0x8,
	dlight_displacement_mask = (dlight_add_displacement_alpha | dlight_subtract_displacement_alpha),
};

struct dlight_t {
	int flags;
	Vector origin;
	float radius;
	Color color;
	float die_time;
	float decay;
	float min_light;
	int	key;
	int	style;
	Vector direction;
	float inner_angle;
	float outer_angle;
};


class iv_effects
{
public:
	dlight_t* cl_alloc_dlight(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[4](this, key);
	}
	dlight_t* cl_alloc_elight(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[5](this, key);
	}
	dlight_t* get_elight_by_key(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[8](this, key);
	}
};