cc_binary(
    name = "day1",
    srcs = ["day1.cc"],
)

cc_library(
    name = "intcode",
    srcs = ["intcode.cc"],
    hdrs = ["intcode.h"],
)

cc_binary(
    name = "day2",
    srcs = ["day2.cc"],
    deps = [":intcode"],
)

cc_binary(
    name = "day3",
    srcs = ["day3.cc"],
)

cc_binary(
    name = "day4",
    srcs = ["day4.cc"],
)

cc_binary(
    name = "day5",
    srcs = ["day5.cc"],
    deps = [":intcode"],
)

cc_binary(
    name = "day6",
    srcs = ["day6.cc"],
)

cc_binary(
    name = "day7",
    srcs = ["day7.cc"],
    deps = [":intcode"],
)

cc_binary(
    name = "day8",
    srcs = ["day8.cc"],
)

cc_binary(
    name = "day9",
    srcs = ["day9.cc"],
    deps = [":intcode"],
)

cc_library(
    name = "xy",
    srcs = ["xy.cc"],
    hdrs = ["xy.h"],
)

cc_binary(
    name = "day10",
    srcs = ["day10.cc"],
    deps = [":xy"],
)

cc_binary(
    name = "day11",
    srcs = ["day11.cc"],
    deps = [
        ":intcode",
        ":xy",
    ],
)

cc_binary(
    name = "day12",
    srcs = ["day12.cc"],
)

cc_binary(
    name = "day13",
    srcs = ["day13.cc"],
    deps = [":intcode"],
)

cc_binary(
    name = "day14",
    srcs = ["day14.cc"],
    deps = ["@com_google_absl//absl/strings"],
)
