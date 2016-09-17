cc_library(
    name = "gtest",
    srcs = glob(
        ["*/src/*.cc"],
        exclude = [
            "googlemock/src/gmock-all.cc",
            "googlemock/src/gmock_main.cc",
            "googletest/src/gtest-all.cc",
            "googletest/src/gtest_main.cc",
        ],
    ),
    hdrs = glob([
        "*/include/**/*.h",
        "*/src/*.h",
    ]),
    copts = [
        "-Iexternal/gtest/googlemock/include",
        "-Iexternal/gtest/googletest",
        "-Iexternal/gtest/googletest/include",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "main",
    srcs = ["googletest/src/gtest_main.cc"],
    hdrs = glob([
        "include/**/*.h",
        "src/*.h",
    ]),
    copts = [
        "-Iexternal/gtest/googlemock/include",
        "-Iexternal/gtest/googletest/include",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
    deps = [":gtest"],
)
