#include "../../include/core/config_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ConfigReader::loadConfig(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    std::string section;

    while (std::getline(file, line))
    {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#')
            continue;

        // 处理节名
        if (line[0] == '[')
        {
            section = line.substr(1, line.find(']') - 1);
            continue;
        }

        // 处理键值对
        auto pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // 去除空白字符
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            configMap_[section + "." + key] = value;
        }
    }

    return true;
}

void ConfigReader::setSolverType(const std::string &type)
{
    solverType_ = type;
    useDirectData_ = true;
}

void ConfigReader::setTolerance(double tolerance)
{
    tolerance_ = tolerance;
    useDirectData_ = true;
}

void ConfigReader::setMaxIterations(int iterations)
{
    maxIterations_ = iterations;
    useDirectData_ = true;
}

void ConfigReader::setMatrixSize(int size)
{
    size_ = size;
    useDirectData_ = true;
}

void ConfigReader::setMatrixA(const std::vector<std::vector<double> > &A)
{
    A_ = A;
    useDirectData_ = true;
}

void ConfigReader::setVectorB(const std::vector<double> &b)
{
    b_ = b;
    useDirectData_ = true;
}

std::string ConfigReader::getSolverType() const
{
    return useDirectData_ ? solverType_ : configMap_.at("Solver.type");
}

double ConfigReader::getTolerance() const
{
    return useDirectData_ ? tolerance_ : std::stod(configMap_.at("Solver.tolerance"));
}

int ConfigReader::getMaxIterations() const
{
    return useDirectData_ ? maxIterations_ : std::stoi(configMap_.at("Solver.max_iterations"));
}

int ConfigReader::getMatrixSize() const
{
    return useDirectData_ ? size_ : std::stoi(configMap_.at("Matrix.size"));
}

std::vector<std::vector<double> > ConfigReader::getMatrixA() const
{
    if (useDirectData_)
        return A_;

    // 首先按分号分割每一行
    std::vector<std::string> rows;
    std::string matrixStr = configMap_.at("Matrix.A");
    std::stringstream rowStream(matrixStr);
    std::string rowStr;

    while (std::getline(rowStream, rowStr, ';'))
    {
        rows.push_back(rowStr);
    }

    int size = getMatrixSize();
    std::vector<std::vector<double> > A(size, std::vector<double>(size));

    // 添加调试信息
    std::cout << "读取到的矩阵大小: " << size << std::endl;
    std::cout << "读取到的矩阵行数: " << rows.size() << std::endl;

    // 处理每一行
    for (int i = 0; i < size; ++i)
    {
        // 按逗号分割每行的元素
        std::vector<double> rowElements = parseNumberList(rows[i], ',');

        std::cout << "第 " << i + 1 << " 行元素: ";
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = rowElements[j];
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return A;
}

std::vector<double> ConfigReader::getVectorB() const
{
    if (useDirectData_)
        return b_;
    return parseNumberList(configMap_.at("Matrix.b"), ',');
}

std::vector<double> ConfigReader::parseNumberList(const std::string &str, char delimiter) const
{
    std::vector<double> numbers;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        // 去除空白字符
        item.erase(0, item.find_first_not_of(" \t\r\n"));
        item.erase(item.find_last_not_of(" \t\r\n") + 1);

        if (!item.empty())
        {
            try
            {
                numbers.push_back(std::stod(item));
            }
            catch (const std::exception &e)
            {
                std::cerr << "解析数字失败: " << item << " (" << e.what() << ")" << std::endl;
            }
        }
    }

    return numbers;
}