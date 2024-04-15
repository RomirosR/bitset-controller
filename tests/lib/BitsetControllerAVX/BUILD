load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_BitsetControllerAVX_lib",
    srcs = ["test.cpp"],
    deps = [
        "//lib/BitsetControllerAVX:BitsetControllerAVX_lib",
        "//tools/catch:catch2",
    ],
    copts = [
        "-m64",
        "-mavx2",
        "-march=native",
        "-mavx512f"
    ],
)