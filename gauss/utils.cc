#include "utils.h"
#include <tclap/CmdLine.h>

/******************************************************************************/
/*                          parse cmd line arguments                          */
/******************************************************************************/

class SizeConstraint : public TCLAP::Constraint<size_t> {
 public:
  [[nodiscard]] std::string description() const override {
    return "Positive non null";
  }
  [[nodiscard]] std::string shortID() const override {
    return "NonNullInteger";
  }
  [[nodiscard]] bool check(size_t const &value) const override {
    return value > 0;
  }
};

void parseCmdArgs(int argc, char **argv, Config &config) {
    // Parse the command line arguments
    try {
        TCLAP::CmdLine cmd("Game of life parameters", ' ', "0.1");
        SizeConstraint sc;
        TCLAP::ValueArg<size_t> sizeArg("s", "size", "Matrix size.", false, 10, &sc);
        cmd.add(sizeArg);
        TCLAP::ValueArg<size_t> nbThreadsArg("t", "threads", "Number of threads.", false, 4, &sc);
        cmd.add(nbThreadsArg);
        TCLAP::ValueArg<bool> printMatrixArg("p", "print", "Print the random matrix.", false, false, "bool");
        cmd.add(printMatrixArg);
        TCLAP::ValueArg<bool> printResultArg("r", "result", "Print the result.", false, false, "bool");
        cmd.add(printResultArg);
        TCLAP::ValueArg<std::string> dotDirArg("d", "dotdir", "Directory in which will be generated the dot files.", false, "gaussImg/", "string");
        cmd.add(dotDirArg);
        cmd.parse(argc, argv);

        config.matrixSize = sizeArg.getValue();
        config.nbThreads = nbThreadsArg.getValue();
        config.printMatrix = printMatrixArg.getValue();
        config.printResult = printResultArg.getValue();
        config.dotDirectory = dotDirArg.getValue();
    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}
