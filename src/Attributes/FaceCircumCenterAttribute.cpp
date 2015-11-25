/* This file is part of PyMesh. Copyright (c) 2015 by Qingnan Zhou */
#include "FaceCircumCenterAttribute.h"

#include <Mesh.h>
#include <Core/Exception.h>
#include <iostream>

void FaceCircumCenterAttribute::compute_from_mesh(Mesh& mesh) {
    const size_t dim = mesh.get_dim();
    const size_t num_faces = mesh.get_num_faces();
    const size_t vertex_per_face = mesh.get_vertex_per_face();

    if (vertex_per_face != 3) {
        throw RuntimeError("Circumcenter is only defined for triangle faces.");
    }

    VectorF& circum_centers = m_values;
    circum_centers.resize(num_faces * dim);

    const VectorF& vertices = mesh.get_vertices();

    for (size_t i=0; i<num_faces; i++) {
        VectorI face = mesh.get_face(i);

        VectorF v0 = vertices.segment(face[0]*dim, dim);
        VectorF v1 = vertices.segment(face[1]*dim, dim);
        VectorF v2 = vertices.segment(face[2]*dim, dim);

        Vector3F e0 = Vector3F::Zero();
        Vector3F e1 = Vector3F::Zero();
        Vector3F e2 = Vector3F::Zero();

        e0.segment(0,dim) = v2 - v1;
        e1.segment(0,dim) = v0 - v2;
        e2.segment(0,dim) = v1 - v0;

        Float sq_l0 = e0.squaredNorm();
        Float sq_l1 = e1.squaredNorm();
        Float sq_l2 = e2.squaredNorm();

        Vector3F coeff(
                sq_l0 * (sq_l1 + sq_l2 - sq_l0),
                sq_l1 * (sq_l0 + sq_l2 - sq_l1),
                sq_l2 * (sq_l0 + sq_l1 - sq_l2));
        coeff /= coeff.sum();

        circum_centers.segment(i*dim, dim) =
            v0 * coeff[0] +
            v1 * coeff[1] +
            v2 * coeff[2];
    }
}
