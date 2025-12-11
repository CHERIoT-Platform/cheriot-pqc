-- Copyright CHERIoT Contributors.
-- SPDX-License-Identifier: MIT

library("mldsa_library")
	add_deps("randombytes")
	add_includedirs("../../include", "../../third_party/mldsa-native/mldsa")
	add_files(
		"../../third_party/mldsa-native/mldsa/mldsa_native.c")
