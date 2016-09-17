cc_library(
    name = "convex-hull",
    hdrs = ["convex-hull.h"],
    srcs = ["convex-hull.cc"],
    deps = ["//base"],
)

cc_test(
    name = "convex-hull_test",
    srcs = ["convex-hull_test.cc"],
    deps = [
        ":convex-hull",
        "@gtest//:main",
    ],
    copts = ["-Iexternal/gtest/googletest/include"],
    size = "small",
)
