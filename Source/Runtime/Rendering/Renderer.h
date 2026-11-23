#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <Core/Core.h>
#include <Core/ArenaAllocator.h>

	class Renderer {
	public:
		Renderer(int w, int h)
			: width_(w), height_(h), color_(size_t(w)* size_t(h)), depth_(size_t(w)* size_t(h)),
			scratch_(1u << 20) {}

		void Clear(uint32_t argb);

		void DrawMesh(const Mesh& mesh, const Matrix4x4& model, 
			const Matrix4x4& view, const Matrix4x4& proj, Vector3 lightDir);

		const uint32_t* Pixels() const { return color_.data(); }

	private:
		static uint32_t Pack(Vector3 c);

		void FillTriangle(const ScreenVertex& a, const ScreenVertex& b, const ScreenVertex& c, uint32_t argb);

		int width_, height_;
		std::vector<uint32_t> color_;
		std::vector<float> depth_;
		bluejay::memory::Arena scratch_;
	};

#endif RENDERER_H
