#pragma once
#include <string>
#include <vector>
#include <map>

class ConfigReader
{
public:
    ConfigReader() : tolerance_(1e-6), maxIterations_(1000), size_(0) {}
    bool loadConfig(const std::string &filename);

    // 添加手动设置方法
    void setSolverType(const std::string &type);
    void setTolerance(double tolerance);
    void setMaxIterations(int iterations);
    void setMatrixSize(int size);
    void setMatrixA(const std::vector<std::vector<double> > &A);
    void setVectorB(const std::vector<double> &b);

    // 获取求解器配置
    std::string getSolverType() const;
    double getTolerance() const;
    int getMaxIterations() const;

    // 获取矩阵配置
    int getMatrixSize() const;
    std::vector<std::vector<double> > getMatrixA() const;
    std::vector<double> getVectorB() const;

private:
    std::vector<double> parseNumberList(const std::string &str, char delimiter) const;
    std::map<std::string, std::string> configMap_;

    // 添加直接存储数据的成员
    std::string solverType_;
    double tolerance_;
    int maxIterations_;
    int size_;
    std::vector<std::vector<double> > A_;
    std::vector<double> b_;
    bool useDirectData_ = false;
};