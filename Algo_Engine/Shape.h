#pragma once
#include "MathGeoLib/include/Math/float3.h"
#include "Globals.h"

void CreateCube(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
void CreateSphere(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint rings = 24, uint sectors = 24);
void CreateCilinder(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint rings = 12, uint sectors = 12);
void CreateCone(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint sectors = 12);
void CreatePlane(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position = { 0,0,0 }, float3 size = { 1,1,1 });