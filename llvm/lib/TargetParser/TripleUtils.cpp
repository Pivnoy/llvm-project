
#include "llvm/TargetParser/TripleUtils.h"
#include <cassert>
using namespace llvm;

static unsigned getArchPointerBitWidth(llvm::Triple::ArchType Arch) {
    switch (Arch) {
        case llvm::Triple::UnknownArch:
            return 0;

        case llvm::Triple::avr:
        case llvm::Triple::msp430:
            return 16;

        case llvm::Triple::aarch64_32:
        case llvm::Triple::amdil:
        case llvm::Triple::arc:
        case llvm::Triple::arm:
        case llvm::Triple::armeb:
        case llvm::Triple::csky:
        case llvm::Triple::dxil:
        case llvm::Triple::hexagon:
        case llvm::Triple::hsail:
        case llvm::Triple::kalimba:
        case llvm::Triple::lanai:
        case llvm::Triple::le32:
        case llvm::Triple::loongarch32:
        case llvm::Triple::m68k:
        case llvm::Triple::mips:
        case llvm::Triple::mipsel:
        case llvm::Triple::nvptx:
        case llvm::Triple::ppc:
        case llvm::Triple::ppcle:
        case llvm::Triple::r600:
        case llvm::Triple::renderscript32:
        case llvm::Triple::riscv32:
        case llvm::Triple::shave:
        case llvm::Triple::sparc:
        case llvm::Triple::sparcel:
        case llvm::Triple::spir:
        case llvm::Triple::spirv32:
        case llvm::Triple::tce:
        case llvm::Triple::tcele:
        case llvm::Triple::thumb:
        case llvm::Triple::thumbeb:
        case llvm::Triple::wasm32:
        case llvm::Triple::x86:
        case llvm::Triple::xcore:
        case llvm::Triple::xtensa:
            return 32;

        case llvm::Triple::aarch64:
        case llvm::Triple::aarch64_be:
        case llvm::Triple::amdgcn:
        case llvm::Triple::amdil64:
        case llvm::Triple::bpfeb:
        case llvm::Triple::bpfel:
        case llvm::Triple::hsail64:
        case llvm::Triple::le64:
        case llvm::Triple::loongarch64:
        case llvm::Triple::mips64:
        case llvm::Triple::mips64el:
        case llvm::Triple::nvptx64:
        case llvm::Triple::ppc64:
        case llvm::Triple::ppc64le:
        case llvm::Triple::renderscript64:
        case llvm::Triple::riscv64:
        case llvm::Triple::sparcv9:
        case llvm::Triple::spir64:
        case llvm::Triple::spirv64:
        case llvm::Triple::systemz:
        case llvm::Triple::ve:
        case llvm::Triple::wasm64:
        case llvm::Triple::x86_64:
            return 64;
    }
    llvm_unreachable("Invalid architecture value");
}

bool TripleUtils::isArch16Bit(const llvm::Triple& tripleImpl) {
  return getArchPointerBitWidth(tripleImpl.getArch()) == 16;
}

bool TripleUtils::isArch32Bit(const llvm::Triple& tripleImpl) {
    return getArchPointerBitWidth(tripleImpl.getArch()) == 32;
}

bool TripleUtils::isArch64Bit(const llvm::Triple& tripleImpl){
    return getArchPointerBitWidth(tripleImpl.getArch()) == 64;
}

bool TripleUtils::isAndroidVersionLT(const llvm::Triple& tripleIpml, unsigned Major) {
	assert(isAndroid() && "Not an Android triple!");

    VersionTuple Version = tripleIpml.getEnvironmentVersion();

    // 64-bit targets did not exist before API level 21 (Lollipop).
    if (isArch64Bit(tripleIpml) && Version.getMajor() < 21)
      return VersionTuple(21) < VersionTuple(Major);

    return Version < VersionTuple(Major);
}

bool TripleUtils::isOSVersionLT(const llvm::Triple& tripleImpl, unsigned Major, unsigned Minor, unsigned Micro){
	if (Minor == 0) {
      return tripleImpl.getOSVersion() < VersionTuple(Major);
    }
    if (Micro == 0) {
      return tripleImpl.getOSVersion() < VersionTuple(Major, Minor);
    }
    return tripleImpl.getOSVersion() < VersionTuple(Major, Minor, Micro);
}

bool TripleUtils::isOSVersionLT(const llvm::Triple& tripleImpl, const llvm::Triple &Other) {
    return tripleImpl.getOSVersion() < Other.getOSVersion();
}

bool TripleUtils::isMacOSXVersionLT(const llvm::Triple& tripleImpl, unsigned Major, unsigned Minor, unsigned Micro) {
    assert(isMacOSX() && "Not an OS X triple!");

    // If this is OS X, expect a sane version number.
    if (tripleImpl.getOS() == Triple::MacOSX)
        return isOSVersionLT(tripleImpl, Major, Minor, Micro);

    // Otherwise, compare to the "Darwin" number.
    if (Major == 10) {
        return isOSVersionLT(tripleImpl, Minor + 4, Micro, 0);
    } else {
        assert(Major >= 11 && "Unexpected major version");
        return isOSVersionLT(tripleImpl, Major - 11 + 20, Minor, Micro);
    }
}

std::string TripleUtils::merge(const llvm::Triple& tripleImpl, const llvm::Triple& Other) {
    // If vendor is apple, pick the triple with the larger version number.
    if (tripleImpl.getVendor() == Triple::Apple)
        if (isOSVersionLT(Other, tripleImpl))
            return tripleImpl.str();

    return Other.str();
}

bool TripleUtils::isMacOSX(const llvm::Triple& tripleImpl){
    return tripleImpl.getOS() == Triple::Darwin || tripleImpl.getOS() == Triple::MacOSX;
}

bool TripleUtils::isTvOS(const llvm::Triple& tripleImpl) {
    return tripleImpl.getOS() == Triple::TvOS;
}

bool TripleUtils::isiOS(const llvm::Triple& tripleImpl) {
    return tripleImpl.getOS() == Triple::IOS || isTvOS(tripleImpl);
}

bool TripleUtils::isWatchOS(const llvm::Triple& tripleImpl) {
    return tripleImpl.getOS() == Triple::WatchOS;
}

bool TripleUtils::isWatchABI(const llvm::Triple& tripleImpl) {
    return tripleImpl.getSubArch() == Triple::ARMSubArch_v7k;
}

bool TripleUtils::isDriverKit(const llvm::Triple& tripleImpl) {
    return tripleImpl.getOS() == Triple::DriverKit;
}

bool TripleUtils::isOSzOS(const llvm::Triple& tripleImpl) {
    return tripleImpl.getOS() == Triple::ZOS;
}

bool TripleUtils::isOSDarwin(const llvm::Triple& tripleImpl) {
    return isMacOSX(tripleImpl) || isiOS(tripleImpl) || isWatchOS(tripleImpl) || isDriverKit(tripleImpl);
}

bool TripleUtils::isSimulatorEnvironment(const llvm::Triple& tripleImpl) {
    return tripleImpl.getEnvironment() == Triple::Simulator;
}

bool TripleUtils::isMacCatalystEnvironment(const llvm::Triple& tripleImpl) {
    return tripleImpl.getEnvironment() == Triple::MacABI;
}

bool TripleUtils::isTargetMachineMac(const llvm::Triple& tripleImpl) {
    return isMacOSX(tripleImpl) || (isOSDarwin(tripleImpl) && (isSimulatorEnvironment(tripleImpl) ||
                                           isMacCatalystEnvironment(tripleImpl)));
}
