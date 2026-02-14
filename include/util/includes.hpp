#pragma once

#include <array>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <utility>

#define WIDTH  1280
#define HEIGHT 720

enum class FaceDir { PZ, NZ, PX, NX, PY, NY };

static inline int floorDiv(int a, int b) {
	int q = a / b;
	int r = a % b;
	if (r != 0 && ((r < 0) != (b < 0))) --q;
	return q;
}

static inline int floorMod(int a, int b) {
	int r = a % b;
	if (r < 0) r += b;
	return r;
}