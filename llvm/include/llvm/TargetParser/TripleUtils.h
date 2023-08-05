
#ifndef LLVM_TARGETPARSER_TRIPLE_UTILS_H
#define LLVM_TARGETPARSER_TRIPLE_UTILS_H

#include "llvm/TargetParser/Triple.h"
#include <string>

namespace llvm {

class TripleUtils {
public:

    /// Test whether the architecture is 64-bit
    ///
    /// Note that this tests for 64-bit pointer width, and nothing else. Note
    /// that we intentionally expose only three predicates, 64-bit, 32-bit, and
    /// 16-bit. The inner details of pointer width for particular architectures
    /// is not summed up in the triple, and so only a coarse grained predicate
    /// system is provided.

    static bool isArch64Bit(const llvm::Triple& tripleImpl);

    /// Test whether the architecture is 32-bit
    ///
    /// Note that this tests for 32-bit pointer width, and nothing else.

    static bool isArch32Bit(const llvm::Triple& tripleImpl);

    /// Test whether the architecture is 16-bit
    ///
    /// Note that this tests for 16-bit pointer width, and nothing else.

    static bool isArch16Bit(const llvm::Triple& tripleImpl);

    static bool isAndroidVersionLT(const llvm::Triple& tripleIpml, unsigned Major);

    /// Helper function for doing comparisons against version numbers included in
    /// the target triple.

    static bool isOSVersionLT(const llvm::Triple& tripleImpl, unsigned Major, unsigned Minor = 0, unsigned Micro = 0);

    static bool isOSVersionLT(const llvm::Triple& tripleImpl, const llvm::Triple& Other);

    /// Comparison function for checking OS X version compatibility, which handles
    /// supporting skewed version numbering schemes used by the "darwin" triples.

    static bool isMacOSXVersionLT(const llvm::Triple& tripleImpl, unsigned Major, unsigned Minor = 0, unsigned Micro = 0); 
    
    /// Is this a Mac OS X triple. For legacy reasons, we support both "darwin"
    /// and "osx" as OS X triples.
    static bool isMacOSX(const llvm::Triple& tripleImpl);

    /// Merge target triples.
    static std::string merge(const llvm::Triple& tripleImpl,const llvm::Triple &Other);

    /// Is this an Apple tvOS triple.
    static bool isTvOS(const llvm::Triple& tripleImpl);

    /// Is this an iOS triple.
    /// Note: This identifies tvOS as a variant of iOS. If that ever
    /// changes, i.e., if the two operating systems diverge or their version
    /// numbers get out of sync, that will need to be changed.
    /// watchOS has completely different version numbers so it is not included.
    static bool isiOS(const llvm::Triple& tripleImpl);

    /// Is this an Apple watchOS triple.
    static bool isWatchOS(const llvm::Triple& tripleImpl);

    static bool isWatchABI(const llvm::Triple& tripleImpl);

    /// Is this an Apple DriverKit triple.
    static bool isDriverKit(const llvm::Triple& tripleImpl);

    static bool isOSzOS(const llvm::Triple& tripleImpl);

    /// Is this a "Darwin" OS (macOS, iOS, tvOS, watchOS, or DriverKit).
    static bool isOSDarwin(const llvm::Triple& tripleImpl);

    static bool isSimulatorEnvironment(const llvm::Triple& tripleImpl);

    static bool isMacCatalystEnvironment(const llvm::Triple& tripleImpl);

    /// Returns true for targets that run on a macOS machine.
    static bool isTargetMachineMac(const llvm::Triple& tripleImpl);
    
};

} // namespace llvm

#endif
