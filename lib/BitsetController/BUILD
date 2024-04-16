load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "BitsetController_lib",
    srcs = ["BitsetController.cpp"],
    hdrs = ["BitsetController.h"],
    visibility = [
        "//tests/lib/BitsetController:__pkg__",
        "//tests/benchmark:__pkg__",
    ],
)