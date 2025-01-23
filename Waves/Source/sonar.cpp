
#include "AEEngine.h"
#include "sonar.hpp"

namespace sonar {

    AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments) {
        // Start creating the mesh
        AEGfxMeshStart();

        // Loop through the segments to define vertices
        for (int i = 0; i < numSegments; ++i)
        {
            // Calculate angles for the current and next segments

            f32 theta1 = (2.0f * PI * i) / numSegments;
            f32 theta2 = (2.0f * PI * (i + 1)) / numSegments;

            // Outer ring vertices
            f32 x1_outer = outerRadius * cosf(theta1);
            f32 y1_outer = outerRadius * sinf(theta1);
            f32 x2_outer = outerRadius * cosf(theta2);
            f32 y2_outer = outerRadius * sinf(theta2);

            // Inner ring vertices
            f32 x1_inner = innerRadius * cosf(theta1);
            f32 y1_inner = innerRadius * sinf(theta1);
            f32 x2_inner = innerRadius * cosf(theta2);
            f32 y2_inner = innerRadius * sinf(theta2);

            // Add two triangles to form a quad between the inner and outer rings
            AEGfxTriAdd(
                x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
                x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f,  // Outer vertex 2
                x1_outer, y1_outer, 0xFFFFFFFF, 0.0f, 0.0f   // Outer vertex 1
            );

            AEGfxTriAdd(
                x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
                x2_inner, y2_inner, 0xFF888888, 1.0f, 1.0f,  // Inner vertex 2
                x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f   // Outer vertex 2
            );
        }

        // End the mesh and return it
        return AEGfxMeshEnd();
    }

    void CircleSonar(struct CircleSonar_c& wave, AEGfxVertexList* mesh, int& active) {

        // update logic
        if (wave.currentSize >= wave.maxSize) {
            active = 0;

            wave.currentSize = 0;
            wave.speed = 50.f;
        }

        f32 dt = (f32)AEFrameRateControllerGetFrameTime();
        AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 }; AEMtx33 translate = { 0 };

        AEMtx33Trans(&translate, wave.posx, wave.posy);
        AEMtx33Scale(&scale, wave.currentSize, wave.currentSize);
        AEMtx33Concat(&wave.transform, &translate, &scale);

        AEGfxSetTransform(wave.transform.m);

        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

        wave.currentSize += wave.speed * dt;
        wave.speed *= 1.05f;

    }

}