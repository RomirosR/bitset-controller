load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "BitsetControllerAVX_lib",
    srcs = ["BitsetControllerAVX.cpp"],
    hdrs = ["BitsetControllerAVX.h"],
    visibility = [
        "//tests/lib/BitsetControllerAVX:__pkg__",
        "//tests/benchmark:__pkg__",
    ],
    copts = [
        "-m64",
        "-mavx2",
        "-march=native",
        "-mavx512f"
    ],
)