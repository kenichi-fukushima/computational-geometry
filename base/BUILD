cc_library(
    name = "base",
    hdrs = ["base.h"],
    srcs = ["base.cc"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "base_test",
    srcs = ["base_test.cc"],
    deps = [
        ":base",
        "@gtest//:main",
    ],
    copts = ["-Iexternal/gtest/googletest/include"],
    size = "small",
)

cc_inc_library(
    name = "stl-utils",
    hdrs = ["stl-utils.h"],
    visibility = ["//visibility:public"],
)
