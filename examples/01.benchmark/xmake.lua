-- Copyright CHERIoT Contributors.
-- SPDX-License-Identifier: MIT

set_project("CHERIoT Hello World")
sdkdir = os.getenv("CHERIOT_SDK") or
	"../../../cheriot-rtos/sdk/"
includes(sdkdir, "../../lib")

set_toolchains("cheriot-clang")

option("board")
	set_default("ibex-safe-simulator")

compartment("pqc_benchmark")
	add_deps("freestanding", "debug", "mlkem_library", "mldsa_library")
	add_files("benchmark.cc")


-- Firmware image for the example.
firmware("pqc_benchmark_example")
	add_deps("pqc_benchmark")
	on_load(function(target)
		target:values_set("board", "$(board)")
		target:values_set("threads", {
			{
				compartment = "pqc_benchmark",
				priority = 1,
				entry_point = "benchmark",
				stack_size = 0xff00,
				trusted_stack_frames = 2
			}
		}, {expand = false})
	end)
