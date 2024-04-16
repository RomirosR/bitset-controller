load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "benchmark_BitsetController_lib",
    srcs = ["benchmark.cpp"],
    deps = [
        "//lib/BitsetController:BitsetController_lib",
        "//tools/catch:catch2",
    ],
    size = "large",
    # copts = [
    #     "-O3",
    # ],
)

cc_test(
    name = "benchmark_BitsetControllerAVX_lib",
    srcs = ["benchmark_AVX.cpp"],
    deps = [
        "//lib/BitsetControllerAVX:BitsetControllerAVX_lib",
        "//tools/catch:catch2",
    ],
    size = "small",
    copts = [
        "-m64",
        "-mavx2",
        "-march=native",
        "-mavx512f"
    ],
)
        