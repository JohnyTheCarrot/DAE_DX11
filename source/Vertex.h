#ifndef VERTEX_H
#define VERTEX_H

namespace dae {
	struct Vertex final {
		Vector3 position{};
		static_assert(sizeof(Vector3) == sizeof(float) * 3);
		ColorRGB color{};
		static_assert(sizeof(Vector3) == sizeof(float) * 3);
	};
}// namespace dae

#endif
