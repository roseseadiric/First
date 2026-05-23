#include "Quadrature.hpp"
#include <cmath>
#include <stdexcept>
// 生成三维单位球面上的均匀离散方向。
// angularN = N。
// 最终方向数 M = N * N。
// 每个方向包含：
// 1. direction = (omega_x, omega_y, omega_z)
// 2. weight    = 该方向的权重
std::vector<Ordinate> Quadrature::uniformSphere(int angularN) {
    if (angularN < 2) throw std::runtime_error("angularN 至少为 2");
    std::vector<Ordinate> ords;
    const double pi = std::acos(-1.0);

    // N 是角度网格数量,最终会生成 N 个 mu 层，每层 N 个 theta 方向。
    const int N = angularN;

    // 这里令所有方向权重相同，并且总权重和为 1,因为方向数 M = N * N，
    // 所以每个方向 weight = 1 / M。
    const double weight = 1.0 / static_cast<double>(N * N);


    //// 外层循环：沿 z 方向生成 N 个 mu 层,mu 表示方向向量的 z 分量：mu = omega_z
    // mu 的范围是 [-1, 1]
    for (int l = 1; l <= N; ++l) {
        // mu=z 分量，取 [-1,1] 中点网格，使三维上下方向都有覆盖。
        double mu = -1.0 + (2.0 * l - 1.0) / static_cast<double>(N);

        // r 是方向向量在 xy 平面上的投影长度：r = sqrt(1 - mu^2)
        // 因为单位方向满足：omega_x^2 + omega_y^2 + omega_z^2 = 1
        // 又 omega_z = mu，所以 omega_x^2 + omega_y^2 = 1 - mu^2。
        double r = std::sqrt(std::max(0.0, 1.0 - mu * mu));
        for (int k = 1; k <= N; ++k) {
            // 根据球坐标构造单位方向：omega_x = r cos(theta),omega_y = r sin(theta)
            // omega_z = mu
            // 然后把该方向和对应权重放入ords
            double theta = 2.0 * pi * (2.0 * k - 1.0) / (2.0 * N);
            ords.push_back({{r * std::cos(theta), r * std::sin(theta), mu}, weight});
        }
    }
    return ords;
}
