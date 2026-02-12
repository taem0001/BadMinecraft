#pragma once

#include "gfx.hpp"
#include "../util/includes.hpp"

namespace Minecraft {
	namespace GFX {
		class Shader {
			public:
				~Shader();
				void init(const char *, const char *);
				void use();
				void setBool(const char *, bool) const;
				void setInt(const char *, int) const;
				void setFloat(const char *, float) const;
				void setMat4(const char *, glm::mat4) const;

			private:
				GLuint handle;
		};
	} // namespace GFX
} // namespace Minecraft
