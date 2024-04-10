#include "utils.h"
#include <cstdlib>
#include <numeric>
#include <vector>
#include <sstream>
#include <cmath>
#include <tclap/CmdLine.h>

/******************************************************************************/
/*                                   timers                                   */
/******************************************************************************/

double avgTimers(std::vector<long> const &timers) {
    return (double) std::accumulate(timers.cbegin(), timers.cend(), (long) 0) / (double) timers.size();
}

double stdevTimers(std::vector<long> const &timers) {
    double avg = avgTimers(timers),
                 temp = 0;

    for (auto const &timer : timers) {
        temp += (((double) timer - avg) * ((double) timer - avg));
    }
    return std::sqrt(temp / (double) timers.size());
}

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

std::pair<size_t, size_t> parseSize(std::string const& sizeStr) {
    std::pair<size_t, size_t> size;
    char separator;
    std::istringstream iss(sizeStr);
    iss >> size.first;
    iss >> separator;
    iss >> size.second;
    return size;
}

void parseCmdArgs(int argc, char **argv, Config &config) {
    // Parse the command line arguments
    try {
        TCLAP::CmdLine cmd("Game of life parameters", ' ', "0.1");
        SizeConstraint sc;
        TCLAP::ValueArg<std::string> sizeArg("s", "size", "Grid size (ex: 10x10).", false, "10x10", "string");
        cmd.add(sizeArg);
        TCLAP::ValueArg<size_t> blockSizeArg("b", "block", "Block Size.", false, 3, &sc);
        cmd.add(blockSizeArg);
        TCLAP::ValueArg<size_t> nbThreadsArg("t", "threads", "Number of threads.", false, 4, &sc);
        cmd.add(nbThreadsArg);
        TCLAP::ValueArg<size_t> nbEpocsArg("e", "epocs", "Number of epocs.", false, 10, &sc);
        cmd.add(nbEpocsArg);
        TCLAP::ValueArg<bool> animateArg("a", "animate", "Animate the simulation (no perf measurement)", false, false, "bool");
        cmd.add(animateArg);
        TCLAP::ValueArg<bool> printArg("p", "print", "Print the starting and final grid.", false, false, "bool");
        cmd.add(printArg);
        TCLAP::ValueArg<bool> graphArg("g", "graph", "Generate the dot file at the end of the first epoch.", false, false, "bool");
        cmd.add(graphArg);
        cmd.parse(argc, argv);

        auto size = parseSize(sizeArg.getValue());
        config.gridWidth = size.first;
        config.gridHeight = size.second;
        config.blockSize = blockSizeArg.getValue();
        config.nbThreads = nbThreadsArg.getValue();
        config.nbEpocs = nbEpocsArg.getValue();
        config.animation = animateArg.getValue();
        config.printGrid = printArg.getValue();
    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}
