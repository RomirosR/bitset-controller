load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "benchmark_BitsetController_lib",
    srcs = ["benchmark.cpp"],
    deps = [
        "//lib/BitsetController:BitsetController_lib",
        "//tools/catch:catch2",
    ],
    size = "small",
    linkstatic = True,
    copts = [
        "-O3",
        # "-Wl,-stack=256000000"
        # "-D_GLIBCXX_DEBUG",
        # "-D_GLIBCXX_DEBUG_PEDANTIC",
    ],
)

cc_test(
    name = "benchmark_BitsetControllerAVX_lib",
    srcs = ["benchmark_AVX.cpp"],
    deps = [
        "//lib/BitsetControllerAVX:BitsetControllerAVX_lib",
        "//tools/catch:catch2",
    ],
    size = "medium",
    linkstatic = True,
    copts = [
        "-O3",
        "-march=native",
        "-m64",
        "-mavx2",
    ],
)
        