cc_library(
    name = "segment-intersection",
    hdrs = ["segment-intersection.h"],
    srcs = ["segment-intersection.cc"],
    deps = ["//base",
            "//base:stl-utils",
    ],
)

cc_test(
    name = "segment-intersection_test",
    srcs = ["segment-intersection_test.cc"],
    deps = [
        ":segment-intersection",
	"@gtest//:main",
    ],
    copts = [
        "-Iexternal/gtest/googlemock/include",
        "-Iexternal/gtest/googletest/include",
    ],
    size = "small",
)
