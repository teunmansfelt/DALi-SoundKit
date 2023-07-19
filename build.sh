#!/usr/bin/bash

# === GLOBAL VARIABLES ========================================================
declare -A BUILD_CONFIG=(
    ["TARGET"]=                     # Directory of the subproject to be build.
    ["TARGET_BIN_NAME"]=            # Name of the final binary.
    ["TARGET_BIN_FORMAT"]=          # Format of the final binary. (executable or library)
    ["BUILD_MODE"]=                 # Level of optimizations and debug info. (debug, default, optimized).
    ["CFLAGS"]=                     # List of compiler flags.
    ["LIBS"]=                       # List of dependencies.
    ["FORCE_CLEAN_BUILD"]=          # Flag to force the whole project to rebuild.
    ["RUN_AFTER_BUILD"]=            # Flag to run the binary after compiling. (Only for executables).
)

# === UTILITY FUNCTIONS =======================================================
error() {
    local desc=$1
    printf "[\033[91mERROR:\033[39m]: ${desc}\n"
}

info() {
    local label=$1
    local desc=$2
    printf "[\033[35m${label}\033[39m]: ${desc}\n"
}

usage() {
    local program=$0
    info "USAGE ${program} --target=<target>\n"
    echo "   [--mode=(debug|default)]"
    echo "   [--clean]"
    echo "   [--run]"
}

append() {
    local -n list=$1
    local new_items="$2"
    if [ ! -z "${new_items}" ]; then
        list="${list} ${new_items}"
    fi
}

# === BUILD FUNCTIONS =========================================================
read_command_line() {
    local target=${1%/*}
    local args=(${@:2})

    if [ -z ${target} ]; then
        usage
        error "No target specified."
        exit
    fi
    BUILD_CONFIG["TARGET"]=${target}

    for a in ${args[@]}; do
        local opt=${a%=*}
        local arg=${a#*=}
        case ${opt} in
            --mode)
                BUILD_CONFIG["BUILD_MODE"]=${arg}
                ;;
            --clean)
                BUILD_CONFIG["FORCE_CLEAN_BUILD"]="YES"
                ;;
            --run)
                BUILD_CONFIG["RUN_AFTER_BUILD"]="YES"
                ;;
            *)
                usage
                error "Unknown build option: '${opt}'"
                exit
                ;;
        esac
    done
}

read_build_config_file() {
    local target=${BUILD_CONFIG["TARGET"]}

    while read line; do
        if [ "${line}" = "[${target}]" ]; then
            # Read section belonging to the specified target
            while read line; do
                local key=${line%=*}
                local value=${line#*=}
                case ${key} in
                    TARGET_BIN_NAME | TARGET_BIN_FORMAT | LIBS)
                        BUILD_CONFIG[${key}]=${value}
                        ;;
                    CFLAGS)
                        append BUILD_CONFIG["CFLAGS"] ${value}
                        ;;
                    "")
                        # Empty line means end of the section.
                        break
                        ;;
                    *)
                        # Everything else can be ignored.
                        ;;
                esac
            done
            return 0  # The target was found.
        fi
    done
    return 1  # The target was not found.
}

configure_target_bin_format() {
    local target_bin_format=${BUILD_CONFIG["TARGET_BIN_FORMAT"]}
    local -n cflags=BUILD_CONFIG["CFLAGS"]

    case ${target_bin_format} in
        executable)
            ;;
        library)
            append cflags "-fPIC"
            ;;
        "")
            error "No target format specified in build configuration of '${target}'"
            exit
            ;;
        *)
            error "Unknown target format in build configuration of '${target}': '${TARGET_BIN_format}'"
            exit
            ;;
    esac
}

configure_build_mode() {
    local build_mode=${BUILD_CONFIG["BUILD_MODE"]}
    local -n cflags=BUILD_CONFIG["CFLAGS"]

    case ${build_mode} in
        debug)
            append cflags "-O0 -ggdb -DDALI_DEBUG_MODE"
            ;;
        default)
            append cflags "-O1"
            ;;
        optimized)
            append cflags "-O2 -DDALI_OPTIMIZED_MODE"
            ;;
        *)
            usage
            error "Invalid build mode: '${build_mode}'"
            exit
            ;;
    esac
}

clean_before_build() {
    local target=${BUILD_CONFIG["TARGET_BIN_NAME"]}

    if [ ${BUILD_CONFIG["FORCE_CLEAN_BUILD"]} = "YES" ]; then
        info "Cleaning ${target}" "..."
        make clean
    fi
}

build_target() {
    local target=${BUILD_CONFIG["TARGET"]}
    local target_bin_name=${BUILD_CONFIG["TARGET_BIN_NAME"]}
    local target_bin_format=${BUILD_CONFIG["TARGET_BIN_FORMAT"]}
    local build_mode=${BUILD_CONFIG["BUILD_MODE"]}
    local cflags=${BUILD_CONFIG["CFLAGS"]}
    local libs=${BUILD_CONFIG["LIBS"]}
    
    info "Building ${target}" "..."
    make ${target_bin_format}
    if [ $? -ne 0 ]; then
        error "Could not build '${target}'"
        exit
    fi

    info "BUILD_SUMMARY" ""
    echo "   - Target        : ${target_bin_name}"
    echo "   - Target format : ${target_bin_format}"
    echo "   - Build mode    : ${build_mode}"
    echo "   - CFLAGS        : ${cflags}"
    echo "   - Dependencies  : ${libs}"
}

run_after_build() {
    local target=${BUILD_CONFIG["TARGET_BIN_NAME"]}
    local target_bin_format=${BUILD_CONFIG["TARGET_BIN_FORMAT"]}

    if [ ${BUILD_CONFIG["RUN_AFTER_BUILD"]} = "YES" ]; then
        if [ ${target_bin_format} = "executable" ]; then
            info "Running ${target}" "..."
            ./bin/${target}
        else
            error "Cannot run library as executable"
            exit
        fi
    fi
}

# === MAIN ====================================================================
main() {
    BUILD_CONFIG["BUILD_MODE"]="default"
    BUILD_CONFIG["CFLAGS"]="-Wall -Wextra -std=gnu11"
    BUILD_CONFIG["FORCE_CLEAN_BUILD"]="NO"
    BUILD_CONFIG["RUN_AFTER_BUILD"]="NO"

    read_command_line $@
    read_build_config_file < ./build.conf
    if [ $? -ne 0 ]; then
        error "Invalid target: ${BUILD_CONFIG["TARGET"]}"
        exit
    fi
    if [ -z ${BUILD_CONFIG["TARGET_BIN_NAME"]} ]; then
        error "No target name specified in build configuration of ${BUILD_CONFIG["TARGET"]}."
        exit
    fi
    configure_target_bin_format
    configure_build_mode

    TARGET=${BUILD_CONFIG["TARGET"]}
    TARGET_BIN_NAME=${BUILD_CONFIG["TARGET_BIN_NAME"]}
    CFLAGS=${BUILD_CONFIG["CFLAGS"]}
    LIBS=${BUILD_CONFIG["LIBS"]}
    export TARGET TARGET_BIN_NAME CFLAGS LIBS

    clean_before_build
    build_target
    run_after_build
}

main $@