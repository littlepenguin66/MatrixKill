#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include "../include/core/solver.h"
#include "../include/solvers/jacobi_solver.h"
#include "../include/solvers/gauss_solver.h"
#include "../include/core/config_reader.h"
#include "../include/utils/timer.h"
#include "../include/solvers/sor_solver.h"

void saveResults(const std::string &filename,
                 const std::vector<std::vector<double> > &A,
                 const std::vector<double> &b,
                 const std::vector<double> &x,
                 const std::string &solverType,
                 double tolerance,
                 int maxIterations,
                 int actualIterations,
                 double timeMs)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "无法创建结果文件: " << filename << std::endl;
        return;
    }

    // 设置输出精度
    file << std::fixed << std::setprecision(6);

    // 写入求解信息
    file << "求解方法: " << solverType << "\n";
    file << "矩阵规模: " << A.size() << "\n";
    file << "收敛精度: " << tolerance << "\n";
    file << "最大迭代次数: " << maxIterations << "\n";
    file << "实际迭代次数: " << actualIterations << "\n";
    file << "计算时间: " << timeMs << "ms\n\n";

    // 写入矩阵A
    file << "系数矩阵 A:\n";
    for (const auto &row : A)
    {
        for (double val : row)
        {
            file << std::setw(12) << val;
        }
        file << "\n";
    }

    // 写入向量b
    file << "\n常数向量 b:\n";
    for (double val : b)
    {
        file << std::setw(12) << val;
    }
    file << "\n";

    // 写入解向量x
    file << "\n解向量 x:\n";
    for (double val : x)
    {
        file << std::setw(12) << val;
    }
    file << "\n";

    // 计算并写入残差
    file << "\n残差向量 (Ax-b):\n";
    std::vector<double> residual(b.size());
    for (size_t i = 0; i < A.size(); ++i)
    {
        double sum = 0.0;
        for (size_t j = 0; j < A[i].size(); ++j)
        {
            sum += A[i][j] * x[j];
        }
        residual[i] = sum - b[i];
        file << std::setw(12) << residual[i];
    }
    file << "\n";

    // 计算残差范数
    double residualNorm = 0.0;
    for (double val : residual)
    {
        residualNorm += val * val;
    }
    residualNorm = std::sqrt(residualNorm);
    file << "\n残差范数: " << residualNorm << "\n";

    file.close();
}

void printUsage(const char *programName)
{
    std::cout << "用法: " << programName << " <配置文件路径> [选项]\n\n"
              << "参数:\n"
              << "  <配置文件路径>            必需，指定配置文件的路径\n\n"
              << "选项:\n"
              << "  -h, --help                显示帮助信息\n"
              << "  -o, --output <文件路径>    指定输出文件路径 (默认: result.txt)\n"
              << "  -s, --solver <求解器>      指定求解器类型 (默认: 使用配置文件中的设置)\n"
              << "                           可选值: jacobi, gauss, sor\n"
              << "  -t, --tolerance <精度>     设置求解精度 (默认: 使用配置文件中的设置)\n"
              << "  -m, --max-iter <次数>      设置最大迭代次数 (默认: 使用配置文件中的设置)\n"
              << "  -w, --omega <系数>         设置SOR松弛因子 (默认: 1.5, 仅用于SOR求解器)\n"
              << "  -q, --quiet               安静模式，减少输出信息\n"
              << "  -v, --verbose             详细模式，显示更多信息\n\n"
              << "示例:\n"
              << "  " << programName << " config.ini                    # 使用配置文件中的设置\n"
              << "  " << programName << " input.ini -s sor -w 1.2      # 使用SOR求解器，松弛因子为1.2\n"
              << "  " << programName << " data.ini -t 1e-8 -m 2000     # 设置精度和最大迭代次数\n"
              << std::endl;
}

struct ProgramOptions
{
    std::string configFile;
    std::string outputFile = "result.txt";
    std::string solverType;
    double tolerance = -1; // -1表示使用配置文件中的值
    int maxIterations = -1;
    double omega = 1.5;
    bool quiet = false;
    bool verbose = false;
};

ProgramOptions parseArguments(int argc, char *argv[])
{
    ProgramOptions options;

    if (argc < 2)
    {
        printUsage(argv[0]);
        exit(1);
    }

    // 第一个参数必须是配置文件路径（不是选项）
    if (argv[1][0] == '-')
    {
        std::cerr << "错误: 必须首先指定配置文件路径" << std::endl;
        printUsage(argv[0]);
        exit(1);
    }
    options.configFile = argv[1];

    // 解析其他选项
    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            printUsage(argv[0]);
            exit(0);
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (++i >= argc)
            {
                std::cerr << "错误: -o/--output 选项需要一个参数" << std::endl;
                exit(1);
            }
            options.outputFile = argv[i];
        }
        else if (arg == "-s" || arg == "--solver")
        {
            if (++i >= argc)
            {
                std::cerr << "错误: -s/--solver 选项需要一个参数" << std::endl;
                exit(1);
            }
            options.solverType = argv[i];
        }
        else if (arg == "-t" || arg == "--tolerance")
        {
            if (++i >= argc)
            {
                std::cerr << "错误: -t/--tolerance 选项需要一个参数" << std::endl;
                exit(1);
            }
            options.tolerance = std::stod(argv[i]);
        }
        else if (arg == "-m" || arg == "--max-iter")
        {
            if (++i >= argc)
            {
                std::cerr << "错误: -m/--max-iter 选项需要一个参数" << std::endl;
                exit(1);
            }
            options.maxIterations = std::stoi(argv[i]);
        }
        else if (arg == "-w" || arg == "--omega")
        {
            if (++i >= argc)
            {
                std::cerr << "错误: -w/--omega 选项需要一个参数" << std::endl;
                exit(1);
            }
            options.omega = std::stod(argv[i]);
        }
        else if (arg == "-q" || arg == "--quiet")
        {
            options.quiet = true;
        }
        else if (arg == "-v" || arg == "--verbose")
        {
            options.verbose = true;
        }
        else if (arg[0] == '-')
        {
            std::cerr << "错误: 未知选项 " << arg << std::endl;
            exit(1);
        }
        else
        {
            options.configFile = arg;
        }
    }

    if (options.configFile.empty())
    {
        std::cerr << "错误: 必须指定配置文件路径" << std::endl;
        exit(1);
    }

    return options;
}

int main(int argc, char *argv[])
{
    auto options = parseArguments(argc, argv);

    Timer totalTimer("总计算");

    ConfigReader config;
    if (!config.loadConfig(options.configFile))
    {
        std::cerr << "无法加载配置文件: " << options.configFile << std::endl;
        return 1;
    }

    // 创建求解器
    std::unique_ptr<Solver> solver;
    std::string solverType = options.solverType.empty() ? config.getSolverType() : options.solverType;

    if (options.verbose)
    {
        std::cout << "\n选择求解方法: " << solverType << std::endl;
    }

    if (solverType == "jacobi")
    {
        solver.reset(new JacobiSolver());
    }
    else if (solverType == "gauss")
    {
        solver.reset(new GaussSolver());
    }
    else if (solverType == "sor")
    {
        solver.reset(new SORSolver(options.omega));
    }
    else
    {
        std::cerr << "未知的求解器类型: " << solverType << std::endl;
        return 1;
    }

    // 获取矩阵和向量
    auto A = config.getMatrixA();
    auto b = config.getVectorB();

    // 设置参数
    double tolerance = options.tolerance > 0 ? options.tolerance : config.getTolerance();
    int maxIterations = options.maxIterations > 0 ? options.maxIterations : config.getMaxIterations();

    if (options.verbose)
    {
        std::cout << "求解精度: " << tolerance << std::endl;
        std::cout << "最大迭代次数: " << maxIterations << std::endl;
    }

    solver->setParameters(tolerance, maxIterations);
    solver->setEquation(A, b);

    // 检查矩阵可解性
    if (!solver->checkSolvability())
    {
        std::cerr << "矩阵不满足求解条件" << std::endl;
        return 1;
    }

    // 设置初始解向量
    std::vector<double> x(config.getMatrixSize(), 0.0);

    // 求解方程
    Timer solveTimer("求解");
    bool success = solver->solve(x);
    double solveTime = solveTimer.getElapsedMilliseconds();

    if (success)
    {
        // 保存结果到文件
        saveResults(options.outputFile, A, b, x, solverType,
                    tolerance, maxIterations, 0, solveTime);

        if (!options.quiet)
        {
            std::cout << "计算结果已保存到: " << options.outputFile << std::endl;
        }
    }
    else
    {
        std::cerr << "求解失败" << std::endl;
        return 1;
    }

    return 0;
}
