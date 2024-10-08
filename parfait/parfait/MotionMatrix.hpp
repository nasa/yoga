
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space
// Administration. No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
//
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
#pragma once
#include "MotionMatrix.h"

namespace Parfait {
inline MotionMatrix::MotionMatrix() { clearMotions(); }

inline MotionMatrix::MotionMatrix(double dx, double dy, double dz) {
    double translation[3];
    translation[0] = dx;
    translation[1] = dy;
    translation[2] = dz;
    setTranslation(translation);
}

inline MotionMatrix::MotionMatrix(const double translation[3]) { setTranslation(translation); }

inline MotionMatrix::MotionMatrix(const double line_start[3], const double line_end[3], double rotation_angle) {
    setRotation(line_start, line_end, rotation_angle);
}

inline void MotionMatrix::clearMotions() {
    // sets the motion matrix to the identity matrix
    for (int i = 0; i < 16; i++) mat[i] = 0.0;
    for (int i = 0; i < 4; i++) mat[4 * i + i] = 1.0;
}

inline void MotionMatrix::addTranslation(const double translation[3]) {
    MotionMatrix tran;
    tran.setTranslation(translation);
    // multiply matrices
    double A[16], result[16];
    tran.getMatrix(A);
    MatrixMatrixMultiply(A, mat, 4, 4, 4, 4, result);
    setMotionMatrix(result);
}

inline void MotionMatrix::addRotation(const double line_start[3], const double line_end[3], double angle) {
    MotionMatrix rot;
    rot.setRotation(line_start, line_end, angle);
    // multiply matrices
    double A[16], result[16];
    rot.getMatrix(A);
    MatrixMatrixMultiply(A, mat, 4, 4, 4, 4, result);
    setMotionMatrix(result);
}

inline void MotionMatrix::addMotion(const MotionMatrix& motion) {
    double A[16], result[16];
    motion.getMatrix(A);
    MatrixMatrixMultiply(A, mat, 4, 4, 4, 4, result);
    setMotionMatrix(result);
}

inline void MotionMatrix::setTranslation(const double translation[3]) {
    clearMotions();
    mat[3] = translation[0];
    mat[7] = translation[1];
    mat[11] = translation[2];
}

inline void MotionMatrix::setRotation(const double line_start[3], const double line_end[3], double angle) {
    double a, b, c, u, v, w, L, sin_theta, cos_theta, theta, pi;
    a = line_start[0];
    b = line_start[1];
    c = line_start[2];
    u = line_end[0] - a;
    v = line_end[1] - b;
    w = line_end[2] - c;
    L = u * u + v * v + w * w;

    pi = acos(-1.0);
    theta = angle * pi / 180.0;
    sin_theta = sin(theta);
    cos_theta = cos(theta);

    mat[0] = (u * u + (v * v + w * w) * cos_theta) / L;
    mat[1] = (u * v * (1.0 - cos_theta) - w * sqrt(L) * sin_theta) / L;
    mat[2] = (u * w * (1.0 - cos_theta) + v * sqrt(L) * sin_theta) / L;
    mat[3] =
        ((a * (v * v + w * w) - u * (b * v + c * w)) * (1.0 - cos_theta) + (b * w - c * v) * sqrt(L) * sin_theta) / L;
    mat[4] = (u * v * (1.0 - cos_theta) + w * sqrt(L) * sin_theta) / L;
    mat[5] = (v * v + (u * u + w * w) * cos_theta) / L;
    mat[6] = (v * w * (1.0 - cos_theta) - u * sqrt(L) * sin_theta) / L;
    mat[7] =
        ((b * (u * u + w * w) - v * (a * u + c * w)) * (1.0 - cos_theta) + (c * u - a * w) * sqrt(L) * sin_theta) / L;
    mat[8] = (u * w * (1.0 - cos_theta) - v * sqrt(L) * sin_theta) / L;
    mat[9] = (v * w * (1.0 - cos_theta) + u * sqrt(L) * sin_theta) / L;
    mat[10] = (w * w + (u * u + v * v) * cos_theta) / L;
    mat[11] =
        ((c * (u * u + v * v) - w * (a * u + b * v)) * (1.0 - cos_theta) + (a * v - b * u) * sqrt(L) * sin_theta) / L;
    mat[12] = 0.0;
    mat[13] = 0.0;
    mat[14] = 0.0;
    mat[15] = 1.0;
}

inline void MotionMatrix::setMotionMatrix(const double matrix[16]) {
    for (int i = 0; i < 16; i++) mat[i] = matrix[i];
}

inline void MotionMatrix::getMatrix(double matrix[16]) const {
    for (int i = 0; i < 16; i++) matrix[i] = mat[i];
}

inline void MotionMatrix::movePoint(double p[3]) const {
    double vec_in[4], vec_out[4];
    vec_in[0] = p[0];
    vec_in[1] = p[1];
    vec_in[2] = p[2];
    vec_in[3] = 1.0;
    MatrixVectorMultiply(mat, vec_in, vec_out, 4, 4);
    p[0] = vec_out[0];
    p[1] = vec_out[1];
    p[2] = vec_out[2];
}

inline std::string MotionMatrix::toString() const {
    std::string s;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double val = mat[4 * i + j];
            char buffer[50];
            snprintf(buffer, 50, "%.16e", val);
            s.append(std::string(buffer) + " ");
        }
        s.append("\n");
    }
    return s;
}
inline void MotionMatrix::setRotation(const Point<double>& a, const Point<double>& b, double angle) {
    setRotation(a.data(), b.data(), angle);
}
inline void MotionMatrix::addRotation(const Point<double>& a, const Point<double>& b, double angle) {
    addRotation(a.data(), b.data(), angle);
}
inline void MotionMatrix::movePoint(Parfait::Point<double>& p) const { movePoint(p.data()); }
}
