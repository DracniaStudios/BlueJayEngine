#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <Core/Core.h>
#include <Core/ArenaAllocator.h>

	class Renderer {
	public:
		Renderer(int w, int h)
			: width(w),
			height(h),
			color(size_t(w)* size_t(h)),
			depth(size_t(w)* size_t(h)),
			pixelAllocator(1u << 20) {}

		void Clear(uint32_t argb);

		void DrawMesh(const Mesh& mesh, const Matrix4x4& model, 
			const Matrix4x4& view, const Matrix4x4& proj, Vector3 lightDir);

		const uint32_t* Pixels() const { return color.data(); }

	private:
		static uint32_t Pack(Vector3 c);

		void FillTriangle(const ScreenVertex& a, const ScreenVertex& b, const ScreenVertex& c, uint32_t argb);

		int width, height;
		std::vector<uint32_t> color; // The Frame Buffer (list of color for each pixel on screen)
		std::vector<float> depth;
		bluejay::memory::Arena pixelAllocator;
	};

#endif RENDERER_H
