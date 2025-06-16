#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GPLE9G",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v1.4.1"
config.objdiff_tag = "v2.7.1"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    "-i src",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",

    "-i include/PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Include",
    "-i include/PowerPC_EABI_Support/msl/MSL_C++/MSL_Common/Include",

    "-D__GEKKO__"
]

cflags_piglet = [
    *cflags_base,
    "-RTTI on",
    "-i src/Piglet",
    "-DGAMECUBE",
    "-inline deferred",
    "-opt nopeephole",
    "-opt noschedule",
    "-pool off",
    "-fp_contract off",
    "-char unsigned", # Helps a function in CEntityManager, not sure if this is accurate yet though
]

cflags_engine = [
    *cflags_base,
    "-i src/engine",
    "-DGAMECUBE",
    "-inline deferred",
    "-opt nopeephole",
    "-opt noschedule",
    "-pool off",
    "-fp_contract off",
]

cflags_engine_input = [
    *cflags_base,
    "-i src/engine",
    "-DGAMECUBE",
    "-RTTI on",
    "-opt nopeephole",
    "-opt noschedule",
    "-pool off",
]

cflags_engine_sound = [
    *cflags_base,
    "-i src/engine",
    "-DGAMECUBE",
    "-RTTI on",
    "-opt nopeephole",
    "-opt noschedule",
    "-pool off",
    "-Cpp_exceptions on",
]

cflags_rw = [
    *cflags_base,
    "-i src/rwsdk/tool/png/libpng",
    "-i src/rwsdk/tool/png/zlib",
    "-DPNG_NO_STDIO",
    "-DPNG_NO_WRITE_tIME",
    "-DPNG_USER_MEM_SUPPORTED",
    "-DPNG_NO_PROGRESSIVE_READ",

    "-fp_contract off",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for RenderWare libraries
def RenderWareLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rw,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for DkEngine libraries
def DkEngineLib(lib_name: str, cflags: List[str], objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags,
        "progress_category": "engine",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Piglet",
        "mw_version": config.linker_version,
        "cflags": cflags_piglet,
        "progress_category": "game",
        "objects": [
            Object(Equivalent, "Piglet/main.cpp"),
            Object(NonMatching, "Piglet/CGame.cpp"),
            Object(NonMatching, "Piglet/CGamePartCredits.cpp"),
            Object(Equivalent, "Piglet/CGamePartFrontend.cpp"),
            Object(NonMatching, "Piglet/CGamePartIngame.cpp"),
            Object(Equivalent, "Piglet/CGamePartIntroduction.cpp"),
            Object(Equivalent, "Piglet/CGamePart.cpp"),
            Object(NonMatching, "Piglet/CResourceFactory.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityManager.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityMesh.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityWarp.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityWorld.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityZone.cpp"),
            Object(NonMatching, "Piglet/entities/CEntity.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagData.cpp"),
            Object(Equivalent, "Piglet/CCAKey.cpp"),
            Object(NonMatching, "Piglet/CCASystem.cpp"),
            Object(NonMatching, "Piglet/CCAZone.cpp"),
            Object(NonMatching, "Piglet/CCACamera.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagMessage.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagPosition.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagResponse.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagSendMessage.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagBehavior.cpp"),
            Object(NonMatching, "Piglet/CGameRoomManager.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagVisibility.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagCollision.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagActivation.cpp"),
            Object(NonMatching, "Piglet/CMenuControl.cpp"),
            Object(NonMatching, "Piglet/CControlValue.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityPiglet.cpp"),
            Object(NonMatching, "Piglet/CDebugMenu.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityObject.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagOrientation.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPC.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityLight.cpp"),
            Object(NonMatching, "Piglet/CGamePartDMRoomLauncher.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCBasicEffalump.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCArrow.cpp"),
            Object(NonMatching, "Piglet/entities/CEntitySeqKey.cpp"),
            Object(NonMatching, "Piglet/CPSplines.cpp"),
            Object(NonMatching, "Piglet/CRTCAnimForEntity.cpp"),
            Object(NonMatching, "Piglet/CRTCEntityAnimated.cpp"),
            Object(NonMatching, "Piglet/CRTCEvent.cpp"),
            Object(Equivalent, "Piglet/CRTCFade.cpp"),
            Object(Equivalent, "Piglet/CRTCFadeEvent.cpp"),
            Object(Equivalent, "Piglet/CRTCFXEvent.cpp"),
            Object(NonMatching, "Piglet/entities/CRTCPlayerEntity.cpp"),
            Object(NonMatching, "Piglet/CRTCSequencer.cpp"),
            Object(NonMatching, "Piglet/CRTCAddColorEvent.cpp"),
            Object(NonMatching, "Piglet/CMailBox.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCBeeEffalump.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCHnSWoozle.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCJackpotEffalump.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCMirrorWoozle.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCRoadSweeperEffalump.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCTennisWoozle.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCTree.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCTubaEffalump.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCBasicWoozle.cpp"),
            Object(Equivalent, "Piglet/CRTCFOVEvent.cpp"),
            Object(Equivalent, "Piglet/CRTCCamera.cpp"),
            Object(NonMatching, "Piglet/CPathFinder.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityTimer.cpp"),
            Object(NonMatching, "Piglet/entities/CEntitySearchableZone.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityPushingBox.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityCookie.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityWinnie.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityTigger.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityHero.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagTeleport.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagAnim.cpp"),
            Object(NonMatching, "Piglet/CMission.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityFX.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityScaryFaceBox.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityEscapingObject.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityBhvTagCameraSystem.cpp"),
            Object(NonMatching, "Piglet/CLoading.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityPhial.cpp"),
            Object(NonMatching, "Piglet/CEventAnimationHandlers.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityPathFinder.cpp"),
            Object(NonMatching, "Piglet/CGuiManager.cpp"),
            Object(NonMatching, "Piglet/CEventGuiHandlers.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityBhvTagGuiCtrl.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagSettings.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCDoor.cpp"),
            Object(NonMatching, "Piglet/CGameBackup.cpp"),
            Object(NonMatching, "Piglet/CMiniMapIcon.cpp"),
            Object(NonMatching, "Piglet/CMiniMapSpline.cpp"),
            Object(NonMatching, "Piglet/CMiniMap.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityBhvTagBoxPosition.cpp"),
            Object(NonMatching, "Piglet/entities/CEntity3DSound.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagFinishMission.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagNarrator.cpp"),
            Object(NonMatching, "Piglet/CFxManager.cpp"),
            Object(NonMatching, "Piglet/CShadowZone.cpp"),
            Object(NonMatching, "Piglet/entities/CEntityNPCGralosaurus.cpp"),
            Object(NonMatching, "Piglet/CGamePartScrapBook.cpp"),
            Object(NonMatching, "Piglet/CGamePartMovieClips.cpp"),
            Object(NonMatching, "Piglet/CIcon.cpp"),
            Object(Equivalent, "Piglet/CGuiScrapBookEventHandler.cpp"),
            Object(NonMatching, "Piglet/CGuiMovieClipsEventHandler.cpp"),
            Object(NonMatching, "Piglet/CLoadingAdventure.cpp"),
            Object(NonMatching, "Piglet/CLoadingCatchThemAll.cpp"),
            Object(NonMatching, "Piglet/CloadingTigger.cpp"),
            Object(NonMatching, "Piglet/CloadingWinnie.cpp"),
            Object(Equivalent, "Piglet/CGamePartStartScreen.cpp"),
            Object(Equivalent, "Piglet/CRTCVibrationEvent.cpp"),
            Object(Equivalent, "Piglet/CRTCMessageEvent.cpp"),
            Object(NonMatching, "Piglet/CFullScreenEffect.cpp"),
            Object(Equivalent, "Piglet/CRTCGrimaceEvent.cpp"),
            Object(Equivalent, "Piglet/entities/CEntityBhvTagReceiveShadow.cpp"),
            Object(NonMatching, "Piglet/CGCNFont.cpp"),
        ]
    },
    DkEngineLib(
        "DKI", cflags_engine_input,
        [
            Object(NonMatching, "engine/input/IInputEngine.cpp"),
            Object(NonMatching, "engine/input/CCombo.cpp"),
            Object(Matching, "engine/input/CInput.cpp"),
            Object(NonMatching, "engine/input/CState.cpp"),
            Object(Matching, "engine/input/CGCNPad.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "engine/input/CInputHalfAxe.cpp"),
        ]
    ),
    DkEngineLib(
        "DKSND", cflags_engine_sound,
        [
            Object(Matching, "engine/sound/DkSound.cpp", extra_cflags=["-inline deferred"]),
            Object(NonMatching, "engine/sound/ISoundEngine.cpp"),
            Object(Matching, "engine/sound/ISample.cpp"),
            Object(Matching, "engine/sound/ISoundEmitter.cpp"),
            Object(Matching, "engine/sound/ISoundListener.cpp"),
            Object(NonMatching, "engine/sound/ISoundStreamed.cpp"),
            Object(NonMatching, "engine/sound/ISampleDictionary.cpp"),
            Object(NonMatching, "engine/sound/CSampleBank.cpp"),
            Object(NonMatching, "engine/sound/CSampleDictionary.cpp"),
            Object(NonMatching, "engine/sound/CSound2D.cpp", extra_cflags=["-inline noauto"]),
            Object(NonMatching, "engine/sound/CSound3D.cpp"),
            Object(NonMatching, "engine/sound/CSoundEmitter.cpp"),
            Object(NonMatching, "engine/sound/CSoundEngine.cpp"),
            Object(NonMatching, "engine/sound/CSoundListener.cpp"),
            Object(NonMatching, "engine/sound/CSoundStreamed.cpp"),
            Object(NonMatching, "engine/sound/CSample.cpp"),
            Object(NonMatching, "engine/sound/CSound3DRWA.cpp"),
            Object(NonMatching, "engine/sound/CSound2DRWA.cpp"),
            Object(NonMatching, "engine/sound/CSoundStreamedRWA.cpp"),
            Object(NonMatching, "engine/sound/CSoundEngineRWA.cpp"),
            Object(NonMatching, "engine/sound/CSampleRWA.cpp"),
            Object(NonMatching, "engine/sound/CSoundListenerRWA.cpp"),
            Object(NonMatching, "engine/sound/CSoundEmitterRWA.cpp"),
        ]
    ),
    {
        # FIXME: Should we name this "DK" instead? Or some variant?
        "lib": "engine",
        "mw_version": config.linker_version,
        "cflags": cflags_engine,
        "progress_category": "engine",
        "objects": [
            Object(NonMatching, "engine/Allocator.cpp"),
            Object(Matching, "engine/GCN_MW_Memory.cpp"),
            Object(NonMatching, "engine/filesys/DkFileSysDlfs.c"),
            Object(NonMatching, "engine/filesys/DkFileSys.cpp"),
            Object(NonMatching, "engine/filesys/DkFileSysFat.cpp"),
            Object(NonMatching, "engine/filesys/DkFileSysUnCompress.cpp"),
            Object(NonMatching, "engine/display/DkDisplay.cpp"),
            Object(NonMatching, "engine/display/CEngine.cpp"),
            Object(NonMatching, "engine/display/IEngine.cpp"),
            Object(NonMatching, "engine/display/CDkWrapEngine.cpp"),
            Object(NonMatching, "engine/display/CObjectDictionary.cpp"),
            Object(NonMatching, "engine/display/IAnimDictionary.cpp"),
            Object(NonMatching, "engine/display/IObjectDictionary.cpp"),
            Object(NonMatching, "engine/display/ITextureDictionary.cpp"),
            Object(NonMatching, "engine/display/CAnimDictionary.cpp"),
            Object(NonMatching, "engine/display/IAtomic.cpp"),
            Object(NonMatching, "engine/display/IClump.cpp"),
            Object(Matching, "engine/display/IFramable.cpp"),
            Object(Matching, "engine/display/IFramableGeometry.cpp"),
            Object(NonMatching, "engine/display/IGeometry.cpp"),
            Object(NonMatching, "engine/display/CClump.cpp"),
            Object(NonMatching, "engine/display/IRaster.cpp"),
            Object(NonMatching, "engine/display/ITexture.cpp"),
            Object(NonMatching, "engine/display/IScene.cpp"),
            Object(NonMatching, "engine/display/CScene.cpp"),
            Object(NonMatching, "engine/display/ICamera.cpp"),
            Object(NonMatching, "engine/display/CCamera.cpp"),
            Object(NonMatching, "engine/display/ILight.cpp"),
            Object(NonMatching, "engine/display/CLight.cpp"),
            Object(NonMatching, "engine/display/IRWStream.cpp"),
            Object(NonMatching, "engine/display/CRWStream.cpp"),
            Object(NonMatching, "engine/display/IAnimation.cpp"),
            Object(NonMatching, "engine/display/IDMorphAnimation.cpp"),
            Object(NonMatching, "engine/display/IHierarchy.cpp"),
            Object(NonMatching, "engine/display/CAnimation.cpp"),
            Object(NonMatching, "engine/display/CDMorphAnimation.cpp"),
            Object(Matching, "engine/display/CTimer.cpp"),
            Object(NonMatching, "engine/display/ICharset.cpp"),
            Object(Matching, "engine/display/ITimer.cpp"),
            Object(NonMatching, "engine/display/CCharset.cpp"),
            Object(NonMatching, "engine/display/CHierarchy.cpp"),
            Object(Matching, "engine/display/IImage.cpp"),
            Object(NonMatching, "engine/display/CImage.cpp"),
            Object(NonMatching, "engine/display/CTexture.cpp"),
            Object(NonMatching, "engine/display/CTextureDictionary.cpp"),
            Object(NonMatching, "engine/display/CRaster.cpp"),
            Object(NonMatching, "engine/display/CShadowMap.cpp"),
            Object(NonMatching, "engine/display/IShadowMap.cpp"),
            Object(NonMatching, "engine/display/IShadowMapPipeline.cpp"),
            Object(NonMatching, "engine/display/CShadowMapPipelineGCN.cpp"),
            Object(NonMatching, "engine/display/CAtomic.cpp"),
            Object(NonMatching, "engine/display/CGeometry.cpp"),
            Object(NonMatching, "engine/display/IMaterialAnimationPipeline.cpp"),
            Object(NonMatching, "engine/display/CMaterialAnimationPipelineGCN.cpp"),
            Object(NonMatching, "engine/display/IMirror.cpp"),
            Object(NonMatching, "engine/display/CMirror.cpp"),
            Object(NonMatching, "engine/display/IMaterialAnimation.cpp"),
            Object(NonMatching, "engine/display/CMaterialAnimation.cpp"),
            Object(Matching, "engine/display/IWarp.cpp"),
            Object(NonMatching, "engine/display/CWarp.cpp"),
            Object(NonMatching, "engine/display/ISpline.cpp"),
            Object(NonMatching, "engine/display/IMaterialAnimationInterpolator.cpp"),
            Object(NonMatching, "engine/display/CMaterialAnimationInterpolator.cpp"),
            Object(NonMatching, "engine/display/IController.cpp"),
            Object(NonMatching, "engine/display/IGenericAnimation.cpp"),
            Object(NonMatching, "engine/display/CController.cpp"),
            Object(NonMatching, "engine/display/ISubController.cpp"),
            Object(NonMatching, "engine/display/CControllerHAnim.cpp"),
            Object(NonMatching, "engine/display/CControllerDMorph.cpp"),
            Object(NonMatching, "engine/display/CControllerMaterial.cpp"),
            Object(NonMatching, "engine/display/CControllerAnimEntry.cpp"),
            Object(NonMatching, "engine/display/CSpline.cpp"),
            Object(NonMatching, "engine/display/IAnimationStar.cpp"),
            Object(NonMatching, "engine/display/CAnimationStar.cpp"),
            Object(NonMatching, "engine/display/CAnimationStarEntry.cpp"),
            Object(NonMatching, "engine/display/IAnimationStarController.cpp"),
            Object(NonMatching, "engine/display/CAnimationStarController.cpp"),
            Object(NonMatching, "engine/display/IImmediate.cpp"),
            Object(NonMatching, "engine/display/CImmediate.cpp"),
            Object(NonMatching, "engine/display/IAnimationStarEntry.cpp"),
            Object(NonMatching, "engine/display/IIm3DBatch.cpp"),
            Object(NonMatching, "engine/display/IImBatch.cpp"),
            Object(NonMatching, "engine/display/IIm2DBatch.cpp"),
            Object(NonMatching, "engine/display/CIm3DBatch.cpp"),
            Object(NonMatching, "engine/display/CIm2DBatch.cpp"),
            Object(NonMatching, "engine/display/CParticleEmitter.cpp"),
            Object(NonMatching, "engine/display/IParticleEmitter.cpp"),
            Object(NonMatching, "engine/display/ITextureCallback.cpp"),
            Object(NonMatching, "engine/display/IEventAnimation.cpp"),
            Object(NonMatching, "engine/display/CControllerEvent.cpp"),
            Object(NonMatching, "engine/display/CEventAnimation.cpp"),
            Object(NonMatching, "engine/display/IEventAnimationCallback.cpp"),
            Object(NonMatching, "engine/display/CParticleEmitterDefinition.cpp"),
            Object(NonMatching, "engine/display/IParticleEmitterDefinition.cpp"),
            Object(Matching, "engine/display/CTimerGCN.cpp"),
            Object(Matching, "engine/display/IShadowMapValidationCallback.cpp"),
            Object(NonMatching, "engine/display/IPipeline.cpp"),
            Object(NonMatching, "engine/display/IDisplacementPipeline.cpp"),
            Object(NonMatching, "engine/display/CDisplacementPipelineGCN.cpp"),
            Object(NonMatching, "engine/display/CShadowMapDenyCallback.cpp"),
            Object(NonMatching, "engine/backup/DkBackUp.cpp"),
            Object(NonMatching, "engine/backup/CGCNBAKEngine.cpp"),
            Object(NonMatching, "engine/GCNIcon.c"),
            Object(Matching, "engine/gui/DkGUI.cpp", extra_cflags=["-Cpp_exceptions on"]),
            Object(NonMatching, "engine/gui/CGUIAnim.cpp"),
            Object(NonMatching, "engine/gui/CGUIEngine.cpp"),
            Object(NonMatching, "engine/gui/CGUIMenu.cpp"),
            Object(NonMatching, "engine/physics/DkPhysics.cpp"),
            Object(NonMatching, "engine/physics/DkPh_Sweeper.cpp"),
            Object(NonMatching, "engine/physics/DkPh_Collider.cpp"),
            Object(NonMatching, "engine/physics/DkPh_Dynamics.cpp"),
            Object(NonMatching, "engine/physics/DkPh_Primitives.cpp"),
            Object(NonMatching, "engine/xmd/CChunkIterator.cpp", extra_cflags=["-sym on"]),
            Object(NonMatching, "engine/xmd/CXmdFile.cpp"),
            Object(NonMatching, "engine/wrap/DKW_2dTools.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Animation.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Atomic.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Camera.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Charset.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Clump.cpp"),
            Object(NonMatching, "engine/wrap/DKW_ClumpAnim.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Engine.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Frame.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Geometry.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Hierarchy.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Im2D.cpp"),
            Object(NonMatching, "engine/wrap/DKW_ImageTGA.cpp"),
            Object(NonMatching, "engine/wrap/DKW_InstanceEngine.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Light.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Material.cpp"),
            Object(NonMatching, "engine/wrap/DKW_MatFX.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Matrix.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Primitive.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Quat.cpp"),
            Object(NonMatching, "engine/wrap/DKW_RenderProperty.cpp"),
            Object(Matching, "engine/wrap/DKW_RGBA.cpp"),
            Object(Matching, "engine/wrap/DKW_RGBAReal.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Spline.cpp"),
            Object(NonMatching, "engine/wrap/DKW_TexDictionary.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Texture.cpp"),
            Object(Matching, "engine/wrap/DKW_V2d.cpp"),
            Object(NonMatching, "engine/wrap/DKW_V3d.cpp"),
            Object(NonMatching, "engine/wrap/DKW_Warp.cpp"),
            Object(NonMatching, "engine/wrap/DKW_World.cpp"),
            Object(NonMatching, "engine/wrap/DKW_WorldSector.cpp"),
            Object(NonMatching, "engine/wrap/DkWrap.cpp"),
            Object(Matching, "engine/wrap/DKW_GCNEngine.cpp"),
            Object(Matching, "engine/video/CVideoEngine.cpp", extra_cflags=["-RTTI on"]),
            Object(NonMatching, "engine/video/CVideoEngineGCN.cpp"),
            Object(NonMatching, "engine/video/DkVideo.cpp"),
        ]
    },
    DolphinLib(
        "ai",
        [
            Object(NonMatching, "dolphin/ai/ai.c"),
        ]
    ),
    DolphinLib(
        "am",
        [
            Object(NonMatching, "dolphin/am/am.c"),
        ]
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(NonMatching, "dolphin/amcstubs/AmcExi2Stubs.c"),
        ]
    ),
    DolphinLib(
        "ar",
        [
            Object(NonMatching, "dolphin/ar/ar.c"),
            Object(NonMatching, "dolphin/ar/arq.c"),
        ]
    ),
    DolphinLib(
        "ax",
        [
            Object(NonMatching, "dolphin/ax/AX.c"),
            Object(NonMatching, "dolphin/ax/AXAlloc.c"),
            Object(NonMatching, "dolphin/ax/AXAux.c"),
            Object(NonMatching, "dolphin/ax/AXCL.c"),
            Object(NonMatching, "dolphin/ax/AXOut.c"),
            Object(NonMatching, "dolphin/ax/AXSPB.c"),
            Object(NonMatching, "dolphin/ax/AXVPB.c"),
            Object(NonMatching, "dolphin/ax/AXComp.c"),
            Object(NonMatching, "dolphin/ax/DSPCode.c"),
            Object(NonMatching, "dolphin/ax/AXProf.c"),
        ]
    ),
    DolphinLib(
        "axfx",
        [
            Object(NonMatching, "dolphin/axfx/reverb_hi.c"),
            Object(NonMatching, "dolphin/axfx/reverb_std.c"),
            Object(NonMatching, "dolphin/axfx/axfx.c"),
            Object(NonMatching, "dolphin/axfx/reverb_hi_4ch.c"),
        ]
    ),
    DolphinLib(
        "base",
        [
            Object(NonMatching, "dolphin/base/PPCArch.c"),
        ]
    ),
    DolphinLib(
        "card",
        [
            Object(NonMatching, "dolphin/card/CARDBios.c"),
            Object(NonMatching, "dolphin/card/CARDUnlock.c"),
            Object(NonMatching, "dolphin/card/CARDNet.c"),
            Object(NonMatching, "dolphin/card/CARDRdwr.c"),
            Object(NonMatching, "dolphin/card/CARDBlock.c"),
            Object(NonMatching, "dolphin/card/CARDDir.c"),
            Object(NonMatching, "dolphin/card/CARDCheck.c"),
            Object(NonMatching, "dolphin/card/CARDMount.c"),
            Object(NonMatching, "dolphin/card/CARDFormat.c"),
            Object(NonMatching, "dolphin/card/CARDOpen.c"),
            Object(NonMatching, "dolphin/card/CARDCreate.c"),
            Object(NonMatching, "dolphin/card/CARDRead.c"),
            Object(NonMatching, "dolphin/card/CARDWrite.c"),
            Object(NonMatching, "dolphin/card/CARDDelete.c"),
            Object(NonMatching, "dolphin/card/CARDStat.c"),
        ]
    ),
    DolphinLib(
        "db",
        [
            Object(NonMatching, "dolphin/db/db.c"),
        ]
    ),
    DolphinLib(
        "dsp",
        [
            Object(NonMatching, "dolphin/dsp/dsp.c"),
            Object(NonMatching, "dolphin/dsp/dsp_debug.c"),
            Object(NonMatching, "dolphin/dsp/dsp_task.c"),
        ]
    ),
    DolphinLib(
        "dvd",
        [
            Object(NonMatching, "dolphin/dvd/dvdlow.c"),
            Object(NonMatching, "dolphin/dvd/dvdfs.c"),
            Object(NonMatching, "dolphin/dvd/dvd.c"),
            Object(NonMatching, "dolphin/dvd/dvdqueue.c"),
            Object(NonMatching, "dolphin/dvd/dvderror.c"),
            Object(NonMatching, "dolphin/dvd/dvdidutils.c"),
            Object(NonMatching, "dolphin/dvd/dvdFatal.c"),
            Object(NonMatching, "dolphin/dvd/emu_level2/fstload.c"),
        ]
    ),
    DolphinLib(
        "exi",
        [
            Object(NonMatching, "dolphin/exi/EXIBios.c"),
            Object(NonMatching, "dolphin/exi/EXIUart.c"),
        ]
    ),
    DolphinLib(
        "fileCache",
        [
            Object(NonMatching, "dolphin/fileCache/fileCache.c"),
        ]
    ),
    DolphinLib(
        "gx",
        [
            Object(NonMatching, "dolphin/gx/GXInit.c"),
            Object(NonMatching, "dolphin/gx/GXFifo.c"),
            Object(NonMatching, "dolphin/gx/GXAttr.c"),
            Object(NonMatching, "dolphin/gx/GXMisc.c"),
            Object(NonMatching, "dolphin/gx/GXGeometry.c"),
            Object(NonMatching, "dolphin/gx/GXFrameBuf.c"),
            Object(NonMatching, "dolphin/gx/GXLight.c"),
            Object(NonMatching, "dolphin/gx/GXTexture.c"),
            Object(NonMatching, "dolphin/gx/GXBump.c"),
            Object(NonMatching, "dolphin/gx/GXTev.c"),
            Object(NonMatching, "dolphin/gx/GXPixel.c"),
            Object(NonMatching, "dolphin/gx/GXStubs.c"),
            Object(NonMatching, "dolphin/gx/GXDisplayList.c"),
            Object(NonMatching, "dolphin/gx/GXTransform.c"),
            Object(NonMatching, "dolphin/gx/GXPerf.c"),
        ]
    ),
    DolphinLib(
        "mtx",
        [
            Object(NonMatching, "dolphin/mtx/mtx.c"),
            Object(NonMatching, "dolphin/mtx/mtx44.c"),
        ]
    ),
    DolphinLib(
        "odemustubs",
        [
            Object(NonMatching, "dolphin/odemustubs/odemustubs.c"),
        ]
    ),
    DolphinLib(
        "os",
        [
            Object(Equivalent, "dolphin/os/OS.c"),
            Object(Matching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSArena.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/os/OSCache.c"),
            Object(Matching, "dolphin/os/OSContext.c"),
            Object(Equivalent, "dolphin/os/OSError.c"), # should be matching, something is making it not match though...
            Object(NonMatching, "dolphin/os/OSFont.c"),
            Object(Matching, "dolphin/os/OSInterrupt.c"),
            Object(Matching, "dolphin/os/OSLink.c"),
            Object(Matching, "dolphin/os/OSMessage.c"),
            Object(Matching, "dolphin/os/OSMemory.c"),
            Object(Matching, "dolphin/os/OSMutex.c"),
            Object(NonMatching, "dolphin/os/OSReboot.c"),
            Object(NonMatching, "dolphin/os/OSReset.c"),
            Object(Matching, "dolphin/os/OSResetSW.c"),
            Object(Matching, "dolphin/os/OSRtc.c"),
            Object(Matching, "dolphin/os/OSSemaphore.c"),
            Object(Matching, "dolphin/os/OSSync.c"),
            Object(Matching, "dolphin/os/OSThread.c"),
            Object(Matching, "dolphin/os/OSTime.c"),
            Object(NonMatching, "dolphin/os/init/__start.c"),
            Object(NonMatching, "dolphin/os/init/__ppc_eabi_init.cpp"),
        ]
    ),
    DolphinLib(
        "pad",
        [
            Object(NonMatching, "dolphin/pad/Padclamp.c"),
            Object(NonMatching, "dolphin/pad/Pad.c"),
        ]
    ),
    DolphinLib(
        "si",
        [
            Object(NonMatching, "dolphin/si/SIBios.c"),
            Object(NonMatching, "dolphin/si/SISamplingRate.c"),
        ]
    ),
    DolphinLib(
        "support",
        [
            Object(NonMatching, "dolphin/support/List.c"),
        ]
    ),
    DolphinLib(
        "texPalette",
        [
            Object(NonMatching, "dolphin/texPalette/texPalette.c"),
        ]
    ),
    DolphinLib(
        "vi",
        [
            Object(NonMatching, "dolphin/vi/vi.c"),
        ]
    ),
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/abort_exit.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/alloc.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/errno.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/ansi_files.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Src/ansi_fp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/buffer_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/char_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/ctype.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/locale.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/direct_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/file_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/FILE_POS.C"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/mbstring.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/mem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/mem_funcs.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/misc_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/printf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/qsort.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/rand.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/scanf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/signal.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/string.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/float.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/strtold.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/strtoul.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Src/uart_console_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/wmem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common/Src/wchar_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_atan2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_exp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_fmod.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_log.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_pow.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/e_rem_pio2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/k_cos.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/k_rem_pio2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/k_sin.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/k_tan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_atan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_copysign.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_cos.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_floor.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_frexp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_ldexp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_modf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_nextafter.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_sin.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/s_tan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/w_atan2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/w_exp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/w_fmod.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/w_log.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/MSL_Common_Embedded/Math/w_pow.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C/PPC_EABI/SRC/math_ppc.c"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C++/MSL_Common/Src/ios.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C++/MSL_Common/Src/iostream.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/msl/MSL_C++/MSL_Common/Src/locale.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__va_arg.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/NMWException.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/runtime.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/Gecko_setjmp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/Gecko_ExceptionPPC.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__mem.c"),
        ],
    },
    {
        "lib": "TRK_Minnow_Dolphin",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/mainloop.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/nubevent.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/nubinit.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/msg.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/msgbuf.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/serpoll.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/usr_put.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/dispatch.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/msghndlr.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/support.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/mutex_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/notify.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/ppc/Generic/flush_cache.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/mem_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/ppc/Export/targsupp.s"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/ppc/Generic/__exception.s"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/ppc/Generic/mpc_7xx_603e.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Portable/main_TRK.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk_glue.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Os/dolphin/targcont.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/Os/dolphin/target_options.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/MetroTRK/Export/mslsupp.c"),
        ]
    },
    {
        "lib": "OdemuExi2",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "OdemuExi2/DebuggerDriver.c"),
        ]
    },
    RenderWareLib(
        "rpcollis",
        [
            Object(NonMatching, "rwsdk/plugin/collis/ctbuild.c"),
            Object(NonMatching, "rwsdk/plugin/collis/ctworld.c"),
            Object(NonMatching, "rwsdk/plugin/collis/ctbsp.c"),
            Object(NonMatching, "rwsdk/plugin/collis/rpcollis.c"),
        ]
    ),
    RenderWareLib(
        "rpdmorph",
        [
            Object(NonMatching, "rwsdk/plugin/dmorph/dmphanim.c"),
            Object(NonMatching, "rwsdk/plugin/dmorph/dmphgeom.c"),
            Object(NonMatching, "rwsdk/plugin/dmorph/dmphintp.c"),
            Object(NonMatching, "rwsdk/plugin/dmorph/rpdmorph.c"),
        ]
    ),
    RenderWareLib(
        "rphanim",
        [
            Object(NonMatching, "rwsdk/plugin/hanim/stdkey.c"),
            Object(NonMatching, "rwsdk/plugin/hanim/rphanim.c"),
        ]
    ),
    RenderWareLib(
        "rpmatfx",
        [
            Object(NonMatching, "rwsdk/plugin/matfx/rpmatfx.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/effectPipesGcn.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/multiTexGcnData.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/multiTexGcnPipe.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/multiTexGcn.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/multiTex.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/multiTexEffect.c"),
        ]
    ),
    RenderWareLib(
        "rpmipkl",
        [
            Object(NonMatching, "rwsdk/plugin/mipkl/rpmipkl.c"),
        ]
    ),
    RenderWareLib(
        "rppatch",
        [
            Object(NonMatching, "rwsdk/plugin/patch/patchmesh.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchatomic.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchgeometry.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchnoskin.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchstream.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchlod.c"),
            Object(NonMatching, "rwsdk/plugin/patch/rppatch.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchexpander.c"),
            Object(NonMatching, "rwsdk/plugin/patch/gcn/patchplatform.c"),
            Object(NonMatching, "rwsdk/plugin/patch/patchgeom.c"),
            Object(NonMatching, "rwsdk/plugin/patch/gcn/patchgcn.c"),
        ]
    ),
    RenderWareLib(
        "rpprtstd",
        [
            Object(NonMatching, "rwsdk/plugin/prtstd/eclass.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/emitter.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/pclass.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/prop.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/pptank.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/standard.c"),
            Object(NonMatching, "rwsdk/plugin/prtstd/rpprtstd.c"),
        ]
    ),
    RenderWareLib(
        "rpptank",
        [
            Object(NonMatching, "rwsdk/plugin/ptank/rpptank.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcncallbacks.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcnrender.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcntransforms.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_nc_ppm.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_cc_ppm.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_nc_cs_nr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_cc_cs_nr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_nc_pps_nr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_cc_pps_nr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_nc_cs_ppr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_cc_cs_ppr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_nc_pps_ppr.c"),
            Object(NonMatching, "rwsdk/plugin/ptank/gcn/ptankgcn_cc_pps_ppr.c"),
        ]
    ),
    RenderWareLib(
        "rprandom",
        [
            Object(NonMatching, "rwsdk/plugin/random/rprandom.c"),
        ]
    ),
    RenderWareLib(
        "rpskinmatfx",
        [
            Object(NonMatching, "rwsdk/plugin/skin2/bsplit.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/rpskin.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skingcn.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skinstream.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skinmatrixblend.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skingcnasm.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skingcng.c"),
        ]
    ),
    RenderWareLib(
        "rpspline",
        [
            Object(NonMatching, "rwsdk/plugin/spline/rpspline.c"),
        ]
    ),
    RenderWareLib(
        "rpworld",
        [
            Object(NonMatching, "rwsdk/world/babinwor.c"),
            Object(NonMatching, "rwsdk/world/baclump.c"),
            Object(NonMatching, "rwsdk/world/bageomet.c"),
            Object(NonMatching, "rwsdk/world/balight.c"),
            Object(NonMatching, "rwsdk/world/bamateri.c"),
            Object(NonMatching, "rwsdk/world/bamatlst.c"),
            Object(NonMatching, "rwsdk/world/bamesh.c"),
            Object(NonMatching, "rwsdk/world/bameshop.c"),
            Object(NonMatching, "rwsdk/world/basector.c"),
            Object(NonMatching, "rwsdk/world/baworld.c"),
            Object(NonMatching, "rwsdk/world/baworobj.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/bapipew.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/gcpipe.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/vtxfmt.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/wrldpipe.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/nodeGameCubeAtomicAllInOne.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/nodeGameCubeWorldSectorAllInOne.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/displaylist.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/gclights.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/gcmorph.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/native.c"),
        ]
    ),
    RenderWareLib(
        "rt2d",
        [
            Object(NonMatching, "rwsdk/tool/2d/brush.c"),
            Object(NonMatching, "rwsdk/tool/2d/fill.c"),
            Object(NonMatching, "rwsdk/tool/2d/font.c"),
            Object(NonMatching, "rwsdk/tool/2d/gstate.c"),
            Object(NonMatching, "rwsdk/tool/2d/path.c"),
            Object(NonMatching, "rwsdk/tool/2d/stroke.c"),
            Object(NonMatching, "rwsdk/tool/2d/2dstr.c"),
            Object(NonMatching, "rwsdk/tool/2d/object.c"),
            Object(NonMatching, "rwsdk/tool/2d/pickreg.c"),
            Object(NonMatching, "rwsdk/tool/2d/scene.c"),
            Object(NonMatching, "rwsdk/tool/2d/shape.c"),
            Object(NonMatching, "rwsdk/tool/2d/tri.c"),
        ]
    ),
    RenderWareLib(
        "rt2danim",
        [
            Object(NonMatching, "rwsdk/tool/2d/anim/anim.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/keyframe.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/props.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/bucket.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/strlabel.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/button.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/message.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/maestro.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/handler.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/cel.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/gstate.c"),
            Object(NonMatching, "rwsdk/tool/2d/anim/rt2danim.c"),
        ]
    ),
    RenderWareLib(
        "rtanim",
        [
            Object(NonMatching, "rwsdk/tool/anim/rtanim.c"),
        ]
    ),
    RenderWareLib(
        "rtbezpat",
        [
            Object(NonMatching, "rwsdk/tool/bezpat/bezquad.c"),
            Object(NonMatching, "rwsdk/tool/bezpat/rtbezpat.c"),
        ]
    ),
    RenderWareLib(
        "rtbmp",
        [
            Object(NonMatching, "rwsdk/tool/bmp/rtbmp.c"),
        ]
    ),
    RenderWareLib(
        "rtcharset",
        [
            Object(NonMatching, "rwsdk/tool/charset/chrprint.c"),
            Object(NonMatching, "rwsdk/tool/charset/rtcharse.c"),
        ]
    ),
    RenderWareLib(
        "rtintsec",
        [
            Object(NonMatching, "rwsdk/tool/intsec/rtintsec.c"),
        ]
    ),
    RenderWareLib(
        "rtpick",
        [
            Object(NonMatching, "rwsdk/tool/pick/rtpick.c"),
        ]
    ),
    RenderWareLib(
        "rtpng",
        [
            Object(Matching, "rwsdk/tool/png/zlib/adler32.c"),
            Object(Matching, "rwsdk/tool/png/zlib/deflate.c"),
            Object(Matching, "rwsdk/tool/png/zlib/trees.c"),
            Object(Matching, "rwsdk/tool/png/zlib/zutil.c"),
            Object(Matching, "rwsdk/tool/png/zlib/inflate.c"),
            Object(Matching, "rwsdk/tool/png/zlib/infblock.c"),
            Object(Matching, "rwsdk/tool/png/zlib/inftrees.c"),
            Object(Matching, "rwsdk/tool/png/zlib/infcodes.c"),
            Object(Matching, "rwsdk/tool/png/zlib/infutil.c"),
            Object(Matching, "rwsdk/tool/png/zlib/inffast.c"),
            Object(Matching, "rwsdk/tool/png/zlib/crc32.c"),
            Object(Matching, "rwsdk/tool/png/libpng/png.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngerror.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngget.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngmem.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngread.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngrio.c"),
            Object(Equivalent, "rwsdk/tool/png/libpng/pngrtran.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngrutil.c", extra_cflags=["-inline level=2"]),
            Object(Matching, "rwsdk/tool/png/libpng/pngset.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngtrans.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngwio.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngwrite.c"),
            Object(Matching, "rwsdk/tool/png/libpng/pngwtran.c"),
            Object(Equivalent, "rwsdk/tool/png/libpng/pngwutil.c"),
            Object(NonMatching, "rwsdk/tool/png/rtpng.c"),
        ]
    ),
    RenderWareLib(
        "rtquat",
        [
            Object(NonMatching, "rwsdk/tool/quat/rtquat.c"),
        ]
    ),
    RenderWareLib(
        "rwcore",
        [
            Object(NonMatching, "rwsdk/src/plcore/babinary.c"),
            Object(NonMatching, "rwsdk/src/plcore/babinmtx.c"),
            Object(NonMatching, "rwsdk/src/plcore/bacolor.c"),
            Object(NonMatching, "rwsdk/src/plcore/baerr.c"),
            Object(NonMatching, "rwsdk/src/plcore/bafsys.c"),
            Object(NonMatching, "rwsdk/src/plcore/baimmedi.c"),
            Object(NonMatching, "rwsdk/src/plcore/balist.c"),
            Object(NonMatching, "rwsdk/src/plcore/bamatrix.c"),
            Object(NonMatching, "rwsdk/src/plcore/bamemory.c"),
            Object(NonMatching, "rwsdk/src/plcore/baresour.c"),
            Object(NonMatching, "rwsdk/src/plcore/bastream.c"),
            Object(NonMatching, "rwsdk/src/plcore/batkbin.c"),
            Object(NonMatching, "rwsdk/src/plcore/batkreg.c"),
            Object(NonMatching, "rwsdk/src/plcore/bavector.c"),
            Object(NonMatching, "rwsdk/src/plcore/resmem.c"),
            Object(NonMatching, "rwsdk/src/plcore/rwstring.c"),
            Object(NonMatching, "rwsdk/os/gcn/osintf.c"),
            Object(NonMatching, "rwsdk/src/babbox.c"),
            Object(NonMatching, "rwsdk/src/babincam.c"),
            Object(NonMatching, "rwsdk/src/babinfrm.c"),
            Object(NonMatching, "rwsdk/src/babintex.c"),
            Object(NonMatching, "rwsdk/src/bacamera.c"),
            Object(NonMatching, "rwsdk/src/badevice.c"),
            Object(NonMatching, "rwsdk/src/baframe.c"),
            Object(NonMatching, "rwsdk/src/baimage.c"),
            Object(NonMatching, "rwsdk/src/baimras.c"),
            Object(NonMatching, "rwsdk/src/baraster.c"),
            Object(NonMatching, "rwsdk/src/baresamp.c"),
            Object(NonMatching, "rwsdk/src/basync.c"),
            Object(NonMatching, "rwsdk/src/batextur.c"),
            Object(NonMatching, "rwsdk/src/batypehf.c"),
            Object(NonMatching, "rwsdk/driver/common/palquant.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dl2drend.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlconvrt.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dldevice.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlmem.c"), # is this right?
            Object(NonMatching, "rwsdk/driver/gcn/dlraster.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlrendst.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlsprite.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dltexdic.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dltextur.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dltoken.c"),
            Object(NonMatching, "rwsdk/driver/gcn/gekkomul.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/baim3d.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/bapipe.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2altmdl.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2core.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2define.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2dep.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2heap.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2renderstate.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2resort.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/gcn/im3dpipe.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/gcn/nodeDolphinSubmitNoLight.c"),
        ]
    ),
    # FIXME: Figure out the proper name and directory structure for this library (or libraries)
    RenderWareLib(
        "rwa",
        [
            Object(NonMatching, "rwa/rwaobjdefalias.c"),
            Object(NonMatching, "rwa/rwaintf.c"),
            Object(NonMatching, "rwa/rwaobjdef.c"),
            Object(NonMatching, "rwa/rwaobj.c"),
            Object(NonMatching, "rwa/rwaobjinterface.c"),
            Object(NonMatching, "rwa/rwaparam.c"),
            Object(NonMatching, "rwa/rwasymboltable.c"),
            Object(NonMatching, "rwa/rwamemory.c"),
            Object(NonMatching, "rwa/rwafile.c"),
            Object(NonMatching, "rwa/rwabase.c"),
            Object(NonMatching, "rwa/rwawavedict.c"),
            Object(NonMatching, "rwa/rwawaveconv.c"),
            Object(NonMatching, "rwa/rwawavedef.c"),
            Object(NonMatching, "rwa/rwawave.c"),
            Object(NonMatching, "rwa/rwauuid.c"),
            Object(NonMatching, "rwa/rwavoicemanager.c"),
            Object(NonMatching, "rwa/rwathread.c"),
            Object(NonMatching, "rwa/rwafreelist.c"),
            Object(NonMatching, "rwa/rwallist.c"),
            Object(NonMatching, "rwa/rwamath.c"),
            Object(NonMatching, "rwa/rwaheap.c"),
            Object(NonMatching, "rwa/rwastreamcreate.c"),
            Object(NonMatching, "rwa/rwstream.c"),
            Object(NonMatching, "rwa/rwaallocbitmap.c"),
            Object(NonMatching, "rwa/rwavirtualvoice.c"),
            Object(NonMatching, "rwa/rwavoice3d.c"),
            Object(NonMatching, "rwa/rwavoice.c"),
            Object(NonMatching, "rwa/rwaoutput.c"),
            Object(NonMatching, "rwa/rwalistener.c"),
            Object(NonMatching, "rwa/rwastreamint.c"),
            Object(NonMatching, "rwa/rwastreamnodeint.c"),
            Object(NonMatching, "rwa/rwastreamnodetypes.c"),
            Object(NonMatching, "rwa/rwastreamrendererint.c"),
            Object(NonMatching, "rwa/rwastreamsourceint.c"),
            Object(NonMatching, "rwa/rwastreamcodecint.c"),
            Object(NonMatching, "rwa/rwavirtualvoiceobj.c"),
            Object(NonMatching, "rwa/rwalistenerobj.c"),
            Object(NonMatching, "rwa/rwastreamsource.c"),
            Object(NonMatching, "rwa/rwastreamfanout.c"),
            Object(NonMatching, "rwa/rwastreamsourcecommon.c"),
            Object(NonMatching, "rwa/rwastreamfilequeue.c"),
            Object(NonMatching, "rwa/rwastreamcontainer.c"),
            Object(NonMatching, "rwa/rwastreamcontainercmd.c"),
            Object(NonMatching, "rwa/rwastream.c"),
            Object(NonMatching, "rwa/rwanullwave.c"),
            Object(NonMatching, "rwa/osbase.c"),
            Object(NonMatching, "rwa/osmultithread.c"),
            Object(NonMatching, "rwa/ostimer.c"),
            Object(NonMatching, "rwa/rwaax.c"),
            Object(NonMatching, "rwa/rwaaxoutput.c"),
            Object(NonMatching, "rwa/rwaaxoutputchannel.c"),
            Object(NonMatching, "rwa/rwaaxoutputint.c"),
            Object(NonMatching, "rwa/rwaaxvoice.c"),
            Object(NonMatching, "rwa/rwaaxvoiceint.c"),
            Object(NonMatching, "rwa/rwaaxwave.c"),
            Object(NonMatching, "rwa/rwaarammanager.c"),
            Object(NonMatching, "rwa/rwaaxrenderer.c"),
            Object(NonMatching, "rwa/rwaaxrenderersub.c"),
        ]
    ),
    # FIXME: Figure this one out
    {
        "lib": "thp",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "thp/THPAudioDecode.c"),
            Object(NonMatching, "thp/THPDraw.c"),
            Object(NonMatching, "thp/THPPlayer.c"),
            Object(NonMatching, "thp/THPRead.c"),
            Object(NonMatching, "thp/THPVideoDecode.c"),
            Object(NonMatching, "thp/THPDec.c"),
            Object(NonMatching, "thp/THPAudio.c"),
        ]
    }
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("engine", "Engine Code"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
