#include "AEEngine.h"
#include<iostream>

// Function to create a donut mesh with an empty center
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color)
{
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
            x1_inner, y1_inner, color, 0.0f, 1.0f,  // Inner vertex 1
            x2_outer, y2_outer, color, 1.0f, 0.0f,  // Outer vertex 2
            x1_outer, y1_outer, color, 0.0f, 0.0f   // Outer vertex 1
        );

        AEGfxTriAdd(
            x1_inner, y1_inner, color, 0.0f, 1.0f,  // Inner vertex 1
            x2_inner, y2_inner, color, 1.0f, 1.0f,  // Inner vertex 2
            x2_outer, y2_outer, color, 1.0f, 0.0f   // Outer vertex 2
        );
    }

    // End the mesh and return it
    return AEGfxMeshEnd();
}

void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation)
{
    // Create transform matrices
    AEMtx33 scaleDonut = { 0 };
    AEMtx33 rotateDonut = { 0 };
    AEMtx33 translateDonut = { 0 };

    // Apply scaling, rotation, and translation
    AEMtx33Scale(&scaleDonut, scaleX, scaleY);           // Scaling
    AEMtx33Rot(&rotateDonut, rotation);                  // Rotation
    AEMtx33Trans(&translateDonut, posX, posY);           // Translation

    // Concatenate the matrices in the correct order (scale, then rotate, then translate)
    AEMtx33 transformDonut = { 0 };
    AEMtx33Concat(&transformDonut, &rotateDonut, &scaleDonut);
    AEMtx33Concat(&transformDonut, &translateDonut, &transformDonut);

    // Apply the transform and draw the mesh
    AEGfxSetTransform(transformDonut.m);
    AEGfxMeshDraw(NameOfMesh, AE_GFX_MDM_TRIANGLES);
}
