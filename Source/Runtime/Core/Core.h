#pragma once
#ifndef CORE_H
#define CORE_H

#include <algorithm>
#include <cmath>
#include <vector>

struct Vector2 {
	float x = 0;
	float y = 0;
};

inline Vector2 operator+(Vector2 a, Vector2 b) { return {a.x + b.x, a.y + b.y}; }
inline Vector2 operator-(Vector2 a, Vector2 b) { return {a.x - b.x, a.y - b.y}; }
inline Vector2 operator*(Vector2 a, Vector2 b) { return {a.x * b.x, a.y * b.y}; }
inline Vector2 operator/(Vector2 a, Vector2 b) { return {a.x / b.x, a.y / b.y}; }

struct Vector3 {
	float x = 0;
	float y = 0;
	float z = 0;
};

inline Vector3 operator+(Vector3 a, Vector3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline Vector3 operator+(Vector3 a, float b) { return {a.x + b, a.y + b, a.z + b}; }
inline Vector3 operator-(Vector3 a, Vector3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline Vector3 operator-(Vector3 a, float b) { return {a.x - b, a.y - b, a.z - b}; }
inline Vector3 operator*(Vector3 a, float s) { return{a.x * s, a.y * s, a.z * s}; }
inline Vector3 operator*(Vector3 a, Vector3 b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
inline Vector3 operator/(Vector3 a, Vector3 b) { return {a.x / b.x, a.y / b.y, a.z / b.z}; }
inline Vector3 operator/(Vector3 a, float b) { return {a.x / b, a.y / b, a.z / b}; }

inline float DotProduct(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vector3 CrossProduct(Vector3 a, Vector3 b) {
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}
inline Vector3 Normalize(Vector3 v) {
	const float len = std::sqrt(DotProduct(v, v));
	return len > 0 ? v * (1.0f / len) : v;
}

/// Matrix
// Row-major 4x4, column-vector convention: p' = M * p.
struct Matrix4x4 {
	float m[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
};

inline Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			r.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) r.m[i][j] += a.m[i][k] * b.m[k][j];
		}
	return r;
}
inline Matrix4x4 Translation(Vector3 t) {
	Matrix4x4 r;
	r.m[0][3] = t.x; r.m[1][3] = t.y; r.m[2][3] = t.z;
	return r;
}
inline Matrix4x4 RotationX(float a) {
	Matrix4x4 r;
	r.m[1][1] = std::cos(a); r.m[1][2] = -std::sin(a);
	r.m[2][1] = std::sin(a); r.m[2][2] = std::cos(a);
	return r;
}
inline Matrix4x4 RotationY(float a) {
	Matrix4x4 r;
	r.m[0][0] = std::cos(a);  r.m[0][2] = std::sin(a);
	r.m[2][0] = -std::sin(a); r.m[2][2] = std::cos(a);
	return r;
}
// Right-handed, camera looks down -Z, clip depth in [0, 1].
inline Matrix4x4 Perspective(float fovY, float aspect, float zn, float zf) {
	const float f = 1.0f / std::tan(fovY * 0.5f);
	Matrix4x4 r;
	r.m[0][0] = f / aspect;
	r.m[1][1] = f;
	r.m[2][2] = zf / (zn - zf);
	r.m[2][3] = zn * zf / (zn - zf);
	r.m[3][2] = -1.0f;
	r.m[3][3] = 0.0f;
	return r;
}

inline Vector3 TransformPoint(const Matrix4x4& M, Vector3 p, float* wOut = nullptr) {
	const float x = M.m[0][0] * p.x + M.m[0][1] * p.y + M.m[0][2] * p.z + M.m[0][3];
	const float y = M.m[1][0] * p.x + M.m[1][1] * p.y + M.m[1][2] * p.z + M.m[1][3];
	const float z = M.m[2][0] * p.x + M.m[2][1] * p.y + M.m[2][2] * p.z + M.m[2][3];
	const float w = M.m[3][0] * p.x + M.m[3][1] * p.y + M.m[3][2] * p.z + M.m[3][3];
	if (wOut) *wOut = w;
	return { x, y, z };
}

/// Pixels

struct Triangle {
	int a, b, c;
	Vector3 color;
};

struct ScreenVertex {
	float x, y; // Pixels
	float z; //NDC Depth 0..1
};

/// Meshes

struct Mesh {
	std::vector<Vector3> vertices;
	std::vector<Triangle> tris; // Counter - Clockwise when viewed from outside
};

/// Colors
struct Color4 { // 0-255
	unsigned int r, g, b, a;
};
inline Color4 operator+(const Color4 a, const Color4 b) {
	auto clamp = [](unsigned int a) {
		
		if (a > 255) a = 255;
		return a;
	};
	return { clamp(a.r + b.r), clamp(a.g + b.g), clamp(a.b + b.b), clamp(a.a + b.a) };
}

/// Primitives 

inline Mesh MakeCube() {
	Mesh m;
	m.vertices = { { -1, -1, -1 },{ 1, -1, -1 },{ 1, 1, -1 },{ -1, 1, -1 },
				  { -1, -1, 1 },{ 1, -1, 1 },{ 1, 1, 1 },{ -1, 1, 1 } };
	const Vector3 red{ 0.9f, 0.3f, 0.3f }, green{ 0.3f, 0.85f, 0.4f }, blue{ 0.3f, 0.5f, 0.95f },
		yellow{ 0.95f, 0.85f, 0.3f }, magenta{ 0.85f, 0.4f, 0.9f }, cyan{ 0.3f, 0.85f, 0.9f };
	m.tris = { { 4, 5, 6, red },{ 4, 6, 7, red },      // +Z
			  { 1, 0, 3, green },{ 1, 3, 2, green },    // -Z
			  { 5, 1, 2, blue },{ 5, 2, 6, blue },     // +X
			  { 0, 4, 7, yellow },{ 0, 7, 3, yellow },   // -X
			  { 7, 6, 2, magenta },{ 7, 2, 3, magenta },  // +Y
			  { 0, 1, 5, cyan },{ 0, 5, 4, cyan } };    // -Y
	return m;
}
inline Mesh MakeCube(
	const Vector3 zp,
	const Vector3 zm,
	const Vector3 xp,
	const Vector3 xm,
	const Vector3 yp,
	const Vector3 ym
) {
	Mesh m;
	m.vertices = { { -1, -1, -1 },{ 1, -1, -1 },{ 1, 1, -1 },{ -1, 1, -1 },
				  { -1, -1, 1 },{ 1, -1, 1 },{ 1, 1, 1 },{ -1, 1, 1 } };
	m.tris = { { 4, 5, 6, zp },{ 4, 6, 7, zp },      // +Z
			  { 1, 0, 3, zm },{ 1, 3, 2, zm },    // -Z
			  { 5, 1, 2, xp },{ 5, 2, 6, xp },     // +X
			  { 0, 4, 7, xm },{ 0, 7, 3, xm },   // -X
			  { 7, 6, 2, yp },{ 7, 2, 3, yp },  // +Y
			  { 0, 1, 5, ym },{ 0, 5, 4, ym } };    // -Y
	return m;
}

inline Mesh MakePyramid(
	const Vector3 zp,
	const Vector3 zm,
	const Vector3 xp,
	const Vector3 xm,
	const Vector3 ym
) {
	Mesh m;
	m.vertices = { { -1, -1, -1 },{ 1, -1, -1 },{ 1, -1, 1 },{ -1, -1, 1 },{ 0, 1.2f, 0 } };
	m.tris = { { 0, 1, 2, ym },{ 0, 2, 3, ym },  // base (-Y)
			  { 0, 4, 1, zp },{ 1, 4, 2, zm },{ 2, 4, 3, xp },{ 3, 4, 0, xm } };
	return m;
}
inline Mesh MakePyramid() {
	Mesh m;
	m.vertices = { { -1, -1, -1 },{ 1, -1, -1 },{ 1, -1, 1 },{ -1, -1, 1 },{ 0, 1.2f, 0 } };
	const Vector3 orange{ 0.95f, 0.55f, 0.2f }, white{ 0.9f, 0.9f, 0.9f };
	m.tris = { { 0, 1, 2, white },{ 0, 2, 3, white },  // base (-Y)
			  { 0, 4, 1, orange },{ 1, 4, 2, orange },{ 2, 4, 3, orange },{ 3, 4, 0, orange } };
	return m;
}

#endif